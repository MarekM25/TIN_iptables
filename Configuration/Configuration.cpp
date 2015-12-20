//
// Created by anowikowski on 10.12.15.
//

#include "Configuration.h"
#include "../Logger/Logger.h"
#include "../Extensions/StringExtensions.h"
#include "../Exception/ConfigurationException.h"

Configuration& Configuration::getInstance()
{
    static Configuration instance;

    return instance;
}

Configuration::Configuration()
{
    this->mIsServerIpAddressSet = false;
}

void Configuration::initialize( std::string configurationFilePath )
{
    parseConfigFile( configurationFilePath );

    parseBlacklistFile();

    parseUsersFile();
}

bool Configuration::parseConfigFile( std::string& configurationFilePath )
{
    std::map< std::string, std::string > params;

    std::ifstream file( configurationFilePath );

    if ( !file )
    {
        throw exception::configuration::invalid_config_path();
    }

    std::string line;
    std::string key;
    std::string value;
    std::string section;

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
    if (params.find( "core/server_ip" ) != params.end())
    {
        mServerIpAddress = params[ "core/server_ip" ];
        mIsServerIpAddressSet = true;
    }
    else
    {
        mIsServerIpAddressSet = false;
    }

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
    std::ifstream file( mBlacklistFilePath );

    if ( !file )
    {
        throw exception::configuration::invalid_blacklist_path();
    }

    std::string line;
    std::string range;
    std::string rangeEnd;

    while ( getline( file, line ) )
    {
        if ( !line.length() ) continue;
        if ( line[ 0 ] == '#' ) continue;
        if ( line[ 0 ] == ';' ) continue;

        auto index = line.find( '-' );

        if ( index == std::string::npos )
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

bool Configuration::parseUsersFile()
{
    std::ifstream file( mUsersFilePath );

    if ( !file )
    {
        throw exception::configuration::invalid_users_path();
    }

    std::string line;
    std::string range;
    std::string rangeEnd;

    while ( getline( file, line ) )
    {
        if ( !line.length() ) continue;
        if ( line[ 0 ] == '#' ) continue;
        if ( line[ 0 ] == ';' ) continue;

        auto index = line.find( ':' );

        if ( index == std::string::npos )
        {
            LOG_ERR("Wrong line in users file");
            continue;
        }

        range = string_extensions::trim( line.substr( 0, index ) );
        rangeEnd = string_extensions::trim( line.substr( index + 1 ) );

        mUsers.insert(make_pair(range, rangeEnd));
    }

    file.close();

    return true;
}

bool Configuration::isIPAddressBlocked( const std::string& ipAddress )
{
    uint32_t inputAddress = IPToUInt( ipAddress );

    if ( inputAddress == 0 )
        return true;

    for ( unsigned long i = 0; i < mBlacklist.size(); i++ )
    {
        if ( inputAddress >= IPToUInt( mBlacklist.at( i ).first )
             && inputAddress <= IPToUInt( mBlacklist.at( i ).second ) )
                    return true;
    }

    return false;
}

uint32_t Configuration::IPToUInt( const std::string& ipAddress )
{
    unsigned int a, b, c, d;
    uint32_t result = 0;

    if ( sscanf( ipAddress.c_str(), "%u.%u.%u.%u", &a, &b, &c, &d ) != 4 )
        return 0;

    result = a << 24;
    result |= b << 16;
    result |= c << 8;
    result |= d;
    return result;
}

std::string Configuration::getHostName()
{
    return mHostName;
}

std::string Configuration::getServerIpAddress()
{
    if (!this->isServerIpAddressSet())
    {
        throw exception::configuration::config_field_not_set();
    }

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

std::string Configuration::getLogPath()
{
    return mLogPath;
}

std::string Configuration::getUsersFilePath()
{
    return mUsersFilePath;
}

std::string Configuration::getBlacklistFilePath()
{
    return mBlacklistFilePath;
}

std::string Configuration::getUserPassword( std::string username )
{
    return mUsers[ username ];
}

bool Configuration::isServerIpAddressSet()
{
    return mIsServerIpAddressSet;
}
