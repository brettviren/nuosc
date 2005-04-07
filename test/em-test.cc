// test Earth-Matter calculations

#include "nuosc_prob.h"

int main (int argc, char *argv[])
{
    OscParam op;                // defaults

    ComplexVector nu0(3);
    nu0(0) = 0.0;
    nu0(1) = 1.0;
    nu0(2) = 0.0;

    double baseline = 2540e5;
    double energy = 1e9;
    double maxen = 10e9;
    double step = 10e6;
    double start = 100e6;
    for (energy = maxen; energy>=start; energy -= step) {
        ComplexVector amp1 = 
            nuosc_prob_prem_matrix(nu0,op,energy,baseline);

        vector<double> p1 = nuosc_amplitude_to_prob(amp1);


        cout << energy
             << " " << p1[0] << " " << p1[1] << " " << p1[2] << " "
             << endl;
        
    }

    return 0;
} // end of main()
