#include <assert.h>
#include "nuosc_earth.h"

const double Rearth = 6371.0e5; // in cm

// Some earth related functions.  In all, d=dist along base line,
// D=baseline

double earth_dist_to_radius(double d, double D)
{
    return sqrt(Rearth*Rearth + d*d - d*D);
}
// Before mid point of baseline
double earth_radius_to_dist_start(double r, double D)
{
    if (fabs(r-Rearth) < 1) return 0;
    return 0.5*D - sqrt(r*r + 0.25*D*D - Rearth*Rearth);
}
// After mid point of baseline
double earth_radius_to_dist_end(double r, double D)
{
    if (fabs(r-Rearth) < 1) return D;
    return 0.5*D + sqrt(r*r + 0.25*D*D - Rearth*Rearth);
}

// Return Y_e, the electron fraction.  This is based on a crude
// estimation using numbers from hep-ph/0002149.
double earth_electron_fraction(double d, double D)
{
    double rad = earth_dist_to_radius(d,D);
    if (rad < 3480.0e5) return 0.466; // core
    if (rad >= 3480e5 && rad < 6346.6e5) return 0.494; // mantle
    return 0.5;                 // crust
}
double earth_electron_fraction_by_density(double density)
{
    if (density >= 9.90) return 0.466; // core: rad < 3480e5 cm
    if (density >= 3.38) return 0.494; // mantle: rad < 6346.6e5 cm
    return 0.5; // crust
}

// Return Earth density in g/cm^3 according to table 1 in hep-ph/0002149.
// based on http://geowww.gcn.ou.edu/~jahern/solid_earth/prem.html

double earth_density(double d, double D) 
{
    double rad = earth_dist_to_radius(d,D);
    return earth_density_by_radius(rad);
}
double earth_density_by_radius(double rad)
{
    double radrel = rad/Rearth;

//    cerr << "Rad = " << rad << endl;

    if (rad < 1221.5e5) 
        return 13.0885-8.8381*radrel*radrel;
    if (rad >= 1221.5e5 && rad < 3480.0e5) 
        return 12.5815 - 1.2638*radrel - 3.6426*radrel*radrel - 5.5281*radrel*radrel*radrel;
    if (rad >= 3480.0e5 && rad < 5701.0e5)
        return 7.9565 - 6.4761*radrel + 5.5283*radrel*radrel - 3.0807 *radrel*radrel*radrel;
    if (rad >= 5701.0e5 && rad < 5771.0e5)
        return 5.3197 - 1.4836*radrel;
    if (rad >= 5771.0e5 && rad < 5971.0e5)
        return 11.2494 - 8.0298*radrel;
    if (rad >= 5971.0e5 && rad < 6151.0e5)
        return 7.1089 - 3.8045*radrel;
    if (rad >= 6151.0e5 && rad < 6346.6e5)
        return 2.6910 + 0.6924*radrel;
    if (rad >= 6346.6e5 && rad < 6356.0e5)
        return 2.900;
    if (rad >= 6356.0e5 && rad < 6368.0e5)
        return 2.6;
//    return 1.020;                // water
    return 2.6;                 // ignore water.
}

const int earth_nradii = 9;
const double  earth_region_radii[earth_nradii] = {
    6371.0e5,                   // surface radius
    6356.0e5,
    6346.6e5,                   // Begin of crust
    6151.0e5,
    5971.0e5,
    5771.0e5,
    5701.0e5,
    3480.0e5,                   // Begin of mantle
    1221.5e5
};

int earth_radius_number(double D)
{
    int nradii;
    double Rsineta = sqrt(Rearth*Rearth - 0.25*D*D);
    for(nradii = 0; nradii < earth_nradii-1; ++nradii) {
        if (earth_region_radii[nradii] > Rsineta &&
            earth_region_radii[nradii+1] < Rsineta) return nradii;
    }
    return -1;
}

int earth_get_slant_distances(double x0[], double xf[], double D)
{
    const double epsilon = 1.0e-5;

    int radius_number = earth_radius_number(D);
    if (radius_number < 0) return radius_number;
    assert(radius_number < 8);

    int ind=0;
    for (int n = 0; n < radius_number; ++n, ++ind) {
        x0[ind] = earth_radius_to_dist_start(earth_region_radii[n],D)+epsilon;
        xf[ind] = earth_radius_to_dist_start(earth_region_radii[n+1],D)-epsilon;
    }
    x0[ind] = earth_radius_to_dist_start(earth_region_radii[ind],D)+epsilon;
    xf[ind] = earth_radius_to_dist_end  (earth_region_radii[ind],D)-epsilon;
    ++ind;
    for (int n = radius_number; n > 0; --n, ++ind) {
        x0[ind] = earth_radius_to_dist_end(earth_region_radii[n],D)+epsilon;
        xf[ind] = earth_radius_to_dist_end(earth_region_radii[n-1],D)-epsilon;
    }

    return ind;
}
