
#ifndef _QENGINEH
#define _QENGINEH

#include "q_vect.h"
#include "q_tools.h"
#include "q_file.h"
#include "q_kdtree.h"
#include "q_opengl.h"
#include "q_shader.h"
#include "q_font.h"
#include "q_str.h"


class objfile;
class s_bbox2;


struct s_texture
{
	uchar *tgabuf;
	int xh,yh,ch,scan,id;
};


void error(const char *ss) ;
void error2(const char *ss,const char *ss2) ;

void qsleep(float1 dt);
double q_getcpuclock();
double xtimer(const char *ss);
double xtimer(const char *ss,float scl);
int isletter(char u);
int64_t filesize(FILE *ff);
int qload(const char *ss,void *buf,int n);
int qload(const char *ss,void **buf);
void printProgramInfoLog(GLuint obj);
GLuint glxtexture(void *mem,int xc,int yc,int col,int wrap=1,int needmip=1);//rtg
GLuint glxtextureb(void *mem,int xc,int yc,int col,int wrap=1);
qstring int2string(int a);
void qsetrtg(qstring name,IDTYPE &id);
void create_rtg(qstring nm,int floatrtg,int nattach,int x2,int y2,int needDeep=1);//tx +1 +2 attachment!!!!!!!!11
void create_rtg(qstring nm,int floatrtg,int nattach,int needDeep=1);//tx +1 +2 attachment!!!!!!!!11
void qevent(void (*fnc)());
void glxline(int x1,int y1,int x2,int y2,int color);
void glxlineb(float1 x1,float1 y1,float1 x2,float1 y2,int color);

void initsintab();
float qsin(float n);//quick sin()
float qcos(float n);

void pixel(int x,int y,int col);
void line(int x1,int y1,int x2,int y2,int color);
void lineb(float1 x1,float1 y1,float1 x2,float1 y2,int color);

void qclear(int);
void qflush();

void init_system5();
void wait();
void qclear(int op);
void qflush();


int lerpcol(int col1,int col2,float1 t);
void create_texture(qstring name,int _x,int _y,int _c,uchar *buf);
void loadbmp(qstring name,qstring fname,int opac);
void loadtga(qstring name,qstring fname,int colxchg=0);
void loadbmpb(qstring name,qstring fname,int ecut=0,int colxchg=1);
void loadtgab(qstring name,qstring fname,int colxchg=0);
void savetga(std::string fname,uchar *mem,int xh,int yh,int ch);
void qsettx(int tx,qstring nm,IDTYPE &id,IDTYPE &id2);
void qsettx3d(int tx,qstring nm,IDTYPE &id,IDTYPE &id2);
void qsettxNOS(int tx,qstring nm,IDTYPE &id,IDTYPE &id2);
void qsettxbyid(int tx,int id);
s_texture *qgettexture(const char *name,IDTYPE &id);

void defmesh(qstring meshname,qstring type);
void endmesh();
void qdrawmesh(qstring name,IDTYPE &id);
void delmesh(qstring name);

void quadtx(
	vec3 v1,vec3 v2,vec3 v3,vec3 v4,
	vec3 n1,vec3 n2,vec3 n3,vec3 n4,
	vec3 col,
	vec3 tx1,vec3 tx2,vec3 tx3,vec3 tx4);
void quadn(
	vec3 v1,vec3 v2,vec3 v3,vec3 v4,
	vec3 n1,vec3 n2,vec3 n3,vec3 n4,
	vec3 col);
void print_trace(void);
void quad3d(vec3 v1,vec3 v2,vec3 v3,vec3 v4,vec3 col);
void rainbow4(int &r,int &g,int &b,int t,int h,
	int r1,int g1,int b1,
	int r2,int g2,int b2,
	int r3,int g3,int b3,
	int r4,int g4,int b4,
	int r5,int g5,int b5);


#define setshader(A) qsetshader(A,idtabl[__COUNTER__])
#define settx(A,B) qsettx(A,B,idtabl[__COUNTER__],idtabl[__COUNTER__])
#define settx3d(A,B) qsettx3d(A,B,idtabl[__COUNTER__],idtabl[__COUNTER__])
#define settxNOS(A,B) qsettxNOS(A,B,idtabl[__COUNTER__],idtabl[__COUNTER__])
#define shader_setparm2(A,B) qshader_setparm2(A,B,idtabl[__COUNTER__])
#define shader_setparm3(A,B,C) qshader_setparm3(A,B,C,idtabl[__COUNTER__])
#define setrtg(A) qsetrtg(A,idtabl[__COUNTER__])
#define drawmesh(A) qdrawmesh(A,idtabl[__COUNTER__])
#define gettexture(A)  qgettexture(A,idtabl[__COUNTER__])



extern int hmap;
extern int xmap;
extern vec3 *mapv;

extern vec3 look,up,eye,light;
extern qmat cam_mat;
extern qmat cam_mat_shad;
extern qmat cam_mat_shad2;
extern axis cam;

extern float1 linewidth;
extern float1 win_scale;
extern vec3 win_pos;
extern float1 gametime;

extern int mbutpress[8],mbut[8],mbute[8],mx,my,dmx,dmy,key,qwheel,mx32,my32;
extern short tkey[100000]; 

extern int joyx0,joyy0, 	joyxb0,joyyb0;
extern int joyx1,joyy1, 	joyxb1,joyyb1,
	isjoy,ejoy,
	jbutt0,jbutt1,jbutt0_old,jbutt1_old,
	jbutpress0,jbutpress1;

extern int joyx0_min,joyx0_max,
	joyy0_min,joyy0_max,
	joyx1_min,joyx1_max,
	joyy1_min,joyy1_max;



extern std::unordered_map<int64_t,s_texture> textures;
extern fix_array<IDTYPE,5000> idtabl;


#include "q_objloader.h"
#include "q_loader.h"
#include "q_fly.h"



#endif


