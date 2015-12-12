/*
 * @file Log.h
 * Header file for Logger module classes.
 *
 * @brief
 * Generic class Logger is specified by the class that implements LogPolicyInterface.
 * LogPolicyInterface define logs-writing behavior.
 * This file contains FileLogPolicy class definition.
 *
 * @author Pawel Andruszkiewicz
 *
 * @version 0.3
 */

#ifndef TIN_IPTABLES_LOG_H
#define TIN_IPTABLES_LOG_H

#include <memory>
#include <string>
#include <string.h>
#include <fstream>
#include <mutex>
#include <sstream>

enum severity_type
{
    ERROR,
    ACCESS,
    INFO
};

class LogPolicyInterface
{
public:
    virtual void open_ostream(const std::string& error_name, const std::string& access_name,const std::string& info_name) = 0;
    virtual void close_ostream() = 0;
    virtual void write_error(const std::string& msg) = 0;
    virtual void write_access(const std::string& msg) = 0;
    virtual void write_info(const std::string& msg) = 0;
    virtual ~LogPolicyInterface() = 0;
};

inline LogPolicyInterface::~LogPolicyInterface() { }

class FileLogPolicy : LogPolicyInterface
{
    std::unique_ptr<std::ofstream> error_out_stream;  //TODO: is unique_ptr needed?
    std::unique_ptr<std::ofstream> access_out_stream;
    std::unique_ptr<std::ofstream> info_out_stream;
public:
    FileLogPolicy() : error_out_stream(new std::ofstream), access_out_stream(new std::ofstream), info_out_stream(new std::ofstream) {}
    ~FileLogPolicy();

    void open_ostream(const std::string& error_name, const std::string& access_name,const std::string& info_name);
    void close_ostream();

    void write_error(const std::string& msg);
    void write_access(const std::string& msg);
    void write_info(const std::string& msg);

};

template<typename log_policy>
class Logger
{
    log_policy policy;

    std::mutex write_error_mutex;
    std::mutex write_access_mutex;
    std::mutex write_info_mutex;

    void print_impl(severity_type severity, std::stringstream&&);

    template<typename First, typename...Rest>
    void print_impl(severity_type severity, std::stringstream&&, First&& param1, Rest&&...param);

public:
    Logger();

    template<severity_type severity, typename...Args>
    void print(Args...args);

    ~Logger();
};

template<typename log_policy>
Logger<log_policy>::Logger()
{
    policy.open_ostream("error_log.txt", "access_log.txt", "info_log.txt");
}

template<typename log_policy>
Logger<log_policy>::~Logger()
{
    policy.close_ostream();
}

template<typename log_policy>
template<severity_type severity, typename...Args>
void Logger<log_policy>::print(Args...args)
{

    std::stringstream log_stream;

    auto cur_time = std::chrono::high_resolution_clock::now();
    std::time_t tt = std::chrono::high_resolution_clock::to_time_t(cur_time);
    char* tt_s = ctime(&tt);
    tt_s[strlen(tt_s)-1]=0;

    log_stream<<tt_s<<" - ";

    print_impl(severity, std::forward<std::stringstream>(log_stream), std::move(args)...);
};

template<typename log_policy>
void Logger<log_policy>::print_impl(severity_type severity, std::stringstream&& log_stream)
{
    switch (severity) {
        case severity_type::ERROR: {
            std::lock_guard<std::mutex> lock_write(write_error_mutex);
            policy.write_error(log_stream.str());
            break;
        }
        case severity_type::ACCESS:
        {
            std::lock_guard<std::mutex> lock_access(write_access_mutex);
            policy.write_access(log_stream.str());
            break;
        }
        case severity_type::INFO:
        {
            std::lock_guard<std::mutex> lock_info(write_info_mutex);
            policy.write_info(log_stream.str());
            break;
        }
    }
}


template<typename log_policy>
    template<typename First, typename...Rest>
void Logger<log_policy>::print_impl(severity_type severity, std::stringstream &&log_stream, First &&param1, Rest&&...param)
{
    log_stream<<param1;
    print_impl(severity, std::forward<std::stringstream>(log_stream), std::move(param)...);
}

#endif //TIN_IPTABLES_LOG_H
