#include "nuosc_matrix.h"
#include "constants.h"

ComplexMatrix cp_phase_matrix(double phi)
{
    ComplexMatrix k(3,3);
    k = complex<double>(0,0);

    k(0,0) = complex<double>(cos(phi),sin(phi));
    k(1,1) = 1;
    k(2,2) = 1;
    return k;
}

// Note, i,j must be {1,2}, {2,3}, or {1,3} to match usual sign
// convention.

ComplexMatrix rotation_matrix(int i, int j, double angle)
{
    ComplexMatrix r(3,3);
    r = complex<double>(0,0);

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

ComplexMatrix mixing_matrix(double theta12,
                            double theta23,
                            double theta13,
                            double cp_phase)
{

    ComplexMatrix K = cp_phase_matrix(cp_phase);
    ComplexMatrix Kstar = complex_conjugate(K);
    ComplexMatrix R12 = rotation_matrix(1,2,theta12);
    ComplexMatrix R23 = rotation_matrix(2,3,theta23);
    ComplexMatrix R13 = rotation_matrix(1,3,theta13);

    // (R23)(K)(R13)(Kstar)(R12)

    ComplexMatrix u = matrix_product(Kstar,R12);
    u = matrix_product(R13,u);
    u = matrix_product(K,u);
    u = matrix_product(R23,u);
    return u;

}

ComplexMatrix mass_squared_matrix(double dm2_21, double dm2_31)
{
    ComplexMatrix m(3,3);

    // have subtracted m1^2 from diagonal.
    m(0,0) = 0;
    m(1,1) = dm2_21;
    m(2,2) = dm2_31;
    return m;
}


ComplexMatrix mass_state_transport_matrix(double dm2sol, double dm2atm,
                                          double energy, double distance)
{

    ComplexMatrix T(3,3);
    T = complex<double>(0,0);

    T(0,0) = 1.0;
    T(1,1) = exp(-0.5*EYE*dm2sol*distance/(energy*hbarc));
    T(2,2) = exp(-0.5*EYE*(dm2sol+dm2atm)*distance/(energy*hbarc));

    return T;
}

