#!/usr/bin/python
#modify a filename

import sys
import os
import fileinput


tempfilename = "temp"

files = sys.argv[1]

cmd="ls " + files + "* > " + tempfilename
print cmd

os.system(cmd)
file1 = open(tempfilename,"r")

for line in file1:
  back = line[22:-1]
  oldfile = line[:-1]
  newname = "NC1"+back
  cmd = "cp " + oldfile + " " + newname
  print cmd
  os.system(cmd)
  cmd = "rm " + oldfile
  os.system(cmd)

cmd = "rm " + tempfilename
os.system(cmd)


