//
// Created by anowikowski on 10.12.15.
//

#include <arpa/inet.h>
#include <pthread.h>
#include "HttpServer.h"

HttpServer::HttpServer(unsigned short port)
{
    this->localAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    this->localAddress.sin_family = AF_INET;
    this->localAddress.sin_port = htons(port);
    this->bIsRunning = false;
}

HttpServer::HttpServer(string ipAddress, unsigned short port)
{
    if (inet_aton(ipAddress.c_str(), &this->localAddress.sin_addr) == 0)
    {
        //TODO Throw custom exception class
        throw "IP address is invalid.";
    }

    this->localAddress.sin_family = AF_INET;
    this->localAddress.sin_port = htons(port);
    this->bIsRunning = false;
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
        //TODO  Throw custom exception
        throw "Server already running";
    }

    int ret = pthread_create(&this->serverThread, NULL, HttpServer::serverThreadWorkHelper, this);
    if (ret)
    {
        //TODO  Throw custom exception
        throw "Error creating server thread";
    }

    this->bIsRunning = true;
}

void HttpServer::stop()
{
    if (!this->isRunning())
    {
        //TODO  Throw custom exception
        throw "Server is not running";
    }

    //TODO Cancel thread more elegant?
    pthread_cancel(this->serverThread);

    this->bIsRunning = false;
}

bool HttpServer::isRunning()
{
    return this->bIsRunning;
}

void* HttpServer::serverThreadWork()
{
    printf("Running\n");

    return nullptr;
}

void* HttpServer::serverThreadWorkHelper(void *context)
{
    return ((HttpServer*)context)->serverThreadWork();
}