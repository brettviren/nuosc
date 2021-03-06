#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <blitz/array.h>

typedef blitz::Array<std::complex<double>,2> ComplexMatrix;
typedef blitz::Array<std::complex<double>,1> ComplexVector;

// Util functions.
std::complex<double> phase(double phi);

// solve a system of equations with a tri-diagonal matrix a'la
// N.R. a[0] and c[n-2] is not used, b is the matrix diagonal, a is
// just below, c is just above.  Return false if fail.
bool tridiag_solve(std::vector<double>& a, std::vector<double>& b, 
                   std::vector<double>& c,
                   std::vector<double>& rhs, std::vector<double>& res);


// Complex scalars
std::complex<double> complex_conjugate(const std::complex<double>& c);
double complex_magnitude_squared(const std::complex<double>& c);
double complex_magnitude(const std::complex<double>& c);


// Complex Vectors
double vector_magnitude(const ComplexVector& v);

ComplexVector complex_conjugate(const ComplexVector& v);

/// Complex Matrices
/// M*
ComplexMatrix complex_conjugate(const ComplexMatrix& m);
/// M1.M2
ComplexMatrix matrix_product(const ComplexMatrix& m1, const ComplexMatrix& m2);
/// M*t
ComplexMatrix hermitian_conjugate(const ComplexMatrix& m); // dagger
std::complex<double> determinant(const ComplexMatrix& m);
#endif  // MATRIX_H
