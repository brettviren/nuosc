#include "nuosc_earth.h"
#include <iostream>
#include <cstdlib>
using namespace std;

int main (int argc, char *argv[])
{
    double step = Rearth/1000;
    for (double x = 0; x <= Rearth; x+=step) {
        double density = earth_density_by_radius(x);
        cout << x << " "
             << x/Rearth << " "
             << density << " "
             << endl;
    }


    return 0;
} // end of main()
