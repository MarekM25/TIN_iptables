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
    std::map<std::string,std::string> m_usernameChallangeMap;
    void updateMap(std::string oldChallange, std::string username,std::string newChallange)
    {
        std::map<std::string,std::string>::iterator it = m_usernameChallangeMap.find(oldChallange);
        if (it != m_usernameChallangeMap.end())
            m_usernameChallangeMap.erase(it);
        m_usernameChallangeMap.insert(std::pair<std::string,std::string>(newChallange,username));
    }

    void insertToMap(std::string challange, std::string username)
    {
        m_usernameChallangeMap.insert(std::pair<std::string,std::string>(challange,username));
    }

    void removeFromMap(std::string challange)
    {
        std::map<std::string,std::string>::iterator it = m_usernameChallangeMap.find(challange);
        if (it != m_usernameChallangeMap.end())
            m_usernameChallangeMap.erase(it);
    }

public:
    HttpResponse httpRequestHandler(HttpRequestContext httpRequestContext)
    {
        HttpResponse httpResponse;
        HttpRequest httpRequest = httpRequestContext.GetHttpRequest();

        if (httpRequest.GetMethod() != HttpRequestMethod::POST)
        {
            httpResponse.SetStatus(HttpResponseStatus::METHOD_NOT_ALLOWED_405);
            return httpResponse;
        }

        Json::Reader reader;
        Json::FastWriter writer;
        Json::Value jsonRequest, jsonResponse;
        if (reader.parse(httpRequest.GetData(),jsonRequest))
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
                Authorization auth;
                if (jsonRequest["command"].isNull())
                {
                    jsonResponse["error_code"] = 11;
                    jsonResponse["error_message"] = "Request was not in valid format";
                }
                else {
                    switch (jsonRequest["command"].asInt()) {
                        case LOGIN_INIT:
                            jsonRequest = auth.loginInit(jsonRequest["username"].asString());
                            if (jsonRequest["error_code"] == "0") {
                                insertToMap(jsonRequest["challange"].asString(), jsonRequest["username"].asString());
                            }
                            break;
                        case LOGIN_REQUEST:

                            break;
                        case LOGOUT:
                            break;
                        case GET_ALL_RULES:
                            iptexec.executeCommand(GET_ALL_RULES);
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
        }
        else
        {
            jsonResponse["error_code"] = 10;
            jsonResponse["error_message"] = "Request was not in JSON format";
        }
        writer.write(jsonResponse);
        httpResponse.SetData(httpRequest.GetData());
        httpResponse.SetStatus(HttpResponseStatus::OK_200);
        return httpResponse;
    }

};

#endif //TIN_IPTABLES_HANDLER_H
