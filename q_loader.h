
#ifndef _QLOADERH
#define _QLOADERH


//original jobb , de a masik profibb megoldas
//ez gyorsabb mert keves a haromszog!
#define _ORIGINAL_KDTREE_UPDATE
#define _KDTREE_VS_KDTREE_CHECK

//1: +(all verts to kd) vs -(tra bbox)       ,2:  +(kd/kd) vs -(edge/kd)
//10x phys
// -- 390 220 
// +- 250 190
// -+ 230 160  ez a pro csak keves a haromszog
// ++ 180 130


#include "q_engine.h"


extern int physics;
extern double timedump;
extern double timedump2;

extern float1 ratio;
extern char sellevel[110];


//float           int egyforma gyors    q_loader5.cpp
class s_bbox
{
public:
	vec3 bbmin;
	vec3 bbmax;
	vec3 bbmin_tra;
	vec3 bbmax_tra;
	vec3 bbmin2;//origin for check
	vec3 bbmax2;
	int indx;
	int next1;
	int next2;
	int root;// NA  nem kell de jo ha van

	int valid;
	int transf;
	axis4 *tramat;
	int type;
			
	s_bbox()
	{
		clear();
		return;
	}
	void clear()
	{
		bbmin=vec3(1e16,1e16,1e16);
		bbmax=vec3(-1e16,-1e16,-1e16);
		indx=-1;//uninitialized
		next1=-1;
		next2=-1;
		root=-1;

		valid=0;
		tramat=NULL;
		transf=1;
		type=1;
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
	void set_vec3(const vec3 &v1)
	{
		bbmin=v1;
		bbmax=v1;
	}
	void add_bbox(s_bbox *bbx)
	{
		add_vec3(bbx->bbmin);
		add_vec3(bbx->bbmax);
	}
	void set_bbox(s_bbox *bbx)
	{
		bbmin=bbx->bbmin;
		bbmax=bbx->bbmax;
	}
	int is_empty()
	{
		if(bbmin.x>bbmax.x) return 1;
		return 0;
	}
	void validate()
	{
		if(valid==0)
		{
			if(transf)
			{
				vec3 edge1x=tramat->vec3_x*bbmin2.x;// ezzel carrieren fekve 30 =0.3ms   , masik update 40 chk 20 = 70
				vec3 edge1y=tramat->vec3_y*bbmin2.y;
				vec3 edge1z=tramat->vec3_z*bbmin2.z;
				vec3 edge2x=tramat->vec3_x*bbmax2.x;// nem delta!
				vec3 edge2y=tramat->vec3_y*bbmax2.y;
				vec3 edge2z=tramat->vec3_z*bbmax2.z;
			

				if(edge1x.x<edge2x.x) {bbmin_tra.x =edge1x.x;bbmax_tra.x =edge2x.x;} else {bbmax_tra.x =edge1x.x;bbmin_tra.x =edge2x.x;} // 30-40
				if(edge1x.y<edge2x.y) {bbmin_tra.y =edge1x.y;bbmax_tra.y =edge2x.y;} else {bbmax_tra.y =edge1x.y;bbmin_tra.y =edge2x.y;}
				if(edge1x.z<edge2x.z) {bbmin_tra.z =edge1x.z;bbmax_tra.z =edge2x.z;} else {bbmax_tra.z =edge1x.z;bbmin_tra.z =edge2x.z;}
				if(edge1y.x<edge2y.x) {bbmin_tra.x+=edge1y.x;bbmax_tra.x+=edge2y.x;} else {bbmax_tra.x+=edge1y.x;bbmin_tra.x+=edge2y.x;}
				if(edge1y.y<edge2y.y) {bbmin_tra.y+=edge1y.y;bbmax_tra.y+=edge2y.y;} else {bbmax_tra.y+=edge1y.y;bbmin_tra.y+=edge2y.y;}
				if(edge1y.z<edge2y.z) {bbmin_tra.z+=edge1y.z;bbmax_tra.z+=edge2y.z;} else {bbmax_tra.z+=edge1y.z;bbmin_tra.z+=edge2y.z;}
				if(edge1z.x<edge2z.x) {bbmin_tra.x+=edge1z.x;bbmax_tra.x+=edge2z.x;} else {bbmax_tra.x+=edge1z.x;bbmin_tra.x+=edge2z.x;}
				if(edge1z.y<edge2z.y) {bbmin_tra.y+=edge1z.y;bbmax_tra.y+=edge2z.y;} else {bbmax_tra.y+=edge1z.y;bbmin_tra.y+=edge2z.y;}
				if(edge1z.z<edge2z.z) {bbmin_tra.z+=edge1z.z;bbmax_tra.z+=edge2z.z;} else {bbmax_tra.z+=edge1z.z;bbmin_tra.z+=edge2z.z;}
				bbmin_tra+=tramat->vec3_w;
				bbmax_tra+=tramat->vec3_w;
			}
			else
			{
				bbmin_tra=bbmin;
				bbmax_tra=bbmax;
			}			
			
			bbmin_tra-=vec3(0.1,0.1,0.1);// safety
			bbmax_tra+=vec3(0.1,0.1,0.1);
			valid=1;
		}
	}
	int chk_bbox(s_bbox *bbx)
	{
#ifdef _ORIGINAL_KDTREE_UPDATE
		if(bbmax.x<bbx->bbmin.x) return 0;
		if(bbx->bbmax.x<bbmin.x) return 0;
		if(bbmax.y<bbx->bbmin.y) return 0;
		if(bbx->bbmax.y<bbmin.y) return 0;
		if(bbmax.z<bbx->bbmin.z) return 0;
		if(bbx->bbmax.z<bbmin.z) return 0;
		return 1;
#else
		validate();
		bbx->validate();
		
		if(bbmax_tra.x<bbx->bbmin_tra.x) return 0;
		if(bbx->bbmax_tra.x<bbmin_tra.x) return 0;
		if(bbmax_tra.y<bbx->bbmin_tra.y) return 0;
		if(bbx->bbmax_tra.y<bbmin_tra.y) return 0;
		if(bbmax_tra.z<bbx->bbmin_tra.z) return 0;
		if(bbx->bbmax_tra.z<bbmin_tra.z) return 0;
		return 1;
#endif
	}
};






void transpose(axis4 *mat1,axis4 *mat2);

struct int2
{
	int a,b;
};


struct s_coll
{
	int iface;
	vec3 obj;
	vec3 nor;
	vec3 deep;
	
	s_coll() {iface=0;}
};


class c_mesh
{
public:	
	qvector<vec3> pnts;
	qvector<vec3> pnts_trn;
	qvector<vec3> spds;
	qvector<vec3> accs;
	qvector<vec3> txcoord;
	qvector<vec3> txcoord2;

	qvector<float1> areas;
	qvector<vec3> normals;
	qvector<vec3> vnorm;//vertex normals
	qvector<vec4> color;
	qvector<vec3> tricenters;
	qvector<float1> forces[2];
	
	typedef qvector<int> intvec;
	typedef qvector<float1> ffvec;
	
	qvector<int> faces;
	qvector<int> faces_LOD;
	qvector<int2> edges;
	qvector<int> types;
	qvector<int> vtypes;
	qvector<int> inverz_normal;
	qvector<intvec>vf_links;
	qvector<intvec>vv_links;
	qvector<ffvec>vv_len;
	qvector<intvec>f_links;
	qvector<int> blackverts;
	qvector<int> blackverts2;
	vec3 txc_center;

	axis4 tramat;//main trans mat     ,a tobbi egyenkent 
	axis4 itramat;
//	s_kdtree kdtree;
	
	float1 metr;//=57.0;
	vec3 wcenter;//=vec3(921, 453, 1206);
	vec3 wcenter2;

//	qvector<s_bbox> bboxseg[2];		s_bbox bboxfull; // old NA
	
	int is_carrier=0;
	int invalid_kdtree=1;

	int pnts_mir_base=0;
	int faces_mir_base=0;

	void savepnts(char *filename);
	void loadpnts(const char *filename,const char *txname,int op,float1 scl=1.0);
	void loadpnts_old(int op);

	void draw(vec3 pos,axis &ax,int mir=0);
	void trans(vec3 pos,axis &ax);
	void transf4(vec3 pos,axis4 &ax,int mask);
	void transf4all(vec3 pos,axis4 &ax);
	void transf42(vec3 pos,axis4 &ax,int mask,int mask2);
	int getcollision4(vec3 pos,vec3 pos2,vec3 &deep,vec3 &normal,vec3 &obj,int &indx);// carrier kdtree
	int getcollision5(vec3 pos,vec3 pos2,int facei,vec3 &deep,vec3 &normal,vec3 &obj,int &indx);// carrier kdtree
	void add_edge(int f1,int f2);
	void distorForce(int f0,vec3 F,int sp,vec3 v1,float1 dt);
	void addForce(vec3 v1,int f0,vec3 F,int sp);
//	void restoreEdges(int f0,int sp,float1 D);
	void restoreEdges(float1 dt);  // v3.0
	void updateSpd(float1 dt,float1 D,float1 K);

	
	int chk_plane_ray(vec3 pos,vec3 pos2,float1 &deep,int ex);
	void chk_surface_occlusion();
	void chk_normal_occlusion();
	void chk_normal_occlusion_trn();
	int chk_mesh2meshcollis4(c_mesh* mesh,vec3 refd,qvector<s_coll> &_colls,int op);//kdtree


	struct s_kdtree
	{
		qvector<s_bbox> bboxs;
		int rootindx=0;

		vec3 pos_kd;
		int cc5=0;
		
		void build_kdtree(qvector<vec3> &verts,qvector<int> &faces,qvector<int> &types);
		int build_kdtree_branch(qvector<int> &indx);

		void update_kdtree(qvector<vec3> &verts,qvector<int> &faces,vec3 pos);
		void update_kdtree_branch(int indx);
		
		void chk_kdtree(int indx,s_bbox &ray_bbox,qvector<int> &selected_faces);
		void chk_kdtree2x(qvector<s_bbox> &bboxs2,int indx1,int indx2,qvector<int> &selected_faces);

		void invalidate()
		{
			loopd(i,0,bboxs.size(),1) bboxs[i].valid=0;
		}
		void set_tramat(axis4 *tramat)
		{
			loopd(i,0,bboxs.size(),1) bboxs[i].tramat=tramat;
		}
		void store()
		{
			loopd(i,0,bboxs.size(),1) 
			{
				bboxs[i].bbmin2=bboxs[i].bbmin;//origin
				bboxs[i].bbmax2=bboxs[i].bbmax;
			}
		}
	};
	
	s_kdtree kdt;
	
	void update_kdtree();
	void invalidate_kdtree()
	{
		invalid_kdtree=1;
//update_kdtree();//test
		
#ifdef _ORIGINAL_KDTREE_UPDATE
		update_kdtree();
#else		
		kdt.invalidate();
#endif		
	}
};




void chk_bbox5(vec3 &v1,vec3 &mmin,vec3 &mmax);


//--------------------------------------------------------------------------------

void preloadlevel(int op,int extx=-1);
void loadlevel(int op,int extx=-1,int needgmap=1);


#endif


