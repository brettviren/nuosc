#include "nuosc_prob.h"

int main (int argc, char *argv[])
{
    OscParam op;                // defaults

//    op.set_dms31(op.get_dms31() + op.get_dms21());

    ComplexVector nu0(3);
    nu0(0) = 0.0;
    nu0(1) = 1.0;
    nu0(2) = 0.0;

    double baseline = 2540e5;
    double energy = 1e9;
    if (argc >= 2) baseline = atof(argv[1])*1e5;
    if (argc >= 3) energy = atof(argv[2])*1e9;

    ComplexVector amp1 = nuosc_prob_vacuum_matrix(nu0,op,energy,baseline);
    ComplexVector amp2 = nuosc_prob_vacuum_step(nu0,op,energy,baseline);
    vector<double> p1 = nuosc_amplitude_to_prob(amp1);
    vector<double> p2 = nuosc_amplitude_to_prob(amp2);


    cout << "baseline = " << baseline/1e5 << " km\n"
         << "energy = " << energy/1e9 << " GeV\n"
         << "Matrix P = "
         << " " << p1[0] << " " << p1[1] << " " << p1[2] << endl
         << "Stepped P = "
         << " " << p2[0] << " " << p2[1] << " " << p2[2] << endl;

    return 0;
} // end of main()
