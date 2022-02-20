
#ifndef _QKDTREEH
#define _QKDTREEH


#include "q_engine.h"

#if 1
#define f2int(N) (*reinterpret_cast<int *>(&N))
#else
#define f2int(N) N
#endif


#define SORXLIMIT 16


#ifdef _VOXELBASED
#include "q_quadtree.h"
#endif



//#include <vector>#include <map>using namespace std;

extern int rootindx;

struct sbbox
{
	vec3 bmin,bmax;
	int left,right,tri,root;
	
	sbbox() {bmin=vec3(1e16,1e16,1e16);bmax=vec3(-1e16,-1e16,-1e16);root=-1;tri=0;}
};
extern qvector<sbbox> bbox;

inline short float2half(float f)
{
	if (f == 0.0) return 0;
	int *i = (int *)&f;
	short sh = ((((*i) >> 16) & 0x8000) | (((((*i) & 0x7f800000) - 0x38000000) >> 13) & 0x7c00) | (((*i) >> 13) & 0x03ff));
	return sh;
}
struct half4
{
	short x, y, z, w;
};


extern qvector<vec3> trilist,norlist,txclist,collist;
extern vec3 wbmin,wbmax,dwbox;
extern vec3 *kdtree_data1;
extern half4 *kdtree_data1half;
extern vec3 *quadtree_data1;

extern vec3 *kdtree_data2;
extern vec3 *kdtree_data3;
extern int xy,hh;


extern vec3 tx_kdtreesize;
extern vec3 tx_kdtreemax;


void chk_bbox_mm(vec3 &v1,vec3 &bbmin,vec3 &bbmax);
int chk_bbox_mm2(vec3 &v1,vec3 &bbmin,vec3 &bbmax);//bbox extension
void deftri(vec3 &v1,vec3 &v2,vec3 &v3,vec3 &n1,vec3 &n2,vec3 &n3,vec3 col,int tx);
void deftri_txc(vec3 &v1,vec3 &v2,vec3 &v3,vec3 &n1,vec3 &n2,vec3 &n3,vec3 &t1,vec3 &t2,vec3 &t3,vec3 col,int tx);
void fill_tx_quadtree();// csak shaderhez, ezert itt
int walktree(int leaf,int sp);
int build_kdtree(qvector<int> &indx);
void buildtree();
void precalc();
void add_phyfile(const char *name,int s,vec3 base);//oke


#endif


