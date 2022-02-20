/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
works:

news:
utkozes ex _mir problem
class rigidbody           a c_mesh egy RB de kell bele a phys mint a sebesseg stb!
tores .. multi mesh / phys
chk_normal_occlusion  torony normal rossz


2197 mir_base ???????

## tengerpart
## terkep
## fuel, loszer/ammo
## whlbreak rossz
## bbox trafo rebuild after rotation
## enemy AI path checkpoint
## collision quad tree, linear map?  3d id
## plane vs plane coll


old:
## fa-fa utkozes

???:
radar height
forma hajtomu modellen 
setup I  GC 
minden f18 fuggoseg filebol  /tx mass .../



3+1  cs + gl,  3 ny2,  4 drt +ny1
*/


// legacy: do not use _USE_SU35  NA  



#include "q_airplane.h"



int uuu=5;
vec3 target;
vec3 mypos;
vec3 enemypos;
vec3 enemyspd;
vec3 enemyacc;
int selenemy=1;

int typecc=0; // 0:f18   1:su35   2:f16
double timedump=0;
double timedump2=0;

class c_airplane;
qvector<c_airplane *> airplanelist;
float1 xq=0.0;///init delta x


c_mesh airplanemesh[64];
c_mesh carriermesh;


class c_airplane;
c_airplane *selairplane=0;
c_airplane *camairplane=0;
c_airplane *camairplane2=0; // secondary cam
int selcamplane=0;



int chkdir(int n)
{
	if(n<0) n=360+n;//-!
	if(n>=360) n-=360;
	
	return n;
}
float1 chkdir2(float1 n)
{
	if(n<0) n=360+n;//-!
	if(n>=360) n-=360;
	
	return n;
}
float1 fsqrabs(float1 n)
{
	return n*fabs(n);
}


void deftrustquad(float1 i,float1 j,vec3 base,vec3 dir)
{
	vec3 v1;
	float1 le=length(dir);
	float1 sp=60.0;//length(spd);
	
	float1 aa=i*M_PI*2.0;
	float1 r=0.49-0.45*j;
	r*=(0.8+0.2*cos(j*M_PI*2.0/0.2-gametime*sp));
	
	glMultiTexCoord2f(0,0.5+r*sin(aa) ,0.5+r*cos(aa) );	

	dir=normalize(dir);
	vec3 diry=dir; 	diry.x=dir.z;diry.z=dir.y;diry.y=dir.x;
	vec3 dirx=cross(dir,diry);
	diry=cross(dirx,dir);
	dirx=cross(dir,diry);
	
	r*=1.1;
	v1=base + dir*j*7.0*le +dirx*cos(aa)*r + diry*sin(aa)*r;
	glVertex3fv(&v1.x);
}



float1 flydatas[]={//spd vs aoa
4.427897e+02, -5.237647e+00, 4.026425e+00, 
4.303172e+02, -5.237647e+00, 4.025842e+00, 
4.048422e+02, -5.221176e+00, 4.012345e+00, 
3.753050e+02, -5.171765e+00, 3.974734e+00, 
3.550713e+02, -5.138823e+00, 3.949230e+00, 
3.355485e+02, -5.105882e+00, 3.923461e+00, 
3.137878e+02, -5.007059e+00, 3.849699e+00, 
2.912117e+02, -4.957647e+00, 3.810280e+00, 
2.616980e+02, -4.809412e+00, 3.697664e+00, 
2.424554e+02, -4.694118e+00, 3.609234e+00, 
2.198672e+02, -4.562353e+00, 3.506657e+00, 
1.981306e+02, -4.348235e+00, 3.341874e+00, 
1.744310e+02, -3.985882e+00, 3.063947e+00, 
1.553113e+02, -3.574118e+00, 2.748799e+00, 
1.354808e+02, -2.997647e+00, 2.305117e+00, 
1.110203e+02, -1.811765e+00, 1.388749e+00, 
9.006915e+01, 1.482353e-01, 1.153499e-01, 
0.006915e+01, 1.482353e-01, 1.153499e-01, //stop!
0.006915e+01, 1.482353e-01, 1.153499e-01, //stop!
0.006915e+01, 1.482353e-01, 1.153499e-01, //stop!
7.479256e+01, 2.552941e+00, 1.993018e+00, 
6.531814e+01, 4.891765e+00, 3.841908e+00,
0.0e+00, 4.891765e+00, 3.841908e+00
};



vec3 transform(vec3 v1,axis4 &ax)
{
	vec4 w4;
	w4=ax.x*v1.x +ax.y*v1.y +ax.z*v1.z +ax.w;

	vec3 w3;
	w3.x=w4.x;
	w3.y=w4.y;
	w3.z=w4.z;
	return w3;
}
vec3 transformNorm(vec3 v1,axis4 &ax)
{
	vec4 w4;
	w4=ax.x*v1.x +ax.y*v1.y +ax.z*v1.z ;

	vec3 w3;
	w3.x=w4.x;
	w3.y=w4.y;
	w3.z=w4.z;
	return w3;
}


float1 cut(float1 n,float1 low,float1 hig)
{
	if(n<low) n=low;
	if(n>hig) n=hig;
	return n;
}




qvector<s_shoot> shoots;
qvector<vec3> lastshoots;



void addShoot(vec3 pos,vec3 dir,vec3 spd,int id)
{
	dir+=vec3(-1.0+frnd(2.0),-1.0+frnd(2.0),-1.0+frnd(2.0))*0.003;
	dir=normalize(dir);
	
	s_shoot tmp;
	tmp.pos=pos;
	tmp.poslast=pos;
	tmp.spd=dir*1000.0+spd;//300 but 1000
	tmp.time=5.0;
	tmp.laserID=id;
	tmp.t2=0;
	
	tmp.pos+=tmp.spd*0.01;
	
	shoots.push_back(tmp);
}

void drawShoots(float1 dtime)
{
	vec3 resol(xx,yy,0);
	int scam=selcam;

	glDisable (GL_DEPTH_TEST);
	setshader("shader10");
	shader_setparm3("mat1",&cam_matb[scam],16);//uniform
	shader_setparm3("eye",&cam_eyeb[scam],3);
	shader_setparm3("resol",&resol,2);
	shader_setparm3("eye",&cam_eyeb[scam],3);
	shader_setparm3("camx",&cam_axs[scam].x,3);//uniform
	shader_setparm3("camy",&cam_axs[scam].y,3);
	shader_setparm3("camz",&cam_axs[scam].z,3);
	
	if(shoots.size()==0)	lastshoots.clear();
	
	float1 t2=0;
	glBegin2(GL_LINES);
	for(int i=0;i<shoots.size();i++)
	{
		vec3 v1=shoots[i].pos;
		vec3 v2=shoots[i].pos + normalize(shoots[i].spd)*15.0;		
		glVertex3f(v1.x,v1.y,v1.z);
		glVertex3f(v2.x,v2.y,v2.z);

		float1 t1=dot(v1-enemypos, normalize(enemypos-mypos));
		if(t1<0.0) glMultiTexCoord4f(1,0.7,0.0,0.0,1.0);			
		else glMultiTexCoord4f(1,1.0,1.0,0.0,1.0);			
		
		if(fsgn(t1)!=fsgn(shoots[i].t2))	lastshoots.push_back(v1);
		shoots[i].t2=t1;
		
		//update
		shoots[i].poslast=shoots[i].pos;
		shoots[i].pos+=shoots[i].spd*dtime;
		shoots[i].spd.y -= 9.8*dtime;//grav
		shoots[i].time-=dtime;
	}
//	while(lastshoots.size()>12) lastshoots.pop();
	
	vec3 v2;
	glMultiTexCoord4f(1,1.0,0.0,1.0,1.0);			
	for(int i=0;i<lastshoots.size();i++)
	{
		vec3 v1=lastshoots[i];
		if(i)
		{
			glVertex3f(v1.x,v1.y,v1.z);
			glVertex3f(v2.x,v2.y,v2.z);
		}
		v2=v1;
	}
	glEnd2();
	

	for(int i=0;i<shoots.size();i++)
	{
		if(shoots[i].time<0.0 || shoots[i].pos.y<-0.0)
		{
			shoots[i]=shoots.back();
			shoots.pop_back();
			break;
		}
	}
}	
	
	




void drawBraket(vec3 p1, vec3 v2)
{
	vec3 p3=p1+ v2*15.0;// right braket
	vec3 p2=p1+ v2*5.0;
	vec3 p4=p2; p4.y+=7.0; //  <>
	vec3 p5=p2; p5.y-=7.0;

	glMultiTexCoord4f(1,1.0,0.0,0.2,1.0);			
	glVertex3f(p3.x,p3.y,p3.z);
	glVertex3f(p4.x,p4.y,p4.z);
	glVertex3f(p3.x,p3.y,p3.z);
	glVertex3f(p5.x,p5.y,p5.z);
	
	p3=p1- v2*15.0;// left braket
	p2=p1- v2*5.0;
	p4=p2; p4.y+=7.0; //  <>
	p5=p2; p5.y-=7.0;

	glVertex3f(p3.x,p3.y,p3.z);
	glVertex3f(p4.x,p4.y,p4.z);
	glVertex3f(p3.x,p3.y,p3.z);
	glVertex3f(p5.x,p5.y,p5.z);
}
void drawPath(float1 dtime)
{
	vec3 resol(xx,yy,0);
	int scam=selcam;

	glDisable (GL_DEPTH_TEST);
	setshader("shader10");
	shader_setparm3("mat1",&cam_matb[scam],16);//uniform
	shader_setparm3("eye",&cam_eyeb[scam],3);
	shader_setparm3("resol",&resol,2);
	shader_setparm3("eye",&cam_eyeb[scam],3);
	shader_setparm3("camx",&cam_axs[scam].x,3);//uniform
	shader_setparm3("camy",&cam_axs[scam].y,3);
	shader_setparm3("camz",&cam_axs[scam].z,3);
	
	
	vec3 tpos=carriermesh.pnts_trn[74];//carrier_pos
	vec3 p1=  tpos;//carrier_pos
	axis ax;
	ax.x=normalize(carrier_spd);// tangent
	ax.y=vec3(0,1,0);
	ax.z=cross(ax.x,ax.y);// right
	
	//p1.y+=620.0/3.0;// 600 lb  + deck   v1.0
//	p1 -=ax.z*200.0;

	p1.y+=10.0;// deck       v2.0
	ax.x=-ax.x;
	
	float q=20.0;
	glBegin2(GL_LINES);


	rotay(ax,-8.5);
	for(int i=0;i<10;i++) // glide slope
	{
		drawBraket(p1,ax.z);
		p1+=ax.x*1000.0*mph/10.0; // 1m slope
		p1.y+=(400.0/3.0)/10.0;
	}
	for(int i=0;i<20;i++)// last curve
	{
		drawBraket(p1,ax.z);
	
		p1+=ax.x*790.0*mph*M_PI/q;  //  1.5m      200m kisebb radius  690        753/9
		rotay(ax,188.5/q);
	}
	for(int i=0;i<6;i++)// last line
	{
		drawBraket(p1,ax.z);
	
		p1+=ax.x*2300.0*mph/5.0; // ~ 400m ++ 100m
		p1.y+=(200.0/3.0)/6.0; // 600 rol 400 ra esik
	}
	for(int i=0;i<20;i++)// first curve
	{
		drawBraket(p1,ax.z);
	
		p1+=ax.x*750.0*mph*M_PI/q;  //  1.5merfold az atmero
		rotay(ax,180.0/q);
	}
	for(int i=0;i<6;i++)
	{
		drawBraket(p1,ax.z);
	
		p1+=ax.x*1500.0*mph/5.0; // ~ 400m
	}

	p1-=tpos;// check
//printf("%e %e \n",p1.x,p1.z);	

	glEnd2();

}	
	
	
	
	
	
	

