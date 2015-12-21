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
        jsonResponse["error_code"] = responseCode::RESPONSE_CODE_NOT_JSON_FORMAT;
        jsonResponse["error_message"] = "Request was not in JSON format";
        jsonResponse["challenge"] = "";
        httpResponse.SetData(jsonResponse.toStyledString());
        httpResponse.SetStatus(HttpResponseStatus::BAD_REQUEST);
        return httpResponse;
    }
    Configuration &config= Configuration::getInstance();

    if (config.isIPAddressBlocked(httpRequestContext.GetClientIpAddress()))
    {
        jsonResponse["error_code"] = responseCode::RESPONSE_CODE_UNAUTHORIZED;
        jsonResponse["error_message"] = "You are not authorized to use this server.";
        jsonResponse["challenge"] = "";
        httpResponse.SetData(jsonResponse.toStyledString());
        httpResponse.SetStatus(HttpResponseStatus::UNAUTHORIZED);
        return httpResponse;
    }

    IPTablesExecutor iptexec;
    Validator validator;
    if (!validator.validate(jsonRequest))
    {
        jsonResponse["error_code"] = responseCode::RESPONSE_CODE_INVALID_JSON_REQUEST_FORMAT;
        jsonResponse["error_message"] = "Request or parameters was not in valid format";
        jsonResponse["challenge"] = "";
        writer.write(jsonResponse);
        httpResponse.SetData(jsonResponse.toStyledString());
        httpResponse.SetStatus(HttpResponseStatus::OK);
        return httpResponse;
    }

    int jsonCommand = jsonRequest["command"].asInt();
    if (jsonCommand == LOGIN_INIT)
    {
        jsonResponse = this->m_auth.loginInit(jsonRequest["params"]["username"].asString());
        if (jsonResponse["error_code"].asInt() == responseCode::RESPONSE_CODE_OK) {
            this->m_auth.insertToMap(jsonResponse["challenge"].asString(), jsonRequest["params"]["username"].asString());
        }
        writer.write(jsonResponse);
        httpResponse.SetData(jsonResponse.toStyledString());
        httpResponse.SetStatus(HttpResponseStatus::OK);
        return httpResponse;
    }
    if (!this->m_auth.checkIfChallengeInMap(jsonRequest["challenge"].asString()))
    {
        jsonResponse["error_code"] = responseCode::RESPONSE_CODE_UNAUTHORIZED;
        jsonResponse["error_message"] = "Authorization failed.";
        jsonResponse["challenge"] = "";
        writer.write(jsonResponse);
        httpResponse.SetData(jsonResponse.toStyledString());
        httpResponse.SetStatus(HttpResponseStatus::UNAUTHORIZED);
        return httpResponse;
    }
    if (jsonCommand==LOGOUT)
    {
        this->m_auth.removeFromMap(jsonRequest["challenge"].asString());
        jsonResponse["error_code"] = responseCode::RESPONSE_CODE_OK;
        jsonResponse["error_message"] = "OK";
        jsonResponse["challenge"] = "";
        writer.write(jsonResponse);
        httpResponse.SetData(jsonResponse.toStyledString());
        httpResponse.SetStatus(HttpResponseStatus::OK);
        return httpResponse;
    }

    bool checkIfAuthorized = this->m_auth.authorize(jsonRequest["challenge"].asString(), jsonRequest["hash"].asString());
    if (!checkIfAuthorized)
    {
        jsonResponse["error_code"] = responseCode::RESPONSE_CODE_UNAUTHORIZED;
        jsonResponse["error_message"] = "Authorization failed.";
        jsonResponse["challenge"] = "";
        writer.write(jsonResponse);
        httpResponse.SetData(jsonResponse.toStyledString());
        httpResponse.SetStatus(HttpResponseStatus::UNAUTHORIZED);
        return httpResponse;
    }
    std::string challenge = this->m_auth.generateChallenge();
    this->m_auth.updateMap(jsonRequest["challenge"].asString(),challenge);

    try
    {
        switch (jsonCommand)
        {
            case GET_ALL_RULES:
                jsonResponse["data"] = iptexec.getAllRules();
                break;
            case DELETE_RULE:
                iptexec.deleteRule( (chainType)jsonRequest["params"]["chainType"].asInt(),(unsigned short)jsonRequest["params"]["line"].asUInt() );
                break;
            case BLOCK_IP:
                iptexec.blockIP( (chainType)jsonRequest["params"]["chainType"].asInt(), jsonRequest["params"]["ip"].asString() );
                break;
            case BLOCK_TCP_PORT:
                iptexec.blockTCP((chainType)jsonRequest["params"]["chainType"].asInt(), (unsigned short)jsonRequest["params"]["tcpPort"].asUInt() );
                break;
            case BLOCK_UDP_PORT:
                iptexec.blockUDP((chainType)jsonRequest["params"]["chainType"].asInt(), (unsigned short)jsonRequest["params"]["udpPort"].asUInt() );
                break;
            case BLOCK_INCOMING_MAC:
                iptexec.blockMAC( jsonRequest["params"]["mac"].asString());
                break;
            case RAW:
                iptexec.rawCommand( jsonRequest["params"]["raw"].asString());
                break;
        }
    }
    catch ( const exception::iptables::invalid_command &e )
    {
        LOG_ERR( "Error: invalid command" );
        jsonResponse["error_code"] = responseCode::RESPONSE_CODE_INVALID_COMMAND;
        jsonResponse["error_message"] = "Error: invalid command.";
        jsonResponse["challenge"] = "";
        writer.write(jsonResponse);
        httpResponse.SetData(jsonResponse.toStyledString());
        httpResponse.SetStatus(HttpResponseStatus::BAD_REQUEST);
        return httpResponse;
    }
    catch ( const exception::iptables::exec_error &e )
    {
        LOG_ERR( "Error executing command" );
        jsonResponse["error_code"] = responseCode::RESPONSE_CODE_INTERNAL_ERROR;
        jsonResponse["error_message"] = "Error: executing command.";
        jsonResponse["challenge"] = "";
        writer.write(jsonResponse);
        httpResponse.SetData(jsonResponse.toStyledString());
        httpResponse.SetStatus(HttpResponseStatus::INTERNAL_SERVER_ERROR);
        return httpResponse;
    }

    jsonResponse["error_code"] = responseCode::RESPONSE_CODE_OK;
    jsonResponse["error_message"] = "OK";
    jsonResponse["challenge"] = challenge ;
    writer.write(jsonResponse);
    httpResponse.SetData(jsonResponse.toStyledString());
    httpResponse.SetStatus(HttpResponseStatus::OK);
    return httpResponse;
}
