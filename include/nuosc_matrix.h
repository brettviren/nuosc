/**
 *
 * $Id: nuosc_matrix.h,v 1.2 2002-11-20 22:34:55 bviren Exp $
 *
 * \class nuosc_matrix
 *
 * \package libnuosc++
 *
 * \brief functions returning neutrino oscilation related matrices
 *
 * Note: All angles are in radians.
 *
 * Contact: bv@bnl.gov
 *
 * Created on: Tue Oct 15 16:57:07 2002
 *
 */

#ifndef NUOSC_H
#define NUOSC_H

#include "octave.h"

// Matrix builders.
ComplexMatrix cp_phase_matrix(double cp_phase);
ComplexMatrix rotation_matrix(int i, int j, double angle);
ComplexMatrix mixing_matrix(double theta12, // theta1 in Marciano's notation
                            double theta23, // theta2 in Marciano's notation
                            double theta13, // theta3 in Marciano's notation
                            double cp_phase);
ComplexMatrix mass_squared_matrix(double dm2_sol, double dm2_atm);
ComplexMatrix mass_state_transport_matrix(double dm2sol, double dm2atm, // eV^2
                                          double energy, // eV
                                          double distance); // cm

#endif  // NUOSC_H
