#include <harpe-client/defines.hpp>
#include <harpe-client/functions.hpp>
///data base
#include <Socket/Config.hpp>
#include <Socket/client/Client.hpp>


#define WEBSITE_HOST 1
#define WEBSITE_PORT 2

int register_to_website(char host[],int port)
{
    int status = 0;

    std::cout<<"[Loggin to website] "<<host<<":"<<port<<std::endl;
    ntw::Socket website_sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP);
    website_sock.connect(host,port);

    std::string msg;
    msg += "GET /get/ ";
    msg += "HTTP/1.1\r\nHOST: ";
    msg += host;
    msg += "\r\n\r\n";

    char buffer[1024];
    website_sock.send(msg.c_str(),msg.size());

    int recv;
    float version = 0;
    std::string page;
    while((recv = website_sock.receive(buffer,1024))>0)
    {
        page.append(buffer,recv);
    }
    std::cout<<page<<std::endl<<std::flush;

    return status;
}


int main(int argc,char* argv[])
{
    std::cout<<"===\nHarpe client\nversion:"<<MAJOR_VERSION<<"."<<MINOR_VERSION<<"."<<PATCH_VERSION<<"\n===\n"<<std::endl;

    if(argc < WEBSITE_PORT+1)
    {
        std::cout<<"Usage are: "<<argv[0]<<" <website-host> <website-port>"<<std::endl;
        return 1;
    }
    
    register_to_website(argv[WEBSITE_HOST],atoi(argv[WEBSITE_PORT]));

    //// inti config
    ntw::Config::port_server = atoi(argv[WEBSITE_PORT]);

    if(not ini_context("./harpe-sort.so"))
        return 0;

    ntw::cli::Client client;
    client.connect(argv[WEBSITE_PORT],ntw::Config::port_server);


    run(client);
    clean_context();
    
    return 0;
}