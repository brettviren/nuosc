#include "OscParam.h"
#include "nuosc_matrix.h"

#include <cmath>


OscParam::OscParam(double dm2_21, double dm2_31,
                   double theta_12, double theta_23, double theta_13,
                   double delta_cp)
    : m_dm2_21(dm2_21)
    , m_dm2_31(dm2_31)
    , m_theta_12(theta_12)
    , m_theta_23(theta_23)
    , m_theta_13(theta_13)
    , m_delta_cp(delta_cp)
    , m_mixing_matrix(3,3)
    , m_matter_matrix(3,3)
    , m_dirty(true)
{
}
OscParam::~OscParam()
{
}

double OscParam::get_dms21() const
{
    return m_dm2_21;
}
double OscParam::get_dms31() const
{
    return m_dm2_31;
}
double OscParam::get_theta12() const
{
    return m_theta_12;
}
double OscParam::get_theta23() const
{
    return m_theta_23;
}
double OscParam::get_theta13() const
{
    return m_theta_13;
}
double OscParam::get_deltacp() const
{
    return m_delta_cp;
}
double OscParam::get_ss2t12() const
{
    double s = sin(2.0*m_theta_12);
    return s*s;
}
double OscParam::get_ss2t23() const
{
    double s = sin(2.0*m_theta_23);
    return s*s;
}
double OscParam::get_ss2t13() const
{
    double s = sin(2.0*m_theta_13);
    return s*s;
}

void OscParam::calculate() const
{
    if (! m_dirty) return;

    m_mixing_matrix = mixing_matrix(m_theta_12,m_theta_23,m_theta_13,
                                    m_delta_cp);
    ComplexMatrix Udagger = hermitian_conjugate(m_mixing_matrix);
    ComplexVector onezerozero(3);
    onezerozero(0) = 1.0;
    onezerozero(1) = 0.0;
    onezerozero(2) = 0.0;

    using namespace blitz::tensor; // for i,j,k
    m_matter_matrix = sum(Udagger(i,k)*onezerozero(k)*m_mixing_matrix(k,j),k);

//    cerr << "Udagger * diag(1,0,0) * U = " << m_matter_matrix << endl;
    m_matter_matrix(0,0) -= 1.0/3.0; 
    m_matter_matrix(1,1) -= 1.0/3.0; 
    m_matter_matrix(2,2) -= 1.0/3.0; 

    m_dirty = false;
}

ComplexMatrix OscParam::get_mixing_matrix() const
{
    this->calculate();
    return m_mixing_matrix;
}
ComplexMatrix OscParam::get_matter_matrix() const
{
    this->calculate();
    return m_matter_matrix;
}

void OscParam::set_dms21(double ev2)
{
    m_dm2_21 = ev2;
}
void OscParam::set_dms31(double ev2)
{
    m_dm2_31 = ev2;
}
void OscParam::set_theta12(double rad)
{
    m_dirty = true;
    m_theta_12 = rad;
}
void OscParam::set_theta23(double rad)
{
    m_dirty = true;
    m_theta_23 = rad;
}
void OscParam::set_theta13(double rad)
{
    m_dirty = true;
    m_theta_13 = rad;
}
void OscParam::set_deltacp(double rad)
{
    m_dirty = true;
    m_delta_cp = rad;
}
