#include "NuEvolverVacuum.h"
#include "nuosc_matrix.h"
#include "constants.h"

NuEvolverVacuum::NuEvolverVacuum()
    : NuEvolver()
    , m_transform(3,3)
{
    this->calculate();
}
NuEvolverVacuum::NuEvolverVacuum(OscParam op, double energy, double baseline)
    : NuEvolver(op,energy,baseline)
    , m_transform(3,3)
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

ComplexVector
NuEvolverVacuum::operator()(ComplexVector nu, double x) const 
{
//    cerr << x << endl;
    using namespace blitz::tensor; // for i,j
    ComplexVector ret(3);
    ret = sum(m_transform(i,j)*nu(j),j);
    return ret;
}

void NuEvolverVacuum::calculate()
{
    const OscParam& op = this->get_oscparams();

    ComplexMatrix msqrd = mass_squared_matrix(op.get_dms21(),op.get_dms31());
    ComplexMatrix U = op.get_mixing_matrix();
    ComplexMatrix Udagger = hermitian_conjugate(U);

    using namespace blitz::tensor; // for i,j,k.

    // -i/2E (U)(M^2)(Udagger) / (hbar*c)
    m_transform = matrix_product(msqrd,Udagger);
    m_transform = matrix_product(U,m_transform);
    m_transform *= -0.5*EYE/this->get_energy()/hbarc;
}
