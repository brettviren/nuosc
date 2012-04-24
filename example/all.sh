#!/bin/bash

# Run from examples/ like ./all.sh

# To run nuxVbl.sh you need nuosc in ../
# To run nuxVbl.py you need PyROOT ('python -c "import ROOT"' must work)

# fine and coarse binned numu->nux and nue->nux for LBNE and reactor
# energies, respectively.

do_vac() {
    foo=$(./nuxVbl.sh 1 0.004 20 0.01) && python nuxVbl.py $foo && echo $foo.pdf
    foo=$(./nuxVbl.sh 1 0.004 200 0.1) && python nuxVbl.py $foo && echo $foo.pdf
    foo=$(./nuxVbl.sh 2 1 2000 1) && python nuxVbl.py $foo && echo $foo.pdf
    foo=$(./nuxVbl.sh 2 1 20000 10) && python nuxVbl.py $foo && echo $foo.pdf
}

do_mh_4 () {
    ./nuxVbl.sh 2 4 8000 10
    python nuxVbl.py numu_4_8000
    python nuxVbl.py numu_4_8000_nh
    python nuxVbl.py numu_4_8000_rh
}

do_mh_3 () {
    ./nuxVbl.sh 2 3 3000 10
    python nuxVbl.py numu_3_3000
    python nuxVbl.py numu_3_3000_nh
    python nuxVbl.py numu_3_3000_rh
}
do_mh_1 () {
    ./nuxVbl.sh 2 1 2000 2
    python nuxVbl.py numu_1_2000
    python nuxVbl.py numu_1_2000_nh
    python nuxVbl.py numu_1_2000_rh
}
    

do_mh_1
do_mh_3
do_mh_4
do_vac

#foo=$(./nuxVbl.sh 2 1 20000 10) && python nuxVbl.py $foo && echo $foo.pdf