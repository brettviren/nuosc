/**
 *
 * $Id: NuEvolver.h,v 1.4 2002-12-03 20:54:44 bviren Exp $
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
#include "OdeFunc.h"

class NuEvolver : public OdeFunc
{

public:

    NuEvolver();
    NuEvolver(OscParam op, double energy=1.0e9, double baseline=1.0e5);
    virtual ~NuEvolver();

    /// Subclasses should return the d(nu(x))/dx at the given x in:
    // ComplexVector operator() (const ComplexVector& nu, double x);

    /// Access the oscillation and other parameters.
    const OscParam& get_oscparams() const;
    virtual void set_oscparams(const OscParam& op);

    // in eV
    double get_energy(void) const;
    virtual void set_energy(double energy_ev);

    // in cm
    double get_baseline(void) const;
    virtual void set_baseline(double baseline_cm);
    
private:

    OscParam m_oscparam;
    double m_energy, m_baseline;

};                              // end of class NuEvolver

#endif  // NUEVOLVER_H
