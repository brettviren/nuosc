#!/usr/bin/env python
# encoding: utf-8
'''
This is a waf wscript file.  It builds this package.  To use it do:

  ./waf --prefix=/path/to/install configure
  ./waf

'''

def options(opt):
    opt.load('compiler_cxx')

def configure(conf):
    conf.load('compiler_cxx') 
    conf.check(lib='blitz',
               compiler='cxx',
               uselib_store='BLITZ',
               mandatory=True)

def build(bld):
    lib_source = bld.path.ant_glob('source/*.cc')
    main_source = bld.path.ant_glob('main/*.cc')

    bld.program(
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

