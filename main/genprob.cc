#include "nuosc_prob.h"
#include "options.h"

typedef struct {
    OscParam op;                // angles, masses, anti?
    ComplexVector nu0;          // normalized initial neutrino

                                // Do we iterate on a param?
    bool baseline_iterate, energy_iterate, density_iterate;
                                // Only use first unless we iterate
    double baseline, blstart, blstop, blstep;
    double energy, estart, estop, estep;

    double density;             // ignored if doing PREM
    int calculation;            // 1=matrix, 2=step, 3=PREM

} GenProbConfig;
GenProbConfig gpc;
Options* options_ptr = 0;

static void usage(const char* msg)
{
    if (!options_ptr) {
        cerr << msg << endl;
        return;
    }
    options_ptr->usage(cerr,msg);
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
    if (nunum < 0) gpc.op.set_antineutrino();
    gpc.nu0 = nu;
}
static int parse_calc(const char* calctype)
{
    if (!calctype) usage ("Expecting calculation type");
    string ct(calctype);
    if (ct == "matrix") return 1;
    if (ct == "step") return 2;
    if (ct == "prem") return 3;
    usage ("Unknown calculation type");
    return 0;
}
void parse_args(int argc, char* argv[])
{

    const char* optv[] = {
        "e+energy <energy | Estart Estop Estep> [eV] default = 1e9 eV",
        "b+baseline <baseline | Bstart Bstop Bstep> [cm] default = 1e5 cm",
        "t+theta <theta_12 theta_23 theta_13> degrees",
        "S+sin  <sin^2(2theta_12) sin^2(2theta_23) sin^2(2theta_13)>",
        "s|sol <dm2_sol = dm2_21 in eV^2> default = 5.0e-5 eV^2",
        "a|atm <dm2_atm = dm2_31 in eV^2> default = 2.5e-3 eV^2",
        "d|delta <CP phase in deg> default = 0",
        "n:neutrino <nu number> nue=1,numu=2,nutau=3, anti *= -1",
        "D:density <constant density in g/cm^3> default = 0",
        "c:calculation <calculation type> \"matrix\"(def), \"step\" or \"prem\"",
        0
    };

    Options options(*argv,optv);
    options_ptr = &options;
    OptArgvIter optitr(argc-1,argv+1);
    const char* optarg;
    vector<const char*> newargv;
    newargv.push_back(argv[0]);

    char optchar;
    while ((optchar = (options)(optitr,optarg))) {
        switch (optchar) {
        case 'e':
            if (!optarg) usage("No energy given with -e");
            gpc.energy = atof(optarg);
            optarg = optitr();
            if (optarg) {
                gpc.estart = gpc.energy;
                gpc.estop = atof(optarg);
                optarg = optitr();
                if (!optarg) gpc.estep = (gpc.estop-gpc.estart)/100.;
                else gpc.estep = atof(optarg);
            }
            break;
        case 'b':
            if (!optarg) usage("No baseline given with -b");
            gpc.baseline = atof(optarg);
            optarg = optitr();
            if (optarg) {
                gpc.blstart = gpc.baseline;
                gpc.blstop = atof(optarg);
                optarg = optitr();
                if (!optarg) gpc.blstep = (gpc.blstop-gpc.blstart)/100.;
                else gpc.blstep = atof(optarg);
            }
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
}


int main (int argc, char *argv[])
{
    parse_args(argc,argv);

    

    return 0;
} // end of main()
