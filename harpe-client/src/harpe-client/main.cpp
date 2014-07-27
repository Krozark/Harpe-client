#include <harpe-client/defines.hpp>
#include <harpe-client/functions.hpp>
///data base
#include <Socket/client/Client.hpp>
#include <utils/log.hpp>
#include <utils/string.hpp>
#include <utils/json/Driver.hpp>

#include <csignal>
#include <list>

#define WEBSITE_HOST 1
#define WEBSITE_PORT 2

volatile bool running = false;

extern "C" {
    int _get_output_format( void ){ return 0; } //to avoid linker error on windows
}

/**
 * \brief Signal hnadler to stop
 * proprey
 */
void stop_handler(int sig)
{
    utils::log::warning("Signal","Recv signal",sig,".Stoping.\n Please wait.");
    running = false;
}

struct server {
    std::string name;
    int version;
    std::string ip;
    int port;
};

int tentative_to_sec(int i)
{
    if(i <= 1)
        return 10;
    if( i == 2)
        return 30;
    if(i <= 3)
        return 60*1;
    if(i == 4)
        return 60*2.5;
    return 60*5;
}

void wait(int tentative)
{
    int second = tentative_to_sec(tentative);
    int time = second*1000; //min * secondes * millisecondes
    const int delta = 500;
    
    int status = -1;
    char c[] = "|/-\\";
    utils::log::info("Wait",int(time/1000)," secondes.");
    while(time > 0 and running)
    {
        status = (status+1)%4;
        std::clog<<utils::log::colors::yellow<<"\rWait "<<int(time/1000)<<" secondes ... ["<<c[status]<<"]."<<utils::log::colors::reset<<std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delta));
        time -= delta;
    }
    std::clog<<std::endl;
}


std::list<server> register_to_website(char host[],int port)
{
    utils::log::info("Loggin to website",host,":",port);

    ntw::Socket website_sock(ntw::Socket::Domain::IP,ntw::Socket::Type::TCP);
    std::list<server> res;

    bool connected = false;

    int tentative = 0;

    while(not connected and running)
    {
        res.clear();
        if(website_sock.connect(host,port))
        {

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
                utils::json::Value* json_v = utils::json::Driver::parse(v[1]);
                if(json_v)
                {
                    utils::json::Object& json = *json_v;
                    if(json["status"].as_string() == "ok")
                    {
                        connected = true;

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
        }
        else
        {
            utils::log::error("Connection","Unable to connect to website.");
            wait(++tentative);
            website_sock.disconnect();
        }
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

    running = true;
    std::signal(SIGINT, stop_handler);

    int tentative = 0;

    while(running)
    {
        std::list<server> servers = register_to_website(argv[WEBSITE_HOST],atoi(argv[WEBSITE_PORT]));

        if(running)
        {
            if(servers.size()>0)
            {
                servers.sort([](const server& _1,const server& _2)->bool{
                             return _1.version > _2.version;
                             });

                #if __WIN32
                char lib[] = "./harpe-sort.dll";
                #elif __unix || __unix__
                char lib[] = "./harpe-sort.so";
                #else
                #error "System not detected"
                #endif // __WIN32

                if(not ini_context(lib))
                    return 0;

                ntw::Socket::init();
                ntw::cli::Client client;

                bool connected = false;

                for(server& ser : servers)
                {
                    utils::log::info("Connect","Use server",ser.name,"on ip",ser.ip,"and port",ser.port,"of version",ser.version);

                    if(client.connect(ser.ip,ser.port) == ntw::Status::ok)
                    {
                        utils::log::info("Connect","Connexion etablish");
                        if(send_config_inf(client))
                        {
                            connected = true;
                            run(client);
                        }
                        else
                        {
                            utils::log::warning("Connect","Version not compatible");
                        }
                        break;
                    }
                    else
                        utils::log::error("Connect","Connexion failed");
                }
                clean_context();

                if(not connected)
                {
                    utils::log::warning("Unable to connect to any server");
                    wait(++tentative);
                }
                else
                    tentative = 0;
            }
            else
            {
                utils::log::error("No server avalible");
                wait(++tentative);
            }
        }
    }

    ntw::Socket::close();

    return 0;
}
