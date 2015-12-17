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

std::string HttpResponse::GetData()
{
    return this->m_sData;
}

void HttpResponse::SetData(const std::string &data)
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
        case HttpResponseStatus::OK:
            return "OK";
        case HttpResponseStatus::BAD_REQUEST:
            return "Bad Request";
        case HttpResponseStatus::UNAUTHORIZED:
            return "Unauthorized";
        case HttpResponseStatus::NOT_FOUND:
            return "Not Found";
        case HttpResponseStatus::METHOD_NOT_ALLOWED:
            return "Method Not Allowed";
        case HttpResponseStatus::INTERNAL_SERVER_ERROR:
            return "Internal Server Error";
        default:
            throw exception::http::unknown_http_response_status();
    }
}

void HttpResponse::SetDefaultHttpVersion()
{
    this->SetHttpVersion(this->m_sDefaultHttpVersion);
}

HttpHeaderCollection &HttpResponse::GetHttpHeaderCollection()
{
    return this->m_httpHeaderCollection;
}

void HttpResponse::AddHeader(HttpHeader httpHeader)
{
    this->m_httpHeaderCollection.AddHeader(httpHeader);
}
