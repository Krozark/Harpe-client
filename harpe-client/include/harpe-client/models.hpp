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


/*************************
 ******* AA modified *****
 ************************/

class AAModificationPosition
{
    /*CHOICES = ((1,"partout"),(2,"N-term"),(3,"C-term")*/

    public:
        int pk;
        AA aa;
        int           position;

        friend ntw::Serializer& operator>>(ntw::Serializer& stream,AAModificationPosition& self);
        friend std::ostream& operator<<(std::ostream& stream,AAModificationPosition& self);

};

class AAModification
{
    public:
        int     pk;
        float   delta;
        std::list<AAModificationPosition> aas;

        friend ntw::Serializer& operator>>(ntw::Serializer& stream,AAModification& self);
        friend std::ostream& operator<<(std::ostream& stream,AAModification& self);
};


/*****************************
 ****** AnalyseMgf ***********
 ****************************/

class AnalyseMgf
{
    public:
        int pk;
        std::list<AA> AAs;
        std::list<AAModification> modifications;
        
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
