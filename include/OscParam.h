/**
 *
 * $Id: OscParam.h,v 1.1 2002-10-20 00:33:32 bviren Exp $
 *
 * \class OscParam
 *
 * \package libnuosc++
 *
 * \brief Parameters determining neutrino 3 neutrino vacuum
 * oscilation.
 * 
 * This class encapsulates the basic neutrino oscilation parameters.
 * They are:
 *
 * dm2_21 = m^2_2 - m^2_1 = Solar nu mass squared difference scale
 * when using a 2 neutrino scenario.  equal to about 5e-5 ev^2 (LMA)
 *
 * dm2_31 = m^2_3 - m^2_1 = dm2_32 - dm2_21.  dm2_32 is the
 * Atmospheric nu mass squared difference scale when using a 2
 * neutrino scenario.  equal to about 2.5e-3 (SK best fit)
 *
 * theta12 = angle governing R_12 rotation.  The solar neutrino angle
 * and equal to about 31 deg or sin^2(2theta12) = 0.8 (LMA).
 *
 * theta23 = angle governing R_23 rotation.  The atmospheric neutrino
 * angle and equal to 45 deg or sin^2(2theta23) = 1.0 (Max mixing).
 *
 * theta13 = angle governing R_13 rotaion.  The reactor neutrino angle
 * and less than 9 deg (depending on dm2_31) or sin^2(2theta13) < .1.
 * Note that this angle's indices are not cyclic from the other two.
 * This is critical.  The more correct labeling (imo) would be theta31
 * but if so, then the mixing matrix needs some signs re-arranged.
 * With this choice the Ue3 element is positive ie equal to
 * sin(theta13)exp(-i delta_CP).
 *
 * delta_CP = CP violation (dirac) angle.  No information known
 * 
 * What is not here is Energy, Baseline, matter and density.
 *
 * Note: all angles are in radians unless explicitly stated as
 * degrees.
 *
 * Contact: bv@bnl.gov
 *
 * Created on: Tue Oct 15 16:00:29 2002
 *
 */

#ifndef OSCPARAM_H
#define OSCPARAM_H

#include <libnuosc++/matrix.h>

class OscParam
{

public:

    OscParam(double dm2_21=5.0e-5, /*ev^2*/
             double dm2_31=2.5e-3, /*ev^2*/
             double theta_12=0.553574358897045, /*31.7 deg => sin22t12 = 0.8*/
             double theta_23=0.785398163397448, /*45.0 deg => sin22t23 = 1.0*/
             double theta_13=0.219074515292085, /*12.5 deg => sin22t13
                                                 * = 0.18 (CHOOZ at SK
                                                 * best dm2_23 fit
                                                 * point */
             double delta_cp = 0.0); 
    ~OscParam();

    double get_dm2_21();        // ev^2
    double get_dm2_31();        // ev^2
    double get_theta_12();      // rad
    double get_theta_23();      // rad
    double get_theta_13();      // rad
    double get_delta_cp();      // rad
    double get_s22t12();        // as sin^2(2theta)
    double get_s22t23();        // as sin^2(2theta)
    double get_s22t13();        // as sin^2(2theta)


    void set_dm2_21(double ev2);
    void set_dm2_31(double ev2);
    void set_theta_12(double deg);
    void set_theta_23(double deg);
    void set_theta_13(double deg);
    void set_delta_cp(double deg);
    
    ComplexMatrix get_mixing_matrix();

private:

    double m_dm2_21, m_dm2_31;
    double m_theta_12, m_theta_23, m_theta_13;
    double m_delta_cp;
    
    bool m_dirty_mixing;

    ComplexMatrix m_mixing_matrix;
    
};                              // end of class OscParam

#endif  // OSCPARAM_H
