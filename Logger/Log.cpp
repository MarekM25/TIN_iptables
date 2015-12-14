/*
 * @file Log.cpp
 * Source file for Logger module classes.
 *
 * @brief
 * This file contains FileLogPolicy class implementation.
 *
 * @author Pawel Andruszkiewicz
 *
 * @version 0.3
 */

#include "Log.h"

void FileLogPolicy::open_ostream(const std::string& error_name, const std::string& access_name,const std::string& info_name)
{
    error_out_stream->open(error_name.c_str(), std::ios_base::binary | std::ios_base::out | std::ios_base::app);
    if(!error_out_stream->is_open())
    {
        throw(std::runtime_error("LOGGER: Unable to open an output stream"));
    }

    access_out_stream->open(access_name.c_str(), std::ios_base::binary | std::ios_base::out | std::ios_base::app);
    if(!access_out_stream->is_open())
    {
        throw(std::runtime_error("LOGGER: Unable to open an output stream"));
    }

    info_out_stream->open(info_name.c_str(), std::ios_base::binary | std::ios_base::out | std::ios_base::app);
    if(!info_out_stream->is_open())
    {
        throw(std::runtime_error("LOGGER: Unable to open an output stream"));
    }
}

void FileLogPolicy::close_ostream()
{
    if(error_out_stream)
    {
        error_out_stream->close();
    }

    if(access_out_stream)
    {
        access_out_stream->close();
    }

    if(info_out_stream)
    {
        info_out_stream->close();
    }
}

void FileLogPolicy::write_error(const std::string &msg)
{
    (*error_out_stream)<<msg<<std::endl;
}

void FileLogPolicy::write_access(const std::string &msg)
{
    (*access_out_stream)<<msg<<std::endl;
}

void FileLogPolicy::write_info(const std::string &msg)
{
    (*info_out_stream)<<msg<<std::endl;
}


FileLogPolicy::~FileLogPolicy()
{
    close_ostream();
}

