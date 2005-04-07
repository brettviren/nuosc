#include "OscParam.h"

int main (int argc, char *argv[])
{
    OscParam oparam;          // defaults
    OscParam* op2 = new OscParam(oparam);

    op2->get_matter_matrix();


    return 0;
} // end of main()
