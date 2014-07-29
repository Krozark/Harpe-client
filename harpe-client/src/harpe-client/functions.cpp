#include <harpe-client/functions.hpp>
#include <harpe-client/defines.hpp>
#include <harpe-client/models.hpp>

#include <Socket/SocketSerialized.hpp>
#include <Socket/client/Client.hpp>

#include <mgf/Driver.hpp>

#include <harpe-algo/Analyser.hpp>
#include <harpe-algo/Context.hpp>

#include <utils/log.hpp>

#include <Monitoring/sys.hpp>

#include <sstream>

namespace ntw
{
    int dispatch(int id,SocketSerialized& request)
    {
        return 0;
    }
}

bool ini_context(char* sort_lib_path)
{
    if (not harpe::Context::loadFromLib(sort_lib_path))
        return false;

    harpe::Context::error=0.05;
    harpe::Context::finds_max_size=1000;
    harpe::Context::finds_max_size_tmp=1000*5;
    
    return true;
}

void clean_context()
{
    harpe::Context::closeLib();
}


bool run(ntw::cli::Client& client)
{
    bool res = true;
    while (running and res)
    {
        client.call<void>(clientWaitForWork);
        short int status = client.request_sock.getStatus();
        switch(status)
        {
            case ERRORS::STOP :
            {
                utils::log::error("Recv Stop","The server is probably down.","\nThe programme will now stop.");
                client.request_sock.clear();
                res = false;
            }break;
            case ERRORS::OK :
            {
                utils::log::info("Recv","Start procecing datas");
                process(client);                           
                res = true;
                /// ask new task
            }break;
            case ERRORS::TIMEOUT :
            {
                utils::log::info("Recv","Timeout");
                client.request_sock.clear();
                res = true;
                /// ask new task
            }break;
            default :
            {
                utils::log::error("Recv","Server error code:",status);
                client.request_sock.clear();
                res = false;
                /// server error???
            }break;
        }
    }
    return res;
}

int process(ntw::cli::Client& client)
{
    AnalysePeptide pep;
    //extract data
    client.request_sock>>pep;

    // add AAs
    for(AA& aa : pep.analyse.AAs)
    {
        harpe::Context::aa_tab.add(aa.pk,aa.slug,aa.mass);
    }

    //PTMs
    for(AAModification& mod : pep.analyse.modifications)
    {
        for(AAModificationPosition& mod_pos: mod.aas)
        {
            harpe::Context::aa_tab.add(mod_pos.aa.pk,
                                       mod_pos.aa.slug,
                                       mod_pos.aa.mass,
                                       mod.pk,
                                       mod.delta,
                                       mod_pos.position);
        }
    }

    if(harpe::Context::aa_tab.size()<=0)
    {
        //\todo ERROR, invalid input( no AAs)
        utils::log::error("Input","error on input (no AAs)");
        return -1;
    }
    harpe::Context::aa_tab.sort();


    std::stringstream stream(pep.mgf_part);

    int r=0;

    harpe::Context::error=pep.analyse.error;

    mgf::Driver driver(stream);
    mgf::Spectrum* spectrum = nullptr;

    while((spectrum = driver.next(pep.analyse.max_charge)) != nullptr)
    {
        //spectrum->__print__(std::cout);
        int status;
        std::vector<harpe::SequenceToken*> token_ptr;

        std::vector<harpe::Sequence> res = harpe::Analyser::analyse(*spectrum,token_ptr,status,-1);

        sendResults(client.request_sock,pep.pk,res,status); 

        harpe::Analyser::free(token_ptr);
        delete spectrum;
        ++r;
        break;
    }
    harpe::Context::aa_tab.clear();
    
    return r;
}

int sendResults(ntw::SocketSerialized& sock,int pep_pk,std::vector<harpe::Sequence>& results,int status)
{
    int res = 0;
    //clear the data
    sock.clear();
    sock.setStatus(ntw::Status::ok);
    //set the function id
    sock<<(int)sendPeptideResults
        <<pep_pk ///<the peptide pk
        <<status; //<the calculation status

    //add the sequences
    unsigned int size = results.size();
    //TODO get the Max_results from the server
    if(size > 100)
        size = 100;
    sock<<size;
    for(unsigned int i=0;i<size;++i)
        sock<<results[i];

    sock.send();
    //verify return
    res = sock.receive();
    if (res > 0)
    {
        utils::log::info("Recive","Status:",sock.getStatus());
    }
    return res;
}



bool send_config_inf(ntw::cli::Client& client)
{
    int version = VERSION;
    int ram = ((get_max_mem(1)/1024)/1024);
    return client.call<bool>(sendClientInfo,version,ram);
}
