#include "nuosc_earth.h"
#include <cmath>
#include <iostream>
using namespace std;

int main (int argc, char *argv[])
{
    for (double deg=0; deg<=180; deg += 10) {
	double rad = deg/180.*M_PI;
	double bl = earth_zenith_to_baseline(rad,1);
	cerr << deg << " " << rad << " " << bl << endl;
    }


    return 0;
} // end of main()
