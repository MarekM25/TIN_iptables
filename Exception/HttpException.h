//
// Created by anowikowski on 14.12.15.
//

#include <exception>

#ifndef TIN_IPTABLES_HTTPEXCEPTION_H
#define TIN_IPTABLES_HTTPEXCEPTION_H

namespace exception
{
    namespace http
    {
        class http_exception : std::exception { };
        class http_header_not_present : http_exception { };
    }
}


#endif //TIN_IPTABLES_HTTPEXCEPTION_H
