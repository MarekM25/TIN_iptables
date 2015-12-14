//
// Created by anowikowski on 10.12.15.
//

#include "IPTablesExecutor.h"

IPTablesExecutor::IPTablesExecutor()
{

}

std::string IPTablesExecutor::exec( const char* cmd )
{
    std::shared_ptr< FILE > pipe( popen( cmd, "r" ), pclose );

    if ( !pipe )
        return "ERROR";

    char buffer[ 128 ];
    std::string result = "";

    while ( !feof( pipe.get() ) )
    {
        if ( fgets( buffer, 128, pipe.get() ) )
            result += buffer;
    }

    return result;
}