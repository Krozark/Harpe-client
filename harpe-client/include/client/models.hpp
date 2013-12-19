#ifndef MODELS_HPP
#define MODELS_HPP

#include <string>
#include <iostream>

namespace ntw
{
    class Serializer;
}

class AnalysePeptide
{
    public:
        int pk;
        int analyse_fk;
        std::string mgf_part;

        friend ntw::Serializer& operator>>(ntw::Serializer& stream,AnalysePeptide& self);
        friend std::ostream& operator<<(std::ostream& stream,AnalysePeptide& self);
};


#endif
