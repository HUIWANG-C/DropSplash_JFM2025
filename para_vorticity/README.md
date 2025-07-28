# Paraview post-processing

This folder contains an example to further process the simulation results.  

1. You have to first generate the .htg files using the code shared in the previous folder.

2. Using paraview to open one of the .htg files (better at a representative time point or phenomenon). Configure the visualisation as you want and output the state as a python script (instead of .pvsm).
Then you need to modify a bit of the saved .py file into a function, in order to loop all the available .htg files using the LoopVTU.py file.
Please note:
- You could write the python script directly if you are confident and familiar with paraview specific grammar
- You could also try to use the _Tools--start_ trace to trace the python code 
