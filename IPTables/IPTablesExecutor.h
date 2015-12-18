//
// Created by anowikowski on 10.12.15.
//

#ifndef TIN_IPTABLES_IPTABLESEXECUTOR_H
#define TIN_IPTABLES_IPTABLESEXECUTOR_H

#include <string>
#include <sstream>

enum commandType
{
    LOGIN_INIT = 0,
    LOGIN_REQUEST = 1,
    LOGOUT = 2,
    GET_ALL_RULES = 3,      // no params
    DELETE_RULE = 4,        // param: unsigned short: line_number
    BLOCK_IP = 5,           // param: string        : incoming_ip_address
    BLOCK_TCP_PORT = 6,     // param: unsigned short: tcp_port_number
    BLOCK_UDP_PORT = 7,     // param: unsigned short: udp_port_number
    BLOCK_INCOMING_MAC = 8, // param: string        : incoming_mac_address
    RAW = 9                 // param: string        : raw_command
};

enum chainType
{
    INPUT = 0,
    OUTPUT = 1
};

class IPTablesExecutor
{
public:
    IPTablesExecutor();

    std::string getAllRules();
    std::string deleteRule( chainType chain, unsigned short line );
    std::string blockIP( chainType chain, std::string ipAddress );
    std::string blockTCP( chainType chain, unsigned short tcpPort );
    std::string blockUDP( chainType chain, unsigned short udpPort );
    std::string blockMAC( std::string macAddress );
    std::string rawCommand( std::string cmd );

private:
    std::string exec( const char* cmd );
    bool hasSpecialCharacter( const std::string& source );
    std::string mChainStrings[ 2 ];
};

#endif //TIN_IPTABLES_IPTABLESEXECUTOR_H
