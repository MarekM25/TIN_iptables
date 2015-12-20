//
// Created by mmoraczynski on 20.12.15.
//

bool Validator::validate(Json::Value jsonRequest)
{
    if (jsonRequest["command"].IsNull() ||
            jsonRequest["hash"].IsNull() ||
            jsonRequest["challenge"].IsNull ||
            jsonRequest["params"].IsNUll)
        return false;
    if (!jsonRequest["command"].IsInt() ||
            jsonRequest["command"].asInt()<0 ||
            jsonRequest["command"].asInt()> std::numeric_limits<enum commandType>::max())
        return false;
    int command = jsonRequest["command"].IsInt();
    switch (command)
    {
        case LOGIN_INIT:
            if (jsonRequest["params"]["username"].isNull())
                return false;
            break;
        case LOGOUT:

            break;
        case GET_ALL_RULES:
            iptexec.getAllRules();
            break;
        case DELETE_RULE:
            if (jsonRequest["params"]["chainType"].isNull() ||
                    jsonRequest["params"]["line"].isNull())
                return false;
            iptexec.deleteRule( (chainType)jsonRequest["chainType"].asInt(),jsonRequest["line"].asUInt() );
            break;
        case BLOCK_IP:
            iptexec.blockIP( (chainType)jsonRequest["chainType"].asInt(), jsonRequest["ip"].asString() );
            break;
        case BLOCK_TCP_PORT:
            //iptexec.blockTCP( chainType chain, unsigned short tcpPort );
            break;
        case BLOCK_UDP_PORT:
            //iptexec.blockUDP( chainType chain, unsigned short udpPort );
            break;
        case BLOCK_INCOMING_MAC:
            //iptexec.blockMAC( std::string macAddress );
            break;
        case RAW:
            //iptexec.rawCommand( std::string cmd );
            break;

    }
}
