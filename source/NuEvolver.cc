#include "NuEvolver.h"


NuEvolver::NuEvolver()
    : m_oscparam()
    , m_energy(1.0e9)
    , m_baseline(1.0e5)
{
}
NuEvolver::NuEvolver(OscParam op, double energy, double baseline)
    : m_oscparam(op)
    , m_energy(energy)
    , m_baseline(baseline)
{
}
NuEvolver::~NuEvolver()
{
}

const OscParam& NuEvolver::get_oscparams() const 
{
    return m_oscparam; 
}
void NuEvolver::set_oscparams(const OscParam& op) 
{
    m_oscparam = op; 
}


double NuEvolver::get_density(void) const 
{ 
    return m_density; 
}
void NuEvolver::set_density(double density) 
{
    m_density = density; 
}

double NuEvolver::get_energy(void) const 
{ 
    return m_energy; 
}
void NuEvolver::set_energy(double energy_ev) 
{ 
    m_energy = energy_ev; 
}

double NuEvolver::get_baseline(void) const
{ 
    return m_baseline; 
}
void NuEvolver::set_baseline(double baseline_cm) 
{ 
    m_baseline = baseline_cm; 
}
