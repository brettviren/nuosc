#!/usr/bin/env
'''
Make nodes plot
'''

import os
import ROOT
import nuosc

def make_hist(hist_maker, *args):
    h = hist_maker(*args)
    h.SetDirectory(0)
    ROOT.SetOwnership(h,0)
    return h
    
def h2d(*args):
    return make_hist(ROOT.TH2D, *args)
def h1d(*args):
    return make_hist(ROOT.TH1D, *args)

nu_num2name = { '1':'nue',   '2':'numu',   '3':'nutau',
               '-1':'anue', '-2':'anumu', '-3':'anutau'}

nu_num2rlatex = { '1':'#nu_e',        '2':'#nu_#mu',        '3':'#nu_#tau',
                 '-1':'#bar{#nu}_e', '-2':'#bar{#nu}_#mu', '-3':'#bar{#nu}_#tau'}

def params_add_final_nu(nu, **params):
    inu = params['neutrino']
    fnu = str(nu)
    if inu.startswith('-'):
        fnu = '-' + fnu
    params.update(inu=inu,fnu=fnu,
                  inu_name = nu_num2name[inu],
                  fnu_name = nu_num2name[fnu],
#                  inu_rlatex = nu_num2rlatex[inu],
#                  fnu_rlatex = nu_num2rlatex[fnu],
                  inu_rlatex = nu_num2name[inu],
                  fnu_rlatex = nu_num2name[fnu])
    return params


def og_binargs(prob):
    return (len(prob.bl),min(prob.bl),max(prob.bl),len(prob.en),min(prob.en),max(prob.en))

def og_book(binargs, name_pat = 'og_{inu_name}2{fnu_name}', title_pat = 'P {inu_name} to {fnu_name}', **params):
    name = name_pat.format(**params)
    title = title_pat.format(**params)
    return h2d(name, title, *binargs)

def og_fill(hist, nu_ind, prob):
    for d in prob.data:
        en,bl = d[:2]
        p = d[2+nu_ind]
        hist.Fill(bl, en, p)
    return

def og_three(prob, **params):
    ba = og_binargs(prob)
    ret = []
    for ind in range(3):
        p = params_add_final_nu(str(ind+1), **params)
        h = og_book(ba, **p)
        og_fill(h, ind, prob)
        ret.append(h)
    return ret


def pbl_book(binargs, name_pat = 'pbl_{inu_name}2{fnu_name}_{bl_name}',
             title_pat = 'P {inu_name} to {fnu_name} at {blname}km', **params):
    name = name_pat.format(**params)
    title = title_pat.format(**params)
    return h1d(name, title, *binargs)
    

def pbl_fill(hist, ind, prob):
    for d in prob.data:
        en,bl = d[:2]
        p = d[2+nu_ind]
        hist.Fill(bl, en, p)
    
def pbl_fill_three(hists, data):
    for d in data:
        hists[0].Fill(d.en, d.a)
        hists[1].Fill(d.en, d.b)
        hists[2].Fill(d.en, d.c)
    

interesting_baselines = [1300]

def per_baseline(prob, **params):
    from collections import defaultdict
    prob_bybl = defaultdict(set)
    for d in prob.data:
        if int(d.bl) in interesting_baselines:
            prob_bybl[d.bl].add(d)

    ba = (len(prob.en),min(prob.en),max(prob.en))

    peach = []
    for x in range(3):
        p = params_add_final_nu(x+1, **params)    
        peach.append(p)

    ret = list()
    for bl, data in prob_bybl.items():
        hists = []
        for p in peach:
            p['blname'] = str(bl)
            p['bl_name'] = str(bl).replace('.','_')
            h = pbl_book(ba, **p)
            hists.append(h)
        pbl_fill_three(hists, data)
        ret += hists
    return ret

def main(rootfile, params_file, data_file, *plots):
    params = nuosc.read_param_file(params_file)
    data = nuosc.read_data(data_file)

    fp = ROOT.TFile.Open(rootfile, 'recreate')
    allres = []
    for plotter in plots:
        meth = eval(plotter)
        res = meth(data, **params)
        for r in res:
            r.SetDirectory(fp)
            #r.Write()
        allres += res
    fp.Write()
    return fp,allres

    
if '__main__' == __name__:
    import sys
    main(*sys.argv[1:])
