//
// Created by anowikowski on 14.12.15.
//

#include "HttpResponse.h"

HttpResponse::HttpResponse()
{
    this->data = "OK";
}

HttpResponse::~HttpResponse()
{

}

std::string HttpResponse::getData()
{
    return this->data;
}

std::string HttpResponse::setData(const std::string &data)
{
    this->data = data;
}
