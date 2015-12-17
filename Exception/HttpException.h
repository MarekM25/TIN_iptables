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
        class http_exception : public std::exception { };
        class http_server_exception : public std::exception { };
        class http_server_not_running : public http_server_exception { };
        class http_server_already_running : public http_server_exception { };
        class http_header_not_present : public http_exception { };
        class http_header_already_present : public http_exception { };
        class invalid_ip_address : public std::exception { };
        class invalid_http_request : public http_exception { };
        class invalid_http_header : public invalid_http_request { };
        class invalid_http_method : public invalid_http_request { };
        class unknown_http_response_status : public http_exception { };
        class internal_socket_error : public http_server_exception { };
    }
}


#endif //TIN_IPTABLES_HTTPEXCEPTION_H
