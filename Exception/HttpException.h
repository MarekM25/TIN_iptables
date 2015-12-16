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
        class http_server_exception : std::exception { };
        class http_server_not_running : http_server_exception { };
        class http_server_already_running : http_server_exception { };
        class http_header_not_present : http_exception { };
        class http_header_already_present : http_exception { };
        class invalid_ip_address : std::exception { };
        class invalid_http_request : http_exception { };
        class invalid_http_header : invalid_http_request { };
        class invalid_http_method : invalid_http_request { };
        class unknown_http_response_status : http_exception { };
        class internal_socket_error : http_server_exception { };
    }
}


#endif //TIN_IPTABLES_HTTPEXCEPTION_H
