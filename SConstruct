#!/usr/bin/env scons  # -*- python -*-

pkg = 'nuosc++'

AddOption('--prefix',
          dest='prefix',
          type='string',
          nargs=1,
          action='store',
          metavar='DIR',
          help='installation prefix')


VariantDir('build/source','source',duplicate=0)
VariantDir('build/main','main',duplicate=0)

# The shared library
env = Environment(CPPPATH = ['include'],PREFIX = GetOption('prefix'))
so = env.SharedLibrary(pkg,Glob('build/source/*.cc'))
env.Install('$PREFIX/lib',so)

# The nuosc executable
nuosc_source = ['build/main/nuosc.cc',
                'build/main/options.cc',
                'build/main/Config.cc',
                'build/main/ProbCalc.cc',
                'build/main/util.cc']
#nuosc_env = Environment(CPPPATH = ['include','main'])
exe = env.Program('nuosc',nuosc_source,PREFIX = GetOption('prefix'),
                        LIBS=[pkg,'blitz'], LIBPATH='.',RPATH='.')
env.Install('$PREFIX/bin/',exe)

