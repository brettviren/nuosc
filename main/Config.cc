#include "Config.h"
#include "util.h"
#include "options.h"

#include <cmath>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

const char* optv[] = {
    "n:neutrino <initial neutrino number>",
    "e:energy <energy range description>",
    "b:baseline <baseline range description>",
    "m:mixing <mixing angle description>",
    "s:sol <delta m^2_solar (dm^2_21)>",
    "a:atm <delta m^2_atm (dm^2_31)>",
    "d:delta <CP phase>",
    "D:density <density description>",
    "c:calculation <calculation description",
    "h|help",
    0
};

// Describe each option
const char* desc[] = {
    // neutrino
    "\tSets the initial neutrino e,mu,tau = 1,2,3; anti *= -1\n"
    "\tDefault is 2 (mu)\n",

    // energy
    "\tDescribe the energy range.  Can be one of:\n"
    "\t\tLinear energy range:      \"lin:start,stop,step\"\n"
    "\t\tLogarithmic energy range: \"log:start,stop,step\"\n"
    "\t\tA single energy value:    \"one:energy\"\n"
    "\tLinear energy units are in GeV,\n"
    "\tLogarithmic values give 10^start GeV - 10^stop GeV,\n"
    "\tDefault is \"one:1\"\n",
    
    // baseline
    "\tDescribe the baseline range.  Can be one of:\n"
    "\t\tLinear disance range:       \"lin:start,stop,step\"\n"
    "\t\tLogarithmic distance range: \"log:start,stop,step\"\n"
    "\t\tA single distance value:    \"one:distance\"\n"
    "\t\tZenith angle:               \"zen:start,stop,step[,depth]\"\n"
    "\t\tcos(Zenith angle):          \"cos:start,stop,step[,depth]\"\n"
    "\tDistance units are km, angle units are degrees.\n"
    "\tLogarithmic values give 10^start km - 10^stop km,\n"
    "\tThe \"depth\" value for zenith related gives detector depth (def=0),\n"
    "\tDefault is \"one:1\".\n",

    // mixing
    "\tSet the mixing angles description.  Can be one of:\n"
    "\t\tAngles:        \"ang:theta_12,theta_23,theta_13\"\n"
    "\t\tSin^2(2theta): \"sin:sin^2(2t_12),sin^2(2t_23),sin^2(2t_13)\"\n"
    "\tAngle units are in degrees, default \"sin:0.8,1.0,0.1\"\n",

    // sol
    "\tSet the solar delta-m^2 (dm^2_21).\n"
    "\tUnits are eV^2, default is 2.5e-3 eV^2\n",

    // atm
    "\tSet the atmospheric delta-m^2 (dm^2_31).\n"
    "\tUnits are eV^2, default is 5.0e-5 eV^2\n",
    
    // delta
    "\tSet the CP phase angle.\n"
    "\tUnits are degrees, default is 0 degrees\n",

    // density
    "\tSet the density description.  Can be one of:\n"
    "\t\tConstant density: \"con:density\"\n"
    "\t\tPREM density:     \"prem\"\n"
    "\t\tLookup table:     \"lut:filename\"\n"
    "\tDensity is in g/cc.\n"
    "\tThe lookup text file has columns of bin centered position and density\n"
    "\tDefault is \"con:0.0\"\n",

    // calculation
    "\tSet the calculation description.  Can be one of:\n"
    "\t\tMatrix method: \"matrix\"\n"
    "\t\tFull stepping: \"step\"\n"
    "\tDefault is \"matrix\"\n",

    // help
    "\tPrint this message\n",

    0
};

void Config::usage(const char* message, bool full)
{
    cerr << "Option summary:" << endl;
    options->usage(cerr,"");

    
    if (full) {
	cerr << "\nOptions details:\n";
	for (int ind=0; optv[ind]; ++ind) {
	    string odesc(optv[ind]);
	    cerr << "[-" << odesc[0] << "|--" << odesc.substr(2) << "]\n";
	    cerr << desc[ind] << endl;
	}
    }

    if (message)
	cerr << "Error:\n\t" << message << endl;
    exit (1);
}

static double unfrob(double s22t) { return asin(sqrt(s22t))/2.0; }

static bool set_mixing(OscParam& op, string desc)
{
    vector<double> vd = strings2doubles(split((desc.substr(4)),','));
    if (vd.size() != 3) return false;
    
    if (desc.substr(0,3) == "ang") {
	op.set_theta12(vd[0]);
	op.set_theta23(vd[1]);
	op.set_theta13(vd[2]);
    }
    else if (desc.substr(0,3) == "sin") {
	op.set_theta12(unfrob(vd[0]));
	op.set_theta23(unfrob(vd[1]));
	op.set_theta13(unfrob(vd[2]));
    }
    else return false;
    return true;
}

Config::Config(int argc, const char** argv)
{
    // defaults
    nu_num = 2;
    energy_desc = "one:1";
    baseline_desc = "one:1";
    calc_desc = "matrix";
    density_desc = "con:0.0";

    options = new Options(*argv,optv);
    OptArgvIter optitr(argc-1,argv+1);
    const char* optarg;

    if(argc==1) usage("No args supplied");

    char optchar;
    while ((optchar = (*options)(optitr,optarg))) {
        switch (optchar) {
        case 'n':		// neutrino
	    if (!optarg) usage("No number given with -n");
	    nu_num = atoi(optarg);
	    op.set_antineutrino(nu_num < 0);
	    break;
        case 'e':		// energy
	    if (!optarg) usage("No energy description given with -e");
	    energy_desc = optarg;
	    break;
        case 'b':		// baseline
	    if (!optarg) usage("No baseline description given with -b");
	    baseline_desc = optarg;
	    break;
        case 'm':		// mixing 
	    if (!optarg) usage("No mixing description given with -m");
	    if (!set_mixing(op,optarg))
		usage("Failed to parse mixing description");
	    break;
        case 's':		// sol
	    if (!optarg) usage("No solar dm^2 given with -s");
	    op.set_dms21(atof(optarg));
	    break;
        case 'a':		// atm
	    if (!optarg) usage("No atmospheric dm^2 given with -a");
	    op.set_dms31(atof(optarg));
	    break;
        case 'd':		// delta
	    if (!optarg) usage("No CP phase angle given with -d");
	    op.set_deltacp(atof(optarg));
	    break;
        case 'D':		// density
	    if (!optarg) usage("No density description given with -D");
	    density_desc = optarg;
	    break;
        case 'c':		// calculation
	    if (!optarg) usage("No calculation description given with -c");
	    calc_desc = optarg;
	    break;
	case 'h':
	    usage(0,true);
	default:
	    usage("Unknown option given");
	    break;
	}
    }
}

Config::~Config() { delete options; options = 0; }

