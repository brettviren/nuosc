/**
 *
 * $Id: NuEvolverConstant.h,v 1.2 2002-11-26 22:09:26 bviren Exp $
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

    NuEvolverConstant(bool anti_neutrino = false);
    NuEvolverConstant(OscParam op, double energy=1.0e9, double baseline=1.0e5,
                      bool anti_neutrino = false);

    // This could be sussed from the neutrino passed into operator()()
    // but that would add to the calculation.  Just set it by hand!
    void assume_anti_neutrino(bool tf = false);

    // Rely on base class for:
    // virtual ComplexVector operator()(ComplexVector nu, double x) const;


protected:
    // override vacuum calc to add our chunk.
    void calculate();

private:
    // our chunk
    void real_calculate();
    bool m_antineutrino;
};                              // end of class NuEvolverConstant

#endif  // NUEVOLVERCONSTANT_H
