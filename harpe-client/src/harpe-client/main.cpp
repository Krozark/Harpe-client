#include <harpe-client/defines.hpp>
#include <harpe-client/functions.hpp>
///data base
#include <Socket/client/Client.hpp>
#include <utils/log.hpp>
#include <utils/string.hpp>
#include <utils/json/Driver.hpp>

#include <list>

#define WEBSITE_HOST 1
#define WEBSITE_PORT 2

struct server {
    std::string name;
    int version;
    std::string ip;
    int port;
};

std::list<server> register_to_website(char host[],int port)
{
    utils::log::info("Loggin to website",host,":",port);
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

    std::list<server> res;

    if(v.size() ==2)
    {
        utils::json::Value* json_v = utils::json::Driver::parse(v[1]);
        if(json_v)
        {
            utils::json::Object& json = *json_v;
            if(json["status"].as_string() == "ok")
            {
                utils::json::Array& data = json["data"];
                for(auto& u : data)
                {
                    utils::json::Object& ser= u;
                    server tmp;
                    tmp.name = ser["name"].as_string();
                    tmp.port = ser["port"].as_int();
                    tmp.version = ser["version"].as_int();
                    tmp.ip = ser["ip"].as_string();

                    res.emplace_back(std::move(tmp));
                }
            }
            else
            {
                utils::log::error("register to website","error code:",json["status"],", message:",json["message"]);
            }
        }
        delete json_v;
        
    }

    return res;
}


int main(int argc,char* argv[])
{
    std::cout<<utils::log::colors::green<<"===\nHarpe client\nversion:"<<MAJOR_VERSION<<"."<<MINOR_VERSION<<"."<<PATCH_VERSION<<"\n===\n"<<utils::log::colors::reset<<std::endl;

    if(argc < WEBSITE_PORT+1)
    {
        utils::log::error("Usage",argv[0],"<website-host> <website-port>");
        return 1;
    }


    std::list<server> servers = register_to_website(argv[WEBSITE_HOST],atoi(argv[WEBSITE_PORT]));

    if(servers.size()>0)
    {
        servers.sort([](const server& _1,const server& _2)->bool{
                     return _1.version > _2.version;
                     });

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

        for(server& ser : servers)
        {
            utils::log::info("Connect","Use server",ser.name,"on ip",ser.ip,"and port",ser.port,"of version",ser.version);

            if(client.connect(ser.ip,ser.port) == ntw::Status::ok)
            {
                utils::log::info("Connect","Connexion etablish");
                run(client);
                break;
            }
            else
                utils::log::error("Connect","Connexion failed");
        }

        clean_context();
    }
    else
    {
        utils::log::error("No server avalible");
    }

    ntw::Socket::close();

    return 0;
}
