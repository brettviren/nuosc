#ifndef FP_H
#define FP_H
#include <fpu_control.h>
#include <fenv.h>

struct FloatXImp
{
    FloatXImp(bool activate);
    ~FloatXImp();

#if defined(__GLIBC__)&&(__GLIBC__>2 || __GLIBC__==2 && __GLIBC_MINOR__>=1)
    fenv_t old_setting;
#else
    fpu_control_t old_setting;
#endif
    bool activate_;

    static bool validate();
};

#endif
