# Splashing regimes of high-speed drop impact (DropSplash_JFM2025) 
This repository contains codes related to paper [**Splashing regimes of high-speed drop impact**]() that is under review in the _Journal of Fluid Mechanics_ in 2025.   
## 📚 Citation
If you use this code or data, please cite the following:  

```bibtex
coming soon
```

## Basilisk
To run the simulation, please check and install the open-source finite-volume adaptive [Basilisk](http://basilisk.fr/) solver.  
Please note that the code may not compile or run correctly depending on the software version.

### Running on local computer  

🛠️ For compiling the code in parallel:  
`CC99='mpicc -std=c99' qcc -Wall -O2 -D_MPI=1 drop.c -o drop -L$BASILISK/gl -lglutils -lfb_tiny -lm`  

▶️ Run the code in parallel:  
`mpirun -np 4 ./drop`

### Running on HPC

🛠️ For compiling the code in parallel:  
1. First, you have to generate a portable (ISO C99) source code on the local computer using:  
`qcc -source -grid=octree -D_MPI=1 drop.c`  
2. Second, copy the portable source code *_drop.c* on the supercomputer and compile it  
`CC99='mpicc -std=c99' qcc -Wall -O2 -D_MPI=1 drop.c -o drop -L$BASILISK/gl -lglutils -lfb_tiny -lm`  

▶️ Run the code in parallel:  
`mpirun -np 4 ./drop`
