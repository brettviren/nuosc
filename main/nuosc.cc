#include "Config.h"
#include "ProbCalc.h"
#include "util.h"
#include <nuosc_prob.h>
#include <vector>
#include <iostream>
using namespace std;
int main (int argc, const char *argv[])
{
    Config config(argc,argv);

    ProbCalc* pcp = make_probability_calculator(config.calc_desc,
						config.density_desc);
    if (!pcp) config.usage("Failed to parse calculation/density descriptions");
    
    pcp->SetOscParams(config.op);
    pcp->SetInitialNu(config.nu_num);

    vector<double> energies = desc2vector(config.energy_desc);
    vector<double> baselines = desc2vector(config.baseline_desc);

    for (size_t bli = 0; bli < baselines.size(); ++bli) {
	double baseline = baselines[bli]*1.0e5;	// km to cm
	for (size_t eni = 0; eni < energies.size(); ++eni) {
	    double energy = energies[eni]*1.0e9; // GeV to eV

	    ComplexVector nuf = (*pcp)(energy,baseline);
	    vector<double> p = nuosc_amplitude_to_prob(nuf);
	    cout << energies[eni] << " " << baselines[bli] << " " 
		 << p[0] << " " << p[1] << " " << p[2] << endl;
	}
    }

    return 0;
} // end of main()
