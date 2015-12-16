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

    parseBlacklistFile();
}

bool Configuration::parseConfigFile( string& configurationFilePath )
{
    map< string, string > params;

    //TODO uncomment after debugging
    //ifstream file( configurationFilePath.c_str() );
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

        auto index = ( unsigned short ) line.find( ' ' );

        key = string_extensions::trim( line.substr( 0, index ) );
        value = string_extensions::trim( line.substr( index + 1 ) );

        params[ section + '/' + key ] = value;
    }

    file.close();

    mHostName = params[ "core/host_name" ];
    mServerIpAddress = params[ "core/server_ip" ];
    mServerPort = string_extensions::stous( params[ "core/server_port" ] );
    mSessionTimeout = string_extensions::stous( params[ "core/session_timeout" ] );
    mTransmissionTimeout = string_extensions::stous( params[ "core/transmission_timeout" ] );
    mLogPath = params[ "paths/log_path" ];
    mUsersFilePath = params[ "paths/users_path" ];
    mBlacklistFilePath = params[ "paths/blacklist_path" ];

    return true;
}

bool Configuration::parseBlacklistFile()
{
    //TODO uncomment after debugging
    //ifstream file( mBlacklistFilePath.c_str() );
    ifstream file( "../blacklist.txt" );

    if ( !file )
    {
        LOG_ERR( "Cannot open Blacklist file" );
        return false;
    }

    string line;
    string range;
    string rangeEnd;

    while ( getline( file, line ) )
    {
        if ( !line.length() ) continue;
        if ( line[ 0 ] == '#' ) continue;
        if ( line[ 0 ] == ';' ) continue;

        auto index = line.find( ' ' );

        if ( index == string::npos )
        {
            mBlacklist.push_back( make_pair( line, line ) );
            continue;
        }

        range = string_extensions::trim( line.substr( 0, index ) );
        rangeEnd = string_extensions::trim( line.substr( index + 1 ) );

        mBlacklist.push_back( make_pair( range, rangeEnd ) );
    }

    file.close();

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

string Configuration::getHostName()
{
    return mHostName;
}

string Configuration::getServerIpAddress()
{
    return mServerIpAddress;
}

unsigned short Configuration::getServerPort()
{
    return mServerPort;
}

unsigned short Configuration::getSessionTimeout()
{
    return mSessionTimeout;
}

unsigned short Configuration::getTransmissionTimeout()
{
    return mTransmissionTimeout;
}

string Configuration::getLogPath()
{
    return mLogPath;
}

string Configuration::getUsersFilePath()
{
    return mUsersFilePath;
}

string Configuration::getBlacklistFilePath()
{
    return mBlacklistFilePath;
}