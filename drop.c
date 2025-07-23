/*This is an axisymmetric case, we use this code to do parametric study in 3D-sh of drop impact. 
We change drop shape under varied impact condition, to observe drop shape effects on impact dynamics*/

/******************************Head Files**************************************************************************************/
#include "axi.h"
#include "navier-stokes/centered.h"             //N-S       
#include "maxruntime.h"                         //restart We can control the maximum runtime.
#define FILTERED                                //Smear density and viscosity jumps                            
#include "two-phase.h"                          //2-phase                        
#include "navier-stokes/conserving.h"           //momentum-conserving VOF advection of the velocity components                    
#include "tension.h"                            //surface tension
#include "reduced.h"                            //gravity  
#include "tracer.h"                             //passive tracer
#include "view.h"                               //generate pics&movies online                                                                                                

#include "adapt_wavelet_limited.h" 
/******************************Macros******************************************************************************************/
#define L_Ref 0.01  
#define DB (0.004/L_Ref)    

// Define the drop shape
#define D_h   (4.2e-01)             // modify shape    
#define D_v   (pow(DB, 3)/sq(D_h))
#define Alpha (D_v/D_h)

#define y_D (0.)                           
#define x_D (0.1*DB+D_v/2.)                                              
#define z_D (0.)                                             
#define Drop(x,y) (sq(((x-x_D)*4)/(D_v*10))+sq(((y-y_D)*4)/(D_h*10)))  

#define Re (10000.)                 // modify impact condition   
#define We (1500.)
 
//Domain:LDB=20
#define DOMAIN (8.)                                
//Density: densities are non-dimensionalize using the density of sea water RHO_SW=1018.3                           
#define RHO_SW (1.)
#define RHO_A  (1.3/1018.3)
//Viscosity: Re=RHO_SW*DB*V/MU_SW
#define MU_SW (DB/Re)
#define MU_A  (MU_SW*1.8e-5/1.e-3)
//Surface Tension: We=RHO_SW*DB*V/SIGMA_SW
#define SIGMA_SW (DB/We)                
//Gravity           
#define Gravity (1./(36.*36.*DB))   

//Geometry
#define drop(x,y) (sq(DB/2) - Drop(x,y))
#define pool(x) (-x)
//Initial refinement
#define epi (0.05*DB)
#define rfdrop(x,y) ((sq(DB/2-epi) < Drop(x,y)) && (sq(DB/2+epi) > Drop(x,y)))
#define rfpool(x) ((-epi < x) && (epi > x))

//Running Time
#define MAXTIME 0.1801  //0.25ms

int MINLEVEL = 7;                          
int MAXLEVEL = 15;
double fErr = 1e-4;          
double uErr = 1e-2;   
     
/******************************Boundary conditions*******************************************************************************/
//Right: outflow 
u.n[right]    = neumann (0.);
p[right]      = dirichlet (0.);
pf[right]     = dirichlet (0.);

/******************************Passive tracer*************************************************************************************/
//Passive Tracer for drop and pool
scalar Tdrop[], Tpool[];                                
scalar * tracers = {Tdrop, Tpool};  

/******************************Main Function*************************************************************************************/
int main (int argc, char * argv[]) {

  maxruntime (&argc, argv);
  if (argc > 1)
    MAXLEVEL = atoi (argv[1]);

  size (DOMAIN);
  origin (-DOMAIN/2., 0.);
  init_grid (pow(2.0, MINLEVEL));

  rho1 = RHO_SW;    //density of seawater        
  rho2 = RHO_A;     //density of air                                               
  mu1 = MU_SW;           
  mu2 = MU_A;
  f.sigma = SIGMA_SW;

  Tdrop.gradient = minmod2;
  Tpool.gradient = minmod2;

  G.x = -Gravity;
  
  //TOLERANCE*sq(dx)/sq(dt).We reduce the tolerance on the divergence of the flow
  //This is important to minimise mass conservation errors for these simulations which are very long.
  TOLERANCE = 1e-4 [*];
  
  fprintf(ferr,"\n\n# CASE SETTING SUMMARY:\n\n"); 
  fprintf(ferr,"# Numerical settings \n");
  fprintf(ferr,"# MAXLEVEL uErr Tolerance DB/delta\n");
  fprintf(ferr,"# %d, %g, %g, %g\n\n", MAXLEVEL, uErr, TOLERANCE, DB/(DOMAIN/(1 << MAXLEVEL)));

  fprintf(ferr,"# Drop shape values:\n");
  fprintf(ferr,"# D_h(mm) D_v(mm) Alpha\n");
  fprintf(ferr,"# %g %g %g\n\n", D_h*10, D_v*10, Alpha);

  fprintf(ferr,"# Problem parameters:\n");
  fprintf(ferr,"# Re We \n");
  fprintf(ferr,"# %g %g\n\n", Re, We);
  
  run();
}                   

/******************************Initial Conditions*********************************************************************************/
event init (t = 0) {
  //restore the simulation from a previous “restart”
  if (!restore (file = "restart", list = (scalar *){all})){
    scalar f4[], f6[];

    refine ((rfdrop(x,y) || rfpool(x)) && (level < MAXLEVEL));
  
    fraction (f4, pool(x));
    fraction (f6, drop(x,y));
    foreach() {
      f[] = f4[] + f6[];
      //if (f[] < 1 && f[] > 0) // Only add noise to cells that contain the interface
        //f[] = clamp(f[] + 0.05*noise(), 0, 1);
      u.x[] = -f6[];
    }

    //Passive tracers
    fraction (Tdrop, drop(x,y));    //Tdrop
    fraction (Tpool, pool(x));      //Tpool

    boundary ((scalar *){f,Tdrop,Tpool});
  }

}

/******************************Adaptive Mesh*******************************************************************************************/
int refRegion_max(double x, double y, double z)
{
  int levmax;
  if (sq(x)+sq(y) < sq(3.9))
    levmax = MAXLEVEL;  
  else
    levmax = MINLEVEL;

  return levmax;
}

event adapt (i++) { 
  adapt_wavelet_limited ((scalar *){f, u.x, u.y}, (double[]){fErr, uErr, uErr}, refRegion_max, MINLEVEL);
  //we refine the region close to the free surface at the initial time of impact
  if (t < 0.1801)   
    refine(x>(-0.025*DB) && x<(0.1*DB) && y<(0.75*DB) && level < MAXLEVEL);
}

//////////////////////////////////////////////////////////////////////OUTPUT/////////////////////////////////////////////////////////////OUTPUT///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////OUTPUT/////////////////////////////////////////////////////////////OUTPUT///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************Output pics*********************************************************************************************/
/*we output early time pics every 1us*/
event earlypics(t = 0; t += 0.72/1000; t <= MAXTIME) { 
  char name[80]; 

  scalar omega[];
  vorticity (u, omega);

  scalar l[];
  foreach()
    l[] = level;

  /*f+cells*/
  {
    view (camera = "front", width = 1024, height = 1024, fov = 3); 
    clear();     
    cells();                                                    
    mirror ({0,1}){  
      squares ("f", min = 0, max = 1);
    }
    sprintf (name, "2fcells_fov3_%05g.png", t);
    save (name);
  } 
  /*level+vorticity+omega+vof*/
  {
    view (camera = "front", width = 1024, height = 1024, fov = 3); 
    clear();  
    draw_vof("f", lc = {255., 218., 185.});
    squares ("l", min=MINLEVEL, max=MAXLEVEL); 
    mirror ({0,1}){                                                 
      draw_vof("f", lc = {255., 218., 185.});
      squares ("omega", linear = true, map = cool_warm);
    }
    sprintf (name, "2levor_fov3_%05g.png", t);
    save (name);                       
  } 
}

/******************************Output Movies*********************************************************************************************/
/*we output movies every 0.01ms*/
event earlymovies(t = 0; t += 0.72/1000; t <= MAXTIME) {
  char s[80];
  sprintf (s, "t = %.2f ms", t/0.72);

  scalar omega[];
  vorticity (u, omega);

  scalar l[];
  foreach()
    l[] = level;

  /*level+vorticity+omega+vof*/
  {
    view (camera = "front", width = 1024, height = 1024, fov = 3); 
    clear();  
    draw_vof("f", lc = {255., 218., 185.});
    squares ("l", min=MINLEVEL, max=MAXLEVEL); 
    mirror ({0,1}){                                                 
      draw_vof("f", lc = {255., 218., 185.});
      squares ("omega", linear = true, map = cool_warm);
    }
    draw_string (s, pos = 0, size = 40, lw = 2);    
    save ("levvor3.mp4");                       
  }
  /*f+cells*/
  {
    view (camera = "front", width = 1024, height = 1024, fov = 3); 
    clear();                                                        
    cells(); 
    mirror ({0,1}){  
      squares ("f", min = 0, max = 1);
    }
    draw_string (s, pos = 0, size = 40, lw = 2);    
    save ("vofcells3.mp4");
  } 
}

/******************************Perfs*******************************************************************************************/
event perfs (i++) 
{
  static FILE * fp = fopen ("perfs", "w");
  if (i == 0)
    fprintf (fp,
	     "t dt mgp.i mgp.nrelax mgpf.i mgpf.nrelax mgu.i mgu.nrelax "
	     "grid->tn perf.t perf.speed npe\n");
  fprintf (fp, "%g %g %d %d %d %d %d %d %ld %g %g %d\n", 
	   t, dt, mgp.i, mgp.nrelax, mgpf.i, mgpf.nrelax, mgu.i, mgu.nrelax,
	   grid->tn, perf.t, perf.speed, npe());
  fflush (fp);
}

/******************************Output Snapshot************************************************************************************************/
event dumpfile(t = 0.; t += 0.72/1000; t <= MAXTIME) {
  scalar omega[];
  vorticity (u, omega);

  scalar l[];
  foreach()
    l[] = level;

  p.nodump = false; 
  char name[80];
  sprintf (name, "dump%g", t);
  dump (name);
}

/******************************End Simulation*******************************************************************************************/
event end (t = MAXTIME) {                                         
  printf("i=%d t=%g\n",i,t);
}
