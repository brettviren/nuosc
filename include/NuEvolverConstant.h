/**
 *
 * $Id: NuEvolverConstant.h,v 1.4 2002-12-05 18:55:17 bviren Exp $
 *
 * \class NuEvolverConstant
 *
 * \package libnuosc++
 *
 * \brief Differential evolution of neutrino in constant density Earth matter. 
 *
 * Giving density of 0.0 should reproduce the case of vacuum
 * oscillations or use NuEvolverVacuum for slight optimization.  
 *
 * Note: using differential evolution to find P(nu_x->nu_y) in
 * constant density is inefficient.
 *
 * Contact: bv@bnl.gov
 *
 * Created on: Thu Nov 21 15:14:14 2002
 *
 */

#ifndef NUEVOLVERCONSTANT_H
#define NUEVOLVERCONSTANT_H

#include "NuEvolverVacuum.h"

class NuEvolverConstant : public NuEvolverVacuum
{

public:

    NuEvolverConstant(double density=0);
    NuEvolverConstant(OscParam op, double energy=1.0e9, double baseline=1.0e5,
                      double density = 0);

    // Density.  Note: this can be changed by operator().
    double get_density(void) const;
    virtual void set_density(double density);

    // Rely on base class for:
    // virtual ComplexVector operator()(ComplexVector nu, double x) const;

protected:
    // override vacuum calc to add our chunk.
    void calculate();

private:
    // our chunk
    void real_calculate();
    double m_density;
};                              // end of class NuEvolverConstant

#endif  // NUEVOLVERCONSTANT_H
