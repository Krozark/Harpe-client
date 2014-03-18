#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <vector>

namespace ntw
{
    class SocketSerialized;
    namespace cli
    {
        class Client;
    }
}

namespace harpe
{
    class Sequence;
}

#define clientWaitForWork 3
#define sendPeptideResults 4

bool ini_context(char* sort_lib_path);
void clean_context();

void run(ntw::cli::Client& client);

int process(ntw::cli::Client& client);


int sendResults(ntw::SocketSerialized& sock,int pep_pk,std::vector<harpe::Sequence>& results,int status);


#endif
