//
// Created by andrusza2 on 12/20/15.
//

#ifndef TIN_IPTABLES_LOGGEREXCEPTION_H
#define TIN_IPTABLES_LOGGEREXCEPTION_H


#include <exception>

namespace exception
{
    namespace logger
    {
        class logger_exception : public std::exception { };
        class logger_not_initialized : public logger_exception { };
        class logger_unable_to_open_output_stream : public logger_exception { };
    }
}


#endif //TIN_IPTABLES_LOGGEREXCEPTION_H
