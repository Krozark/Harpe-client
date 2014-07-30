#ifndef DEFINES_HPP
#define DEFINES_HPP

#include <Socket/Status.hpp>

#define MAJOR_VERSION 0 //major version number (0..9999)
#define MINOR_VERSION 4 //minor version number (0..99)
#define PATCH_VERSION 8 //patch version number (0..99)

#define VERSION ((MAJOR_VERSION*100*100)+(MINOR_VERSION*100)+PATCH_VERSION)
#define LOG(sock,func,msg) std::cerr<<"[id:"<<sock.id()<<"] "<< func<<"()"<<" => "<<msg <<std::endl;

/**
 * \brief store the erros codes
 */
enum ERRORS {
    STOP = ntw::Status::stop,
    OK = ntw::Status::ok,
    INPUT_NOT_VALID = ntw::Status::user_define,
    EMPTY_INPUT,
    PK_ERROR,
    EMPTY_DATA_SEND,
    TIMEOUT,
};

#endif
