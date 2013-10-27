#!/usr/bin/env python
'''
Make an "oscillogram" plot from nuosc data
'''
import math
import nuosc

import common
canvas = common.canvas
ROOT = common.ROOT

from common import get_axis_desc, set_energy_axis_label, nop_params


def enlbbl(opd, nu):
    '''Plot a ROOT TH2F using the .enl vs. .bbl of the osc prob data <opd>
    for index neutrino <nu> (0,1,2).
    '''
    nbbins = len(opd.bll)
    nebins = len(opd.enl)

    h = ROOT.TF2F("og","oscillogram",
                  nbbins, min(opd.bll), max(opd.bll),
                  nebins, min(opd.enl), max(opd.enl))
    for d in opd.data:
        p = d[2+nu]
        h.Fill(d.bll, d.enl, p)
    return h


def make_og(nod, fnu=1):
    'Make a simple OG based on the nu. osc. data and which neutrino.'
    h = ROOT.TH2F("og","oscillogram",
                  len(nod.bll), min(nod.bll), max(nod.bll),
                  len(nod.enl), min(nod.enl), max(nod.enl))
    for d in nod.data:
        # d=en,bl,e,mu,tau,enl,bll
        h.Fill(d.bll, d.enl, d[1+fnu])
    return h
    
def title_og(h, **params):
    tit = 'P(%(inu_rlatex)s #rightarrow %(fnu_rlatex)s), %(hier)s, #delta_{cp}=%(delta)s'
    h.SetTitle(tit % params)
    x = h.GetXaxis()
    x.SetTitle('Baseline (km)')
    x.CenterTitle()
    y = h.GetYaxis()
    y.SetTitle('E_{#nu} (GeV)')
    y.CenterTitle()
    h.SetStats(0)



def make_base_line(h, bl=1300):
    y = h.GetYaxis()
    ny,bot,top = get_axis_desc(y)
    l = ROOT.TLine(bl, bot, bl, top)
    l.SetLineWidth(2)
    return l

def make_vac_osc(h, lovere):
    g = ROOT.TGraph()
    nx,xmin,xmax = get_axis_desc(h.GetXaxis())
    dx = (xmax-xmin)/nx
    for ind in range(nx):
        if not ind: continue
        bl = xmin + dx*ind
        e = bl/lovere
        g.SetPoint(g.GetN(),bl, math.log10(e))
    g.SetLineWidth(2)
    return g

def shape_linlog_og(h, blrange=(100,3000), erange=(-1,1), zrange=(0,0.2)):
    '''
    Shape the histogram for an explicitly log energy scale but a linear plot.
    '''
    x = h.GetXaxis()
    x.SetRangeUser(*blrange)

    y = h.GetYaxis()
    y.SetRangeUser(*erange)
    set_energy_axis_label(y)

    z = h.GetZaxis()
    z.SetRangeUser(*zrange)
    
def make_canvas_og():
    #c = ROOT.TCanvas("c","canvas",500,500)
    canvas.Clear()
    canvas.SetFrameFillColor(ROOT.gStyle.GetColorPalette(1))
    canvas.SetRightMargin(0.12)             
    return canvas

def make_text(text, x, y, a=0):
    w = ROOT.TLatex()
    w.SetTitle(text)
    w.SetX(x)
    w.SetY(y)
    w.SetTextAngle(a)
    return w


def main(nop, nod, out, fnu=1):
    '''
    Using parameters in nop and data in nod, print a plot to out
    '''
    nop = nuosc.read_param_file(nop)
    nod = nuosc.read_data(nod)

    h = make_og(nod, fnu)
    params = nop_params(nop, fnu)
    title_og(h, **params)
    shape_linlog_og(h)

    c = make_canvas_og()
    h.Draw("CONTZ")

    lovere = math.pi / (2*1.267*abs(float(nop['atm'])))
    g1 = make_vac_osc(h, lovere)
    g2 = make_vac_osc(h, 3*lovere)
    l = make_base_line(h)
    lt = make_text("1300 km", 1250, math.log10(3), 90)
    where = 1500
    g1t = make_text("1{}^{st} vac. osc. max",where,g1.Eval(1.10*where),15)
    g2t = make_text("2{}^{nd} vac. osc. max",where,g2.Eval(1.10*where),15)

    for o in [l, g1, g2, lt, g1t, g2t]:
        o.Draw("same")
    
    c.Print(out)


if '__main__' == __name__:
    import sys
    main(*sys.argv[1:])
