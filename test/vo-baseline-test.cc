#include "nuosc_prob.h"
#include "NuEvolverVacuum.h"
#include "constants.h"

int main (int argc, char *argv[])
{
    OscParam op;                // defaults

//    op.set_dms31(op.get_dms31() + op.get_dms21());

    ComplexVector nu0(3);
    nu0(0) = 0.0;
    nu0(1) = 1.0;
    nu0(2) = 0.0;
    ComplexVector amp2(3);
    amp2(0) = 0.0;
    amp2(1) = 1.0;
    amp2(2) = 0.0;

//    double energy = 1.15e9;
    double energy = 1.e9;

    double baseline = 2540e5;
    double start = 1e5;
    double stop = 2540e5;
    double step =   10e5;


    double oscilation_length = 3.14159*8.0*energy*hbarc/op.get_dms31();
    double prec = 1.0e-6*baseline/oscilation_length;
    double length = oscilation_length < baseline ? oscilation_length : baseline;
    NuEvolverVacuum nev(op,energy,baseline);
    nev.SetPrecision(prec);
    nev.Solve(0,baseline,1e-7*length,nu0);
//    nev.Solve(0,baseline,926772,nu0); // for 1e-10
//    nev.Solve(0,baseline,5.80592e+06,nu0); // for 1e-6
    vector<double> steps = nev.GetSteps();
    vector<ComplexVector> values = nev.GetStepValues();

    int siz = steps.size();
    for (int ind = 0; ind < siz; ++ind) {
        // Direct matrix method
        ComplexVector mamp = nuosc_prob_vacuum_matrix(nu0,op,energy,steps[ind]);
        vector<double> pm = nuosc_amplitude_to_prob(mamp);
        // Stepping method
        ComplexVector samp = values[ind];
        vector<double> ps = nuosc_amplitude_to_prob(samp);

        cout << steps[ind]
             << " " << pm[0] << " " << pm[1] << " " << pm[2] << " "
//             << " " << p2[0] << " " << p2[1] << " " << p2[2] << " "
             << " " << ps[0] << " " << ps[1] << " " << ps[2] << " "
             << endl;
    }

    return 0;
} // end of main()
