/**
 *
 * $Id: nuosc_octave.cc,v 1.1 2002-11-23 22:23:06 bviren Exp $
 *
 * \class nuosc_octave
 *
 * \package libnuosc++
 *
 * \brief Interface glue for nuosc functions to be seen in octave
 *
 * Contact: bv@bnl.gov
 *
 * Created on: Thu Nov 21 16:53:22 2002
 *
 */


#include "nuosc_matrix.h"
#include "OscParam.h"
#include "NuEvolverVacuum.h"
#include "NuEvolverConstant.h"
#include <octave/oct.h>



static OscParam osc_params;
static double baseline = 254000; // (cm) BNL2HS
static double energy = 1.0e9;   // eV

static NuEvolver* evolver = 0;
static void set_evolver(NuEvolver* ne)
{
    if (evolver) delete evolver;
    evolver = ne;
}

DEFUN_DLD(nuosc,args,,"An interface to neutrino oscillation calculations")
{
    cerr << "An interface to neutrino oscillation calculations" << endl;

    cerr << "Defaulting to vacuum oscilations" << endl;
    set_evolver(new NuEvolverVacuum(osc_params,energy,baseline));

    return octave_value();
}

#define NUOSC_VAR(var,doc) \
DEFUN_DLD(nuosc_set_ ## var,args,,"Set " ## doc) \
{ var = args(0).double_value(); return octave_value();} \
DEFUN_DLD(nuosc_get_ ## var,args,nargout,"Get " ## doc) \
{ return var; }

NUOSC_VAR(baseline," neutrino baseline (cm)")
NUOSC_VAR(energy," neutrino energy (cm)")
#undef NUOSC_VAR


#define NUOSC_PARAM(param,doc) \
DEFUN_DLD(nuosc_set_ ## param,args,,"Set " ## doc) \
{ osc_params.set_ ## param(args(0).double_value()); return octave_value();} \
DEFUN_DLD(nuosc_get_ ## param,args,nargout,"Get " ## doc) \
{ return osc_params.get_ ## param(); }
    
NUOSC_PARAM(dms21, "delta m^2_21 (eV^2)")
NUOSC_PARAM(dms31, "delta m^2_31 (eV^2)")
NUOSC_PARAM(theta12, "theta_12 (rad)")
NUOSC_PARAM(theta23, "theta_23 (rad)")
NUOSC_PARAM(theta13, "theta_13 (rad)")
NUOSC_PARAM(deltacp, "delta_cp (rad)")
#undef NUOSC_PARAM

DEFUN_DLD(nuosc_current_mixing_matrix,args,nargout, \
          "Get the current mixing matrix")
{
    return osc_params.get_mixing_matrix();
}

    
//ComplexMatrix cp_phase_matrix(double cp_phase);
DEFUN_DLD(nuosc_cp_phase_matrix,args,nargout,
          "-*- texinfo -*-\n\
@deftypefn{Loadable Function} {@var{r} =} nuosc_cp_phase_matrix(@var{angle})\n\
\n\
@cindex CP phase matrix\n\
\n\
Return the complex transformation matrix for the given CP phase angle.\n\
Angle should be in radians\n\
@end deftypefn\n\
")
{
    double angle = args(0).double_value();

    return cp_phase_matrix(angle);
}

// ComplexMatrix rotation_matrix(int i, int j, double angle);
DEFUN_DLD(nuosc_rotation_matrix,args,nargout,
          "-*- texinfo -*-\n\
@deftypefn{Loadable Function} {@var{r} =} nuosc_rotation_matrix(@var{i},@var{j},@var{angle})\n\
\n\
@cindex Rotation matrix\n\
\n\
Return the transformation matrix for a rotation of the given angle in\n\
the i-j plane.  i and j are 1,2,3 and angle should be in radians.\n\
@end deftypefn\n\
")
{
    int i = args(0).int_value();
    int j = args(1).int_value();
    double angle = args(2).double_value();
    return rotation_matrix(i,j,angle);
}



// ComplexMatrix mixing_matrix(double theta12,double theta23,
//                             double theta13,double cp_phase);
DEFUN_DLD(nuosc_mixing_matrix,args,nargout,
          "-*- texinfo -*-\n\
@deftypefn{Loadable Function} {@var{r} =} nuosc_mixing_matrix(@var{theta12},@var{theta23},@var{theta13},@var{cp_phase})\n\
\n\
@cindex Neutrino mixing matrix\n\
\n\
Return the transformation matrix for three neutrino mixing with the\n\
given angles, all in radians.  CP phase is optional and assumed zero if\n\
not specified.\n\
@end deftypefn\n\
")
{
    int len = args.length();
    if (len < 3 || len > 4) return octave_value();

    double theta12 = args(0).double_value();
    double theta23 = args(1).double_value();
    double theta13 = args(2).double_value();
    double cpphase = 0.0;
    if (len == 4) cpphase = args(3).double_value();

    return mixing_matrix(theta12,theta23,theta13,cpphase);
}

//ComplexMatrix mass_squared_matrix(double dm2_sol, double dm2_atm);
DEFUN_DLD(nuosc_mass_squared_matrix,args,nargout,
          "-*- texinfo -*-\n\
@deftypefn{Loadable Function} {@var{r} =} nuosc_mass_squared_matrix(@var{dms21},@var{dms31})\n\
\n\
@cindex Neutrino mass matrix\n\
\n\
Return the mass squared matrix in the form diag(0,dms21,dms31)\n\
@end deftypefn\n\
")
{
    if (args.length() != 2) return octave_value();

    double dm2sol = args(0).double_value();
    double dm2atm = args(1).double_value();
    return mass_squared_matrix(dm2sol,dm2atm);
}

//ComplexMatrix mass_state_transport_matrix(double dm2sol, double dm2atm,
//                                          double energy, double distance);
DEFUN_DLD(nuosc_mass_state_transport_matrix,args,nargout,
          "-*- texinfo -*-\n\
@deftypefn{Loadable Function} {@var{r} =} nuosc_mass_state_transport_matrix(@var{dms21},@var{dms31},@var{energy},@var{distance})\n\
\n\
@cindex Neutrino mass state transport matrix\n\
\n\
Return the space development of a mass eigenstate of given \n\
energy (in eV) across given distance (in cm) assuming given \n\
mass differences (in eV^2)\n\
@end deftypefn\n\
")
{
    if (args.length() != 4) return octave_value();

    double dm2sol = args(0).double_value();
    double dm2atm = args(1).double_value();
    double energy = args(2).double_value();
    double distance = args(3).double_value();

    return mass_state_transport_matrix(dm2sol,dm2atm,energy,distance);
}


DEFUN_DLD(nuosc_set_evolver,args,,
          "-*- texinfo -*-\n\
@deftypefn{Loadable Function} {@var{r} =} nuosc_set_evolver(@var{evolver_name},@var{...})\n\
\n\
@cindex Set the evolver to use for the nuosc_evolve function\n\
\n\
The current energy and baseline is used\n\
Choices are:\n\
 vacuum oscillations:\n\
        (\"vacuum\")\n\
 constant matter density (in g/cm3).  pass true for last if want anti-nu:\n\
        (\"constant\",density,[antinu=false])\n\
@end deftypefn\n\
")
{
    if (args.length() < 1) return octave_value();

    string s = args(0).string_value();
    if (s == "vacuum") {
        set_evolver(new NuEvolverVacuum(osc_params,energy,baseline));
        return octave_value();
    }
    if (s == "constant") {
        if (args.length() < 2) return octave_value();
        double density = args(1).double_value();
        set_evolver(new NuEvolverConstant(osc_params,energy,baseline,density));
        if (args.length() > 2) evolver->set_density(args(2).double_value());
        return octave_value();
    }
    return octave_value();
}

DEFUN_DLD(nuosc_evolve,args,nargout,
 "-*- texinfo -*-\n\
@deftypefn{Loadable Function} {@var{r} =} nuosc_evolve(@var{neutrino-vector},@var{position})\n\
\n\
@cindex ODE function for neutrino oscillations.\n\
\n\
Return the differential evolution of the given neutrino at the \n\
given location.\n\
\n\
The neutrino should be expressed as a unit complex column 3-vector\n\
The position is in cm along the baseline\n\
@end deftypefn\n\
")
{
    if (args.length() != 2) return ComplexColumnVector(3,Complex(0,0));

    return (*evolver)(args(0).complex_column_vector_value(),
                      args(1).double_value());
}
