#!/usr/bin/env python
import ROOT

import os
import nuosc

def get_params():
    z = dict(
        neutrino = '2',
        energy = 'lin:0.01,10,0.01',
        baseline = 'one:1300',
        mixing = 'sin:0.7584,0.948,0.094',
        sol = '7.5e-5',
        atm = '2.5e-3',
        delta = '0',
        density = 'prem',
        calculation = 'matrix',
        info = '/dev/null'
        )
    m45 = dict(z, delta='-45')
    m90 = dict(z, delta='-90')
    p45 = dict(z, delta='+45')
    p90 = dict(z, delta='+90')

    return (m90,m45,z,p45,p90)

def get_graph(nu, **p):
    data = nuosc.call(**p)
    g = ROOT.TGraph()
    for ind, d in enumerate(data):
        prob = d[2+nu]
        energy = d[0]
        g.SetPoint(ind, energy, prob)
    print p['delta'], len(data)
    return g


def plot(canvas, graphs):
    canvas.Clear()
    frame = canvas.DrawFrame(0,0,10,0.25,"Prob numu to nue")
    canvas.Modified()
    canvas.Update()
    print 'Frame at',frame
    #frame.SetTitle('P(#nu_#mu #rightarrow #nu_e')
    frame.SetXTitle('E_#nu (GeV)')
    frame.SetYTitle('Probability')
    thicks = [2,2,1,2,2]
    colors = [4,4,1,2,2]
    styles = [2,1,1,1,2]
    titles = ['-90','-45','0','+45','+90']
    leg = ROOT.TLegend(5, .1, 8, .2, "", "")
    leg.SetFillColor(ROOT.kWhite)
    for ind, g in enumerate(graphs):
        print ind,colors[ind],g
        g.SetFillStyle(0)
        g.SetLineColor(colors[ind])
        g.SetLineStyle(styles[ind])
        g.SetLineWidth(thicks[ind])
        g.Draw('L')
        leg.AddEntry(g, titles[ind], 'l')
    leg.Draw("same")

    return frame,leg

def main():
    params = get_params()
    graphs = [get_graph(0, **p) for p in params]
    canvas = ROOT.TCanvas()
    garbage = plot(canvas, graphs)
    outname = 'test_cp_phi'
    for ext in ['pdf','png','svg']:
        canvas.Print(outname+'.'+ext,ext)

    print 'exiting'

if '__main__' == __name__:
    main()


