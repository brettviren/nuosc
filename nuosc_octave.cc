#include "nuosc_matrix.h"
#include <octave/oct.h>

//ComplexMatrix cp_phase_matrix(double cp_phase);
DEFUN_DLD(nuosc_cp_phase_matrix,args,nargout,
          "-*- texinfo -*-\n\
@deftypefn{Loadable Function} \
&var{r} = nuosc_cp_phase_matrix(@var{angle})\n\
\n\
@cindex CP phase matrix\n\
\n\
Return the complex transformation matrix for the given CP phase angle.
Angle should be in radians\n\
")
{
    double angle = args(0).double_value();

    return cp_phase_matrix(angle);
}

// ComplexMatrix rotation_matrix(int i, int j, double angle);
DEFUN_DLD(nuosc_rotation_matrix,args,nargout,
          "-*- texinfo -*-\n\
@deftypefn{Loadable Function} \
&var{r} = nuosc_rotation_matrix(@var{i},@var{j},@var{angle})\n\
\n\
@cindex Rotation matrix\n\
\n\
Return the transformation matrix for a rotation of the given angle in\n\
the i-j plane.  i and j are 1,2,3 and angle should be in radians.\n\
")
{
    int i = args(0).int_value();
    int j = args(1).int_value();
    double angle = args(2).double_value();
    return rotation_matrix(i,j,angle);
}


#if 0
ComplexMatrix mixing_matrix(double theta12, // theta1 in Marciano's notation
                            double theta23, // theta2 in Marciano's notation
                            double theta13, // theta3 in Marciano's notation
                            double cp_phase);
ComplexMatrix mass_squared_matrix(double dm2_sol, double dm2_atm);
ComplexMatrix mass_state_transport_matrix(double dm2sol, double dm2atm, // eV^2
                                          double energy, // eV
                                          double distance); // cm
#endif
