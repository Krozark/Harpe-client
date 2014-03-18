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
/**
 * \brief A class to manage amino acids
 */
class AA
{
    public:
        int pk; ///< the pk in the DB
        std::string slug; ///< the slug of the AA (identifier)
        float mass; ///< his mass (in Da)

        friend ntw::Serializer& operator>>(ntw::Serializer& stream,AA& self);
        friend std::ostream& operator<<(std::ostream& stream,AA& self);
};


/*************************
 ******* AA modified *****
 ************************/
/**
 * \brief A class to manage post traductional modifications for one AA
 */
class AAModificationPosition
{

    public:
        int pk; ///< the pk in the DB
        AA aa; ///< the AA affected
        int           position; ///< the position of the modification, CHOICES = ((1,"partout"),(2,"N-term"),(3,"C-term")*/

        friend ntw::Serializer& operator>>(ntw::Serializer& stream,AAModificationPosition& self);
        friend std::ostream& operator<<(std::ostream& stream,AAModificationPosition& self);

};

/**
 * \brief A class to manage post traductional modifications
 */
class AAModification
{
    public:
        int     pk; ///< his pk in DB
        float   delta; ///< the delta of the mass (in Da)
        std::list<AAModificationPosition> aas;///< the AA affected

        friend ntw::Serializer& operator>>(ntw::Serializer& stream,AAModification& self);
        friend std::ostream& operator<<(std::ostream& stream,AAModification& self);
};


/*****************************
 ****** AnalyseMgf ***********
 ****************************/
/**
 * \brief A analyse
 */
class AnalyseMgf
{
    public:
        int pk; ///< hise pk in DB
        std::list<AA> AAs; ///< the AA to use to analyse it
        std::list<AAModification> modifications;///< all the additional modifications
        
        friend ntw::Serializer& operator>>(ntw::Serializer& stream,AnalyseMgf& self);
        friend std::ostream& operator<<(std::ostream& stream,AnalyseMgf& self);

};

/***********************************
 ********** AnalysePeptide ********
 * ********************************/
/**
 * \brief One peptide to analyse
 */
class AnalysePeptide
{
    public:
        int pk; ///< his pk in DB
        std::string mgf_part; ///< the mgf datas to analyse
        AnalyseMgf analyse; ///< related to 

        friend ntw::Serializer& operator>>(ntw::Serializer& stream,AnalysePeptide& self);
        friend std::ostream& operator<<(std::ostream& stream,AnalysePeptide& self);
};

#endif
