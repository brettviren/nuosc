////////////////////////////////////////////////////////////////////////
//
// $Id: nuosc_earth.h,v 1.2 2002-11-21 22:40:59 bviren Exp $
//
// earth
//
// Package: elbo
//
// Some routines to return physical values about the earth
//
// Contact: bv@bnl.gov
//
// Created on: Mon Oct  8 16:48:10 2001
//
////////////////////////////////////////////////////////////////////////


#ifndef EARTH_H
#define EARTH_H

// Convert a distance (d) along a baseline (D) into earth radius.
double earth_dist_to_radius(double d, double D);
// vice versa.
double earth_radius_to_dist_start(double r, double D);
double earth_radius_to_dist_end(double r, double D);

// Return the earth density at a distance (d) along a baseline (D),
// uses the above.
double earth_density(double d, double D);
double earth_density_by_radius(double rad);

// Return Y_e, the electron fraction.  This is based on a crude
// estimation using numbers from hep-ph/0002149.
double earth_electron_fraction(double d, double D);
// optimization of above.  Makes use of monotonic decrease of density
// with radius.
double earth_electron_fraction_by_density(double density);

int earth_get_slant_distances(double x0[], double xf[], double D);
int earth_radius_number(double D);

#endif  // EARTH_H
