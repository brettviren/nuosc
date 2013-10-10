#!/usr/bin/env python

import os
import sys
import tempfile

test_dir = os.path.dirname(__file__)
src_dir = os.path.dirname(test_dir)
sys.path.insert(0,os.path.join(src_dir,'python'))

import nuosc

osc_params = {'atm': '0.0025',
              'baseline': 'one:1300',
              'calculation': 'matrix',
              'delta': '0',
              'density': 'con:0.0',
              'energy': 'one:1',
              'mixing': 'sin:0.8,1.0,0.1',
              'neutrino': '2',
              'sol': '5e-05'}

def test_param_file():
    d = dict(osc_params)
    tfd, tname = tempfile.mkstemp()
    nuosc.write_param_file(tname, **d)
    d2 = nuosc.read_param_file(tname)
    assert d == d2, 'failure in %s' % tname
    os.remove(tname)


def test_call():
    tdir = tempfile.mkdtemp()
    print tdir

    info_file = os.path.join(tdir,"test.info")
    cmdl_file = os.path.join(tdir,"test.cmdline")

    d = dict(osc_params, info = info_file, cmdline = cmdl_file)
    res = [x for x in nuosc.call(**d)]
    assert res, "No results for: %s" % str(d)
    
    args = nuosc.read_param_file(cmdl_file)
    res2 = [x for x in nuosc.call(**args)]
    
    print 'res:', res
    print 'res2:', res2
    for x1,x2 in zip(res, res2):
        assert x1 == x2
        print x1
    

if '__main__' == __name__:
    test_param_file()
    test_call()
