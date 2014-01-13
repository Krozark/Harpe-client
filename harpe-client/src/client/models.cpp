#include <client/models.hpp>

#include <Socket/Serializer.hpp>

/****************************
 ****   AA ******************
 ***************************/

ntw::Serializer& operator>>(ntw::Serializer& stream,AA& self)
{
    stream>>self.pk
        >>self.slug
        >>self.mass;
    return stream;
}

std::ostream& operator<<(std::ostream& stream, AA& self)
{
    stream<<"pk:"<<self.pk
        <<", slug "<<self.slug
        <<", mass: "<<self.mass;
    return stream;
};

/*****************************
 ****** AnalyseMgf ***********
 ****************************/

ntw::Serializer& operator>>(ntw::Serializer& stream,AnalyseMgf& self)
{
    unsigned int size;
    stream>>self.pk
        >>size;
    for(unsigned int i=0;i<size;++i)
    {
        AA tmp;
        stream>>tmp;
        self.AAs.emplace_back(std::move(tmp));
    }
    return stream;
};

std::ostream& operator<<(std::ostream& stream, AnalyseMgf& self)
{
    stream<<"pk:"<<self.pk
        <<", size: "<<self.AAs.size();
    for(AA& aa : self.AAs)
        stream<<", AA: "<<aa;
    return stream;
};

/***********************************
 ********** AnalysePeptide ********
 * ********************************/
ntw::Serializer& operator>>(ntw::Serializer& stream, AnalysePeptide& self)
{
    stream>>self.pk
        >>self.mgf_part
        >>self.analyse;
    return stream;
};

std::ostream& operator<<(std::ostream& stream, AnalysePeptide& self)
{
    stream<<"pk:"<<self.pk
        <<", mgf: "<<self.mgf_part
        <<", analyse: "<<self.analyse;
    return stream;
};
