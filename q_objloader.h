
#ifndef _QOBJLOADERH
#define _QOBJLOADERH

//---------------------------------------------------------------------------

#include "q_engine.h"

// blender export obj triangulate	  ON !! TODO


extern float gscale;// 1

class q_mesh2;

class s_bbox2
{
public:
	vec3 bbmin;
	vec3 bbmax;
	
	s_bbox2()
	{
		reset();
	}
	void reset()
	{
		bbmin=vec3(1e16,1e16,1e16);
		bbmax=vec3(-1e16,-1e16,-1e16);
	}
	void add_vec3(const vec3 &v1)
	{
		if(v1.x<bbmin.x) bbmin.x=v1.x;
		if(v1.x>bbmax.x) bbmax.x=v1.x;
		if(v1.y<bbmin.y) bbmin.y=v1.y;
		if(v1.y>bbmax.y) bbmax.y=v1.y;
		if(v1.z<bbmin.z) bbmin.z=v1.z;
		if(v1.z>bbmax.z) bbmax.z=v1.z;
	}
};



class objfile
{
public:
	void load_obj(const char *name,float1 scale2,int type,int file_len);
	void OBJparser();
	void transf(vec3 axis);
	void draw(vec3 pos,int gear);
	void setmat(const char *name,axis4 ax);
	void getpivot(const char *name,vec3 *ax,vec3 *origin);
	void setrotate(const char *name,float ang1,axis4 *root,vec3 *tra=0);
	q_mesh2 *getmesh(const char *name);

	s_bbox2 bbox;
	int n_mater=0,nv=0,ni=0;
	int cc1=0,cc2=0; // pivot face cc
	qvector<q_mesh2> meshs;
};


class q_mesh2
{
public:
	void draw(vec3 pos,int gear);
	void setmat(axis4 ax);
	void getpivot(vec3 *ax,vec3 *origin);
	void setpivot();
	vec3 getpivotTRA(axis4 *ax);
	void transf(vec3 axis); 
	void makelink();
	
	objfile *obj=NULL;
	std::string name;

	s_bbox2 bbox;
	axis4 mat;			
	vec3 pivot_ax;
	vec3 pivot_pos;
	vec3 pivot_pos_tra;
	int visible=3;
	int haspivot=0;
	int phytype=1; // 0= simple air resistance      1 = wing profile
	int hasmatrix=0;// dump unused
	
	qvector<vec3> vert;
	qvector<vec3> vert_tra;
	qvector<vec3> face_center;
	qvector<vec3> face_normal;
	qvector<vec3> face_normal_tra;
	qvector<vec4> face_color;
	qvector<float> tri_area3;
//	qvector<float> face_order;
	
	qvector<int> faces1;// verts
	qvector<int> faces2;// txc
	qvector<int> faces3;// norms

	qvector<vec3> norm;
	qvector<vec3> norm_tra;
	qvector<vec3> txcoord;

	int n_faces=0;
	int n_vert=0;
	int n_txcoord=0;
	int n_norm=0;
	
	typedef qvector<int> intvec;
	qvector<intvec> linked;
	qvector<vec3> wind;
	vec3 vertymin;
};


#endif


