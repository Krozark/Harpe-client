#ifndef DEFINES_HPP
#define DEFINES_HPP

#include <Socket/define.hpp>

#define MAJOR_VERSION 0 //major version number (0..9999)
#define MINOR_VERSION 4 //minor version number (0..99)
#define PATCH_VERSION 0 //patch version number (0..99)

#define VERSION ((MAJOR_VERSION*100*100)+(MINOR_VERSION*100)+PATCH_VERSION)

/**
 * \brief store the erros codes
 */
enum ERRORS {
    STOP = NTW_STOP_CONNEXION,
    OK = 0,
    INPUT_NOT_VALID = 1,
    EMPTY_INPUT = 2,
    PK_ERROR = 3,
    EMPTY_DATA_SEND = 4,
    TIMEOUT = 5,
};



#endif
