#!/usr/bin/python

#
# Imports
#
from ROOT import TH1I,TH2,TH3,TFile, TF1, TCanvas, gROOT, gPad, TLegend
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
hE3p2s = f.FindObjectAny("hE3p2s")
hE4p2s = f.FindObjectAny("hE4p2s")
hE5p2s = f.FindObjectAny("hE5p2s")
hE3p2sHas2s = f.FindObjectAny("hE3p2sHas2s")
hE4p2sHas2s = f.FindObjectAny("hE4p2sHas2s")
hE5p2sHas2s = f.FindObjectAny("hE5p2sHas2s")


  
 

hE2D.SetDirectory(0)
hE2D2s.SetDirectory(0)
hE2D2s1s.SetDirectory(0)
hEPrimary.SetDirectory(0)
hEInit.SetDirectory(0)
hEPrimary.SetDirectory(0)
hE3p2s.SetDirectory(0)
hE4p2s.SetDirectory(0)
hE5p2s.SetDirectory(0)
hE3p2sHas2s.SetDirectory(0)
hE4p2sHas2s.SetDirectory(0)
hE5p2sHas2s.SetDirectory(0)

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

coinc_histos=[]
coinc_truth_histos=[]

hE3p2s_36 = hE3p2s.ProjectionX("hE3p2s_36",1,1);
hE3p2s_72 = hE3p2s.ProjectionX("hE3p2s_72",2,2);
hE3p2s_108 = hE3p2s.ProjectionX("hE3p2s_108",3,3);
hE3p2s_144 = hE3p2s.ProjectionX("hE3p2s_144",4,4);
hE3p2s_180 = hE3p2s.ProjectionX("hE3p2s_180",5,5);
hE3p2sHas2s_36 = hE3p2s.ProjectionX("hE3p2sHas2s_36",1,1);
hE3p2sHas2s_72 = hE3p2s.ProjectionX("hE3p2sHas2s_72",2,2);
hE3p2sHas2s_108 = hE3p2s.ProjectionX("hE3p2sHas2s_108",3,3);
hE3p2sHas2s_144 = hE3p2s.ProjectionX("hE3p2sHas2s_144",4,4);
hE3p2sHas2s_180 = hE3p2s.ProjectionX("hE3p2sHas2s_180",5,5);



coinc_histos=[hE3p2s_36,hE3p2s_72,hE3p2s_108,hE3p2s_144,hE3p2s_180]
coinc_truth_histos=[hE3p2sHas2s_36,hE3p2sHas2s_72,hE3p2sHas2s_108,hE3p2sHas2s_144,hE3p2sHas2s_180]

coinc_yields = [[],[]]
i=0
for h in coinc_histos:
  fitresult = FitPeak(h,1640)
  coinc_yields[0].append(36+i*36)
  coinc_yields[1].append(h.Integral(1635,1645))
  i=i+1


coinc_truth_yields = [[],[]]
i=0
for h in coinc_truth_histos:
  fitresult = FitPeak(h,1640)
  coinc_truth_yields[0].append(36+i*36)
  coinc_truth_yields[1].append(h.Integral(1635,1645))
  i=i+1

coinc_yields[1][4]=coinc_yields[1][4]*2
coinc_truth_yields[1][4]=coinc_truth_yields[1][4]*2

print coinc_yields
print coinc_truth_yields

#
# Plot
#

plt.plot(coinc_yields[0],coinc_yields[1],"bs",label="2s1s yield with 3p1s coincidence")
plt.plot(coinc_truth_yields[0],coinc_truth_yields[1],"gs",label="2s1s yield with 3p1s coincidence, truth")
plt.xlabel("angle between detectors (degrees)")
plt.ylabel("2s1s yield (arb. units)")
plt.legend(loc='upper right')
plt.show(block=False)

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

c2 = TCanvas("c2","3p2s",1500,1200)
c2.Divide(1,3)


hE3p2s_45.SetLineColor(1)
hE3p2s_90.SetLineColor(2)
hE3p2s_135.SetLineColor(3)
hE3p2sHas2s_45.SetLineColor(4)
hE3p2sHas2s_90.SetLineColor(5)
hE3p2sHas2s_135.SetLineColor(6)

c2.cd(1)
gPad.SetLogy()
l1 = TLegend(0.8,0.99,0.8,0.99);
l1.AddEntry(hE3p2s_45,"3p2s 45 degrees","l");
hE3p2s_45.GetXaxis().SetRangeUser(1300,2200)
hE3p2s_45.Draw()
hE3p2s_90.Draw("SAME")
hE3p2s_135.Draw("SAME")
l1.Draw()
c2.cd(2)
gPad.SetLogy()
hE3p2sHas2s_45.GetXaxis().SetRangeUser(1300,2200)
hE3p2sHas2s_45.Draw()
hE3p2sHas2s_90.Draw("SAME")
hE3p2sHas2s_135.Draw("SAME")
c2.cd(3)
gPad.SetLogy()
hE3p2s_45.GetXaxis().SetRangeUser(1300,2200)
hE3p2sHas2s_45.Draw()
hE3p2s_45.Draw("SAME")
hE3p2s_135.Draw("SAME")
hE3p2sHas2s_135.Draw("SAME")

c1.Update()
c2.Update()

#
# Close
#



raw_input("Press enter to close ...")


f.Close()





