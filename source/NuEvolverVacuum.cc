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

ComplexMatrix NuEvolverVacuum::get_transform()
{
    return m_transform; 
}

//ComplexMatrix NuEvolverVacuum::get_transform() const
//{
//    return m_transform;
//}

ComplexVector
NuEvolverVacuum::operator()(ComplexVector nu, double x) const 
{
//    cerr << x << endl;
    ComplexVector ret(3);
    ret(0) = ret(1) = ret(2) = 0.0;

#if 0
    using namespace blitz::tensor; // for i,j
    ret = sum(m_transform(i,j)*nu(j),j);
#else
    for (int i=0; i<3; ++i) {
        for (int j=0; j<3; ++j) {
            ret(i) += m_transform(i,j)*nu(j);
        }
    }
#endif
    return ret;
}

void NuEvolverVacuum::calculate()
{
//    cerr << "NuEvolverVacuum::calculate()\n";
    const OscParam& op = this->get_oscparams();

    ComplexMatrix msqrd = mass_squared_matrix(op.get_dms21(),op.get_dms31());
    ComplexMatrix U = op.get_mixing_matrix();
    ComplexMatrix Udagger = hermitian_conjugate(U);

    using namespace blitz::tensor; // for i,j,k.

    // -i/2E (U)(M^2)(Udagger) / (hbar*c)
    ComplexMatrix tmp = matrix_product(msqrd,Udagger);
    for (int i=0;i<3;++i) for(int j=0;j<3;++j) 
        m_transform(i,j) = tmp(i,j);
    ComplexMatrix tmp2 = matrix_product(U,m_transform);
    for (int i=0;i<3;++i) for(int j=0;j<3;++j) 
        m_transform(i,j) = tmp2(i,j);
    m_transform *= -0.5*EYE/this->get_energy()/hbarc;
}
