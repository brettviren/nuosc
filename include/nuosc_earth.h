////////////////////////////////////////////////////////////////////////
//
// $Id: nuosc_earth.h,v 1.5 2005-01-12 21:37:12 bviren Exp $
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

// The max number of regions before going 1/2 the track (8)
extern const int earth_max_radius_number;
// The maximum number of density regions that a neutrino could
// traverse (2*8-1);
extern const int earth_max_regions;
// Nominal radius of the earth in cm
extern const double Rearth;

// Convert a distance (d) along a baseline (D) into earth radius.
double earth_dist_to_radius(double d, double D);
// vice versa.
double earth_radius_to_dist_start(double r, double D);
double earth_radius_to_dist_end(double r, double D);

// Return the baseline from the Earth's surface to a detector at given
// depth coresponding to the given zenith angle in radians
double earth_zenith_to_baseline(double zrad, double depth=0.0/*cm*/);

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

// Return the average density along baseline D between points x0 and
// xf.  This assumes that the density is linear between its values at
// these two points.  It also assumes x0 and xf are the begin and
// ending of a region (ie as returned from next function below).
// However, it will work in the case that x0 and xf spans less than a
// region as long as any central region is spanned symmetrically.
double earth_average_region_density(double x0, double xf, double D);

// Find the start and stop points to just avoid discontinuities in the
// earths density profile for given secant D.  Returns the number of
// segments (at most 15).
int earth_get_slant_distances(double x0[], double xf[], double D);
int earth_radius_number(double D);

#endif  // EARTH_H
