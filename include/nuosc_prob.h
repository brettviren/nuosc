/**
 *
 * $Id: nuosc_prob.h,v 1.1 2002-11-26 22:09:26 bviren Exp $
 *
 * \class nuosc_prob
 *
 * \package libnuosc++
 *
 * \brief Calculate neutrino oscillation probabilities in various ways.
 *
 * Contact: bv@bnl.gov
 *
 * Created on: Mon Nov 25 11:57:49 2002
 *
 */

#ifndef NUOSC_PROB_H
#define NUOSC_PROB_H

#include "matrix.h"
#include "OscParam.h"
#include <vector>

/// Return |(amp)(amp*)|
vector<double> nuosc_amplitude_to_prob(ComplexVector amp);


/// Probability for no matter, calculated with analytically:
/// nu(x=l) = U*exp(-i/(2E)*M^2*l/hbarc)*Udagger]nu(x=0)

ComplexVector nuosc_prob_vacuum_matrix(ComplexVector initial_neutrino,
                                       const OscParam& op,
                                       double energy, double baseline);

/// Probability for no matter, calculated by stepping the evolution
/// equation.
ComplexVector nuosc_prob_vacuum_step(ComplexVector initial_neutrino,
                                     const OscParam& op,
                                     double energy, double baseline);

#endif  // NUOSC_PROB_H
