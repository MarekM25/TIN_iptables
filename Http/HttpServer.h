//
// Created by anowikowski on 10.12.15.
//

#ifndef TIN_IPTABLES_HTTPSERVER_H
#define TIN_IPTABLES_HTTPSERVER_H

#include <string>
#include <thread>
#include <netinet/in.h>
#include "HttpResponse.h"
#include "HttpRequest.h"

class HttpServer
{
public:
    HttpServer();
    ~HttpServer();
    void SetPort(unsigned short port);
    void SetListeningIpAddress(std::string ipAddress);
    void Start();
    void Stop();
    bool IsRunning();
    void SetMaxConnectionQueueLength(int maxConnectionQueueLength);
    void SetSendTimeout(int sendTimeout);
    void SetReceiveTimeout(int receiveTimeout);
    void SetHttpRequestHandler(HttpResponse (*httpRequestHandler)(HttpRequest));
private:
    sockaddr_in m_localAddress;
    std::thread m_serverThread;
    void ServerThreadWork();
    void ClientConnectionThreadWork(int clientSocket);
    int m_iMaxConnectionQueueLength;
    int m_iSendTimeout;
    int m_iReceiveTimeout;
    bool m_bIsRunning;
    std::string ReadLine(int socket);
    std::string ReadString(int socket, int length);
    void SendResponse(int socket, HttpResponse &httpResponse);
    void SendString(int socket, const std::string &str);
    static const std::string m_sNewLineString;
    static const std::string m_sHttpRequestHeadersDataSeparator;
    static const std::string m_sHttpRequestHeaderNameValueSeparator;
    static const std::size_t m_bufferSize;
    HttpResponse (*m_pHttpRequestHandler)(HttpRequest httpRequest);
    void SendBadRequestResponse(int iSocket);
};


#endif //TIN_IPTABLES_HTTPSERVER_H
