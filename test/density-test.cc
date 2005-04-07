#include "nuosc_prob.h"
#include "nuosc_earth.h"
#include "constants.h"

using namespace std;

int main (int argc, char *argv[])
{
    double baseline = 2540e5;
    if (argc >= 2)
	baseline = atof(argv[1]);

    double x0[earth_max_regions], xf[earth_max_regions];
    int n = earth_get_slant_distances(x0,xf,baseline);
    cerr << "Baseline = " << baseline / 1e5
	 << " km has " << n
	 << " PREM segments along baseline\n";

    
    double *length = new double[n]; // leak
    for (int ind=0; ind<n; ++ind) {
        double dx = (xf[ind]-x0[ind]);
	double dens = earth_average_region_density(x0[ind],xf[ind],baseline);
        cerr << ind << " "
             << x0[ind]/1e5 << " -- " << xf[ind]/1e5 << " is " << dx/1e5
	     << " dens=" << dens
	     << endl;
    }
    return 0;
} // end of main()
