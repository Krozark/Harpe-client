#include <harpe-client/models.hpp>

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
    stream<<"{[AA]pk:"<<self.pk
        <<", slug: "<<self.slug
        <<", mass: "<<self.mass
        <<"}";
    return stream;
};


/****************************
 ***** AAModification *******
****************************/
ntw::Serializer& operator>>(ntw::Serializer& stream,AAModificationPosition& self)
{
    stream>>self.pk
        >>self.aa
        >>self.position;
    return stream;
}

std::ostream& operator<<(std::ostream& stream,AAModificationPosition& self)
{
    stream<<"{[AAModificationPosition]pk: "<<self.pk
        <<", aa: "<<self.aa
        <<", position: "<<self.position
        <<"}";
    return stream;
}

/*************************
 ****** AAModification ***
 ************************/
ntw::Serializer& operator>>(ntw::Serializer& stream,AAModification& self)
{
    unsigned int size;
    stream>>self.pk
        >>self.delta
        >>size;

    for(unsigned int i=0;i<size;++i)
    {
        self.aas.emplace_back();
        stream>>self.aas.back();
    }

    return stream;
}

std::ostream& operator<<(std::ostream& stream,AAModification& self)
{
    stream<<"{[AAModification]pk:"<<self.pk
        <<", delta: "<<self.delta
        <<", aas:[";
    for(auto& u : self.aas)
        stream<<u;
    stream<<"]}";
    return stream;
}



/*****************************
 ****** AnalyseMgf ***********
 ****************************/

ntw::Serializer& operator>>(ntw::Serializer& stream,AnalyseMgf& self)
{
    stream>>self.pk
        >>self.max_charge
        >>self.error;
    //AAs
    {
        unsigned int size;
        stream>>size;
        for(unsigned int i=0;i<size;++i)
        {
            self.AAs.emplace_back();
            stream>>self.AAs.back();
        }
    }
    //PTMs
    {
        unsigned int size;
        stream>>size;
        for(unsigned int i=0;i<size;++i)
        {
            self.modifications.emplace_back();
            stream>>self.modifications.back();
        }
    }
    return stream;
};

std::ostream& operator<<(std::ostream& stream, AnalyseMgf& self)
{
    stream<<"{[AnalyseMgf]pk:"<<self.pk
        <<", max_charge:"<<self.max_charge
        <<", error:"<<self.error
        <<", AAs: [";
    for(AA& aa : self.AAs)
        stream<<", "<<aa;
    stream<<"], modifications:[";

    for(AAModification& mod: self.modifications)
        stream<<", "<<mod;
    stream<<"]}";
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
    stream<<"{[AnalysePeptide]pk:"<<self.pk
        <<", mgf: "<<self.mgf_part
        <<", analyse: "<<self.analyse
        <<"}";
    return stream;
};
