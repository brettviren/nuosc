#include "nuosc_prob.h"
#include "nuosc_earth.h"
#include "options.h"
#include "fp.h"
#include <string>
#include <cstring>
#include <fstream>
#include <vector>

using namespace std;

// Trivial iterator like ABC
class Iterator {
public:
    virtual bool valid() = 0;
    virtual double operator()(void) = 0;
    virtual bool next() = 0;
    virtual void dump(ostream& o) = 0;
};

// One shot iterator
class OneNumber : public Iterator {
    double value;
    bool done;
public:
    OneNumber(double Value)
    { value = Value; done = false; }
    bool valid() { return !done; }
    bool next() { done = true; return valid(); }
    double operator()(void) { return value; }
    void dump(ostream &o) { o << value; }
};


// Iterate from start to stop via linear steps
class FlatIterator : public Iterator{
protected:
    double start, stop, step, value;
public:
    FlatIterator(double Start, double Stop, double Step) 
    { start = Start; stop = Stop; step = Step; value = Start; }
    bool valid() { return value >= start && value <= stop; }
    bool next() { value += step; return valid(); }
    double operator()(void) { return value; }
    void dump(ostream &o) { o << start << " -> " << stop << " += " << step; }
};

// iterate over cos(zenith) evenly distributed between Start and Stop
// values in the given number of bins.  Functor returns baseline.
class CosZenithIterator : public FlatIterator {
    double start, stop, step, value;
public:
    CosZenithIterator(int Bins,double Start=-1.0,double Stop=1.0)
	: FlatIterator(Start,Stop,(Stop-Start)/Bins) {}
    double operator()(void) { return earth_zenith_to_baseline(value);}
    void dump(ostream &o) {
	o << earth_zenith_to_baseline(start) << " -> "
	  << earth_zenith_to_baseline(stop) << " in "
	  << (stop-start)/step << " bins";
    }
};


struct GenProbConfig {
    OscParam op;                // angles, masses, anti?
    ComplexVector nu0;          // normalized initial neutrino
    int nu_num;
    const char* nu_str;
                                // Do we iterate on a param?

    Iterator *energy_itr, *baseline_itr;

    double depth;		// depth of det. used with zenith
    double density;             // <0 if doing PREM, 0 for vacuum
    int calculation;            // 1=matrix, 2=step

    vector<double> denlu, poslu; // density/position lookup tables

    GenProbConfig() : nu0(3) {
        nu0 = 0.0;
        nu0(1) = 1.0;
        nu_num = 2;
        nu_str = "nu_mu";
	energy_itr = 0;
        baseline_itr = 0;
	depth = 0;
        density = 0.0;
        calculation = 1;
    }
};
GenProbConfig gpc;
Options* options_ptr = 0;

static void dump_gpc()
{
    cerr << gpc.nu_str << endl
         << "dms21   = " << gpc.op.get_dms21() << endl
         << "dms31   = " << gpc.op.get_dms31() << endl
         << "theta12 = " << gpc.op.get_theta12() << endl
         << "theta23 = " << gpc.op.get_theta23() << endl
         << "theta13 = " << gpc.op.get_theta13() << endl
         << "ss2t12  = " << gpc.op.get_ss2t12() << endl
         << "ss2t23  = " << gpc.op.get_ss2t23() << endl
         << "ss2t13  = " << gpc.op.get_ss2t13() << endl
         << "deltacp = " << gpc.op.get_deltacp() << endl
         << "calculation = " << gpc.calculation 
         << (gpc.calculation == 1 ? " (matrix)" : " (step)") << endl;
    if (gpc.baseline_itr) {
        cerr << "baseline = ";
	gpc.baseline_itr->dump(cerr);
	cerr << endl;
    }
    if (gpc.energy_itr) {
        cerr << "energy = ";
	gpc.energy_itr->dump(cerr);
	cerr << endl;
    }
    int n = gpc.denlu.size();
    if (n) {
        cerr << "density lookup table = " << endl;
        for (int i=0; i<n; ++i)
            cerr << gpc.poslu[i] << " " << gpc.denlu[i] << endl;
    }
    else
        cerr << "density = " <<  gpc.density << endl;
}

static void usage(const char* msg)
{
    cerr << msg << endl;

    if (options_ptr) {
        options_ptr->usage(cerr,"");
    }
    exit(1);
}
static double d2r(const char* degrees_string)
{
    if (!degrees_string) usage("Expecting angle");
    double degrees = atof(degrees_string);
    while (degrees < 0) degrees += 360.0;
    while (degrees >= 360) degrees -= 360.0;
    return degrees*M_PI/180.;
}
static double sstt2t(const char* sin22theta)
{
    if (!sin22theta) usage("Expecting sin^2(2theta)");
    double x = atof(sin22theta);
    if (x<0) usage("Negative sin^2(2theta)");
    x = sqrt(x);
    if (x<-1 || x>+1) usage("Illegal value for sin(2theta)");
    x = asin(x);
    return x/2.0;
}
static void nunum2nu(const char* nunum_string)
{
    if (!nunum_string) usage ("No nu number given with -n");
    int nunum = atoi(nunum_string);
    if (nunum < -3 || nunum > 3 || !nunum) usage("Illegal nu num");
    ComplexVector nu(3);
    nu = 0.;
    nu(abs(nunum)-1) = 1.;
    string nu_str;
    if (nunum < 0) {
        gpc.op.set_antineutrino();
        nu_str = "anti-";
    }
    gpc.nu0 = nu;
    switch (abs(nunum)) {
    case 1: nu_str += "nu_e"; break;
    case 2: nu_str += "nu_mu"; break;
    case 3: nu_str += "nu_tau"; break;
    }
    gpc.nu_num = nunum;
    gpc.nu_str = strdup(nu_str.c_str());
}
static int parse_calc(const char* calctype)
{
    if (!calctype) usage ("Expecting calculation type");
    string ct(calctype);
    if (ct == "matrix") return 1;
    if (ct == "step") return 2;
    usage ("Unknown calculation type");
    return 0;
}
static void fill_density_lookup(const char* filename)
{
    ifstream fstr(filename);

    if (!fstr) {
        string msg = "Failed to open lookup file ";
        msg += filename;
        usage (msg.c_str());
        return;
    }

    double x;
    while (fstr>>x) {
        gpc.poslu.push_back(x*1.0e5);
        fstr>>x;
        gpc.denlu.push_back(x);
    }
}


void parse_args(int argc, char* argv[])
{

    const char* optv[] = {
        "e:energy <energy> [GeV] default = 1 GeV",
        "b:baseline <baseline> [km] default = 1 km",
	"z:zenith <zenith angle> [deg] default unused",
	"h:depth <depth of detector> [km] default unused (use with Zenith)",
        "E+energy-range <Estart Estop Estep> [GeV]",
        "B+baseline-range <Bstart Bstop Bstep> [km]",
	"Z+zenith-range <Zstart Zstop Zsetp> [deg]",
        "t+theta <theta_12 theta_23 theta_13> degrees",
        "S+sin  <sin^2(2theta_12) sin^2(2theta_23) sin^2(2theta_13)>",
        "s:sol <dm2_sol = dm2_21 in eV^2> default = 5.0e-5 eV^2",
        "a:atm <dm2_atm = dm2_31 in eV^2> default = 2.5e-3 eV^2",
        "d:delta <CP phase in deg> default = 0",
        "n:neutrino <nu number> nue=1,numu=2,nutau=3, anti *= -1",
        "D:density <constant density in g/cm^3> default = 0. <0 => PREM",
        "L:lookup <density lookup file name - columns of pos & dens>",
        "c:calculation <calculation type> \"matrix\"(def), \"step\"",
        0
    };

    Options options(*argv,optv);
    options_ptr = &options;
    OptArgvIter optitr(argc-1,argv+1);
    const char* optarg;

    if(argc==1) usage("No args supplied");

    char optchar;
    while ((optchar = (options)(optitr,optarg))) {
        switch (optchar) {
        case 'e':
            if (!optarg) usage("No energy given with -e");
            gpc.energy_itr = new OneNumber(atof(optarg)*1e9);
            break;
        case 'E': {
	    double start = atof(optarg)*1e9;
            optarg = optitr();
            double stop = atof(optarg)*1e9;
	    double step = (stop-start)/100.*1e9;
            optarg = optitr();
            if (optarg) step = atof(optarg)*1e9;
	    gpc.energy_itr = new FlatIterator(start,stop,step);
            break;
	}
        case 'b':
            if (!optarg) usage("No baseline given with -b");
            gpc.baseline_itr = new OneNumber(atof(optarg)*1e5);
            break;
        case 'B': {
	    double start = atof(optarg)*1e5;
            optarg = optitr();
            double stop = atof(optarg)*1e5;
	    double step = (stop-start)/100.*1e5;
            optarg = optitr();
            if (optarg) step = atof(optarg)*1e5;
	    gpc.baseline_itr = new FlatIterator(start,stop,step);
            break;
	}
        case 'z':
            if (!optarg) usage("No baseline given with -b");
            gpc.baseline_itr =
		new OneNumber(earth_zenith_to_baseline(acos(atof(optarg)/180.*M_PI)));
            break;
        case 'Z': {
	    double start = acos(atof(optarg)/180.*M_PI);
            optarg = optitr();
            double stop = acos(atof(optarg)/180.*M_PI);
	    int bins = 100;
            optarg = optitr();
            if (optarg) bins = atoi(optarg);
	    gpc.baseline_itr = new CosZenithIterator(bins,start,stop);
            break;
	}
	case 'h':
	    gpc.depth = atof(optarg)*1e5;
	    break;
        case 't':
            gpc.op.set_theta12(d2r(optarg));
            optarg = optitr();
            gpc.op.set_theta23(d2r(optarg));
            optarg = optitr();
            gpc.op.set_theta13(d2r(optarg));
            break;
        case 'S':
            gpc.op.set_theta12(sstt2t(optarg));
            optarg = optitr();
            gpc.op.set_theta23(sstt2t(optarg));
            optarg = optitr();
            gpc.op.set_theta13(sstt2t(optarg));
            break;
        case 's':
            if (!optarg) usage("No dm^2_21 given with -s");
            gpc.op.set_dms21(atof(optarg));
            break;
        case 'a':
            if (!optarg) usage("No dm^2_31 given with -a");
            gpc.op.set_dms31(atof(optarg));
            break;
        case 'd':
            if (!optarg) usage("No delta_cp given with -d");
            gpc.op.set_deltacp(d2r(optarg));
            break;
        case 'n':
            nunum2nu(optarg);
            break;
        case 'D':
            if (!optarg) usage ("No density given with -D");
            gpc.density = atof(optarg);
            break;
        case 'L':
            if (!optarg) usage ("No density lookup filename given with -L");
            fill_density_lookup(optarg);
            break;
        case 'c':
            if (!optarg) usage ("No calculation type given with -c");
            gpc.calculation = parse_calc(optarg);
            break;
        default:
            usage("");
        }
    }

    dump_gpc();
}

static void output(double energy, double baseline, const ComplexVector& nu)
{
    vector<double> p = nuosc_amplitude_to_prob(nu);
    cout << energy/1e9 << " " << baseline/1e5 << " " 
         << p[0] << " "
         << p[1] << " "
         << p[2] << endl;
}

// Uniform interface to all calculations
typedef ComplexVector (*do_prob_f)(double energy, double baseline);
inline ComplexVector do_vacuum_step(double energy, double baseline)
{return nuosc_prob_vacuum_step(gpc.nu0,gpc.op,energy,baseline);}
inline ComplexVector do_vacuum_matrix(double energy, double baseline)
{return nuosc_prob_vacuum_matrix(gpc.nu0,gpc.op,energy,baseline);}
inline ComplexVector do_matter_step(double energy, double baseline)
{return nuosc_prob_constant_step(gpc.nu0,gpc.op,energy,baseline,gpc.density);}
inline ComplexVector do_matter_matrix(double energy, double baseline)
{return nuosc_prob_constant_matrix(gpc.nu0,gpc.op,energy,baseline,gpc.density);}
inline ComplexVector do_prem_step(double energy, double baseline)
{return nuosc_prob_prem_step(gpc.nu0,gpc.op,energy,baseline);}
inline ComplexVector do_prem_matrix(double energy, double baseline)
{return nuosc_prob_prem_matrix(gpc.nu0,gpc.op,energy,baseline);}
inline ComplexVector do_lookup_matrix(double energy, double baseline)
{return nuosc_prob_lookup_matrix(gpc.nu0,gpc.op,energy,baseline,
                                 gpc.poslu,gpc.denlu);}

do_prob_f do_prob;


int main (int argc, char *argv[])
{
    FloatXImp trap_fpe(true);

    parse_args(argc,argv);

    // Pick probability calculator
    if (gpc.denlu.size() > 0) {
        do_prob = do_lookup_matrix;
    }
    else if (gpc.density < -0.0001) {
        if (gpc.calculation == 1)
            do_prob = do_prem_matrix;
        else
            do_prob = do_prem_step;
    }
    else if (gpc.density > 0.0001) {
        if (gpc.calculation == 1)
            do_prob = do_matter_matrix;
        else 
            do_prob = do_matter_step;
    }
    else {
        if (gpc.calculation == 1)
            do_prob = do_vacuum_matrix;
        else
            do_prob = do_vacuum_step;
    }

    for (double baseline = 0; gpc.baseline_itr->valid(); gpc.baseline_itr->next()) {
	baseline = (*gpc.baseline_itr)();
	for (double energy = 0; gpc.energy_itr->valid(); gpc.energy_itr->next()) {
	    energy = (*gpc.energy_itr)();
	    ComplexVector nu = do_prob(energy,baseline);
            output(energy,baseline,nu);
	}
    }

    return 0;
} // end of main()
