/**
 *
 * $Id: nuosc_matrix.h,v 1.4 2002-11-23 22:23:06 bviren Exp $
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

#include "matrix.h"


// Matrix builders.
ComplexMatrix cp_phase_matrix(double cp_phase);
ComplexMatrix rotation_matrix(int i, int j, double angle);
ComplexMatrix mixing_matrix(double theta12,
                            double theta23,
                            double theta13,
                            double cp_phase = 0);
ComplexMatrix mass_squared_matrix(double dm2_21, double dm2_31);
ComplexMatrix mass_state_transport_matrix(double dm2_21, // eV^2
                                          double dm2_31, // eV^2
                                          double energy, // eV
                                          double distance); // cm

#endif  // NUOSC_H
