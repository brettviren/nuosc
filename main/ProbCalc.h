#ifndef PROBCALC_H
#define PROBCALC_H


#include <string>

#include "OscParam.h"

class ProbCalc {
protected:

    ComplexVector nu0;
    OscParam op;

public:

    ProbCalc();
    virtual ~ProbCalc();

    void SetInitialNu(int nu_num);
    void SetOscParams(const OscParam& oscparams);

    const ComplexVector&  GetInitialNu(void);
    const OscParam& GetOscParams(void);

    virtual ComplexVector operator()(double energy, double baseline) = 0;
};


/// Probability calculator factory.  Description strings are as in
/// Config.  Return 0 if fail to parse descriptions.  Afterwards, you
/// should set teh initial neutrino and oscparams before calling
/// operator().   Caller takes ownership
ProbCalc* make_probability_calculator(std::string calc_desc,
				      std::string density_desc);


#endif  // PROBCALC_H
