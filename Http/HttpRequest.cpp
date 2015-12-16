//
// Created by anowikowski on 10.12.15.
//

#include "HttpRequest.h"
#include "../Extensions/StringExtensions.h"
#include "../Exception/HttpException.h"

#include <vector>

const std::string HttpRequest::m_sNewLineString = "\r\n";
const std::string HttpRequest::m_sHttpHeaderNameValueDelimiter = ": ";
const std::string HttpRequest::m_sContentLengthHeaderName = "Content-Length";
const std::string HttpRequest::m_sHostHeaderName = "Host";

void HttpRequest::SetRawHttpRequest(std::string httpRequestString)
{
    this->Clear();

    std::size_t httpPrologueEndIndex = httpRequestString.find(this->m_sNewLineString);
    if (httpPrologueEndIndex == std::string::npos)
    {
        throw exception::http::invalid_http_request();
    }

    std::string httpPrologueLine = httpRequestString.substr(0, httpPrologueEndIndex);
    std::vector<std::string> httpPrologueTokens = string_extensions::split(httpPrologueLine);

    if (httpPrologueTokens.size() != 3)
    {
        throw exception::http::invalid_http_request();
    }

    std::string method = httpPrologueTokens[0];
    std::string path = httpPrologueTokens[1];
    std::string httpVersion = httpPrologueTokens[2];

    try
    {
        this->SetHttpMethod(method);
    }
    catch (const exception::http::invalid_http_method &e)
    {
        throw exception::http::invalid_http_request();
    }

    this->m_sPath = path;
    this->m_sHttpVersion = httpVersion;

    std::size_t currentIndex = httpPrologueEndIndex + this->m_sNewLineString.length();
    while(true)
    {
        std::size_t newLineIndex = httpRequestString.find(this->m_sNewLineString, currentIndex);
        if (newLineIndex == std::string::npos || newLineIndex == 0)
        {
            break;
        }

        std::string httpHeaderString = httpRequestString.substr(currentIndex, newLineIndex - currentIndex);
        if (!httpHeaderString.empty())
        {
            try
            {
                HttpHeader httpHeader(httpHeaderString);
                this->AddHeader(httpHeader);
            }
            catch (const exception::http::invalid_http_header &e)
            {
                throw exception::http::invalid_http_request();
            }
        }

        currentIndex = newLineIndex + this->m_sNewLineString.length();
    }

    if (!this->IsHeaderPresent(this->m_sHostHeaderName))
    {
        throw exception::http::invalid_http_request();
    }

    this->m_sData = httpRequestString.substr(currentIndex);
}

void HttpRequest::Clear()
{
    this->m_httpHeaderCollection.ClearCollection();
    this->m_sData = "";
}

unsigned int HttpRequest::GetContentLength()
{
    std::string contentLengthHeaderValue = this->GetHeader(this->m_sContentLengthHeaderName).GetHeaderValue();
    return string_extensions::stoui(contentLengthHeaderValue);
}

void HttpRequest::SetData(std::string data)
{
    this->m_sData = data;
}

void HttpRequest::SetHttpMethod(HttpRequestMethod method)
{
    this->m_method = method;
}

void HttpRequest::SetHttpMethod(const std::string &method)
{
    this->SetHttpMethod(this->MapHttpRequestMethod(method));
}

HttpRequestMethod HttpRequest::MapHttpRequestMethod(const std::string &method)
{
    if (method == "GET")
    {
        return HttpRequestMethod::GET;
    }
    else if (method == "POST")
    {
        return HttpRequestMethod::POST;
    }
    else if (method == "OPTIONS")
    {
        return HttpRequestMethod::OPTIONS;
    }
    else if (method == "HEAD")
    {
        return HttpRequestMethod::HEAD;
    }
    else if (method == "PUT")
    {
        return HttpRequestMethod::PUT;
    }
    else if (method == "DELETE")
    {
        return HttpRequestMethod::DELETE;
    }
    else if (method == "TRACE")
    {
        return HttpRequestMethod::TRACE;
    }
    else if (method == "CONNECT")
    {
        return HttpRequestMethod::CONNECT;
    }
    else if (method == "LINK")
    {
        return HttpRequestMethod::LINK;
    }
    else if (method == "UNLINK")
    {
        return HttpRequestMethod::UNLINK;
    }

    throw exception::http::invalid_http_method();
}

std::string HttpRequest::GetData()
{
    return this->m_sData;
}

HttpRequestMethod HttpRequest::GetMethod()
{
    return this->m_method;
}

HttpHeader HttpRequest::GetHeader(std::string sHeaderName)
{
    return this->m_httpHeaderCollection.GetHeader(sHeaderName);
}

void HttpRequest::AddHeader(HttpHeader httpHeader)
{
    this->m_httpHeaderCollection.AddHeader(httpHeader);
}

bool HttpRequest::IsHeaderPresent(const std::string &sHeaderName)
{
    return this->m_httpHeaderCollection.IsHeaderPresent(sHeaderName);
}
