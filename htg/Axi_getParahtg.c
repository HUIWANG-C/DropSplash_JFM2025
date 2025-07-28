/* Title:  Axi_Control case--Extracting droplet statistics from dump file using bview*/
/* Author: Hui WANG*/
/* Email:  huiwang_m@outlook.com*/ 
/* Journal: Journal of Fluid Mechanics*/ 

/******************************Head files*******************************************************************************************/
#include "axi.h"
#include "tag.h"
#include "run.h"

#include "output_htg_old.h"
/******************************Macros*******************************************************************************************/
//Domain
#define DOMAIN (8.)   

//Mesh Refinement
int MINLEVEL = 7;
int MAXLEVEL = 15;
                
/******************************Main*******************************************************************************************/
int main(){
  size (DOMAIN);
  origin (-DOMAIN/2., 0.);
  run();
}

/******************************Output*******************************************************************************************/
event init(t = 0)
{
  //define begin, timestep and end of the input, file names
  double timebgn = 0.;   
  double timestp = 0.72/1000;
  double timeend = 0.1801;     

  char namefile[100];    //name of inputs
  //char name[100];        //name of output images
  double timeload;       //iterator     
  int cellnum;  

  system("mkdir -p htg");  //create a file "htg"

  //output loop
  for(timeload = timebgn; timeload <= timeend; timeload += timestp){
    /*input dump files*/
    scalar f[];  
    scalar Tdrop[], Tpool[];
    scalar p[];
    vector u[];
    sprintf(namefile,"dump%g",timeload);
    printf("load the file %s!\n",namefile);  
    restore(file=namefile, list=all);
    boundary((scalar *){f, u.x, u.y, Tdrop, Tpool, p});

    /*output number of cells*/
    cellnum=0;
    foreach(){
      cellnum += 1;
    }
    printf("number of cells are %d.\n",cellnum);
    /*-------------------------------------------------------------------------------------------*/
    scalar omega[];
    vorticity (u, omega);

    boundary(all);
    restriction(all);

    char path[]="htg/"; // no slash at the end!!
    char prefix[80];
    sprintf(prefix, "paraHTG_%06g", timeload);
    output_htg((scalar *) {f, Tdrop, Tpool, p, omega},(vector *){u}, path, prefix, i, timeload);	

  }
}

event end(t=0.0){
}
