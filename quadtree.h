

#include "q_engine.h"


#if 1
#define vx_size 64
#define addr(x,y,z)	(((x)<<12) + ((y)<<6) + (z))

//64x64x64=262144 = 512x512
#else

//lasabb
//#define vx_size 128
//#define addr(x,y,z)	(((x)<<14) + ((y)<<7) + (z))

//#define vx_size 256
//#define addr(x,y,z)	(((x)<<16) + ((y)<<8) + (z))
#endif			


typedef qvector<int> vint;
extern vint *quadtree;
extern qvector<vec3> cells;

extern vec3 dvoxel;
extern float1 dvoxel_step;
extern int xh_quadtree;


void setup_quadtree(vec3 wbmin,vec3 wbmax);
void dump_quadtree();




