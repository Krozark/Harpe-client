#ifndef DEFINES_HPP
#define DEFINES_HPP

#include <Socket/define.hpp>

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
