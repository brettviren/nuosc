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
    , m_dirty_mixing(true)
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

ComplexMatrix OscParam::get_mixing_matrix() const
{
    if (!m_dirty_mixing) return m_mixing_matrix;
    
    m_mixing_matrix = mixing_matrix(m_theta_12,m_theta_23,m_theta_13,
                                    m_delta_cp);
    m_dirty_mixing = false;
    return m_mixing_matrix;
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
    m_theta_12 = rad;
    m_dirty_mixing = true;
}
void OscParam::set_theta23(double rad)
{
    m_theta_23 = rad;
    m_dirty_mixing = true;
}
void OscParam::set_theta13(double rad)
{
    m_theta_13 = rad;
    m_dirty_mixing = true;
}
void OscParam::set_deltacp(double rad)
{
    m_delta_cp = rad;
    m_dirty_mixing = true;
}
