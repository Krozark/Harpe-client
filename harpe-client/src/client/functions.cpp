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
                run = false;
            }break;
            case ERRORS::OK :
            {
                std::cout<<"[Recv] Start procecing datas "<<client.request_sock.size()<<std::endl;
                process(client);
                ///\todo start analyse
                /// ask new task
            }break;
            case ERRORS::TIMEOUT :
            {
                std::cout<<"[Recv] Timeout"<<std::endl;
                /// ask new task
            }break;
            default :
            {
                std::cout<<"[Recv] Server error code:"<<client.request_sock.getStatus()<<std::endl;
                /// server error???
            }break;
        }
        client.request_sock.clear();
    }
}

int process(ntw::cli::Client& client)
{
    AnalysePeptide pep;
    std::cout<<"[RECV] "<<client.request_sock<<std::endl;
    client.request_sock>>pep;

    std::cout<<"[RECV] "<<pep<<std::endl;

    for(AA& aa : pep.analyse.AAs)
        harpe::Context::aa_tab.add(aa.pk,aa.slug,aa.mass);
    /*harpe::Context::aa_tab.add(0,"A",71.037110);
    harpe::Context::aa_tab.add(1,"C",103.009185);
    harpe::Context::aa_tab.add(2,"D",115.026943);
    harpe::Context::aa_tab.add(3,"E",129.042593);
    harpe::Context::aa_tab.add(4,"F",147.068414);
    harpe::Context::aa_tab.add(5,"G",57.021464);
    harpe::Context::aa_tab.add(6,"H",137.058912);
    harpe::Context::aa_tab.add(7,"I-L",113.084064);
    harpe::Context::aa_tab.add(8,"K",128.094963);
    harpe::Context::aa_tab.add(9,"M",131.040485);
    harpe::Context::aa_tab.add(10,"N",114.042927);
    harpe::Context::aa_tab.add(11,"P",97.052764);
    harpe::Context::aa_tab.add(12,"Q",128.058578);
    harpe::Context::aa_tab.add(13,"R",156.101111);
    harpe::Context::aa_tab.add(14,"S",87.032028);
    harpe::Context::aa_tab.add(15,"T",101.047679);
    harpe::Context::aa_tab.add(16,"V",99.068414);
    harpe::Context::aa_tab.add(17,"W",186.079313);
    harpe::Context::aa_tab.add(18,"Y",163.063320);*/

    harpe::Context::aa_tab.sort();


    std::stringstream stream(pep.mgf_part);

    /*mgf::Driver driver(stream);
    mgf::Analyse analyse = driver.parse();

    //is a valid MGF format
    if (not driver.isValid())
    {
        LOG(sock,"analyse","INPUT_NOT_VALID");
        return -1;
    }

    /// as peptides
    const unsigned int size = analyse.size();
    LOG(sock,"analyse",size<<" peptides to analyse");
    if(size<=0)
    {
        LOG(sock,"analyse","EMPTY_INPUT");
        return -2;
    }

    ///\todo save in bdd
    const std::list<mgf::Spectrum*>& spectrums = analyse.getSpectrums();
   
    for(mgf::Spectrum* spectrum : spectrums)
    {*/
    

    int r=0; 

    mgf::Driver driver(stream);
    mgf::Spectrum* spectrum = nullptr;
    while((spectrum = driver.next()) != nullptr)
    {
        spectrum->__print__(std::cout);
        std::vector<harpe::Sequence> res = harpe::Analyser::analyse(*spectrum,-1);
        harpe::Analyser::free();
        delete spectrum;
        ++r;
    }
    
    return r;
}
