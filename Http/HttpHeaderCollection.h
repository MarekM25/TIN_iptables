//
// Created by anowikowski on 16.12.15.
//

#ifndef TIN_IPTABLES_HTTPHEADERCOLLECTION_H
#define TIN_IPTABLES_HTTPHEADERCOLLECTION_H

#include <iterator>
#include <string>
#include <vector>

#include "HttpHeader.h"

class HttpHeaderCollection : public std::iterator<std::input_iterator_tag, HttpHeader>
{
public:
    HttpHeader GetHeader(std::string sHeaderName);
    void AddHeader(HttpHeader httpHeader);
    void ClearCollection();
    bool IsHeaderPresent(const std::string &headerName);
    std::vector<HttpHeader>::iterator begin();
    std::vector<HttpHeader>::iterator end();
private:
    std::vector<HttpHeader> m_headers;
};


#endif //TIN_IPTABLES_HTTPHEADERCOLLECTION_H
