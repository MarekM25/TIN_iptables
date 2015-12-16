//
// Created by anowikowski on 16.12.15.
//

#include "HttpHeaderCollection.h"
#include "../Extensions/StringExtensions.h"
#include "../Exception/HttpException.h"


HttpHeader HttpHeaderCollection::GetHeader(std::string sHeaderName)
{
    std::string sHeaderLowerCase = string_extensions::tolower(sHeaderName);

    for (std::vector<HttpHeader>::iterator it = this->m_headers.begin(); it != this->m_headers.end(); it++ )
    {
        std::string sCurrentHeaderNameLowerCase = string_extensions::tolower(it->GetHeaderName());
        if (sHeaderLowerCase == sCurrentHeaderNameLowerCase)
        {
            return *it;
        }
    }

    throw exception::http::http_header_not_present();
}

void HttpHeaderCollection::AddHeader(HttpHeader httpHeader)
{
    if (this->IsHeaderPresent(httpHeader.GetHeaderName()))
    {
        throw exception::http::http_header_already_present();
    }

    this->m_headers.push_back(httpHeader);
}

void HttpHeaderCollection::ClearCollection()
{
    this->m_headers = std::vector<HttpHeader>();
}

bool HttpHeaderCollection::IsHeaderPresent(const std::string &headerName)
{
    std::string sHeaderLowerCase = string_extensions::tolower(headerName);

    for (std::vector<HttpHeader>::iterator it = this->m_headers.begin(); it != this->m_headers.end(); it++ )
    {
        std::string sCurrentHeaderNameLowerCase = string_extensions::tolower(it->GetHeaderName());
        if (sHeaderLowerCase == sCurrentHeaderNameLowerCase)
        {
            return true;
        }
    }

    return false;
}

std::vector<HttpHeader>::iterator HttpHeaderCollection::begin()
{
    return this->m_headers.begin();
}

std::vector<HttpHeader>::iterator HttpHeaderCollection::end()
{
    return this->m_headers.end();
}
