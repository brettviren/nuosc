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

def tar_it_up(source, target, env, for_signature):
    sl = ['cd ..','rm -f %s'%target[0]]
    sl.append('tar -czf %s libnuosc++/{%s}'%(target[0],','.join(map(str,source))))
    cmd = ' && '.join(sl)
    return cmd


def get_source(l,dir,fnames):
    if not dir in ['.','./test','./main']:
        return
    print dir
    fnames.remove('CVS')
    for n in fnames:
        file=None
        if n in ['Makefile', 'README', 'INSTALL', 'config.mk', 'SConstruct']:
            file = n
        elif os.path.splitext(n)[1] in ['.h','.cc']:
            file = n
        if file:
            l.append(dir+'/'+n)

bld = Builder(generator = tar_it_up,suffix = '.tar.gz')
dst = Environment(BUILDERS = {'Dist' : bld})

filelist = []
os.path.walk('.',get_source,filelist)

dst.Dist('libnuosc++.tar.gz',filelist)
dst.Alias('dist','libnuosc++.tar.gz')
