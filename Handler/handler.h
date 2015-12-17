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
    std::map<std::string,std::string> m_usernameChallangeMap;
    bool updateMap(std::string oldChallange, std::string newChallange);

    void insertToMap(std::string challange, std::string username);
    void removeFromMap(std::string challange);

public:
    Handler() {}
    ~Handler() {}
    HttpResponse HandleHttpRequest(HttpRequestContext httpRequestContext);


};

#endif //TIN_IPTABLES_HANDLER_H
