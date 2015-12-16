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

        std::string httpHeader = httpRequestString.substr(currentIndex, newLineIndex - currentIndex);
        if (!httpHeader.empty())
        {
            std::pair<std::string, std::string> httpHeaderNameValue;
            try
            {
                httpHeaderNameValue = this->SplitHttpHeaderNameValue(httpHeader);
            }
            catch (const exception::http::invalid_http_header &e)
            {
                throw exception::http::invalid_http_request();
            }

            this->m_headers.insert(std::pair<std::string, std::string>(httpHeaderNameValue.first, httpHeaderNameValue.second));
        }

        currentIndex = newLineIndex + this->m_sNewLineString.length();
    }

    this->m_sData = httpRequestString.substr(currentIndex);
}

void HttpRequest::Clear()
{
    this->m_headers = std::map<std::string, std::string>();
    this->m_sData = "";
}

HttpRequest::HttpRequest()
{

}

HttpRequest::~HttpRequest()
{

}

unsigned int HttpRequest::GetContentLength()
{
    std::string contentLengthHeaderValue = this->GetHeaderValue(this->m_sContentLengthHeaderName);
    return string_extensions::stoui(contentLengthHeaderValue);
}

std::string HttpRequest::GetHeaderValue(std::string header)
{
    std::string headerLowerCase = string_extensions::tolower(header);
    if (!this->IsHeaderPresent(headerLowerCase))
    {
        throw exception::http::http_header_not_present();
    }

    return this->m_headers[headerLowerCase];
}

void HttpRequest::SetData(std::string data)
{
    this->m_sData = data;
}

bool HttpRequest::IsHeaderPresent(const std::string &headerName)
{
    std::string headerNameLowerCase = string_extensions::tolower(headerName);
    return this->m_headers.count(headerNameLowerCase) > 0;
}

std::pair<std::string, std::string> HttpRequest::SplitHttpHeaderNameValue(const std::string &httpHeader)
{
    std::size_t headerDelimiterIndex = httpHeader.find(this->m_sHttpHeaderNameValueDelimiter);
    if (headerDelimiterIndex == std::string::npos || headerDelimiterIndex == 1)
    {
        throw exception::http::invalid_http_header();
    }

    std::string httpHeaderName = httpHeader.substr(0, headerDelimiterIndex);
    std::string httpHeaderValue = httpHeader.substr(headerDelimiterIndex + this->m_sHttpHeaderNameValueDelimiter.length());
    httpHeaderName = string_extensions::tolower(httpHeaderName);

    return std::pair<std::string, std::string>(httpHeaderName, httpHeaderValue);
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
