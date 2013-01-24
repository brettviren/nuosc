#!/usr/bin/env python
'''
Look at sending neutrinos to the south pole.

This executes the "nuosc" program which must be installed into PATH
'''

import os
import json
import time
#import pickle
import cPickle as pickle
import shelve
import subprocess
from collections import namedtuple
import ROOT

class OscParams(namedtuple('OscParams','th12 th23 th13 cp dm21 dm31')):
    __slots__ = ()
    
    def __str__(self):
        return 'OscParams: (th12=%s t23=%s th13=%s cp=%s)deg (dm21=%s dm31=%s)eV' %\
            (self.th12, self.th23, self.th13, self.cp, self.dm21, self.dm31)
    def dict(self):
        d = dict()
        for k in self._fields:
            d[k] = str(self.__dict__[k])
        return d
    
    def mixing(self):
        return 'ang:%s,%s,%s' % (self.th12,self.th23,self.th13)

class OtherParams(namedtuple('OtherParams','neutrino energy baseline density calculation')):
    __slots__ = ()
    
    def __str__(self):
        return 'OtherParams: ' + str(self.__dict__)
    def dict(self):
        d = dict()
        for k in self._fields:
            d[k] = str(self.__dict__[k])
        return d

class OscProb(namedtuple('OscProb','energy baseline a b c en bl')):
    __slots__ = ()
    
    def __str__(self):
        return 'OscProb: ' + str(self.__dict__)
    def dict(self):
        return self.__dict__


class Nuosc(object):

    nuosc_command = os.environ.get("NUOSC","nuosc")

    def __init__(self, osc, other):
        self.osc = osc
        self.other = other
        self._data = None

    def dict(self):
        ret = dict(mixing = self.osc.mixing(), 
                   sol = '%s' % self.osc.dm21,
                   atm = '%s' % self.osc.dm31)
        ret.update(self.other.dict())
        return ret
               
    def __str__(self):
        return '%s, %s' % (self.osc, self.other)

    def _gen(self):
        cmd = [self.nuosc_command]
        for k,v in sorted(self.dict().items()):
            cmd += ['--'+k,str(v)]
        print ' '.join(cmd)
        from subprocess import Popen, PIPE
        proc = Popen(cmd, stderr=PIPE, stdout=PIPE)
        proc.wait()
        if proc.returncode:
            print ' '.join(cmd)
            print proc.stdout
            print proc.stderr
            raise RuntimeError, 'Error return code %d' % (proc.returncode,)
        
        self._info = proc.stderr.read()

        probs = []
        for line in proc.stdout.readlines():
            p = OscProb(*map(float,line.strip().split()))
            probs.append(p)
        return probs

    def data(self):
        if not self._data:
            self._data = self._gen()
        return self._data

    pass

class NuoscFactory(object):
    


    def __init__(self, cache_file = '/dev/shm/NuoscFactory.cache'):
        self.shelf = shelve.open(cache_file)
        return

    def dumps_key(self, osc, oth):
        'Form a str based on the osc/other params'
        return json.dumps( (sorted(osc.dict().items()), sorted(oth.dict().items())) )

    def loads_key(self, key):
        'Return (OscParams,OtherParams) tuple built from str from dumps_key()'
        oscitems,keyitems = json.loads(key)
        osc = {k:v for k,v in oscitems}
        oth = {k:v for k,v in othitems}
        return OscParams(**osc), OtherParams(**oth)

    def __call__(self, osc, oth):
        'Get Nuosc associated with (OscParams, OtherParams)'
        key = self.dumps_key(osc,oth)
        val = self.shelf.get(key)
        if val: return val
        val = Nuosc(osc, oth)
        val.data()
        self.shelf[key] = val
        return val

    def __del__(self):
        self.close()
    def close(self):
        self.shelf.close()

    pass
factory = NuoscFactory()        

# Approximately his numbers, ignoring NH/IH differences and taking
# care of his \Delta m^2 defintion http://arxiv.org/abs/1205.5254
#RADIANS: th12=0.587, th23=0.670, th13=0.156, 
fogli = OscParams(th12=33.6, th23=38.4, th13=8.94,
                  cp=0.0, dm21=7.54e-5, dm31=2.47e-3)

print fogli
# FNAL is at 41.841 degrees latitude, assume spherical earth
fnal_pole_baseline = str(int(1.826*6371.0)) # km
numu_fnal_pole = OtherParams(neutrino=2, energy='lin:0.5,25,.1', baseline='one:%s'%fnal_pole_baseline, 
                             density="prem", calculation="matrix")
nominal_osc = fogli
nominal_oth = numu_fnal_pole
nominal_nuosc = factory(fogli, numu_fnal_pole)

def change(osc = nominal_osc, oth=nominal_oth, **kwds):
    d = osc.dict()
    osc = {k:str(kwds.get(k,d.get(k))) for k in d.keys()}

    d = oth.dict()
    oth = {k:str(kwds.get(k,d.get(k))) for k in d.keys()}

    return factory(OscParams(**osc), OtherParams(**oth))

def get_tgraphs(params):
    pa = ROOT.TGraph()
    pb = ROOT.TGraph()
    pc = ROOT.TGraph()

    for count,prob in enumerate(params.data()):
        pa.SetPoint(count, prob.energy, prob.a)
        pb.SetPoint(count, prob.energy, prob.b)
        pc.SetPoint(count, prob.energy, prob.c)
        continue
    pa.SetLineColor(2)
    pc.SetLineColor(4)
    return (pa,pb,pc)




def draw_progression(canvas):

    pdf_file = '/dev/shm/pole.pdf'
    def cprint(extra=''):
        canvas.Print(pdf_file + extra,"pdf")

    nu_sign_types = [(+1,'nu'),(-1,'anu')]
    nunames = ['nue','numu','nutau']
    dens = [('PREM','prem'), ('CON','con:3.0'), ('VAC','con:0')]
    maxe = 25       ; stepe = 0.1
    maxbl_Mm = 12.5 ; stepbl_Mm = 0.1   # mega-meters
    max_pnue = 0.5  ; max_pnux = 1.0
    
    def make_hname(den, nutype, nuname):
        return '_'.join([den, nutype, nuname])
    def make_htitle(den, nutype, nuname):
        inu = 'numu'
        if nutype == 'anu':
            inu = 'anti-%s'%inu
            nuname = 'anti-%s'%nuname
        return ' '.join([den.upper(),inu,'-->',nuname])


    # make "oscilligram" hists
    prob_graphs = {}
    og_hists = {}
    for den_name, den_arg in dens:
        for nuname in nunames:
            for nusign, nutype in nu_sign_types:

                hname = make_hname(den_name, nutype, nuname)
                htitle = make_htitle(den_name, nutype, nuname)

                h = ROOT.TH2F(hname, 'OscGram for ' + htitle,
                              int(maxe/stepe)  , 0.0, float(maxe),
                              int(maxbl_Mm/stepbl_Mm), 0.0, maxbl_Mm)
                h.SetXTitle('Energy (GeV)')
                h.SetYTitle('Baseline (Mm)')
                              
                h.SetStats(0)
                if nuname == 'nue':
                    h.SetMaximum(max_pnue)
                else:
                    h.SetMaximum(max_pnux)
                og_hists[h.GetName()] = h
                continue
            continue
        continue

                    
    cprint("[")

    t1 = time.time()
    lbne_baseline = 1300
    for count,bl_km in enumerate(range(100, int(1000*maxbl_Mm), int(1000*stepbl_Mm))):


        canvas.Clear()
        canvas.Divide(6,3)
        
        col = -1

        for den_name, den_arg in dens:
            for nusign, nutype in nu_sign_types:
                col += 1        # range(6)

                job = change(neutrino=nusign*2, baseline=bl_km, density=den_arg,
                             energy='lin:%.1f,%.1f,%.1f' % (stepe,maxe,stepe))
                graphs = get_tgraphs(job)

                anti = ''
                if nutype == 'anu':
                    anti = 'anti'

                for row,graph in enumerate(graphs):
                    npad = col + 6*row +1
                    pad = canvas.cd(npad)
                    print col,row,npad, graph.GetN(), pad.GetName()
                    if row:
                        graph.SetMaximum(max_pnux) # numu/nutau
                    else:
                        graph.SetMaximum(max_pnue) # nue

                    nam = '%s_%snumu_to_%s%s_at_%skm'%(den_name,anti,anti,nunames[row],bl_km)
                    graph.SetName(nam)
                    prob_graphs[nam] = graph # keep alive
                    tit = '%s %snumu to %s%s at %skm'%(den_name,anti,anti,nunames[row],bl_km)
                    graph.SetTitle(tit)
                    graph.Draw("AL")

                    hname = make_hname(den_name, nutype, nunames[row])
                    h = og_hists[hname]
                    for n in range(graph.GetN()):
                        d = bl_km/1000.0 + 0.5*stepbl_Mm  # in case of roundoff err
                        e = graph.GetX()[n] + 0.5*stepe   # land in middle of bin
                        p = graph.GetY()[n]
                        h.Fill(e, d, p)
                        continue
                    continue
            continue

        # drew 6x3 canvas

        canvas.Modified()
        canvas.Update()
        cprint()
        continue

    factory.shelf.sync()

    
    for den_name, den_arg in dens:
        canvas.Clear()
        canvas.Divide(3,2)

        for row, (nusign, nutype) in enumerate(nu_sign_types):
            for col, nuname in enumerate(nunames):
                npad = col + row*3 +1
                pad = canvas.cd(npad)

                hname = make_hname(den_name, nutype, nuname)
                h = og_hists[hname]
                h.Draw("colz")
                continue
            continue
        
        canvas.Modified()
        canvas.Update()
        cprint()
        continue

    cprint(']')
    t2 = time.time()
    print t2-t1, 'seconds'

    return (prob_graphs, og_hists);
