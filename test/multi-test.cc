

#include <iostream>

#include <blitz/array.h>

int main (int argc, char *argv[])
{
    using namespace blitz::tensor;

    blitz::Array<double,2> m1(3,3);

    m1 = 1.0+1*(i+1) + 10.0*(j+1);
    cerr << "m1=" << m1 << endl;

    return 0;
} // end of main()
