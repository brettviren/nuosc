#include "OdeFunc.h"

using namespace blitz;




OdeFunc::OdeFunc(){ this->SetStepper(); }
OdeFunc::~OdeFunc(){}

void OdeFunc::SetStepper(OdeFunc::StepperType st)
{
    m_prec = 0;
    switch (st) {
    case euler:
        m_stepper = &OdeFunc::euler_stepper;
        break;
    case runge_kutta: 
        m_stepper = &OdeFunc::runge_kutta_4th_stepper;
        break;
    case runge_kutta_adaptive: default:
        m_stepper = &OdeFunc::runge_kutta_adaptive_stepper;
        m_prec = 1.0e-15;
        break;
    }
}

ComplexVector OdeFunc::Step(double x, double& step_size, 
                            ComplexVector y)
{
    assert (m_stepper);
    return (this->*m_stepper)(x,step_size,y,m_prec);
}


void OdeFunc::SaveStep(ComplexVector y, double x)
{
    m_steps.push_back(x);
    ComplexVector save(3);
    save = y;           // make sure we copy.  CV v1 = v2 is a ref
    m_values.push_back(save);
}

// Integrate over one region.
ComplexVector OdeFunc::Solve(double x0, double xf, double step_size,
                             ComplexVector y0)
{
    assert(m_stepper);

    ComplexVector y(3);
    y = y0;
    double x;
    double requested_step_size=step_size;
    for (x = x0; x+step_size< xf; x += step_size) {
        requested_step_size = step_size;
        y = this->Step(x,step_size, y);
//        cerr  << "(" << step_size << ")"
//              << "x=" << x << ", y= " << y << endl;
//        double mag =  vector_magnitude(y);
//        if (fabs(1-mag) > 0.001)
//            cerr << "|y|=" << mag << endl;
//        y /= mag;
    }
#if 1
    cerr << "\nxf - x = " << xf - x
         << " requested_step = " << requested_step_size
         << " step = " << step_size
         << endl;
#endif
    // The actual location of the last evaluation for Y.  Assumes that
    // the step wasn't recursive!.
    double last_x = x - step_size + requested_step_size;

    // Already over stepped, just live with it
    if (last_x>xf) {
        m_steps.push_back(last_x);
        ComplexVector save(3);
        save = y;
        m_values.push_back(save);
        return y;
    }

    // Next step will over step, so just end things with an explicit
    // last step.  Use RK 4th for a that extra bit of accuracy.

    m_steps.push_back(last_x);
    ComplexVector save(3);
    double dx = xf-last_x;
    save = this->runge_kutta_4th_stepper(last_x,dx,y);
    m_values.push_back(save);
    return save;
}

// Integrate over discrete regions.  This will integrate from x0[0] to
// xf[0] with step_size[0], then jump to x0[1], etc.  This can be used
// to give more steps to certain regions, or to break the integration
// at discontinuities.  
ComplexVector OdeFunc::Solve(const double x0[], const double xf[], 
                             const double step_size[],
                             int nregions, ComplexVector y0)
{
    ComplexVector y(3);
    y = y0;
    for (int region = 0; region < nregions; ++region) {
        double ss = step_size[region];
        y = this->Solve(x0[region],xf[region],ss,y);
    }
    return y;
}


// Euler method, error = order(step_size^2)
ComplexVector OdeFunc::euler_stepper(double x, double& step_size, 
                                     ComplexVector y, double)
{
    OdeFunc& ode_func = (*this);
    ComplexVector ret(3);
    ret = y + step_size*ode_func(y,x);
    this->SaveStep(y,x+step_size);
    return ret;
}


// 4th order Runge-Kutta, error = order(step_size^5)
ComplexVector OdeFunc::runge_kutta_4th_stepper(double x, double& step_size, 
                                               ComplexVector y, double)
{
    OdeFunc& ode_func = (*this);

    ComplexVector k1(3), k2(3), k3(3), k4(3);
    k1 = step_size*ode_func(y,               x);
    k2 = step_size*ode_func(y+0.5*k1, x+0.5*step_size);
    k3 = step_size*ode_func(y+0.5*k2, x+0.5*step_size);
    k4 = step_size*ode_func(y+    k3, x+    step_size);
    ComplexVector ret(3);
    ret = y + (1.0/6.0) * (k1 + 2.0*k2 + 2.0*k3 + k4);
    this->SaveStep(y,x+step_size);
    return ret;
}

ComplexVector OdeFunc::runge_kutta_adaptive_stepper(double x, double& h,
                                                    ComplexVector y, double prec)
{
    ComplexVector k1(3), k2(3), k3(3), k4(3), k5(3), k6(3);
    ComplexVector y5(3), dy(3);

    // Cash-Karp parameters for embedded R-K.
    const double a[7] = { 0, 0, 0.2, 0.3, 0.6, 1.0, 0.875 };
    const double c4[7] = {0, 2825.0/27648.0, 0, 18575.0/48384.0, 13525.0/55296.0, 277.0/14336.0, 0.25};
    const double c5[7] = { 0, 37.0/378.0, 0.0, 250.0/621.0, 125.0/594.0, 0.0, 512.0/1771.0 };
    const double b2[2] = {0, 0.2};
    const double b3[3] = {0, 3.0/40.0, 9.0/40.0};
    const double b4[4] = {0, 0.3, -0.9, 1.2};
    const double b5[5] = {0, -11.0/54.0, 2.5, -70.0/27.0, 35.0/27.0};
    const double b6[6] = {0, 1631.0/55296.0, 175.0/512.0, 575.0/13824.0, 44275.0/110592.0, 253.0/4096.0};

    OdeFunc& ode_func = (*this);

    k1 = h*ode_func(y,       x);
    k2 = h*ode_func(y+b2[1]*k1,x+a[2]*h);
    k3 = h*ode_func(y+b3[1]*k1+b3[2]*k2,x+a[3]*h);
    k4 = h*ode_func(y+b4[1]*k1+b4[2]*k2+b4[3]*k3,x+a[4]*h);
    k5 = h*ode_func(y+b5[1]*k1+b5[2]*k2+b5[3]*k3+b5[4]*k4,x+a[5]*h);
    k6 = h*ode_func(y+b6[1]*k1+b6[2]*k2+b6[3]*k3+b6[4]*k4+b6[5]*k5,x+a[6]*h);

    // 5th order R-K step and 5th/4th order diff
    y5 = y + c5[1]*k1 + c5[2]*k2 + c5[3]*k3 + c5[4]*k4 + c5[5]*k5 + c5[6]*k6;
    dy =
        (c5[1]-c4[1])*k1 + 
        (c5[2]-c4[2])*k2 +
        (c5[3]-c4[3])*k3 + 
        (c5[4]-c4[4])*k4 +
        (c5[5]-c4[5])*k5 + 
        (c5[6]-c4[6])*k6;
    
//    double err = vector_magnitude(dy);
    double err = 0.0;
    for (int ii=0; ii<3; ++ii) {
        double mag = sqrt(abs(dy(ii)*complex_conjugate(dy(ii))));
        if (mag > err) err = mag;
    }

//    double prc = prec * vector_magnitude(y);
    double prc = prec;
    double ratio;
    if (err == 0.0) {
        cerr << "zero error\n";
        ratio = 2.0;
    }
    else ratio = prc/err;

    // NumRec's "Safety" of a few percent less than 1.
    const double S = 0.95;

    if (ratio > 1) {
        // accept and try next step at a further distance
//        cerr << "+ " << h;
//        double mult = S*pow(ratio,0.01);
        double mult = S*pow(ratio,0.10);

        if (mult > 1.00001)  {
//            cerr << "Conditioning: h=" << h << " mult=" << mult << endl;
            cerr << ".";
            h *= mult;
            return this->runge_kutta_adaptive_stepper(x, h, y, prec);
        }

//        double mult = S*pow(ratio,0.20);
        cerr << "+";
#if 0
        cerr << h
             << " * " <<  mult
             << " r= " << ratio 
             << " = " << prc << " / " << err
             << " at " << x
             << endl;
#endif

        this->SaveStep(y5,x+h);
        h *= mult;
        return y5;
    }

//    cerr << " at " << x << ", decrease step from " << h;
//    h = S*h*pow(ratio,0.0125);
    h = S*h*pow(ratio,0.125);
//    h = S*h*pow(ratio,0.25);
//    cerr << " to " << h << endl;
//    cerr <<  "- " << h
//         << " r= " << ratio 
//         << " = " << prc << " / " << err
//         << " at " << x 
//         << endl;
    cerr << "\n-\a\n";
    return this->runge_kutta_adaptive_stepper(x, h, y, prec);
}
