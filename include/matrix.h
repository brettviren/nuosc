#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <blitz/array.h>

typedef blitz::Array<complex<double>,2> ComplexMatrix;
typedef blitz::Array<complex<double>,1> ComplexVector;

// Util functions.
complex<double> phase(double phi);

// solve a system of equations with a tri-diagonal matrix a'la
// N.R. a[0] and c[n-2] is not used, b is the matrix diagonal, a is
// just below, c is just above.  Return false if fail.
bool tridiag_solve(vector<double>& a, vector<double>& b, vector<double>& c,
                   vector<double>& rhs, vector<double>& res);


// Complex scalars
complex<double> complex_conjugate(complex<double> c);


// Complex Vectors
double vector_magnitude(ComplexVector v);

ComplexVector complex_conjugate(ComplexVector v);

// Complex Matrices
ComplexMatrix complex_conjugate(ComplexMatrix m);
ComplexMatrix matrix_product(ComplexMatrix m1, ComplexMatrix m2);
ComplexMatrix hermitian_conjugate(ComplexMatrix m); // dagger

#endif  // MATRIX_H
