#!/bin/bash
#
# Generate osc prob for 4 MeV anue->nux as function of baseline
#
# Run from example/ like:
# 
#  ./numxVbl.sh

nunum=$1 ; shift
case $nunum in
    1) name=nue;;
    2) name=numu;;
    3) name=nutau;;
esac

gev=$1; shift
km=$1; shift
step=$1; shift

basename="${name}_${gev}_${km}"
vec=${basename}.vec
nfo=${basename}.nfo

export PATH=..:$PATH
export LD_LIBRARY_PATH=..
source params.sh
params="-a $dm31 -s $dm21 -m sin:$ss2t12,$ss2t23,$ss2t13 "
../nuosc -n $nunum -e one:$gev -b lin:$step,$km,$step $params > $vec 2> $nfo
echo $basename


vec=${basename}_p90.vec
nfo=${basename}_p90.nfo
cp="-d +90"
../nuosc -n $nunum -e one:$gev -b lin:$step,$km,$step $params $cp > $vec 2> $nfo

vec=${basename}_m90.vec
nfo=${basename}_m90.nfo
cp="-d -90"
../nuosc -n $nunum -e one:$gev -b lin:$step,$km,$step $params $cp > $vec 2> $nfo