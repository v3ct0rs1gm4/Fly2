
#ifndef _QAIRPLANEH
#define _QAIRPLANEH



#include "q_engine.h"

extern float1 fontscale5;
extern float1 fontsize5;




//kell
#define	add_forceB_origin()\
	{\
	vec3 vel_v=vec3(0,0,0) - spd - cross(R,omega)-wind;\
	vel=length(vel_v);\
	float1 q=dot(normalize(vel_v),normal);\
	q/=qlim;\
	if(q> 1.0) q= 1.0;\
	if(q<-1.0) q=-1.0;\
	F=normal*A*vel*fabs(vel)*ro*0.5*q*1.29;\
	F_all+=F;\
	M_all+=cross(F,R);\
	}
	


#if 0
	F=normal*A*vel*vel*ro*0.5*fsgn(q)*getC(q)*1.5;\
normal iranyu legyen!
*1.5
	F=normal*A*vel*vel*ro*0.5*fsgn(q)*getC(q)*1.5;
	0.62
#endif

#define	add_forceB()\
	{\
	vec3 vdrag=vec3(0,0,0) - spd - cross(R,omega)-wind;\
	vel=length(vdrag);\
	float1 q=dot(normalize(vdrag),normal);\
	F=normal*A*vel*vel*ro*0.5*fsgn(q)*getC(q);\
	F_all+=F;\
	M_all+=cross(F,R);\
	}


#define	add_forceB4()\
	{\
	vec3 vdrag=vec3(0,0,0) - spd - cross(R,omega)-wind;\
	vel=length(vdrag);\
	float1 q=dot(normalize(vdrag),normal);\
	if(q>0.0) q=0.0;\
	F=normal*A*vel*vel*ro*fsgn(q)*getC(q)/3.0;\
	F_all+=F;\
	M_all+=cross(F,R);\
	}
	

//  /5
//	F=normal*A*vel*vel*ro*0.5*fsgn(q)*getC(q);


//0.9=0.5 ??? TODO
//one side
#define	add_forceBN3()\
	{\
	vec3 vdrag=vec3(0,0,0) - spd - cross(R,omega)-wind;\
	vel=length(vdrag);\
	float1 q=dot(normalize(vdrag),normal);\
	vec3 normal2=normal;\
	float1 z=1.0;\
	if(q>0.0) {normal2=vec3(0,0,0)-normal2;z=0.7;}\
	F=normal2*A*vel*vel*ro*0.5*fsgn(q)*getC(q)*z;\
	F_all+=F;\
	M_all+=cross(F,R);\
	}
	

#if 0
	vec3 normal2=normal;\
	if(q>0.0) normal2=vec3(0,0,0)-normal2;\

	if(q<0.0)forog
#endif

//ellenszel
#if 0
kell
	F=vdrag*A*vel*vel*ro*Cdrag*0.5*q;
	F+=vlift*A*vel*vel*ro*Clift*0.5;

	vec3 normal2=normal;\
	if(q>0.0) normal2=vec3(0,0,0)-normal2;\
	vec3 vlift=normal2-vdrag*dot(normal2,vdrag);\

	float1 qa=fabs(q);\
	if( qa<=qlim) Clift=0.5+(1.75-0.5)*qa/qlim;\
	else	if(qa>qlim && qa<=qlim2) Clift=1.75;\
	else	if(qa>qlim2) Clift=1.75+(0.0-1.75)*saturate((qa-qlim2)/(1.0-qlim2));\

#define	add_forceB()\szar
	{\
	vec3 vdrag=vec3(0,0,0) - spd - cross(R,omega)-wind;\
	vel=length(vdrag);\
	vdrag=normalize(vdrag);\
	float1 q=dot(vdrag,normal);\
	vec3 normal2=normal;\
	if(q>0.0) normal2=vec3(0,0,0)-normal2;\
	vec3 vlift=normalize(normal2-vdrag*dot(normal2,vdrag));\
	F =vdrag*A*vel*vel*ro*0.5*fabs(q)*0.5;\
	F+=vlift*A*vel*vel*ro*0.5*getC(q)*1.5;\
	F=normal2*dot(normal2,F);\
	F_all+=F;\
	M_all+=cross(F,R);\
	}
	
#define	add_forceBN3()\
	{\
	vec3 vdrag=vec3(0,0,0) - spd - cross(R,omega)-wind;\
	vel=length(vdrag);\
	vdrag=normalize(vdrag);\
	float1 q=dot(vdrag,normal);\
	vec3 normal2=normal;\
	if(q>0.0) normal2=vec3(0,0,0)-normal2;\
	if(q<0.0)\
	{\
	vec3 vlift=normal2-vdrag*dot(normal2,vdrag);\
	q=fabs(q);\
	F=vdrag*A*vel*vel*ro*Cdrag*0.5*q;\
	Clift=0.5;\
	if(q<=qlim) Clift=0.5+(1.75-0.5)*q/qlim;\
	else	if(q>qlim && q<=qlim2) Clift=1.75;\
	else	if(q>qlim2) Clift=1.75+(0.0-1.75)*saturate(q-qlim2);\
	F+=vlift*A*vel*vel*ro*Clift*0.5;\
	F_all+=F;\
	M_all+=cross(F,R);\
	}\
	}
	

	if(q<0.0)\


#endif

#if 0
//two side  szar
#define	add_forceB222()\
	{\
	vec3 vdrag=vec3(0,0,0) - spd - cross(R,omega)-wind;\
	vel=length(vdrag);\
	vdrag=normalize(vdrag);\
	float1 q=dot(vdrag,normal);\
	F=normal*A*vel*vel*ro*Cdrag*0.5*q;\
	float1 qa=fabs(q);\
	Clift=0.5;\
	if( qa<=qlim) Clift=0.5+(1.75-0.5)*qa/qlim;\
	else	if(qa>qlim && qa<=qlim2) Clift=1.75;\
	else	if(qa>qlim2) Clift=1.75+(0.0-1.75)*saturate((qa-qlim2)/(1.0-qlim2));\
	F+=normal*A*vel*vel*ro*Clift*0.5*(1.0-fabs(q))*fsgn(q);\
	F_all+=F;\
	M_all+=cross(F,R);\
	}
#endif


	
#if 0	
#define	add_forceBN3()\
	{\
	vec3 vdrag=vec3(0,0,0) - spd - cross(R,omega)-wind;\
	vel=length(vdrag);\
	vdrag=normalize(vdrag);\
	float1 q=dot(vdrag,normal);\
	vec3 normal2=normal;\
	if(q>0.0) normal2=vec3(0,0,0)-normal2;\
	{\
	F=normal2*A*vel*vel*ro*Cdrag*0.5*q;\
	float1 qa=fabs(q);\
	Clift=0.5;\
	if( qa<=qlim) Clift=0.5+(1.75-0.5)*qa/qlim;\
	else	if(qa>qlim && qa<=qlim2) Clift=1.75;\
	else	if(qa>qlim2) Clift=1.75+(0.0-1.75)*saturate((qa-qlim2)/(1.0-qlim2));\
	F+=normal2*A*vel*vel*ro*Clift*0.5*(1.0-fabs(q))*fsgn(q);\
	F_all+=F;\
	M_all+=cross(F,R);\
	}\
	}
#endif

//N+  ha -spd /vdrag/ ellentetes a normallal!
//	if(q>0.0) q3=-q3;


//empty
//#define	add_forceBN3b();


/*
//mivel F mindig +, ezert vec_attack megfordul ha vel<0


L=Iw = rmv =rxp
M=Ia= I dw/dt = rxF

I=mr^2
alf =M/I = a/r = M/r/m/r  !!!!
w   =L/I = v/r

I=mr2  
L=rxp
dL=M
L=Iw
M=rxF
*/



extern int uuu;
extern vec3 target;
extern vec3 mypos;
extern vec3 enemypos;
extern vec3 enemyspd;
extern vec3 enemyacc;
extern int selenemy;

extern int typecc; // 0:f18   1:su35   2:f16
extern double timedump;
extern double timedump2;

class c_airplane;
extern qvector<c_airplane *> airplanelist;
extern float1 xq;


extern c_mesh airplanemesh[64];
extern c_mesh carriermesh;


class c_airplane;
extern c_airplane *selairplane;
extern c_airplane *camairplane;
extern c_airplane *camairplane2; // secondary cam
extern int selcamplane;



int chkdir(int n);
float1 chkdir2(float1 n);
float1 fsqrabs(float1 n);
void deftrustquad(float1 i,float1 j,vec3 base,vec3 dir);


extern float1 flydatas[];

vec3 transform(vec3 v1,axis4 &ax);
vec3 transformNorm(vec3 v1,axis4 &ax);
float1 cut(float1 n,float1 low,float1 hig);




struct s_shoot
{
	vec3 pos;
	vec3 poslast;
	vec3 spd;
	float1 time;
	float1 t2;
	int laserID;
};
extern qvector<s_shoot> shoots;
extern qvector<vec3> lastshoots;


void addShoot(vec3 pos,vec3 dir,vec3 spd,int id);
void drawShoots(float1 dtime);
void drawBraket(vec3 p1, vec3 v2);
void drawPath(float1 dtime);

	
	
	





class c_airplane:public component//<c_airplane>
{
public:
//component<c_scene> *scene;
c_airplane(component *_root)//_base
{
	//scene=static_cast<component<c_scene> *>(_root);
	init();
	return;
}

	vec3 plane_pos;
	vec3 spd,acc;//mph -> m/s
	vec3 omega,L;
	axis plane_mat,head_mat[16];
	vec3 canopy_pos;

	int laserID;
	bool alive;
	
	int planetype;
	objfile plane_obj;
//	vec3 airbox[64][64][64]; // 64=3Mbyte

	float1 wing_ang1,wing_ang2,wing_ang3,wing_ang22;
	vec3 hookpnt,wheeldy[4],wheelymin[4],hookend2;
	int vectortrust_on,verticalmode,verticalheadmode;
	vec3 hookvector,hookforce,hookend,hookendold;
	float1 alf, alf3,	alf4,	alf5,gdir,gdir2;
	float1 speedbrake,sidewing,flapwing;
	float1 wing, wing2;
	int gear,gearneed,whlbreak,autopilot_on,ehook,hookconnect,hookcable,enable_physics,flapstate,speedbrakestate;
	float1 geartimer,trusttime;
	float1 alf1,alf1b,alf1c,alf2,alf2b,trust,vec_dol,gforce,gforce2;

	float1 trimm_x;
	float1 trimm_z;
	int etrim;
	vec3 trim_dir;

	vec3 F_all;
	vec3 M_all;
	vec3 Fcoll,Icoll;// plane vs plane collision
	int collvalid;
	float1 qlim,qlim2,qlim3,Cdrag,Clift,ro;

	float1 ap_alf1,ap_alf2;//autopilot
	float1 trg_hg,trg_dir;//autopilot
	float1 pilottime;
	
	float1 fuel,fuelCons=0;
	int ammo;

	vec3 kondenzcsik[2200][2];
	int n_kondenzcsik, kondcc;
	float1 partcsize[2200];
	vec3 partspd[2200];

	qvector<vec3> debverts;
	qvector<vec3> debverts_old;
	
	vec3 cstick1,cstick2;

	vec3 trust_vec[4];
	vec3 trust_pos[4];
	c_mesh *mesh=0;


	c_airplane()
	{
		init();
		return;
	}

	float1 txc_mir(float1 n)
	{
		n-=mesh->txc_center.y;
		n=-n;
		n+=mesh->txc_center.y;
		
		return n;
	}


void initplaneobj()
{
	if(planetype==1) plane_obj=f18obj; // copy
	if(planetype==2) plane_obj=su35obj;
	if(planetype==3) plane_obj=f16obj;
}
void init()
{
	alive=true;
	laserID=rand()&0xff; laserID<<=8;
	laserID|=rand()&0xff; laserID<<=8;
	laserID|=rand()&0xff; laserID<<=8;
	laserID|=rand()&0xff; 


//planetype=1+rand()%3;
planetype=1+(typecc++)%3;
//if(this==selairplane)	planetype=3;//_LOADplane_obj
//planetype=1;


/*	
preloaded
	if(planetype==1) plane_obj.load_obj("data/f18a9.obj",17.0/9.611,0,2000000); // q_airplane.cpp      17m
	if(planetype==2) plane_obj.load_obj("data/su35a.obj",22.0/9.4,0,2000000); // 22m
	if(planetype==3) plane_obj.load_obj("data/f16a.obj",14.5/9.96,0,2000000); // 14.5m 9.5
	*/
	initplaneobj();


//	loadbmpb("f18aobj","data/f18q.bmp",0,1); // global


	pilottime=0;
	gforce=0,gforce2=0,trust=0.25;
	trg_hg=4000,trg_dir=0;
	ap_alf1=0,ap_alf2=0;
	gear=0,gearneed=0,whlbreak=0,autopilot_on=0,ehook=0,hookconnect=0,hookcable=0;
	geartimer=3.0;
	collvalid=0;
	wing=0;wing2=0;trusttime=0;
	speedbrake=0,sidewing=0,flapwing=0;flapstate=0;speedbrakestate=0;
	vectortrust_on=0,verticalmode=0,verticalheadmode=0;
	wing_ang1=0,wing_ang2=0,wing_ang3=0,wing_ang22=0;
	trimm_x=0.0;
	trimm_z=0.0;
	fuel=6000,ammo=1000; // mass 10-16t
	etrim=0;
	enable_physics=1;
	n_kondenzcsik=0;kondcc=0;
	mesh=0;

	loopd(i,0,2000,1)
	{
		kondenzcsik[i][0]=vec3(i,0,0);
		kondenzcsik[i][1]=vec3(i,0,0);

		partcsize[i]=0;
		partspd[i]=vec3(0,0,0);
	}


	alf=0,alf3=0,	alf4=0,	alf5=0,gdir=0,gdir2=0;
	float1 aa=90.0*rad;//20  70
	
//	plane_pos=vec3(15100,3000,2600);
	plane_pos=vec3(-15100,1000,60);
	plane_pos=vec3(-15100,20000,90000);
plane_pos=vec3(5510,500,2110);
plane_pos=vec3(15510,35000,12110);
plane_pos=vec3(15510,5000,12110);
plane_pos=vec3(15510.0+xq,4000.0,12110.0); xq+=100.0;

	
//	plane_pos=vec3(-5100.0*cos(aa),1000.0,5100.0*sin(aa));
//	plane_pos=vec3(-2100,300,60);//land test
	//plane_pos=vec3(-300,51,50);
	
	lookb[selcam]=vec3(400,300,100);
lookb[selcam]=plane_pos+vec3(-sin(aa),0.0f,-cos(aa) )*400.0f;
//	look=vec3(-400.0*cos(aa),00.0,400.0*sin(aa));
	upb[selcam]=vec3(0,1,0);

	plane_mat.z=normalize(lookb[selcam]-plane_pos);
	plane_mat.x=cross(plane_mat.z,upb[selcam]);
	plane_mat.y=cross(plane_mat.x,plane_mat.z);
	plane_mat.x=normalize(plane_mat.x);
	plane_mat.y=normalize(plane_mat.y);

	spd=plane_mat.z*210.0*knots/3.6;



//	plane_mat.z=vec3(1,0,0);	plane_mat.y=vec3(0,1,0);	plane_mat.x=vec3(0,0,1);
	loopd(i,0,8,1)
	{
	head_mat[i].z=vec3(0,0,1);//look around
	head_mat[i].y=vec3(0,1,0);
	head_mat[i].x=vec3(1,0,0);
	}
#ifdef _TEST	
	rotaxis(head_mat[1],head_mat[1].y,35.0*rad);
	rotaxis(head_mat[1],head_mat[1].x,-20.0*rad);
#else
	rotaxis(head_mat[1],head_mat[1].y,-35.0*rad);
	rotaxis(head_mat[1],head_mat[1].x,-20.0*rad);
#endif
	omega=vec3(1e-15,0.0,0.0);
	L=vec3(1e-15,0.0,0.0);

}
void add_force_rec(vec3 v11,vec3 v22,vec3 v33,int type,vec3 normal,int op)
{
	vec3 R,F;
	float1 A,vel;
	
	if(op==0)
	{
		vec3 cent=(v11 + v22 + v33)/3.0;
		R=cent-plane_pos;
		A=tri_area(v11,v22,v33);
		if(type)  {add_forceB();}
		else	  {add_forceBN3();}
	}
	else
	{
		vec3 w1=(v11+v22+v33)/3.0;
		add_force_rec(w1,v11,v22,type,normal,op-1);
		add_force_rec(w1,v22,v33,type,normal,op-1);
		add_force_rec(w1,v33,v11,type,normal,op-1);
	}
}

// diffgeo4.cpp

float1 getC(float1 q)//qlim   original nem hasznalja!
{
	q=fabs(q);
//	q=acos(q); // 7 fps xD

	//ok old
//	if(q<qlim)	return (0.5+1.25*sin((q/qlim)*M_PI/2.0));//1.75 max  OK
//	else		return (1.3+0.45*(0.5+0.5*cos( ((q-qlim)/(1.0-qlim))*M_PI)));//OK  common! drag+lift!   flat plane=1.3
//test
	if(q<qlim)	return (0.5+1.25*sin((q/qlim)*M_PI/2.0));//1.75 max  OK
	else		return (1.3+0.45*(0.5+0.5*cos( ((q-qlim)/(1.0-qlim))*M_PI)));//OK  common! drag+lift!   flat plane=1.3
}
//	if(q<qlim)	return (0.5+1.25*sin((q/qlim)*M_PI/2.0));//1.75 max  OK
//	else		return (1.2+0.55*cos( ((q-qlim)/(1.0-qlim))*M_PI/2.0));//meredeken le
//	else		return (1.3+0.45*(0.5+0.5*cos( ((q-qlim)/(1.0-qlim))*M_PI)));//OK


float1 getC2(float1 q)
{
	q=fabs(q);
	
// diffgeo4.cpp  curve test
//         1.15=150   1.25=135  fuel=5ton   1.2=130[3ton]=145[5.7ton]  perfect!
	if(q<qlim)	return (0.0+1.2*sin((q/qlim)*M_PI/2.0));//   1.25     1.2 JO!
//	if(q<qlim)	return (0.5+1.25*sin((q/qlim)*M_PI/2.0)); // ertelmetlen 0.5 ellenallas   0 foknal   TEST old
	else		return (1.2-0.25*(0.5-0.5*cos( ((q-qlim)/(1.0-qlim))*M_PI)));

}


	
//cam     z be megy x az oldal  y magassag
//model   x be megy y az oldal  z magassag
vec3 qtransform(vec3 v1)
{
	vec3 v2;
	
//	v2= plane_mat.x*v1.y - plane_mat.y*v1.z -plane_mat.z*v1.x;//forditva x!
	v2= plane_mat.x*v1.y + plane_mat.y*v1.z +plane_mat.z*v1.x;
	return v2;
}
/*	cam2.x=head_mat.x*plane_mat.x.x +head_mat.y*plane_mat.x.y +head_mat.z*plane_mat.x.z;
	cam2.y=head_mat.x*plane_mat.y.x +head_mat.y*plane_mat.y.y +head_mat.z*plane_mat.y.z;
	cam2.z=head_mat.x*plane_mat.z.x +head_mat.y*plane_mat.z.y +head_mat.z*plane_mat.z.z;*/

float1 getaoa(float1 &aoa33)
{
	float1 aoa11=flydatas[1];
	float1 v=fabs(dot(plane_mat.z,spd));
	float1 ve=1e6;
	float1 aoa2=flydatas[1];	//elso
	float1 aoa3=flydatas[2];	//elso
//printf("%e %e %e \n",plane_mat.z.x,plane_mat.z.y,plane_mat.z.z);printf("%e %e %e \n\n",spd.x,spd.y,spd.z);
	
	loopd(i,0,18*3,3)	//64m/s ig
	{
		if(v<ve)
		if(v>=flydatas[i])
		{
			float1 t1=(v-flydatas[i])/(ve-flydatas[i]);//interp
			aoa11=flydatas[i+1]	+(aoa2-flydatas[i+1])*t1;
			aoa33=flydatas[i+2]	+(aoa3-flydatas[i+2])*t1;
		}
		
		ve=flydatas[i];
		aoa2=flydatas[i+1];
		aoa3=flydatas[i+2];
	}
//printf("%e \n",aoa);	

	aoa33*=rad;
	return aoa11*rad;
}

void drawhud(float1 xtime)
{
QDUMPFNC();
	c_font *fnt=qgetcomponent<c_font>("font",LID);
	
	int dy=0;//50
/*	if(selcam!=0)	//HUD setup:  vagy a kicsin van vagy a nagyon
	{
		win_scale=0.4;
		win_pos=vec3(xx/2+200,0,0);
		ecut=0;
	}
	else*/
	{
		win_scale=1.0;
		win_pos=vec3(0,0,0);
	}
	
	
	
	
	if(selcam3==0)
	{
		int hh=30;
     	glViewport(xx/2+hh, yy/2+hh, xx/2-hh*2, yy/2-hh*2);
     }


	//glDisable(GL_DEPTH);	
	glDisable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);
//	glBlendFunc (GL_SRC_COLOR ,GL_ONE_MINUS_SRC_COLOR);//GL_ONE

	setshader("shader0");
	vec3 resol(xx,yy,0);
	shader_setparm3("resol",&resol,2);//uniform
	shader_setparm2("ecut",ecut);//1


	float1 aoa=acos(dot(plane_mat.z,normalize(spd)))/rad;//attack of angle
	aoa=fabs(aoa);
	
//TODO
//	loopd(ly,0,3,1)//3 layers
int ly=2;
	{
	float1 powe=1.0;
	linewidth=1.2;
	vec3 colr1(0.0,0.5,0.0);
	vec3 colr2(0.2,1.0,0.2);


	if(ly==2) 	 glBlendFunc (GL_ONE,GL_ONE_MINUS_SRC_ALPHA );
	if(ly==1) 	//
	{
		linewidth=3.0;
		powe=0.9;
		colr1=vec3(0.0,0.3,0.0);
		colr2=vec3(0.0,0.6,0.0);
		glBlendFunc (GL_ONE,GL_ONE_MINUS_SRC_ALPHA );
	}
	if(ly==0) 	//glow
	{
		linewidth=8.0;
		powe=0.02;
		colr1=vec3(0.0,0.2,0.0);
		colr2=vec3(0.0,0.5,0.0);
		glBlendFunc (GL_ONE,GL_ONE);
	}

	float1 t6=(aoa-10.0)/10.0; if(t6>1.0) t6=1.0;if(t6<0.0) t6=0.0;
//	colr1=lerp(colr1,vec3(1,1,0),t6);
	colr2=lerp(colr2,vec3(1,0,0),t6);
	
	shader_setparm3("power",&powe,1);//uniform
	shader_setparm3("linewidth",&linewidth,1);
	shader_setparm3("col1",&colr1,3);
	shader_setparm3("col2",&colr2,3);
	

	
//?	vec3 eye3=eye/1.75;//9x200m 1800/1024

	
//itt volt



	int col1=0x33ff33;//0x55ff00;
	float1 t5=(fabs(gforce)-5.0)/5.0;//5G tol -10ig
	if(t5<0.0) t5=0.0;	
	if(t5>1.0) t5=1.0;	
	t5=1.0-pow(t5,2.0);

	gforcecolor.x=1.0;
	gforcecolor.y=1.0;
#if 1	
//off
	if(gforce>0.0)
	{
		gforcecolor.x=t5;//blackout 
		gforcecolor.y=1.0;//redout
	}
	else
	{
		gforcecolor.x=1.0;
		gforcecolor.y=t5;
	}

	col1=lerpcol(col1,0xff0000,t5);
#endif
	
//	int col=col1;
	int x4,x2,y4,y2,x3,y3;

//	int col4=0xff3300;
	int col4=0x008800;
	int col5=0x00ff00;
	int col6=0x00ff00;
	int col11=0xff0000;

	
glBegin2(GL_QUADS);//new



	int dy6=-30;//szalag
//height
	x3=xx/2-110; y3=110+dy6;//120
	line(x3,y3,x3,y3+140,col4);
	line(x3,y3+70,x3-5,y3+70-5,col4);
	line(x3,y3+70,x3-5,y3+70+5,col4);
	
	float1 height=(plane_pos.y*3.0+70.0);
	int height2=((int)height)%10;
	int height3=(int)height;
	height3/=10; height3*=10;
	y3+=height2;
	
	loopd(i,0,140,10)//szalag
	{
		int q=10;
		if((((int)height3-i)/10)&1) q=14;
		line(x3,y3+i,x3+q,y3+i,col4);
		
		if(q==14)
		if(i>=50)
		if(i<=90)
			fnt->drawnums(x3+20,y3+i,height3-i,col5);
	}
	{//height ora
	int xb=xx/2-140,yb=280-35;//70;
	float1 r=24.0;
	loopd(i,0,11,1)
	{
		float1 aa=(float1)i*M_PI/5.0;
		float1 x1=xb+(r*sin(aa));
		float1 y1=yb+(r*cos(aa));

		fnt->quad(x1,y1,0,0,col4);
	}
	{
		int u=(int)(plane_pos.y*3.0*10.0);
		u=(u/1)%100;
		float1 aa=(float1)u*M_PI/50.0;
		float1 r2=14.0;
		float1 x1=xb+(r*sin(aa));
		float1 y1=yb-(r*cos(aa));
		float1 x2=xb+(r2*sin(aa));
		float1 y2=yb-(r2*cos(aa));
		
		line(x1,y1,x2,y2,col4);
	}
	fnt->drawnums(xb-10,yb-3,(int)(plane_pos.y*3.0),col5);
	}
	

	{//vertical speeed ora      notacceleration
	int xb=xx/2-140-60,yb=280-20-35;//70;
	float1 r=24.0;
	loopd(i,0,11,1)
	{
		float1 aa=(float1)i*M_PI/5.0;
		float1 x1=xb+(r*sin(aa));
		float1 y1=yb+(r*cos(aa));

		fnt->quad(x1,y1,0,0,col4);
	}
	{
		int u=(int)(spd.y*3.0*10.0*60.0/100.0);// per minute
		u=(u/1)%100;
		float1 aa=(float1)u*M_PI/50.0;
		float1 r2=14.0;
		float1 x1=xb+(r*sin(aa));
		float1 y1=yb-(r*cos(aa));
		float1 x2=xb+(r2*sin(aa));
		float1 y2=yb-(r2*cos(aa));
		
		line(x1,y1,x2,y2,col4);
	}
	fnt->drawnums(xb-10,yb-3,(int)(spd.y*3.0*60.0),col5);
	}


	float1 trust5=trust+sin(trusttime*M_PI/1.0); // + ideiglenes impulse '1'
	if(trust5>1.0) trust5=1.0;
	
//thrust
	fnt->drawalps(200,280+dy,"PWR Q/A",col4,2);//+16
	fnt->drawnums(250,280+dy,(int)(trust5*100.0),col5); //240
	fnt->drawnums(185,300+dy,(int)gforce,col5);
	fnt->drawalps(175,300+dy,"G",col5,2);//+16

	if(ehelp)
	{
		int x=20,y=10,h=12;
		fnt->drawalps(x,y,"PHYSICS TECH DEMO",col6,2);y+=h;
		fnt->drawalps(x,y,"ESC   EXIT",col6,2);y+=h;
		fnt->drawalps(x,y,"F1    HELP",col6,2);y+=h;
		fnt->drawalps(x,y,"SPACE AUTO PILOT",col6,2);y+=h;
		fnt->drawalps(x,y,"P     PAUSE",col6,2);y+=h;
		fnt->drawalps(x,y,"C     CAMERA",col6,2);y+=h;
		fnt->drawalps(x,y,"L     NIGHTMODE",col6,2);y+=h;
		fnt->drawalps(x,y,"O     WIND",col6,2);y+=h;
		fnt->drawalps(x,y,"R     RESET POS",col6,2);y+=h;
		fnt->drawalps(x,y,"JOYSTICK BAD",col6,2);y+=h;
		fnt->drawalps(x,y,"J     JOYSTICK/MOUSE",col6,2);y+=h;
		fnt->drawalps(x,y,"R BRACKET   VTOL MODE",col6,2);y+=h;
		fnt->drawalps(x,y,"K CHANGE PLANE",col6,2);y+=h;
		fnt->drawalps(x,y,"MOUSE RIGHT DOWN",col6,2);y+=h;
		fnt->drawalps(x,y,"HEAD MOVE",col6,2);y+=h;
		
		x=130,y=10+h;
		fnt->drawalps(x,y,"G     GEAR",col6,2);y+=h;
		fnt->drawalps(x,y,"H     HOOK",col6,2);y+=h;
		fnt->drawalps(x,y,"T     VECTOR THRUSTING",col6,2);y+=h;
		fnt->drawalps(x,y,"1     IMPULSE",col6,2);y+=h;
		fnt->drawalps(x,y,"2     NEXT TARGET",col6,2);y+=h;
		fnt->drawalps(x,y,"Q/A   POWER",col6,2);y+=h;
		fnt->drawalps(x,y,"F     FLAPS 50%",col6,2);y+=h;///V
		fnt->drawalps(x,y,"B     SPEEDBRAKE 50%",col6,2);y+=h;///N
		fnt->drawalps(x,y,"Z/X   SIDE PEDAL",col6,2);y+=h;
	}
	
//spd
	x3=110; y3=110+dy6;//120
	line(x3,y3,x3,y3+140,col4);
	line(x3,y3+70,x3+5,y3+70-5,col4);
	line(x3,y3+70,x3+5,y3+70+5,col4);
	
	float1 v1=length(spd)*3.6/knots;//m/s -> mph
	int spd3=(int)(v1+70.0);
	int spd4=spd3%10;
	spd3/=10; spd3*=10;
	y3+=spd4;
	
	loopd(i,0,140,10)//szalag
	{
		int q=10;
		if(((spd3-i)/10)&1) q=14;
		line(x3-q,y3+i,x3,y3+i,col4);
		
		if(q==14)
		if(i>=50)
		if(i<=90)
			fnt->drawnums(x3-35,y3+i,spd3-i,col5);
	}
	{//spd ora
	int xb=140+60,yb=280-20-35;//140,yb=280;
	float1 r=24.0;
	loopd(i,0,11,1)
	{
		float1 aa=(float1)i*M_PI/5.0;
		int x1=xb+(int)(r*sin(aa));
		int y1=yb+(int)(r*cos(aa));

		fnt->quad(x1,y1,0,0,col4);
	}
	{
		int u=(int)(v1*10.0);//100?
		u=(u/1)%100;
		float1 aa=(float1)u*M_PI/50.0;
		float1 r2=14.0;
		float1 x1=xb+(r*sin(aa));
		float1 y1=yb-(r*cos(aa));
		float1 x2=xb+(r2*sin(aa));
		float1 y2=yb-(r2*cos(aa));
		
		line(x1,y1,x2,y2,col4);
	}
	fnt->drawnums(xb-10,yb-3,(int)v1,col5);
//	fnt->drawnums(120,300,(int)v1,col);
	}


	{// acceleration
	int xb=140+0,yb=280-0-35;//70; +60 +20
	float1 r=24.0;
	loopd(i,0,11,1)
	{
		float1 aa=(float1)i*M_PI/5.0;
		float1 x1=xb+(r*sin(aa));
		float1 y1=yb+(r*cos(aa));

		fnt->quad(x1,y1,0,0,col4);
	}
	float1 a1=length(acc)*10.0/9.8;//	float1 a1=length(acc)*3.6/knots;
	{
		int u=(int)(a1*10.0);
		u=(u/1)%100;
		float1 aa=(float1)u*M_PI/50.0;
		float1 r2=14.0;
		float1 x1=xb+(r*sin(aa));
		float1 y1=yb-(r*cos(aa));
		float1 x2=xb+(r2*sin(aa));
		float1 y2=yb-(r2*cos(aa));
		
		line(x1,y1,x2,y2,col4);
	}
	fnt->drawnums(xb-10,yb-3,(int)a1,col5);
	}



//heading, irany
	x3=xx22/2-80; y3=100;//	x3=xx22-80; y3=50;
//	line(x3,y3,x3+200,y3,col);
	line(x3+80,y3+2,x3+80-5,y3+7,col4);
	line(x3+80,y3+2,x3+80+5,y3+7,col4);
	fnt->drawnums(x3+73,y3+12,(int)gdir2,col5);
	
	float1 dir3=(gdir2-20);
	if(dir3<0.0) dir3+=360.0;//mod bug
	float1 dir4=fmod(dir3,5.0);
	dir3/=5; dir3*=5;
	x3-=(int)(dir4*4.0);// step=20
x3+=20;//new
	
	loopd(i,1,8,1)//8
	{
		int q=10,ir=chkdir((int)dir3+i*5);
		if(((ir/5)&1)==0) q=14;
//		line(x3+i,y3,x3+i,y3+q,col);
		fnt->quad(x3,y3,0,0,col4);
		
		if(q==14)
			fnt->drawnums(x3-5,y3-9,ir/10,col5,2);//+16
		x3+=20;
	}


//moving direction, TVV true velocity vector
	{
	float1 r=5.0;
	int x2,y2,xb=0,yb=0;
	vec3 spd5=normalize(spd);
	vec3 v5=spd5*300.0;//no eye pos! 
	
//fake	xb=xx+(int)(900.0*asin(dot(camx,spd5))/M_PI); //yy=400 = 40 fok
//	yb=yy22-(int)(900.0*asin(dot(camy,spd5))/M_PI);
	
	
//	if(fnt->conv3dchk2(v5,xb,yb))	if(xb>20)	if(yb>10)	if(xb<xx-20)	if(yb<yy-10)
	{
	if(fnt->conv3dchk2b(v5,xb,yb))
	{
		lineb(xx22,yy22,xb,yb,col6);//merre?
		fnt->drawnums(xb/2+10,yb/2,aoa,col6,3);		
	}
	
	lineb(xb-5,yb,xb-15,yb,col6);
	lineb(xb+5,yb,xb+15,yb,col6);

	loopd(i,0,11,1)
	{
		float1 aa=(float1)i*M_PI/5.0;
		int x1=xb+(int)(r*cos(aa));
		int y1=yb+(int)(r*sin(aa));
		
		if(i)
			lineb(x1,y1,x2,y2,col6);
		x2=x1;
		y2=y1;
	}
	}
		
	if(xb>20)//new
	if(yb>10)
	if(xb<xx-20)
	if(yb<yy-10)
	{
	//aoa
	if(aoa>=2.0)
	{              //15          10
		int dy=((int)(15.0*(-(aoa-7.0))));//10 foknal egal      5fok=50pix  10 kellene
		xb-=22;
		yb+=dy;
		lineb(xb,yb, xb,yb+50,col6);
		lineb(xb,yb, xb+10,yb,col6);
		lineb(xb,yb+50, xb+10,yb+50,col6);
	}

//	iiddee
	}
	}



	
//horizont	
	//spd bol kell a horizont kozepe
	vec3 wwx=spd,wwy(0,1,0),wwz;//fix=>camz ,   f16 style => spd
	wwx.y=0;
	wwx=normalize(wwx);
	wwz=cross(wwx,wwy);

	float1 c1=cos(alf2);	
	float1 s1=sin(alf2);	
	
	loopd(i,0,360,5)
	{
		float1 aa=(float1)i*M_PI/180.0;
		
		vec3 v1=(wwx*cos(aa) + wwy*sin(aa))*100.0;
		float1 t2=7.0;
		if(i==0) t2=21.0;
		vec3 v2=v1+wwz*t2;//17
		vec3 v3=v1-wwz*t2;
		
		
		if(fnt->conv3dchk2(v2,x2,y2))
		if(fnt->conv3dchk2(v3,x3,y3))
		{
//			fnt->quad(x1,y1,0x00ff00);
//			fnt->quad(x2,y2,0x00ff00);

			int x2b,y2b,x3b,y3b;
			vec3 v2b=v1+wwz*2.4;//17
			vec3 v3b=v1-wwz*2.4;
			fnt->conv3dchk2(v2b,x2b,y2b);
			fnt->conv3dchk2(v3b,x3b,y3b);
			
//			col=col1;
//			if(i>180)  col=0xff5500;

			if(i<180)
			{
				lineb(x2,y2,x2b,y2b,col6);//degress lines
				lineb(x3b,y3b,x3,y3,col6);
			}
			else
			{
				lineb(x2,y2,x2b,y2b,col6);//5
				lineb(x3b,y3b,x3,y3,col6);
			}

			int i2=i;
			if(i2>180) i2=(i2-360);
			
			if(i>180) fnt->drawnumsr(x3/2,y3/2,i2,col6,3,-9,c1,s1);//degress
			else      fnt->drawnumsr(x3/2,y3/2,i2,col6,3, 7,c1,s1);

			vec3 v4;
			if(i>=180 || i==0) //jel belul , ground
			{
				v4=v2b;		v4.y+=1.2;		if(i==0) v4.y-=1.2*2;
				if(fnt->conv3dchk2(v4,x4,y4))	lineb(x2b,y2b,x4,y4,col6);

				v4=v3b;		v4.y+=1.2;		if(i==0) v4.y-=1.2*2;
				if(fnt->conv3dchk2(v4,x4,y4))	lineb(x3b,y3b,x4,y4,col6);
			}
			else//kivul   , sky
			{
				v4=v2;		v4.y-=1.2;
				if(fnt->conv3dchk2(v4,x4,y4))	lineb(x2,y2,x4,y4,col6);

				v4=v3;		v4.y-=1.2;
				if(fnt->conv3dchk2(v4,x4,y4))	lineb(x3,y3,x4,y4,col6);
			}
		}
	}


//carrier ILS point
	vec3 tpos=carriermesh.pnts_trn[74];//carrier_pos


//marker  
	{
//		col=0x55ff00;
	vec3 v4(0,0,0);
//	v4+=carrier_pos;	v4.y+=20.0;//!!!!!!!!!!!!!!!!11
	v4=tpos;
	
	v4-=plane_pos;
	if(fnt->conv3dchk3(v4,x4,y4))	
//	if(x4<20) x4=20;	if(y4<20) y4=20;	if(x4>xx22*2-20) x4=xx22*2-20;	if(y4>yy22*2-20) y4=yy22*2-20;
	{
		int h=4;
		lineb(x4-h,y4-h, x4+h,y4-h,col5);//col4 sotet
		lineb(x4+h,y4-h, x4+h,y4+h,col5);
		lineb(x4+h,y4+h, x4-h,y4+h,col5);
		lineb(x4-h,y4+h, x4-h,y4-h,col5);
	}
//marker direction	
		float1 r=5.0;
		int xb=xx22-60;
		int yb=yy22+30;
		loopd(i,0,11,1)
		{
			float1 aa=(float1)i*M_PI/5.0;
			int x1=xb+(int)(r*cos(aa));
			int y1=yb+(int)(r*sin(aa));
		
			if(i)
				lineb(x1,y1,x2,y2,col4);
			x2=x1;
			y2=y1;
		}
		
		int dx=x4-xb;
		int dy=y4-yb;
		int t1=(int)sqrt((float1)(dx*dx+dy*dy));
		if(t1==0) t1=1;
		x4=xb+15*dx/t1;
		y4=yb+15*dy/t1;
		lineb(xb,yb,x4,y4,col4);
	}


	{
//enemy markers
	int sel=-1;
	float1 tmin=1e16;
	loopd(i,1,airplanelist.size(),1)//sajat nem
	{
		vec3 v4=airplanelist[i]->plane_pos-plane_pos;
		float1 t4=length(v4);
		
		if(i==selenemy)
		{
			enemypos=airplanelist[i]->plane_pos;
			enemyacc=(airplanelist[i]->spd -enemyspd)/xtime;
			enemyspd=airplanelist[i]->spd;
		}
		sel=selenemy;
/*		
		if(t4<tmin) 
		{
			tmin=t4;
			sel=i;
			enemypos=airplanelist[i]->plane_pos;
			enemyacc=(airplanelist[i]->spd -enemyspd)/xtime;
			enemyspd=airplanelist[i]->spd;

			if(selenemy!=i) enemyacc=vec3(0,0,0);		selenemy=i;
		}*/
	}
	loopd(i,1,airplanelist.size(),1)//sajat nem
	{
	vec3 v4=airplanelist[i]->plane_pos;
	vec3 v5=airplanelist[i]->plane_pos + airplanelist[i]->spd*2.0;
	v4-=plane_pos;
	if(v4.x==0.0) v4.x=0.01;
	float1 aa=90.0-atan(v4.z/v4.x)/iradian;
	if(v4.x>0) aa+=180.0;
	
	int ok=0;
	int vis=fnt->conv3dchk3c(v4,x4,y4,ok);
//	if(fnt->conv3dchk3(v4,x4,y4))	
	{
	/*	if(x4<20) x4=20;
		if(y4<20) y4=20;
		if(x4>xx22*2-20) x4=xx22*2-20;
		if(y4>yy22*2-20) y4=yy22*2-20;*/
	fontscale5=1.5;
	fontsize5=0.5;
	
		int col4b=0x005500;
		int col5b=0x00aa00;
		int col9;

		int col68=col5b,col49=col4b;//6,4
		{
		int h=4,col48=col5b;
		if(i==sel) 	col48=0xffff00;
		if(!airplanelist[i]->alive) {col48=0x550000;col68=0x550000;col49=0x550000;}
		col9=col48;
		if(vis==0) col48&=0x7f7f7f7f;//col4b;
		lineb(x4-h,y4-h, x4+h,y4-h,col48);
		lineb(x4+h,y4-h, x4+h,y4+h,col48);
		lineb(x4+h,y4+h, x4-h,y4+h,col48);
		lineb(x4-h,y4+h, x4-h,y4-h,col48);

		if(ok)// vonalak kozeprol az enemy fele
		{
			float1 dx=x4-xx/2;
			float1 dy=y4-yy/2;
			float1 t1=sqrt(dx*dx+dy*dy);
			dx=dx*50.0/t1; dx=x4-dx;
			dy=dy*50.0/t1; dy=y4-dy;
			lineb(x4,y4,dx,dy,col48);
		}
		
		float1 t4=length(v4);
		fnt->drawnums(x4/2,y4/2+12,floor(t4/100.0),col68,3);		//+-15
		t4=dot(airplanelist[i]->spd -spd,plane_mat.z);
		fnt->drawnums(x4/2,y4/2,aa,col49,3);		
		fnt->drawnums(x4/2,y4/2-12,t4,col68,4);		
	fontscale5=2.0;//restore
	fontsize5=1.0;
		}


//kis relative enemy irany half-sphere
		if(i==sel)
		{
		float1 r=65.0;
		int xb=xx22+390;
		int yb=yy22+150;
		int col95=col9;

		vec3 v41=airplanelist[i]->plane_pos;
		vec3 v44=transform(v41,mesh->itramat);
		vec3 dir=normalize(v44);

		vec3 v54=transform(v5,mesh->itramat);// precalc pos
		vec3 dir5=normalize(v54);

		if(dir.x<0.0) col9&=0x7f7f7f7f;//dark
//		if(dir5.x<0.0) col95&=0x7f7f7f7f;//dark

		x4=xb+(int)(r*dir.y);
		y4=yb-(int)(r*dir.z);
		lineb(xb,yb, x4,y4,col9);

		int x45=xb+(int)(r*dir5.y);
		int y45=yb-(int)(r*dir5.z);
		lineb(x4,y4, x45,y45,0xff0000);

		loopd(i,0,21,1)
		{
			float1 aa=(float1)i*M_PI/10.0;
			int x1=xb+(int)(r*cos(aa));
			int y1=yb+(int)(r*sin(aa));
		
			if(i)
				lineb(x1,y1,x2,y2,col4b);
			x2=x1;
			y2=y1;
		}
		}
		


//marker direction	
		float1 r=5.0;
		int xb=xx22-60;
		int yb=yy22+30;
		if(i==sel)//legkozelebbi
		{
		loopd(i2,0,11,1)
		{
			float1 aa=(float1)i2*M_PI/5.0;
			int x1=xb+(int)(r*cos(aa));
			int y1=yb+(int)(r*sin(aa));
		
			if(i2)
				lineb(x1,y1,x2,y2,col4);
			x2=x1;
			y2=y1;
		}
		
		int dx=x4-xb;
		int dy=y4-yb;
		int t1=(int)sqrt((float1)(dx*dx+dy*dy));
		if(t1==0) t1=1;
		x4=xb+21*dx/t1;//long
		y4=yb+21*dy/t1;
		lineb(xb,yb,x4,y4,col4);
		}
	}
/*
	else//mogotte
	{
		vec3 axy=vec3(0,1,0);
		vec3 axx=cross(plane_mat.z,axy);
		
		int x5=dot(v4,axx)/1.0 + xx/2;
		int y5=dot(v4,axy)/1.0 + yy/2;
		int z5=dot(v4,plane_mat.z);
		if(x5<10) x5=10;
		if(x5>xx-10) x5=xx-10;
		if(y5<10) y5=10;
		if(y5>yy-10) y5=yy-10;
		
		if(z5<0)
		{
			lineb(x5,10, x5,yy-10,col5);
			lineb(10,y5, xx-10,y5,col5);
		}
		else
		{
			lineb(x5,10, x5,yy-10,col4);
			lineb(10,y5, xx-10,y5,col4);
		}
	}*/
	
	}
	}
	

	{
	//gun target
	
	vec3 rocket=plane_pos,  spdrocket=plane_mat.z*1000.0+spd; // 300
	float1 t1=0,tenemy=10000.0,dt4=0.02;
	vec3 dir=normalize(enemypos-plane_pos);

	vec3 enemypos2=enemypos;
	vec3 enemyspd2=enemyspd;
	while(t1>=0 && t1<5000.0 && t1<tenemy)
	{
		rocket+=spdrocket*dt4;//kisebb stepp
		spdrocket.y -= 9.8*dt4;//grav
		enemypos2+=enemyspd2*dt4;
		enemyspd2+=enemyacc*dt4;

		t1=dot(rocket-plane_pos,dir );
		tenemy=dot(enemypos2-plane_pos,dir );
	}
	if(t1>0)
	if(t1<5000.0)	
	{
		target=rocket + enemypos - enemypos2;//nem a mostani pos hoz kepest hanem a jovobelihez


		vec3 v4=target-plane_pos;//relative 
		vec3 v5=rocket-enemypos2;// ez viszont konkret tav
		int ok=0;
		int vis=fnt->conv3dchk3c(v4,x4,y4,ok);
		float1 r=9.0,t6=length(v5);
		int col44=0x000000;
		if(t6<10.0) col44=0xff0000;
		else
		if(t6<50.0) col44=0xffff00;
		else
		if(t6<100.0) col44=0x999900;
		else		 col44=0x000000;

		loopd(i2,0,11,1)
		{
			float1 aa=(float1)i2*M_PI/5.0;
			int x1=x4+(int)(r*cos(aa));
			int y1=y4+(int)(r*sin(aa));
		
			if(i2)
				lineb(x1,y1,x2,y2,col44);
			x2=x1;
			y2=y1;
		}
	}
	}
	
// W sing	
	{
	int x22,y22;
	vec3 v2=plane_mat.z;
	fnt->conv3dchk2(v2,x22,y22);//delta_center miatt
//	col=0x55ff00;
	lineb(x22,y22,     x22+5,y22+10,col6);// W sing
	lineb(x22+5,y22+10,x22+10,y22,col6);
	lineb(x22+10,y22  ,x22+30,y22,col6);
	
	lineb(x22,y22,     x22-5,y22+10,col6);
	lineb(x22-5,y22+10,x22-10,y22,col6);
	lineb(x22-10,y22  ,x22-30,y22,col6);
	}


//ILS
	vec3 trg(3.159943e+03,0.000000e+00, -1.174027e+03 );//h
	if(selils==1) trg=vec3(3.658545e+03, 0.000000e+00, -4.201796e+02); //m
	if(selils==2) trg=vec3(-1.905261e+03, 0.000000e+00, 3.045283e+02 ); //m
	if(selils==3) trg=vec3(0.0e+00, 0.000000e+00, -0.0e+00); //m
	float1 alfils=atan2(trg.x,trg.z)/rad;//zx 90.0+
alfils=82.0;//carrier

	{
//printf("%e \n",alf5);		
	float1 alf=alfils*rad;//69.5
	float1 alf2=-4.0*rad;
	vec3 runwayxz(sin(alf),0.0f,cos(alf));//normal!!
	runwayxz=cross(runwayxz,vec3(0,1,0));
	vec3 runwayy(sin(alf2),cos(alf2),0.0f);

//	vec3 runwayxz2(cos(alf),0.0,sin(alf));//normal!!
//	vec3 nor=cross(runwayxz2,vec3(0,1,0));

	vec3 w1=plane_pos-tpos;//cos alf
	w1.y=0; w1=normalize(w1);
	int a=(int)(dot(runwayxz,w1)*4000.0);//sens 20x!
	if(a<-200) a=-200;
	if(a> 200) a= 200;
	lineb(xx22-a,yy22-100  ,xx22-a,yy22+100,col6);

	w1=plane_pos-tpos;//cos alf
	w1-=runwayxz*dot(runwayxz,w1); //nor
	w1.x=sqrt(w1.x*w1.x + w1.z*w1.z);//make 2d
	w1.z=0;
	w1=normalize(w1);
	a=(int)(-dot(runwayy,w1)*4000.0);//1200
	if(a<-170) a=-170;
	if(a> 170) a= 170;
	lineb(xx22-100,yy22-a  ,xx22+100,yy22-a,col6);
	}
	
//if(0)
	{
	int x5,x6,x7,y5,y6,y7;//3d ILS
//	col=0x55ff00;
	float1 alf=91.0*rad;
	vec3 v4a(-250,0,-10);//x be vagyok
	vec3 dirx((float1)sin(alf),(float1)sin(5.0f*rad),(float1)cos(alf));
	vec3 diry(0.0,1.0,0.0);//0.5
	vec3 dirz=dirx;
	dirz.x= dirx.z;
	dirz.z=-dirx.x;
	dirz.y=0.0;
	dirz=normalize(dirz);
	
	v4a+=dirx*100.0;
	float1 cubesize=25.0;
	
#if 0	
	srand(55);
	loopd(i,0,31,1)//10 km 15
	{
		vec3 v4=v4a;
		vec3 eye=cam_eyeb[selcam];
		
		float1 aa=(float1)i*M_PI*10.0/45.0;
		v4.z+=sin(aa)*150.0;	v4.y+=cos(aa)*150.0;
//v4.x=frnd(8000.0);v4.z=frnd(8000.0);v4.y=25;
		
		if(fnt->conv3dchk2(v4-eye -diry*cubesize/2 -dirz*cubesize,x4,y4))//-eye
		{	
		fnt->conv3dchk2(v4-eye +diry*cubesize/2 -dirz*cubesize,x5,y5);	
		fnt->conv3dchk2(v4-eye +diry*cubesize/2 +dirz*cubesize,x6,y6);	
		fnt->conv3dchk2(v4-eye -diry*cubesize/2 +dirz*cubesize,x7,y7);	

		lineb(x4,y4, x5,y5,col);
		lineb(x5,y5, x6,y6,col);
		lineb(x6,y6, x7,y7,col);
		lineb(x7,y7, x4,y4,col);
		}
//		cubesize*=1.2;
		v4a+=dirx*700.0;
	}
#endif
	
	}
	
	//control stick pos
	line(cstick2.x/2,cstick2.y/2,cstick1.x/2,cstick1.y/2,col4);
	

//	fnt->drawnums(200,320+dy,(int)length(spd*3.6),col,5);//+16 500

	fnt->drawalps(200,300+dy,"FLAP F",col4,2);//+16  /V
	fnt->drawnums(250,300+dy,(int)(flapwing*100.0),col5,2);//+16
	fnt->drawalps(275,300+dy,"Z/X",col4,2);//+16
	fnt->drawnums(295,300+dy,(int)(sidewing*100.0),col5,2);//+16
	fnt->drawalps(320,300+dy,"SBRK B",col4,2);//+16  /N
	fnt->drawnums(370,300+dy,(int)(speedbrake*100.0),col5,2);//+16

	//if(gear) 
//	fnt->drawnums(280,280+dy,gear,col,1);
//	fnt->drawnums(290,280+dy,ehook,col,1);
	const char *ss="UP";	
	if(gearneed!=gear) ss="XXX"; 	
	else	if(gear) ss="DOWN"; 	
	if(gearneed==2) ss="ERR"; 	
	fnt->drawalps(330,280+dy,ss,col5,1);
	fnt->drawalps(280,280+dy,"GEAR G/G",col4,2);//+16

	ss="OFF";	
	if(ehook) ss="ON"; 	
	fnt->drawalps(410,280+dy,ss,col5,1);
	fnt->drawalps(360,280+dy,"HOOK H/H",col4,2);//+16


	vec3 w5=carrier_pos-plane_pos;
	float1 ils_dir=atan2(-w5.x,-w5.z)/rad;//+180.0;   -x?
	ils_dir=chkdir2(ils_dir);

	float1 ils_dst=length(w5);
//	fnt->drawnums(265,280,(int)ils_dir,col);//hatrebb
//	fnt->drawnums(295,280,(int)(ils_dst/knots),col);//merfold




//	fnt->drawnums(340,280,(int)(wing*10.0),0x00ff00,3);
//	fnt->drawnums(340,300,(int)(wing2*10.0),0x00ff00,3);

//	fnt->drawnums(340,280+dy,(int)(trg_hg),0xffff00,3);
//	fnt->drawnums(340,300+dy,(int)(trg_dir),0xffff00,3);
	fnt->drawalps(400,300+dy,"FPS",col4,3);
	fnt->drawnums(420,300+dy,(int)(1.0/xtime),col5,3);// TODO
	
//	fnt->drawnums(450,300+dy,(int)(timedump2*100000.0),col5,5);
//	fnt->drawnums(450,280+dy,(int)(timedump*100000.0),col5,5);
//	fnt->drawnums(340,320+dy,(int)(facecc),0xffff00,5);
	fnt->drawalps(150,200+dy,"ALP",col4,3);
	fnt->drawnums(170,200+dy,(int)(aoa*10.0),col5,3);

	fnt->drawalps(120,280+dy,"FUEL",col4,3); // timedump helyen
	fnt->drawnums(150,280+dy,(int)(fuel),col5,4);

	float1 fuel_dt=fuel/fuelCons;    //  s=v*dt dt=fuel/fuelcons   s=v*f/fc  merfold
	float1 fuel_s=length(spd)*fuel_dt/mph/1e3;
	fnt->drawalps(120,300+dy,"DIST",col4,3); // timedump helyen
	fnt->drawnums(150,300+dy,(int)fuel_s,col5,4);
	fnt->drawalps(120,316+dy,"TIME",col4,3); // timedump helyen
	fnt->drawnums(150,316+dy,(int)(fuel_dt/60.0),col5,4);// minute
	
	fnt->drawalps(440,280+dy,"AMMO",col4,3); // timedump helyen
	fnt->drawnums(470,280+dy,(int)(ammo),col5,4);
timedump=0;
timedump2=0;


	int dy2=-110;
//enemy pos radar  old   terkep
	{
	int xb=75,yb=362+20+dy2;
	vec3 dir2d=plane_mat.z;	dir2d.y=0;	dir2d=normalize(dir2d);
	vec3 dir2d2=cross(dir2d,vec3(0,1,0));
//	vector<component_base*> *tmp;	getchild(tmp);	loopd(i,1,(*tmp)->size(),1)
	float1 zoomfactor=1.0/exp2(mapzoom);
	float1 ar5=90.0/(200000.0*zoomfactor);// 1x == 250pixel          
	
	loopd(i,0,airplanelist.size(),1)
	{
//		vec3 w1=((c_airplane*)(*tmp)[i])->plane_pos;
		vec3 w1=airplanelist[i]->plane_pos;
		w1-=plane_pos;
		int x3=xb+0-(int)(dot(w1,dir2d2)*ar5);	// *0.002     mapzoom       
		int y3=yb+0-(int)(dot(w1,dir2d)*ar5);
		if(x3<xb-40) x3=xb-40;
		if(y3<yb-40) y3=yb-40;
		if(x3>xb+40) x3=xb+40;
		if(y3>yb+40) y3=yb+40;
		
		fnt->quad(x3,y3,0,0,col11);
	}
	{
		vec3 w1=carrier_pos;
		w1-=plane_pos;
		int x3=xb+0-(int)(dot(w1,dir2d2)*ar5);	// *0.002
		int y3=yb+0-(int)(dot(w1,dir2d)*ar5);
		if(x3<xb-40) x3=xb-40;
		if(y3<yb-40) y3=yb-40;
		if(x3>xb+40) x3=xb+40;
		if(y3>yb+40) y3=yb+40;
		
		fnt->quad(x3,y3,0,0,col11);
	}
	}


//heading  terkep
	shader_setparm2("ecut",0);
	{//hdg ora
	int xb=75,yb=362+20+dy2;
	float1 r=44.0;
	loopd(i,0,37,1)
	{
		float1 aa=(float1)i*M_PI/18.0;
		float1 x1=xb+(r*sin(aa));
		float1 y1=yb+(r*cos(aa));

		fnt->quad(x1,y1,0,0,col4);

		{
		aa+=(gdir+180.0)*rad;
		x1=xb+(r*sin(aa)*0.7);
		y1=yb+(r*cos(aa)*0.7);
		float1 c1=cos(-aa);	
		float1 s1=sin(-aa);	
		if(i==0)  fnt->drawnumr2(x1,y1,'n',col5,c1,s1);
//		if(i==3)  fnt->drawnumr2(x1,y1,'3',col5,c1,s1);
		if(i==9)  fnt->drawnumr2(x1,y1,'w',col5,c1,s1);//chg
		if(i==18) fnt->drawnumr2(x1,y1,'s',col5,c1,s1);
		if(i==27) fnt->drawnumr2(x1,y1,'e',col5,c1,s1);
		}
	}
	fnt->drawnums(50,350+20+dy2,(int)ils_dir,col5);//hatrebb
	fnt->drawnumsD(90,350+20+dy2,(int)(ils_dst*10.0/mph/1e3),col5);//merfold
	
		int u=(int)(-gdir);//north
		float1 aa=(float1)u*M_PI/180.0;
		float1 r3=47.0;
		float1 r2=37.0;
		int x1=xb+(int)(r3*sin(aa));
		int y1=yb-(int)(r3*cos(aa));
		int x2=xb+(int)(r2*sin(aa));
		int y2=yb-(int)(r2*cos(aa));
		
		line(x1,y1,x2,y2,col4);

		
		u=(int)(ils_dir -gdir);//trg
		//u=(u/1)%100;
		aa=(float1)u*M_PI/180.0;
		r2=22.0;
		x1=xb+(int)(r*sin(aa));
		y1=yb-(int)(r*cos(aa));
		x2=xb+(int)(r2*sin(aa));
		y2=yb-(int)(r2*cos(aa));
		
		line(x1,y1,x2,y2,col4);


		u=(int)(alfils- gdir);//ILS orient
		aa=(float1)u*M_PI/180.0;
		r2= 22.0;
		r3=-22.0;
		x1=xb+(int)(r3*sin(aa));
		y1=yb-(int)(r3*cos(aa));//-
		x2=xb+(int)(r2*sin(aa));
		y2=yb-(int)(r2*cos(aa));//-
		
		line(x1,y1,x2,y2,col4);


		line(xb-5,yb,xb+5,yb,col4);//hdg cross
		line(xb,yb-3,xb,yb+3,col4);

		line(xb-4,yb-44-2,xb,yb-44,col4);// >
		line(xb+4,yb-44-2,xb,yb-44,col4);

	}	
glEnd2();//lineb quad(
	
	shader_setparm2("ecut",ecut);
	}
	
	
	
}

/*
//old transform
		if(vtypes[i]&32)//wheel
		{
			int o=2;//elso
			if(v11.x<0.0) 	o=0;//hatso
			float1 dy=wheeldy[o];
			v11.z+=dy;			
			
			vec3 rcent=pnts[90];//90 134
			if(o!=2) rcent=pnts[134];
			v11=rotatevradcent(v11,vec3(0,1,0),geartimer*90.0*rad/3.0,rcent);
		}
		if(vtypes[i]&4)	v11=rotatevradcent(v11,vec3(0,1,0),-wing_ang2-wing_ang1/1.5,rotcent11);//hatso wing
		if(vtypes[i]&2)	v11=rotatevradcent(v11,axis0,wing_ang1,rotcent21);//elso wing
		if(vtypes[i]&8)	v11=rotatevradcent(v11,axis3,wing_ang3,rotcent31);//felso wing


		v11.y=-v11.y;
		if(mesh->types[i]&32)//wheel
		{
			int o=2;//elso
			if(v11.x<0.0) 	o=1;//hatso
			float1 dy=wheeldy[o];
			v11.z+=dy;			

			vec3 rcent=mesh->pnts[90];//90 134
			if(o!=2) rcent=mesh->pnts[134];		//rcent.y=-rcent.y;
			v11=rotatevradcent(v11,vec3(0,1,0),geartimer*90.0*rad/3.0,rcent);
		}
		if(mesh->types[i]&4)	v11=rotatevradcent(v11,vec3(0,1,0),-wing_ang2+wing_ang1/1.5,rotcent12);//hatso wing
		if(mesh->types[i]&2)	v11=rotatevradcent(v11,axis2,wing_ang1,rotcent22);//elso wing
		if(mesh->types[i]&8)	v11=rotatevradcent(v11,axis32,wing_ang3,rotcent32);//felso wing

*/

void pretransform(float1 xtime)
{
QDUMPFNC();
qassert(mesh);
//printf("mesh->pnts %d \n",mesh->pnts.size());

#ifdef _USE_SU35
//su35
	vec3 rcentwhl1=mesh->pnts[90];//2  elso
	vec3 rcentwhl2=mesh->pnts[134];

	vec3 axis0=normalize(mesh->pnts[41]-mesh->pnts[40]);//nem transformalt kell!
	vec3 axis02=axis0;	axis02.y=-axis02.y;
	vec3 axis3=normalize(mesh->pnts[58]-mesh->pnts[61]);
	vec3 axis32=axis3;	axis32.y=-axis32.y;

	vec3 axis40=normalize(mesh->pnts[53]-mesh->pnts[52]);//52 53	
	vec3 axis42=axis40;	axis42.y=-axis42.y;
	

	vec3 rotcent11=mesh->pnts[95];//88
	vec3 rotcent12=rotcent11;	rotcent12.y=-rotcent12.y;

	vec3 rotcent21=mesh->pnts[40];
	vec3 rotcent22=rotcent21;	rotcent22.y=-rotcent22.y;

	vec3 rotcent31=mesh->pnts[61];
	vec3 rotcent32=rotcent31;	rotcent32.y=-rotcent32.y;

	vec3 rotcent41=mesh->pnts[52];
	vec3 rotcent42=rotcent41;	rotcent42.y=-rotcent42.y;
#else
//f18
//51-52  21
//56-60  31
//55     11
//47-49  41
//lwng   =2        =21 ax0  
//flp    =128/1024 =21 ax0   / 41/ax40
//lifter =4        =11 --
//rudder =8        =31 ax3 
	vec3 rcentwhl1=mesh->pnts[144];//2  elso
	vec3 rcentwhl2=mesh->pnts[158];//
	vec3 rcentwhl22=rcentwhl2;	rcentwhl22.y=-rcentwhl22.y;


	vec3 axis0=normalize(mesh->pnts[51]-mesh->pnts[52]);//nem transformalt kell!
	vec3 axis02=axis0;	axis02.y=-axis02.y;
	vec3 axis3=normalize(mesh->pnts[60]-mesh->pnts[56]);
	vec3 axis32=axis3;	axis32.y=-axis32.y;

	vec3 axis40=normalize(mesh->pnts[47]-mesh->pnts[49]);
	vec3 axis42=axis40;	axis42.y=-axis42.y;
	

	vec3 rotcent11=mesh->pnts[55];
	vec3 rotcent12=rotcent11;	rotcent12.y=-rotcent12.y;

	vec3 rotcent21=mesh->pnts[51];
	vec3 rotcent22=rotcent21;	rotcent22.y=-rotcent22.y;

	vec3 rotcent31=mesh->pnts[56];
	vec3 rotcent32=rotcent31;	rotcent32.y=-rotcent32.y;

	vec3 rotcent41=mesh->pnts[47];
	vec3 rotcent42=rotcent41;	rotcent42.y=-rotcent42.y;

	vec3 rotcent51=mesh->pnts[189];//speed brake
	vec3 rotcent52=rotcent51;	rotcent52.y=-rotcent52.y;
#endif


	axis ax3;
	ax3.x=vec3(0,0,1);//z==x
	ax3.y=vec3(1,0,0);//x==y
	ax3.z=vec3(0,1,0);//y==z 
	ax3=axis_mul(ax3,plane_mat);

	axis4 ax4;
	copyaxis4(ax4,ax3);
	ax4.w=plane_pos;	ax4.w.w=1.0;
	mesh->tramat=ax4;//main trans mat
	mesh->itramat=invmatrix(ax4);
	
int mask=(2|4|8|32|64|128|1024|2048);//0xffff^  |4096
//	mesh->transf4(plane_pos,ax4,0);//plane_mat
	mesh->transf42(plane_pos,ax4,mask,0);//mindent megcsinal            



	axis4 ax5=getrotation2axis4(rotcent21,axis0,wing_ang1);//wing_ang2 test +wing_ang2
	ax5=axis4_mul(ax5,ax4);
	mesh->transf4(plane_pos,ax5,2);

	ax5=getrotation2axis4(rotcent21,axis0,-flapwing*45.0*rad);//35?   real45!
	ax5=axis4_mul(ax5,ax4);
	mesh->transf4(plane_pos,ax5,128);//flaps
	ax5=getrotation2axis4(rotcent41,axis40,-flapwing*45.0*rad);
	ax5=axis4_mul(ax5,ax4);
	mesh->transf4(plane_pos,ax5,1024);//flaps2

	ax5=getrotation2axis4(rotcent11,vec3(0,1,0),clamp(-wing_ang2-wing_ang1/1.0,-40.0*rad,40.0*rad));//-wing_ang1/1.5
	ax5=axis4_mul(ax5,ax4);
	mesh->transf4(plane_pos,ax5,4);

	ax5=getrotation2axis4(rotcent31,axis3,wing_ang3);
	ax5=axis4_mul(ax5,ax4);
	mesh->transf4(plane_pos,ax5,8);

/*origin	int o=2;//elso	if(v11.x<0.0) 	o=0;//hatso	float1 dy=wheeldy[o];	v11.z+=dy;			
//	vec3 rcent=pnts[90];//90 134	if(o!=2) rcent=pnts[134]; */
#if 1
	ax5=getrotation2axis4(rcentwhl1,vec3(0,1,0),geartimer*90.0*rad/3.0);//elso wheel
	ax5=axis4_mul(ax5,ax4);
//	ax5.w.y+=wheeldy[2]; ez itt nem lehet! mert a kerek besullyedesehez is ez kell
	mesh->transf4(plane_pos,ax5,32);

#ifdef _USE_SU35
	ax5=getrotation2axis4(rcentwhl2,vec3(0,1,0),geartimer*90.0*rad/3.0);
#else
	ax5=getrotation2axis4(rcentwhl2,vec3(1,0,0),geartimer*90.0*rad/3.0);
#endif
	ax5=axis4_mul(ax5,ax4);
//	ax5.w.y+=wheeldy[0];
	mesh->transf4(plane_pos,ax5,64);
#endif

	ax5=getrotation2axis4(rotcent51,vec3(0,1,0),-speedbrake*70.0*rad);
	ax5=axis4_mul(ax5,ax4);
	mesh->transf4(plane_pos,ax5,2048);



//mirrored---------------
	mesh->transf42(plane_pos,ax4,mask,8192);//mindent megcsinal             |8192 nem jo! _mir    felulirja a mar meglevoket!

	ax5=getrotation2axis4(rotcent22,axis02,wing_ang1);//test-wing_ang2
	ax5=axis4_mul(ax5,ax4);
	mesh->transf4(plane_pos,ax5,2|8192);

	ax5=getrotation2axis4(rotcent22,axis02,flapwing*45.0*rad);
	ax5=axis4_mul(ax5,ax4);
	mesh->transf4(plane_pos,ax5,128|8192);//flaps
	ax5=getrotation2axis4(rotcent42,axis42,flapwing*45.0*rad);
	ax5=axis4_mul(ax5,ax4);
	mesh->transf4(plane_pos,ax5,1024|8192);//flaps

	ax5=getrotation2axis4(rotcent12,vec3(0,1,0),clamp(-wing_ang2+wing_ang1/1.0,-40.0*rad,40.0*rad));//+wing_ang1/1.5
	ax5=axis4_mul(ax5,ax4);
	mesh->transf4(plane_pos,ax5,4|8192);

	ax5=getrotation2axis4(rotcent32,axis32,wing_ang3);
	ax5=axis4_mul(ax5,ax4);
	mesh->transf4(plane_pos,ax5,8|8192);

/*origin	int o=2;//elso	if(v11.x<0.0) 	o=0;//hatso	float1 dy=wheeldy[o];	v11.z+=dy;			
//	vec3 rcent=pnts[90];//90 134	if(o!=2) rcent=pnts[134]; */
#if 1
	ax5=getrotation2axis4(rcentwhl1,vec3(0,1,0),geartimer*90.0*rad/3.0);
	ax5=axis4_mul(ax5,ax4);
//	ax5.w.y+=wheeldy[2];
	mesh->transf4(plane_pos,ax5,32|8192);

#ifdef _USE_SU35
	ax5=getrotation2axis4(rcentwhl22,vec3(0,1,0),geartimer*90.0*rad/3.0);
#else
	ax5=getrotation2axis4(rcentwhl22,vec3(1,0,0),-geartimer*90.0*rad/3.0);
#endif
	ax5=axis4_mul(ax5,ax4);
//	ax5.w.y+=wheeldy[1];
	mesh->transf4(plane_pos,ax5,64|8192);
#endif

	ax5=getrotation2axis4(rotcent52,vec3(0,1,0),-speedbrake*70.0*rad);
	ax5=axis4_mul(ax5,ax4);
	mesh->transf4(plane_pos,ax5,2048|8192);


	mesh->invalidate_kdtree();//	mesh->update_kdtree();
}

void pretransformB(float1 xtime)
{
QDUMPFNC();
	axis ax3;
	axis4 ax4,ax6;
	ax3.x=vec3(0,0,-1);//z==x  001
	ax3.y=vec3(0,1,0);//x==y  100
	ax3.z=vec3(1,0,0);//y==z  010
	ax3=axis_mul(ax3,plane_mat);

	copyaxis4(ax4,ax3);
	ax4.w=plane_pos;	ax4.w.w=1.0;
	vec3 pivot_pnt,pivot_axis(1,0,0);
	plane_obj.setmat("body",ax4);
	plane_obj.setmat("fulke",ax4);
	plane_obj.setmat("bodywingr",ax4);
	plane_obj.setmat("bodywingl",ax4);
	if(planetype==3)
	{
		plane_obj.setmat("stabilizerwing",ax4);
		plane_obj.setmat("tower",ax4);
		plane_obj.setmat("lowwingl",ax4);
		plane_obj.setmat("lowwingr",ax4);
	}
	else
	{
		plane_obj.setmat("enginel",ax4);
		plane_obj.setmat("enginer",ax4);
		plane_obj.setmat("stabilizerwingr",ax4);
		plane_obj.setmat("stabilizerwingl",ax4); // no pivot bodies
	}
	plane_obj.setmat("wingr",ax4);
	plane_obj.setmat("wingl",ax4);
	
	plane_obj.setmat("axis_wheelbackl",ax4);
	plane_obj.setmat("axis_wheelbackr",ax4);
	plane_obj.setmat("axis_wheelfront",ax4);




	plane_obj.getmesh("body")->phytype=0;
	plane_obj.getmesh("fulke")->phytype=0;
/*	plane_obj.getmesh("bodywingl")->phytype=0; NA
//	plane_obj.getmesh("bodywingl")->phytype=0;*/
	if(planetype!=3)
	{
		plane_obj.getmesh("enginel")->phytype=0;
		plane_obj.getmesh("enginer")->phytype=0;
	}

	
	plane_obj.setrotate("aileronr",wing_ang1,&ax4);
	plane_obj.setrotate("aileronl",-wing_ang1,&ax4);
	plane_obj.setrotate("flapsr",flapwing*45.0*rad,&ax4);
	plane_obj.setrotate("flapsl",flapwing*45.0*rad,&ax4);
	plane_obj.setrotate("frontflapsr",(12.0-flapwing*35.0)*rad,&ax4); // new  45
	plane_obj.setrotate("frontflapsl",(12.0-flapwing*35.0)*rad,&ax4);
	plane_obj.setrotate("backwingr",clamp(wing_ang2-wing_ang1/1.0,-40.0*rad,40.0*rad),&ax4);
	plane_obj.setrotate("backwingl",clamp(wing_ang2+wing_ang1/1.0,-40.0*rad,40.0*rad),&ax4);
	if(planetype==3)
	{
		plane_obj.setrotate("stabilizer",wing_ang3,&ax4);
	}
	else
	{
		plane_obj.setrotate("stabilizerr",wing_ang3,&ax4);
		plane_obj.setrotate("stabilizerl",-wing_ang3,&ax4);
	}

	plane_obj.setrotate("airbrake",speedbrake*70.0*rad,&ax4);

//#ifdef _LOADplane_obj
if(planetype!=99)
{
	plane_obj.setrotate("wheelbackl",geartimer*90.0*rad/3.0,&ax4,&wheeldy[0]);
	plane_obj.setrotate("wheelbackr",geartimer*90.0*rad/3.0,&ax4,&wheeldy[1]);
	plane_obj.setrotate("wheelfront",-geartimer*90.0*rad/3.0,&ax4,&wheeldy[2]);
//	plane_obj.setrotate("wheelfrontl",geartimer*90.0*rad/3.0,&ax4,&wheeldy[2]);
	plane_obj.setrotate("hook",-ehook*60.0*rad,&ax4);

	wheelymin[0]=plane_obj.getmesh("axis_wheelbackl")->vertymin; //wheelymin[0].y-=0.2; // 30cm lejebb
	wheelymin[1]=plane_obj.getmesh("axis_wheelbackr")->vertymin; //wheelymin[1].y-=0.2;
	wheelymin[2]=plane_obj.getmesh("axis_wheelfront")->vertymin; //wheelymin[2].y-=0.2;
	hookend2=plane_obj.getmesh("hook")->vertymin;
//	printf("%e %e %e \n",wheelymin[0].x,wheelymin[0].y,wheelymin[0].z);
}
else
{
	wheelymin[0]=plane_pos; // plane_obj.getmesh("body")->vertymin; wheelymin[0].y-=0.2; // 30cm lejebb
	wheelymin[1]=plane_pos; // plane_obj.getmesh("body")->vertymin; wheelymin[1].y-=0.2;
	wheelymin[2]=plane_pos; // plane_obj.getmesh("body")->vertymin; wheelymin[2].y-=0.2;
	hookend2=plane_pos; // plane_obj.getmesh("body")->vertymin;
}	


	trust_pos[0]=plane_obj.getmesh("pivot_enginel")->getpivotTRA(&ax4);
	trust_pos[1]=plane_obj.getmesh("pivot_enginer")->getpivotTRA(&ax4);
	
	canopy_pos=plane_obj.getmesh("pivot_fulke")->getpivotTRA(&ax4);

	plane_obj.transf(normalize(spd));
}


void draw_forces(int scam)//airplane
{
QDUMPFNC();
	float1 zoom4=1.0,zz=1e3;
	vec3 resol(xx,yy,0);

	qassert(mesh);
/*
already
	glEnable(GL_DEPTH);	glEnable (GL_DEPTH_TEST);

	setshader("shader6");
	shader_setparm3("resol",&resol,2);
	shader_setparm3("zoom",&zoom4,1);
*/


	vec4 col44;

	mesh->vnorm.resize(mesh->pnts_trn.size());
	mesh->color.resize(mesh->pnts_trn.size());

	loopd(i,0,mesh->faces.size(),3)//triangles
	if((i+2)<mesh->faces.size())
	{
		int f1=mesh->faces[i];
		int f2=mesh->faces[i+1];
		int f3=mesh->faces[i+2];

		vec3 v1=mesh->pnts_trn[f1];
		vec3 v2=mesh->pnts_trn[f2];
		vec3 v3=mesh->pnts_trn[f3];

		vec3 norm=normalize(cross(v3-v1 , v2-v1));
		mesh->vnorm[f1]=norm;
		mesh->vnorm[f2]=norm;
		mesh->vnorm[f3]=norm;

		

		vec4 col44;
		float1 g=mesh->forces[0][i/3]/1e4;
		if(g<0.0)	col44=vec4(1.0f+g,1.0f+g,-g,1.0f);//blue-yellow  -
		else		col44=vec4(1.0f,1.0f-g,0.0f,1.0f);//red-yellow  +
		mesh->color[f1]=col44;
		mesh->color[f2]=col44;
		mesh->color[f3]=col44;

		g=mesh->forces[1][i/3]/1e4;
		if(g<0.0)	col44=vec4(1.0f+g,1.0f+g,-g,1.0f);//blue-yellow  -
		else		col44=vec4(1.0f,1.0f-g,0.0f,1.0f);//red-yellow  +
//		mesh->color_mir[f1]=col44;
	}

	
#ifdef _NEWPHY2
//force debug
	setshader("shader10");
	shader_setparm3("mat1",&cam_matb[scam],16);//uniform
	shader_setparm3("eye",&cam_eyeb[scam],3);
	shader_setparm3("resol",&resol,2);
	shader_setparm3("eye",&cam_eyeb[scam],3);
	shader_setparm3("camx",&cam_axs[scam].x,3);//uniform
	shader_setparm3("camy",&cam_axs[scam].y,3);
	shader_setparm3("camz",&cam_axs[scam].z,3);
	glBegin2(GL_LINES);
	loopd(i,0,mesh->faces.size(),3)//triangles
	if((i+2)<mesh->faces.size())
	if(((mesh->types[i]&32)==0 && (mesh->types[i]&64)==0) || gear==1||gearneed==1 )//gear
	if((mesh->types[i]&4096)==0)//nem hook
	{
		int f1=mesh->faces[i];
		int f2=mesh->faces[i+1];
		int f3=mesh->faces[i+2];
		
		vec3 v1=mesh->pnts_trn[f1];
		vec3 v2=mesh->pnts_trn[f2];
		vec3 v3=mesh->pnts_trn[f3];

		if(mesh->types[i]&32) 
		{
			v1+=wheeldy[2];//igy nem jo, nert normal iranyba kellene mennie az utkozo felulethez kepest
			v2+=wheeldy[2];//.y.y.y
			v3+=wheeldy[2];
		}
		if(mesh->types[i]&64) 
		{
			v1+=wheeldy[0];//.y.y.y
			v2+=wheeldy[0];
			v3+=wheeldy[0];
		}
		
		
		float1 g=mesh->forces[0][i/3]/1e4;
		if(g<0.0)	col44=vec4(1.0f+g,1.0f+g,-g,1.0f);//blue-yellow  -
		else		col44=vec4(1.0f,1.0f-g,0.0f,1.0f);//red-yellow  +
		glMultiTexCoord4f(1,col44.x,col44.y,col44.z,col44.w);			

		glVertex3f(v1.x,v1.y,v1.z);glVertex3f(v2.x,v2.y,v2.z);
		glVertex3f(v2.x,v2.y,v2.z);glVertex3f(v3.x,v3.y,v3.z);
		glVertex3f(v3.x,v3.y,v3.z);glVertex3f(v1.x,v1.y,v1.z);





//		v1=mesh->pnts_trn_mir[f1];//mirrored Y

		if(mesh->types[i]&32) 
		{
			v1+=wheeldy[2];//igy nem jo, nert normal iranyba kellene mennie az utkozo felulethez kepest
			v2+=wheeldy[2];//.y.y.y
			v3+=wheeldy[2];
		}
		if(mesh->types[i]&64) 
		{
			v1+=wheeldy[1];//.y.y.y
			v2+=wheeldy[1];
			v3+=wheeldy[1];
		}

		g=mesh->forces[1][i/3]/1e4;
		if(g<0.0)	col44=vec4(1.0f+g,1.0f+g,-g,1.0f);
		else		col44=vec4(1.0f,1.0f-g,0.0f,1.0f);
		glMultiTexCoord4f(1,col44.x,col44.y,col44.z,col44.w);			

		glVertex3f(v1.x,v1.y,v1.z);glVertex3f(v2.x,v2.y,v2.z);
		glVertex3f(v2.x,v2.y,v2.z);glVertex3f(v3.x,v3.y,v3.z);
		glVertex3f(v3.x,v3.y,v3.z);glVertex3f(v1.x,v1.y,v1.z);
	}
	glEnd2();
#else
	setshader("shader7");
	shader_setparm3("mat1",&cam_matb[scam],16);//uniform
	shader_setparm3("eye",&cam_eyeb[scam],3);
	shader_setparm3("resol",&resol,2);
	shader_setparm3("camx",&cam_axs[scam].x,3);//uniform
	shader_setparm3("camy",&cam_axs[scam].y,3);
	shader_setparm3("camz",&cam_axs[scam].z,3);
	
	settx(0,"tx43");
	settx(1,"skytx");

#ifdef _USE_SU35
int q5=38;
#else
int q5=45;
#endif


if(0) // OFF old render   iiddee
	for(int s=0;s<2;s++)	 // s=1 shadow render
	{
	if(s==1) settx(0,"skytx");

	
#if 1
	glBegin2(GL_TRIANGLES);
//	glBegin2(GL_LINES);
//airplane draw main
	loopd(i,0,mesh->faces.size(),3)//triangles
	if((i+2)<mesh->faces.size())
	if(((mesh->types[i]&32)==0 && (mesh->types[i]&64)==0) || gear==1||gearneed==1 )//gear
//	if((mesh->types[i]&32)==0 || gear==1||gearneed==1 )//gear
	if((mesh->types[i]&4096)==0)//nem hook
	{
		int f1=mesh->faces[i];
		int f2=mesh->faces[i+1];
		int f3=mesh->faces[i+2];
		
		vec3 v1=mesh->pnts_trn[f1];
		vec3 v2=mesh->pnts_trn[f2];
		vec3 v3=mesh->pnts_trn[f3];
		
		if(s==1)
		{
			v1.y=0.2;
			v2.y=0.2;
			v3.y=0.2;
		}


		if(mesh->types[i]&32) 
		{
			v1+=wheeldy[2];//igy nem jo, nert normal iranyba kellene mennie az utkozo felulethez kepest
			v2+=wheeldy[2];//.y.y.y
			v3+=wheeldy[2];
		}
		if(mesh->types[i]&64) 
		{
			v1+=wheeldy[0];//.y.y.y
			v2+=wheeldy[0];
			v3+=wheeldy[0];
		}
		
		
/*		float1 g=mesh->forces[0][i/3]/1e4;
		if(g<0.0)	col44=vec4(1.0f+g,1.0f+g,-g,1.0f);//blue-yellow  -
		else		col44=vec4(1.0f,1.0f-g,0.0f,1.0f);//red-yellow  +
		glMultiTexCoord4f(1,col44.x,col44.y,col44.z,col44.w);			
		*/
		col44=mesh->color[f1];
		vec4 col442=mesh->color[f2];
		vec4 col443=mesh->color[f3];
		
		if(s==1)
		{
			col44=vec3(0,0,0);
			col442=vec3(0,0,0);
			col443=vec3(0,0,0);
		}

		if((mesh->types[i]&16)==0)
		{
		glMultiTexCoord2f(0,mesh->txcoord[f1].x,mesh->txcoord[f1].z);		
		glMultiTexCoord4f(1,col44.x,col44.y,col44.z,col44.w);			
		glMultiTexCoord3f(2,mesh->vnorm[f1].x,mesh->vnorm[f1].y,mesh->vnorm[f1].z);		
		glVertex3f(v1.x,v1.y,v1.z);
		glMultiTexCoord2f(0,mesh->txcoord[f2].x,mesh->txcoord[f2].z);		
		glMultiTexCoord4f(1,col442.x,col442.y,col442.z,col442.w);			
		glMultiTexCoord3f(2,mesh->vnorm[f2].x,mesh->vnorm[f2].y,mesh->vnorm[f2].z);		
		glVertex3f(v2.x,v2.y,v2.z);
		glMultiTexCoord2f(0,mesh->txcoord[f3].x,mesh->txcoord[f3].z);		
		glMultiTexCoord4f(1,col443.x,col443.y,col443.z,col443.w);			
		glMultiTexCoord3f(2,mesh->vnorm[f3].x,mesh->vnorm[f3].y,mesh->vnorm[f3].z);		
		glVertex3f(v3.x,v3.y,v3.z);
		}
		else
		{
		glMultiTexCoord2f(0,mesh->txcoord[f1].x,mesh->txcoord[f1].y);		
		glMultiTexCoord4f(1,col44.x,col44.y,col44.z,col44.w);			
		glMultiTexCoord3f(2,mesh->vnorm[f1].x,mesh->vnorm[f1].y,mesh->vnorm[f1].z);		
		glVertex3f(v1.x,v1.y,v1.z);
		glMultiTexCoord2f(0,mesh->txcoord[f2].x,mesh->txcoord[f2].y);		
		glMultiTexCoord4f(1,col442.x,col442.y,col442.z,col442.w);			
		glMultiTexCoord3f(2,mesh->vnorm[f2].x,mesh->vnorm[f2].y,mesh->vnorm[f2].z);		
		glVertex3f(v2.x,v2.y,v2.z);
		glMultiTexCoord2f(0,mesh->txcoord[f3].x,mesh->txcoord[f3].y);		
		glMultiTexCoord4f(1,col443.x,col443.y,col443.z,col443.w);			
		glMultiTexCoord3f(2,mesh->vnorm[f3].x,mesh->vnorm[f3].y,mesh->vnorm[f3].z);		
		glVertex3f(v3.x,v3.y,v3.z);
		}


		if(s==0)
		{
		//38 wing end
		
			if(mesh->types[i]&8192) //mir TODO
			{
				if(f1==q5)	kondenzcsik[n_kondenzcsik][1]=v1;
				if(f2==q5)	kondenzcsik[n_kondenzcsik][1]=v2;
				if(f3==q5)	kondenzcsik[n_kondenzcsik][1]=v3;//38
			}
			else
			{
				if(f1==q5)	kondenzcsik[n_kondenzcsik][0]=v1;
				if(f2==q5)	kondenzcsik[n_kondenzcsik][0]=v2;
				if(f3==q5)	kondenzcsik[n_kondenzcsik][0]=v3;//38
			}
		}




//		v1=mesh->pnts_trn_mir[f1];//mirrored Y

		if(mesh->types[i]&32) 
		{
			v1+=wheeldy[2];//igy nem jo, nert normal iranyba kellene mennie az utkozo felulethez kepest
			v2+=wheeldy[2];//.y.y.y
			v3+=wheeldy[2];
		}
		if(mesh->types[i]&64) 
		{
			v1+=wheeldy[1];//.y.y.y
			v2+=wheeldy[1];
			v3+=wheeldy[1];
		}

/*		g=mesh->forces[1][i/3]/1e4;
		if(g<0.0)	col44=vec4(1.0f+g,1.0f+g,-g,1.0f);//blue-yellow  -
		else		col44=vec4(1.0f,1.0f-g,0.0f,1.0f);//red-yellow  +
		glMultiTexCoord4f(1,col44.x,col44.y,col44.z,col44.w);			*/	
		
//		col44=mesh->color_mir[f1];

#if 0
		int mirb=mesh->pnts_mir_base; // NA  8192!
		int f1m=f1+mirb;
		int f2m=f2+mirb;
		int f3m=f3+mirb;

		if((mesh->types[i]&16)==0)
		{
		glMultiTexCoord2f(0,mesh->txcoord[f1].x,(mesh->txcoord[f1].z));		
		glMultiTexCoord4f(1,col44.x,col44.y,col44.z,col44.w);			
		glMultiTexCoord3f(2,mesh->vnorm[f1m].x,mesh->vnorm[f1m].y,mesh->vnorm[f1m].z);		
		glVertex3f(v1.x,v1.y,v1.z);
		glMultiTexCoord2f(0,mesh->txcoord[f2].x,(mesh->txcoord[f2].z));		
		glMultiTexCoord4f(1,col442.x,col442.y,col442.z,col442.w);			
		glMultiTexCoord3f(2,mesh->vnorm[f2m].x,mesh->vnorm[f2m].y,mesh->vnorm[f2m].z);		
		glVertex3f(v2.x,v2.y,v2.z);
		glMultiTexCoord2f(0,mesh->txcoord[f3].x,(mesh->txcoord[f3].z));		
		glMultiTexCoord4f(1,col443.x,col443.y,col443.z,col443.w);			
		glMultiTexCoord3f(2,mesh->vnorm[f3m].x,mesh->vnorm[f3m].y,mesh->vnorm[f3m].z);		
		glVertex3f(v3.x,v3.y,v3.z);
		}
		else
		{
		glMultiTexCoord2f(0,mesh->txcoord[f1].x,(mesh->txcoord[f1].y));		
		glMultiTexCoord4f(1,col44.x,col44.y,col44.z,col44.w);			
		glMultiTexCoord3f(2,mesh->vnorm[f1m].x,mesh->vnorm[f1m].y,mesh->vnorm[f1m].z);		
		glVertex3f(v1.x,v1.y,v1.z);
		glMultiTexCoord2f(0,mesh->txcoord[f2].x,(mesh->txcoord[f2].y));		
		glMultiTexCoord4f(1,col442.x,col442.y,col442.z,col442.w);			
		glMultiTexCoord3f(2,mesh->vnorm[f2m].x,mesh->vnorm[f2m].y,mesh->vnorm[f2m].z);		
		glVertex3f(v2.x,v2.y,v2.z);
		glMultiTexCoord2f(0,mesh->txcoord[f3].x,(mesh->txcoord[f3].y));		
		glMultiTexCoord4f(1,col443.x,col443.y,col443.z,col443.w);			
		glMultiTexCoord3f(2,mesh->vnorm[f3m].x,mesh->vnorm[f3m].y,mesh->vnorm[f3m ].z);		
		glVertex3f(v3.x,v3.y,v3.z);
		}
#endif

		if(s==0)
		{
		if(f1==q5)	kondenzcsik[n_kondenzcsik][1]=v1;
		if(f2==q5)	kondenzcsik[n_kondenzcsik][1]=v2;
		if(f3==q5)	kondenzcsik[n_kondenzcsik][1]=v3;
		}
	}
	glEnd2();
#endif
#endif
	}





	{
//?	pretransformB(0.1); // mar ready


#if 0
// TODO
	plane_obj.getpivot("pivot_airbrake",&pivot_axis,&pivot_pnt);
	ax5=getrotation2axis4(pivot_pnt,pivot_axis,-speedbrake*70.0*rad);
	ax5=axis4_mul(ax5,ax4);
	plane_obj.setmat("airbrake",ax5);
#endif


	vec3 resol2(xx,yy,0);
	setshader("shader4b");//9?  raytrace reflect

	shader_setparm3("mat1",&cam_matb[scam],16);//uniform
	shader_setparm3("resol",&resol2,2);
	shader_setparm3("gforcecolor",&gforcecolor,2);
	shader_setparm3("ratio",&ratio,1);//tx wide in meter
	shader_setparm3("eye",&cam_eyeb[scam],3);
	shader_setparm3("camx",&cam_axs[scam].x,3);//uniform	
	shader_setparm3("camy",&cam_axs[scam].y,3);	
	shader_setparm3("camz",&cam_axs[scam].z,3);
	shader_setparm3("nightvalue",&nightvalue,1);
	

#ifdef _GMAP	
	settx(0,"tx1");
	settx(1,"tx2");
#else
	settx(0,"tx40");
	settx(1,"tx41");
#endif	
	settx(2,"tx3");
	settx(3,"tx4");
	settx(4,"tx5");
	settx(5,"tx6");
	settx(6,"tx7");
	settx(7,"tx8");
	settx(8,"tx9");
	settx(9,"tx10");

	if(planetype==1) settx(10,"f18objtx");
	if(planetype==2) settx(10,"su35objtx");
	if(planetype==3) settx(10,"f16objtx");
	settx(11,"skytx");

//glEnable (GL_BLEND);glBlendFunc (GL_ONE,GL_ONE);
	plane_obj.draw(plane_pos,gear); 
//glDisable (GL_BLEND);

	}

#if 0
	glBegin2(GL_LINES);
	for(auto &mesh:plane_obj.meshs)	
	if(strstr(mesh.name.c_str(),"pivot_") ) 
	{
		vec3 v1=mesh.pivot_pos-mesh.pivot_ax*3.0;
		vec3 v2=mesh.pivot_pos+mesh.pivot_ax*3.0;
		
		vec4 v1b=ax4.x*v1.x+ax4.y*v1.y+ax4.z*v1.z + ax4.w;
		vec4 v2b=ax4.x*v2.x+ax4.y*v2.y+ax4.z*v2.z + ax4.w;//
		
	// pivots
		setshader("shader10");
		shader_setparm3("mat1",&cam_matb[scam],16);//uniform
		shader_setparm3("eye",&cam_eyeb[scam],3);
		shader_setparm3("resol",&resol,2);
		shader_setparm3("eye",&cam_eyeb[scam],3);
		shader_setparm3("camx",&cam_axs[scam].x,3);//uniform
		shader_setparm3("camy",&cam_axs[scam].y,3);
		shader_setparm3("camz",&cam_axs[scam].z,3);
		glMultiTexCoord4f(1,1.0,0.0,0.0,1.0);			

		glVertex3f(v1b.x,v1b.y,v1b.z);glVertex3f(v2b.x,v2b.y,v2b.z);
	}
	glEnd2();
#endif





//#ifdef _NEWPHY2
#ifdef _DEBUGVECTOR

	debverts_old.clear();
	loopd(i,0,debverts.size(),1) debverts_old.push_back(debverts[i]);

	glDisable (GL_DEPTH_TEST);

	setshader("shader10");
	shader_setparm3("mat1",&cam_matb[scam],16);//uniform
	shader_setparm3("eye",&cam_eyeb[scam],3);
	shader_setparm3("resol",&resol,2);
	shader_setparm3("eye",&cam_eyeb[scam],3);
	shader_setparm3("camx",&cam_axs[scam].x,3);//uniform
	shader_setparm3("camy",&cam_axs[scam].y,3);
	shader_setparm3("camz",&cam_axs[scam].z,3);
	shader_setparm3("nightvalue",&nightvalue,1);
	glBegin2(GL_LINES);
///	glBegin2(GL_TRIANGLES);
	loopd(i,0,debverts_old.size(),2)//2
	{
		vec3 v1=debverts_old[i];//+plane_pos;
//		vec3 v2=v1+debverts_old[i+1]*3e-4;
		vec3 v2=debverts_old[i+1];
		vec3 v3;
//normal
		if((i>>1)&1) glMultiTexCoord4f(1,0.9,0.2,0.9,1.0);			
		else glMultiTexCoord4f(1,0.0,0.5,0.0,1.0);			
//		glMultiTexCoord2f(0,0.0f,1.0f);		glVertex3f( v1.x,v1.y,v1.z);
	//	glMultiTexCoord2f(0,0.0f,1.0f);		glVertex3f(	v2.x,v2.y,v2.z);
//normal Force
		//glMultiTexCoord4f(1,0.0,0.9,0.9,1.0);			
		glMultiTexCoord2f(0,0.0f,1.0f);		glVertex3f( v1.x,v1.y,v1.z);
		glMultiTexCoord2f(0,0.0f,1.0f);		glVertex3f(	v2.x,v2.y,v2.z);
	//	glMultiTexCoord2f(0,0.0f,1.0f);		glVertex3f(	v2.x,v2.y,v2.z);
//vel
#if 0
		v2=v1+debverts[i+6]*1e-2;
		glMultiTexCoord4f(1,0.0,0.9,0.9,1.0);			
		glMultiTexCoord2f(0,0.0f,1.0f);		glVertex3f( v1.x,v1.y,v1.z);
		glMultiTexCoord2f(0,0.0f,1.0f);		glVertex3f(	v2.x,v2.y,v2.z);
#endif
//tri
#if 0
		int r,g,b;
		rainbow4(r,g,b,(int)length(debverts[i+5]*1e-4*100.0),100.0,
			125,0,0,
			255,255,0,
			0,150,0,
			0,250,250,
			0,0,150);
			
		glMultiTexCoord4f(1,r/255.0,g/255.0,b/255.0,1.0);			
	
		v1=debverts[i+2]+plane_pos;
		v2=debverts[i+3]+plane_pos;
		v3=debverts[i+4]+plane_pos;
#if 1
		glMultiTexCoord2f(0,0.0f,1.0f);		glVertex3f( v1.x,v1.y,v1.z);
		glMultiTexCoord2f(0,0.0f,1.0f);		glVertex3f(	v2.x,v2.y,v2.z);
		glMultiTexCoord2f(0,0.0f,1.0f);		glVertex3f( v3.x,v3.y,v3.z);
		glMultiTexCoord2f(0,0.0f,1.0f);		glVertex3f(	v2.x,v2.y,v2.z);
		glMultiTexCoord2f(0,0.0f,1.0f);		glVertex3f( v1.x,v1.y,v1.z);
		glMultiTexCoord2f(0,0.0f,1.0f);		glVertex3f(	v3.x,v3.y,v3.z);
#else
		glMultiTexCoord2f(0,0.0f,1.0f);		glVertex3f( v1.x,v1.y,v1.z);
		glMultiTexCoord2f(0,0.0f,1.0f);		glVertex3f(	v2.x,v2.y,v2.z);
		glMultiTexCoord2f(0,0.0f,1.0f);		glVertex3f( v3.x,v3.y,v3.z);
#endif		
#endif		
	}
	glEnd2();
	glEnable (GL_DEPTH_TEST);
#endif



	kondcc++;
//	if(length(spd)>80.0)//100 m/s
	if(kondcc>4)//10
	{
		kondcc=0;
		partcsize[n_kondenzcsik]=1.0;//meter
		partspd[n_kondenzcsik]=spd;
		n_kondenzcsik++;
		n_kondenzcsik&=127;//511
	}

	







//particles
#if 0
	setshader("shader8");
	shader_setparm3("mat1",&cam_matb[scam],16);//uniform
	shader_setparm3("eye",&cam_eyeb[scam],3);
	shader_setparm3("resol",&resol,2);
	shader_setparm3("eye",&cam_eyeb[scam],3);
	shader_setparm3("camx",&cam_axs[scam].x,3);//uniform
	shader_setparm3("camy",&cam_axs[scam].y,3);
	shader_setparm3("camz",&cam_axs[scam].z,3);

	vec3 w2;
	glBegin2(GL_LINES);
	glMultiTexCoord4f(1,0.7,0.9,1.0,1.0);			
	loopd(i,0,511,1)
	{
		vec3 &w1=kondenzcsik[i][0];
	
		if(i!=n_kondenzcsik+1)
		if(i)
		{
			vec3 dx=(w2-w1)*0.1;
			glVertex3f(w1.x,w1.y,w1.z);
			glVertex3f(w2.x-dx.x,w2.y-dx.y,w2.z-dx.z);
		}
		w2=w1;
	}
	loopd(i,0,511,1)
	{
		vec3 &w1=kondenzcsik[i][1];
		
		if(i!=n_kondenzcsik+1)
		if(i)
		{
			vec3 dx=(w2-w1)*0.1;
			glVertex3f(w1.x,w1.y,w1.z);
			glVertex3f(w2.x-dx.x,w2.y-dx.y,w2.z-dx.z);
		}
		w2=w1;
	}
	glEnd2();


#else

	glEnable (GL_BLEND);
//	glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
	glBlendFunc (GL_ONE,GL_ONE);
//	glBlendFunc (GL_ONE,GL_ONE_MINUS_SRC_COLOR );
	glBlendFunc (GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR );

	setshader("shader8");
	shader_setparm3("mat1",&cam_matb[scam],16);//uniform
	shader_setparm3("eye",&cam_eyeb[scam],3);
	shader_setparm3("resol",&resol,2);
	shader_setparm3("eye",&cam_eyeb[scam],3);
	shader_setparm3("camx",&cam_axs[scam].x,3);//uniform
	shader_setparm3("camy",&cam_axs[scam].y,3);
	shader_setparm3("camz",&cam_axs[scam].z,3);

	settx(0,"tx44");

	vec3 camx=cam_axs[scam].x;
	vec3 camy=cam_axs[scam].y;
#if 1

	vec3 w2;
	glBegin2(GL_QUADS);
	glMultiTexCoord4f(1,0.7,0.9,1.0,1.0);			
	loopd(j,0,2,1)
	loopd(i,0,512,1)
	{
		vec3 &w1=kondenzcsik[i][j];
		float1 xb=0.25*(i/4);
		float1 yb=0.25*(i%4);
		yb=fmod(yb,1.0);
	
		if(i!=n_kondenzcsik+1)
		if(i!=n_kondenzcsik)
		if(i)
		{
//			vec3 dx=(w2-w1)*0.1;
			float1 dx=length(w1-w2);
			
			float1 h=partcsize[i]/2.0;
	vec3 v1=w1-camy*h;	vec3 v2=w1+camy*h;	vec3 v3=w2+camy*h;	vec3 v4=w2-camy*h;
//	vec3 v1=w1-camx*h-camy*h;	vec3 v2=w1+camx*h-camy*h;	vec3 v3=w1+camx*h+camy*h;	vec3 v4=w1-camx*h+camy*h;

			glMultiTexCoord2f(0,0.0 ,yb+0.0 );	glVertex3fv(&v1.x);
			glMultiTexCoord2f(0,0.0 ,yb+0.25);	glVertex3fv(&v2.x);
			glMultiTexCoord2f(0,1.0 ,yb+0.25);	glVertex3fv(&v3.x);
			glMultiTexCoord2f(0,1.0 ,yb+0.0 );	glVertex3fv(&v4.x);
		}
		w2=w1;

	}
	glEnd2();
	glDisable (GL_BLEND);
#endif
#endif


	
	
	
//trust	  lang afterburner
#if 1
#if 1
	glEnable (GL_BLEND);
	glBlendFunc (GL_ONE,GL_ONE);

	glMultiTexCoord4f(1,1.0,1.0,0.0,1.0);			
	settx(0,"tx45");



	vec3 w1=mesh->pnts_trn[133];//	w1=qtransform(w1);	w1+=plane_pos; 93
	w1=trust_pos[0];// new

	glBegin2(GL_QUADS);
		loopd(j,0,32,1)
		loopd(i,0,16,1) 
		{
			deftrustquad((float1)(i  )/16.0,(float1)(j  )/32.0,w1,trust_vec[0]);
			deftrustquad((float1)(i+1)/16.0,(float1)(j  )/32.0,w1,trust_vec[0]);
			deftrustquad((float1)(i+1)/16.0,(float1)(j+1)/32.0,w1,trust_vec[0]);
			deftrustquad((float1)(i  )/16.0,(float1)(j+1)/32.0,w1,trust_vec[0]);
		}
//		w1=mesh->pnts[93]; w1.y=-w1.y;		w1=qtransform(w1);		w1+=plane_pos;
		w1=mesh->pnts_trn[133+mesh->pnts_mir_base];
		w1=trust_pos[1];
		
		loopd(j,0,32,1)
		loopd(i,0,16,1) 
		{
			deftrustquad((float1)(i  )/16.0,(float1)(j  )/32.0,w1,trust_vec[1]);
			deftrustquad((float1)(i+1)/16.0,(float1)(j  )/32.0,w1,trust_vec[1]);
			deftrustquad((float1)(i+1)/16.0,(float1)(j+1)/32.0,w1,trust_vec[1]);
			deftrustquad((float1)(i  )/16.0,(float1)(j+1)/32.0,w1,trust_vec[1]);
		}
		if(verticalmode)
		{
		w1=trust_pos[2];
		
		loopd(j,0,32,1)
		loopd(i,0,16,1) 
		{
			deftrustquad((float1)(i  )/16.0,(float1)(j  )/32.0,w1,trust_vec[2]);
			deftrustquad((float1)(i+1)/16.0,(float1)(j  )/32.0,w1,trust_vec[2]);
			deftrustquad((float1)(i+1)/16.0,(float1)(j+1)/32.0,w1,trust_vec[2]);
			deftrustquad((float1)(i  )/16.0,(float1)(j+1)/32.0,w1,trust_vec[2]);
		}
//		w1=mesh->pnts[93]; w1.y=-w1.y;		w1=qtransform(w1);		w1+=plane_pos;
		w1=mesh->pnts_trn[133+mesh->pnts_mir_base];
		w1=trust_pos[3];
		
		loopd(j,0,32,1)
		loopd(i,0,16,1) 
		{
			deftrustquad((float1)(i  )/16.0,(float1)(j  )/32.0,w1,trust_vec[3]);
			deftrustquad((float1)(i+1)/16.0,(float1)(j  )/32.0,w1,trust_vec[3]);
			deftrustquad((float1)(i+1)/16.0,(float1)(j+1)/32.0,w1,trust_vec[3]);
			deftrustquad((float1)(i  )/16.0,(float1)(j+1)/32.0,w1,trust_vec[3]);
		}
		}
	glEnd2();
#endif	
#else
	glBegin2(GL_LINES);
	{
	glMultiTexCoord4f(1,0.9,0.3,0.0,1.0);			
	vec3 w1=mesh->pnts[93];
	w1=qtransform(w1);
	w1+=plane_pos;
	glVertex3f(w1.x,w1.y,w1.z);
	w1+=trust_vec[0]*9.0;
	glVertex3f(w1.x,w1.y,w1.z);

	w1=mesh->pnts[93];	w1.y=-w1.y;
	w1=qtransform(w1);
	w1+=plane_pos;
	glVertex3f(w1.x,w1.y,w1.z);
	w1+=trust_vec[1]*9.0;
	glVertex3f(w1.x,w1.y,w1.z);
	}
	glEnd2();
#endif	


	glDisable (GL_BLEND);
}


void update(float1 xtime)
{
QDUMPFNC();
	qassert(mesh);

	
//enable_physics=1;//TODO
//	if(enable_physics==0) return;
	
	loopd(i,0,512/4,1)//512
		{		
		partcsize[i]+=2.1*xtime;//2
		if(partcsize[i]>2.5) partcsize[i]=2.5;//1.5
//		kondenzcsik[i][0]+=partspd[i]*xtime;		kondenzcsik[i][1]+=partspd[i]*xtime;		partspd[i]-=1.0*xtime;
		}


//test
#if 0
	float1 we=getC(cos((90.0-0.0)*rad));	printf("0 %e \n",we);
	 we=getC(cos((90.0-10.0)*rad));	printf("10 %e \n",we);
	 we=getC(cos((90.0-15.0)*rad));	printf("15 %e \n",we);
	 we=getC(cos((90.0-20.0)*rad));	printf("20 %e \n",we);
	 we=getC(cos((90.0-25.0)*rad));	printf("25 %e \n",we);
	 we=getC(cos((90.0-45.0)*rad));	printf("45 %e \n",we);
	 we=getC(cos((90.0-65.0)*rad));	printf("65 %e \n",we);
	 we=getC(cos((90.0-75.0)*rad));	printf("75 %e \n",we);
	 we=getC(cos((90.0-90.0)*rad));	printf("90 %e \n",we);
#endif



//bolintas
	vec3 w1(0,1,0);
	w1-=plane_mat.x*dot(w1,plane_mat.x);//up yz comp
	w1=normalize(w1);
	alf1=atan2(dot(w1,plane_mat.z),dot(w1,plane_mat.y));
	alf1b=atan2(dot(w1,spd),dot(w1,plane_mat.y));
	
	
//doles
	vec3 w2(0,1,0);
	w2-=plane_mat.z*dot(w1,plane_mat.z);//up xy componens
	vec_dol=length(w2);
	w2=normalize(w2);
	alf2=atan2(dot(w2,plane_mat.x),(dot(w2,plane_mat.y)));//fejjel bug    fabs
//	alf1/=rad;	alf2/=rad;
//printf("%f %f \n",alf1,alf2);

//heading
	w2=plane_mat.z;
	w2.y=0;
	w2=normalize(w2);
	float1 dir=atan2(w2.x,w2.z)/rad+180.0;  // -x
	dir=chkdir2(dir);
	gdir=dir;
	
//heading2
	axis ax=getheadaxis(0);
	w2=ax.z;//plane_mat.z;
	w2.y=0;
	w2=normalize(w2);
	float1 dir2=atan2(w2.x,w2.z)/rad+180.0;  // -x
	gdir2=chkdir2(dir2);
	



#if 0	
	xtimes[nxtimes]=xtime;
	nxtimes++; if(nxtimes>9) nxtimes=0;
	
	float1 xtime2=0.0;
	loopd(i,0,20,1) xtime2+=xtimes[i];
	xtime2/=20.0;
printf("%e %e\n",xtime2,1.0/xtime2);//30fps
#endif

//physics
//#ifdef _USE_SU35
//_LOADplane_obj
	float1 mass=1,enginepower=0;
	if(planetype==1)// f18                 
	{
//		mass=13.5e3;//10-16
		mass=10.0e3+fuel;//10-16
	//mass=10.0e3;//10-16	 empty
	//	enginepower=80e3;//90? x2  origin
		enginepower=98e3;//x2
	//enginepower*=2;//cheat NA
	// max spd = 1915 km/h = 1034 knts
	}
	if(planetype==2)// su35
	{
		mass=20e3+fuel;// f18=10 su35=20 27 tonna
		enginepower=142e3;//trust   86+afterburner
		// max spd = 2778 km/h =1500 knts
	}
	if(planetype==3) // f16
	{
		mass=10e3+fuel;// 10-16 t
		enginepower=130e3/2.0;//trust   23-29p  102-130 kN               csak 1 hajtomu, de kette osztom
		// max spd = 2175 km/h =1350 knts
	}
	
#ifdef _INSTANTDIE
	if(!alive) enginepower=0;//down
#endif
	if(fuel<=0.0) enginepower=0;//down

	

//	vec3 I,half_size=vec3(5,5,17)/2.0;//real size=12,5,17  but slim  oldal,fel,elore 5~8
//	vec3 I,half_size=vec3(7,6,21)/2.0;	//su35
//	vec3 I,half_size=vec3(14,4,22)/2.0;	//new


//#ifdef _USE_SU35
// _LOADplane_obj
//	vec3 I,half_size=vec3(7,4,22)/2.0;	//new OKKEE! sy35    /?/  old
	vec3 I,half_size=vec3(5,4,7)/2.0/2; // 2   su35

//vec3 I,half_size=vec3(2.4,9.0,2.4)/2.0;	 //2.4 9  f18  ?  nem rossz  ???
	if(planetype==1) half_size=vec3(4,3,5)/2.0;//test   5.2.7    f18
	if(planetype==3) half_size=vec3(4,3,5)/2.0; // f16
//vec3 I,half_size=vec3(8,8,8)/2.0;//test   5.2.7        iiddee     nagy konnyebben fordul, de lazabb

	I.x=half_size.y*half_size.y + half_size.z*half_size.z;
	I.y=half_size.z*half_size.z + half_size.x*half_size.x;
	I.z=half_size.x*half_size.x + half_size.y*half_size.y;	
	I/=2.0;	I*=mass;
	
	float1 w=0.3*0.3*14.6;//slug ft^2
//printf("I.xyz %e %e %e km m2\n",I.x,I.y,I.z);printf("I.xyz %e %e %e slug ft2\n",I.x/w,I.y/w,I.z/w);

//1lbF=4.45N   1ft (foot)=0.3m    =12in=12*25.6mm=30.7cm               ?1000p=1kp=9.8N  pond?
//1slug=14.6kg       a=1ft/s2  F=1p  m=F/a=1p/1ft/s2=4.45/0.3=14.6kg OK
//slugft2   roll=16.1e3  pitch=119.3e3  yaw=131.5e3
	//17 12 4.7 f18  m~15000       


	if(autopilot_on)
	{
//		if(trust<0.3) trust=0.3;//30 % 50
#ifdef _SHOOT_TEST
		if(trust<0.4) trust=0.4;//70 %
#else		
		if(trust<0.8) trust=0.8;
#endif

		if(mbut[1])
		{
			vec3 ray=
				cam_axs[selcam].z*cam_zoom[selcam] - 
				cam_axs[selcam].y*(my-yy22) 	+ 
				cam_axs[selcam].x*(mx-xx22);//hova nez? cam2!
			float1 t1=(-cam_eyeb[selcam].y)/ray.y;
		
			camera_trg=cam_eyeb[selcam]+ ray*t1;
			
//			printf("trg: %e %e %e \n",camera_trg.x,camera_trg.y,camera_trg.z);
		}
	}



	xtime/=10.0;
	if(physics)
	loopd(j,0,10,1)//--------------------------------------
	{


//physics
	F_all=vec3(0,0,0);
	M_all=vec3(0,0,0);
	vec3 F;
	
	float1 bb=(10.0+0.0+25.0*sin(gametime*M_PI/10.0))*rad;//180
//	wind=vec3(sin(bb),0.0,cos(bb))*(15.0+3.0*sin(gametime*M_PI/5.0)+2.0*sin(gametime*M_PI/2.0))*knots/3.6;//10 5 4
	wind=vec3(sin(bb),0.0f,cos(bb))*(7.0+2.0*sin(gametime*M_PI/5.0)+2.0*sin(gametime*M_PI/2.0))*knots/3.6;//10 5 4
if(ewind==0) wind=vec3(0,0,0);

	float1 x,y,z;
//div I
	x=dot(L,plane_mat.x)/I.x;
	y=dot(L,plane_mat.y)/I.y;
	z=dot(L,plane_mat.z)/I.z;
	omega=plane_mat.x*x + plane_mat.y*y + plane_mat.z*z;




	
//mouse control	
	float1 A,velx,vel,		c4,s4,c1a,s1a,c1b,s1b,c2,s2,c3,s3,		t1;
	
	
	//NA !
	Cdrag=0.5,// 0.2-1.29     flat=1.3 ball~0.5 wing~0.1   
	Clift=1.75;//szogfuggo
	vec3 normal,R;

	qlim=cos((90.0-15.0)*rad);//lim angle of attack aoa  15-20
	qlim2=cos((90.0-20.0)*rad);
	qlim3=cos((15.0)*rad);//new newphy
//printf("%e %e \n",qlim,qlim2);	qlim<qlim2


	velx=dot(spd,plane_mat.z);//+
	velx=length(spd);
	float1 aoa3=0;
	float1 aoa=0;
	aoa=getaoa(aoa3);//!!!!!!11~~~~~~~~~~~~~~~~~~~~~``
	wing2=aoa/rad;

	float1 q5=saturate((velx-100.0)/200.0);
	float1 angspd=40.0- 35.0*q5*q5;//300
angspd=35.0;//angspd=40.0;//old  55????????? 35?? 25
//21 mert szog!!!!!!!1

	int mx3=mx,my3=my;
	if(mbut[3])
	{
		mx3=mx32;my3=my32;
	}
	else
	{
		mx32=mx3;my32=my3;
	}
	cstick1.x=mx3;
	cstick1.y=my3;
	cstick2.x=xx22;
	cstick2.y=yy22;
	
	wing_ang22= fsqrabs((float1)(my3-yy22)/(float1)yy22)*angspd*rad;//70?  55 21   test=7
	float1 wing_ang11= fsqrabs((float1)(mx3-xx22)/(float1)xx22)*35.0*rad;//55 35?  25
	float1 wing_ang33= sidewing*40.0*rad;//	float1 wing_ang3= 0.0; 55 sok
//wing_ang33 += (float1)(mx3-xx22)*2.0*rad/xx22*sin(alf2);	//?????? rasegites

	
	if(autopilot_on)
//	if(this==selairplane) // TODO test!!
	{
//new AI       checkpoint
	int enem=0,AItype=0;
	if(this==selairplane) enem=selenemy;//user
#ifndef _SHOOT_TEST	
	else if(plane_pos.y<5000.0) AItype=1;
#else	
	else if(plane_pos.y<335000.0) AItype=1;//test
#endif

	
	if(AItype==0)
	{
	vec3 enemypos=airplanelist[enem]->plane_pos;// user [0]!           test [1]
	vec3 enemyspd=airplanelist[enem]->spd;

	//korbe integralja a sebessegtol az enemy sebesseg vektoraig
	vec3 pos5=plane_pos;
#if 0
NA!	
	for(int i=0;i<100;i++)// dt=0.01 1000step = 10sec     de csak az eleje (1s) kell!
	{
		float1 t1=(float1)i/1000.0;
		vec3 spd5=lerp(spd,enemyspd,t1);
		pos5+=spd5*0.01;
	}
#else	
pos5=enemypos; // OK!
#endif	
	vec3 dv=pos5-plane_pos;

	vec3 dvYZ=dv-plane_mat.x*dot(dv,plane_mat.x);//oldal kivonva = emelkedes
	dvYZ=normalize(dvYZ);
	float1 w=dot(dvYZ,plane_mat.y);
//	if(dot(dvYZ,plane_mat.z)<0.0) w=-w;
	wing_ang22=(uuu+w*15.0)*rad;// magassag my

	vec3 dvXY=dv-plane_mat.z*dot(dv,plane_mat.z);// mozgasirany kivonva = doles
	dvXY=normalize(dvXY);
	w=dot(dvXY,plane_mat.x);
	if(dot(dvXY,plane_mat.y)<0.0) w=-w;
	if(dot(dvYZ,plane_mat.y)<0.0) w=-w;//!!!!! mindig huzza a botkormanyt!
	wing_ang11=15.0*rad*w;//jobb bal mx

	}
//	printf("%f \n",wing_ang22/rad);//  5  / v=~350
	
	
	
//old
	if(this!=selairplane)
	if(AItype==1)
	{
//old AI   high only	
		float1 limdy=30.0;
		float1 dy=(plane_pos.y-trg_hg)*0.03;//feet bnem
		if(dy<-limdy) dy=-limdy;
		if(dy> limdy) dy= limdy;
		
		ap_alf1=dy;

		float1 w=(alf1*0.2+aoa3+ap_alf1*rad)/(20.0*rad);//10
		//if(dy>0.0) 	
		if(plane_mat.y.y<0.0) w=-w;//ha fejjel
		if(w<-1.0) w=-1.0;
		if(w> 1.0) w=1.0;
#ifdef _INSTANTDIE
if(!alive)	w=-1.0;
#endif
		wing_ang22=-5.0*rad*w;
wing_ang22=-15.0*rad*w;


				

		limdy=50.0;
		int dy2=(trg_dir-gdir)*2.0;		//HDG
		if(dy2<-limdy) dy2=-limdy;
		if(dy2> limdy) dy2= limdy;
		ap_alf2=dy2;
	
		w=(alf2+ap_alf2*rad)/(20.0*rad);//10
		if(dy>0.0) 	
//			if(plane_mat.z.y>0.0)
			if(plane_mat.y.y>-0.1)	w+=3.0*rad;//fejre all    //lefele nem mehet kozvetlenul    !lefele
			
		if(w<-1.0) w=-1.0;
		if(w> 1.0) w=1.0;
#ifdef _INSTANTDIE
if(!alive)	w=-1.0;		
#endif
		wing_ang11=10.0*rad*w;
wing_ang11=20.0*rad*w;
	}
	}

	
	
#if 0
	float1 wing_ang1= 0.0;
	float1 wing_ang3= (float1)(mx-xx22)*55.0*rad/xx22;
#endif	
	if(ejoy)
	if(isjoy)
	{
/*		static float joyy0e=0.0;
		static float joyx0e=0.0;
		joyy0e+=(float1)joyy0*1e-2;
		joyx0e+=(float1)joyx0*1e-2;
		joyy0e=clamp(joyy0e,-1000.0,1000.0);
		joyx0e=clamp(joyx0e,-1000.0,1000.0);*/
		
		float1 ty=(float1)joyy0/1000.0; //*fsgn(joyy0);// ty*=fabs(ty);
		float1 tx=(float1)joyx0/1000.0; //*fsgn(joyx0); // tx*=fabs(tx);
#if 0
// wing NA
		wing_ang22= (ty)*35.0*rad;//70?  64    25    /256
		wing_ang11= (tx)*35.0*rad;//32
		wing_ang33= sidewing*40.0*rad;//	float1 wing_ang3= 0.0;
#endif
//power only
		trust -=ty*xtime/2.0; // 2sec max
		if(length(spd)>50.0*knots/3.6)			{if(trust<0.1) trust=0.1;} // saturate        10% min
		else {if(trust<0.0) trust=0.0;}
		if(trust>1.0) trust=1.0;


	}
//	wing_ang22+=trimm*rad;//printf("%e \n",trimm);	

	if(etrim)
	{
	//auto trim   new!
//x axis	
		vec3 spdnor=normalize(spd);// V1.0
//		vec3 spdnor=normalize(plane_mat.z);//V2.0
		spdnor+=normalize(plane_mat.z);		spdnor=normalize(spdnor);//V3.0

//		vec3 spdnor2=normalize(trim_dir);
		vec3 spdnor2=normalize(vec3(0,1,0));
		spdnor-=plane_mat.x*dot(spdnor,plane_mat.x);//up yz comp
		spdnor2-=plane_mat.x*dot(spdnor2,plane_mat.x);
		spdnor=normalize(spdnor);
		spdnor2=normalize(spdnor2);
//		vec3 d_spdnor=(spdnor-spdnorold)/xtime;

		float1 w=((dot(spdnor,spdnor2)-sin(wing_ang22+10.0*rad)))/sin(30.0*rad); ///xtime; 5
	//	float1 w=acos(fabs(dot(spdnor,spdnor2)))/(20.0*rad); ///xtime;
//		vec3 nor=cross(spdnor2,plane_mat.y);		nor=cross(nor,spdnor2);		if(dot(nor,spdnor)<0.0) w=-w;		if(dot(nor,spdnor)<0.0) w=-w;
//		spdnorold=spdnor;

		if(w<-1.0) w=-1.0;		
		if(w> 1.0) w=1.0;
//		w=sqrt(fabs(w))*fsgn(w);
//		trimm_x+=fsgn(w)*0.1;//*xtime; 0001
		trimm_x+=(w-trimm_x)*xtime*5.1;

		if(trimm_x<-1.0) trimm_x=-1.0;		
		if(trimm_x> 1.0) trimm_x=1.0;

//		wing_ang22+=-30.0*rad*trimm_x;
		wing_ang22=-30.0*rad*trimm_x;


// z axis
		spdnor=normalize(plane_mat.x);
		spdnor2=normalize(vec3(0,1,0));
		spdnor-=plane_mat.z*dot(spdnor,plane_mat.z);
		spdnor2-=plane_mat.z*dot(spdnor2,plane_mat.z);

		w=((dot(spdnor,spdnor2)+sin(2*wing_ang11)))/sin(30.0*rad); ///xtime;

		if(w<-1.0) w=-1.0;		
		if(w> 1.0) w=1.0;
//		w=sqrt(fabs(w))*fsgn(w);
		trimm_z+=(w-trimm_z)*xtime*5.1;

		if(trimm_z<-1.0) trimm_z=-1.0;		
		if(trimm_z> 1.0) trimm_z=1.0;

//		wing_ang11+=5.0*rad*trimm_z;
		wing_ang11=5.0*rad*trimm_z;
	}
	
	if(wing_ang11<-55.0*rad) wing_ang11=-55.0*rad;
	if(wing_ang11> 55.0*rad) wing_ang11= 55.0*rad;
	if(wing_ang22<-55.0*rad) wing_ang22=-55.0*rad;
	if(wing_ang22> 55.0*rad) wing_ang22= 55.0*rad;
	if(wing_ang33<-45.0*rad) wing_ang33=-45.0*rad;
	if(wing_ang33> 45.0*rad) wing_ang33= 45.0*rad;


int l=0;	
//	loopd(l,0,20,1)	//gforce setup
	//{
		if(l==1) if(fabs(gforce)<10.0) break;//
		if(l) //step 2
		{
			wing_ang22*=0.7;//OK~
//			break;
		}

aoa=0;
#if 0
//old
	wing_ang1=wing_ang11;	
	wing_ang2=wing_ang22+aoa;	//OK
	wing_ang3=wing_ang33;
#else
//interpol
//80-100 deg/sec a valos
	float1 alim=0.01;//0.1 0.5 0.02
	float1 danglim=100.0*rad;
	float1 delta_ang=(wing_ang11-wing_ang1);	
//	if(fabs(delta_ang)>danglim) delta_ang=fsgn(delta_ang)*danglim;
	if(fabs(delta_ang)>alim*rad) wing_ang1+=danglim*xtime*fsgn(delta_ang);	//0.1
	if(fabs(wing_ang11)<0.01*rad) wing_ang1=0.0;
	
	delta_ang=(wing_ang22+aoa-wing_ang2);	
//	if(fabs(delta_ang)>danglim) delta_ang=fsgn(delta_ang)*danglim;
	if(fabs(delta_ang)>alim*rad) wing_ang2+=danglim*xtime*fsgn(delta_ang);	
	if(fabs(wing_ang22)<0.01*rad) wing_ang2=0.0;
	
	delta_ang=(wing_ang33-wing_ang3);	
	//if(fabs(delta_ang)>danglim) delta_ang=fsgn(delta_ang)*danglim;
	if(fabs(delta_ang)>alim*rad) wing_ang3+=danglim*xtime*fsgn(delta_ang);	
	if(fabs(wing_ang22)<0.01*rad) wing_ang2=0.0;
	
#endif	
wing=wing_ang2/rad;
//wing=winglimit/rad;
	
	F_all=vec3(0,0,0);//recalc
	M_all=vec3(0,0,0);

	c1a=cos(+wing_ang1);//wing_ang2
	s1a=sin(+wing_ang1);
	c1b=cos(-wing_ang1);
	s1b=sin(-wing_ang1);
	c2=cos(wing_ang2);
	s2=sin(wing_ang2);
	c3=cos(wing_ang3);
	s3=sin(wing_ang3);
	c4=cos(-2.0*rad);//5 sok
	s4=sin(-2.0*rad);//c4=cos(-0.0*rad);s4=sin(-0.0*rad);//test
	ro=1.29+(0.1-1.29)*sqrt(0.01+saturate(plane_pos.y/50000.0));//air density 1.1-1.4
//nagy a legellenallas!!!!!!!!! ???????? ro*=0.5;	
//printf("%e ",wing_ang1);printf("%e \n",wing_ang2);

	
//forces
	F_all+=vec3(0,-1,0)*9.8*mass;//          gravity

//trust
//	F_all+=plane_mat.z*(trust)*2.0*80e3;// 2x80 kN  f18
//	F_all+=plane_mat.z*(trust)*2.0*142e3;// 2x142 kN  su35


float1 trust5=trust+sin(trusttime*M_PI/1.0);
if(trust5>1.0) trust5=1.0;
	if(enginepower>0.0 && trust5>0.0) 
	{
		float eff=12.0;// 8
		if(trust5>90.0) eff=25.0;//15?
		fuelCons=trust5*eff;
//fuelCons*=2.0;		//test   OK
		if(fuel>0.0) fuel-=fuelCons*xtime;
	}
	if(fuel<=0.0) trust5=0.0;


// 2x142 kN  su35  vektor!
float1 d=1.0;//2?
if(verticalmode)
{
#if 0
//regi rossz
    float1	aa=wing_ang2*3.0;//2
    float1	bb=-wing_ang1*3.0;//jobb bal
	vec3 F22=(plane_mat.y*cos(aa)*cos(bb) +plane_mat.z*sin(aa) +plane_mat.x*sin(bb))*trust5*enginepower;//142
	F22/=2.0;// 2*4/2
	
	F=F22*(1.0+bb*1.4);
	R=plane_mat.x*(3.0);
	F_all+=F;
	M_all+=cross(F,R);

	F=F22*(1.0-bb*1.4);
	R=plane_mat.x*(-3.0);
	F_all+=F;
	M_all+=cross(F,R);

	F=F22*(1.0+aa*1.4);
	R=plane_mat.z*( 3.0);//oldal
	F_all+=F;
	M_all+=cross(F,R);

	F=F22*(1.0-aa*1.4);
	R=plane_mat.z*(-3.0);
	F_all+=F;
	M_all+=cross(F,R);
#else
// jo vertical mode
    float1	aa=wing_ang2;//2
    float1	bb=-wing_ang1;//jobb bal
//	vec3 F22=(plane_mat.y*cos(aa)*cos(bb) +plane_mat.z*sin(aa) +plane_mat.x*sin(bb))*trust5*enginepower;//142	F22/=2.0;// 2*4/2
	vec3 F22=plane_mat.y*trust5*enginepower; // + lefele

// stabilizer   OK
{
vec3 up3(0,1,0);
up3-=plane_mat.z*dot(plane_mat.z,up);// mozgasi irany levag
up3=normalize(up3); // roll up
bb= 1.0-dot(plane_mat.y,up3); if(dot(plane_mat.x,up3)>0.0) bb= -bb; // side stabilizer
bb*=30.0;
bb+=-wing_ang1*2.5;// half manual control
if(bb<-1.0) bb=-1.0;
if(bb> 1.0) bb= 1.0;
}

{
vec3 up3(0,1,0);
up3-=plane_mat.x*dot(plane_mat.x,up);// olval irany
up3=normalize(up3);
aa=(1.0- dot(plane_mat.y,up3)); if(dot(plane_mat.z,up3)>0.0) aa= -aa; // bolintas stabilizer
aa*=30.0;
aa+=wing_ang2*2.5;// half manual control
if(aa<-1.0) aa=-1.0;
if(aa> 1.0) aa= 1.0;
}

	float1 scl=0.5; // 3*0.2=0.6
//side	
	F=F22*(1.0+bb*scl);
	R=plane_mat.x*(4.0);// position       3m old
	F_all+=F;
	M_all+=cross(F,R);
trust_pos[0]=plane_pos+R;	
trust_vec[0]=vec3(0,0,0) -F/enginepower;

	F=F22*(1.0-bb*scl);
	R=plane_mat.x*(-4.0);
	F_all+=F;
	M_all+=cross(F,R);
trust_pos[1]=plane_pos+R;	
trust_vec[1]=vec3(0,0,0) -F/enginepower;

//bolint    ,z be megy
	F=F22*(1.0+aa*scl);
	R=plane_mat.z*( 5.0);
	F_all+=F;
	M_all+=cross(F,R);
trust_pos[2]=plane_pos+R;	
trust_vec[2]=vec3(0,0,0) -F/enginepower;

	F=F22*(1.0-aa*scl);
	R=plane_mat.z*(-5.0);
	F_all+=F;
	M_all+=cross(F,R);
trust_pos[3]=plane_pos+R;	
trust_vec[3]=vec3(0,0,0) -F/enginepower;
	


	float1 cc=sidewing*3.0;

	vec3 F33=plane_mat.x*trust5*enginepower; // oldalra  tolja az elejet
	F=F33*(cc*scl*0.1);
	R=plane_mat.z*(3.0); // elol
	F_all+=F;
	M_all+=cross(F,R);
#endif	
}
else // original hajtomu
{
#ifdef _USE_SU35
	float1 h2=2.0,h5=8.0;
#else
	float1 h2=1.0,h5=6.0;
#endif
    float1	aa=clamp(-wing_ang2/d -wing_ang1/3.0,-30.0*rad,30.0*rad);//max 30 fok!!    wing1+-????
    float1 bb=clamp(-wing_ang3/d,-30.0*rad,30.0*rad);
if(vectortrust_on==0) {aa=0;bb=0;}
	F=(plane_mat.z*cos(aa)*cos(bb) +plane_mat.y*sin(aa) +plane_mat.x*sin(bb))*trust5*enginepower;//142
//	R=plane_mat.z*(-h5)+plane_mat.x*(h2);//2,5
	R=trust_pos[0]-plane_pos;
	F_all+=F;
	M_all+=cross(F,R);
//trust_pos[0]=mesh->pnts_trn[133];
trust_vec[0]=vec3(0,0,0) -F/enginepower;

	aa=clamp(-wing_ang2/d +wing_ang1/3.0,-30.0*rad,30.0*rad);
if(vectortrust_on==0) {aa=0;bb=0;}
	F=(plane_mat.z*cos(aa)*cos(bb) +plane_mat.y*sin(aa) +plane_mat.x*sin(bb))*trust5*enginepower;//142
//	R=plane_mat.z*(-h5)+plane_mat.x*(-h2);
	R=trust_pos[1]-plane_pos;
	F_all+=F;
	M_all+=cross(F,R);
//trust_pos[1]=mesh->pnts_trn[133+mesh->pnts_mir_base];
trust_vec[1]=vec3(0,0,0) -F/enginepower;
}

#if 0
wing_ang1=0;c1a=1;s1a=0;c1b=1;s1b=0;//test
wing_ang2=0;c2=1;s2=0;
wing_ang3=0;c3=1;s3=0;	
#endif

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2	

#ifdef _NEWPHY
//[41]
//veges elem fizika, new phy
//40-41 88  pnts wings   453 mirror Y   wcenter 904 453 1223

//BUG-f18 180mph/10fok nem jo 

	pretransform(xtime);//minden phy lepesben kell!!
	pretransformB(xtime);

	debverts.clear();
//	float1 alf;

	static int elsooo=1;//first
	if(elsooo)
	{
		elsooo=0;
#if 1
		loopd(i,0,mesh->pnts.size(),1)//move WeightCenter WC
		{
			mesh->pnts[i].x-=0.5;//- elorebb  +hatrebb  +1.5kerek!  OK!    1mar nem jo!   .x+=
			mesh->pnts[i].z-=0.4;//+lejjebb   -feljebb  +0.7  1.2
//			mesh->pnts_mir[i].x-=0.5;//- elorebb  +hatrebb  +1.5kerek!  OK!    1mar nem jo!   .x+=
//			mesh->pnts_mir[i].z-=0.4;//+lejjebb   -feljebb  +0.7  1.2
		}
#endif
		mesh->chk_surface_occlusion();//set 256 512 bit 
	}
	float1 A1=0,A2=0;
	float allarea	=0;
	
//	if(this!=selairplane) // old newphy     rossz
	if(0) // old always
	{
// old newphy   .adt filebol    TODO
 	//no physics test       6600 face ->20 fps !
	loopd(i33,0,mesh->faces.size(),3)
	if((i33+2)<mesh->faces.size())
	if((mesh->types[i33]&32)==0 &&(mesh->types[i33]&64)==0 &&(mesh->types[i33]&4096)==0 )//no wheel   no hook
	{
		int i3=i33/3;
//left side	
		int f1=mesh->faces[i33];
		int f2=mesh->faces[i33+1];
		int f3=mesh->faces[i33+2];
		if(f1!=f2)
		if(f1!=f3)
		if(f2!=f3)
		{
		vec3 v11=mesh->pnts_trn[f1];
		vec3 v22=mesh->pnts_trn[f2];
		vec3 v33=mesh->pnts_trn[f3];
		vec3 cent=(v11 + v22 + v33)/3.0;

		if((mesh->types[i33]&8192)==0)	normal=normalize(cross(v33-v11 , v22-v11));//v31-v21 felfele         //nem _mir
		else							normal=normalize(cross(v22-v11 , v33-v11));//forditva

//		if(mesh->types[i33]&256) normal=vec3(0,0,0)-normal;//add_force_rec(v11,v22,v33,mesh->types[i33]&1,normal,2);
#if 1
		R=cent-plane_pos;
		A=mesh->areas[i3];// *2 CHEAT
		A=tri_area(v11,v22,v33);

//if(mesh->types[i33]!=0x15) A1+=A;else A2+=A;//printf("%e \n",R);
//ok		ketoldalu kell mert negativ nyomas is keletkezik a takart oldalon!     //150 nel atesik lol
		if(A>0.01 && A<10.0)
		{
			if(mesh->types[i33]&1)  {add_forceB();}//wing 2 side  1
			else			 		{add_forceBN3();}//body closed 1 side   
		}
#endif

#ifdef _DEBUGVECTOR
		debverts.push_back(cent);	debverts.push_back(cent+normal*A*1.0);
		debverts.push_back(cent);	debverts.push_back(cent+F*1.5e-4);	
//		debverts.push_back(plane_pos);	debverts.push_back(plane_pos+R);	
#endif
	
		mesh->forces[0][i3]=length(F)*fsgn(dot(F,normal));
//		mesh->forces[1][i3]=length(F)*fsgn(dot(F,normal));

/*			{
			vec3 vdrag=vec3(0,0,0) - spd - cross(R,omega)-wind;
			vel=length(vdrag);
			float1 q=dot(normalize(vdrag),normal);
			allarea+=fabs(A*q);
			}*/
		}
	}
	//printf("AA %e %e\n",A1,A2);	//0x11=24.5  ,0x12=0.73    0x15=5.4
//	if(this!=selairplane)		printf("allarea %f \n",allarea	);// 9-20 ua
	}
	else // csak sajat
	{
	//ne newphy -> .obj filebol  TODO
	
	for(auto &mesh2:plane_obj.meshs)
	{
		if(gear==1 || strstr(mesh2.name.c_str(),"wheel")==0)  // gear
		if(mesh2.visible) // no pivot_xx   no axis_xx
		loopd(i3,0,mesh2.faces1.size(),3)
		{
			int f1=mesh2.faces1[i3];
			int f2=mesh2.faces1[i3+1];
			int f3=mesh2.faces1[i3+2];
			if(f1!=f2)
			if(f1!=f3)
			if(f2!=f3)// nem vonal
			{
//			if(f1>=mesh2.vert_tra.size()) continue;		if(f2>=mesh2.vert_tra.size()) continue;		if(f3>=mesh2.vert_tra.size()) continue;		

			vec3 cent=mesh2.face_center[i3];
			float A=mesh2.tri_area3[i3];
			
			vec3 R=cent-plane_pos;
			vec3 normal=mesh2.face_normal_tra[i3];
			
			if(mesh2.haspivot) R=mesh2.pivot_pos_tra - plane_pos; // BUG a tengelyre adja a nyomatekot!
			
#if 0
			vec3 v1=mesh2.vert_tra[f1];
			vec3 v2=mesh2.vert_tra[f2];
			vec3 v3=mesh2.vert_tra[f3];
			int fn1=mesh2.faces3[i3];
			vec3 normal2=normalize(cross(v3-v1,v2-v1  ));
			if(dot(normal2,mesh2.norm_tra[fn1])<0.0) normal2=-normal2;
			printf("%e \n",dot(normal,normal2));  // check precalculated normal         ~1 OK
#endif

			if(A>0.001 && A<10.0) // 0.1x0.1 =0.01
			{
//				add_forceB4(); // TODO   modified here
//printf("%e \n",A);
				vec3 vdrag=vec3(0,0,0) - spd - cross(R,omega)-wind;
				vel=length(vdrag);
				float1 q=dot(normalize(vdrag),normal);


//getC2 test

//	NA			if(q>0) q=-q*0.7;	//add_forceBN3  ha q>0 akkor is -         fos
// NA				if(q>0) q=q*0.5;	// 2 side   ?? masik is 0.5!
				if(q<0.0) // tuti kell, mert amugy 2x felulet!
				{                                          
#if 0
// OK				
					if(mesh2.phytype==1) F=normal*A*vel*vel*ro*fsgn(q)*getC2(q);// iiddee     szarnyak    igy JO!
					else                 F=normal*A*vel*vel*ro*q; // body es a tobbi nem szarny
#else
// TEST : 2 normal komponens a sebessegbol[vdrag] lebontva        max spd
					if(mesh2.phytype==1) 
					{
						vec3 dir=normalize(vdrag);
						vec3 normalX=dir*dot(dir,normal);
						vec3 normalY=normal-normalX;
//						normalX=normalize(normalX);//?
//						normalY=normalize(normalY);
						F=normalY*A*vel*vel*ro*fsgn(q)*getC2(q);// iiddee     szarnyak    igy JO!
						F+=normalX*A*vel*vel*ro*q*1.0;// szelirany               ,1.5 amugy kicsinek tunik? TODO
					}
					else                 
					{
						F=normal*A*vel*vel*ro*q; // body es a tobbi nem szarny
					}
#endif					
					F_all+=F;
					M_all+=cross(F,R);

#ifdef _DEBUGVECTOR
		debverts.push_back(cent);	debverts.push_back(cent+normal*A*1.0);
		debverts.push_back(cent);	debverts.push_back(cent+F*1.5e-4);	
//		debverts.push_back(plane_pos);	debverts.push_back(plane_pos+R);	
#endif					
					float r=0,g=0,b=0;
					if(q<0) g=-q*2.0;
					else    r=q*2.0;
//				r=getC2(q);
					mesh2.face_color[i3]=vec4(r,g,b,0.7);
					
//					allarea+=fabs(A*q);

#if 0
// a feluleten aramlik a levego a szomszed haromszogek fele!  csillapitva					
					vdrag-=normal*dot(normal,vdrag)*0.7; // csillapitja az elso utkozes a szelet
					int i33=i3/3;
					for(int j=0;j<mesh2.linked[i33].size();j++)
					{
						int f=mesh2.linked[i33][j];
						
						vec3 dir=mesh2.face_center[f] -cent;
						if(dot(dir,vdrag)>0.8) // arra van a szomszed tri amerre megy az aramlat
						{
							mesh2.wind[f/3]=vdrag; // +=
						}
					}
#else
/*
					vec3 v11=mesh2.vert[f1]; // original
					vec3 v22=mesh2.vert[f2];
					vec3 v33=mesh2.vert[f3];
					
					vec3 bbsize=plane_obj.bbox.bbmax-plane_obj.bbox.bbmin;
					int xi=(int)((v11.x-plane_obj.bbox.bbmin.x)*64.0/bbsize.x);
					int yi=(int)((v11.y-plane_obj.bbox.bbmin.y)*64.0/bbsize.y);
					int zi=(int)((v11.z-plane_obj.bbox.bbmin.z)*64.0/bbsize.z);
					airbox[xi&63][yi&63][zi&63]=vdrag;
					*/
#endif
				}
			}
			}
		}
//		if(this!=selairplane)	printf("allarea %f \n",allarea	); // 9-20

# if 0					
		if(mesh2.visible)
		loopd(i3,0,mesh2.faces1.size(),3)
		{
			int f1=mesh2.faces1[i3];
			int f2=mesh2.faces1[i3+1];
			int f3=mesh2.faces1[i3+2];

			vec3 cent=mesh2.face_center[i3];
			float A=mesh2.tri_area[i3];
			
			vec3 R=cent-plane_pos;
			vec3 normal=mesh2.face_normal_tra[i3];

			
			if(A>0.01 && A<10.0)
			{
				vec3 vdrag=mesh2.wind[i3/3];
				mesh2.wind[i3/3]=vec3(0,0,0);
				
				vel=length(vdrag);
				if(vel>0.1)
				{
				float1 q=dot(normalize(vdrag),normal);
//				if(q>0.0)  q=0.0;
				if(q<0.0)
				{
//					F=normal*A*vel*vel*ro*fsgn(q)*getC(q)/2.0;
					F=normal*A*vel*vel*ro*q;
					F_all+=F;
					M_all+=cross(F,R);
#if 0
// a feluleten aramlik a levego a szomszed haromszogek fele!  csillapitva					
					vdrag-=normal*dot(normal,vdrag)*0.7; // csillapitja az elso utkozes a szelet
					int i33=i3/3;
					for(int j=0;j<mesh2.linked[i33].size();j++)
					{
						int f=mesh2.linked[i33][j];
						
						vec3 dir=mesh2.face_center[f] -cent;
						if(dot(dir,vdrag)>0.0) // arra van a szomszed tri amerre megy az aramlat
						{
							mesh2.wind[f/3]+=vdrag;
						}
					}
#endif					
				}
				}
			}
		}
#endif

	}		
	}
	
/*		normal=normals[i3];
		R=tricenters[i3];
		normal=qtransform(normal);//to airplane matrix
		R=qtransform(R);*/


	
	gforce=length(F_all/mass)/9.8;
#else	
//old phy
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2	
//c1a=1; s1a=0;c1b=1; s1b=0;c2=1;s2=0;//stop
//iiddee

float1 ml=2.0;//2? su35 regi rossz

if(verticalmode==0)
{
#if 1
//roll -----------------------
	A=2*(4.0+4.0);//3-4   m^2  + hatso szarmy!
A=ml*(2.4+5.3);	//+hatso!!!!!1
//right wing roll  airelon,littlewing
	normal=plane_mat.y*c1a + plane_mat.z*s1a;//surface normal
	R=plane_mat.z*(-1.5) + plane_mat.x*(5.0);//-1.5 5
	add_forceB_ori();

//left wing roll
	normal=plane_mat.y*c1b + plane_mat.z*s1b;//surface normal  
	R=plane_mat.z*(-1.5) - plane_mat.x*(5.0);
	add_forceB_ori();
#endif	
}

#if 1
//c4== -5fok
//	A=2*(15.0+flapwing*2.4);//9~11  38/2  19su35
	A=ml*(22.0+flapwing*2.4);//9~11  38/2  19su35  15??????  25
//right wing and lift force, -5 degree?
	normal=plane_mat.y*c4 + plane_mat.z*s4;//surface normal
	R=plane_mat.z*(-0.0) + plane_mat.x*(6.0);//0 6
	add_forceB_ori();
//left
	normal=plane_mat.y*c4 + plane_mat.z*s4;//surface normal
	R=plane_mat.z*(-0.0) - plane_mat.x*(6.0);
	add_forceB_ori();
#endif




#if 1
	A=2*7.0;// //hatul 
A=ml*5.3;	
//side stable right   , rudder
	normal=plane_mat.x*c3 + plane_mat.z*s3;//surface normal
	R=plane_mat.z*(-4.0) + plane_mat.x*(1.5);//-3 1.5  !
	add_forceB_ori();
//left
	normal=vec3(0,0,0)-plane_mat.x;//*c3 + plane_mat.z*s3;//surface normal
	R=plane_mat.z*(-4.0) - plane_mat.x*(1.5);
	add_forceB_ori();

#if 1
	A=ml*6.0;// oldala 10? 6
//A=ml*22.0;	//12 40??? ???????????????
//side stable 
	normal=plane_mat.x;//surface normal
	R=plane_mat.z*(0.0) + plane_mat.x*(1.5);//-3??? 1.5
	add_forceB_ori();

	normal=vec3(0,0,0)-plane_mat.x;
	R=plane_mat.z*(0.0) - plane_mat.x*(1.5);
	add_forceB_ori();
#endif
#endif


if(verticalmode==0)
{
#if 1
	A=2*4.0;// full wing move  2x4
A=ml*5.3;	
//lifter right   tailplane
	normal=plane_mat.y*c2 + plane_mat.z*s2;//surface normal
	R=plane_mat.z*(-5.0) + plane_mat.x*(2.0);//   -5 2
	add_forceB_ori();
//left
	normal=plane_mat.y*c2 + plane_mat.z*s2;//surface normal
	R=plane_mat.z*(-5.0) - plane_mat.x*(2.0);
	add_forceB_ori();
#endif
}




#if 1
//	A=4.0*0.1+speedbrake*4.0+flapwing*2.0;//1-2
//A=6.2*0.1+speedbrake*4.0+flapwing*2.0;//1-2
//A=5.9+speedbrake*4.0+flapwing*2.0;//1-2
A=1.5+speedbrake*2.0+flapwing*2.0;//1-2
//air resistance
	normal=plane_mat.z;//surface normal
	R=plane_mat.z*(0.0) + plane_mat.x*(0.0);
	add_forceB_ori();
#endif

	gforce=length(F_all/mass)/9.8;
//gforce=0;
#endif// _ NEWPHY


	//}//regi gforce setup
	
//printf("c %.02f %.02f %.02f \n",plane_mat.z.x,plane_mat.z.y,plane_mat.z.z);
//printf("N %.02f %.02f %.02f \n",normal.x,normal.y,normal.z);



vec3 forward=plane_mat.z;
vec3 sideward=plane_mat.x;//forward.y=0; forward=normalize(forward);//sideward.y=0; sideward=normalize(sideward);




	
//landing
//#ifndef _NEWPHY
#if 1
	{
	vec3 dy(0,0,0);
	if(gear==0) dy.z=1.5;//down
	
	float1 q1=0.1;   //elnyeles  lefele  0.03  2sok   0.9 sok    0.7 sok
	float1 q3=0.7;//side  0.3      0.9 megcsavarja valamiert
//float1 q1=0.2;	//?
	float1 q2=0.001;//sikba  0.01 sok
	if(gear) //down
	if(whlbreak) 
	{
		q2=0.02;//0.1 sok 0.01
		if(length(spd)<	40.0*knots/3.6) q2=0.2;
	}
	
	if(gear) if(length(spd)>260.0*knots/3.6) enable_physics=0; //300?TODO



#ifdef _USE_SU35
	int pntwhl1=136,pntwhl2=111;//su35   136   136  111
#else
	int pntwhl1=139,pntwhl2=169;//f18 hatso elso 	154 139
#endif
//float1 hz=2.0;q1=(1.0/hz)/(2.0*M_PI); q1=q1*q1; q1=(mass/3.0)/q1; printf("%e \n",q1);//D=2.6e5 7.1e5
#ifdef _USE_SU35
	float1 D=2e5;//6e4  T=2pi sqrt(m/D)  	5Hz      //sin3fok *60m/s ~3.1m/s *10000kg 30KN ~0.5m 30?  50k  !2e5  6?8
#else
	float1 D=3.5e5;//f18  hatso  1,5    1.2e5 7.1
	q1=1.0;//1.5
#endif
//float1 D=8e5;	//?
//D=4e5;//2 HZ kellene	
//	R=plane_mat.z*(-0.5) + plane_mat.x*(2.0) + plane_mat.y*(-1.0-dy);//wheel rel pos 3
/*	R=qtransform(mesh->pnts[136]+dy);//hatso [111] [136]    //old
	vec3 p1=plane_pos+R; */
//	vec3 p1=mesh->pnts_trn[pntwhl2];//136 new visszafele  elso wheel
	vec3 p1=wheelymin[0];
	R=p1-plane_pos;
	vec3 deep;
	vec3 normal,obj;
	int indx=0;
	int hit=carriermesh.getcollision4(plane_pos,p1,deep,normal,obj,indx);
	
//	float1 carrier_dy=getcarrierhigh(p1);//old collision, only y!!
//	vec3 carrier_spd2=carrier_spd; if(carrier_dy==0) carrier_spd2=vec3(0,0,0);
	vec3 carrier_spd2=vec3(0,0,0); if(plane_pos.y>10.0) carrier_spd2=carrier_spd;

	wheeldy[0]=normal*deep.x; //if(wheeldy[0]<0.0) wheeldy[0]=0.0;
/*	wheeldy[0]=waterlevel+carrier_dy-p1.y; if(wheeldy[0]<0.0) wheeldy[0]=0.0;
	if(p1.y<waterlevel+carrier_dy-1.0) enable_physics=0;//broken
	if(p1.y<waterlevel+carrier_dy) //underground  */
	if(deep.x>1.0) {enable_physics=0;}//broken   printf("1 %e \n",deep);
	if(hit)
	{
		vec3 vel=spd + cross(R,omega)-carrier_spd2;
//normal=vec3(0,1,0);//old
//printf("3 %e %e %e \n",normal.x,normal.y,normal.z);

//		F=vec3(0,1,0)*fabs((waterlevel+carrier_dy-p1.y))*D;//rugo  plane_pos  spring
		F=normal*deep.x*D;//rugo  plane_pos  spring
		F-=normal*dot(vel,normal)*q1*mass;//vel.y
		//F-=vec3(1,0,1)*vel*q2*mass;
		vec3 forward2=forward; forward2-=normal*dot(forward2,normal);forward2=normalize(forward2);//normalra meroleges
		vec3 sideward2=sideward; sideward2-=normal*dot(sideward2,normal);sideward2=normalize(sideward2);
		F-=forward2*dot(vel,forward2)*q2*mass;
		F-=sideward2*dot(vel,sideward2)*q3*mass;

		F_all+=F;
		M_all+=cross(F,R);
	}

	//R=plane_mat.z*(-0.5) + plane_mat.x*(-2.0) + plane_mat.y*(-1.0-dy);//wheel rel plane_pos
/*	vec3 w2=mesh->pnts[136]; w2.y=-w2.y;  //old mir
	R=qtransform(w2+dy);//hatso 
	p1=plane_pos+R; */
	//p1=mesh->pnts_trn[pntwhl2+mesh->pnts_mir_base];//new visszafele
	p1=wheelymin[1];
	R=p1-plane_pos;
	hit=carriermesh.getcollision4(plane_pos,p1,deep,normal,obj,indx);
	
	//carrier_dy=getcarrierhigh(p1);
//	carrier_spd2=carrier_spd; if(carrier_dy==0) carrier_spd2=vec3(0,0,0);
	carrier_spd2=vec3(0,0,0); if(plane_pos.y>10.0) carrier_spd2=carrier_spd;

	wheeldy[1]=normal*deep.x; //if(wheeldy[1]<0.0) wheeldy[1]=0.0;
/*	wheeldy[1]=waterlevel+carrier_dy-p1.y; if(wheeldy[1]<0.0) wheeldy[1]=0.0;
	if(p1.y<waterlevel+carrier_dy-1.0) enable_physics=0;//broken
	if(p1.y<waterlevel+carrier_dy) //underground */
	if(deep.x>1.0) {enable_physics=0;}//broken  printf("2 %e \n",deep);
	if(hit)
	{
		vec3 vel=spd + cross(R,omega)-carrier_spd2;
//normal=vec3(0,1,0);//old

//		F=vec3(0,1,0)*fabs((waterlevel+carrier_dy-p1.y))*D;//rugo
		F=normal*deep.x*D;//rugo
		F-=normal*dot(vel,normal)*q1*mass;
//		F-=vec3(1,0,1)*vel*q2*mass;
		vec3 forward2=forward; forward2-=normal*dot(forward2,normal);forward2=normalize(forward2);//normalra meroleges
		vec3 sideward2=sideward; sideward2-=normal*dot(sideward2,normal);sideward2=normalize(sideward2);
		F-=forward2*dot(vel,forward2)*q2*mass;
		F-=sideward2*dot(vel,sideward2)*q3*mass;

		F_all+=F;
		M_all+=cross(F,R);
	}
#ifdef _USE_SU35
	D=2e5;//6e4  T=2pi sqrt(m/D)  	5Hz      //sin3fok *60m/s ~3.1m/s *10000kg 30KN ~0.5m 30?  50k  !2e5  6?8
#else
	D=2.0e5;//f18 elso  1.2   1.0e5  6
#endif


	float1 c1c=cos(+wing_ang1*0.1);//wing_ang2
	float1 s1c=sin(+wing_ang1*0.1);

 forward=plane_mat.z*c1c + plane_mat.x*s1c;//wheel kormany  c3!
 sideward=plane_mat.x*c1c -plane_mat.z*s1c;

//	R=plane_mat.z*(2.0) + plane_mat.x*(0.0) + plane_mat.y*(-1.0-dy);//wheel rel pos rear
/*	R=qtransform(mesh->pnts[111]+dy);//elso   //old
	p1=plane_pos+R; */
//	p1=mesh->pnts_trn[pntwhl1];//111 new visszafele
	p1=wheelymin[2];
	R=p1-plane_pos;
	hit=carriermesh.getcollision4(plane_pos,p1,deep,normal,obj,indx);
	
	//carrier_dy=getcarrierhigh(p1);
//	carrier_spd2=carrier_spd; if(carrier_dy==0) carrier_spd2=vec3(0,0,0);
	carrier_spd2=vec3(0,0,0); if(plane_pos.y>10.0) carrier_spd2=carrier_spd;
	
	wheeldy[2]=normal*deep.x; //if(wheeldy[2]<0.0) wheeldy[2]=0.0;
/*	wheeldy[2]=waterlevel+carrier_dy-p1.y; if(wheeldy[2]<0.0) wheeldy[2]=0.0;
	if(p1.y<waterlevel+carrier_dy-1.0) enable_physics=0;//broken
	if(p1.y<waterlevel+carrier_dy) //underground  */
	if(deep.x>1.0) {enable_physics=0;}//broken  printf("3 %e \n",deep);
	if(hit)
	{
		vec3 vel=spd + cross(R,omega)-carrier_spd2;
//normal=vec3(0,1,0);//old

//		F=vec3(0,1,0)*fabs((waterlevel+carrier_dy-p1.y))*D;//rugo
		F=normal*deep.x*D;//rugo
		F-=normal*dot(vel,normal)*q1*mass;
//		F-=vec3(1,0,1)*vel*q2*mass;
		vec3 forward2=forward; forward2-=normal*dot(forward2,normal);forward2=normalize(forward2);//normalra meroleges
		vec3 sideward2=sideward; sideward2-=normal*dot(sideward2,normal);sideward2=normalize(sideward2);
		F-=forward2*dot(vel,forward2)*q2*mass;
		F-=sideward2*dot(vel,sideward2)*q3*mass;

		F_all+=F;
		M_all+=cross(F,R);
	}
	
	}

#if 0
//wheel broken
//TODO	if(this==selairplane) physics=enable_physics;
	if(this==selairplane) 
	if(enable_physics==0) 
	{
		gear=0;
		gearneed=2;
		
		if(gearneed==0)	geartimer=3.0;//reset
		else			geartimer=0.0;

	}
#endif	


#ifdef _USE_SU35
	int pnthookend=132,pnthookforce=50;//su35
#else
	int pnthookend=177,pnthookforce=182;//f18  83
#endif
	
//hook
	if(ewater)
	if(ehook)
	{
/*	R=qtransform(mesh->pnts[132]);//end of hook 
	vec3 hookend=plane_pos+R;
	vec3 cable1=carrier_pos +vec3(-10.0,20.0,0.0);//mar kozepen!
	vec3 cable2=carrier_pos +vec3(  0.0,20.0,0.0);
	vec3 cable3=carrier_pos +vec3( 10.0,20.0,0.0);*/
//	hookend=mesh->pnts_trn[pnthookend];//plane_pos+R;
	hookend=hookend2;
	vec3 cable1=(carriermesh.pnts_trn[65]+carriermesh.pnts_trn[66])/2.0;
	vec3 cable2=(carriermesh.pnts_trn[67]+carriermesh.pnts_trn[68])/2.0;
	vec3 cable3=(carriermesh.pnts_trn[69]+carriermesh.pnts_trn[70])/2.0;
	vec3 cable4=(carriermesh.pnts_trn[71]+carriermesh.pnts_trn[72])/2.0;
/*	R=qtransform(mesh->pnts[50]);//force point [89] 50/12
	vec3 hookforce=plane_pos+R; */
	hookforce=mesh->pnts_trn[pnthookforce];
//	float1 carrier_dy=getcarrierhigh(p1+carrier_pos);//hook cable!

	float1 hooklen=length(hookend-hookforce);
	vec3 deep;
	vec3 normal,obj;
	int indx=0;
	int hit=carriermesh.getcollision4(hookforce,hookend,deep,normal,obj,indx);//plane_pos
	if(hit)
	{
		hookend=hookend+normal*deep.x;
		vec3 w1=normalize(hookend-hookforce);
		hookend=hookforce +w1*hooklen;
		loopd(k,0,5,1)
		{
			hit=carriermesh.getcollision4(plane_pos,hookend,deep,normal,obj,indx);
			if(hit==0) break;
			hookend=hookend+normal*deep.x;
			vec3 w1=normalize(hookend-hookforce);
			hookend=hookforce +w1*hooklen;
		}
	}

	vec3 p1=hookend - cable1; 
	vec3 p2=hookend - cable2; 
	vec3 p3=hookend - cable3; 
	vec3 p4=hookend - cable4; 

	
	vec3 w1=p1; w1.y=0;//2D
	if(hookconnect==0)// && length(w1)<20.0) 		if(p1.y<waterlevel+carrier_dy)	
	{
		hookcable=0;
//printf("%e \n",p1.x);		//-x!
		if((hookend.x<=cable1.x && hookendold.x>=cable1.x) ||
		   (hookendold.x<=cable1.x && hookend.x>=cable1.x))  //athaladt a kotelen
		if(p1.z>=-15.0)// cable wide 30!
		if(p1.z<= 15.0)//palyan belul
		if(p1.y<0.2)//magassag ok
		if(p1.y>-0.2) hookcable=1;//drotkotel beakad
		
		if((hookend.x<=cable2.x && hookendold.x>=cable2.x) ||
		   (hookendold.x<=cable2.x && hookend.x>=cable2.x))  //athaladt a kotelen
		if(p2.z>=-15.0)
		if(p2.z<= 15.0)//palyan belul
		if(p2.y<0.2)//magassag ok
		if(p2.y>-0.2) hookcable=2;//drotkotel beakad
		
		if((hookend.x<=cable3.x && hookendold.x>=cable3.x) ||
		   (hookendold.x<=cable3.x && hookend.x>=cable3.x))  //athaladt a kotelen
		if(p3.z>=-15.0)
		if(p3.z<= 15.0)//palyan belul
		if(p3.y<0.2)//magassag ok
		if(p3.y>-0.2) hookcable=3;//drotkotel beakad
		
		if((hookend.x<=cable4.x && hookendold.x>=cable4.x) ||
		   (hookendold.x<=cable4.x && hookend.x>=cable4.x))  //athaladt a kotelen
		if(p4.z>=-15.0)
		if(p4.z<= 15.0)//palyan belul
		if(p4.y<0.2)//magassag ok
		if(p4.y>-0.2) hookcable=4;//drotkotel beakad
		
		if(hookcable)	
		{
			hookconnect=1;
//physics=0;//pause
		

//		float1 alf=69.5*rad;
/*		float1 alf=90*rad;	
		vec3 irx=vec3( sin(alf),0.0,cos(alf));		
		float1 dx=dot(p1,irx);
		hookcable=0;
		if(dx<=20.0)	if(dx>5.0)		{hookcable=1;	}
		if(dx<=5.0)		if(dx>-10.0)	{hookcable=2;	}		
		if(dx<=-10.0)	if(dx>-25.0)	{hookcable=3;	}		*/
		
//		hookvector=p1;	
	
//		if(hookcable==0) hookconnect=0;
		/*
		printf("touch: \n%e %e %e\n%e %e %e\n%e %d\n",
			plane_pos.x,plane_pos.y,plane_pos.z,
			p1.x,p1.y,p1.z,
			length(w1),hookcable
			);*/
		}
	}
	if(hookcable==1) hookvector=(hookforce-cable1);//recalc
	if(hookcable==2) hookvector=(hookforce-cable2);
	if(hookcable==3) hookvector=(hookforce-cable3);
	if(hookcable==4) hookvector=(hookforce-cable4);
	
	hookendold=hookend;
#if 0	
dumpvec1=hookforce;
if(hookcable==1) dumpvec2=cable1;
if(hookcable==2) dumpvec2=cable2;
if(hookcable==3) dumpvec2=cable3;
dumpvec1=plane_pos;
dumpvec2=plane_pos+forward*15.0;
dumpvec3=plane_pos+sideward*5.0;
#endif	
	
	vec3 w11=spd-carrier_spd; w11.y=0;
	//if(length(w1)<1.0) hookconnect=0;//dont disconnect
	
	if(hookconnect==1)
//	if(p1.y<waterlevel+carrier_dy)//drotkotel
	{
		vec3 norm=normalize(spd-carrier_spd)*vec3(1,0,1);//          spd realis
		norm.y=0.04;//kicsit felfele hat  /lefele mert forditott/ 0.4??

		vec3 norm2=normalize(hookvector);//new
		float1 cablelen=length(hookvector);
		norm=normalize(norm);
		
		if(dot(norm,norm2)>0.0)
		{
			if(length(w11)>1.0 &&cablelen>5.0)//amig mozog! es eleg tavol  5?
			{
//				float1 t1=(cablelen-5.0)/100.0;		if(t1>1.0) t1=1.0;
				F-=norm2*mass*9.81*1.5;//2g = a = 60m/s / 3sec    1.8
			
				F_all+=F;
//F&R egyvonalban!				M_all+=cross(F,R);//masscnterben! no torque?

				vec3 cable_islinear=hookforce-normalize(hookvector)*hooklen;
//				hookend=lerp(hookend,cable_islinear,0.5+0.5*cos(gametime*M_PI*2.0/1.5));//fake
				hookend=lerp(hookend,cable_islinear,saturate(length(w11)/60.0));
			}
		}
		if(w11.x>0.0 ||length(w11)<1.0 ||cablelen>180.0)//hatra vagy sok 80
				{hookconnect=2;}//ehook=0;
				
//printf("x %e %e %e \n",w11.x,spd.x,carrier_spd.x);		
//printf("z %e %e %e %d %d \n",w11.z,spd.z,carrier_spd.z,hookconnect,ehook);		


//4draw				
		hookpnt=hookend;// hookpnt.y=0.2;
//hookpnt=hookforce;//dump
	}
	}
	if(ehook==0) hookconnect=0;

	if(this==selairplane)
	{
		slowtime=1.0;
if(0)	
		if(selcammode!=0)//select!
		if(hookconnect) {selcammode=2;}//2,3//slowtime=0.1;
	}
#endif	//no wheel hook

	if(this==selairplane)
	if(physics)
	{
		carrier_spd=vec3(-30.0*knots/3.6,0.0,0.0);// knots -> km/h -> m/s
		carrier_pos+=carrier_spd*xtime;
		
 		mypos=plane_pos;
		//lassan megy , nem kene transformalni, meg az airplannel csak j==0 ban utkozik
	}




	float1 mass2=2500.0; //      kg/darab

//carrier airplane collision
//	if((j==0))//faster  csak egyszer a 10 iteraciobol
	if(this==selairplane)//osszesre lassu
	{
//body hit collision
	qvector<s_coll> _colls;
	double t1=q_getcpuclock();
//	int hit=mesh->chk_mesh2meshcollis2(&carriermesh,normalize(spd),&_colls,0);///90-50 FPS
	int hit=carriermesh.chk_mesh2meshcollis4(mesh,normalize(spd),_colls,0); // getcollision(
	t1=q_getcpuclock()-t1;	timedump+=t1; // 2ms 0.6ms!
	if(hit)
	{
#if 1	
		float1 D=7.0e5;//?    rugoallando 
//		float1 D=2.0e5;//?    rugoallando 
		vec3 carrier_spd2=vec3(0,0,0); if(plane_pos.y>10.0) carrier_spd2=carrier_spd;
		
//		int n=_colls.size()/3; 
		int n=1; //??????? TODO  kell normalizalni?    :nem, igy jo 
		
		mesh->blackverts.clear();// egy utkozes csak 1x visz 1 vertexet
		mesh->blackverts2.clear();		
		loopd(i,0,_colls.size(),1)//pos norm deep
		{
//			debverts.push_back(_colls[i]);		debverts.push_back(_colls[i]+_colls[i+1]*5.0);
			
			R=_colls[i].obj-plane_pos;//utkozes elotti!
			if(dot(R,R)>0.0)//legyen radius
			{
//				F=_colls[i+1]*dot(spd*(-mass*1.5/n)/xtime,_colls[i+1]);//normal       /xtime

				vec3 vel=spd + cross(R,omega)-carrier_spd2;
				//NA if(length(vel)>10.0)// 10 m/s   ~ 36 km/h  ne torjon
				{
//		_colls[i].nor=normalize(R);// test TODO
				F=_colls[i].nor*(_colls[i].deep.x)*D/n;//normal  deep     /n?     TODO fabs? 
				F-=_colls[i].nor*dot(_colls[i].nor,vel)*0.8*mass;//elnyelodes  0.7  kicsi atmegy   0.9   -? TODO

	//?			F=-vel*mass*10.0; // /xtime; // ?/xtime;// impulse   0.1sec

				F_all+=F;
				M_all+=cross(F,R);
			
			
#if 0			
				//tores gyurodes roncsolodas
				int ii=_colls[i].iface;
				int a1=mesh->faces[ii];
				int a2=mesh->faces[ii+1];
				int a3=mesh->faces[ii+2];

//				F=_colls[i].nor*dot(_colls[i].nor,vel)*mass2/0.1; // ?/xtime;// impulse   0.1sec                normal nem jo!
				F=-vel*mass2*10.0; // ?/xtime;// impulse   0.1sec
				
				//F*=0.3;//ennyit nyel el a deformacio
//				F=normalize(-vel)*0.02;// ?????  0.02
				vec3 obj=transform(_colls[i].obj,mesh->itramat);// coll point
				F=transformNorm(F,mesh->itramat);// KELL!

//				float1 D5=D*xtime/mass2;// 1e6;// ~
				vec3 a=F/mass2; //  a=F/m            accs[i]*dt !
				
//				F=_colls[i].nor*fabs(_colls[i].deep.x);//  *xtime*16.0;//new v2.0   2.5

/*				mesh->addForce(obj,a1,a,4);
				mesh->addForce(obj,a2,a,4);
				mesh->addForce(obj,a3,a,4);*/
				
				mesh->distorForce(a1,a,4,obj,xtime);// 5 recursion  2?
				mesh->distorForce(a2,a,4,obj,xtime);
				mesh->distorForce(a3,a,4,obj,xtime);
				
/*				mesh->restoreEdges(a1,4,D5);// rugo restore          vegen     
				mesh->restoreEdges(a2,4,D5);
				mesh->restoreEdges(a3,4,D5);*/
#endif			
				}	
			}
		}
#endif		
	}
#if 1
	loopd(k,1,airplanelist.size(),1)
	{
	hit=mesh->chk_mesh2meshcollis4(airplanelist[k]->mesh,normalize(spd),_colls,1);///90-50 FPS  3b
	if(hit)
	{
		float1 D=7.0e5;//?
//		vec3 carrier_spd2=vec3(0,0,0); if(plane_pos.y>10.0) carrier_spd2=carrier_spd;  whut?
		
//		int n=_colls.size()/3;
		int n=1; //??????? TODO  kell normalizalni?    :nem, igy jo 

		loopd(i,0,_colls.size(),1)//pos norm deep
		{
			R=_colls[i].obj-plane_pos;//utkozes elotti!
			if(dot(R,R)>0.0)//legyen radius
			{
				vec3 vel=spd + cross(R,omega); //-carrier_spd2;
				vel-=(airplanelist[k]->spd + cross(R,airplanelist[k]->omega));// -?
			
				F=_colls[i].nor*fabs(_colls[i].deep.x)*D/n;//normal  deep     /n?
				F-=_colls[i].nor*dot(_colls[i].nor,vel)*0.8*mass;//elnyelodes   0.9
			
				F_all+=F;
				M_all+=cross(F,R);
				
				//enemy
				R=_colls[i].obj- airplanelist[k]->plane_pos;
				if(dot(R,R)>0.0)//legyen radius
				{
					airplanelist[k]->Fcoll=-F;//atveszi
					airplanelist[k]->Icoll=-cross(F,R);
					airplanelist[k]->collvalid=1;
				}
			}
		}
	}
	}
#endif		
	}
	
#if 1	
//	float1	D5=17e6/2500.0;// 7e5 rugoallando        szilardsag 7e6? TODO              *xtime
	float1	D5=7e5/mass2;	//2e5?                                   acc=D*x=kg*m/s2  /m
	float1	K5=2e3/xtime; // 0.5e6/2500.0; // ~50/1.5         acc=m/s *1/s	
//	mesh->updateSpd(xtime,D5,K5);
#else	
//	mesh->restoreEdges(xtime);
#endif	

	if(collvalid)
	{
		F_all+=Fcoll;
		M_all+=Icoll;

		collvalid=0;
	}




//all forces  ---------------
	acc=F_all/mass;
	spd+=acc*xtime;  //a=F/m   v+=a dt
	L+=M_all*xtime;//L=M*dt

	
	
	gforce=length(F_all/mass)/9.8;
	if(dot(F_all,plane_mat.y)<0.0) gforce=-gforce;//negativ G
//printf("%e \n",length(R));
//if(gforce>10.0) printf("%e %e\n",gforce,length(F_all));//30 g 3MN
//gforce=0;//TODO


//div I
	x=dot(L,plane_mat.x)/I.x;//re
	y=dot(L,plane_mat.y)/I.y;
	z=dot(L,plane_mat.z)/I.z;
	omega=plane_mat.x*x + plane_mat.y*y + plane_mat.z*z;

//move	
	plane_pos+=spd*xtime;//4 gyors
	rotaxis(plane_mat,omega,xtime);//w     
//spd=plane_mat.z*120.0;

	plane_mat.x=cross(plane_mat.y,plane_mat.z);//zy?  yz right hand OK
	plane_mat.y=cross(plane_mat.z,plane_mat.x);//xz?  zx
	plane_mat.x=normalize(plane_mat.x);
	plane_mat.y=normalize(plane_mat.y);
	plane_mat.z=normalize(plane_mat.z);


	
	

	}//10 phy
	xtime*=10.0;

//printf("%e %e %e \n",plane_pos.x,plane_pos.y,plane_pos.z);


//keys
	if(this==selairplane)
	{
#if 1
/*	if(key=='1') plane_pos.y=4.0;//reset
	if(key=='2') plane_pos.y=5.0;//reset
	if(key=='3') plane_pos.y=6.0;//reset
	if(key=='4') plane_pos.y=7.0;//reset
*/

	if(trusttime>0.0) trusttime-=xtime;
	else trusttime=0.0;


if(0)
	if(key=='8') 
	{
		plane_pos.y=2.1;//reset
		plane_pos.x=0.0;
		plane_pos.z=0.0;
		
		spd.x=0;
		spd.y=0;
		spd.z=0;
		
		gear=1;
		trust=0;
		
		plane_mat.z=vec3(1,0,0);//look around
		plane_mat.y=vec3(0,1,0);
		plane_mat.x=vec3(0,0,1);
		L=vec3(0,0,0);

		physics=1;
	}
#endif
	if(key=='y') 
	{
		plane_pos.y=10000.0;//reset
	}
	if(key=='r') 
	{
		float1 aa=70.0*rad;
		plane_pos=vec3(-2100,300,60);//land test
		plane_pos=vec3(-frnd(12100.0),300.0+frnd(11000.0),frnd(12060.0));//land test
plane_pos=vec3(3000.0+frnd(2110.0),1300.0+frnd(500.0),frnd(1260.0)-500.0);//land test
plane_pos=vec3(7000.0,800.0,0.0);

		//plane_pos=vec3(-300,51,50);



	plane_pos=vec3(5510,500,2110);
plane_pos=vec3(1600.0,100.0,-20.0);
//plane_pos=vec3(2100.0,300.0,0.0);
//plane_pos=vec3(4100.0,500.0,0.0);

plane_pos=vec3(32000.0,5800.0,12220.0);// ok
plane_pos=vec3(2600.0,120.0,-20.0);//new





	lookb[selcam]=vec3(400,300,100);
//float1 aa2=70.0*rad;	
float1 aa2=91.0*rad;	
	lookb[selcam]=plane_pos+vec3(-sin(aa2),0.0f,-cos(aa2) )*400.0f;
	upb[selcam]=vec3(0,1,0);

	plane_mat.z=normalize(lookb[selcam]-plane_pos);
	plane_mat.x=cross(plane_mat.z,upb[selcam]);
	plane_mat.y=cross(plane_mat.x,plane_mat.z);
	plane_mat.x=normalize(plane_mat.x);
	plane_mat.y=normalize(plane_mat.y);

	spd=plane_mat.z*210.0*knots/3.6;


//plane_pos=vec3(900.0,120.0,-20.0);spd=plane_mat.z*160.0*knots/3.6;selcammode=0;	trust=0.35;ehook=1;gearneed=1;flapstate=2;speedbrakestate=1; ehelp=0;
plane_pos=vec3(1600.0,430.0/3.0,180.0);spd=plane_mat.z*150.0*knots/3.6;selcammode=0;	trust=0.3;ehook=1;gearneed=1;flapstate=2;speedbrakestate=1; ehelp=0;


//plane_pos=vec3(12000.0,3.0,12220.0);// vertical test
//spd=vec3(0,0,0);

#if 0		
// carrier test
spd=vec3(0,0,0);
plane_pos=vec3(-70.0,23.0,0.0);//new
#endif
		
		lookb[selcam]=vec3(400,300,100);
		physics=1;
	}
	if(key=='t')//trimm
	{
//		joyxb=-1234;		joyyb=-1234;
	}

	if(key=='o') ewind^=1;
	
	if(key=='1')
			trusttime=1.0;
//	if(key=='t')		bumptest^=1;
//	if(key=='u')		ecut^=1;
	if(key=='j')
			ejoy^=1;
	if(key=='m')	{emount^=1;idtabl.dirty();}
	if(key=='c') {selcammode++;}
	if(selcammode>5) selcammode=0;
	
	if(selcammode==0) {selcam=0;selcam3=-1;}  // selcam2=HUD nem itt!
	if(selcammode==1) {selcam=0;selcam3=1;}
	if(selcammode==2) {selcam=1;selcam3=0;}
	if(selcammode==3) {selcam=2;selcam3=0;}
	if(selcammode==4) {selcam=0;selcam3=2;}
	if(selcammode==5) {selcam=0;selcam3=4;}// vertical



	//if(key=='1') {ddx--;printf("%d %d \n",ddx,ddy);}
//	if(key=='2') {ddx++;printf("%d %d \n",ddx,ddy);}
	//if(key=='3') {ddy++;printf("%d %d \n",ddx,ddy);}
//	if(key=='4') {ddy--;printf("%d %d \n",ddx,ddy);}

	if(key=='p') //flydatas aoa
	{
//		printf("%e, %e, %e, \n",dot(plane_mat.z,spd),wing_ang22/rad,acos(dot(plane_mat.z,normalize(spd)))/rad);
		physics^=1;
	}

	/*if(ejoy)
	{
		if(jbutpress0&4) trimm+=0.1;
		if(jbutpress0&8) trimm-=0.1;
	}
	else*/
	{
//		if(key=='\\') trimm+=0.5;
//		if(key=='\'') trimm-=0.5;
	}
	
//	if(ejoy)	trust=(float1)(qwheel)*1.0/50.0;
//	if(ejoy)	trust=(0.55-(float1)(joyy0)*1.0/1000.0)*2.0-1.0;//128
//	if(ejoy)	trust-=0.02*(((float1)(joyy0)*1.0/1000.0));

	if(key=='q') trust+=0.02;//power
	if(key=='a') trust-=0.02;
	trust=saturate(trust);

	if(key=='\'') uuu+=1;
	if(key==';') uuu-=1;

	whlbreak=0;
	if(key=='w') whlbreak=1;
	if(key==' ')  autopilot_on^=1;
	if(key=='h')  ehook^=1;

//	if(key=='[') vectortrust_on^=1;//TODO
	if(key==']') verticalmode^=1;
/*
	if(key==']') 
	{
		etrim^=1;
//		trim_dir=normalize(spd);
//		trim_dir=normalize(plane_mat.z);
		trim_dir=normalize(plane_mat.y);
	}*/

	if(key=='k')  
	{
		planetype++;
		if(planetype>3) planetype=1;
		
		initplaneobj();
	}


	if(gearneed!=2)
	{
	if(gearneed==gear)
		if(key=='g') {gearneed^=1;}

	if(gearneed!=gear)//	 //geartimer 3
	{
		if(gearneed==0)//ehuz,  fel 3 ig
		{
			if(geartimer<3.0) geartimer+=xtime;
			else
			{
				gear=gearneed;
				geartimer=3.0;
			}
		}
		else//kienged, alapallapot 0
		{
			if(geartimer>0.0) geartimer-=xtime;
			else
			{
				gear=gearneed;
				geartimer=0.0;
			}
		}
	}
	}

	if(key=='i') selils++;
	selils&=3;

	
	if(tkey['z']) sidewing-=0.05;//0.1
	else
	if(tkey['x']) sidewing+=0.05;
	else
		sidewing+=(0.0-sidewing)*10.0*xtime;
	
	if(sidewing> 1.0) sidewing=1.0;
	if(sidewing<-1.0) sidewing=-1.0;
	
	if(key==65470) ehelp^=1; // f1
	
/*	if(key=='v') flapwing-=0.1;
	if(key=='f') flapwing+=0.1;
	flapwing=saturate(flapwing);*/
	if(key=='f') {flapstate++;flapstate%=3;}
	switch(flapstate)
	{
	case 0: 
		if(flapwing>0.0) 
		{
			flapwing-=0.3*xtime;
			if(flapwing<0.0) flapwing=0.0;
		}
		break;
	case 1: 
		if(flapwing!=0.5) 
		{
			if(flapwing<0.5) flapwing+=0.3*xtime;
			if(flapwing>0.5) flapwing-=0.3*xtime;

			if(flapwing>0.49)
			if(flapwing<0.51) flapwing=0.5;
		}
		break;
	case 2: 
		if(flapwing<1.0) 
		{
			flapwing+=0.3*xtime;
			if(flapwing>1.0) flapwing=1.0;
		}
		break;
	}

/*	if(key=='n') speedbrake-=0.1;
	if(key=='b') speedbrake+=0.1;
	speedbrake=saturate(speedbrake);*/
	if(key=='b') {speedbrakestate++;speedbrakestate%=3;}
	switch(speedbrakestate)
	{
	case 0: 
		if(speedbrake>0.0) 
		{
			speedbrake-=0.3*xtime;
			if(speedbrake<0.0) speedbrake=0.0;
		}
		break;
	case 1: 
		if(speedbrake!=0.5) 
		{
			if(speedbrake<0.5) speedbrake+=0.3*xtime;
			if(speedbrake>0.5) speedbrake-=0.3*xtime;

			if(speedbrake>0.49)
			if(speedbrake<0.51) speedbrake=0.5;
		}
		break;
	case 2: 
		if(speedbrake<1.0) 
		{
			speedbrake+=0.3*xtime;
			if(speedbrake>1.0) speedbrake=1.0;
		}
		break;
	}
	
//	if(key=='m') {ewater^=1;idtabl.dirty();}
	if(key=='t') vectortrust_on^=1;

	if(key=='l') enight^=1;
	if(enight) nightvalue=0.001;
	else nightvalue=1.0;

	if(key=='5') selcamplane^=1;//	if(selcamplane>5) selcamplane=1;
	if(key=='2') {selenemy++;if(selenemy>4) selenemy=1;}//circle

	
//	if(key==']') selsky++;	if(key=='[') selsky--;

	if(key==65360) mapzoom+=1.0;//home + end
	if(key==65367) mapzoom-=1.0;
	if(mapzoom<1.0) mapzoom=1.0;
	if(mapzoom>10.0 ) mapzoom=10.0;

#if 0
//autopilot
if(key=='7') trg_hg+=100.0;
if(key=='6') trg_hg-=100.0;
if(key=='9') trg_dir+=5.0;
if(key=='8') trg_dir-=5.0;
#else
//center of gravity /weigth center/ trim
#if 0
if(key=='6') 	loopd(i,0,mesh->pnts.size(),1)	mesh->pnts[i].x-=0.1;// - elorebb  
if(key=='7') 	loopd(i,0,mesh->pnts.size(),1)	mesh->pnts[i].x+=0.1;// + hatrebb 
if(key=='8') 	loopd(i,0,mesh->pnts.size(),1)	mesh->pnts[i].z+=0.1;// + lejjebb   
if(key=='9') 	loopd(i,0,mesh->pnts.size(),1)	mesh->pnts[i].z-=0.1;// - feljebb 
#endif
#endif


	if(mbut[1])//shoot
	{
		if(ammo>0)
		{
			addShoot(plane_pos,plane_mat.z,spd,laserID);
			ammo-=1;
		}
	}
	if((plane_pos.y*3.0)<64.0)// carrier top
	if(length(spd)<	40.0*knots/3.6)
	{
		ammo=1000;
		fuel=6000.0;
	}
	



//look aroundy
/*	if(key==65430) rotaxis(head_mat,head_mat.y,-10.0*rad);//le  y
	if(key==65432) rotaxis(head_mat,head_mat.y, 10.0*rad);//ri
	if(key==65431) rotaxis(head_mat,head_mat.z,-10.0*rad);//up x
	if(key==65433) rotaxis(head_mat,head_mat.z, 10.0*rad);//dw
	*/
	if(key=='[') 
	{
		verticalheadmode^=1;
		
		head_mat[selcam].z=vec3(0,0,1);// reset
		head_mat[selcam].y=vec3(0,1,0);
		head_mat[selcam].x=vec3(1,0,0);

		if(verticalheadmode)
		{
			rotaxis(head_mat[selcam],head_mat[selcam].x,-20.0*rad);// lefele nez
		}
	}
	
	if(verticalheadmode)
	{
	}
	else
	{
	
	if(mbut[3]) 
	{
		int h=30;
		int dx=(mx-xx22);
		int dy=(my-yy22);
		if(dx>0) 
		{
			dx-=h;
			if(dx<0) dx=0;
		}
		else
		{
			dx+=h;
			if(dx>0) dx=0;
		}
		if(dy>0) 
		{
			dy-=h;
			if(dy<0) dy=0;
		}
		else
		{
			dy+=h;
			if(dy>0) dy=0;
		}
		rotaxis(head_mat[selcam],head_mat[selcam].y,-dx*0.0001);
		rotaxis(head_mat[selcam],head_mat[selcam].x,-dy*0.0001);
	}
	else
	if(selcam!=1)
	{
		head_mat[selcam].z=vec3(0,0,1);// reset
		head_mat[selcam].y=vec3(0,1,0);
		head_mat[selcam].x=vec3(1,0,0);
	}
	}
	
	
		
	if(key==65437) //center
	{
		head_mat[selcam].z=vec3(0,0,1);// reset
		head_mat[selcam].y=vec3(0,1,0);
		head_mat[selcam].x=vec3(1,0,0);
	}
//	if(key) printf("%d \n",key);//debug
	key=0;
	}
	else
	{
		autopilot_on=1;
	}

	if(autopilot_on)
	{
		pilottime+=xtime;
#ifdef _SHOOT_TEST			
		if(pilottime>60.0)//10 60
#else
		if(pilottime>10.0)//10 60
#endif		
		{
			trg_hg+=(-300.0+frnd(900.0));//300 600   felfele menjen
			if(trg_hg<6000.0) trg_hg=6000.0;
			if(trg_hg>18000.0) trg_hg=18000.0;
			trg_dir=frnd(360.0);
			
			pilottime=0;
		}
	}
}

axis getheadaxis(int i)
{
QDUMPFNC();
	axis camt;
	camt.x=plane_mat.x*head_mat[i].x.x +plane_mat.y*head_mat[i].x.y +plane_mat.z*head_mat[i].x.z;
	camt.y=plane_mat.x*head_mat[i].y.x +plane_mat.y*head_mat[i].y.y +plane_mat.z*head_mat[i].y.z;
	camt.z=plane_mat.x*head_mat[i].z.x +plane_mat.y*head_mat[i].z.y +plane_mat.z*head_mat[i].z.z;
	return camt;
}

void chkShoot(float1 dtime )
{
	double t1=q_getcpuclock();

	for(int i=0;i<shoots.size();i++)
	{
		if(shoots[i].laserID!=laserID) // sajat ne
		{
#if 0		
			vec3 dx=shoots[i].pos-plane_pos;
			if(length(dx)<5.0) alive=false;
#else			
			vec3 deep;
			vec3 normal,obj;
			int indx=0;
			int hit=mesh->getcollision4(shoots[i].poslast,shoots[i].pos,deep,normal,obj,indx);//getcollision2
			if(hit==1) // ground(2) ne! BUG
			{
				alive=false;
			
				int f1=mesh->faces[indx];
				int f2=mesh->faces[indx+1];
				int f3=mesh->faces[indx+2];
		
//				mesh->pnts[f1]+=normalize(shoots[i].pos-shoots[i].poslast)*0.5;// 50cm serules
//				mesh->pnts[f2]=mesh->pnts[f1];
//				mesh->pnts[f3]=mesh->pnts[f1];
			}
#endif			
		}
	}
	t1=q_getcpuclock()-t1;	timedump+=t1; // 4/plane = 1.6ms   0.6ms !!!!!!
//	printf("%d \n",shoots.size());// 300 sem lassit jelentosen!!!!!!!
}

//void draw(float1 xtime,int dat,int op){	draw_forces(dat);//selcam}
void recvcmd(s_cmd *cmd)
{
QDUMPFNC();
qassert(cmd);
qassert(this);
//printf("%s %s %e\n",cmd->s[0],cmd->s[1],cmd->f[0]);
	if(strcmp(cmd->s[0],"draw")==0)	draw_forces(cmd->i[0]);
	if(strcmp(cmd->s[0],"update")==0)	update(cmd->f[0]);
	if(strcmp(cmd->s[0],"transform")==0)	{pretransform(cmd->f[0]);pretransformB(cmd->f[0]);}
	if(strcmp(cmd->s[0],"shoot")==0)	chkShoot(cmd->f[0]);
}

};


#endif

