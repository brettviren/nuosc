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
void print(ComplexMatrix m)
{
    cerr << "[ ";
    for(int i=0; i<3; ++i) {
        for (int j=0; j<3; ++j)
            cerr << real(m(i,j)) << "+" << imag(m(i,j)) << "i ";
        if (i < 2)
            cerr << endl;
    }
    cerr << " ]\n";
}

int main (int argc, char *argv[])
{
    OscParam op;                // defaults

//    op.set_dms31(op.get_dms31() + op.get_dms21());

    cerr << "mixing = \n";
    print(op.get_mixing_matrix());
    cerr << "matter = \n";
    print(op.get_matter_matrix());

    ComplexVector nu0(3);
    nu0(0) = 0.0;
    nu0(1) = 1.0;
    nu0(2) = 0.0;

    double baseline = 2540e5;
    double energy = 3e9;
    double density = 3.0;
    double maxden = 10.;
    double step = 0.1;
    double start = 0.;

    for (density = maxden; density>=start; density -= step) {
        if (fabs(density) < 1.0e-10) density = 0.0;
        ComplexVector amp1 = nuosc_prob_vacuum_matrix(nu0,op,energy,baseline);
        ComplexVector amp2 = 
            nuosc_prob_constant_matrix(nu0,op,energy,
                                       baseline,density);

//        amp1 = normalize(amp1);
//        amp2 = normalize(amp2);

        vector<double> p1 = nuosc_amplitude_to_prob(amp1);
        vector<double> p2 = nuosc_amplitude_to_prob(amp2);


        cout << density
             << " " << p1[0] << " " << p1[1] << " " << p1[2] << " "
             << " " << p2[0] << " " << p2[1] << " " << p2[2] << " "
             << endl;
        
    }

    return 0;
} // end of main()
