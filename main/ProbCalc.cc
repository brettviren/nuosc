#include "ProbCalc.h"
#include <nuosc_prob.h>
#include <fstream>
using namespace std;


// ProbCalc base class.

ProbCalc::ProbCalc() :nu0(3) {}
ProbCalc::~ProbCalc() {}

void ProbCalc::SetInitialNu(int nu_num)
{
    ComplexVector nu(3);
    nu = 0.;
    nu(abs(nu_num)-1) = 1.;
    nu0 = nu;
    op.set_antineutrino(nu_num < 0);
}

void ProbCalc::SetOscParams(const OscParam& oscparams)
{ op = oscparams; }
   
const ComplexVector& ProbCalc::GetInitialNu(void) { return nu0; }
const OscParam& ProbCalc::GetOscParams(void) { return op; }



// Prem calculator

class PCPremCalc : public ProbCalc {
    ComplexVector (*func)(ComplexVector initial_neutrino,
			  const OscParam& op,
			  double energy, double baseline);

public:
    PCPremCalc(string calc_type) {
	if (calc_type == "matrix") func = nuosc_prob_prem_matrix;
	else func = nuosc_prob_prem_step;
    }
    ComplexVector operator()(double energy, double baseline) {
	return func(nu0,op,energy,baseline);
    }
};

// Constant density calculator

class PCConCalc : public ProbCalc {
    ComplexVector (*func)(ComplexVector initial_neutrino,
			  const OscParam& op,
			  double energy, double baseline, double density);
    double density;

public:

    PCConCalc(string calc_type, double dens) : density(dens) {
	if (calc_type == "matrix") {
	    if (dens < 0.0) {
		func = nuosc_prob_vacuum_matrix;
	    }
	    else {
		func = nuosc_prob_constant_matrix;
	    }
	}
	else {
	    if (dens < 0.0) {
		func = nuosc_prob_vacuum_step;
	    }
	    else {
		func = nuosc_prob_constant_step;
	    }
	}
    }
    ComplexVector operator()(double energy, double baseline) {
	return func(nu0,op,energy,baseline,density);
    }
};    

// Density lookup tables calculator

class PCLutCalc : public ProbCalc {
    vector<double> poslu, denlu;

public:
    PCLutCalc(string /*ignored for now*/, string filename) {
	ifstream fstr(filename.c_str());
	if (!fstr) {
	    cerr << "Failed to open file: " << filename << endl;
	}
	assert (fstr);

	double x;
	while (fstr>>x) {
	    poslu.push_back(x*1.0e5);
	    fstr>>x;
	    denlu.push_back(x);
	}
    }
    ComplexVector operator()(double energy, double baseline) {
	return nuosc_prob_lookup_matrix(nu0,op,energy,baseline,poslu,denlu);
    }
};

ProbCalc* make_probability_calculator(string calc_desc, string density_desc)
{
    if (calc_desc != "matrix" and calc_desc != "step") return 0;

    if (density_desc == "prem") return new PCPremCalc(calc_desc);

    size_t delim = density_desc.find(":");
    if (delim == string::npos) return 0;
    string density_type = density_desc.substr(0,delim);
    string density_data = density_desc.substr(delim+1);

    if (density_type == "con")
	return new PCConCalc(calc_desc,atof(density_data.c_str()));
    if (density_type == "lut")
	return new PCLutCalc(calc_desc,density_data);

    return 0;
}
