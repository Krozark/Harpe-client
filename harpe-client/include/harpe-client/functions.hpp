#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <vector>

/**
 * \brief define some class used later
 */
namespace ntw
{
    class SocketSerialized;
    namespace cli
    {
        class Client;
    }
}

/**
 * \brief define some class used later
 */
namespace harpe
{
    class Sequence;
}

#define clientWaitForWork 3 ///< the function id in the server
#define sendPeptideResults 4 ///< the function id in the server
#define sendClientInfo 5 ///< the function id in the server

/**
 * \brief initialise the context
 * \param sort_lib_path the path of harpe-sort.[so/dll]
 */
bool ini_context(char* sort_lib_path); 

/**
 * \brief close the open dynamic lib
 */
void clean_context();

/**
 * \brief start the client
 * \param client the client to start
 */
void run(ntw::cli::Client& client);

/**
 * \brief start the calculation of the analyse recive by the network
 * \param the client who recive the datas
 */
int process(ntw::cli::Client& client);

/**
 * \brief send back to the server the results of the calculation
 * \param sock the client/server socket
 * \param pep_pk the peptide pk recive
 * \param results the results to send
 * \param status the status of the calculation
 */
int sendResults(ntw::SocketSerialized& sock,int pep_pk,std::vector<harpe::Sequence>& results,int status);

/**
 * \brief send client info:
 * - version (int)
 * - ram in Mo (int)
 */
bool send_config_inf(ntw::cli::Client& client);

#endif
