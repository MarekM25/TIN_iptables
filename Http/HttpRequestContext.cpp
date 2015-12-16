//
// Created by anowikowski on 16.12.15.
//

#include "HttpRequestContext.h"

HttpRequestContext::HttpRequestContext()
{

}

HttpRequestContext::~HttpRequestContext()
{

}

void HttpRequestContext::SetHttpRequest(HttpRequest httpRequest)
{
    this->m_httpRequest = httpRequest;
}

HttpRequest& HttpRequestContext::GetHttpRequest()
{
    return this->m_httpRequest;
}

void HttpRequestContext::SetClientIpAddress(std::string sourceIpAddress)
{
    this->m_sClientIpAddress = sourceIpAddress;
}

std::string HttpRequestContext::GetClientIpAddress()
{
    return this->m_sClientIpAddress;
}
