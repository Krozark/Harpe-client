#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

namespace ntw
{
    class SocketSerialized;
    namespace cli
    {
        class Client;
    }
}

#define clientWaitForWork 3
#define sendPeptideResults 4

bool ini_context(char* sort_lib_path);
void clean_context();

void run(ntw::cli::Client& client);

int process(ntw::cli::Client& client);


#endif
