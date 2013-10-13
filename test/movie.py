#!/usr/bin/env python

import os
import nuosc
from collections import defaultdict
import ROOT

title_format = 'Oscillation Probability at {bl:.0f} km'

def make_frame_graphs(fd, *hargs, **params):
    graphs = []
    for flavor, color in enumerate([ROOT.kRed, ROOT.kGreen, ROOT.kBlue]):
        g = ROOT.TGraph()
        graphs.append(g)
        g.SetLineColor(color)
        g.SetLineWidth(2)
        for ibin,d in enumerate(sorted(fd)):
            g.SetPoint(ibin, d.en, d[2+flavor])

    return graphs

def draw_frame_graphs(canvas, bounds, graphs):
    graphs.reverse()            # put nue last/top
    f = canvas.DrawFrame(bounds[0],0,bounds[1],1,"")
    for g in graphs:
        g.Draw("L")
    return f

def format_frame(frame, **params):
    frame.SetTitle(title_format.format(**params))
    frame.SetXTitle('E_#nu (GeV)')
    frame.SetYTitle('Probability')

def make_frames(prob_data, outpat, **params):
    ext = os.path.splitext(outpat)[1][1:]
    hargs = (len(prob_data.en), min(prob_data.en), max(prob_data.en))
    frame_data = defaultdict(set)
    for d in prob_data.data:
        frame_data[d.bl].add(d)
    canvas = ROOT.TCanvas()
    for ind,(bl,fd) in enumerate(sorted(frame_data.items())):
        form = dict(params, ind=ind,ext=ext,bl=bl)
        graphs = make_frame_graphs(fd, *hargs, **form)
        frame = draw_frame_graphs(canvas,hargs[1:3], graphs)
        format_frame(frame, **form)
        outfile = outpat.format(**form)
        canvas.Print(outfile,ext)
    
def main(cmdline_file, outpat):
    outdir = os.path.dirname(outpat)
    data_file = os.path.join(outdir,'magic.nod')
    params = nuosc.read_param_file(cmdline_file)
    params['data'] = data_file
    params['cmdline'] = os.path.join(outdir,'magic.nop')
    params['info'] = '/dev/null'
    nuosc.call(**params)
    prob_data = nuosc.read_data(data_file)
    make_frames(prob_data, outpat, **params)
        
if '__main__' == __name__:
    import sys
    main(*sys.argv[1:])
