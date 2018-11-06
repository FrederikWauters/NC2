#!/usr/bin/python

#
# Imports
#

from ROOT import TH1I,TH2, TCanvas, TVector3
import itertools
import math

v0=TVector3(2.083,0.467,9440.597)
v2=TVector3(70.610,58.285,9564.941)
v2=v2-v0
v3=TVector3(97.267,-11.483,9557.375)
v3=v3-v0
v4=TVector3(57.587,-84.839,9552.423)
v4=v4-v0
v5=TVector3(-9.787,-97.016,9556.683)
v5=v5-v0
v6=TVector3(-75.010,-80.888,9552.919)
v6=v6-v0
v7=TVector3(-107.477,-9.624,9574.415)
v7=v7-v0
v8=TVector3(-92.412,63.811,9581.075)
v8=v8-v0
v9=TVector3(121.574,-9.747,9460.619)
v9=v9-v0
v10=TVector3(-121.375,-5.159,9488.936)
v10=v10-v0

positions=[v2,v3,v4,v5,v6,v7,v8,v9,v10]

pairs = itertools.combinations(positions,2)

h = TH1I("h","Run 2017: angles between detectors; Angle (degrees)",9,0,180);

for pair in pairs:
  angle = math.degrees(pair[0].Angle(pair[1]))
  h.Fill(angle)
  
h.Draw()

raw_input("press enter to continue ... ")
  
  
