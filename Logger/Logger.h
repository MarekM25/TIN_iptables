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

#define LOG  Logger<FileLogPolicy>::getInstance().print< severity_type::INFO >
#define LOG_ERR Logger<FileLogPolicy>::getInstance().print< severity_type::ERROR >
#define LOG_ACS Logger<FileLogPolicy>::getInstance().print< severity_type::ACCESS >

#endif //TIN_IPTABLES_LOGGER_H
