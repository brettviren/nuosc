#include "NuEvolverVacuum.h"
#include "nuosc_matrix.h"

NuEvolverVacuum::NuEvolverVacuum()
    : NuEvolver()
{
    this->calculate();
}
NuEvolverVacuum::NuEvolverVacuum(OscParam op, double energy, double baseline)
    : NuEvolver(op,energy,baseline)
{
    this->calculate();
}


void NuEvolverVacuum::set_oscparams(const OscParam& op)
{
    this->NuEvolver::set_oscparams(op);
    this->calculate();
}
void NuEvolverVacuum::set_energy(double energy)
{
    this->NuEvolver::set_energy(energy);
    this->calculate();
}

ComplexColumnVector
NuEvolverVacuum::operator()(const ComplexColumnVector& nu, double x) const 
{
    ComplexColumnVector ret = m_transform*nu;
    cerr<< "Start with:\n" << nu << "\nEnd with:\n" << ret << endl;

    return ret;
}

void NuEvolverVacuum::calculate()
{
    const OscParam& op = this->get_oscparams();

    ComplexMatrix msqrd = mass_squared_matrix(op.get_dms21(),op.get_dms31());

    ComplexMatrix U = op.get_mixing_matrix();
    ComplexMatrix Udagger = U.hermitian();

    m_transform = -EYE/(2.0*this->get_energy()) * (U*(msqrd*Udagger));
}
