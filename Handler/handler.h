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

class Handler
{
    std::map<std::string,std::string> usernameChallangeMap;
public:
    static HttpResponse httpRequestHandler(/*HttpRequest httpRequest,*/Json::Value testJson)
    {
        HttpResponse httpResponse;
        Json::Reader reader;
        Json::FastWriter writer;
        Json::Value jsonRequest, jsonResponse;
        jsonRequest = testJson;
        cout << jsonRequest;
        if (/*reader.parse(httpRequest.GetData(),jsonRequest))*/true)
        {
            Configuration &config= Configuration::getInstance();
            config.initialize("iptables.conf");
            if (config.isIPAddressBlocked("192.0.0.1"))
            {
                jsonResponse["error_code"] = 20;
                jsonResponse["error_message"] = "You are not authorized to use this server.";
                jsonResponse["challange"] = "";
            }
            else
            {
                IPTablesExecutor iptexec;
                switch (jsonRequest["command"].asInt())
                {
                    case LOGIN_INIT:

                        break;
                    case LOGIN_REQUEST:
                        break;
                    case LOGOUT:
                        break;
                    case GET_ALL_RULES:
                        iptexec.executeCommand( GET_ALL_RULES );
                        break;
                    case DELETE_RULE:

                        break;
                    case BLOCK_IP:

                        break;
                    case BLOCK_TCP_PORT:

                        break;
                    case BLOCK_UDP_PORT:

                        break;
                    case BLOCK_INCOMING_MAC:

                        break;
                    case RAW:

                        break;

                }
            }
        }
        else
        {
            jsonResponse["error_code"] = 10;
            jsonResponse["error_message"] = "Request was not in JSON format";
        }
        writer.write(jsonResponse);
        httpResponse.SetData(jsonResponse.asString());
        return httpResponse;
    }

};

#endif //TIN_IPTABLES_HANDLER_H
