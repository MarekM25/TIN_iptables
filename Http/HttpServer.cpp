//
// Created by anowikowski on 10.12.15.
//

#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include "HttpServer.h"
#include "../Extensions/StringExtensions.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

HttpServer::HttpServer()
{
    this->localAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    this->localAddress.sin_family = AF_INET;
    this->bIsRunning = false;
    this->maxConnectionQueueLength = 1;
    this->sendTimeout = 1000;
    this->receiveTimeout = 1000;
}

void HttpServer::setPort(unsigned short port)
{
    this->localAddress.sin_port = htons(port);
}

void HttpServer::setListeningIpAddress(std::string ipAddress)
{
    if (inet_aton(ipAddress.c_str(), &this->localAddress.sin_addr) == 0)
    {
        //TODO Throw custom exception class
        throw "IP address is invalid.";
    }
}

HttpServer::~HttpServer()
{
    if (this->isRunning())
    {
        this->stop();
    }
}

void HttpServer::start()
{
    if (this->isRunning())
    {
        //TODO Throw custom exception
        throw "Server already running";
    }

    this->serverThread = std::thread([this]() {
        this->serverThreadWork();
    });

    this->bIsRunning = true;
}

void HttpServer::stop()
{
    if (!this->isRunning())
    {
        //TODO Throw custom exception
        throw "Server is not running";
    }

    //TODO Cancel thread
    //pthread_cancel(this->serverThread);

    this->bIsRunning = false;
}

bool HttpServer::isRunning()
{
    return this->bIsRunning;
}

void HttpServer::serverThreadWork()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        //TODO Throw error
        throw "Error creating socket";
    }

    struct timeval sendTimeout;
    sendTimeout.tv_sec = this->sendTimeout;
    sendTimeout.tv_usec = 0;

    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (void*)&sendTimeout, sizeof(sendTimeout)) == -1)
    {
        //TODO Throw error
        throw "Error when setting send timeout";
    }

    struct timeval receiveTimeout;
    receiveTimeout.tv_sec = this->receiveTimeout;
    receiveTimeout.tv_usec = 0;

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (void*)&receiveTimeout, sizeof(receiveTimeout)) == -1)
    {
        //TODO Throw error
        throw "Error when setting receive timeout";
    }

    int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void*)&reuse, sizeof(reuse)) == -1)
    {
        //TODO Throw error
        throw "Error when setting socket reuse address";
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (void*)&reuse, sizeof(reuse)) == -1)
    {
        //TODO Throw error
        throw "Error when setting socket reuse port";
    }

    if (bind(sockfd, (struct sockaddr*)&this->localAddress, sizeof(this->localAddress)) != 0)
    {
        //TODO Throw error
        throw "Error when binding socket";
    }

    if (listen(sockfd, this->maxConnectionQueueLength) != 0)
    {
        //TODO Throw error
        throw "Error when switching socket into listening mode";
    }

    //TODO while(true) really?
    while(true)
    {
        int clientsockfd;
        struct sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);

        clientsockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientAddrSize);
        if (clientsockfd == -1)
        {
            //TODO Throw error
            int x = errno;
            throw "Error accepting client connection";
        }

        //std::thread clientThread([this, clientsockfd]() {
            this->clientConnectionThreadWork(clientsockfd);
        //});

        //clientThread.detach();
    }

    close(sockfd);
}

void HttpServer::setMaxConnectionQueueLength(int maxConnectionQueueLength)
{
    this->maxConnectionQueueLength = maxConnectionQueueLength;
}

void HttpServer::setSendTimeout(int sendTimeout)
{
    this->sendTimeout = sendTimeout;
}

void HttpServer::setReceiveTimeout(int receiveTimeout)
{
    this->receiveTimeout = receiveTimeout;
}

void HttpServer::clientConnectionThreadWork(int clientSocket)
{
    std::string httpRequestString;
    std::string line;
    //Read http headers
    while(true)
    {
        line = this->readLine(clientSocket);
        httpRequestString += line;

        if (string_extensions::ends_with(httpRequestString, this->httpRequestHeadersDataSeparator))
        {
            break;
        }
    }

    HttpRequest httpRequest(httpRequestString);

    //TODO When content-length is not set send INVALID_REQUEST Http response
    unsigned int contentLength = httpRequest.getContentLength();
    std::string httpRequestDataString = this->readString(clientSocket, contentLength);

    httpRequest.setHttpRequestData(httpRequestDataString);

    HttpResponse httpResponse = this->httpRequestHandler(httpRequest);

    //Set http headers etc

    this->sendResponse(clientSocket, httpResponse);

    close(clientSocket);
}

std::string HttpServer::readLine(int socket)
{
    std::string line;
    while(true)
    {
        char c;

        switch(recv(socket, &c, sizeof(char), 0))
        {
            case -1:
                if (errno == EWOULDBLOCK)
                {
                    continue;
                }

                return "";
            case 0:
                return "";
            default:
                line += c;
                break;
        }

        if (string_extensions::ends_with(line, this->httpRequestHeadersDataSeparator))
        {
            break;
        }
    }

    return line;
}

std::string HttpServer::readString(int socket, int length)
{
    ssize_t received = 0;
    std::string str;

    std::array<char, this->bufferSize> bufferArray;
    while (received < length)
    {
        ssize_t ret = recv(socket, bufferArray.data(), std::min(this->bufferSize - 1, (size_t)length - (size_t)received), 0);
        if (ret == -1)
        {
            if (errno == EWOULDBLOCK)
            {
                continue;
            }

            str = "";
            break;
        }

        std::string bufferString(bufferArray.data(), (size_t)ret);
        str += bufferString;
        received += ret;
    }

    return str;
}

void HttpServer::setHttpRequestHandler(
        HttpResponse (*httpRequestHandler)(HttpRequest httpRequest))
{
    this->httpRequestHandler = httpRequestHandler;
}

void HttpServer::sendResponse(int socket, HttpResponse &httpResponse)
{
    //TODO Send HTTP Header

    this->sendString(socket, httpResponse.getData());
}

void HttpServer::sendString(int socket, const std::string &str)
{
    ssize_t sent = 0;

    while (sent < str.length())
    {
        ssize_t ret = send(socket, str.c_str(), (size_t)str.length() - (size_t)sent, 0);
        if (ret == -1)
        {
            //TODO What to do?
            if (errno == EWOULDBLOCK)
            {
                continue;
            }

            break;
        }

        sent += ret;
    }
}
