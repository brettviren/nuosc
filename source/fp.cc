
#include "fp.h"

FloatXImp::FloatXImp(bool a):activate_(a)
{
    if(activate_==true) {
        // only activate invalid operand, division by zero, and
        // overflow exceptions

#if defined(__GLIBC__)&&(__GLIBC__>2 || __GLIBC__==2 && __GLIBC_MINOR__>=1)
        // use fenv
        fenv_t newval;
        fegetenv(&old_setting);
        fegetenv(&newval);
        newval.__control_word &= ~(FE_INVALID|FE_DIVBYZERO|FE_OVERFLOW);
        fesetenv(&newval);
#else
        // fpu control stuff
        old_setting = __fpu_control;
        fpu_control_t newval =
            old_setting & ~(
                _FPU_MASK_IM
                |_FPU_MASK_ZM
                |_FPU_MASK_OM
                );

        _FPU_SETCW(newval);
#endif
    }
}

FloatXImp::~FloatXImp()
{
    if(activate_==true) {
#if defined(__GLIBC__)&&(__GLIBC__>2 || __GLIBC__==2 && __GLIBC_MINOR__>=1)
        fesetenv(&old_setting);
#else
        _FPU_SETCW(old_setting);
#endif
    }
}

