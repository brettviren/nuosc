# -*- python -*-
import commands,os

# point to where blitz++ is installed
BLITZ = '/usr'

# Where to install: PREFIX/{lib,bin,include}
PREFIX = os.path.expanduser("~")
inst=PREFIX+'/bin'
print 'will install to %s'%inst

# rest is not changed.
env = Environment(CCFLAGS = '-g -Wall', CPPPATH='.')
env.SharedLibrary('nuosc++',commands.getoutput("ls *.cc").split("\n"))

prg = Environment(CCFLAGS = '-g -Wall', CPPPATH=['.','main'])
prg.Program('main/genprob',['main/genprob.cc', 'main/options.cc'],\
            LIBS=['nuosc++','blitz'], LIBPATH=['#',BLITZ+'/lib'])
prg.Install(inst,'main/genprob')
prg.Alias('install',inst)


