#include "nuosc_prob.h"

static ComplexVector normalize(ComplexVector v)
{
    complex<double> mag = 0;

    for (int i = 0; i < 3; ++i) mag += v(i)*complex_conjugate(v(i));
    mag = sqrt(abs(mag));
    cerr << "Mag = " << mag << endl;
    ComplexVector ret(3);
    for (int i = 0; i < 3; ++i) ret(i) = v(i)/mag;
    return ret;
}

int main (int argc, char *argv[])
{
    OscParam op;                // defaults

//    op.set_dms31(op.get_dms31() + op.get_dms21());

    ComplexVector nu0(3);
    nu0(0) = 0.0;
    nu0(1) = 1.0;
    nu0(2) = 0.0;

    double baseline = 2540e5;
#if 1
    double energy = 1e9;
    double maxen = 10e9;
    double step = 10e6;
    double start = 100e6;
#else
    double energy = 1e9;
    double maxen = 10.1e9;
    double step = 10e6;
    double start = 9.9e9;
#endif
//    for (energy = start; energy<=maxen; energy += step) {
    for (energy = maxen; energy>=start; energy -= step) {
        ComplexVector amp1 = nuosc_prob_vacuum_matrix(nu0,op,energy,baseline);
        ComplexVector amp2 = nuosc_prob_vacuum_step(nu0,op,energy,baseline);

//        amp1 = normalize(amp1);
//        amp2 = normalize(amp2);

        vector<double> p1 = nuosc_amplitude_to_prob(amp1);
        vector<double> p2 = nuosc_amplitude_to_prob(amp2);


        cout << energy
             << " " << p1[0] << " " << p1[1] << " " << p1[2] << " "
             << " " << p2[0] << " " << p2[1] << " " << p2[2] << " "
             << endl;
        
    }

    return 0;
} // end of main()
