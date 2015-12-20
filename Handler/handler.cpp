//
// Created by mmoraczynski on 16.12.15.
//
#include "handler.h"
#include "../Logger/Logger.h"

const std::string Handler::m_sRequiredPathHttpHeaderValue = "/iptables_mgmt";

HttpResponse Handler::HandleHttpRequest(HttpRequestContext httpRequestContext)
{
    HttpResponse httpResponse;
    HttpRequest httpRequest = httpRequestContext.GetHttpRequest();
    if (httpRequest.GetMethod() != HttpRequestMethod::POST)
    {
        httpResponse.SetStatus(HttpResponseStatus::METHOD_NOT_ALLOWED);
        return httpResponse;
    }
    if (httpRequest.GetPath() != this->m_sRequiredPathHttpHeaderValue)
    {
        httpResponse.SetStatus(HttpResponseStatus::NOT_FOUND);
        return httpResponse;
    }

    Json::Reader reader;
    Json::FastWriter writer;
    Json::Value jsonRequest, jsonResponse;
    if (!reader.parse(httpRequest.GetData(),jsonRequest))
    {
        jsonResponse["error_code"] = 10;
        jsonResponse["error_message"] = "Request was not in JSON format";
        jsonResponse["challenge"] = "";
        httpResponse.SetData(jsonResponse.toStyledString());
        httpResponse.SetStatus(HttpResponseStatus::OK);
        return httpResponse;
    }
    Configuration &config= Configuration::getInstance();

        if (config.isIPAddressBlocked(httpRequestContext.GetClientIpAddress()))
        {
            jsonResponse["error_code"] = 20;
            jsonResponse["error_message"] = "You are not authorized to use this server.";
            jsonResponse["challenge"] = "";
            httpResponse.SetData(jsonResponse.toStyledString());
            httpResponse.SetStatus(HttpResponseStatus::UNAUTHORIZED);
            return httpResponse;
        }
            IPTablesExecutor iptexec;
            Authorization auth;
            Validator validator;
            if (validator.validate(jsonRequest))
            {
                jsonResponse["error_code"] = 11;
                jsonResponse["error_message"] = "Request was not in valid format";
                jsonResponse["challenge"] = "";
                writer.write(jsonResponse);
                httpResponse.SetData(jsonResponse.toStyledString());
                httpResponse.SetStatus(HttpResponseStatus::OK);
                return httpResponse;
            }

                int jsonCommand = jsonRequest["command"].asInt();
                if (jsonCommand == LOGIN_INIT)
                {
                    jsonResponse = auth.loginInit(jsonRequest["params"]["username"].asString());
                    if (jsonResponse["error_code"] == 0) {
                        insertToMap(jsonResponse["challenge"].asString(), jsonRequest["params"]["username"].asString());
                    }
                    writer.write(jsonResponse);
                    httpResponse.SetData(jsonResponse.toStyledString());
                    httpResponse.SetStatus(HttpResponseStatus::OK);
                    return httpResponse;
                }
                if (!checkIfChallengeInMap(jsonRequest["challenge"].asString()))
                {
                    jsonResponse["error_code"] = 22;
                    jsonResponse["error_message"] = "Challenge was not correct";
                    jsonResponse["challenge"] = "";
                    writer.write(jsonResponse);
                    httpResponse.SetData(jsonResponse.toStyledString());
                    httpResponse.SetStatus(HttpResponseStatus::OK);
                    return httpResponse;
                }
                if (jsonCommand==LOGOUT)
                {
                    removeFromMap(jsonRequest["challenge"].asString());
                    jsonResponse["error_code"] = 0;
                    jsonResponse["error_message"] = "OK";
                    jsonResponse["challenge"] = "";
                    writer.write(jsonResponse);
                    httpResponse.SetData(jsonResponse.toStyledString());
                    httpResponse.SetStatus(HttpResponseStatus::OK);
                    return httpResponse;
                }
                bool checkIfAuthorized = auth.authorize(m_usernameChallengeMap[jsonRequest["challenge"].asString()],jsonRequest["hash"].asString(),jsonRequest["challenge"].asString());
                if (!checkIfAuthorized)
                {
                    jsonResponse["error_code"] = 21;
                    jsonResponse["error_message"] = "Authorization failed.";
                    jsonResponse["challenge"] = "";
                    writer.write(jsonResponse);
                    httpResponse.SetData(jsonResponse.toStyledString());
                    httpResponse.SetStatus(HttpResponseStatus::UNAUTHORIZED);
                    return httpResponse;
                }
                std::string challenge = auth.generateChallenge();
                updateMap(jsonRequest["challenge"].asString(),challenge);

                try
                {
                    switch (jsonCommand)
                    {
                        case GET_ALL_RULES:
                            jsonResponse["data"] = iptexec.getAllRules();
                            break;
                        case DELETE_RULE:
                            jsonResponse["data"] = iptexec.deleteRule( (chainType)jsonRequest["params"]["chainType"].asInt(),jsonRequest["params"]["line"].asUInt() );
                            break;
                        case BLOCK_IP:
                            jsonResponse["data"] = iptexec.blockIP( (chainType)jsonRequest["params"]["chainType"].asInt(), jsonRequest["params"]["ip"].asString() );
                            break;
                        case BLOCK_TCP_PORT:
                            jsonResponse["data"] = iptexec.blockTCP((chainType)jsonRequest["params"]["chainType"].asInt(), jsonRequest["params"]["tcpPort"].asUInt() );
                            break;
                        case BLOCK_UDP_PORT:
                            jsonResponse["data"] = iptexec.blockUDP((chainType)jsonRequest["params"]["chainType"].asInt(), jsonRequest["params"]["udpPort"].asUInt() );
                            break;
                        case BLOCK_INCOMING_MAC:
                            jsonResponse["data"] = iptexec.blockMAC( jsonRequest["params"]["mac"].asString());
                            break;
                        case RAW:
                            jsonResponse["data"] = iptexec.rawCommand( jsonRequest["params"]["raw"].asString());
                            break;
                    }
                }
                catch ( const exception::iptables::invalid_command &e )
                {
                    LOG_ERR( "Error: invalid command" );
                }
                catch ( const exception::iptables::exec_error &e )
                {
                    LOG_ERR( "Error executing command" );
                }

                jsonResponse["error_code"] = 0;
                jsonResponse["error_message"] = "OK";
                jsonResponse["challenge"] = challenge ;
                writer.write(jsonResponse);
                httpResponse.SetData(jsonResponse.toStyledString());
                httpResponse.SetStatus(HttpResponseStatus::OK);
                return httpResponse;
}



void Handler::updateMap(std::string oldchallenge, std::string newchallenge)
{
    std::map<std::string,std::string>::iterator it = m_usernameChallengeMap.find(oldchallenge);
    if (it != m_usernameChallengeMap.end())
    {
        std::string username = it->second;
        m_usernameChallengeMap.erase(it);
        m_usernameChallengeMap.insert(std::pair<std::string, std::string>(newchallenge, username));
    }
}

bool Handler::checkIfChallengeInMap(std::string challenge)
{
    std::map<std::string,std::string>::iterator it = m_usernameChallengeMap.find(challenge);
    if (it != m_usernameChallengeMap.end())
    {
        return true;
    }
    return false;
}


void Handler::insertToMap(std::string challenge, std::string username)
{
    m_usernameChallengeMap.insert(std::pair<std::string,std::string>(challenge,username));
}

void Handler::removeFromMap(std::string challenge)
{
    std::map<std::string,std::string>::iterator it = m_usernameChallengeMap.find(challenge);
    if (it != m_usernameChallengeMap.end())
        m_usernameChallengeMap.erase(it);
}