//
// Created by anowikowski on 10.12.15.
//

#include "Configuration.h"

void Configuration::Initialize(string configurationFilePath)
{
    //TODO Load configuration from file
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