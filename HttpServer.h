//
// Created by anowikowski on 10.12.15.
//

#ifndef TIN_IPTABLES_HTTPSERVER_H
#define TIN_IPTABLES_HTTPSERVER_H

#include <string>
#include <netinet/in.h>

using namespace std;

class HttpServer
{
public:
    HttpServer(unsigned short port);
    HttpServer(string ipAddress, unsigned short port);
    ~HttpServer();
    void start();
    void stop();
    bool isRunning();
    bool bIsRunning;
private:
    sockaddr_in localAddress;
    pthread_t serverThread;
    void* serverThreadWork();
    static void *serverThreadWorkHelper(void *context);
};


#endif //TIN_IPTABLES_HTTPSERVER_H
