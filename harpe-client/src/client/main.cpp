#include <client/functions.hpp>
///data base
#include <Socket/Config.hpp>
#include <Socket/client/Client.hpp>


#define SERVER_HOST 1
#define SERVER_PORT 2

int main(int argc,char* argv[])
{
    if(argc < SERVER_HOST+1)
    {
        std::cout<<"Usage are: "<<argv[0]<<" <server-host> [server-port]"<<std::endl;
        return 1;
    }
    //// inti config
    if (argc >= SERVER_PORT+1)
        ntw::Config::port_server = atoi(argv[SERVER_PORT]);
    else
        ntw::Config::port_server = 3987;

    ntw::Config::default_timeout = 5.f;
    ntw::Config::broadcast = false;

    if(not ini_context("./sort.so"))
        return 0;

    ntw::cli::Client client;
    client.connect(argv[SERVER_HOST],ntw::Config::port_server);


    run(client);
    clean_context();
    
    return 0;
}
