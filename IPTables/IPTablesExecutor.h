//
// Created by anowikowski on 10.12.15.
//

#ifndef TIN_IPTABLES_IPTABLESEXECUTOR_H
#define TIN_IPTABLES_IPTABLESEXECUTOR_H

#include <string>
#include <sstream>


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
