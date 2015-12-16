//
// Created by anowikowski on 10.12.15.
//

#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <array>
#include <algorithm>
#include "HttpServer.h"
#include "../Extensions/StringExtensions.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "../Exception/HttpException.h"

const std::string HttpServer::m_sNewLineString = "\r\n";
const std::string HttpServer::m_sHttpRequestHeadersDataSeparator = "\r\n\r\n";
const std::string HttpServer::m_sHttpRequestHeaderNameValueSeparator = ": ";
const std::size_t HttpServer::m_bufferSize = 256;

HttpServer::HttpServer()
{
    this->m_localAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    this->m_localAddress.sin_family = AF_INET;
    this->m_bIsRunning = false;
    this->m_iMaxConnectionQueueLength = 1;
    this->m_iSendTimeout = 1000;
    this->m_iReceiveTimeout = 1000;
}

void HttpServer::SetPort(unsigned short port)
{
    this->m_localAddress.sin_port = htons(port);
}

void HttpServer::SetListeningIpAddress(std::string ipAddress)
{
    if (inet_aton(ipAddress.c_str(), &this->m_localAddress.sin_addr) == 0)
    {
        throw exception::http::invalid_ip_address();
    }
}

HttpServer::~HttpServer()
{
    if (this->IsRunning())
    {
        this->Stop();
    }
}

void HttpServer::Start()
{
    if (this->IsRunning())
    {
        throw exception::http::http_server_already_running();
    }

    this->m_serverThread = std::thread([this]() {
        this->ServerThreadWork();
    });

    this->m_bIsRunning = true;
}

void HttpServer::Stop()
{
    if (!this->IsRunning())
    {
        throw exception::http::http_server_not_running();
    }

    //TODO Cancel thread
    //pthread_cancel(this->m_serverThread);

    this->m_bIsRunning = false;
}

bool HttpServer::IsRunning()
{
    return this->m_bIsRunning;
}

void HttpServer::ServerThreadWork()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        //TODO Throw error
        throw "Error creating socket";
    }

    struct timeval sendTimeout;
    sendTimeout.tv_sec = this->m_iSendTimeout;
    sendTimeout.tv_usec = 0;

    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (void*)&sendTimeout, sizeof(sendTimeout)) == -1)
    {
        //TODO Throw error
        throw "Error when setting send timeout";
    }

    struct timeval receiveTimeout;
    receiveTimeout.tv_sec = this->m_iReceiveTimeout;
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

    if (bind(sockfd, (struct sockaddr*)&this->m_localAddress, sizeof(this->m_localAddress)) != 0)
    {
        //TODO Throw error
        throw "Error when binding socket";
    }

    if (listen(sockfd, this->m_iMaxConnectionQueueLength) != 0)
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
            throw "Error accepting client connection";
        }

        //std::thread clientThread([this, clientsockfd]() {
        this->ClientConnectionThreadWork(clientsockfd);
        //});

        //clientThread.detach();
    }

    close(sockfd);
}

void HttpServer::SetMaxConnectionQueueLength(int maxConnectionQueueLength)
{
    this->m_iMaxConnectionQueueLength = maxConnectionQueueLength;
}

void HttpServer::SetSendTimeout(int sendTimeout)
{
    this->m_iSendTimeout = sendTimeout;
}

void HttpServer::SetReceiveTimeout(int receiveTimeout)
{
    this->m_iReceiveTimeout = receiveTimeout;
}

void HttpServer::ClientConnectionThreadWork(int clientSocket)
{
    std::string httpRequestString;
    std::string line;
    //Read http headers
    while(true)
    {
        line = this->ReadLine(clientSocket);
        httpRequestString += line;

        if (string_extensions::ends_with(httpRequestString, this->m_sHttpRequestHeadersDataSeparator))
        {
            break;
        }
    }

    HttpRequest httpRequest;
    try
    {
        httpRequest.SetRawHttpRequest(httpRequestString);
    }
    catch (const exception::http::invalid_http_request &e)
    {
        this->SendBadRequestResponse(clientSocket);
        close(clientSocket);
        return;
    }

    try
    {
        unsigned int contentLength = httpRequest.GetContentLength();
        std::string httpRequestDataString = this->ReadString(clientSocket, contentLength);
        httpRequest.SetData(httpRequestDataString);
    }
    catch (const exception::http::http_header_not_present &e)
    {
        //If Content-Length header no present, assume there is no data
        httpRequest.SetData("");
    }

    HttpRequestContext httpRequestContext;
    httpRequestContext.SetHttpRequest(httpRequest);
    std::string sourceIpAddress;
    try
    {
        sourceIpAddress = this->GetClientIpAddress(clientSocket);
    }
    catch (const exception::http::http_server_exception &e)
    {
        this->SendInternalServerErrorResponse(clientSocket);
        close(clientSocket);
        return;
    }

    httpRequestContext.SetClientIpAddress(sourceIpAddress);

    HttpResponse httpResponse = this->m_pHttpRequestHandler(httpRequestContext);
    httpResponse.SetStatus(HttpResponseStatus::OK_200);

    //Set http headers etc

    this->SendResponse(clientSocket, httpResponse);

    close(clientSocket);
}

std::string HttpServer::ReadLine(int socket)
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

        if (string_extensions::ends_with(line, this->m_sHttpRequestHeadersDataSeparator))
        {
            break;
        }
    }

    return line;
}

std::string HttpServer::ReadString(int socket, int length)
{
    ssize_t received = 0;
    std::string str;

    std::array<char, m_bufferSize> bufferArray;
    while (received < length)
    {
        ssize_t ret = recv(socket, bufferArray.data(), std::min(this->m_bufferSize - 1, (size_t)length - (size_t)received), 0);
        if (ret == -1)
        {
            if (errno == EWOULDBLOCK)
            {
                continue;
            }

            str = "";
            break;
        }

        std::string bufferString(bufferArray.data(), (std::size_t)ret);
        str += bufferString;
        received += ret;
    }

    return str;
}

void HttpServer::SetHttpRequestHandler(HttpServerFunctionHandlerPrototype *httpRequestHandler)
{
    this->m_pHttpRequestHandler = httpRequestHandler;
}

void HttpServer::SendResponse(int socket, HttpResponse &httpResponse)
{
    std::string httpResponseString = httpResponse.GetHttpVersion();
    httpResponseString += " ";
    httpResponseString += std::to_string(httpResponse.MapHttpResponseStatusToInt(httpResponse.GetStatus()));
    httpResponseString += " ";
    httpResponseString += httpResponse.MapHttpResponseStatusToString(httpResponse.GetStatus());
    httpResponseString += this->m_sNewLineString;

    std::for_each(httpResponse.GetHeaders().begin(), httpResponse.GetHeaders().end(),
        [this, &httpResponseString] (const std::pair<std::string, std::string> &header)
            {
                httpResponseString += header.first;
                httpResponseString += this->m_sHttpRequestHeaderNameValueSeparator;
                httpResponseString += header.second;
                httpResponseString += this->m_sNewLineString;
            });

    httpResponseString += this->m_sNewLineString;
    httpResponseString += httpResponse.GetData();

    this->SendString(socket, httpResponseString);
}

void HttpServer::SendString(int socket, const std::string &str)
{
    ssize_t sent = 0;

    while ((size_t)sent < str.length())
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

void HttpServer::SendBadRequestResponse(int iSocket)
{
    HttpResponse httpResponse;
    httpResponse.SetStatus(HttpResponseStatus::BAD_REQUEST_400);
    this->SendResponse(iSocket, httpResponse);
}

std::string HttpServer::GetClientIpAddress(int iSocket)
{
    char buffer[INET_ADDRSTRLEN] = "";
    struct sockaddr_in client;
    socklen_t len = sizeof(client);

    if (getpeername(iSocket, (struct sockaddr *)&client, &len) != 0)
    {
        throw exception::http::internal_socket_error();
    }

    inet_ntop(AF_INET, &client.sin_addr, buffer, sizeof(buffer));

    return std::string(buffer);
}

void HttpServer::SendInternalServerErrorResponse(int iSocket)
{
    HttpResponse httpResponse;
    httpResponse.SetStatus(HttpResponseStatus::INTERNAL_SERVER_ERROR_500);
    this->SendResponse(iSocket, httpResponse);
}
