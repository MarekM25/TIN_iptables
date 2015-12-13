//
// Created by anowikowski on 10.12.15.
//

#include "Configuration.h"

void Configuration::initialize(string configurationFilePath)
{
    parseConfigFile( configurationFilePath );

    listenIpAddress = params[ "core/listen_ip" ];
    listenPort = ( unsigned short ) stoi( params[ "core/listen_port" ] );
    sessionTimeout = ( unsigned short ) stoi( params[ "core/session_timeout" ] );
    transmissionTimeout = ( unsigned short ) stoi( params[ "core/transmission_timeout" ] );
    logPath = params[ "paths/log_path" ];
    usersFilePath = params[ "paths/users_path" ];

    params.clear();

    this->serverIpAddress = "127.0.0.1";
    this->serverPort = 12345;
}

bool Configuration::parseConfigFile( string& configFile )
{
    ifstream file( configFile.c_str() );
    //ifstream file( "/Users/Pawel/Documents/Studia/TIN/TIN_iptables/iptables.conf" );

    if ( !file )
        return false;

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
            section = trim( line.substr( 1, line.find( ']' ) - 1 ) );
            continue;
        }

        separatorPos = ( unsigned short ) line.find( ' ' );

        key = trim( line.substr( 0, separatorPos ) );
        value = trim( line.substr( separatorPos + 1 ) );

        params[ section + '/' + key ] = value;
    }

    return true;
}

string Configuration::trim( string source )
{
    string delimiters = " \t\r\n";
    string result( source );

    auto index = result.find_last_not_of( delimiters );
    if( index != string::npos )
        result.erase( ++index );

    index = result.find_first_not_of( delimiters );
    if( index != string::npos )
        result.erase( 0, index );
    else
        result.erase();

    return result;
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