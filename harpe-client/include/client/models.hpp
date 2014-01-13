#ifndef MODELS_HPP
#define MODELS_HPP

#include <string>
#include <iostream>

#include <list>

namespace ntw
{
    class Serializer;
}

/****************************
 ****   AA ******************
 ***************************/

class AA
{
    public:
        int pk;
        std::string slug;
        float mass;

        friend ntw::Serializer& operator>>(ntw::Serializer& stream,AA& self);
        friend std::ostream& operator<<(std::ostream& stream,AA& self);
};

/*****************************
 ****** AnalyseMgf ***********
 ****************************/

class AnalyseMgf
{
    public:
        int pk;
        std::list<AA> AAs;
        
        friend ntw::Serializer& operator>>(ntw::Serializer& stream,AnalyseMgf& self);
        friend std::ostream& operator<<(std::ostream& stream,AnalyseMgf& self);

};

/***********************************
 ********** AnalysePeptide ********
 * ********************************/

class AnalysePeptide
{
    public:
        int pk;
        std::string mgf_part;
        AnalyseMgf analyse;

        friend ntw::Serializer& operator>>(ntw::Serializer& stream,AnalysePeptide& self);
        friend std::ostream& operator<<(std::ostream& stream,AnalysePeptide& self);
};

#endif
