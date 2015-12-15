#include <iostream>
#include <unistd.h>

#include "Configuration/Configuration.h"
#include "Http/HttpServer.h"
#include "IPTables/IPTablesExecutor.h"

#include "Logger/Logger.h"
#include "Authorization/auth.h"

using namespace std;

HttpResponse httpRequestHandler(HttpRequest httpRequest)
{
    HttpResponse httpResponse;
    httpResponse.SetData(httpRequest.GetData());
    return httpResponse;
}

int main()
{
    LOG("initialized");
    Authorization auth;
    cout << auth.strToMd5("\n"
                                  "Lorem Ipsum jest tekstem stosowanym jako przykładowy wypełniacz w przemyśle poligraficznym. Został po raz pierwszy użyty w XV w. przez nieznanego drukarza do wypełnienia tekstem próbnej książki. Pięć wieków później zaczął być używany przemyśle elektronicznym, pozostając praktycznie niezmienionym. Spopularyzował się w latach 60. XX w. wraz z publikacją arkuszy Letrasetu, zawierających fragmenty Lorem Ipsum, a ostatnio z zawierającym różne wersje Lorem Ipsum oprogramowaniem przeznaczonym do realizacji druków na komputerach osobistych, jak Aldus PageMaker\n"
                                  "\n"
                                  "Ogólnie znana teza głosi, iż użytkownika może rozpraszać zrozumiała zawartość strony, kiedy ten chce zobaczyć sam jej wygląd. Jedną z mocnych stron używania Lorem Ipsum jest to, że ma wiele różnych „kombinacji” zdań, słów i akapitów, w przeciwieństwie do zwykłego: „tekst, tekst, tekst”, sprawiającego, że wygląda to „zbyt czytelnie” po polsku. Wielu webmasterów i designerów używa Lorem Ipsum jako domyślnego modelu tekstu i wpisanie w internetowej wyszukiwarce ‘lorem ipsum’ spowoduje znalezienie bardzo wielu stron, które wciąż są w budowie. Wiele wersji tekstu ewoluowało i zmieniało się przez lata, czasem przez przypadek, czasem specjalnie (humorystyczne wstawki itd).\n"
                                  "")<<"\n";

    Configuration &configurationInstance = Configuration::getInstance();
    configurationInstance.initialize("iptables.conf");

    IPTablesExecutor iptexec;
    iptexec.executeCommand( GET_ALL_RULES );
    iptexec.executeCommand( DELETE_RULE, 1, INPUT );
    iptexec.executeCommand( DELETE_RULE, 4, OUTPUT );
    iptexec.executeCommand( BLOCK_IP, "192.168.1.101", INPUT );
    iptexec.executeCommand( BLOCK_IP, "192.168.1.104", OUTPUT );
    iptexec.executeCommand( BLOCK_TCP_PORT, 9999, OUTPUT );
    iptexec.executeCommand( BLOCK_UDP_PORT, 1234, INPUT );
    iptexec.executeCommand( BLOCK_INCOMING_MAC, "00:0F:EA:91:04:08" );
    iptexec.executeCommand( RAW, "pwd" );

    HttpServer server;
    server.SetPort(configurationInstance.getServerPort());
    server.SetListeningIpAddress(configurationInstance.getServerIpAddress());
    server.SetHttpRequestHandler(httpRequestHandler);

    LOG("Server IP Address: ", configurationInstance.getServerIpAddress(), " Server Port: ", configurationInstance.getServerPort());
    server.Start();

    cout << "Hello, World!" << endl;


    LOG_ACS("Hello World request");

    while(true) //NOT RECEIVED EXIT SIGNAL OR COMMAND
    {
        //LOG_ERR("Some error occurred");
        sleep(1);
    }

    server.Stop();

    return 0;
}