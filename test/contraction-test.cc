
#include <iostream>
using namespace std;

#include <blitz/array.h>
using namespace blitz;
using namespace blitz::tensor;

int main (int argc, char *argv[])
{
    Array<double,1> nu(3), res(3);
    nu = 0., 1., 0.;
    cout << nu << endl;

    Array<double,2> mm(3,3);
    mm = 
        1., 2., 3., 
        4., 5., 6.,
        7., 8., 9.;
    cout << mm << endl;

    res = sum(mm(i,j)*nu(j), j);
    cout << res << endl;

    return 0;
} // end of main()
