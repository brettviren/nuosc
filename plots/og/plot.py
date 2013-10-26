#!/usr/bin/env python
'''
Make an "oscillogram" plot from nuosc data
'''

import nuosc
import ROOT
import math

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
    

nu_num2rlatex = { 
    '1':'#nu_{e}',        '2':'#nu_{#mu}',        '3':'#nu_{#tau}',
    '+1':'#nu_{e}',       '+2':'#nu_{#mu}',       '+3':'#nu_{#tau}',
    '-1':'#bar{#nu}_{e}', '-2':'#bar{#nu}_{#mu}', '-3':'#bar{#nu}_{#tau}'}


def nop_params(nop, fnu):
    fnu = str(fnu)
    inu = nop['neutrino']
    if inu.startswith('-') and not fnu.startswith('-'):
        fnu = '-' + fnu
    if nop['atm'].startswith('-'):
        hier = 'IH'
    else:
        hier = 'NH'
    return dict(nop, inu=inu, fnu=fnu, hier=hier,
                inu_rlatex = nu_num2rlatex[inu],
                fnu_rlatex = nu_num2rlatex[fnu])

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


def get_axis_desc(a):
    ny = a.GetNbins()
    bot = round(a.GetBinLowEdge(0))
    top = round(a.GetBinUpEdge(ny))
    return ny,bot,top

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

def shape_linlog_og(h, blrange=(100,3000), erange=(0.2,10), zrange=(0,0.2)):
    '''
    Shape the histogram for an explicitly log energy scale but a linear plot.
    '''
    x = h.GetXaxis()
    x.SetRangeUser(*blrange)

    y = h.GetYaxis()
    ny,bot,top = get_axis_desc(y)
    y.SetBinLabel(1,str(math.pow(10, bot)))
    y.SetBinLabel(ny/2, str(math.pow(10, 0.5*(top+bot))))
    y.SetBinLabel(ny, str(math.pow(10, top)))

    z = h.GetZaxis()
    z.SetRangeUser(*zrange)
    
def make_canvas_og():
    c = ROOT.TCanvas("c","canvas",500,500)
    c.SetFrameFillColor(ROOT.gStyle.GetColorPalette(1))
    c.SetRightMargin(0.12)             
    return c

def make_text(text, x, y, a=0):
    w = ROOT.TLatex()
    w.SetTitle(text)
    w.SetX(x)
    w.SetY(y)
    w.SetTextAngle(a)
    return w


def oscillogram(nop, nod, out, fnu=1):
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
    oscillogram(*sys.argv[1:])
