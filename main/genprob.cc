#include "nuosc_prob.h"
#include "options.h"
#include "fp.h"
#include <string>
#include <cstring>

using namespace std;

struct GenProbConfig {
    OscParam op;                // angles, masses, anti?
    ComplexVector nu0;          // normalized initial neutrino
    int nu_num;
    const char* nu_str;
                                // Do we iterate on a param?
    bool baseline_iterate, energy_iterate;
                                // Only use first unless we iterate
    double baseline, blstart, blstop, blstep;
    double energy, estart, estop, estep;

    double density;             // <0 if doing PREM, 0 for vacuum
    int calculation;            // 1=matrix, 2=step

    GenProbConfig() : nu0(3) {
        nu0 = 0.0;
        nu0(1) = 1.0;
        nu_num = 2;
        nu_str = "nu_mu";
        baseline_iterate = energy_iterate = false;
        baseline = 1e5;
        energy = 1e9;
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
    if (gpc.baseline_iterate)
        cerr << "baseline = " << gpc.blstart << " -> " 
             << gpc.blstop << " += " << gpc.blstep << endl;
    else
        cerr << "baseline = " << gpc.baseline << endl;
    if (gpc.energy_iterate)
        cerr << "energy = " << gpc.estart << " -> " 
             << gpc.estop << " += " << gpc.estep << endl;
    else
        cerr << "energy = " << gpc.energy << endl;
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
void parse_args(int argc, char* argv[])
{

    const char* optv[] = {
        "e:energy <energy> [GeV] default = 1 GeV",
        "b:baseline <baseline> [km] default = 1 km",
        "E+energy-range <Estart Estop Estep> [GeV]",
        "B+baseline-range <Bstart Bstop Bstep> [km]",
        "t+theta <theta_12 theta_23 theta_13> degrees",
        "S+sin  <sin^2(2theta_12) sin^2(2theta_23) sin^2(2theta_13)>",
        "s:sol <dm2_sol = dm2_21 in eV^2> default = 5.0e-5 eV^2",
        "a:atm <dm2_atm = dm2_31 in eV^2> default = 2.5e-3 eV^2",
        "d:delta <CP phase in deg> default = 0",
        "n:neutrino <nu number> nue=1,numu=2,nutau=3, anti *= -1",
        "D:density <constant density in g/cm^3> default = 0. <0 => PREM",
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
            gpc.energy = atof(optarg)*1e9;
            gpc.energy_iterate = false;
            break;
        case 'E':
            gpc.energy_iterate = true;
            gpc.estart = atof(optarg)*1e9;
            optarg = optitr();
            gpc.estop = atof(optarg)*1e9;
            optarg = optitr();
            if (!optarg) gpc.estep = (gpc.estop-gpc.estart)/100.*1e9;
            else gpc.estep = atof(optarg)*1e9;
            break;
        case 'b':
            if (!optarg) usage("No baseline given with -b");
            gpc.baseline = atof(optarg)*1e5;
            gpc.baseline_iterate = false;
            break;
        case 'B':
            gpc.baseline_iterate = true;
            gpc.blstart = atof(optarg)*1e5;
            optarg = optitr();
            gpc.blstop = atof(optarg)*1e5;
            optarg = optitr();
            if (!optarg) gpc.blstep = (gpc.blstop-gpc.blstart)/100.*1e5;
            else gpc.blstep = atof(optarg)*1e5;
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

do_prob_f do_prob;

// iterate over both energy and baseline
void do_energy_baseline(void)
{
    for (double baseline = gpc.blstart; baseline <= gpc.blstop; baseline += gpc.blstep) {
        for (double energy = gpc.estart; energy <= gpc.estop; energy += gpc.estep) {
            ComplexVector nu = do_prob(energy,baseline);
            output(energy,baseline,nu);
        }
    }
}
// iterate over just energy
void do_energy(void)
{
    for (double energy = gpc.estart; energy <= gpc.estop; energy += gpc.estep) {
        ComplexVector nu = do_prob(energy,gpc.baseline);
        output(energy,gpc.baseline,nu);
    }
}

// iterate over just baseline
void do_baseline(void)
{
    for (double baseline = gpc.blstart; baseline <= gpc.blstop; baseline += gpc.blstep) {
        ComplexVector nu = do_prob(gpc.energy,baseline);
        output(gpc.energy,baseline,nu);
    }
}

// no iteration, just one shot
void do_single()
{
    ComplexVector nu = do_prob(gpc.energy,gpc.baseline);
    output(gpc.energy,gpc.baseline,nu);
}


int main (int argc, char *argv[])
{
    FloatXImp trap_fpe(true);

    parse_args(argc,argv);

    if (gpc.density < -0.0001) {
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

    if (gpc.baseline_iterate && gpc.energy_iterate) 
        do_energy_baseline();
    else if (gpc.baseline_iterate)
        do_baseline();
    else if (gpc.energy_iterate)
        do_energy();
    else do_single();

    return 0;
} // end of main()
