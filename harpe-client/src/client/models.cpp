#include <client/models.hpp>

#include <Socket/Serializer.hpp>

ntw::Serializer& operator>>(ntw::Serializer& stream, AnalysePeptide& self)
{
    stream>>self.pk>>self.analyse_fk>>self.mgf_part;
    return stream;
};

std::ostream& operator<<(std::ostream& stream, AnalysePeptide& self)
{
    stream<<"pk:"<<self.pk<<", fk: "<<self.analyse_fk<<", mgf: "<<self.mgf_part;
    return stream;
};
