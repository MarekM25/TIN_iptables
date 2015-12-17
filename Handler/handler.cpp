//
// Created by mmoraczynski on 16.12.15.
//
#include "handler.h"

const std::string Handler::m_sRequiredPathHttpHeaderValue = "/iptables_mgmt";

HttpResponse Handler::HandleHttpRequest(HttpRequestContext httpRequestContext)
{
    HttpResponse httpResponse;
    HttpRequest httpRequest = httpRequestContext.GetHttpRequest();
    Configuration &config= Configuration::getInstance();

    std::cout<<"dsaf";
    fflush(stdout);
    if (httpRequest.GetMethod() != HttpRequestMethod::POST)
    {
        httpResponse.SetStatus(HttpResponseStatus::METHOD_NOT_ALLOWED_405);
        return httpResponse;
    }
    std::cout<<"przed required";
    fflush(stdout);
    if (httpRequest.GetPath() != this->m_sRequiredPathHttpHeaderValue)
    {
        httpResponse.SetStatus(HttpResponseStatus::NOT_FOUND_404);
        return httpResponse;
    }

    Json::Reader reader;
    Json::FastWriter writer;
    Json::Value jsonRequest, jsonResponse;
    std::cout<<"przed sparsowaniem";
    fflush(stdout);
    if (reader.parse(httpRequest.GetData(),jsonRequest))
    {
        std::cout<<"json sparsowany";
        fflush(stdout);

        if (config.isIPAddressBlocked(httpRequestContext.GetClientIpAddress()))
        {
            jsonResponse["error_code"] = 20;
            jsonResponse["error_message"] = "You are not authorized to use this server.";
            jsonResponse["challenge"] = "";
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
                    std::string challenge = auth.generateChallenge();

                    bool challengeWasCorrect = updateMap(jsonRequest["challenge"].asString(),challenge);
                    if (challengeWasCorrect)
                    {
                        jsonResponse["error_code"] = 0;
                        jsonResponse["error_message"] = "OK";
                        jsonResponse["challenge"] = challenge;
                    }
                    else
                    {
                        jsonResponse["error_code"] = 22;
                        jsonResponse["error_message"] = "challenge was not correct";
                        jsonResponse["challenge"] = "";
                    }
                }
                std::cout<<"przed switch";
                fflush(stdout);
                switch (jsonCommand) {
                    case LOGIN_INIT:
                        jsonResponse = auth.loginInit(jsonRequest["params"]["username"].asString());
                        writer.write(jsonResponse);
                        std::cout<<jsonResponse;
                        fflush(stdout);
                        if (jsonResponse["error_code"] == "0") {
                            std::cout <<"dodajemy do mapy";
                            insertToMap(jsonResponse["challenge"].asString(), jsonRequest["params"]["username"].asString());
                        }
                        break;
                    case LOGIN_REQUEST:
                        std::cout<<"login request";
                        std::cout<<m_usernameChallengeMap[jsonRequest["challenge"].asString()];
                        jsonResponse = auth.loginRequest(m_usernameChallengeMap[jsonRequest["challenge"].asString()],jsonRequest["hash"].asString(),jsonRequest["challenge"].asString());
                        break;
                    case LOGOUT:
                        break;
                    case GET_ALL_RULES:
                        //iptexec.getAllRules();
                        break;
                    case DELETE_RULE:
                        //iptexec.deleteRule( chainType chain, unsigned short line );
                        break;
                    case BLOCK_IP:
                        //iptexec.blockIP( chainType chain, std::string ipAddress );
                        break;
                    case BLOCK_TCP_PORT:
                        //iptexec.blockTCP( chainType chain, unsigned short tcpPort );
                        break;
                    case BLOCK_UDP_PORT:
                        //iptexec.blockUDP( chainType chain, unsigned short udpPort );
                        break;
                    case BLOCK_INCOMING_MAC:
                        //iptexec.blockMAC( std::string macAddress );
                        break;
                    case RAW:
                        //iptexec.rawCommand( std::string cmd );
                        break;

                }

            }
        }
    }
    else
    {
        jsonResponse["error_code"] = 10;
        jsonResponse["error_message"] = "Request was not in JSON format";
        jsonResponse["challenge"] = "";
    }
    writer.write(jsonResponse);

    std::cout<<jsonResponse;
    fflush(stdout);
    httpResponse.SetData(jsonResponse.toStyledString());
    httpResponse.SetStatus(HttpResponseStatus::OK_200);
    return httpResponse;
}


bool Handler::updateMap(std::string oldchallenge, std::string newchallenge)
{
    std::map<std::string,std::string>::iterator it = m_usernameChallengeMap.find(oldchallenge);
    if (it != m_usernameChallengeMap.end()) {
        m_usernameChallengeMap.erase(it);
        m_usernameChallengeMap.insert(std::pair<std::string, std::string>(newchallenge, it->second));
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