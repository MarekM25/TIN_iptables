//
// Created by anowikowski on 10.12.15.
//
#define LOGGING_LEVEL_1

#include "Configuration.h"
#include "../Logger/Logger.h"
#include "../Extensions/StringExtensions.h"

void Configuration::initialize( string configurationFilePath )
{
    parseConfigFile( configurationFilePath );

    listenIpAddress = params[ "core/listen_ip" ];
    listenPort = string_extensions::stous( params[ "core/listen_port" ] );
    sessionTimeout = string_extensions::stous( params[ "core/session_timeout" ] );
    transmissionTimeout = string_extensions::stous( params[ "core/transmission_timeout" ] );
    logPath = params[ "paths/log_path" ];
    usersFilePath = params[ "paths/users_path" ];

    params.clear();

    this->serverIpAddress = "127.0.0.1";
    this->serverPort = 12345;
}

bool Configuration::parseConfigFile( string& configFile )
{
    //ifstream file( configFile.c_str() );
    ifstream file( "../iptables.conf" );

    if ( !file )
    {
        LOG_ERR( "Cannot open Config File" );
        return false;
    }

    string line;
    string key;
    string value;
    string section;
    unsigned short separatorPos;

    while ( getline( file, line ) )
    {

        if ( !line.length() ) continue;
        if ( line[ 0 ] == '#' ) continue;
        if ( line[ 0 ] == ';' ) continue;

        if ( line[ 0 ] == '[' )
        {
            section = string_extensions::trim( line.substr( 1, line.find( ']' ) - 1 ) );
            continue;
        }

        separatorPos = ( unsigned short ) line.find( ' ' );

        key = string_extensions::trim( line.substr( 0, separatorPos ) );
        value = string_extensions::trim( line.substr( separatorPos + 1 ) );

        params[ section + '/' + key ] = value;
    }

    return true;
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

string Configuration::getListenIpAddress()
{
    return listenIpAddress;
}

unsigned short Configuration::getListenPort()
{
    return listenPort;
}

unsigned short Configuration::getSessionTimeout()
{
    return sessionTimeout;
}

unsigned short Configuration::getTransmissionTimeout()
{
    return transmissionTimeout;
}

string Configuration::getLogPath()
{
    return logPath;
}

string Configuration::getUsersFilePath()
{
    return usersFilePath;
}