import math
import ROOT

canvas = ROOT.TCanvas("canvas","canvas",500,500)

def get_axis_desc(a):
    ny = a.GetNbins()
    bot = round(a.GetBinLowEdge(0))
    top = round(a.GetBinUpEdge(ny))
    return ny,bot,top

def set_energy_axis_label(axis):
    n,bot,top = get_axis_desc(axis)
    l1 = str(math.pow(10, bot))
    l2 = str(math.pow(10, 0.5*(top+bot)))
    l3 = str(math.pow(10, top))
    #print n,bot,top,"[",l1,l2,l3,"]"
    axis.SetBinLabel(1,l1)
    axis.SetBinLabel(n/2, l2)
    axis.SetBinLabel(n, l3)


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
    blname = nop['baseline'].split(':')[1]

    return dict(nop, 
                blname = blname,
                inu=inu, fnu=fnu, hier=hier,
                inu_rlatex = nu_num2rlatex[inu],
                fnu_rlatex = nu_num2rlatex[fnu])

    
