#include "NuEvolverConstant.h"
#include "nuosc_matrix.h"
#include "nuosc_earth.h"
#include "constants.h"

NuEvolverConstant::NuEvolverConstant(double density)
    : NuEvolverVacuum()
    , m_density(density)
{
    this->real_calculate();
}
NuEvolverConstant::NuEvolverConstant(OscParam op, double energy, double baseline,
                                     double density)
    : NuEvolverVacuum(op,energy,baseline)
    , m_density(density)
{
    this->real_calculate();
}

double NuEvolverConstant::get_density(void) const 
{ 
    return m_density; 
}
void NuEvolverConstant::set_density(double density) 
{
    m_density = density; 
    this->calculate();
}

void NuEvolverConstant::calculate()
{
    this->NuEvolverVacuum::calculate();
    this->real_calculate();
}

// Imp optimization detail: this is separated out from calculate() so
// we can call it in the ctors w/out causing the base class's
// calculate() to have to be called twice.

void NuEvolverConstant::real_calculate()
{
    // A = sqrt(2) GF Ne [=7.6e-14*Y*rho eV when rho = g/cm^3]
    double A = 7.6e-14*m_density*earth_electron_fraction_by_density(m_density);
    // Convert from eV to 1/cm
    A /= hbarc;
    if (this->get_oscparams().is_antineutrino()) A *= -1.0;

    // Get reference to transform matrix.
    ComplexMatrix m = this->NuEvolverVacuum::get_transform();
    m(0,0) += -EYE*A;
}
