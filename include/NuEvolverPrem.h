/**
 *
 * $Id: NuEvolverPrem.h,v 1.1 2002-12-04 22:00:44 bviren Exp $
 *
 * \class NuEvolverPrem
 *
 * \package libnuosc++
 *
 * \brief Differential evolution of neutrino in PREM Earth matter
 * density profile.
 *
 * This uses the info in table 1 in hep-ph/0002149 based on the
 * Preliminary Reference Earth Model at
 * http://solid_earth.ou.edu/prem.html
 *
 * Contact: bv@bnl.gov
 *
 * Created on: Wed Dec  4 12:29:20 2002
 *
 */

#ifndef NUEVOLVERPREM_H
#define NUEVOLVERPREM_H

#include "NuEvolverVacuum.h"

class NuEvolverPrem : public NuEvolverVacuum
{

public:

    NuEvolverPrem(bool anti_neutrino = false);
    NuEvolverPrem(OscParam op, double energy=1.0e9, double baseline=1.0e5,
                  bool anti_neutrino = false);

    // This could be sussed from the neutrino passed into operator()()
    // but that would add to the calculation.  Just set it by hand!
    void assume_anti_neutrino(bool tf = false);

    virtual ComplexVector operator()(ComplexVector nu, double x) const;

private:
    bool m_antineutrino;
};                              // end of class NuEvolverPrem

#endif  // NUEVOLVERPREM_H
