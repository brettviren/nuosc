/**
 *
 * $Id: NuEvolverVacuum.h,v 1.3 2002-11-26 22:09:26 bviren Exp $
 *
 * \class NuEvolverVacuum
 *
 * \package libnuosc++
 *
 * \brief Differential evolution of neutrino in vacuum. 
 *
 * Note: using differential evolution to find P(nu_x->nu_y) in vacuum
 * is inefficient.
 * 
 * Contact: bv@bnl.gov
 *
 * Created on: Thu Nov 21 15:14:14 2002
 *
 */

#ifndef NUEVOLVERVACUUM_H
#define NUEVOLVERVACUUM_H

#include "NuEvolver.h"

class NuEvolverVacuum : public NuEvolver
{

public:

    NuEvolverVacuum();                
    NuEvolverVacuum(OscParam op, double energy=1.0e9, double baseline=1.0e5);

    // Hook in to changing parameters to do some pre-calculation
    virtual void set_oscparams(const OscParam& op);
    virtual void set_energy(double energy);

    /// What we are here for
    virtual ComplexVector operator()(ComplexVector nu, double x) const;


protected: 

    // This updates the evolution transform for vacuum.  This is
    // protected so the constant, non-zero matter density class can
    // get at it.
    void calculate();
    ComplexMatrix& get_transform() { return m_transform; }

private:

    virtual void set_density(double density) {}
    ComplexMatrix m_transform;  // -i/(2E)/(hbar*c)*U*M^2*Udagger
};

#endif  // NUEVOLVERVACUUM_H
