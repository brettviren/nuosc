#!/usr/bin/env python
import ROOT

import os
from os import path as osp
from subprocess import check_call
from glob import glob

test_dir=osp.dirname(osp.realpath(__file__))
outdir=osp.join(test_dir,'constant_density')

def find_nuosc():
    src_dir = osp.dirname(test_dir)
    build_maybe = glob('%s/*'%src_dir)
    to_check = [src_dir] + build_maybe + os.environ['PATH'].split(':')
    for maybe_dir in to_check:
        #print 'Checking: %s' % maybe_dir
        if not osp.isdir(maybe_dir):
            continue
        maybe = osp.join(maybe_dir, 'nuosc')
        if osp.exists(maybe):
            print 'Using %s' % maybe
            return maybe
    raise RuntimeError('Failed to locate "nuosc" executable')


nuosc_pat = "{executable} -n{nu} -e{energy} -b{baseline} -D con:{density} -c {calc} -d {dcp} > {vecfile} 2> {infofile}"
nu_names = ['','nue','numu','nutau']
defaults = dict(
    executable = find_nuosc(),
    nu = 2,
    nu_name = nu_names[2],
    energy = 'lin:0.1,10,0.1',
    baseline = 'one:1300',
    density = 0,
    calc = 'matrix',
    dcp = 0,
    vecfile = '{outdir}/{calc}-nu{nu}-dens{density}-cp{dcp}.vec',
    infofile = '{outdir}/{calc}-nu{nu}-dens{density}-cp{dcp}.info',
    outdir=outdir,
    h_name = '{calc}_nu{nu}_dens{density}_cp{dcp}',
    h_title = '{calc} {nu_name} {density}g/cc CP{dcp}',
    )

def gen():
    for dcp in [0, 45, -45, 90]:
        for dens in [-1,0,3]:
            for calc in ['matrix', 'step']:
                p = dict(defaults, dcp=dcp, density=dens, calc=calc)
                for k in p:
                    v = p[k]
                    if isinstance(v, type('')):
                        v = v.format(**p)
                        p[k] = v
                yield p

def make_vecfile(**p):
    if not osp.exists(outdir):
        os.makedirs(outdir)
    already = p['vecfile']
    if osp.exists(already) and not p.get('force'):
        print 'Already produced %s' % already
        return
    cmd = nuosc_pat.format(**p)
    print cmd
    check_call(cmd, shell=True)


def make_graphs(**kwds):
    graphs = [ROOT.TGraph() for x in range(3)]
    with open(kwds['vecfile']) as fp:
        for line in fp.readlines():
            en,bl,a,b,c,_,_ = map(float, line.strip().split())
            for g,p in zip(graphs,[a,b,c]):
                g.SetPoint(g.GetN(), en, p)
    return graphs

def plot_all_numu2nue(printer):
    colors = [1,2,4]
    for p in gen():
        make_vecfile(**p)
        graphs = make_graphs(**p)
        h = ROOT.TH2F('probs_'+p['h_name'], p['h_title'],1,0,10,1,0,1)
        h.Draw()

        for g,c in zip(graphs,colors):
            g.SetLineColor(c)
            g.Draw('L')

        printer()


class Printer:
    def __init__(self, filename, canvas = None):
        self._filename = filename
        self._ext = osp.splitext(filename)[1][1:]
        self._canvas = canvas or ROOT.TCanvas()
    def __enter__(self):
        self('[')
        return self
    def __call__(self, extra=''):
        self._canvas.Print(self._filename + extra, self._ext)
    def __exit__(self, et, ev, tb):
        self(']')
        

with Printer('constant_density.pdf') as printer:
    plot_all_numu2nue(printer)


#nuosc -n2 -e lin:0.1,10,0.1 -b one:1300 -D con:$dens -c $calc -d $dcp -o $prefix/${name}.vec -i $prefix/${name}.info    
