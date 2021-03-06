#include "matrix.h"

using namespace blitz;

complex<double> phase(double phi)
{
    return complex<double>(cos(phi),sin(phi));
}

ComplexMatrix complex_conjugate(const ComplexMatrix& m)
{
    ComplexMatrix mstar(3,3);
    mstar = zip(real(m), -1*imag(m), complex<double>());
    return mstar;
}
ComplexVector complex_conjugate(const ComplexVector& v)
{
    ComplexVector vstar(3);
    vstar = zip(real(v), -1*imag(v), complex<double>());
    return vstar;
}
complex<double> complex_conjugate(const complex<double>& c)
{
    return complex<double>(real(c),-imag(c));
}
double complex_magnitude_squared(const complex<double>& c)
{
    double r = real(c), i = imag(c);
    return r*r+i*i;
}
double complex_magnitude(const complex<double>& c)
{
    return sqrt(complex_magnitude_squared(c));
}

ComplexMatrix matrix_product(const ComplexMatrix& m1, const ComplexMatrix& m2)
{
    ComplexMatrix p(3,3);
#if 1
    using namespace blitz::tensor; // i,j,k
    p = sum(m1(i,k)*m2(k,j),k);
#else
    for (int i=0; i<3; ++i) {
        for (int j=0; j<3; ++j) {
            p(i,j) = 0.0;
            for (int k=0; k<3; ++k) {
                p(i,j) += m1(i,k)*m2(k,j);
            }
        }
    }
#endif
    return p;
}

ComplexMatrix hermitian_conjugate(const ComplexMatrix& m) // dagger
{
    ComplexMatrix h = complex_conjugate(m);
    h.transposeSelf(secondDim,firstDim);
    return h;
}

double vector_magnitude(const ComplexVector& v)
{
    complex<double> s = 0;
    ComplexVector vstar = complex_conjugate(v);

    for (int i = 0; i < 3; ++i) {
        s += vstar(i)*v(i);
    }
    return real(sqrt(s));
}

complex<double> determinant(const ComplexMatrix& m)
{
    complex<double> det = 0;
    det += m(0,0)*m(1,1)*m(2,2);
    det += m(0,1)*m(1,2)*m(2,0);
    det += m(0,2)*m(1,0)*m(2,1);
    det -= m(0,1)*m(1,0)*m(2,2);
    det -= m(0,2)*m(1,1)*m(2,0);
    det -= m(0,0)*m(1,2)*m(2,1);
    return det;
}

bool tridiag_solve(vector<double>& avec, 
                          vector<double>& bvec,
                          vector<double>& cvec,
                          vector<double>& rhs,
                          vector<double>& res)
{
    vector<double>::iterator a,b,c,r;
    a = avec.begin(); b = bvec.begin(); c = cvec.begin(); r = rhs.begin();
    vector<double> gam;
    gam.push_back(0);           // first slot not used

    if (*b == 0.0) {
        cerr << "tridiag_solve: b[0] == 0\n";
        return false;
    }

    double bet = *b;
    res.push_back((*r)/bet);
    ++a;++b;++r; // not c.
    while (r != rhs.end()) {
        gam.push_back((*c)/bet);
        bet=(*b) - (*a)*gam.back();
        if (bet == 0.0) { cerr << "tridiag_solve: error \n"; return false; }
        res.push_back(((*r)-(*a)*res.back())/bet);
        ++a; ++b; ++c; ++r;
    }

    vector<double>::reverse_iterator ur = res.rbegin(), gr = gam.rbegin();
    while (gr+1 != gam.rend()) {
        *(ur+1) -= (*gr)*(*ur);
        ++ur; ++gr;
    }

    return true;
}
