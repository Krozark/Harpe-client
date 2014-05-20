#include <harpe-client/defines.hpp>
#include <harpe-client/functions.hpp>
///data base
#include <Socket/client/Client.hpp>
#include <utils/string.hpp>
#include <utils/json/Driver.hpp>

#include <regex>

#define WEBSITE_HOST 1
#define WEBSITE_PORT 2

int register_to_website(char host[],int port)
{
    int status = 0;

    std::cout<<"[Loggin to website] "<<host<<":"<<port<<std::endl;
    ntw::Socket website_sock(ntw::Socket::Domain::IP,ntw::Socket::Type::TCP);
    website_sock.connect(host,port);

    std::string msg;
    msg += "GET /get/ ";
    msg += "HTTP/1.1\r\nHOST: ";
    msg += host;
    msg += "\r\n\r\n";

    char buffer[1024];
    website_sock.send(msg.c_str(),msg.size());

    int recv;
    //float version = 0;
    std::string page;
    while((recv = website_sock.receive(buffer,1024))>0)
    {
        page.append(buffer,recv);
    }
    auto v = utils::string::split(page,"\r\n\r\n"); 
    if(v.size() ==2)
    {
        std::cout<<v[1]<<std::endl;

        for(auto& c : v[1])
            std::cout<<"<"<<c<<">";
        std::cout<<std::endl;

        utils::json::Value* json = utils::json::Driver::parse(v[1]);
        if(json)
            std::cout<<*json<<std::endl;
        
    }

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

    #if __WIN32
    if(not ini_context("./harpe-sort.dll"))
        return 0;
    #elif __unix || __unix__
    if(not ini_context("./harpe-sort.so"))
        return 0;
    #else
    #error "System not detected"
    #endif // __WIN32

    ntw::Socket::init();
    ntw::cli::Client client;
    client.connect(argv[WEBSITE_HOST],atoi(argv[WEBSITE_PORT]));


    run(client);
    clean_context();
    ntw::Socket::close();

    return 0;
}
