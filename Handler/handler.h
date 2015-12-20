//
// Created by mmoraczynski on 16.12.15.
//

#ifndef TIN_IPTABLES_HANDLER_H
#define TIN_IPTABLES_HANDLER_H

#include "../Authorization/auth.h"
#include <json/json.h>
#include "../Configuration/Configuration.h"
#include "../Exception/IPTablesException.h"
#include "../Http/HttpServer.h"
#include "../IPTables/IPTablesExecutor.h"
#include <unistd.h>
#include <fstream>
#include "Validator.h"

enum commandType
{
    LOGIN_INIT = 0,
    LOGOUT = 1,
    GET_ALL_RULES = 2,      // no params
    DELETE_RULE = 3,        // param: unsigned short: line_number
    BLOCK_IP = 4,           // param: string        : incoming_ip_address
    BLOCK_TCP_PORT = 5,     // param: unsigned short: tcp_port_number
    BLOCK_UDP_PORT = 6,     // param: unsigned short: udp_port_number
    BLOCK_INCOMING_MAC = 7, // param: string        : incoming_mac_address
    RAW = 8                 // param: string        : raw_command
};

class Handler : public HttpServerRequestHandlerInterface
{
    std::map<std::string,std::string> m_usernameChallengeMap;
    bool checkIfChallengeInMap(std::string challenge);
    void updateMap(std::string oldChallenge, std::string newChallenge);
    void insertToMap(std::string challenge, std::string username);
    void removeFromMap(std::string challenge);

    static const std::string m_sRequiredPathHttpHeaderValue;

public:
    Handler() {}
    ~Handler() {}
    HttpResponse HandleHttpRequest(HttpRequestContext httpRequestContext);
};

#endif //TIN_IPTABLES_HANDLER_H
