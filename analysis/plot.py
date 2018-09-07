#!/usr/bin/python

#
# Imports
#
from ROOT import TH1I,TH2,TH3,TFile, TF1, TCanvas, gROOT, gPad
import matplotlib.pyplot as plt

#
# Globals
#

FILENAME="/home/frederik/geant4/user/NC2/analysis/Processed.root"

#
# Methods
#

def FitPeak(h,E):
  fitEnergy = E
  ff1 = TF1("ff1","gausn",fitEnergy-10,fitEnergy+10)
  ff1.SetParameter(0,h.Integral(h.FindBin(fitEnergy-10),h.FindBin(fitEnergy+10)))
  ff1.SetParameter(1,fitEnergy)
  ff1.SetParameter(2,1.4)
  h.Fit(ff1,"RQNL")
  ff2 = TF1("ff2","gausn(0)+[3]",ff1.GetParameter(1)-7,ff1.GetParameter(1)+7)
  ff2.SetParameter(0,ff1.GetParameter(0))
  ff2.SetParameter(1,ff1.GetParameter(1))
  ff2.SetParameter(2,1)
  ff2.SetParameter(3,1.5)
  h.Fit(ff2,"RQLN")
  result={}
  result["integral"] = ff2.GetParameter(0)
  result["max"] = ff2.GetParameter(1)
  result["width"] = ff2.GetParameter(2)
  ndf = ff2.GetNDF()
  if ndf == 0:
    ndf = 1
  result["chisqr"] = ff2.GetChisquare()/ndf
  #h.Draw()
  #raw_input("Press enter to close ...")
  return result
  
  

def Calculate_Eficiency(h,hI):
  nBins = hI.GetNbinsX()
  nDet = h.GetNbinsY()
  efficiencies = {}
  #format output datastruucture
  for det in range(1,nDet+1):
    efficiencies[h.GetYaxis().GetBinLabel(det)]=[[],[]] #for each detector, a list with energies and efficiencies    
  for b in range(1,nBins-1):
    content = hI.GetBinContent(b)
    if content > 10000:
      energy = hI.GetBinCenter(b)
      if ( energy < h.GetXaxis().GetXmax() and energy > h.GetXaxis().GetXmin() and energy < 2100): 
        # do a eff calculation for these energies
        for det in range(1,nDet+1):
          h1D = h.ProjectionX("h"+str(b)+str(det),det,det)
          #fitresult = FitPeak(h1D,energy) doesnt really work with all the duble peaks
          integral = h1D.Integral(h1D.FindBin(energy-10),h1D.FindBin(energy+10)) - h1D.Integral(h1D.FindBin(energy+10),h1D.FindBin(energy+3*10))
          truth = hI.Integral(hI.FindBin(energy-10),hI.FindBin(energy+10)) - hI.Integral(hI.FindBin(energy+10),hI.FindBin(energy+2*10))
          efficiencies[h.GetYaxis().GetBinLabel(det)][0].append(energy)
          efficiencies[h.GetYaxis().GetBinLabel(det)][1].append(integral/truth)
  return efficiencies

#
# Open input
#
f = TFile(FILENAME,"READ")
print "processing " + FILENAME + " ... "

hE2D = f.FindObjectAny("hEdep")
hE2D2s = f.FindObjectAny("hEdep2s")
hE2D2s1s = f.FindObjectAny("hEdep2s1s")
hEInit = f.FindObjectAny("hEInit")
hEPrimary = f.FindObjectAny("hEPrimary")

hE2D.SetDirectory(0)
hE2D2s.SetDirectory(0)
hE2D2s1s.SetDirectory(0)
hEPrimary.SetDirectory(0)

#
# Process
#

hE = hE2D.ProjectionX("hE",1,5)
hE2s = hE2D2s.ProjectionX("hE2s",1,5)
hE2s1s = hE2D2s1s.ProjectionX("hE2s1s",1,5)

#fit 2s1s
result_2s = FitPeak(hE2s,1640)
result_2s1s = FitPeak(hE2s1s,1640)
#print result_2s
#print result_2s1s

print " ratio All(E(2s1s)) / Real(2s1s) = " + str( result_2s["integral"] / result_2s1s["integral"] )

#Calculate efficiency
efficiencies = Calculate_Eficiency(hE2D,hEPrimary)
#print efficiencies["Ge1"]

#
# Plot
#

#plt.plot(efficiencies["Ge1"][0],efficiencies["Ge1"][1],"bs")
#plt.show()

c1 = TCanvas("c1","Summed Energy Spectrum",1000,400)
c1.cd()
gPad.SetLogy()
hE.SetLineColor(1)
hE2s.SetLineColor(2)
hE2s1s.SetLineColor(4)
hE.GetXaxis().SetRangeUser(300,2300)
hE.Draw()
hE2s.Draw("SAME")
hE2s1s.Draw("SAME")

#c1.Update()

#
# Close
#



raw_input("Press enter to close ...")


f.Close()





