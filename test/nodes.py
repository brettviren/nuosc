#!/usr/bin/env
'''
Make nodes plot
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
