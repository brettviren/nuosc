#!/bin/bash
#
# Generate osc prob for 4 MeV anue->nux as function of baseline
#
# Run from example/ like:
# 
#  ./numxVbl.sh nunum gev km step

nunum=$1 ; shift		# initial nu in (1,2,3)
case $nunum in
    1) name=nue;;
    2) name=numu;;
    3) name=nutau;;
esac
gev=$1; shift			# neutrino energy in GeV
km=$1; shift			# baseline in km
step=$1; shift			# baseline step size

basename="${name}_${gev}_${km}"

export PATH=..:$PATH
export LD_LIBRARY_PATH=..
source params.sh
params="-a $dm31 -s $dm21 -m sin:$ss2t12,$ss2t23,$ss2t13 "
vec=${basename}.vec
nfo=${basename}.nfo
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

# mass hierarchy

params="-D prem -a +$dm31 -s $dm21 -m sin:$ss2t12,$ss2t23,$ss2t13 "
cp=""
vec=${basename}_nh.vec
nfo=${basename}_nh.nfo
../nuosc -n $nunum -e one:$gev -b lin:$step,$km,$step $params $cp > $vec 2> $nfo

params="-D prem -a +$dm31 -s $dm21 -m sin:$ss2t12,$ss2t23,$ss2t13 "
cp="-d +90"
vec=${basename}_nh_p90.vec
nfo=${basename}_nh_p90.nfo
../nuosc -n $nunum -e one:$gev -b lin:$step,$km,$step $params $cp > $vec 2> $nfo

params="-D prem -a +$dm31 -s $dm21 -m sin:$ss2t12,$ss2t23,$ss2t13 "
cp="-d -90"
vec=${basename}_nh_m90.vec
nfo=${basename}_nh_m90.nfo
../nuosc -n $nunum -e one:$gev -b lin:$step,$km,$step $params $cp > $vec 2> $nfo


params="-D prem -a -$dm31 -s $dm21 -m sin:$ss2t12,$ss2t23,$ss2t13 "
cp=""
vec=${basename}_rh.vec
nfo=${basename}_rh.nfo
../nuosc -n $nunum -e one:$gev -b lin:$step,$km,$step $params $cp > $vec 2> $nfo

params="-D prem -a -$dm31 -s $dm21 -m sin:$ss2t12,$ss2t23,$ss2t13 "
cp="-d +90"
vec=${basename}_rh_p90.vec
nfo=${basename}_rh_p90.nfo
../nuosc -n $nunum -e one:$gev -b lin:$step,$km,$step $params $cp > $vec 2> $nfo

params="-D prem -a -$dm31 -s $dm21 -m sin:$ss2t12,$ss2t23,$ss2t13 "
cp="-d -90"
vec=${basename}_rh_m90.vec
nfo=${basename}_rh_m90.nfo
../nuosc -n $nunum -e one:$gev -b lin:$step,$km,$step $params $cp > $vec 2> $nfo
