#include "nuosc.h"

using namespace blitz;

ComplexMatrix cp_phase_matrix(double cp_phase)
{
    ComplexMatrix k(Range(1,3),Range(1,3));
    k = 0;
    k(1,1) = phase(cp_phase);
    k(2,2) = 1;
    k(3,3) = 1;
    return k;
}

// Note, i,j must be {1,2}, {2,3}, or {1,3} to match usual sign
// convention.

ComplexMatrix rotation_matrix(int i, int j, double angle)
{
    ComplexMatrix r(Range(1,3),Range(1,3));
    r=0;
    int k = 6-i-j;

    r(i,i) = cos(angle);
    r(j,j) = cos(angle);
    
    r(i,j) = sin(angle);
    r(j,i) = -sin(angle);
    
    r(k,k) = 1;

    return r;
}

ComplexMatrix mixing_matrix(double theta12, // theta1 in Marciano's notation
                            double theta23, // theta2 in Marciano's notation
                            double theta13, // theta3 in Marciano's notation
                            double cp_phase)// all angles in radians.
{
//    cerr << "mixing_matrix("
//         << theta12 << "," 
//         << theta23 << "," 
//         << theta13 << "," 
//         << cp_phase << "): ";

    ComplexMatrix k = cp_phase_matrix(cp_phase);
    ComplexMatrix kstar = complex_conjugate(k);
    ComplexMatrix r12 = rotation_matrix(1,2,theta12);
    ComplexMatrix r23 = rotation_matrix(2,3,theta23);
    ComplexMatrix r13 = rotation_matrix(1,3,theta13);

    ComplexMatrix u = matrix_product(kstar,r12);
    u = matrix_product(r13,u);
    u = matrix_product(k,u);
    u = matrix_product(r23,u);

//    cerr << u << endl;
    return u;
}

// dm2_sol = dm2_21, dm2_atm = dm2_32
ComplexMatrix mass_squared_matrix(double dm2_sol, double dm2_atm)
{
    ComplexMatrix m(Range(1,3),Range(1,3));
    m=0;
    // have subtracted m1^2 from diagonal.
    m(1,1) = 0;
    m(2,2) = dm2_sol;
    m(3,3) = dm2_atm + dm2_sol;
    return m;
}


ComplexMatrix mass_state_transport_matrix(double dm2sol, double dm2atm,
                                          double energy, double distance)
{
    // hbar*c in eV*cm
    const double hbarc = 1.9732696e-05;
    const complex<double> EYE(0,1);

    ComplexMatrix T(Range(1,3),Range(1,3));
    T = 0.0;

    T(1,1) = 1.0;
    T(2,2) = exp(-0.5*EYE*dm2sol*distance/(energy*hbarc));
    T(3,3) = exp(-0.5*EYE*(dm2sol+dm2atm)*distance/(energy*hbarc));

    return T;
}

