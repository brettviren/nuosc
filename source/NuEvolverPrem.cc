#include "NuEvolverPrem.h"
#include "nuosc_matrix.h"
#include "nuosc_earth.h"
#include "constants.h"

NuEvolverPrem::NuEvolverPrem()
    : NuEvolverVacuum()
{
}
NuEvolverPrem::NuEvolverPrem(OscParam op, double energy, double baseline)
    : NuEvolverVacuum(op,energy,baseline)
{
}

ComplexVector NuEvolverPrem::operator()(ComplexVector nu, double x) const
{
    double density = earth_density(x,this->get_baseline());

    // A = sqrt(2) GF Ne [=7.6e-14*Y*rho eV when rho = g/cm^3]
    double A = 7.6e-14*density*earth_electron_fraction_by_density(density);
    // Convert from eV to 1/cm
    A /= hbarc;
    if (this->get_oscparams().is_antineutrino()) A *= -1.0;

    // Get COPY of the vacuum term and add the matter term
    NuEvolverPrem* nonconstthis = const_cast<NuEvolverPrem*>(this);
    ComplexMatrix m = nonconstthis->NuEvolverVacuum::get_transform();
    m(0,0) += -EYE*A;

    ComplexVector ret(3);
    ret(0) = ret(1) = ret(2) = 0.0;

#if 1
    blitz::firstIndex i;
    blitz::secondIndex j;
    ret = blitz::sum(m(i,j)*nu(j),j);
#else
    for (int i=0; i<3; ++i) {
        for (int j=0; j<3; ++j) {
            ret(i) += m(i,j)*nu(j);
        }
    }
#endif
    return ret;
}
