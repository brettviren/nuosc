#include "Config.h"
#include "ProbCalc.h"
#include "util.h"
#include <nuosc_prob.h>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;
int main (int argc, const char *argv[])
{
    Config config(argc,argv);

    ProbCalc* pcp = make_probability_calculator(config.calc_desc,
						config.density_desc);
    if (!pcp) config.usage("Failed to parse calculation/density descriptions");
    
    pcp->SetOscParams(config.op);
    pcp->SetInitialNu(config.nu_num);

    vector<double> en_direct, bl_direct;
    vector<double> energies = desc2vector(config.energy_desc,en_direct);
    vector<double> baselines = desc2vector(config.baseline_desc,bl_direct);

    if (!energies.size()) config.usage("Failed to parse energy description");
    if (!baselines.size()) config.usage("Failed to parse baseline description");

    ofstream info_fstr(config.info_file.c_str());
    info_fstr << config.as_string() << endl;
    if (config.cmdline_file.size()) {
	ofstream fstr(config.cmdline_file.c_str());
	fstr << config.as_string_cmdline() << endl;
    }

    ofstream data_fstr(config.data_file.c_str());

    for (size_t bli = 0; bli < baselines.size(); ++bli) {
	double baseline_cm = baselines[bli]*1.0e5; // km to cm

	for (size_t eni = 0; eni < energies.size(); ++eni) {
	    double energy_ev = energies[eni]*1.0e9; // GeV to eV

	    ComplexVector nuf = (*pcp)(energy_ev,baseline_cm);
	    vector<double> p = nuosc_amplitude_to_prob(nuf);
	    data_fstr << energies[eni] << " " << baselines[bli] << " " 
                      << p[0] << " " << p[1] << " " << p[2] << " "
                      << en_direct[eni] << " " << bl_direct[bli] 
                      << endl;
	}
    }

    return 0;
} // end of main()
