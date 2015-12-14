//
// Created by anowikowski on 10.12.15.
//

#include "HttpRequest.h"
#include "../Extensions/StringExtensions.h"
#include "../Exception/HttpException.h"

const std::string HttpRequest::newLineString = "\r\n";
const std::string HttpRequest::httpHeaderNameValueDelimiter = ": ";

HttpRequest::HttpRequest(std::string httpRequestString)
{
    //Parse http/1.1 method and path and set currentIndex=XXX

    size_t currentIndex = 0;
    while(true)
    {
        size_t newLineIndex = httpRequestString.find(this->newLineString, currentIndex);
        if (newLineIndex == -1 || newLineIndex == 0)
        {
            currentIndex = newLineIndex + this->newLineString.length();
            break;
        }

        std::string httpHeader = httpRequestString.substr(currentIndex, newLineIndex);
        size_t headerDelimiterIndex = httpHeader.find(this->httpHeaderNameValueDelimiter);
        if (headerDelimiterIndex > 0)
        {
            std::string httpHeaderName = httpHeader.substr(0, headerDelimiterIndex);
            std::string httpHeaderValue = httpHeader.substr(headerDelimiterIndex + this->httpHeaderNameValueDelimiter.length());
            httpHeaderName = string_extensions::tolower(httpHeaderName);

            this->headers.insert(std::pair<std::string, std::string>(httpHeaderName, httpHeaderValue));
        }

        currentIndex = newLineIndex + this->newLineString.length();
    }

    this->data = httpRequestString.substr(currentIndex);
}

HttpRequest::~HttpRequest()
{

}

unsigned int HttpRequest::getContentLength()
{
    std::string contentLengthHeaderValue = this->getHeaderValue("Content-Length");
    return string_extensions::stoui(contentLengthHeaderValue);
}

std::string HttpRequest::getHeaderValue(std::string header)
{
    std::string headerLowerCase = string_extensions::tolower(header);
    if (!this->isHeaderPresent(headerLowerCase))
    {
        throw exception::http::http_header_not_present();
    }

    return this->headers[headerLowerCase];
}

void HttpRequest::setHttpRequestData(std::string httpRequestDataString)
{
    this->data = httpRequestDataString;
}

bool HttpRequest::isHeaderPresent(const std::string &headerName)
{
    std::string headerNameLowerCase = string_extensions::tolower(headerName);
    return this->headers.count(headerNameLowerCase) > 0;
}
