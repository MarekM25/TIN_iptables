//
// Created by anowikowski on 14.12.15.
//

#include "HttpResponse.h"

HttpResponse::HttpResponse()
{
    this->m_sData = "OK";
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
