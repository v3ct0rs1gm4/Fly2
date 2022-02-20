/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//--------------------------------------------------------------------------------

//plane struct    
//script


//WARNINGS
// ha nincs depth buffer de a shader irni akar bele, akkor setshader error invalid enum
// glEnable(GL_DEPTH  NA !!!




	


//--------------------------------------------------------------------------------
#include "q_fly.h"
#include "q_airplane.h"



//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
float1 skypowerval=0.7;
//int selsky=31;//9;//13;//6; 13 9  11  9.7.6.5   6  31
int selsky=31;// 31; 2 5
//2 5 6  11?13  15??
//float1 nightvalue=0.001;
float1 nightvalue=1.0;
#ifdef _USEWATER
int ewater=1;//1
#else
int ewater=0;//1
#endif
int enight=0;



vec3 lookb[16],upb[16],cam_eyeb[16];
axis cam_axs[16];
qmat cam_matb[16];
float1 cam_alfa[16];
float1 cam_zoom[16]={2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};


int selcammode=1;//0
int selcam=0;
int selcam2=0;//3;//HUD
int selcam3=0;//kicsi
float delta_center;
int ehelp=0,ewind=0;


//asefile f18ase,carrierASE;
objfile carrierOBJ,f18obj,su35obj,f16obj; // f18obj minden repcsihez kulon,itt preload

//--------------------------------------------------------------------------------
//float1 zoom=yy*2;
int xx22,yy22;
vec3 camera_trg;



int conv3d(vec3 v1,int &xc,int &yc)
{
	vec3 w1=v1-cam_eyeb[selcam2];
	
	float1 x=dot(w1,cam_axs[selcam2].x);
	float1 y=dot(w1,cam_axs[selcam2].y);
	float1 z=dot(w1,cam_axs[selcam2].z);
	if(z<0.0) return 0;

	x=xx22+ x*cam_zoom[selcam2]*yy/z;
	y=yy22- y*cam_zoom[selcam2]*yy/z;

	xc=(int)x;
	yc=(int)y;

	if(xc<0) return 0;
	if(yc<0) return 0;
	if(xc>=xx) return 0;
	if(yc>=yy) return 0;
	
	return 1;
}

//--------------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
s_texture *tx40=NULL;
s_texture *tx50=NULL;

int physics=1;
int selils=0;
int ecut=0;
int bumptest=0;
#ifdef _DRAWMOUNT
int emount=1;
#else
int emount=0;//1
#endif

float1 mapzoom=4.0;
vec3 gforcecolor;







vec3 wind;
vec3 carrier_pos,carrier_spd;
vec3 carrier_min,carrier_max;
axis carrier_ax;
vec3 carrier_drawpos;


vec3 dumpvec1,dumpvec2,dumpvec3;


vec3 clip1,clip2,clip3,clip4;
int ccc;
//void draw_forces(int scam);

float1 slowtime=1.0;
//int ddx=21,ddy=-368;
//int ddx=-280,ddy=-1180;//+-5?
int ddx=0,ddy=00;

float1 waterlevel=0.0;
int facecc=0;
vec3 dcloud(100,100,0);


GLint gscene2[32];
GLint gterrain[16];
vec3 gcol;








float1 getcarrierhigh(vec3 p1)
{
	float1 h=0.0;//ground
	if(p1.x>=carrier_min.x)
	if(p1.x<=carrier_max.x)
	if(p1.z>=carrier_min.z)
	if(p1.z<=carrier_max.z) h=20.0;//carrier

	return h;
}





//--------------------------------------------------------------------------------


class c_scene:public component //<c_scene>
{
public:
	c_scene(component *_root)//_base
	{
		return;
	}

	void update(float1 xtime) 
	{
QDUMPFNC();
		s_cmd tmp;
		tmp.f[0]=xtime;

		gametime+=xtime;
//		update_childs(xtime);//		selairplane->update(xtime);
		sendcmd(tmp.init("update","terrain"));//carrier update!
		sendcmd(tmp.init("transform","airplane"));

		qevent(0);
		//?qsleep(0.0005);//idtabl.dirty();


		qclear(5);		

		sendcmd(tmp.init("update","camera"));//camera_setup(xtime);
		sendcmd(tmp.init("draw","terrain"));

		drawShoots(xtime);
		drawPath(xtime);
		sendcmd(tmp.init("shoot","airplane"));

		selairplane->drawhud(xtime);


		sendcmd(tmp.init("update","airplane"));

			
//		draw_forces();
	   	glViewport(0,0, xx, yy);
		qflush();

//		qsleep(1.0/60.0);
	}
/*	void draw(float1 xtime,int dat,int op) 
	{
		draw_childs(xtime,dat);
	}*/
};




void drawbox(vec3 p1,vec3 size)
{
	vec3 tx1(0,0,0);
	vec3 tx2(1,0,0);
	vec3 tx3(1,1,0);
	vec3 tx4(0,1,0);
	vec3 v1=p1;
	vec3 v2=p1;
	vec3 v3=p1;
	vec3 v4=p1;
	
	v2.x+=size.x;
	v3.x+=size.x;
	v3.z+=size.z;
	v4.z+=size.z;
	
	vec3 v1b=v1;
	vec3 v2b=v2;
	vec3 v3b=v3;
	vec3 v4b=v4;
	v1b.y+=size.y;
	v2b.y+=size.y;
	v3b.y+=size.y;
	v4b.y+=size.y;

	glMultiTexCoord2fv(0,&tx1.x);	   glVertex3fv(&v1.x);
	glMultiTexCoord2fv(0,&tx2.x);	   glVertex3fv(&v2.x);
	glMultiTexCoord2fv(0,&tx3.x);	   glVertex3fv(&v2b.x);
	glMultiTexCoord2fv(0,&tx4.x);	   glVertex3fv(&v1b.x);

	glMultiTexCoord2fv(0,&tx1.x);	   glVertex3fv(&v2.x);
	glMultiTexCoord2fv(0,&tx2.x);	   glVertex3fv(&v3.x);
	glMultiTexCoord2fv(0,&tx3.x);	   glVertex3fv(&v3b.x);
	glMultiTexCoord2fv(0,&tx4.x);	   glVertex3fv(&v2b.x);

	glMultiTexCoord2fv(0,&tx1.x);	   glVertex3fv(&v3.x);
	glMultiTexCoord2fv(0,&tx2.x);	   glVertex3fv(&v4.x);
	glMultiTexCoord2fv(0,&tx3.x);	   glVertex3fv(&v4b.x);
	glMultiTexCoord2fv(0,&tx4.x);	   glVertex3fv(&v3b.x);

	glMultiTexCoord2fv(0,&tx1.x);	   glVertex3fv(&v4.x);
	glMultiTexCoord2fv(0,&tx2.x);	   glVertex3fv(&v1.x);
	glMultiTexCoord2fv(0,&tx3.x);	   glVertex3fv(&v1b.x);
	glMultiTexCoord2fv(0,&tx4.x);	   glVertex3fv(&v4b.x);
}

void drawbox2(vec3 p1,vec3 size)
{
	
	vec3 tx1(1,1,0);
	vec3 tx2(0,1,0);
	vec3 tx3(0,0,0);
	vec3 tx4(1,0,0);
	vec3 v1=p1;
	vec3 v2=p1;
	vec3 v3=p1;
	vec3 v4=p1;
	
	v2.x+=size.x;
	v3.x+=size.x;
	v3.z+=size.z;
	v4.z+=size.z;
	
	vec3 v1b=v1;
	vec3 v2b=v2;
	vec3 v3b=v3;
	vec3 v4b=v4;
	v1b.y+=size.y;
	v2b.y+=size.y;
	v3b.y+=size.y;
	v4b.y+=size.y;

	glMultiTexCoord2fv(0,&tx1.x);	   glVertex3fv(&v1.x);
	glMultiTexCoord2fv(0,&tx2.x);	   glVertex3fv(&v2.x);
	glMultiTexCoord2fv(0,&tx2.x);	   glVertex3fv(&v2b.x);
	glMultiTexCoord2fv(0,&tx1.x);	   glVertex3fv(&v1b.x);

	glMultiTexCoord2fv(0,&tx2.x);	   glVertex3fv(&v2.x);
	glMultiTexCoord2fv(0,&tx3.x);	   glVertex3fv(&v3.x);
	glMultiTexCoord2fv(0,&tx3.x);	   glVertex3fv(&v3b.x);
	glMultiTexCoord2fv(0,&tx2.x);	   glVertex3fv(&v2b.x);

	glMultiTexCoord2fv(0,&tx3.x);	   glVertex3fv(&v3.x);
	glMultiTexCoord2fv(0,&tx4.x);	   glVertex3fv(&v4.x);
	glMultiTexCoord2fv(0,&tx4.x);	   glVertex3fv(&v4b.x);
	glMultiTexCoord2fv(0,&tx3.x);	   glVertex3fv(&v3b.x);

	glMultiTexCoord2fv(0,&tx4.x);	   glVertex3fv(&v4.x);
	glMultiTexCoord2fv(0,&tx1.x);	   glVertex3fv(&v1.x);
	glMultiTexCoord2fv(0,&tx1.x);	   glVertex3fv(&v1b.x);
	glMultiTexCoord2fv(0,&tx4.x);	   glVertex3fv(&v4b.x);

	glMultiTexCoord2fv(0,&tx1.x);	   glVertex3fv(&v1b.x);
	glMultiTexCoord2fv(0,&tx2.x);	   glVertex3fv(&v2b.x);
	glMultiTexCoord2fv(0,&tx3.x);	   glVertex3fv(&v3b.x);
	glMultiTexCoord2fv(0,&tx4.x);	   glVertex3fv(&v4b.x);
}



void genskycr()
{
QDUMPFNC();
//return;
	gscene2[1]=glGenLists(1);
	glNewList(gscene2[1],GL_COMPILE);
	
		glBegin2(GL_QUADS);//GL_TRIANGLES);	//glBegin2(GL_QUADS);
		srand(55);
		loopd(i,0,500,1)//200 2000
			drawbox(vec3(rand()%5000,0,rand()%5000),vec3(30+rand()%10,30+rand()%120,20+rand()%20));
		glEnd2();
		
	glEndList();

}


//--------------------------------------------------------------------------------

	

class c_terrain:public component //<c_terrain>
{
public:
component *scene;//<c_scene>
c_terrain(component *_root)//_base
{
	scene=_root;
//	scene=static_cast<component<c_scene> *>(_root);
		return;

}

int getz(int x,int y)
{
	if(x<0) x=-x;
	if(y<0) y=-y;
	x%=700;
	y%=352;
x&=1023;
y&=1023;
	
//	return buf[0][(y*700+x)*3];
return 0;
}



float1 gethig(vec3 v1)
{
	int x=(int)(v1.x/10.0);
	int y=(int)(v1.z/10.0);
//	tx40=gettexture("tx40");//test
	
	if(x<0) x=-x;
	if(y<0) y=-y;
	x%=tx40->xh;
	y%=tx40->yh;
	return mapv[y*tx40->xh+x].y*2.0;
}
#ifdef __NEWMOUNT
int getheight(int x,int y)
{
	return (int)tx40->tgabuf[(y)*tx40->scan +x*tx40->ch+3];//ocean
}

vec3 getv(int x,int y,vec3 *txc)//new radar height map
{
	vec3 w1;
	if(x<0) return w1;
	if(y<0) return w1;
	if(x>=tx40->xh) return w1;
	if(y>=tx40->yh) return w1;

	int hg=getheight(x,y);

	vec3 obj=vec3(
		-((float1)(x-tx40->xh/2)*20.0f),   
		(float1)hg*1.0f-1.0,
		-((float1)(y-tx40->yh/2)*20.0f) );
		
//shader mappol
//	txc->x=-obj.x/2048.0/200.0;	txc->y=-(obj.z)/2048.0/200.0;	txc->z=0;

	return obj;
}
#else
int getheight(int x,int y)
{
/*	return buf[40][((yh[40]-1-y)*(xh[40])+ (xh[40]-1-x) )*ch[40]];*/
//	return tx40->tgabuf[(y)*tx40->scan +x*tx40->ch];//normal

	return tx40->tgabuf[(y)*tx40->scan +x*tx40->ch]-15;//ocean  
}
vec3 getv(int x,int y,vec3 *txc)//old OK gmap
{
	vec3 w1;
	if(x<0) return w1;
	if(y<0) return w1;
	if(x>=tx40->xh) return w1;
	if(y>=tx40->yh) return w1;

	int hg=getheight(x,y);
/*
	vec3 sc;
	sc.x=104.0;//100px=10km 1px=100m   m-hez megcsinalva a bump3! 104!
	sc.y=1.5*sc.x;
	vec3 obj=vec3((float1)(x+ddx-xh[40]/2)*sc.x,(float1)buf[40][m1]*8.0f*2.0f,(float1)(y+ddy-yh[40]/2)*sc.y);
*/


//iiddee
	float1 basex=0,basey=0,sc_pix=0;
	vec3 sc;
	sc_pix=256.0*ratio/xmap;//1pixel = ?meter  256=9lev 
//printf("%e %e %d\n",sc_pix,ratio,xmap);	
	if(sellevel[0]=='l')
	{
		basex=1.91000e+02;//-xh[40]/2;//LA
		basey=-3.3000e+01;
		sc.x= 7.660018e-01;//   ratio = texture wide in meter   512=10 level
		sc.y=1.24*sc.x;
	}

	if(sellevel[0]=='i')
	{
		basex=5.510e+02;//ib
		basey=-6.051e+02;
		sc.x=3.060e+0;//   ratio = texture wide in meter   512=10 level
		sc.y=1.45*sc.x;
	}
	if(sellevel[0]=='m')
	{
		basex=4.900e+01;//m
		basey=1.906e+03;
		sc.x= 1.501 ;
		sc.y=1.45*sc.x;
	}

	

	vec3 obj=vec3(
		-((float1)(x-tx40->xh/2)*sc.x*2.0f +basex)*sc_pix,   
		(float1)hg*8.0f*4.0f,//8*256 = 2km
		-((float1)(y-tx40->yh/2)*sc.y*2.0f +basey)*sc_pix );
obj.y-=770.0;//inv waterlevel!


	if(bumptest)
	{
		txc->x=(float1)x/(float1)tx40->xh;//1-  map test
		txc->y=(float1)y/(float1)tx40->yh;//1-
		txc->z=0;
	}
	else
	{
		txc->x=-obj.x/(ratio*256.0) + 0.5;//ok
		txc->y=-(obj.z)/(ratio*256.0) + 0.5;
		txc->z=0;
	}
//txc->x=gcol.x;txc->y=gcol.y;txc->z=gcol.z;//test
	
	return obj;//max 2 km
//	return vec3((float1)x*sc,0.0f,(float1)y*sc);
}
#endif


int chkfrust(vec3 v1,vec3 v2,vec3 v3,vec3 v4,int op)
{
	if(op)
	{
		v1.y+=4000.0;
		v2.y+=4000.0;
		v3.y+=4000.0;
		v4.y+=4000.0;
	}
	else 
	{
		v1.y=0;
		v2.y=0;
		v3.y=0;
		v4.y=0;
	}
	vec3 w1=v1-cam_eyeb[selcam];
	vec3 w2=v2-cam_eyeb[selcam];
	vec3 w3=v3-cam_eyeb[selcam];
	vec3 w4=v4-cam_eyeb[selcam];


 	int a1=0,a2=0,a3=0,a4=0;
 	
	if(dot(w1,clip1)>0.0) a1|=1;
	if(dot(w1,clip2)>0.0) a1|=2;
	if(dot(w1,clip3)>0.0) a1|=4;
	if(dot(w1,clip4)>0.0) a1|=8;

	if(dot(w2,clip1)>0.0) a2|=1;
	if(dot(w2,clip2)>0.0) a2|=2;
	if(dot(w2,clip3)>0.0) a2|=4;
	if(dot(w2,clip4)>0.0) a2|=8;

	if(dot(w3,clip1)>0.0) a3|=1;
	if(dot(w3,clip2)>0.0) a3|=2;
	if(dot(w3,clip3)>0.0) a3|=4;
	if(dot(w3,clip4)>0.0) a3|=8;

	if(dot(w4,clip1)>0.0) a4|=1;
	if(dot(w4,clip2)>0.0) a4|=2;
	if(dot(w4,clip3)>0.0) a4|=4;
	if(dot(w4,clip4)>0.0) a4|=8;

	if((a1&1)==0) 	if((a2&1)==0) 	if((a3&1)==0) 	if((a4&1)==0) return 0;
	if((a1&2)==0) 	if((a2&2)==0) 	if((a3&2)==0) 	if((a4&2)==0) return 0;
	if((a1&4)==0) 	if((a2&4)==0) 	if((a3&4)==0) 	if((a4&4)==0) return 0;
	if((a1&8)==0) 	if((a2&8)==0) 	if((a3&8)==0) 	if((a4&8)==0) return 0;

	if((a1==15)) return 1;
	if((a2==15)) return 1;
	if((a3==15)) return 1;
	if((a4==15)) return 1;

	if((a1&1))	if((a2&1)==0) return 1;
	if((a2&1))	if((a1&1)==0) return 1;
	
	if((a2&1))	if((a3&1)==0) return 1;
	if((a3&1))	if((a2&1)==0) return 1;
	
	if((a3&1))	if((a4&1)==0) return 1;
	if((a4&1))	if((a3&1)==0) return 1;
	
	if((a4&1))	if((a1&1)==0) return 1;
	if((a1&1))	if((a4&1)==0) return 1;
	
	return 0;
}

void drawASE(int selcam5)
{
QDUMPFNC();
	vec3 resol(xx,yy,0);
	setshader("shader14");//9? 4?

	shader_setparm3("mat1",&cam_matb[selcam5],16);//uniform
	shader_setparm3("resol",&resol,2);
	shader_setparm3("gforcecolor",&gforcecolor,2);
	shader_setparm3("ratio",&ratio,1);//tx wide in meter
	shader_setparm3("eye",&cam_eyeb[selcam5],3);
	shader_setparm3("camx",&cam_axs[selcam5].x,3);//uniform	
	shader_setparm3("camy",&cam_axs[selcam5].y,3);	
	shader_setparm3("camz",&cam_axs[selcam5].z,3);
	shader_setparm3("nightvalue",&nightvalue,1);
	

	settx(0,"tx49");
//	settx(1,"tx49");
	settx(1,"skytx");
	
//	f18ase.draw(vec3(0,20,0));
//	carrierASE.draw(vec3(0,20,0));

}

void drawOBJ(int selcam5) // test
{
QDUMPFNC();
	vec3 resol(xx,yy,0);
	setshader("shader4");//9?

	shader_setparm3("mat1",&cam_matb[selcam5],16);//uniform
	shader_setparm3("resol",&resol,2);
	shader_setparm3("gforcecolor",&gforcecolor,2);
	shader_setparm3("ratio",&ratio,1);//tx wide in meter
	shader_setparm3("eye",&cam_eyeb[selcam5],3);
	shader_setparm3("camx",&cam_axs[selcam5].x,3);//uniform	
	shader_setparm3("camy",&cam_axs[selcam5].y,3);	
	shader_setparm3("camz",&cam_axs[selcam5].z,3);
	shader_setparm3("nightvalue",&nightvalue,1);
	

	settx(0,"txnimitzx");
	settx(1,"skytx");
	
	carrierOBJ.draw(vec3(0,-7,0),0); //  20
	
//	settx(0,"f18aobj");
//	f18obj.draw(vec3(0,27,0)); //  20

}

void updatecarrier(float1 dtime )
{
QDUMPFNC();
	carrier_min=carrier_pos-vec3(240.0,0.0,35.0);
	carrier_max=carrier_min+vec3(300.0,20.0,70.0);
	carrier_ax.x=vec3(-1,0,0);
	carrier_ax.y=vec3( 0,0,-1);
	carrier_ax.z=vec3( 0,1,0);
if(ewind==0)rotay(carrier_ax,1.0*sin(gametime*M_PI/6.0));//hullamik
else rotay(carrier_ax,2.0*sin(gametime*M_PI/6.0));//hullamik VIHAR


	carrier_drawpos=carrier_max-vec3(45.0,20.0+6.2+1.0,35.0);//  45 6.2

	carriermesh.trans(carrier_drawpos,carrier_ax);

	axis4 ax4;// !!!!!!!!!!!!!! KELL mindennek ami utkozik
	copyaxis4(ax4,carrier_ax);
	ax4.w=carrier_drawpos;	ax4.w.w=1.0;
	carriermesh.tramat=ax4;//main trans mat
	carriermesh.itramat=invmatrix(ax4);


	static int onceB=1;
	if(onceB==1) 
	{
		carriermesh.chk_normal_occlusion_trn();//trafo utan    igy OK
		onceB=0;
	}

	carriermesh.invalidate_kdtree();//	carriermesh.update_kdtree();
}
void drawcarrier(int selcam5,int mir=0)
{
if(ewater==0) return;
QDUMPFNC();
	vec3 resol(xx,yy,0);
	setshader("shader4");//9?

	shader_setparm3("mat1",&cam_matb[selcam5],16);//uniform
	shader_setparm3("resol",&resol,2);
	shader_setparm3("gforcecolor",&gforcecolor,2);
	shader_setparm3("ratio",&ratio,1);//tx wide in meter
	shader_setparm3("eye",&cam_eyeb[selcam5],3);
	shader_setparm3("camx",&cam_axs[selcam5].x,3);//uniform	
	shader_setparm3("camy",&cam_axs[selcam5].y,3);	
	shader_setparm3("camz",&cam_axs[selcam5].z,3);
	shader_setparm3("nightvalue",&nightvalue,1);


	settx(0,"tx49");
//	settx(1,"tx49");
	settx(1,"skytx");
	
	carriermesh.draw(carrier_drawpos,carrier_ax,mir);
	
#if 0
	settx(0,"tx47");
	settx(1,"tx47");
	glBegin2(GL_QUADS);//GL_TRIANGLES);	//glBegin2(GL_QUADS);
		drawbox2(carrier_min,vec3(300.0,20.0,70.0));//250
	glEnd2();
#endif	

}
void drawskycr()
{
if(emount==0) return;
QDUMPFNC();

	vec3 resol(xx,yy,0);
	setshader("shader4");//9?

	shader_setparm3("mat1",&cam_matb[selcam],16);//uniform
	shader_setparm3("resol",&resol,2);
	shader_setparm3("gforcecolor",&gforcecolor,2);
	shader_setparm3("ratio",&ratio,1);//tx wide in meter
	shader_setparm3("eye",&cam_eyeb[selcam],3);
	shader_setparm3("camx",&cam_axs[selcam].x,3);//uniform	
	shader_setparm3("camy",&cam_axs[selcam].y,3);	
	shader_setparm3("camz",&cam_axs[selcam].z,3);
	shader_setparm3("nightvalue",&nightvalue,1);
	

	settx(0,"tx46");
	settx(1,"tx46");


//	glCallList(gscene2[1]);
}


void drawmount()
{
QDUMPFNC();
//return;
	vec3 resol(xx,yy,0);

/*	vec3 camz=cam2.z;
	vec3 camx=cam2.x;
	camz.y=0.0;	camz=normalize(camz);
	camx.y=0.0;	camx=normalize(camx);
*/
	float1 z=1.0/tan(cam_alfa[selcam]*rad);//2.5f
	//z*=1.5;
	clip1=cam_axs[selcam].z+cam_axs[selcam].x*z;	//clipping
	clip2=cam_axs[selcam].z-cam_axs[selcam].x*z;	
	clip3=cam_axs[selcam].z+cam_axs[selcam].y*z;	
	clip4=cam_axs[selcam].z-cam_axs[selcam].y*z;	
	clip1=normalize(clip1);
	clip2=normalize(clip2);
	clip3=normalize(clip3);
	clip4=normalize(clip4);




//	glFrontFace(GL_CW);	glCullFace(GL_FRONT_AND_BACK);
	
	glDisable (GL_BLEND);
//	glEnable(GL_DEPTH);

if(emount==0) return;
#ifndef _DRAWMOUNT
return;
#endif

	if(bumptest)
		setshader("shader4");//test
	else	
#ifdef __NEWMOUNT
		setshader("shader9b");//TODO  9!
#else
		setshader("shader9");
#endif

	shader_setparm3("mat1",&cam_matb[selcam],16);//uniform
	shader_setparm3("resol",&resol,2);
	shader_setparm3("gforcecolor",&gforcecolor,2);
	shader_setparm3("ratio",&ratio,1);//tx wide in meter
	shader_setparm3("eye",&cam_eyeb[selcam],3);
	shader_setparm3("camx",&cam_axs[selcam].x,3);//uniform
	shader_setparm3("camy",&cam_axs[selcam].y,3);
	shader_setparm3("camz",&cam_axs[selcam].z,3);
	shader_setparm3("nightvalue",&nightvalue,1);
	
#ifdef _USEGMAP
//TODO
	if(bumptest)
	{
	settx(0,"tx40");//height
	settx(1,"tx9");//41 decal
	}
	else
	{
#ifdef __NEWMOUNT
	settx(0,"tx40");//height  
	settx(1,"tx55");//rock
#else	
	settx(0,"tx1");
	settx(1,"tx2");
	settx(2,"tx3");
	settx(3,"tx4");
	settx(4,"tx5");
	settx(5,"tx6");
	settx(6,"tx7");
	settx(7,"tx8");
	settx(8,"tx9");
	settx(9,"tx10");
#endif	
	}
#endif

	float1 h=10000;
	facecc=0;
	

	
	vec3 uv=vec3(tx40->xh,1,tx40->yh);
	vec3 tx1,tx2,tx3,tx4,tx5;

//srand(55);
int q=16;//16-8
	int s=q;//4-8
	if(gterrain[1]==0) gterrain[1]=glGenLists(1);
	glNewList(gterrain[1],GL_COMPILE);
	glBegin2(GL_TRIANGLES);	//glBegin2(GL_QUADS);

	
	loopd(y,s,tx40->yh-s*2,s)
	{
		loopd(x,s,tx40->xh-s*2,s)
//		if(getheight(x,y)>0)//csak a hegy   BUG
		{
			vec3 v1=getv(x  ,y,&tx1);
			vec3 v2=getv(x+s,y,&tx1);
			vec3 v3=getv(x+s,y+s,&tx1);
			vec3 v4=getv(x  ,y+s,&tx1);

#ifdef __NEWMOUNT
			if(length2(v1-cam_eyeb[selcam])<5200.0*5200.0)		
#else
			if(length2(v1-cam_eyeb[selcam])<120000.0*120000.0)		
#endif			
			if(v1.y>waterlevel+0.0 || //+20
 			   v2.y>waterlevel+0.0 ||
			   v3.y>waterlevel+0.0 ||
			   v4.y>waterlevel+0.0)
			if(chkfrust(v1,v2,v3,v4,0))//||		   			chkfrust(v1,v2,v3,v4,1))
			{
//				int lod=(int)(length(v1-cam_eyeb[selcam])/7000.0);//3-7 6000
#ifdef __NEWMOUNT
				int lod=(int)(length(v1-cam_eyeb[selcam])/5000.0);//3-7 6000
#else
				int lod=(int)(length(v1-cam_eyeb[selcam])/12000.0);//3-7 6000  gmap OK TODO
#endif
				int st=1+lod/2;
				int lodtype=(lod&1)^1;
lodtype=0;
//lodtype=0;st=1;	
	
				if(lodtype)				
				{
				loopd(y2,0,q+1,st)
				if((y2+y+st)<tx40->yh)
				loopd(x2,0,q+1,st)
				if((x2+x+st)<tx40->xh)
				{
					int x3=x+x2;
					int y3=y+y2;
					vec3 v1=getv(x3   ,y3,&tx1);
					vec3 v2=getv(x3+st,y3,&tx2);
					vec3 v3=getv(x3+st,y3+st,&tx3);
					vec3 v4=getv(x3   ,y3+st,&tx4);
					vec3 v5=getv(x3+st/2 ,y3+st/2,&tx5);
//gcol.x=frnd(1.0);gcol.y=frnd(1.0);gcol.z=frnd(1.0);//test
	
					glMultiTexCoord2fv(0,&tx1.x);	   glVertex3fv(&v1.x);
					glMultiTexCoord2fv(0,&tx2.x);	   glVertex3fv(&v2.x);
					glMultiTexCoord2fv(0,&tx5.x);	   glVertex3fv(&v5.x);

					glMultiTexCoord2fv(0,&tx2.x);	   glVertex3fv(&v2.x);
					glMultiTexCoord2fv(0,&tx3.x);	   glVertex3fv(&v3.x);
					glMultiTexCoord2fv(0,&tx5.x);	   glVertex3fv(&v5.x);

					glMultiTexCoord2fv(0,&tx3.x);	   glVertex3fv(&v3.x);
					glMultiTexCoord2fv(0,&tx4.x);	   glVertex3fv(&v4.x);
					glMultiTexCoord2fv(0,&tx5.x);	   glVertex3fv(&v5.x);

					glMultiTexCoord2fv(0,&tx4.x);	   glVertex3fv(&v4.x);
					glMultiTexCoord2fv(0,&tx1.x);	   glVertex3fv(&v1.x);
					glMultiTexCoord2fv(0,&tx5.x);	   glVertex3fv(&v5.x);
					facecc+=4;
				}
				}
				else
				{
				loopd(y2,0,q+1,st)
				if((y2+y+st)<tx40->yh)
				loopd(x2,0,q+1,st)
				if((x2+x+st)<tx40->xh)
				{
					int x3=x+x2;
					int y3=y+y2;
					vec3 v1=getv(x3   ,y3,&tx1);
					vec3 v2=getv(x3+st,y3,&tx2);
					vec3 v3=getv(x3+st,y3+st,&tx3);
					vec3 v4=getv(x3   ,y3+st,&tx4);

					glMultiTexCoord2fv(0,&tx1.x);	   glVertex3fv(&v1.x);
					glMultiTexCoord2fv(0,&tx2.x);	   glVertex3fv(&v2.x);
					glMultiTexCoord2fv(0,&tx3.x);	   glVertex3fv(&v3.x);

					glMultiTexCoord2fv(0,&tx1.x);	   glVertex3fv(&v1.x);
					glMultiTexCoord2fv(0,&tx3.x);	   glVertex3fv(&v3.x);
					glMultiTexCoord2fv(0,&tx4.x);	   glVertex3fv(&v4.x);
					facecc+=2;
				}
				}
			}
		}
	}

	glEnd2();
	glEndList();

	glCallList(gterrain[1]);//	glDelList( gterrain[1]);
	
//printf("%d \n",cc);
}


float getheight2(int x,int y,int h)
{
	x=x*tx50->xh/h;
	y=y*tx50->yh/h;
			
	return 0.12*(float)tx50->tgabuf[(y)*tx50->scan +x*tx50->ch+0];//hegy
}

void drawmount2()
{
QDUMPFNC();
//return;
	vec3 resol(xx,yy,0);

/*	vec3 camz=cam2.z;
	vec3 camx=cam2.x;
	camz.y=0.0;	camz=normalize(camz);
	camx.y=0.0;	camx=normalize(camx);
*/
	float1 z=1.0/tan(cam_alfa[selcam]*rad);//2.5f
	//z*=1.5;
	clip1=cam_axs[selcam].z+cam_axs[selcam].x*z;	//clipping
	clip2=cam_axs[selcam].z-cam_axs[selcam].x*z;	
	clip3=cam_axs[selcam].z+cam_axs[selcam].y*z;	
	clip4=cam_axs[selcam].z-cam_axs[selcam].y*z;	
	clip1=normalize(clip1);
	clip2=normalize(clip2);
	clip3=normalize(clip3);
	clip4=normalize(clip4);




//	glFrontFace(GL_CW);	glCullFace(GL_FRONT_AND_BACK);
	
	glEnable (GL_DEPTH_TEST);
	glDisable (GL_BLEND);
//	glEnable(GL_DEPTH);


	setshader("shader8"); // 9/9b ?  8

	shader_setparm3("mat1",&cam_matb[selcam],16);//uniform
	shader_setparm3("resol",&resol,2);
	shader_setparm3("gforcecolor",&gforcecolor,2);
	shader_setparm3("ratio",&ratio,1);//tx wide in meter
	shader_setparm3("eye",&cam_eyeb[selcam],3);
	shader_setparm3("camx",&cam_axs[selcam].x,3);//uniform
	shader_setparm3("camy",&cam_axs[selcam].y,3);
	shader_setparm3("camz",&cam_axs[selcam].z,3);
	shader_setparm3("nightvalue",&nightvalue,1);
	

	settx(0,"mount1");//height  
	settx(1,"mount1bm");//rock



	
//	vec3 uv=vec3(tx40->xh,1,tx40->yh);

	static int once5=1;
	
	if(once5)
	{
		once5=0;
//srand(55);
	int s=1;//4-8
	if(gterrain[2]==0) gterrain[2]=glGenLists(1);
	glNewList(gterrain[2],GL_COMPILE);
	glBegin2(GL_TRIANGLES);	//glBegin2(GL_QUADS);

	
	float scl=30.0;// ~ 8x8km  5x5
	vec3 base(8000.0,0.0,-8000.0);
	
	loopd(y,s,300+s,s)
	loopd(x,s,300+s,s)
	{
			vec3 v1=vec3(x  ,0,y);
			vec3 v2=vec3(x+s,0,y);
			vec3 v3=vec3(x+s,0,y+s);
			vec3 v4=vec3(x  ,0,y+s);

			v1.y=getheight2(x  ,y,300);
			v2.y=getheight2(x+s,y,300);
			v3.y=getheight2(x+s,y+s,300);
			v4.y=getheight2(x  ,y+s,300);

			vec3 tx1=vec3(x  ,y,0)/300.0;
			vec3 tx2=vec3(x+s,y,0)/300.0;
			vec3 tx3=vec3(x+s,y+s,0)/300.0;
			vec3 tx4=vec3(x  ,y+s,0)/300.0;
			v1*=scl;
			v2*=scl;
			v3*=scl;
			v4*=scl;
			v1+=base;
			v2+=base;
			v3+=base;
			v4+=base;
			
			glMultiTexCoord2fv(0,&tx1.x);	   glVertex3fv(&v1.x);
			glMultiTexCoord2fv(0,&tx2.x);	   glVertex3fv(&v2.x);
			glMultiTexCoord2fv(0,&tx3.x);	   glVertex3fv(&v3.x);

			glMultiTexCoord2fv(0,&tx1.x);	   glVertex3fv(&v1.x);
			glMultiTexCoord2fv(0,&tx3.x);	   glVertex3fv(&v3.x);
			glMultiTexCoord2fv(0,&tx4.x);	   glVertex3fv(&v4.x);

	}

	glEnd2();
	glEndList();
	}
	
	glCallList(gterrain[2]);//	glDelList( gterrain[1]);
	
//printf("%d \n",cc);
}
void draw_felho()
{
return;
	vec3 resol(xx,yy,0);

	//glDisable(GL_DEPTH);
		glDisable (GL_DEPTH_TEST);//!!!!!!!!!!1

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
	//glBlendFunc (GL_ONE,GL_ONE);
//	glBlendFunc (GL_ONE,GL_ONE_MINUS_SRC_COLOR );
//	glBlendFunc (GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR );

	setshader("shader8");
	shader_setparm3("mat1",&cam_matb[selcam],16);//uniform
	shader_setparm3("eye",&cam_eyeb[selcam],3);
	shader_setparm3("resol",&resol,2);
	shader_setparm3("eye",&cam_eyeb[selcam],3);
	shader_setparm3("camx",&cam_axs[selcam].x,3);//uniform
	shader_setparm3("camy",&cam_axs[selcam].y,3);
	shader_setparm3("camz",&cam_axs[selcam].z,3);

	settx(0,"tx46");


	vec3 camx=cam_axs[selcam].x;
	vec3 camy=cam_axs[selcam].y;


	vec3 w2;
	glBegin2(GL_QUADS);
	glMultiTexCoord4f(1,0.7,0.9,1.0,1.0);			
	loopd(k,0,5,1)
	loopd(j,0,10,1)
	loopd(i,0,10,1)
	{
		vec3 w1=vec3(i*40.0,2000.0+k*10.0,j*40.0);
		float1 xb=0.25*(i/4);
		float1 yb=0.25*(i%4);
		yb=fmod(yb,1.0);
	
		float1 h=25.0;
		vec3 v1=w1-camx*h-camy*h;	
		vec3 v2=w1+camx*h-camy*h;	
		vec3 v3=w1+camx*h+camy*h;	
		vec3 v4=w1-camx*h+camy*h;

		glMultiTexCoord2f(0,0.0 ,yb+0.0 );	glVertex3fv(&v1.x);
		glMultiTexCoord2f(0,0.0 ,yb+0.25);	glVertex3fv(&v2.x);
		glMultiTexCoord2f(0,1.0 ,yb+0.25);	glVertex3fv(&v3.x);
		glMultiTexCoord2f(0,1.0 ,yb+0.0 );	glVertex3fv(&v4.x);

	}
	glEnd2();
	glDisable (GL_BLEND);
	//glEnable(GL_DEPTH);	
	glEnable (GL_DEPTH_TEST);//!!!!!!!!!!1

}
//drot landing kotel
void draw_hookkabel(int scam)
{
if(ewater==0) return;
QDUMPFNC();
	vec3 resol(xx,yy,0);
	//glDisable(GL_DEPTH);
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

//	float1 alf=69.5*rad;
	float1 alf=90*rad;
	vec3 w1[4],w2[4],w3[4],
		irx=vec3( sin(alf),0.0f,cos(alf)),
		iry=vec3(-cos(alf),0.0f,sin(alf)),
		irz(0.0,0.2,0.0);
	glBegin2(GL_LINES);
	glMultiTexCoord4f(1,1.0,1.0,0.0,1.0);


	vec3 base=carrier_pos;
	base.y=20.2;
#if 0	
	w1[0]=irx*-10.0  + iry*15.0 +irz+base;
	w3[0]=irx*-10.0  - iry*15.0 +irz+base;
	w1[1]=irx*0.0  + iry*15.0 +irz+base;
	w3[1]=irx*0.0  - iry*15.0 +irz+base;
	w1[2]=irx*10.0 + iry*15.0 +irz+base;
	w3[2]=irx*10.0 - iry*15.0 +irz+base;
	w2[0]=(w1[0]+w3[0])*0.5;
	w2[1]=(w1[1]+w3[1])*0.5;
	w2[2]=(w1[2]+w3[2])*0.5;
#endif
	w1[0]=carriermesh.pnts_trn[65];
	w3[0]=carriermesh.pnts_trn[66];
	w1[1]=carriermesh.pnts_trn[67];
	w3[1]=carriermesh.pnts_trn[68];
	w1[2]=carriermesh.pnts_trn[69];
	w3[2]=carriermesh.pnts_trn[70];
	w1[3]=carriermesh.pnts_trn[71];
	w3[3]=carriermesh.pnts_trn[72];
	w2[0]=(w1[0]+w3[0])*0.5;
	w2[1]=(w1[1]+w3[1])*0.5;
	w2[2]=(w1[2]+w3[2])*0.5;
	w2[3]=(w1[3]+w3[3])*0.5;

	
	if(selairplane->hookconnect) 
	{
		w2[selairplane->hookcable-1]=selairplane->hookend;//pnt;
	}
	if(selairplane->ehook)
	{
		glVertex3f(selairplane->hookforce.x,selairplane->hookforce.y,selairplane->hookforce.z);
		glVertex3f(selairplane->hookend.x,selairplane->hookend.y,selairplane->hookend.z);
	}


	glVertex3f(w1[0].x,w1[0].y,w1[0].z);
	glVertex3f(w2[0].x,w2[0].y,w2[0].z);
	glVertex3f(w2[0].x,w2[0].y,w2[0].z);
	glVertex3f(w3[0].x,w3[0].y,w3[0].z);
	
	glVertex3f(w1[1].x,w1[1].y,w1[1].z);
	glVertex3f(w2[1].x,w2[1].y,w2[1].z);
	glVertex3f(w2[1].x,w2[1].y,w2[1].z);
	glVertex3f(w3[1].x,w3[1].y,w3[1].z);

	glVertex3f(w1[2].x,w1[2].y,w1[2].z);
	glVertex3f(w2[2].x,w2[2].y,w2[2].z);
	glVertex3f(w2[2].x,w2[2].y,w2[2].z);
	glVertex3f(w3[2].x,w3[2].y,w3[2].z);

	glVertex3f(w1[3].x,w1[3].y,w1[3].z);
	glVertex3f(w2[3].x,w2[3].y,w2[3].z);
	glVertex3f(w2[3].x,w2[3].y,w2[3].z);
	glVertex3f(w3[3].x,w3[3].y,w3[3].z);

#if 0
//dump
	glVertex3f(dumpvec1.x,dumpvec1.y,dumpvec1.z);
	glVertex3f(dumpvec2.x,dumpvec2.y,dumpvec2.z);
	glVertex3f(dumpvec1.x,dumpvec1.y,dumpvec1.z);
	glVertex3f(dumpvec3.x,dumpvec3.y,dumpvec3.z);
#endif

	glEnd2();
	//glEnable(GL_DEPTH);
			glEnable (GL_DEPTH_TEST);
}


void drawterrain(float1 xtime)
{
#if 0
//	glEnable(GL_DEPTH);	//NA!!!!!!!!!!!!
glEnable (GL_DEPTH_TEST);//!!!!!!!!!!1  nincs a kurvaanyad
	////sglDisable(GL_DEPTH);	glDisable (GL_DEPTH_TEST);
	drawcarrier(selcam);
//	draw_hookkabel(selcam);
return;
#endif

QDUMPFNC();
	vec3 resol(xx,yy,0);
 glViewport(0, 0, xx, yy);
 
//iiddee
	//glEnable(GL_DEPTH);	
	glEnable (GL_DEPTH_TEST);//!!!!!!!!!!1
//	glDisable(GL_DEPTH);	glDisable (GL_DEPTH_TEST);
	
	glEnable(GL_TEXTURE_2D);
	glDisable (GL_BLEND);
	glBlendFunc (GL_ONE, GL_ONE);


//reflect of carrier 
	setrtg("rt_reflect");
	glxclearzbuf();
	qclear(5);		
	drawcarrier(selcam,1);//mirror
	setrtg("display");


#ifndef _LODMAP

	if(selsky<1) selsky=1;	if(selsky>20) selsky=20;
	skypowerval=0.9;
//	if(selsky==6) skypowerval=0.7;




	if(ewater==0)
	{
#ifdef _GMAP	
		if(emount)	setshader("shader11");//3 tx only
		else		setshader("shader1");
#else	
		setshader("shader3");//1
#endif
	}


	if(nightvalue>0.1)
	{
	if(ewater)
	{
#ifdef	FASTWATER
	if(ewater) setshader("shader1w2");
#else
//	if(ewater) setshader("shader1w");
	if(ewater) setshader("shader1w3");// mixed desert and sea
#endif
	}
	shader_setparm3("camx",&cam_axs[selcam].x,3);//uniform
	shader_setparm3("camy",&cam_axs[selcam].y,3);
	shader_setparm3("camz",&cam_axs[selcam].z,3);
	shader_setparm3("eye",&cam_eyeb[selcam],3);
	shader_setparm3("zoom",&cam_zoom[selcam],1);
	shader_setparm3("resol",&resol,2);
	shader_setparm3("ratio",&ratio,1);//tx wide in meter
	shader_setparm3("waterlevel",&waterlevel,1);
	shader_setparm3("gforcecolor",&gforcecolor,2);
	shader_setparm3("skypowerval",&skypowerval,1);
	shader_setparm3("mat1",&cam_matb[selcam],16);//uniform
	shader_setparm3("dcloud",&dcloud,2);
	shader_setparm3("nightvalue",&nightvalue,1);
	shader_setparm3("time",&gametime,1);
	
	dcloud.x+=xtime*9.0;// m/s
	
#ifdef _USEGMAP
//TODO
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
#endif
		
#ifdef	FASTWATER
	settx(0,"skytx");///15+selsky
#else
	settx(10,"skytx");///15+selsky
#endif
	settx(11,"tx56");//england bump
//TODO	settx(11,"tx46");//felhokarc
	
	
	
	settx(12,"rt_reflect0");//carrier reflect

#if 1
//terrain & sky
	glBegin2(GL_QUADS);
		glMultiTexCoord2f(0,0.0,0.0);	 		glVertex3f(-1.0,-1.0 , 0.0);
		glMultiTexCoord2f(0,0.99999,0.0);		glVertex3f( 1.0,-1.0 , 0.0);
		glMultiTexCoord2f(0,0.99999,0.99999);	glVertex3f( 1.0, 1.0 , 0.0);
		glMultiTexCoord2f(0,0.0,0.99999);		glVertex3f(-1.0, 1.0 , 0.0);
	glEnd2();
#else	
	glBegin2(GL_TRIANGLES);
		glMultiTexCoord2f(0,0.0,0.0);	 		glVertex3f(-1.0,-1.0 , 0.0);
		glMultiTexCoord2f(0,0.99999,0.0);		glVertex3f( 1.0,-1.0 , 0.0);
		glMultiTexCoord2f(0,0.99999,0.99999);	glVertex3f( 1.0, 1.0 , 0.0);
		
		glMultiTexCoord2f(0,0.0,0.0);	 		glVertex3f(-1.0,-1.0 , 0.0);
		glMultiTexCoord2f(0,0.99999,0.99999);	glVertex3f( 1.0, 1.0 , 0.0);
		glMultiTexCoord2f(0,0.0,0.99999);		glVertex3f(-1.0, 1.0 , 0.0);
	glEnd2();
#endif
	}


	//glEnable(GL_DEPTH);		glEnable (GL_DEPTH_TEST);
	
	drawcarrier(selcam);
//	drawOBJ(selcam);
	draw_hookkabel(selcam);
#ifdef _USEGMAP	
	drawmount();
//	drawskycr();
	draw_felho();
#endif	


//	drawmount2();

	//if(selcam!=0) 	
	s_cmd tmp;	tmp.i[0]=selcam;
	scene->sendcmd(tmp.init("draw","airplane"));//	scene.draw(0.0,selcam);//selairplane->draw_forces(selcam);
	
	//glDisable(GL_DEPTH);	glDisable (GL_DEPTH_TEST);







//kis felso-belso camera
#ifndef _NEWPHY2

	if(selcam3!=-1)//nem OFF
	{
	int hh=30;
     glViewport(xx/2+hh, yy/2+hh, xx/2-hh*2, yy/2-hh*2);
	
	glxclearzbuf();
	
	if(nightvalue>0.1)
	{

	setshader("shader1");
#ifdef	FASTWATER
if(ewater) setshader("shader1w2");	
#else
//if(ewater) setshader("shader1w");	
if(ewater) setshader("shader1w3");	
#endif

	shader_setparm3("camx",&cam_axs[selcam3].x,3);//uniform
	shader_setparm3("camy",&cam_axs[selcam3].y,3);
	shader_setparm3("camz",&cam_axs[selcam3].z,3);
	shader_setparm3("eye",&cam_eyeb[selcam3],3);
	shader_setparm3("zoom",&cam_zoom[selcam3],1);
	shader_setparm3("resol",&resol,2);
	shader_setparm3("ratio",&ratio,1);//tx wide in meter
	shader_setparm3("waterlevel",&waterlevel,1);
	shader_setparm3("gforcecolor",&gforcecolor,2);
	shader_setparm3("skypowerval",&skypowerval,1);
	shader_setparm3("mat1",&cam_matb[selcam3],16);//uniform
	shader_setparm3("nightvalue",&nightvalue,1);

#ifdef _USEGMAP
//TODO
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
#endif

#ifdef	FASTWATER
	settx(0,"skytx");//15+selsky
#else	
	settx(10,"skytx");//15+selsky
#endif	
	settx(11,"tx56");//england bump

	float1 xb=0.75,yb=0.6;//0+-1.0
	float1 h=0.4,h2=0.4;
	glBegin2(GL_QUADS);
		glMultiTexCoord2f(0,0.0,0.0);	   glVertex3f(-1.0,-1.0 , 0.0);
		glMultiTexCoord2f(0,0.9999,0.0);	   glVertex3f( 1.0,-1.0 , 0.0);
		glMultiTexCoord2f(0,0.9999,0.9999);	   glVertex3f( 1.0, 1.0 , 0.0);
		glMultiTexCoord2f(0,0.0,0.9999);	   glVertex3f(-1.0, 1.0 , 0.0);
/*		glMultiTexCoord2f(0,0.0,0.0);	   glVertex3f(-h+xb,-h2+yb , 0.0);
		glMultiTexCoord2f(0,0.9999,0.0);	   glVertex3f( h+xb,-h2+yb , 0.0);
		glMultiTexCoord2f(0,0.9999,0.9999);	   glVertex3f( h+xb, h2+yb , 0.0);
		glMultiTexCoord2f(0,0.0,0.9999);	   glVertex3f(-h+xb, h2+yb , 0.0);*/
	glEnd2();
	}

	//if(selcam3!=0) 	
	s_cmd tmp;	tmp.i[0]=selcam3;
	scene->sendcmd(tmp.init("draw","airplane"));//	scene.draw(0.0,selcam3);//selairplane->draw_forces(selcam3);
	
	drawcarrier(selcam3);
	draw_hookkabel(selcam3);

	if(selcam3==4)// vertical
	{
		selcam2=4;// default 0, de itt 4 kell
		selairplane->drawhud(xtime);
		selcam2=0;
	}

	
     glViewport(0, 0, xx, yy);
	}
#endif




#if 0
// zoom target area
//glDisable(GL_DEPTH);	glDisable (GL_DEPTH_TEST);
	glxclearzbuf();
//camera
	setshader("shader1");//reset!
if(ewater) setshader("shader1w");	
#if 0
//TODO
	settx(0,"tx1");
	settx(1,"tx2");
#endif	
	{
	vec3 resol2(256,256,0);
	float1 ratio2=1.0;
	float1 zoom3=40.0;//20
	axis cam4;
	cam4.z=camera_trg-cam_eyeb[0];
	cam4.y=cam_axs[0].y;
	cam4.x=cross(cam4.z,cam4.y);
	cam4.y=cross(cam4.x,cam4.z);//re

	cam4.x=normalize(cam4.x);
	cam4.y=normalize(cam4.y);
	cam4.z=normalize(cam4.z);

	shader_setparm3("camx",&cam4.x,3);//uniform
	shader_setparm3("camy",&cam4.y,3);
	shader_setparm3("camz",&cam4.z,3);
	shader_setparm3("eye",&cam_eyeb[0],3);
	shader_setparm3("zoom",&zoom3,1);
	shader_setparm3("resol",&resol2,2);
	shader_setparm3("ratio",&ratio,1);//tx wide in meter
	shader_setparm3("waterlevel",&waterlevel,1);
	shader_setparm3("gforcecolor",&gforcecolor,2);
	shader_setparm3("skypowerval",&skypowerval,1);
	shader_setparm3("mat1",&cam_matb[0],16);//uniform
	shader_setparm3("nightvalue",&nightvalue,1);

	float1 xb=0.75,yb=-0.7;
	float1 h=0.2,h2=0.25;
	glBegin2(GL_QUADS);
		glMultiTexCoord2f(0,0.9999,0.0);	   glVertex3f(-h+xb,-h2+yb , 0.0);
		glMultiTexCoord2f(0,0.0,0.0);	   glVertex3f( h+xb,-h2+yb , 0.0);
		glMultiTexCoord2f(0,0.0,0.9999);	   glVertex3f( h+xb, h2+yb , 0.0);
		glMultiTexCoord2f(0,0.9999,0.9999);	   glVertex3f(-h+xb, h2+yb , 0.0);
	glEnd2();
	}
#endif




//map  terkep	
#if 1
//TODO
	glDisable (GL_DEPTH_TEST);

	setshader("shader2");
	vec3 axx,axy=selairplane->plane_mat.z;	axy.y=axy.z;axy.z=0.0; axy=normalize(axy);//2D
	axy.x=-axy.x;//?
	axy.y=-axy.y;
	axx.x= axy.y;//90 fok
	axx.y=-axy.x;

	shader_setparm3("axx",&axx,2);//forditva
	shader_setparm3("axy",&axy,2);//uniform  
	shader_setparm3("eye",&selairplane->plane_pos,3);
	shader_setparm3("resol",&resol,2);
	shader_setparm3("ratio",&ratio,1);
	shader_setparm3("mapzoom",&mapzoom,1);
	shader_setparm3("eye",&cam_eyeb[selcam],3);
	shader_setparm3("camx",&cam_axs[selcam].x,3);//uniform
	shader_setparm3("camy",&cam_axs[selcam].y,3);
	shader_setparm3("camz",&cam_axs[selcam].z,3);


//	settx(1,"tx7");//	shader_setparm3("tx1",6); TODO  6?   old
//	settx(2,"tx56");//england bump

//new	
#ifdef _USEGMAP
//TODO
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
#endif
		
#ifdef	FASTWATER
	settx(0,"skytx");///15+selsky
#else
	settx(10,"skytx");///15+selsky
#endif
	settx(11,"tx56");//england bump
//TODO	settx(11,"tx46");//felhokarc
	

	float1 xb=-0.725,yb=-0.675;//center 0.75,yb=-0.7;    kisebb -x jobbra        xb=-0.725,yb=-0.67
	float1 h=0.17,h2=0.25;//  size    h=0.17,h2=0.25
	glBegin2(GL_QUADS);
		glMultiTexCoord2f(0,0.0,0.0);	   	   glVertex3f(-h+xb,-h2+yb , 0.5);
		glMultiTexCoord2f(0,0.9999,0.0);	   glVertex3f( h+xb,-h2+yb , 0.5);
		glMultiTexCoord2f(0,0.9999,0.9999);	   glVertex3f( h+xb, h2+yb , 0.5);
		glMultiTexCoord2f(0,0.0,0.9999);	   glVertex3f(-h+xb, h2+yb , 0.5);
	glEnd2();
#endif
	
#else
	drawmount();
	
#endif	

//	drawmount2();


		glEnable (GL_DEPTH_TEST);
	
	
//	drawplane(eye+cam2.z*1000.0);
	
#if 0
	loopd(y,-10000,10000,100)
	loopd(x,-10000,10000,100)
	{
		vec3 obj(x,getz(x/100,y/100),y);
		
		int xc,yc;
		if(conv3d(obj,xc,yc))		pixel(xc,yc,0x005500);
	}
	
	
	loopd(y,0,yy,10)
	{
	int xe=-1,ye=0;

	loopd(x,0,xx,10)
	{
		vec3 ray=plane_mat.z*500.0 + plane_mat.x*(x-xx22) + plane_mat.y*(y-yy22);
		ray=normalize(ray);

		if(ray.y<0.0)
		{
			float1 t1=-eye.y/ray.y;
			vec3 obj=eye + ray*t1;
			obj.y=getz(obj.x/10,obj.z/10);

			int xc,yc;
			if(conv3d(obj,xc,yc))
			{
//				pixel(xc,yc,0x005500);
				if(xe!=-1)
					line(xc,yc,xe,ye,0x008800);
				xe=xc; ye=yc;
			}
		}
	}
	}
#endif	
}
public:
void update(float1 xtime)
{
}
void recvcmd(s_cmd *cmd)
{
QDUMPFNC();
//printf("%s %s %e\n",cmd->s[0],cmd->s[1],cmd->f[0]);
	if(strcmp(cmd->s[0],"update")==0)	updatecarrier(cmd->f[0]);
	if(strcmp(cmd->s[0],"draw")==0)		drawterrain(cmd->f[0]);
}

};

//--------------------------------------------------------------------------------


class c_camera:public component //<c_camera>
{
public:
component *scene;//<c_scene>
c_camera(component *_root)//_base
{
	scene=_root;
//	scene=static_cast<component<c_scene> *>(_root);
	return;

}

void update(float1 xtime )
{
}
void recvcmd(s_cmd *cmd)
{
QDUMPFNC();
	if(strcmp(cmd->s[0],"update")==0)	camera_setup(cmd->f[0]);//xtime
}
void camera_setup(float1 xtime)//selcam
{
QDUMPFNC();
	axis camt;
	float1 asp=(float1)xx/(float1)(yy);
	qmat mat2;

	float1 zoom5=1.2;
	if(ejoy==0) zoom5=1.0+(qwheel+2)*0.1;
	if(zoom5<0.4) zoom5=0.4;
	if(zoom5>2.0) zoom5=2.0;
	zoom5*=1.45;//def
	

	loopd(i,0,5,1)// 0123 +1
	{
		c_airplane *camairplane3=camairplane;
		if(camairplane2)
		if(i==4) camairplane3=camairplane2; // 2nd ary

		camt=camairplane3->getheadaxis(i);
	
		delta_center=-5.0;
		rotax(camt,delta_center);//	,-5 fok lefele
	
		cam_axs[i]=camt;
		cam_eyeb[i]=camairplane3->plane_pos;
		cam_alfa[i]=(30.0+2.5)/zoom5;//iiddee  28?    reality 5fok=6cm
		cam_zoom[i]=1.0/tan(30.0*rad)*zoom5;
	
//norm
		if(i==0)
		{
			cam_eyeb[i]=camairplane3->canopy_pos;
			lookb[i]=cam_eyeb[i]+ camt.z*300.0 ;//selairplane->plane_mat.z*300.0;    
/*
//			cam_eyeb[i]+= plane_mat.z*4.0 + plane_mat.y*1.0;//canopy
#ifdef _USE_SU35
			cam_eyeb[i]+= camairplane3->qtransform(airplanemesh[0].pnts[9]);////canopy 7.7,0,1
#else
			cam_eyeb[i]+= camairplane3->qtransform(airplanemesh[0].pnts[111]);////canopy 7.7,0,1
#endif
*/
		}
	
		float1 followmul=20.0;///5.6;
//externalcam
		if(i==1 )
		{
			lookb[i]+=(camairplane3->plane_pos-lookb[i])*xtime*followmul;	
lookb[i]=camairplane3->plane_pos			;
			cam_zoom[i]=2.0*3.5*(zoom5/2.0);	cam_alfa[i]=28.0/3.5/(zoom5/2.0);//2
//			cam_zoom[i]=2.0*0.7;	cam_alfa[i]=28.0/0.7;

			cam_axs[i]=camairplane3->head_mat[i];
			cam_eyeb[i]=lookb[i] -cam_axs[i].z*200.0;//plane_pos    cam_axs miatt!
//cam_eyeb[i]=lookb[i] -cam_axs[i].z*70.0;//kozeli
cam_eyeb[i]=lookb[i] -cam_axs[i].z*120.0;//
		}
		
//groundcontrol
		if(i==2)
		{
			lookb[i]+=(camairplane3->plane_pos-lookb[i])*xtime*followmul;	
			cam_zoom[i]=2.0*5.0;
			cam_alfa[i]=28.0/5.0;
			cam_eyeb[i]=vec3(100,2000/3,100);//lenti
#if 1			
cam_zoom[i]=2.0*1.0;//1 4
cam_alfa[i]=28.0/1.0;
cam_eyeb[i]=vec3(-30,15,15)+carrier_pos+vec3(0,20,0);//tower 30
#endif
			cam_axs[i].z=normalize(lookb[i]-cam_eyeb[i]);
			cam_axs[i].x=cross(vec3(0,1,0),cam_axs[i].z);//LH!
			cam_axs[i].y=cross(cam_axs[i].z,cam_axs[i].x);
		}

//HUD
		if(i==3)
		{
			cam_axs[i]=camairplane3->plane_mat;
			lookb[i]=cam_eyeb[i]+ camairplane3->plane_mat.z*300.0;

			cam_eyeb[i]+= camairplane3->plane_mat.z*4.0 + camairplane3->plane_mat.y*1.0;//canopy
		}
#if 0		
		if(i==4) // vertical cam : modifyed i=0
		{
			vec3 tpos=carriermesh.pnts_trn[74];//carrier_pos
//			lookb[i]=cam_eyeb[i]+ camt.z*300.0 ;//selairplane->plane_mat.z*300.0;    
			lookb[i]+=(tpos-lookb[i])*xtime*followmul;	

/*
		float zoom52=zoom5*2.0;
		cam_alfa[i]=(30.0+2.5)/zoom52;//iiddee  28?    reality 5fok=6cm
		cam_zoom[i]=1.0/tan(30.0*rad)*zoom52;
*/

/*
//			cam_eyeb[i]+= plane_mat.z*4.0 + plane_mat.y*1.0;//canopy
#ifdef _USE_SU35
			cam_eyeb[i]+= camairplane->qtransform(airplanemesh[0].pnts[9]);////canopy 7.7,0,1
#else
			cam_eyeb[i]+= camairplane->qtransform(airplanemesh[0].pnts[111]);////canopy 7.7,0,1
#endif
*/
			cam_eyeb[i]=camairplane->canopy_pos;

			cam_axs[i].z=normalize(lookb[i]-cam_eyeb[i]);
			cam_axs[i].x=cross(vec3(0,1,0),cam_axs[i].z);//LH!
			cam_axs[i].y=cross(cam_axs[i].z,cam_axs[i].x);
		}
#else		
//externalcam2
		if(i==4 )
		{
			lookb[i]+=(camairplane3->plane_pos-lookb[i])*xtime*followmul;	
lookb[i]=camairplane3->plane_pos;
			cam_zoom[i]=2.0*3.5*(10.0*zoom5/2.0);	cam_alfa[i]=28.0/3.5/(10.0*zoom5/2.0);//2
//			cam_zoom[i]=2.0*0.7;	cam_alfa[i]=28.0/0.7;

cam_eyeb[i]=camairplane->plane_pos	;
			cam_axs[i].z=normalize(lookb[i]-cam_eyeb[i]);
			cam_axs[i].x=cross(vec3(0,1,0),cam_axs[i].z);//LH!
			cam_axs[i].y=cross(cam_axs[i].z,cam_axs[i].x);


		}
#endif

	
		upb[i]=cam_axs[i].y;//(0,1,0);
		qMatrixLookAtLH(&cam_matb[i],&cam_eyeb[i],&lookb[i],&upb[i]);//	qMatrixMultiply(&mat1,&mat1,&head_mat);
		qMatrixPerspectiveFovLH(&mat2,cam_alfa[i],asp,5.0,200000.0);//zoom=2  30-2 fok! 
		qMatrixMultiply(&cam_matb[i],&cam_matb[i],&mat2);//qMatrixTranspose(&mat1,&mat1);
	}
}
};

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------



float1 xtimes[20];
int nxtimes=0;





//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------


int main()
{
QDUMPFNC();

preloadlevel(1);
#ifndef FASTWATER
need_glx_versio=3;
#endif


//	if(carrier) h=qload(,shader_source,60000);
	init_system5();
	loadshader("shader0");
	loadshader("shader1");
	loadshader("shader1w");
	loadshader("shader1w2");
	loadshader("shader1w3");
	loadshader("shader2");
	loadshader("shader3");
	loadshader("shader4");
	loadshader("shader4b");
	loadshader("shader5");
	loadshader("shader6");
	loadshader("shader7");
	loadshader("shader8");
	loadshader("shader9");
	loadshader("shader9b");
	loadshader("shader10");
	loadshader("shader11");
	loadshader("shader14");



	carrierOBJ.load_obj("data/carrier4.obj",1.0,0,50000);
	loadbmpb("txnimitzx","data/nimitz1base.bmp",0,1);
	
//	f18obj.load_obj("data/f18a5c.obj",2.0,0,300000);
//	loadbmpb("f18aobj","data/f18a.bmp",0,1);

// _LOADF18OBJ
	loadbmpb("f18objtx","data/f18q.bmp",0,1);
	loadbmpb("su35objtx","data/su35q.bmp",0,1);// TODO
	loadbmpb("f16objtx","data/f16.bmp",0,1);// TODO
//	f18obj.load_obj("data/f18a9.obj",1.8,0,2000000); // q_airplane.cpp

	f18obj.load_obj("data/f18a9.obj",17.0/9.611,0,2000000); // q_airplane.cpp      17m
	su35obj.load_obj("data/su35a.obj",22.0/9.4,0,2000000); // 22m
	f16obj.load_obj("data/f16a.obj",14.5/9.96,0,2000000); // 14.5m 9.5



////	f18ase.load_ase("f18/f18a.ASE",0.001,0,3700000);
//	f18ase.load_ase("f18/f18d.ASE",0.001,0,3700000);
//	carrierASE.load_ase("f18/carrier.ASE",0.001,0,1800000000); // 28sec kilep
	


//glPolygonMode(GL_POLYGON_MODE,GL_LINE);//rndermode
//glRenderMode(GL_LINE);//rndermode



	loadbmpb("tx49","data/nimitz1tx.bmp",0,1);
//	loadbmpb("tx43","data/su35new.bmp",0,1);
	loadbmpb("tx43","data/f18hornet.bmp",0,1);
	airplanemesh[0].loadpnts("data/airplane02.adt","tx43",1);//sel airplane
	airplanemesh[1].loadpnts("data/airplane02.adt","tx43",1);
	airplanemesh[2].loadpnts("data/airplane02.adt","tx43",1);
	airplanemesh[3].loadpnts("data/airplane02.adt","tx43",1);
	airplanemesh[4].loadpnts("data/airplane02.adt","tx43",1);

	carriermesh.loadpnts("data/carrier01.adt","tx49",1,18.5);
//	loadACfile("plane/f14.ac",100.0);



#if 0	
	float1 F=10e3*9.8;
	float1 ro=1.3;
	float1 A=38.0;
	float1 v=62.0;
	float1 c=F/(0.5*A*ro*v*v);
	printf("%e \n",c);
	printf("%e %e\n",v*3.6,v*3.6/knots);
//	v=340.0;//15C	printf("%e %e\n",v*3.6,v*3.6/1.6);
	

	printf(">> %e \n",v*sin(5.0*rad));
	printf(">> %e \n",v*sin(7.0*rad));

	float1 a=9.81;//	s=a/2 t2 ->	t=sqrt(s/(a/2))
	float1 t=sqrt(4.0/(a/2.0));
	v=a*t;
	
	printf(">> %e \n",v);
printf(" %e \n",1.0/tan(5.0*rad));
printf("  %e \n",1.0/tan(10.0*rad));
printf(" %e \n",1.0/tan(15.0*rad));
printf("  %e \n",1.0/tan(20.0*rad));
printf(" %e \n",1.0/tan(25.0*rad));
printf("  %e \n",1.0/tan(30.0*rad));
printf(" %e \n",1.0/tan(35.0*rad));
printf("  %e \n",1.0/tan(40.0*rad));
printf(" %e \n",1.0/tan(45.0*rad));
#endif


#if 0
//	FILE *ff=fopen("data/map01.bmp","rb");int xh=700,yh=352;
//	loadbmp(1,"data/q1.bmp",1131,949);
//	loadbmp(1,"data/q2.bmp",1118,931);
//	loadbmp(1,"data/q3.bmp",1129,933);

	loadbmp(1,"data/q4.bmp",1128,939);
	loadbmp(2,"data/q5.bmp",1131,936);
	loadbmp(3,"data/q6.bmp",1124,937);
	
	
#endif

#ifdef __NEWMOUNT
loadlevel(1,-1,0);
#else
loadlevel(1);//TODO
#endif




#if 0
	loopd(y,0,yh[40],1)
	loopd(x,0,xh[40],1)
	{
		int h=getheight(x,y);
		pixel(x,y,h);
	}
	qflush();	getchar();	exit(3);
#endif

	

//	loopd(i,1,17,1)
	int i=selsky;//13
//i=2;
	{
		char ss[92];
		int q=1;
//		if(i==1) q=0;		if(i==5) q=0;		if(i==13) q=0;		if(i==14) q=0;

		sprintf(ss,"sky/sky%d.bmp",i);
		loadbmpb("skytx",ss,0,q);//15+i
	}
//	loadtga(44,"data/cloud02.tga",0);
	loadbmpb("tx44","data/cloud02.bmp",0);
//	loadtga(46,"data/cumulus01.tga",0);

	loadbmpb("tx45","data/trust01.bmp",0);
//	loadbmpgz("tx45,"data/trust01.gz",0);
//TODO	loadbmpb("tx46","data/skycr.bmp",0);
#ifdef _GMAP	

	genskycr();
//	loadbmp(46,"data/cloud05.bmp",0,0);//3-4-5
#endif

//	loadbmpb("tx55","data/rock.bmp",0);
	loadbmpb("tx55","data/rock02.bmp",0);
	loadbmpb("tx56","data/england.bmp",0);


// mount & bump
//	loadbmpb("mount1","height/mnt5.bmp",0);	loadbmpb("mount1bm","height/mnt5bm.bmp",0);
//	loadbmpb("mount1","height/mount2.bmp",0);	loadbmpb("mount1bm","height/mount2bm.bmp",0);// best
//	loadbmpb("mount1","height/mount3.bmp",0);	loadbmpb("mount1bm","height/mount3bm.bmp",0);



#ifndef _GMAP	
//	loadbmp(43,"sky/cloud2.bmp");
//	loadbmp(40,"data/bump01.bmp");
//	loadbmp(41,"gmaph/l9.bmp");

#if 0
	{
	int tx=41;
	xh[tx]=xh[40];
	yh[tx]=yh[40];
	buf[tx]=new uchar[xh[tx]*yh[tx]*4];
	mapv=new vec3[xh[tx]*yh[tx]];

	float1 ymax=-1e7;
	loopd(y,0,yh[40],1)//terep
	loopd(x,0,xh[40],1)
	{
		int m1=((y  )*xh[40]+(x  ))*3;
		vec3 v1((float1)(x),(float1)buf[40][m1]*0.12f,(float1)(y));//.12 0.08
		mapv[y*xh[40]+x]=v1;
		
		if(v1.y>ymax) ymax=v1.y;
	}
	printf("..%e \n",ymax);//30.0

	vec3 light=normalize(vec3(400,300,-600));//200,300,-600));

	loopd(y,0,yh[40],1)//shadow
	loopd(x,0,xh[40],1)
	{
		int m1=(y*xh[40])+x;
		int ok=1,inshadow3=0;
		vec3 pos=mapv[m1];
		int x3=((int)pos.x);
		int y3=((int)pos.z);
		
		while(ok)
		{
			pos+=light;
			
			int x2=((int)pos.x);
			int y2=((int)pos.z);
			if(x2!=x3 || y2!=y3)
			{
				x3=x2;y3=y2;
				
				if(x2>=0)
				if(y2>=0)
				if(x2<xh[40])
				if(y2<yh[40])
				{
					int m2=(y2*xh[40])+x2;
					if(pos.y<mapv[m2].y) {inshadow3=1;ok=0;}
				}
				
				if(x2<0) ok=0;
				if(y2<0) ok=0;
				if(x2>=xh[40]) ok=0;
				if(y2>=yh[40]) ok=0;
				if(pos.y>ymax) ok=0;
			}
		}
		buf[tx][m1*3]=inshadow3;
	}	
printf("ok\n");
	loopd(y,0,yh[40]-1,1)//normal light
	loopd(x,0,xh[40]-1,1)
	{
		int m1=y*xh[40]+x;
		vec3 v1=mapv[m1];
		vec3 v2=mapv[m1+1];
		vec3 v3=mapv[m1+xh[40]];
		
		vec3 norm=normalize(cross(v3-v1,v2-v1));
		float1 h=v1.y;//+frnd(10.0);
		
		float1 lig=dot(norm,light)*1.2; 
#if 0
		if(lig<0.1) lig=0.1+mapn[m1].y*0.3;
		if(buf[tx][m1]) lig=0.1+mapn[m1].y*0.3;
#else
		if(lig<0.0) lig=0.0;//0.1
		if(buf[tx][m1*3]) lig=0.0;
		lig+= norm.y*0.6;//0.3
#endif

		float1 r1=220,g1=190,b1=40,  r2=120,g2=60,b2=10;
//		int r1=250,g1=240,b1=230,  r2=220,g2=200,b2=180;
		float1 r=r1 + ((r2-r1)*h/(ymax));//46.0
		float1 g=g1 + ((g2-g1)*h/(ymax));
		float1 b=b1 + ((b2-b1)*h/(ymax));
//		if(buf[tx][m1]) 		b+= mapn[m1].y*80.2;
		if(v1.y<=1.0) {r=0;g=30;b=60;}//2
		
		
		r*=lig;		g*=lig;		b*=lig;
//		r=norm.x*255.0;		g=norm.y*255.0;		b=norm.z*255.0;
		if(r>255.0) r=255.0;
		if(g>255.0) g=255.0;
		if(b>255.0) b=255.0;

		m1*=3;
		buf[tx][m1]=(int)r;
		buf[tx][m1+1]=(int)g;
		buf[tx][m1+2]=(int)b;
	}
	textures[tx]=glxtexture(buf[tx],xh[tx],yh[tx]);
	}
#endif
#endif


#ifdef __NEWMOUNT
//new terep
	uchar *mem5=new uchar[2048*2048*4];
	load("data/dati1",mem5,2048*2048*4);
	create_texture("tx40",2048,2048,4,mem5);
#endif

	tx40=gettexture("tx40");//tx40
//	tx50=gettexture("mount1bm");


	component base;//	component<c_scene> scene;
	base.addcomponent<c_scene>("scene1");
	component *scene=base.getcomponent<c_scene>("scene1");
	scene->addcomponent<c_camera>("camera1");
	scene->addcomponent<c_terrain>("terrain1");
	scene->addcomponent<c_airplane>("airplane1");
	scene->addcomponent<c_airplane>("airplane2");
	scene->addcomponent<c_airplane>("airplane3");
	scene->addcomponent<c_airplane>("airplane4");
	scene->addcomponent<c_airplane>("airplane5");
	selairplane=scene->getcomponent<c_airplane>("airplane1");
//selairplane->plane_pos=vec3(2100.0,300.0,-20.0);//TODO
	camairplane=scene->getcomponent<c_airplane>("airplane1");
	
	scene->getcomponent<c_airplane>("airplane1")->addcomponent<c_font>("font");
	scene->getcomponent<c_airplane>("airplane2")->addcomponent<c_font>("font");
	scene->getcomponent<c_airplane>("airplane3")->addcomponent<c_font>("font");
	scene->getcomponent<c_airplane>("airplane4")->addcomponent<c_font>("font");
	scene->getcomponent<c_airplane>("airplane5")->addcomponent<c_font>("font");

	scene->getcomponent<c_airplane>("airplane1")->mesh=&airplanemesh[0];
	scene->getcomponent<c_airplane>("airplane2")->mesh=&airplanemesh[1];
	scene->getcomponent<c_airplane>("airplane3")->mesh=&airplanemesh[2];
	scene->getcomponent<c_airplane>("airplane4")->mesh=&airplanemesh[3];
	scene->getcomponent<c_airplane>("airplane5")->mesh=&airplanemesh[4];
	
	airplanelist.push_back(	scene->getcomponent<c_airplane>("airplane1"));
	airplanelist.push_back(	scene->getcomponent<c_airplane>("airplane2"));
	airplanelist.push_back(	scene->getcomponent<c_airplane>("airplane3"));
	airplanelist.push_back(	scene->getcomponent<c_airplane>("airplane4"));
	airplanelist.push_back(	scene->getcomponent<c_airplane>("airplane5"));
	
	xx22=xx/2;
	yy22=yy/2;//no -100;

	
	create_rtg("rt_reflect",0,1,xx,yy);
	
ejoy=1;	// 0   power only

	xtimer(0);

	while(1)
	{
		float1 xtime=xtimer(0)*slowtime;
		
		scene->update(xtime);

	int a=1;
	if(selcamplane==1) a=selenemy+1;//index!
	if(a==1)	camairplane=scene->getcomponent<c_airplane>("airplane1");
	if(a==2)	camairplane=scene->getcomponent<c_airplane>("airplane2");
	if(a==3)	camairplane=scene->getcomponent<c_airplane>("airplane3");
	if(a==4)	camairplane=scene->getcomponent<c_airplane>("airplane4");
	if(a==5)	camairplane=scene->getcomponent<c_airplane>("airplane5");
	int b=selenemy+1;//index!
	if(b==1)	camairplane2=scene->getcomponent<c_airplane>("airplane1"); // 2nd ary
	if(b==2)	camairplane2=scene->getcomponent<c_airplane>("airplane2");
	if(b==3)	camairplane2=scene->getcomponent<c_airplane>("airplane3");
	if(b==4)	camairplane2=scene->getcomponent<c_airplane>("airplane4");
	if(b==5)	camairplane2=scene->getcomponent<c_airplane>("airplane5");
	}
/*
	loopd(x,0,xx,1)
	{
		float1 aa=x*M_PI*2/xx;
		int y=300-sin(aa)*100.0;
		int y2=300-cos(aa)*100.0;

		pixel(x,y,0x008800);
		pixel(x,y2,0x880000);
	}	
*/
	wait();
//	close(joy_fd);	/* too bad we never get here */
		
	return 0;
}

//--------------------------------------------------------------------------------



