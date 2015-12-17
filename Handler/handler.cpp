//
// Created by mmoraczynski on 16.12.15.
//
#include "handler.h"

HttpResponse Handler::HandleHttpRequest(HttpRequestContext httpRequestContext)
{
    cout<<"dsaf";
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
            else
            {
                int jsonCommand = jsonRequest["command"].asInt();
                if (jsonCommand!=LOGOUT && jsonCommand!=LOGIN_INIT)
                {
                    std::string challange = auth.generateChallenge();

                    bool challangeWasCorrect = updateMap(jsonRequest["challange"].asString(),challange);
                    if (challangeWasCorrect)
                    {
                        jsonResponse["error_code"] = 0;
                        jsonResponse["error_message"] = "OK";
                        jsonResponse["challange"] = challange;
                    }
                    else
                    {
                        jsonResponse["error_code"] = 22;
                        jsonResponse["error_message"] = "Challange was not correct";
                        jsonResponse["challange"] = "";
                    }
                }
                switch (jsonCommand) {
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
        jsonResponse["challange"] = "";
    }
    writer.write(jsonResponse);
    httpResponse.SetData(httpRequest.GetData());
    httpResponse.SetStatus(HttpResponseStatus::OK_200);
    return httpResponse;
}


bool Handler::updateMap(std::string oldChallange, std::string newChallange)
{
    std::map<std::string,std::string>::iterator it = m_usernameChallangeMap.find(oldChallange);
    if (it != m_usernameChallangeMap.end()) {
        m_usernameChallangeMap.erase(it);
        m_usernameChallangeMap.insert(std::pair<std::string, std::string>(newChallange, it->second));
        return true;
    }
    return false;
}

void Handler::insertToMap(std::string challange, std::string username)
{
    m_usernameChallangeMap.insert(std::pair<std::string,std::string>(challange,username));
}

void Handler::removeFromMap(std::string challange)
{
    std::map<std::string,std::string>::iterator it = m_usernameChallangeMap.find(challange);
    if (it != m_usernameChallangeMap.end())
        m_usernameChallangeMap.erase(it);
}