/**
 *
 * $Id: nuosc_prob.h,v 1.7 2003-07-07 22:20:54 bviren Exp $
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
std::vector<double> nuosc_amplitude_to_prob(ComplexVector amp);


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

ComplexVector nuosc_prob_constant_matrix(ComplexVector initial_neutrino,
                                         const OscParam& op,
                                         double energy, double baseline,
                                         double density);
ComplexVector nuosc_prob_constant_step(ComplexVector initial_neutrino,
                                       const OscParam& op,
                                       double energy, double baseline,
                                       double density);

/// Return the matrix U_m [ie. nu_m(x) = U_m*nu_m(0)] which evolves a
/// MASS eigenstate of given energy over given distance through matter
/// of given density and with corresponding vacuum oscillation
/// parameters.
ComplexMatrix constant_density_evolution_matrix(const OscParam& op,
                                                double energy,
                                                double distance,
                                                double density);

// Jump the neutrino state through the earth in pieces assuming
// average density across jumps.  Each jump takes the neutrino across
// a region of continuous earth density profile.
ComplexVector nuosc_prob_prem_matrix(ComplexVector initial_neutrino,
                                     const OscParam& op,
                                     double energy, double baseline);

// Full PREM matter density profile
ComplexVector nuosc_prob_prem_step(ComplexVector initial_neutrino,
                                   const OscParam& op,
                                   double energy, double baseline);


#endif  // NUOSC_PROB_H
