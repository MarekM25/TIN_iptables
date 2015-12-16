//
// Created by anowikowski on 16.12.15.
//

#include "HttpHeader.h"

HttpHeader::HttpHeader(std::string sHeaderName, std::string sHeaderValue)
{
    this->SetHeaderName(sHeaderName);
    this->SetHeaderValue(sHeaderValue);
}

HttpHeader::HttpHeader(std::string sRawHeader)
{

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
