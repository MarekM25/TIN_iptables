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
    virtual ~LogPolicyInterface() {}
};

class FileLogPolicy : LogPolicyInterface
{
    std::ofstream error_out_stream;
    std::ofstream access_out_stream;
    std::ofstream info_out_stream;

public:
    FileLogPolicy() {}
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
    void print_impl(severity_type severity, std::stringstream&&, const First& param1, const Rest&...param);

public:
    static Logger& getInstance()
    {
        static Logger instance;

        return instance;
    }

private:
    Logger();
    ~Logger();

    bool write_error = false;
    bool write_access = false;
    bool write_info = false;

public:
    Logger(Logger const&) = delete;
    void operator=(Logger const&) = delete;

    void enableErrorLogging();
    void enableAccessLogging();
    void enableInfoLogging();
    void disableErrorLogging();
    void disableAccessLogging();
    void disableInfoLogging();

    template<severity_type severity, typename...Args>
    void print(const Args...args);


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
void Logger<log_policy>::print(const Args...args)
{

    if (severity == severity_type::ERROR && !write_error)
    {
        return;
    }
    if (severity == severity_type::ACCESS && !write_access)
    {
        return;
    }
    if (severity == severity_type::INFO && !write_info)
    {
        return;
    }

    std::stringstream log_stream;

#ifdef __APPLE__
    auto cur_time = std::chrono::system_clock::now();
    std::time_t tt = std::chrono::system_clock::to_time_t(cur_time);
#else
    auto cur_time = std::chrono::high_resolution_clock::now();
    std::time_t tt = std::chrono::high_resolution_clock::to_time_t(cur_time);
#endif
    char buf[256];
    char* tt_s = ctime_r(&tt, buf);
    tt_s[strlen(tt_s)-1]=0;

    log_stream<<tt_s<<" - ";

    print_impl(severity, std::forward<std::stringstream>(log_stream), args...);
}

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
void Logger<log_policy>::print_impl(severity_type severity, std::stringstream &&log_stream, const First &param1, const Rest&...param)
{
    log_stream<<param1;
    print_impl(severity, std::forward<std::stringstream>(log_stream), param...);
}

template<typename log_policy>
void Logger<log_policy>::enableErrorLogging()
{
    write_error = true;
}

template<typename log_policy>
void Logger<log_policy>::enableAccessLogging()
{
    write_access = true;
}

template<typename log_policy>
void Logger<log_policy>::enableInfoLogging()
{
    write_info = true;
}

template<typename log_policy>
void Logger<log_policy>::disableErrorLogging()
{
    write_error = false;
}

template<typename log_policy>
void Logger<log_policy>::disableAccessLogging()
{
    write_access = false;
}

template<typename log_policy>
void Logger<log_policy>::disableInfoLogging()
{
    write_info = false;
}

#endif //TIN_IPTABLES_LOG_H
