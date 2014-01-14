#include <client/functions.hpp>

#include <client/defines.hpp>
#include <client/models.hpp>

#include <Socket/SocketSerialized.hpp>
#include <Socket/client/Client.hpp>
#include <Socket/Config.hpp>

#include <mgf/Driver.hpp>
#include <harpe-algo/Analyser.hpp>
#include <harpe-algo/Context.hpp>

#include <sstream>

namespace ntw
{
    int dispatch(int id,SocketSerialized& request){}
}

bool ini_context(char* sort_lib_path)
{
    if (not harpe::Context::loadFromLib(sort_lib_path))
        return false;

    harpe::Context::error=0.05;
    harpe::Context::finds_max_size=1000;
    harpe::Context::finds_max_size_tmp=1000*5;
    
    ntw::Config::default_timeout = 5.f;
    ntw::Config::broadcast = false;



    return true;
}

void clean_context()
{
    harpe::Context::closeLib();
}


void run(ntw::cli::Client& client)
{
    bool run = true;
    while (run)
    {
        client.call<void>(clientWaitForWork);
        //std::cout<<client.request_sock<<std::endl;
        switch(client.request_sock.getStatus())
        {
            case ERRORS::STOP :
            {
                std::cout<<"[Recv] Stop"<<std::endl;
                client.request_sock.clear();
                run = false;
            }break;
            case ERRORS::OK :
            {
                std::cout<<"[Recv] Start procecing datas "<<client.request_sock.size()<<std::endl;
                process(client);
                /// ask new task
            }break;
            case ERRORS::TIMEOUT :
            {
                std::cout<<"[Recv] Timeout"<<std::endl;
                client.request_sock.clear();
                /// ask new task
            }break;
            default :
            {
                std::cout<<"[Recv] Server error code:"<<client.request_sock.getStatus()<<std::endl;
                client.request_sock.clear();
                /// server error???
            }break;
        }
    }
}

int process(ntw::cli::Client& client)
{
    AnalysePeptide pep;
    //extract data
    client.request_sock>>pep;
    
    // add AAs
    for(AA& aa : pep.analyse.AAs)
        harpe::Context::aa_tab.add(aa.pk,aa.slug,aa.mass);

    if(harpe::Context::aa_tab.size()<=0)
    {
        //\todo ERROR, invalid input( no AAs)
        return -1;
    }
    harpe::Context::aa_tab.sort();


    std::stringstream stream(pep.mgf_part);

    int r=0;

    mgf::Driver driver(stream);
    mgf::Spectrum* spectrum = nullptr;
    while((spectrum = driver.next()) != nullptr)
    {
        spectrum->__print__(std::cout);
        std::vector<harpe::Sequence> res = harpe::Analyser::analyse(*spectrum,-1);
        
        //clear the data
        client.request_sock.clear();
        //set the function id
        client.request_sock<<sendPeptideResults
            //the peptide pk
            <<pep.pk;

        //add the sequences
        unsigned int size = res.size();
        if(size > 100)
            size = 100;
        client.request_sock<<size;
        for(unsigned int i=0;i<size;++i)
            client.request_sock<<res[i];

        ///send datas
        client.request_sock.send();
        //verify return
        if (client.request_sock.receive() > 0)
        {
            std::cerr<<"Recive Status: "<<client.request_sock.getStatus()<<std::endl;
        }
        
        harpe::Analyser::free();
        delete spectrum;
        ++r;
    }
    
    return r;
}
