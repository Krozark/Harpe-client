///data base
#include <Socket/client/Client.hpp>
#include <Socket/Config.hpp>
#include <Socket/FuncWrapper.hpp>
#include <Socket/SocketSerialized.hpp>

#define SERVER_HOST 1
#define SERVER_PORT 2


namespace ntw
{
    int dispatch(int id,SocketSerialized& request){}
}

void clientWaitForWork(ntw::SocketSerialized& sock)
{
    ntw::FuncWrapper::cli::send(sock,3);

}

int main(int argc,char* argv[])
{
    if(argc < SERVER_HOST)
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

    ntw::cli::Client client;
    client.connect(argv[SERVER_HOST],ntw::Config::port_server);

    client.call(clientWaitForWork);

    return 0;
}
