#include "nuosc_prob.h"
#include "nuosc_matrix.h"
#include "nuosc_earth.h"
#include "constants.h"
#include "NuEvolverVacuum.h"
#include "NuEvolverConstant.h"
#include "NuEvolverPrem.h"
#include <cmath>                // for M_PI

vector<double> nuosc_amplitude_to_prob(ComplexVector amp)
{
    vector<double> p;
    for (int ii=0; ii<3; ++ii) 
        p.push_back(abs(amp(ii)*complex_conjugate(amp(ii))));
    return p;
}

ComplexVector nuosc_prob_vacuum_matrix(ComplexVector initial_neutrino,
                                       const OscParam& op,
                                       double energy, double baseline)
{
    complex<double> param = -0.5*EYE*baseline/(energy*hbarc);

    ComplexVector msqrd(3);
    msqrd(0) = 1.0;
    msqrd(1) = exp(param*op.get_dms21());
    msqrd(2) = exp(param*op.get_dms31());

    ComplexMatrix U = op.get_mixing_matrix();
    ComplexMatrix Udagger = hermitian_conjugate(U);
//    cerr << "U=\n" << U << "\nUdagger=\n" << Udagger << endl;

    using namespace blitz::tensor; // for i,j.

    // (U)exp(-i/(2E*hbarc) (M^2) x)(Udagger)
    ComplexMatrix T(3,3);

    T = sum(U(i,k)*Udagger(k,j)*msqrd(k),k);

//    cerr << "Transport matrix:\n" << T << endl;

  // Apply neutrino
    ComplexVector vec(3);
    vec = sum(T(i,j)*initial_neutrino(j),j);
//    cerr << vec(0) << " " << vec(1) << " " << vec(2) << endl;
    return vec;
}

ComplexVector nuosc_prob_vacuum_step(ComplexVector initial_neutrino,
                                     const OscParam& op,
                                     double energy, double baseline)
{
    double oscilation_length = M_PI*8.0*energy*hbarc/op.get_dms31();

    // Target precision.  
    //
    // 1e-13 gives < 1% absolute numu->numu and
    // <0.1% absolute error for numu->nue and 2m10 for ~1000 points
    // from 100MeV to 10 Gev for 2540 km.  
    //
    // 1e-10 gives < 3% and < 0.5% in 35sec for same.
    //
    // The error is systematic in that it mimics a smaller L/E.  ie
    // you will get a probability at L/E which is really for L/E -
    // eps.  I think this is due to accumulative uncertainties in
    // stepping.
    double prec = 1.0e-8*baseline/oscilation_length;

    double length = oscilation_length < baseline ? oscilation_length : baseline;

//    cerr << oscilation_length << "cm,  prec = " << prec << endl;
    NuEvolverVacuum nev(op,energy,baseline);
    nev.SetPrecision(prec);
    return nev.Solve(0,baseline,1e-7*length,initial_neutrino);
}


// This is taken from 
// Ohlsson & Snellman J. Math. Phys. 41, No 5 May 2000
// hep-ph/9910564
ComplexMatrix constant_density_evolution_matrix(const OscParam& op,
                                                double energy,
                                                double distance,
                                                double density)
{
    // Calculate the T matrix:

    // A = sqrt(2) GF Ne [=7.6e-14*Y*rho eV when rho = g/cm^3]
    double A = 7.6e-14*density*earth_electron_fraction_by_density(density);
    // Convert from eV to 1/cm
    A /= hbarc;

    double phi21 = op.get_dms21() / (2.0 * energy * hbarc);
    double phi31 = op.get_dms31() / (2.0 * energy * hbarc);
    double phi32 = phi31 - phi21;

    ComplexMatrix mm = op.get_matter_matrix();
    ComplexMatrix T(3,3);
    T = A * mm;
    T(0,0) += (-phi21 + -phi31) / 3.0;
    T(1,1) += (-phi32 +  phi21) / 3.0;
    T(2,2) += ( phi31 +  phi32) / 3.0;

    // Calculate the c0, c1 coefficients (c2 = Trace(T) = 0, by def)

    complex<double> cc0 = -determinant(T);

    complex<double> cc1 =
        T(0,0)*T(1,1) + T(1,1)*T(2,2) + T(0,0)*T(2,2)
        - T(0,1)*T(1,0) - T(1,2)*T(2,1) - T(0,2)*T(2,0);

    double c0 = real(cc0);
    double c1 = real(cc1);

    // Calculate lambdas

    double arg = sqrt(-c0*c0 -4./27.*c1*c1*c1)/c0;
    double a = atan(arg)/3.0;
    if (c0 >= 0) a += M_PI;
    double b = sqrt(-c1);
    const double sqrt1over3 = sqrt(1.0/3.0);
    double bsina = b*sin(a);
    double bcosasqrt1over3 = sqrt1over3*b*cos(a);

    vector<double> lambda;

    double l1 = -bcosasqrt1over3 + bsina;
    double l2 = -bcosasqrt1over3 - bsina;
    double l3 = 2.*bcosasqrt1over3;

    lambda.push_back(l1);
    lambda.push_back(l2);
    lambda.push_back(l3);
    
    ComplexMatrix U = op.get_mixing_matrix();
    ComplexMatrix Udagger = hermitian_conjugate(U);

    ComplexMatrix Twiddle = matrix_product(T,Udagger);
    Twiddle = matrix_product(U,Twiddle);

    ComplexMatrix Twiddle_s = matrix_product(T,T);
    Twiddle_s = matrix_product(Twiddle_s,Udagger);
    Twiddle_s = matrix_product(U,Twiddle_s);


    // There is a phase, exp(-iL * trace(H_m)/3) which we ignore
    
    ComplexMatrix Uf(3,3), IDENT(3,3);
    Uf = 0.0;
    IDENT = 0;
    IDENT(0,0) = 1.0;
    IDENT(1,1) = 1.0;
    IDENT(2,2) = 1.0;
    for (int ii = 0; ii < 3; ++ii) {
        double ls = lambda[ii]*lambda[ii];
        Uf += exp(-EYE*distance*lambda[ii])
            / (3.0*ls + c1)
            * ((ls + c1)*IDENT + lambda[ii]*Twiddle + Twiddle_s);
    }

    return Uf;
}

ComplexVector nuosc_prob_matter_constant_matrix(ComplexVector initial_neutrino,
                                                const OscParam& op,
                                                double energy, double baseline,
                                                double density)
{
    ComplexMatrix Uf = 
        constant_density_evolution_matrix(op,energy,baseline,density);
    ComplexVector vec(3);

    // (Uf)(nu0)

    using namespace blitz::tensor; // for i,j.
    vec = sum(Uf(i,j)*initial_neutrino(j),j);
    return vec;
}

ComplexVector nuosc_prob_matter_constant_step(ComplexVector initial_neutrino,
                                              const OscParam& op,
                                              double energy, double baseline,
                                              double density)
{
    double oscilation_length = M_PI*8.0*energy*hbarc/op.get_dms31();

    // Target precision.  
    //
    // 1e-13 gives < 1% absolute numu->numu and
    // <0.1% absolute error for numu->nue and 2m10 for ~1000 points
    // from 100MeV to 10 Gev for 2540 km.  
    //
    // 1e-10 gives < 3% and < 0.5% in 35sec for same.
    //
    // The error is systematic in that it mimics a smaller L/E.  ie
    // you will get a probability at L/E which is really for L/E -
    // eps.  I think this is due to accumulative uncertainties in
    // stepping.
    double prec = 1.0e-10*baseline/oscilation_length;

    double length = oscilation_length < baseline ? oscilation_length : baseline;

//    cerr << oscilation_length << "cm,  prec = " << prec << endl;
    NuEvolverConstant nev(op,energy,baseline,density);
    assert (fabs(nev.get_density()-density) < 1e15);
    nev.SetPrecision(prec);
    return nev.Solve(0,baseline,1e-7*length,initial_neutrino);
}

// Jump the neutrino state through the earth in pieces assuming
// average density across jumps.  Each jump takes the neutrino across
// a region of continuous earth density profile.
ComplexVector nuosc_prob_matter_earth_matrix_piecewise(ComplexVector initial_neutrino,
                                                       const OscParam& op,
                                                       double energy, double baseline)
{
    double x0[earth_max_regions], xf[earth_max_regions];

    int n = earth_get_slant_distances(x0,xf,baseline);

    ComplexVector vec(3);
    vec = initial_neutrino;
    for (int i=0; i < n; ++i) {
        double density = earth_average_region_density(x0[i],xf[i],baseline);
//        cerr << "density = " << density << endl;

        vec = nuosc_prob_matter_constant_matrix(vec,op,energy,xf[i]-x0[i],density);
    }
//    cerr << "nu=" << vec << endl;
    return vec;
}

// Full detailed matter density profile, stepped
ComplexVector nuosc_prob_matter_earth_step(ComplexVector initial_neutrino,
                                           const OscParam& op,
                                           double energy, double baseline)
{
    double oscilation_length = M_PI*8.0*energy*hbarc/op.get_dms31();
    double prec = 1.0e-13*baseline/oscilation_length;
    double length = oscilation_length < baseline ? oscilation_length : baseline;

//    cerr << oscilation_length << "cm,  prec = " << prec << endl;
    NuEvolverPrem nev(op,energy,baseline);
    nev.SetPrecision(prec);
    return nev.Solve(0,baseline,1e-7*length,initial_neutrino);
}
