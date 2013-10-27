#!/bin/bash

# build, install and use nuosc in an in-source manner
# this is a stop-gap until the installation is imporoved.

nuosc_dir=$(dirname $(readlink -f $BASH_SOURCE))

nuosc_build () {
    pushd $nuosc_dir > /dev/null 2>&1
    ./waf --out=tmp --prefix=install configure build install
    popd > /dev/null 2>&1
}

export PYTHONPATH=$nuosc_dir/python:$PYTHONPATH
PATH=$nuosc_dir/install/bin:$PATH

alias waf=$nuosc_dir/waf
    
