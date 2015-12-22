//
// Created by anowikowski on 10.12.15.
//

#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <algorithm>
#include <array>
#include "HttpServer.h"
#include "../Extensions/StringExtensions.h"
#include "../Exception/HttpException.h"
#include "../Logger/Logger.h"

const std::string HttpServer::m_sNewLineString = "\r\n";
const std::string HttpServer::m_sHttpRequestHeadersDataSeparator = "\r\n\r\n";
const std::string HttpServer::m_sHttpRequestHeaderNameValueSeparator = ": ";
const std::string HttpServer::m_sHttpContentLengthHttpHeaderName = "Content-Length";
const std::size_t HttpServer::m_bufferSize = 256;
std::vector<HttpHeader> HttpServer::m_staticHttpResponseHeaders =
        {
                HttpHeader("Server", "IpTablesServer/1.0"),
                HttpHeader("Content-Type", "application/json"),
                HttpHeader("Content-Encoding", "identity"),
                HttpHeader("Transfer-Encoding", "identity"),
                HttpHeader("Connection", "close"),
        };

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

    this->m_bIsServerStopRequested = false;

    int iSocket = this->InitializeListeningSocket();

    this->m_serverThread = std::thread([this, iSocket]() {
        this->ServerThreadWork(iSocket);
    });

    this->m_bIsRunning = true;
}

void HttpServer::Stop()
{
    if (!this->IsRunning())
    {
        throw exception::http::http_server_not_running();
    }

    this->m_bIsServerStopRequested = true;
    this->m_serverThread.join();

    this->m_bIsRunning = false;
}

bool HttpServer::IsRunning()
{
    return this->m_bIsRunning;
}

void HttpServer::ServerThreadWork(int iSocket)
{
    fd_set ready;
    struct timeval selectTimeout;
    int max_fd;
    selectTimeout.tv_sec = 1;
    selectTimeout.tv_usec = 0;

    while(!this->m_bIsServerStopRequested)
    {
        FD_ZERO(&ready);
        FD_SET(iSocket, &ready);
        max_fd = iSocket;

        if (select(max_fd + 1, &ready, (fd_set *)0, (fd_set *)0, &selectTimeout) == -1)
        {
            throw exception::http::internal_socket_error();
        }

        if (!FD_ISSET(iSocket, &ready))
        {
            continue;
        }

        int clientsockfd;
        struct sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);

        clientsockfd = accept(iSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
        if (clientsockfd == -1)
        {
            if (errno == EAGAIN)
            {
                continue;
            }

#ifndef NDEBUG
            if (errno == EINTR)
            {
                continue;
            }
#endif

            throw exception::http::internal_socket_error();
        }

        std::thread clientThread([this, clientsockfd]() {
            this->ClientConnectionThreadWork(clientsockfd);
        });

        clientThread.detach();
    }

    close(iSocket);
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
    this->SetSocketNonBlocking(clientSocket);

    std::string httpRequestString;

    while(true)
    {
        httpRequestString = this->ReadAvailableString(clientSocket);

        if (httpRequestString.find(this->m_sHttpRequestHeadersDataSeparator) != std::string::npos)
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
        unsigned int unreadContentLength = contentLength - (int)httpRequest.GetData().length();
        std::string httpRequestDataString = this->ReadString(clientSocket, unreadContentLength);
        httpRequest.AddData(httpRequestDataString);
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

    HttpResponse httpResponse = this->m_pHttpServerRequestHandlerContextObject->HandleHttpRequest(httpRequestContext);

    this->SendResponse(clientSocket, httpResponse);

    close(clientSocket);
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

            throw exception::http::internal_socket_error();
        }

        std::string bufferString(bufferArray.data(), (std::size_t)ret);
        str += bufferString;
        received += ret;
    }

    return str;
}

std::string HttpServer::ReadAvailableString(int socket)
{
    std::string str;
    std::array<char, m_bufferSize> bufferArray;

    while(true)
    {
        ssize_t ret = recv(socket, bufferArray.data(), this->m_bufferSize, 0);
        if (ret < 1)
        {
            break;
        }

        std::string bufferString(bufferArray.data(), (std::size_t)ret);
        str += bufferString;
    }

    return str;
}

void HttpServer::SetHttpRequestHandlerContextObject(HttpServerRequestHandlerInterface *pHttpServerRequestHandlerContextObject)
{
    this->m_pHttpServerRequestHandlerContextObject = pHttpServerRequestHandlerContextObject;
}

void HttpServer::SendResponse(int socket, HttpResponse &httpResponse)
{
    this->AddStaticHttpResponseHeadersToHttpResponse(httpResponse);
    this->SetHttpResponseContentLengthHeader(httpResponse);

    std::string httpResponseString = httpResponse.GetHttpVersion();
    httpResponseString += " ";
    httpResponseString += std::to_string((int)httpResponse.GetStatus());
    httpResponseString += " ";
    httpResponseString += httpResponse.MapHttpResponseStatusToString(httpResponse.GetStatus());
    httpResponseString += this->m_sNewLineString;

    HttpHeaderCollection httpHeaderCollection = httpResponse.GetHttpHeaderCollection();

    std::for_each(httpHeaderCollection.begin(), httpHeaderCollection.end(),
        [this, &httpResponseString] (HttpHeader &header)
            {
                httpResponseString += header.GetHeaderName();
                httpResponseString += this->m_sHttpRequestHeaderNameValueSeparator;
                httpResponseString += header.GetHeaderValue();
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
            if (errno == EWOULDBLOCK)
            {
                continue;
            }

            throw exception::http::internal_socket_error();
        }

        sent += ret;
    }
}

void HttpServer::SendBadRequestResponse(int iSocket)
{
    HttpResponse httpResponse;
    httpResponse.SetStatus(HttpResponseStatus::BAD_REQUEST);
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
    httpResponse.SetStatus(HttpResponseStatus::INTERNAL_SERVER_ERROR);
    this->SendResponse(iSocket, httpResponse);
}

void HttpServer::AddStaticHttpResponseHeadersToHttpResponse(HttpResponse &httpResponse)
{
    std::for_each(this->m_staticHttpResponseHeaders.begin(), this->m_staticHttpResponseHeaders.end(),
        [this, &httpResponse] (HttpHeader &header)
        {
            httpResponse.AddHeader(HttpHeader(header.GetHeaderName(), header.GetHeaderValue()));
        });
}

void HttpServer::SetHttpResponseContentLengthHeader(HttpResponse &httpResponse)
{
    unsigned long dataLength = httpResponse.GetData().length();
    std::string contentLengthHeaderValue = std::to_string(dataLength);
    httpResponse.AddHeader(HttpHeader(this->m_sHttpContentLengthHttpHeaderName, contentLengthHeaderValue));
}

void HttpServer::SetSocketNonBlocking(int iSocket)
{
    int iSocketFlags = fcntl(iSocket, F_GETFL, 0);
    if (iSocketFlags == -1)
    {
        throw exception::http::internal_socket_error();
    }

    iSocketFlags |= O_NONBLOCK;
    if (fcntl(iSocket, F_SETFL, iSocketFlags) == -1)
    {
        throw exception::http::internal_socket_error();
    }
}

int HttpServer::InitializeListeningSocket()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        throw exception::http::internal_socket_error();
    }

    struct timeval sendTimeout;
    sendTimeout.tv_sec = this->m_iSendTimeout;
    sendTimeout.tv_usec = 0;

    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (void*)&sendTimeout, sizeof(sendTimeout)) == -1)
    {
        throw exception::http::internal_socket_error();
    }

    struct timeval receiveTimeout;
    receiveTimeout.tv_sec = this->m_iReceiveTimeout;
    receiveTimeout.tv_usec = 0;

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (void*)&receiveTimeout, sizeof(receiveTimeout)) == -1)
    {
        throw exception::http::internal_socket_error();
    }

    //Reuse IP and Port can lead to multiple instances of the http server on the same ip & port
    /*int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void*)&reuse, sizeof(reuse)) == -1)
    {
        throw exception::http::internal_socket_error();
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (void*)&reuse, sizeof(reuse)) == -1)
    {
        throw exception::http::internal_socket_error();
    }*/

    if (bind(sockfd, (struct sockaddr*)&this->m_localAddress, sizeof(this->m_localAddress)) != 0)
    {
        throw exception::http::internal_socket_error();
    }

    if (listen(sockfd, this->m_iMaxConnectionQueueLength) != 0)
    {
        throw exception::http::internal_socket_error();
    }

    return sockfd;
}
