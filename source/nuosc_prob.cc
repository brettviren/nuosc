#include "nuosc_prob.h"
#include "nuosc_matrix.h"
#include "constants.h"
#include "NuEvolverVacuum.h"

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
    double oscilation_length = 3.14159*8.0*energy*hbarc/op.get_dms31();

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
    double prec = 1.0e-13*baseline/oscilation_length;

    double length = oscilation_length < baseline ? oscilation_length : baseline;

    cerr << oscilation_length << "cm,  prec = " << prec << endl;
    NuEvolverVacuum nev(op,energy,baseline);
    nev.SetPrecision(prec);
    return nev.Solve(0,baseline,1e-7*length,initial_neutrino);
}
