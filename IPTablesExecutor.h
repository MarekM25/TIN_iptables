//
// Created by anowikowski on 10.12.15.
//

#ifndef TIN_IPTABLES_IPTABLESEXECUTOR_H
#define TIN_IPTABLES_IPTABLESEXECUTOR_H

#include <string>
#include <sstream>
#include <iostream>

enum commandType
{
    GET_ALL_RULES,      // no params
    DELETE_RULE,        // param: unsigned short: line_number
    BLOCK_IP,           // param: string        : incoming_ip_address
    BLOCK_TCP_PORT,     // param: unsigned short: tcp_port_number
    BLOCK_UDP_PORT,     // param: unsigned short: udp_port_number
    BLOCK_INCOMING_MAC, // param: string        : incoming_mac_address
    RAW                 // param: string        : raw_command
};

enum chainType
{
    INPUT,
    OUTPUT
};

class IPTablesExecutor
{
public:
    IPTablesExecutor();

    template< typename T = std::string >
    std::string executeCommand( commandType command, T param = "", chainType chain = INPUT );
    std::string exec( const char* cmd );
};

template< typename T >
std::string IPTablesExecutor::executeCommand( commandType command, T param, chainType chain )
{
    std::ostringstream paramStr;
    paramStr << param;

    std::string cmd;
    std::string sourceStr;
    std::string chainStr;

    if ( chain == OUTPUT )
    {
        chainStr = "OUTPUT";
        sourceStr = "d";
    }
    else
    {
        chainStr = "INPUT";
        sourceStr = "s";
    }

    switch ( command )
    {
        case GET_ALL_RULES:
            cmd = "iptables -L";
            break;
        case DELETE_RULE:
            cmd = "iptables -D " + chainStr + " " + paramStr.str();
            break;
        case BLOCK_IP:
            cmd = "iptables -A " + chainStr + " -" + sourceStr + " " + paramStr.str() + " -j DROP";
            break;
        case BLOCK_TCP_PORT:
            cmd = "iptables -A " + chainStr + " -p tcp --dport " + paramStr.str() + " -j DROP";
            break;
        case BLOCK_UDP_PORT:
            cmd = "iptables -A " + chainStr + " -p udp --dport " + paramStr.str() + " -j DROP";
            break;
        case BLOCK_INCOMING_MAC:
            cmd = "iptables -A INPUT -m mac --mac-source " + paramStr.str() + " -j DROP";
            break;
        case RAW:
            cmd = paramStr.str();
            break;
    }

    std::cout << cmd.c_str() << std::endl;
    return "";

    //return exec( cmd.c_str() );
}

#endif //TIN_IPTABLES_IPTABLESEXECUTOR_H
