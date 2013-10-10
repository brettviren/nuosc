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
    

def read_param_file(filename):
    '''
    return a dictionary of parameters read in from a nuosc info file.
    '''
    ret = dict()
    with open(filename) as fp:
        for line in fp.readlines():
            line = line.strip()
            if not line: continue
            if line[0] in ['#']: continue
            name,value = [x.strip() for x in line.split('=')]
            ret[name] = value
    return ret

def write_param_file(filename, **kwds):
    lines = []
    for k,v in kwds.items():
        lines.append("%s = %s" % (k, v))
    with open(filename,'w') as fp:
        fp.write('\n'.join(lines))
        fp.write('\n')
    return

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
    
    
