////////////////////////////////////////////////////////////////////////
//
// $Id: OdeFunc.h,v 1.1 2002-11-26 22:09:26 bviren Exp $
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
    ComplexVector Solve(double x0[], double xf[], double step_size[],
                        int nregions, ComplexVector y0);

    ComplexVector Step(double x, double& step_size, ComplexVector y);

private:
    typedef ComplexVector (*OdeStepper)(OdeFunc& ode_func,
                                        double x, double& step_size, 
                                        ComplexVector y, double prec);

    OdeStepper m_stepper;
    double m_prec;
};

#endif  // ODE_H
