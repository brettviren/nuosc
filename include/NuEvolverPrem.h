/**
 *
 * $Id: NuEvolverPrem.h,v 1.2 2002-12-05 18:55:17 bviren Exp $
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

    NuEvolverPrem();
    NuEvolverPrem(OscParam op, double energy=1.0e9, double baseline=1.0e5);

    virtual ComplexVector operator()(ComplexVector nu, double x) const;

};                              // end of class NuEvolverPrem

#endif  // NUEVOLVERPREM_H
