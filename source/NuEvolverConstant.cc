#include "NuEvolverConstant.h"
#include "nuosc_matrix.h"
#include "nuosc_earth.h"

NuEvolverConstant::NuEvolverConstant(bool anti_neutrino)
    : NuEvolverVacuum()
    , m_antineutrino(anti_neutrino)
{
    this->real_calculate();
}
NuEvolverConstant::NuEvolverConstant(OscParam op, double energy, double baseline,
                                     bool anti_neutrino)
    : NuEvolverVacuum(op,energy,baseline)
    , m_antineutrino(anti_neutrino)
{
    this->real_calculate();
}

void NuEvolverConstant::assume_anti_neutrino(bool tf)
{
    if (m_antineutrino == tf) return;
    m_antineutrino = tf;
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
    double density = this->get_density();
    double matter = 7.6e-14 * density;
    matter *= earth_electron_fraction_by_density(density);
    if (m_antineutrino) matter *= -1.0;
    this->NuEvolverVacuum::get_transform()(0,0) += -EYE*matter;
}
