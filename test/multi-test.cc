

#include <iostream>
using namespace std;

#include <blitz/array.h>
using namespace blitz;

int main (int argc, char *argv[])
{
    using namespace blitz::tensor;

    Array<double,2> m1(3,3), m2(3,3), m3(3,3);

    m1 = 1.0+1*(i+1) + 10.0*(j+1);
    m2 = 2.0+1*(i+1) + 2.0*(j+1);
    m3 = (i+1)*(j+1);

    cerr << "m1=" << m1 << endl;
    cerr << "m2=" << m2 << endl;
    cerr << "m3=" << m3 << endl;

    Array<double,2> m4 (3,3);

    m4 = sum(sum(m1(i,l)*m2(l,k),l)*m3(k,j),k);

    cerr << "m4=" << m4 << endl;

    return 0;
} // end of main()
