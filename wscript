#!/usr/bin/env python
# encoding: utf-8
'''
This is a waf wscript file.  It builds this package.  To use it do:

  ./waf --prefix=/path/to/install configure
  ./waf

'''

import os
import sys

top = '.'
out = 'tmp'

def options(opt):
    opt.load('compiler_cxx')

def configure(conf):
    conf.load('compiler_cxx') 

    conf.env.append_unique('CXXFLAGS', ['-O2', '-g']) 

    conf.check(lib='blitz',
               compiler='cxx',
               uselib_store='BLITZ',
               mandatory=True)

    pydir = os.path.join(conf.path.abspath(), 'python')
    sys.path.insert(0,pydir)
    conf.env.env = dict(os.environ)
    conf.env.env['PYTHONPATH'] = os.path.pathsep.join(sys.path)

    conf.recurse('test')

def build(bld):
    pydir = os.path.join(bld.path.abspath(), 'python')
    sys.path.insert(0,pydir)

    lib_source = bld.path.ant_glob('source/*.cc')
    main_source = bld.path.ant_glob('main/*.cc')

    bld.program(
        name = 'nuosc_exe',
        source = main_source, 
        includes = './include ./main',
        target = 'nuosc', 
        use = 'nuosc_static BLITZ') 

    bld.stlib(
        source = lib_source,  
        includes = './include', 
        target = 'nuosc_static',
        install_as = '${PREFIX}/lib/libnuosc++.a')

    bld.install_files(
        '${PREFIX}/include/nuosc', 
        bld.path.ant_glob('include/*.h'))

    bld.shlib(
        source = lib_source,  
        includes = './include', 
        target = 'nuosc++',
        install_path='${PREFIX}/lib') 

    bld.recurse('test')

def test(tst):
    tst.recurse('test')
