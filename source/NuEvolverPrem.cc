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
    ComplexMatrix m(3,3);
    m = this->NuEvolverVacuum::get_transform();
    m(0,0) += -EYE*A;

    using namespace blitz::tensor; // for i,j
    ComplexVector ret(3);
    ret = sum(m(i,j)*nu(j),j);

    return ret;
}
