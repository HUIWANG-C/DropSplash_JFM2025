# Splashing regimes of high-speed drop impact (DropSplash_JFM2025) 
This repository contains codes related to paper [**Splashing regimes of high-speed drop impact**]() that is under review in the _Journal of Fluid Mechanics_ in 2025.   
## 📚 Citation
If you use this code or data, please cite the following:  

```bibtex
coming soon
```

## Physical characteristics
For drop impact phenomenon, two dimensionless numbers are crucial for understanding the dominant physical mechanisms governing fluid flows and classifying various splashing behaviours, namely the Reynolds number (*Re*) and Weber number (*We*).

<!-- ![Reynolds](https://latex.codecogs.com/svg.image?&space;Re=\frac{\rho_ldU_0}{\mu_l},We=\frac{\rho_ldU_0^2}{\sigma}) -->

<p align="center">
  <img src="https://latex.codecogs.com/svg.image?&space;Re=\frac{\rho_ldU_0}{\mu_l},We=\frac{\rho_ldU_0^2}{\sigma}" alt="Re and We">
</p>


## Basilisk
To run the simulation, please check and install the open-source finite-volume adaptive [Basilisk](http://basilisk.fr/) solver.  
Please note that the code may not compile or run correctly depending on the software version.

### drop.c
This is an axisymmetric case (2D) to do a parametric study of drop impact.  
Variations of impact conditions can be made by modifying the following parameters in the code:
- Reynolds number
- Weber number
- Drop shape

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
Please use the batch file `drun.sh` to submit the job in the HPC squeue
