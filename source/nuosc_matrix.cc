#include "nuosc_matrix.h"

ComplexMatrix cp_phase_matrix(double phi)
{
    ComplexMatrix k(3,3,complex<double>(0.0,0.0));

    k(0,0) = complex<double>(cos(phi),sin(phi));
    k(1,1) = 1;
    k(2,2) = 1;
    return k;
}

// Note, i,j must be {1,2}, {2,3}, or {1,3} to match usual sign
// convention.

ComplexMatrix rotation_matrix(int i, int j, double angle)
{
    ComplexMatrix r(3,3,complex<double>(0.0,0.0));

    int k = 6-i-j;

    i -= 1;
    j -= 1;
    k -= 1;

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
    ComplexMatrix kstar = conj(k);
    ComplexMatrix r12 = rotation_matrix(1,2,theta12);
    ComplexMatrix r23 = rotation_matrix(2,3,theta23);
    ComplexMatrix r13 = rotation_matrix(1,3,theta13);

    ComplexMatrix u = kstar*r12;
    u = r13*u;
    u = k*u;
    u = r23*u;

//    cerr << u << endl;
    return u;
}

// dm2_sol = dm2_21, dm2_atm = dm2_32
ComplexMatrix mass_squared_matrix(double dm2_sol, double dm2_atm)
{
    ComplexMatrix m(3,3,complex<double>(0.0,0.0));

    // have subtracted m1^2 from diagonal.
    m(0,0) = 0;
    m(1,1) = dm2_sol;
    m(2,2) = dm2_atm + dm2_sol;
    return m;
}


ComplexMatrix mass_state_transport_matrix(double dm2sol, double dm2atm,
                                          double energy, double distance)
{
    // hbar*c in eV*cm
    const double hbarc = 1.9732696e-05;
    const complex<double> EYE(0,1);

    ComplexMatrix T(3,3,complex<double>(0.0,0.0));

    T(0,0) = 1.0;
    T(1,1) = exp(-0.5*EYE*dm2sol*distance/(energy*hbarc));
    T(2,2) = exp(-0.5*EYE*(dm2sol+dm2atm)*distance/(energy*hbarc));

    return T;
}

