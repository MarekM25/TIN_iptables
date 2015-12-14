//
// Created by anowikowski on 10.12.15.
//
#define LOGGING_LEVEL_1

#include "IPTablesExecutor.h"
#include "Logger/Logger.h"

IPTablesExecutor::IPTablesExecutor()
{

}

std::string IPTablesExecutor::exec( const char* cmd )
{
    std::shared_ptr< FILE > pipe( popen( cmd, "r" ), pclose );

    if ( !pipe )
    {
        LOG_ERR( "Error executing command: ", cmd );
        return "ERROR";
    }

    LOG( "Executed command: ", cmd );

    char buffer[ 128 ];
    std::string result = "";

    while ( !feof( pipe.get() ) )
    {
        if ( fgets( buffer, 128, pipe.get() ) )
            result += buffer;
    }

    return result;
}