/**
 *
 * $Id: NuEvolver.h,v 1.1 2002-11-21 22:40:58 bviren Exp $
 *
 * \class NuEvolver
 *
 * \package libnuosc++
 *
 * \brief Base class for any neutrino evolution functor.
 *
 * These return the differential propagation of the given weak
 * neutrino state at some distance down a baseline.
 *
 * All set() methods are virtual to give subclasses a hook if they
 * want to pre-calculate any intermediate values.
 * 
 * Contact: bv@bnl.gov
 *
 * Created on: Thu Nov 21 13:58:32 2002
 *
 */

#ifndef NUEVOLVER_H
#define NUEVOLVER_H

#include "OscParam.h"

class NuEvolver
{

public:

    NuEvolver();
    NuEvolver(OscParam op, double energy=1.0e9, double baseline=1.0e5);
    virtual ~NuEvolver();

    /// Return the d(nu(x))/dx at the given x.
    virtual ComplexColumnVector operator()(const ComplexColumnVector& nu, double x) const = 0;

    /// Access the oscillation and other parameters.
    const OscParam& get_oscparams() const;
    virtual void set_oscparams(const OscParam& op);

    // Density.  Note: this can be changed by operator().
    double get_density(void) const;
    virtual void set_density(double density);

    // in eV
    double get_energy(void) const;
    virtual void set_energy(double energy_ev);

    // in cm
    double get_baseline(void) const;
    virtual void set_baseline(double baseline_cm);
    
private:

    OscParam m_oscparam;
    double m_energy, m_baseline, m_density;

};                              // end of class NuEvolver

#endif  // NUEVOLVER_H
