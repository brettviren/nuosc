#!/usr/bin/env python
'''
Make some probability energy spectrum plots.
'''

import os
import math
import nuosc

import common
canvas = common.canvas
ROOT = common.ROOT

from common import set_energy_axis_label, nop_params


def make_en(nod, fnu='1'):
    'Make a simple probability energy spectrum histogram'
    h = ROOT.TH1F("en","probability spectrum",
                  len(nod.enl), min(nod.enl), max(nod.enl))
    for d in nod.data:
        # d=en,bl,e,mu,tau,enl,bll
        h.Fill(d.enl, d[1+abs(int(fnu))])
    return h
    

def augment_parameters(fnu, **par):
    legend_order = ['dcp-45', 'dcp0', 'dcp+45', 't130']
    draw_order = ['dcp-45','dcp0','dcp+45','t130']
    anti_draw_order = ['dcp+45','dcp0','dcp-45','t130']
    draw_options = {
        'dcp-45': ("hbar same", 'lf', '#delta_{cp} = -#pi/2', ROOT.kBlue, 1001),
        'dcp0':   ("hbar same", 'lf', '#delta_{cp} = 0',      ROOT.kRed, 1001),
        'dcp+45': ("hbar same", 'lf', '#delta_{cp} = +#pi/2', ROOT.kGreen, 1001),
        't130':   ("L",    'lf', '#theta_{13}=0 (solar term)', ROOT.kYellow, 0)
        }
    typ = par['idname'].split('_')[-1]
    par['legend_index' ] = legend_order.index(typ)
    if par['neutrino'].startswith('-'):
        par['draw_index'] = anti_draw_order.index(typ)
    else:
        par['draw_index'] = draw_order.index(typ)
    par['draw_opts'] = draw_options[typ]
    if par['atm'].startswith("-"):
        par['hier'] = 'IH'
    else:
        par['hier'] = 'NH'
    return nop_params(par, fnu)

def hist2graph_flipped(h):
    nbins = h.GetNbinsX()
    xaxis = h.GetXaxis()
    g = ROOT.TGraph()
    for ibin in range(nbins):
        g.SetPoint(ibin, h.GetBinContent(ibin), xaxis.GetBinCenter(ibin))
    return g
    

def draw_hists(h_nops):
    drawn = list()
    for h,nop in h_nops:
        dopt,lopt,tit,col,fil = nop['draw_opts']
        if (fil):
            h.SetFillStyle(fil)
            h.SetFillColor(col)
            h.Draw(dopt)
            drawn.append(h)
        else:
            g = hist2graph_flipped(h)
            g.SetLineWidth(5)
            g.SetLineColor(col)
            h.SetLineColor(col) # for the legend
            g.Draw(dopt)
            drawn.append(g)
    return drawn

def draw_legend(h_nops):
    leg = ROOT.TLegend(0.08,math.log10(3),0.195,math.log10(9),"","") # fixme: hard coded numbers
    leg.SetLineColor(ROOT.kWhite)
    leg.SetFillColor(ROOT.kWhite)
    for h,nop in h_nops:
        dopt,lopt,tit,col,fil = nop['draw_opts']
        leg.AddEntry(h, tit, lopt)
    leg.Draw("same")
    return leg

def make_canvas():
    canvas.Clear()
    return canvas
    
def shape_hist(h, par):
    tit = 'At {blname} km'.format(**par)
    xtit = 'E_{#nu} (GeV)'
    ytit = 'P(%(inu_rlatex)s #rightarrow %(fnu_rlatex)s), %(hier)s' % par

    h.SetStats(0)
    h.SetTitle(tit)

    ea = h.GetXaxis()
    ea.SetRangeUser(-1,1)
    ea.SetTitle(xtit)
    ea.CenterTitle()

    set_energy_axis_label(ea)

    pa = h.GetYaxis()
    pa.SetRangeUser(0,0.2)
    pa.SetTitle(ytit)
    pa.CenterTitle()

def make_energy_plot(nopds, out, fnu=1):
    '''
    Plot probability energy spectrum.

    The <nopds> is a list of (nop,nod) objects.
    '''
    hists = [make_en(nod, fnu) for nop,nod in nopds]
    augs = [augment_parameters(fnu, **nop) for nop,_ in nopds]

    for h,p in zip(hists,augs):
        shape_hist(h,p)

    c = make_canvas()

    draw_order = [(nop['draw_index'],h,nop) for h,nop in zip(hists,augs)]
    draw_order.sort()
    drawn = draw_hists([h,nop] for _,h,nop in draw_order)

    legend_order = [(nop['legend_index'],h,nop) \
                    for h,(_,_,nop) in zip(drawn, draw_order)]
    #legend_order = [(nop['legend_index'],h,nop) for h,nop in zip(hists,augs)]
    legend_order.sort()
    leg = draw_legend([h,nop] for _,h,nop in legend_order)

    c.Print(out)
    return c,hists,(leg)

def main(fnu, out, *files):
    '''Main interface to making an energy spectrum plot

    <fnu> is final neutrino number to plot.

    <out> is the output file to print

    <files> is a list of .nop and .nod files.  There must be one of
    each and they must have identical  file names (ignoring extension)
    '''

    nops = []
    nods = []
    for f in sorted(files):
        if f.endswith('.nop'):
            p = nuosc.read_param_file(f)
            p['idname'] = os.path.splitext(os.path.basename(f))[0]
            nops.append(p)
        if f.endswith('.nod'):
            d = nuosc.read_data(f)
            nods.append(d)
    fnu = str(fnu)
    if nops[0]['atm'].startswith('-') and not fnu.startswith('-'):
        fnu = '-' + fnu
    return make_energy_plot(zip(nops,nods), out, fnu)


if '__main__' == __name__:
    import sys
    # hard-code the fnu for now
    main(1, *sys.argv[1:])
