# We loop all the vtu file using the paepared pvpython script
from paraview.simple import *
import numpy as np
import face_vorticity as im
import os

#get the filename list from a certain directory
filelist = []
for t in np.arange(0., 0.0901, 0.00072):
  file = "paraHTG_%06g.htg" % (t)
  filelist.append(file)
print(filelist)

path = "..../htg/Dh38/Re2500/We800/"
print(path)

#loop every htg file from the list
for item in filelist:
  print("Processing"+item)
  im.Bloop(path,item)
  ResetSession()
