#include "nuosc_matrix.h"

#include <iostream>
using namespace std;

int main (int argc, char *argv[])
{
    
    double theta_12=0.553574358897045; //31.7 deg => sin22t12 = 0.8*/
    double theta_23=0.785398163397448; //45.0 deg => sin22t23 = 1.0*/
    double theta_13=0.219074515292085; //12.5 deg => sin22t13

    cout << mixing_matrix(theta_12,theta_23,theta_13) << endl;

    return 0;
} // end of main()
