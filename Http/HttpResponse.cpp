//
// Created by anowikowski on 14.12.15.
//

#include "HttpResponse.h"

#include "../Exception/HttpException.h"
#include "../Extensions/StringExtensions.h"

const std::string HttpResponse::m_sDefaultHttpVersion = "HTTP/1.1";

HttpResponse::HttpResponse()
{
    this->SetDefaultHttpVersion();
}

HttpResponse::~HttpResponse()
{

}

std::string HttpResponse::GetData()
{
    return this->m_sData;
}

std::string HttpResponse::SetData(const std::string &data)
{
    this->m_sData = data;
}

void HttpResponse::SetStatus(HttpResponseStatus status)
{
    this->m_status = status;
}

HttpResponseStatus HttpResponse::GetStatus()
{
    return this->m_status;
}

int HttpResponse::MapHttpResponseStatusToInt(HttpResponseStatus status)
{
    switch(status)
    {
        case HttpResponseStatus::OK_200:
            return 200;
        case HttpResponseStatus::BAD_REQUEST_400:
            return 400;
        case HttpResponseStatus::NOT_FOUND_404:
            return 404;
        case HttpResponseStatus::METHOD_NOT_ALLOWED_405:
            return 405;
        default:
            throw exception::http::unknown_http_response_status();
    }
}

void HttpResponse::SetHttpVersion(std::string httpVersion)
{
    this->m_sHttpVersion = httpVersion;
}

std::string HttpResponse::GetHttpVersion()
{
    return this->m_sHttpVersion;
}

std::string HttpResponse::MapHttpResponseStatusToString(HttpResponseStatus status)
{
    switch(status)
    {
        case HttpResponseStatus::OK_200:
            return "OK";
        case HttpResponseStatus::BAD_REQUEST_400:
            return "Bad Request";
        case HttpResponseStatus::NOT_FOUND_404:
            return "Not Found";
        case HttpResponseStatus::METHOD_NOT_ALLOWED_405:
            return "Method Not Allowed";
        default:
            throw exception::http::unknown_http_response_status();
    }
}

void HttpResponse::SetHeader(const std::string &sHeaderName, const std::string &sHeaderValue)
{
    std::string sHeaderNameLowerCase = string_extensions::tolower(sHeaderName);
    this->m_headers.insert(std::pair<std::string, std::string>(sHeaderNameLowerCase, sHeaderValue));
}

std::map<std::string, std::string>& HttpResponse::GetHeaders()
{
    return this->m_headers;
}

void HttpResponse::SetDefaultHttpVersion()
{
    this->SetHttpVersion(this->m_sDefaultHttpVersion);
}
