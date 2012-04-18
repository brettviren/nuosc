#!/bin/bash

# Run from examples/ like ./all.sh

# To run nuxVbl.sh you need nuosc in ../
# To run nuxVbl.py you need PyROOT ('python -c "import ROOT"' must work)

do_fine() {
    foo=$(./nuxVbl.sh 1 0.004 20 0.01) && python nuxVbl.py $foo && echo $foo.pdf
    foo=$(./nuxVbl.sh 1 0.004 200 0.1) && python nuxVbl.py $foo && echo $foo.pdf
    foo=$(./nuxVbl.sh 2 1 2000 1) && python nuxVbl.py $foo && echo $foo.pdf
    foo=$(./nuxVbl.sh 2 1 20000 10) && python nuxVbl.py $foo && echo $foo.pdf
}
do_coarse () {
    foo=$(./nuxVbl.sh 1 0.004 20 0.1) && python nuxVbl.py $foo && echo $foo.pdf
    foo=$(./nuxVbl.sh 1 0.004 200 1) && python nuxVbl.py $foo && echo $foo.pdf
    foo=$(./nuxVbl.sh 2 1 2000 10) && python nuxVbl.py $foo && echo $foo.pdf
    foo=$(./nuxVbl.sh 2 1 20000 100) && python nuxVbl.py $foo && echo $foo.pdf
}

do_fine
