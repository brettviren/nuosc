#include "nuosc_prob.h"
#include "nuosc_earth.h"
#include "NuEvolverPrem.h"
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

    double density = 3.0;
    double energy = 1.e9;

    double baseline = 2540e5;
    double start = 1e5;
    double stop = 2540e5;
    double step =   10e5;


    double oscilation_length = 3.14159*8.0*energy*hbarc/op.get_dms31();
    double prec = 1.0e-13*baseline/oscilation_length;

    NuEvolverPrem nev(op,energy,baseline);
    nev.SetPrecision(prec);

    double x0[earth_max_regions], xf[earth_max_regions];
    int n = earth_get_slant_distances(x0,xf,baseline);
    
    double *length = new double[n]; // leak
    for (int ind=0; ind<n; ++ind) {
        double dx = (xf[ind]-x0[ind]);
        length[ind] = oscilation_length < dx ? oscilation_length : dx;
        cerr << ind << " "
             << x0[ind] << " -- " << xf[ind] << " is " << length[ind] << endl;
        length[ind] *= 1e-7;
    }

    nev.Solve(x0,xf,length,n,nu0);
    vector<double> steps = nev.GetSteps();
    vector<ComplexVector> values = nev.GetStepValues();

    int siz = steps.size();
    double x = 0;
    for (int ind = 0; ind < siz; ++ind) {
        double pos = steps[ind];
        double dx = pos-x;
        double density = earth_density(x,baseline);

        cerr << "x=" << x 
             << " steps[" << ind << "]=" << steps[ind]
             << " dx=" << dx
             << " density=" << density << endl;

        // Direct matrix method
        amp2 = nuosc_prob_matter_constant_matrix(amp2,op,energy,dx,density);
        vector<double> pm = nuosc_amplitude_to_prob(amp2);

        // Stepping method
        ComplexVector samp = values[ind];
        vector<double> ps = nuosc_amplitude_to_prob(samp);

        cout << x
             << " " << pm[0] << " " << pm[1] << " " << pm[2] << " "
//             << " " << p2[0] << " " << p2[1] << " " << p2[2] << " "
             << " " << ps[0] << " " << ps[1] << " " << ps[2] << " "
             << endl;
        x = pos;
    }

    return 0;
} // end of main()
