#include "OscParam.h"

#include <cmath>

OscParam::OscParam(double dm2_21, dm2_31,
                   double theta_12, double theta_23, double theta_13,
                   double delta_cp)
    : m_dm2_21(dm2_21)
    , m_dm2_31(dm2_31)
    , m_theta_12(theta_12)
    , m_theta_23(theta_23)
    , m_theta_13(theta_13)
    , m_delta_cp(delta_cp)
    , m_dirty(true)
{
}
OscParam::~OscParam()
{
}

double get_dm2_21()
{
    return m_dm2_21;
}
double get_dm2_31()
{
    return m_dm2_31;
}
double get_theta_12()
{
    return m_theta_12;
}
double get_theta_23()
{
    return m_theta_23;
}
double get_theta_13()
{
    return m_theta_13;
}
double get_delta_cp()
{
    return m_delta_cp;
}
double get_s22t12()
{
    double s = sin(2.0*m_theta_12);
    return s*s;
}
double get_s22t23()
{
    double s = sin(2.0*m_theta_23);
    return s*s;
}
double get_s22t13()
{
    double s = sin(2.0*m_theta_13);
    return s*s;
}

ComplexMatrix get_mixing_matrix()
{
    if (!m_dirty) return m_mixing_matrix;
    
    
}

void set_dm2_21(double ev2)
{
    m_dm2_21 = ev2;
    m_dirty = true;
}
void set_dm2_31(double ev2)
{
    m_dm2_31 = ev2;
    m_dirty = true;
}
void set_theta_12(double rad)
{
    m_theta_12 = rad;
    m_dirty = true;
}
void set_theta_23(double rad)
{
    m_theta_23 = rad;
    m_dirty = true;
}
void set_theta_13(double rad)
{
    m_theta_13 = rad;
    m_dirty = true;
}
void set_delta_cp(double rad)
{
    m_delta_cp = rad;
    m_dirty = true;
}
