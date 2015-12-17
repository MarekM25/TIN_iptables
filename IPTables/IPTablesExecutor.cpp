//
// Created by anowikowski on 10.12.15.
//

#include "IPTablesExecutor.h"
#include "../Logger/Logger.h"

IPTablesExecutor::IPTablesExecutor()
{
    mChainStrings[ 0 ] = "INPUT";
    mChainStrings[ 1 ] = "OUTPUT";
}

std::string IPTablesExecutor::getAllRules()
{
    std::string cmd = "iptables -L";

    // delete after debugging
    std::cout << "Wywolana komenda: " << cmd.c_str() << std::endl;
    return "";

    // uncomment after debugging
    //return exec( cmd.c_str() );
}

std::string IPTablesExecutor::deleteRule( chainType chain, unsigned short line )
{
    std::string cmd = "iptables -D " + mChainStrings[ chain ] + " " + std::to_string( line );

    // delete after debugging
    std::cout << "Wywolana komenda: " << cmd.c_str() << std::endl;
    return "";

    // uncomment after debugging
    //return exec( cmd.c_str() );
}

std::string IPTablesExecutor::blockIP( chainType chain, std::string ipAddress )
{
    char sourceOrDestination = chain == INPUT ? 's' : 'd';

    std::string cmd = "iptables -A " + mChainStrings[ chain ] + " -" + sourceOrDestination + " " + ipAddress + " -j DROP";

    // delete after debugging
    std::cout << "Wywolana komenda: " << cmd.c_str() << std::endl;
    return "";

    // uncomment after debugging
    //return exec( cmd.c_str() );
}

std::string IPTablesExecutor::blockTCP( chainType chain, unsigned short tcpPort )
{
    std::string cmd = "iptables -A " + mChainStrings[ chain ] + " -p tcp --dport " + std::to_string( tcpPort ) + " -j DROP";

    // delete after debugging
    std::cout << "Wywolana komenda: " << cmd.c_str() << std::endl;
    return "";

    // uncomment after debugging
    //return exec( cmd.c_str() );
}

std::string IPTablesExecutor::blockUDP( chainType chain, unsigned short udpPort )
{
    std::string cmd = "iptables -A " + mChainStrings[ chain ] + " -p udp --dport " + std::to_string( udpPort ) + " -j DROP";

    // delete after debugging
    std::cout << "Wywolana komenda: " << cmd.c_str() << std::endl;
    return "";

    // uncomment after debugging
    //return exec( cmd.c_str() );
}

std::string IPTablesExecutor::blockMAC( std::string macAddress )
{
    std::string cmd = "iptables -A INPUT -m mac --mac-source " + macAddress + " -j DROP";

    // delete after debugging
    std::cout << "Wywolana komenda: " << cmd.c_str() << std::endl;
    return "";

    // uncomment after debugging
    //return exec( cmd.c_str() );
}

std::string IPTablesExecutor::rawCommand( std::string cmd )
{
    // delete after debugging
    std::cout << "Wywolana komenda: " << cmd.c_str() << std::endl;
    return "";

    // uncomment after debugging
    //return exec( cmd.c_str() );
}

std::string IPTablesExecutor::exec( const char* cmd )
{
    std::shared_ptr< FILE > pipe( popen( cmd, "r" ), pclose );

    if ( !pipe )
    {
        LOG_ERR( "Error executing command: ", cmd );
        return "ERROR";
    }

    LOG( "Executed command: ", cmd );

    char buffer[ 128 ];
    std::string result = "";

    while ( !feof( pipe.get() ) )
    {
        if ( fgets( buffer, 128, pipe.get() ) )
            result += buffer;
    }

    return result;
}

