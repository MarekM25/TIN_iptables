//
// Created by anowikowski on 10.12.15.
//

#include "Configuration.h"

void Configuration::initialize(string configurationFilePath)
{
    //TODO Load configuration from file
    this->serverIpAddress = "127.0.0.1";
    this->serverPort = 12345;
}

Configuration& Configuration::getInstance()
{
    static Configuration instance;

    return instance;
}

Configuration::~Configuration()
{

}

Configuration::Configuration()
{

}

string Configuration::getServerIpAddress()
{
    return this->serverIpAddress;
}

unsigned short Configuration::getServerPort()
{
    return this->serverPort;
}