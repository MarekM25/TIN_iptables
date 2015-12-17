//
// Created by anowikowski on 16.12.15.
//

#include "HttpHeader.h"
#include "../Exception/HttpException.h"

const std::string HttpHeader::m_sHttpHeaderNameValueDelimiter = ": ";

HttpHeader::HttpHeader(std::string sHeaderName, std::string sHeaderValue)
{
    this->SetHeaderName(sHeaderName);
    this->SetHeaderValue(sHeaderValue);
}

HttpHeader::HttpHeader(std::string sRawHeader)
{
    std::size_t headerDelimiterIndex = sRawHeader.find(this->m_sHttpHeaderNameValueDelimiter);
    if (headerDelimiterIndex == std::string::npos || headerDelimiterIndex == 1)
    {
        throw exception::http::invalid_http_header();
    }

    std::string httpHeaderName = sRawHeader.substr(0, headerDelimiterIndex);
    std::string httpHeaderValue = sRawHeader.substr(headerDelimiterIndex + this->m_sHttpHeaderNameValueDelimiter.length());

    this->SetHeaderName(httpHeaderName);
    this->SetHeaderValue(httpHeaderValue);
}

std::string HttpHeader::GetHeaderName()
{
    return m_sHeaderName;
}

void HttpHeader::SetHeaderName(const std::string &sHeaderName)
{
    this->m_sHeaderName = sHeaderName;
}

std::string HttpHeader::GetHeaderValue()
{
    return m_sHeaderValue;
}

void HttpHeader::SetHeaderValue(const std::string &sHeaderValue)
{
    this->m_sHeaderValue = sHeaderValue;
}
