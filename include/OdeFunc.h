////////////////////////////////////////////////////////////////////////
//
// $Id: OdeFunc.h,v 1.4 2003-07-07 22:20:54 bviren Exp $
//
// OdeFunc
//
// Package: elbo
//
// Solve dy/dx = F(x,y) where y and F are vectors.  OdeFunc is the
// base class.  Subclasses should define the function via operator().
//
// Contact: bv@bnl.gov
//
// Created on: Mon Sep 17 12:52:45 2001
//
////////////////////////////////////////////////////////////////////////


#ifndef ODEFUNC_H
#define ODEFUNC_H

#include "matrix.h"

class OdeFunc {
public:
    typedef enum {
        // Simple Euler method (the default) as seen in Computational
        // Physics, Koonin & Meredith.  Need high precision.  This
        // evaluates ode_func 1 time per step.
        euler,
        // 4th order Runge-Kutta method as seen in Computational
        // Physics, Koonin & Meredith.  Can do better with lower
        // precision, but can blow up or take forever if necessary
        // step size changes.  This evaluates ode_func 4 times per
        // step.
        runge_kutta,
        // 5th order Runge-Kutta with adaptive step sizing by
        // comparing each step to that found with (an embedded) 4th
        // order R-K.  As seen in Num Rec.  This evaluates the
        // ode_func 6 times per successful step.  In the case of a too
        // big step, a second step with smaller step size is done.
        runge_kutta_adaptive    // default
    } StepperType;

    OdeFunc();
    virtual ~OdeFunc();

    /// Set stepper to use
    void SetStepper(StepperType st = runge_kutta_adaptive);

    /// Override precision.  Only useful for RKA.
    void SetPrecision(double prec) { m_prec = prec; }
    double GetPrecision() { return m_prec; }

    // user overrides this operator.
    virtual ComplexVector operator()(ComplexVector y, double x) const = 0;

    // Returns vector y0 transported from x0 to xf by integrating in
    // one shot. 
    ComplexVector Solve(double x0, double xf, double step_size, 
                        ComplexVector y0);
    // Like above, but integrate over discrete regions.  This will
    // integrate from x0[0] to xf[0] with step_size[0], then restart
    // at x0[1], etc.  This can be used to give more steps to certain
    // regions, or to break the integration at a discontinuity.  
    ComplexVector Solve(const double x0[], const double xf[], 
                        const double step_size[],
                        int nregions, ComplexVector y0);

    // Call before a Solve()
    void ClearSteps() { m_steps.clear(); m_values.clear(); }
    std::vector<double> GetSteps(void) { return m_steps; }
    std::vector<ComplexVector> GetStepValues(void) { return m_values; }

    // Mostly internal.  Do a step with the current stepper.
    ComplexVector Step(double x, double& step_size, ComplexVector y);
    void SaveStep(ComplexVector y, double x);

private:

    // The different method steppers.  Besides doing the step, each
    // will save the result in the Steps and Values vectors.

    // Simple Euler method (the default) as seen in Computational
    // Physics, Koonin & Meredith.  Need high precision.  This
    // evaluates ode_func 1 time per step.
    ComplexVector euler_stepper(double x, double& step_size, 
                                ComplexVector y, double prec=0);
    
    // 4th order Runge-Kutta method as seen in Computational Physics,
    // Koonin & Meredith.  Can do better with lower precision, but can
    // blow up or take forever if necessary step size changes.  This
    // evaluates ode_func 4 times per step.
    ComplexVector runge_kutta_4th_stepper(double x, double& step_size, 
                                          ComplexVector y, double prec=0);

    // 5th order Runge-Kutta with adaptive step sizing by comparing
    // each step to that found with (an embedded) 4th order R-K.  As
    // seen in Num Rec.  This evaluates the ode_func 6 times per
    // successful step.  In the case of a too big step, a second step
    // with smaller step size is done.
    ComplexVector runge_kutta_adaptive_stepper(double x, double& step_size, 
                                               ComplexVector y, double prec=0);

    typedef ComplexVector (OdeFunc::* OdeStepper)(double x, double& step_size, 
                                                  ComplexVector y, double prec=0);

    // The one we use
    OdeStepper m_stepper;
    double m_prec;

    void condition_step_size(double x, double& h, double step_tol,
                             ComplexVector y, double prec);


    std::vector<double> m_steps;
    std::vector<ComplexVector> m_values;
    bool m_save_steps;
};

#endif  // ODE_H
