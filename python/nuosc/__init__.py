#!/usr/bin/env python

import os
import os.path as osp
from subprocess import check_output

def find_nuosc_exec():
    to_check = os.environ['PATH'].split(':')
    for maybe_dir in to_check:
        #print 'Checking: %s' % maybe_dir
        if not osp.isdir(maybe_dir):
            continue
        maybe = osp.join(maybe_dir, 'nuosc')
        if osp.exists(maybe):
            print 'Using %s' % maybe
            return maybe
    raise RuntimeError('Failed to locate "nuosc" executable')
    

def call(**kwds):
    '''Generate oscillation probabilities using the nuosc executable.

    The keyword arguments match the long options of the "nuosc" executable.

    In addition "executable" can set the path to the "nuosc"
    executable, otherwise PATH will be searched.

    '''
    exe = kwds.pop('executable',None) or find_nuosc_exec()
    args  = [exe]
    args += ['--%s %s' % kv for kv in kwds.items()]
    cmd = ' '.join(args)
    out = check_output(cmd, shell=True)
    for line in out.split('\n'):
        line = line.strip()
        if not line:
            continue
        yield map(float, line.split())
    return
    
    
