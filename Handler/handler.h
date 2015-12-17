//
// Created by mmoraczynski on 16.12.15.
//

#ifndef TIN_IPTABLES_HANDLER_H
#define TIN_IPTABLES_HANDLER_H

#include "../Authorization/auth.h"
#include <json/json.h>
#include "../Configuration/Configuration.h"
#include "../Http/HttpServer.h"
#include "../IPTables/IPTablesExecutor.h"
#include <unistd.h>
#include <fstream>

class Handler : public HttpServerRequestHandlerInterface
{
    std::map<std::string,std::string> m_usernameChallengeMap;
    bool updateMap(std::string oldChallenge, std::string newChallenge);

    void insertToMap(std::string challenge, std::string username);
    void removeFromMap(std::string challenge);

    static const std::string m_sRequiredPathHttpHeaderValue;

public:
    Handler() {}
    ~Handler() {}
    HttpResponse HandleHttpRequest(HttpRequestContext httpRequestContext);


};

#endif //TIN_IPTABLES_HANDLER_H
