#!/usr/bin/env python
'''
Use nuosc executable to make an "oscillogram" plot.
'''

import math
import ROOT
import nuosc
from nuosc.util import format_dict


fineness = 0.1

limits = dict(
    de = 0.1*fineness,
    dl = 100.0*fineness,
    maxe = 10.0,
    maxl = 10000.0,
)

oscparams = dict(
    neutrino=2, 
    energy='lin:{de},{maxe},{de}', 
    baseline='lin:{dl},{maxl},{dl}',
    density='prem', 
    calculation='matrix'
)
# fixme: pick latest best fit
oscparams = format_dict(oscparams, **limits)

def make_hist(name, **kwds):
    ne = int(math.ceil(kwds['maxe']/kwds['de']))
    nl = int(math.ceil(kwds['maxl']/kwds['dl']))
    print '"{name}" energy: {ne},{de},{maxe}, {nl},{dl},{maxl}'.format(name=name,ne=ne,nl=nl,**kwds)
    

    h =  ROOT.TH2F(name,'numu -> %s'%name, 
                   ne, 0, kwds['maxe'],
                   nl, 0, kwds['maxl'])
    h.SetStats(0)
    return h

allcfg = dict(oscparams, **limits)
hists = list()
for name in ['nue','numu','nutau']:
    h = make_hist(name, **allcfg)
    hists.append(h)

e_eps = 0.5 * limits['de']
l_eps = 0.5 * limits['dl']

for e,l,a,b,c,_,_ in nuosc.call(**oscparams):
    for h,p in zip(hists, [a,b,c]):
        h.Fill(e+e_eps, l+l_eps, p)

fp = ROOT.TFile('oscillogram.root','recreate')
for h in hists:
    h.Write()
fp.Close()
