#!/usr/bin/env python
'''
Plot oscillation probability as a function of baseline in a unitary way
'''

import os
import ROOT
from array import array

def gev2rootex(gev):
    energy = float(gev)
    if energy < 1:
        return '%d MeV' % int(1000*energy)
    return '%d GeV' % int(energy)

def file2cumgraphs(vecfile, ordering=range(3)):
    'Return 3 cumulatative graphs in the given order'
    prob = [ROOT.TGraph() for x in range(3)]
    fp = open(vecfile)
    for count,line in enumerate(fp):
        chunks = line.strip().split()
        gev,km,a,b,c,junk1,junk2 = map(float,chunks)

        vals = [a,b,c]
        tot = 0
        for ind in range(3):
            tot += vals[ordering[ind]]
            prob[ind].SetPoint(count,km,tot)
            continue
        continue
    return prob


def plot(basename, extra_vecfile = None):
    vecfile = basename + '.vec'
    print basename
    printfile = basename + '.pdf'
    parts = basename.split('_')
    name,gev,maxbl = parts[:3]
    maxbl = int(maxbl)
    try:
        mhv = parts[3]
        mhv = ['Normal Hierarchy','Inverted Hierarchy'][['nh','rh'].index(mhv)]
    except IndexError:
        mhv = 'Vacuum'

    # style
    colors = [2,1,4]            # [nue,numu,nutau]
    name2rootext = '#nu_x'
    ordering = range(3)
    if name == 'nue': 
        name2rootex = '#nu_{e}'
        extra_vecfile = None
    if name == 'numu':
        name2rootex = '#nu_{#mu}'
        ordering = [1, 0, 2]    # put nue between numu/nutau
    if name == 'nutau':
        name2rootex = '#nu_{#tau}'

    title = 'Probability for %s oscillation at %s (%s)' % (name2rootex,gev2rootex(gev),mhv)

    prob = file2cumgraphs(vecfile, ordering)
    for num,g in enumerate(prob):
        color = colors[ordering[num]]
        g.SetLineColor(color)
        g.SetLineWidth(1)
        g.SetFillColor(color)
        continue

    extra_prob = []
    if extra_vecfile:
        for vf in extra_vecfile:
            ep = file2cumgraphs(vf, ordering);
            ep[ordering[0]].SetLineColor(0)
            ep[ordering[0]].SetLineWidth(2)
            ep[ordering[0]].SetLineStyle(2)
            extra_prob.append(ep)
            continue
        pass

    canvas = ROOT.TCanvas("canvas","canvas",600,500)
    frame = canvas.DrawFrame(0,0,maxbl,1)
    
    frame.SetTitle(title)
    frame.SetXTitle('Baseline (km)')
    frame.SetYTitle('Probability')

    graph = ROOT.TGraph()
    prob.reverse()
    stash = []
    for count,p in enumerate(prob):
        x = p.GetX()
        y = p.GetY()
        n = p.GetN()
        h = ROOT.TH1F("p%d"%count, "prob%d"%count, n, x[0], x[n-1])
        h.SetContent(y)
        h.SetLineWidth(0)
        h.SetLineColor(p.GetLineColor())
        h.SetFillColor(p.GetLineColor())
        h.Draw("same")
        stash.append(h)
        print 'Drawing',h.GetName()
        #p.Draw("CB")

    for ep in extra_prob:
        ep[ordering[0]].Draw("C")

    canvas.Print(printfile,'pdf')
    return

if __name__ == '__main__':
    import sys
    base = sys.argv[1]
    extra = []
    for pm in ['p','m']:
        evf = '%s_%s90.vec' % (base,pm)
        if os.path.exists(evf):
            extra.append(evf)
        continue

    plot(sys.argv[1], extra)
