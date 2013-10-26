#!/usr/bin/env python

import os
import os.path as osp
from subprocess import check_output, check_call
from collections import namedtuple

from ConfigParser import SafeConfigParser

OscProb = namedtuple('OscProb', 'en bl a b c enl bll')
OscProbSet = namedtuple('OscProbSet', 'data en bl enl bll')

def config(filename):
    '''Return a dictionary keyed by the section names in the
    configuration file name(s).  Any section "defaults" will be loaded
    first.  Subsequent sections will inherit this default.'''

    cfg = SafeConfigParser()
    cfg.read(filename)
    base = dict(cfg.items("defaults"))
    sections = cfg.sections()
    sections.remove("defaults")
    ret = dict()
    for section in sections:
        sec = dict(base, **dict(cfg.items(section)))
        ret[section] = sec
    return ret

def config_sections(filename):
    from ConfigParser import SafeConfigParser
    cfg = SafeConfigParser()
    cfg.read(filename)
    sections = cfg.sections()
    sections.remove("defaults")
    return sections

def cfg2nops(cfg_file, sections, outfiles):
    '''Write each <section> from <cfg_file> to co responding <outfile>.
    '''
    cfg = config(cfg_file)
    for sec,ofn in zip(sections,outfiles):
        print 'SECTION:',sec,ofn
        params = cfg[sec]
        write_param_file(ofn, **params)
    return

def nop2nod(nopfile, nodfile):
    params = read_param_file(nopfile)
    params['data'] = nodfile
    params['info'] = '/dev/null'
    call(**params)
    return

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


def read_data(filename):
    '''
    Read a nuosc data file and provide the data as an OscProbSet
    '''
    with open(filename) as fp:
        return datafy(fp.readlines())

def datafy(lines):
    '''
    Parse lines as returned by the nuosc executable into an OscProbSet
    '''
    sets = [set() for x in range(5)]

    for line in lines:
        line = line.strip()
        if not line:
            continue
        parts = OscProb(*[float(x.strip()) for x in line.split()])
        sets[0].add(parts)
        sets[1].add(parts[0])
        sets[2].add(parts[1])
        sets[3].add(parts[5])
        sets[4].add(parts[6])
    return OscProbSet(*sets)

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

    if kwds.has_key('data'):
        check_call(cmd, shell=True)
        return 

    ret = []
    out = check_output(cmd, shell=True)
    for line in out.split('\n'):
        line = line.strip()
        if not line:
            continue
        data = map(float, line.split())
        ret.append(data)
    return ret
    
    
