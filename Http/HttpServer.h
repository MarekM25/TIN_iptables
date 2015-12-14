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
    void setPort(unsigned short port);
    void setListeningIpAddress(std::string ipAddress);
    void start();
    void stop();
    bool isRunning();
    bool bIsRunning;
    void setMaxConnectionQueueLength(int maxConnectionQueueLength);
    void setSendTimeout(int sendTimeout);
    void setReceiveTimeout(int receiveTimeout);
    void setHttpRequestHandler(HttpResponse (*httpRequestHandler)(HttpRequest httpRequest));
private:
    sockaddr_in localAddress;
    std::thread serverThread;
    void serverThreadWork();
    void clientConnectionThreadWork(int clientSocket);
    int maxConnectionQueueLength;
    int sendTimeout;
    int receiveTimeout;
    std::string readLine(int socket);
    std::string readString(int socket, int length);
    void sendResponse(int socket, HttpResponse &httpResponse);
    void sendString(int socket, const std::string &str);
    const std::string newLineString = "\r\n";
    const std::string httpRequestHeadersDataSeparator = "\r\n\r\n";
    static const size_t bufferSize = 256;
    HttpResponse (*httpRequestHandler)(HttpRequest httpRequest);
};


#endif //TIN_IPTABLES_HTTPSERVER_H
