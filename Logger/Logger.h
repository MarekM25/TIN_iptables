/*
 * @file Logger.h
 * Header file which should be included in order to use Logger module.
 *
 * @brief
 * In this file there is declaration of extern Logger object and.
 *
 *
 *
 * @author Pawel Andruszkiewicz
 *
 * @version 0.3
 */

#ifndef TIN_IPTABLES_LOGGER_H
#define TIN_IPTABLES_LOGGER_H

#include "Log.h"

extern Logger<FileLogPolicy> log_inst;


#ifdef LOGGING_LEVEL_1
#define LOG log_inst.print< severity_type::INFO >
#define LOG_ERR log_inst.print< severity_type::ERROR >
#define LOG_ACS log_inst.print< severity_type::ACCESS >
#else
#define LOG(...)
#define LOG_ERR(...)
#define LOG_ACS(...)
#endif

#ifdef LOGGING_LEVEL_2
#define ELOG log_inst.print< severity_type::INFO >
#define ELOG_ERR log_inst.print< severity_type::ERROR >
#define ELOG_ACS log_inst.print< severity_type::ACCESS >
#else
#define ELOG(...)
#define ELOG_ERR(...)
#define ELOG_ACS(...)
#endif


#endif //TIN_IPTABLES_LOGGER_H
