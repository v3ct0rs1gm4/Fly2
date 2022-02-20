/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "q_font.h"

float1 fontscale5=2.0;
float1 fontsize5=1.0;




#ifdef _FLIGHTSYM
int c_font::conv3dchk2(vec3 v1,int &xc,int &yc)
{
//	vec3 w1=v1-eye;
	
	float1 x=dot(v1,cam_axs[selcam2].x);
	float1 y=dot(v1,cam_axs[selcam2].y);
	float1 z=dot(v1,cam_axs[selcam2].z);
	if(z==0.0) z=1e-6;

	x=xx22+ x*(cam_zoom[selcam2]*yy)/fabs(z);
	y=yy22- y*(cam_zoom[selcam2]*yy)/fabs(z);

	xc=(int)x;
	yc=(int)y;
	
	if(z<0.0) return 0;
	if(xc<0) return 0;
	if(xc>=xx) return 0;
	if(yc<0) return 0;
	if(yc>=yy) return 0;
	
	return 1;
}
int c_font::conv3dchk2b(vec3 v1,int &xc,int &yc)
{
//	vec3 w1=v1-eye;
	
	float1 x=dot(v1,cam_axs[selcam2].x);
	float1 y=dot(v1,cam_axs[selcam2].y);
	float1 z=dot(v1,cam_axs[selcam2].z);
	if(z==0.0) z=1e-6;

	x=xx22+ x*(cam_zoom[selcam2]*yy)/fabs(z);
	y=yy22- y*(cam_zoom[selcam2]*yy)/fabs(z);//

	xc=(int)x;
	yc=(int)y;
	
//	if(z<0.0) return 0;

//clip
	int ok=0;
	float1 x2=40;
	if(xc<x2) 
	{
		float1 dx=(x-xx22);
		float1 dy=(y-yy22);
		float1 t1=sqrt(dx*dx + dy*dy);
		float1 t2=(x2-xx22)*t1/dx;   //xc=xx22+t2*dx/t1;
		xc=x2;
		yc=yy22+t2*dy/t1;
		ok=1;
	}
	x2=xx-40;
	if(xc>x2) 
	{
		float1 dx=(x-xx22);
		float1 dy=(y-yy22);
		float1 t1=sqrt(dx*dx + dy*dy);
		float1 t2=(x2-xx22)*t1/dx;
		xc=x2;
		yc=yy22+t2*dy/t1;
		ok=1;
	}
	float1 y2=100;
	if(yc<y2) 
	{
		float1 dx=(x-xx22);
		float1 dy=(y-yy22);
		float1 t1=sqrt(dx*dx + dy*dy);
		float1 t2=(y2-yy22)*t1/dy;
		xc=xx22+t2*dx/t1;
		yc=y2;
		ok=1;
	}
	y2=yy-20;
	if(yc>y2) 
	{
		float1 dx=(x-xx22);
		float1 dy=(y-yy22);
		float1 t1=sqrt(dx*dx + dy*dy);
		float1 t2=(y2-yy22)*t1/dy;
		xc=xx22+t2*dx/t1;
		yc=y2;
		
		ok=1;
	}
	
	return ok;//clipped!
}
int c_font::conv3dchk3(vec3 v1,int &xc,int &yc)
{
//	vec3 w1=v1-eye;
	
	float1 x=dot(v1,cam_axs[selcam2].x);
	float1 y=dot(v1,cam_axs[selcam2].y);
	float1 z=dot(v1,cam_axs[selcam2].z);
	if(z==0.0) z=1e-6;

	x=xx22+ x*(cam_zoom[selcam2]*yy)/fabs(z);
	y=yy22- y*(cam_zoom[selcam2]*yy)/fabs(z);//

	xc=(int)x;
	yc=(int)y;
	
	if(z<0.0) return 0;

//clip
	int ok=0;
	float1 x2=20;
	if(xc<x2) 
	{
		float1 dx=(x-xx22);
		float1 dy=(y-yy22);
		float1 t1=sqrt(dx*dx + dy*dy);
		float1 t2=(x2-xx22)*t1/dx;   //xc=xx22+t2*dx/t1;
		xc=x2;
		yc=yy22+t2*dy/t1;
		ok=1;
	}
	x2=xx-20;
	if(xc>x2) 
	{
		float1 dx=(x-xx22);
		float1 dy=(y-yy22);
		float1 t1=sqrt(dx*dx + dy*dy);
		float1 t2=(x2-xx22)*t1/dx;
		xc=x2;
		yc=yy22+t2*dy/t1;
		ok=1;
	}
	float1 y2=20;
	if(yc<y2) 
	{
		float1 dx=(x-xx22);
		float1 dy=(y-yy22);
		float1 t1=sqrt(dx*dx + dy*dy);
		float1 t2=(y2-yy22)*t1/dy;
		xc=xx22+t2*dx/t1;
		yc=y2;
		ok=1;
	}
	y2=yy-20;
	if(yc>y2) 
	{
		float1 dx=(x-xx22);
		float1 dy=(y-yy22);
		float1 t1=sqrt(dx*dx + dy*dy);
		float1 t2=(y2-yy22)*t1/dy;
		xc=xx22+t2*dx/t1;
		yc=y2;
		
		ok=1;
	}
	
	return 1;//latszik
}

int c_font::conv3dchk3c(vec3 v1,int &xc,int &yc,int &ok)
{

	float1 x=dot(v1,cam_axs[selcam2].x);
	float1 y=dot(v1,cam_axs[selcam2].y);
	float1 z=dot(v1,cam_axs[selcam2].z);
	if(z==0.0) z=1e-6;

	x=xx22+ x*(cam_zoom[selcam2]*yy)/fabs(z);
	y=yy22- y*(cam_zoom[selcam2]*yy)/fabs(z);//

	xc=(int)x;
	yc=(int)y;


//clip
	ok=0;
	int h=20;


	if((xc>h) && (xc<(xx-h)) &&(yc>h) && (yc<(yy-h))) ;
	else
	{
		int x5=xx22;
		int y5=yy22;
		float1 dx=(x-x5);
		float1 dy=(y-y5);
		float1 t1=sqrt(dx*dx + dy*dy);
		float1 t2=t1;
		if(t2>y5*80/100) t2=y5*80/100;
		xc=x5+dx*t2/t1;
		yc=y5+dy*t2/t1;
		ok=1;
	}
	/*
	float1 x2=20;
	if(xc<x2) 
	{
		float1 dx=(x-xx22);
		float1 dy=(y-yy22);
		float1 t1=sqrt(dx*dx + dy*dy);
		float1 t2=(x2-xx22)*t1/dx;   //xc=xx22+t2*dx/t1;
		xc=x2;
		yc=yy22+t2*dy/t1;
		ok=1;
	}
	x2=xx-20;
	if(xc>x2) 
	{
		float1 dx=(x-xx22);
		float1 dy=(y-yy22);
		float1 t1=sqrt(dx*dx + dy*dy);
		float1 t2=(x2-xx22)*t1/dx;
		xc=x2;
		yc=yy22+t2*dy/t1;
		ok=1;
	}
	float1 y2=20;
	if(yc<y2) 
	{
		float1 dx=(x-xx22);
		float1 dy=(y-yy22);
		float1 t1=sqrt(dx*dx + dy*dy);
		float1 t2=(y2-yy22)*t1/dy;
		xc=xx22+t2*dx/t1;
		yc=y2;
		ok=1;
	}
	y2=yy-20;
	if(yc>y2) 
	{
		float1 dx=(x-xx22);
		float1 dy=(y-yy22);
		float1 t1=sqrt(dx*dx + dy*dy);
		float1 t2=(y2-yy22)*t1/dy;
		xc=xx22+t2*dx/t1;
		yc=y2;
		
		ok=1;
	}
*/

	if(z<0.0)	 return 0;
	return 1;//latszik
}

#endif

void c_font::quad(float1 xb,float1 yb,float1 x,float1 y,int col)
{
	float1 h=fontsize5;// 1.0
	x*=fontscale5;//2
	y*=fontscale5;
	xb*=2.0;
	yb*=2.0;	
	lineb(xb+x,yb+y,xb+x+h,yb+y,col);
	lineb(xb+x+h,yb+y,xb+x+h,yb+y+h,col);
	lineb(xb+x+h,yb+y+h,xb+x,yb+y+h,col);
	lineb(xb+x,yb+y+h,xb+x,yb+y,col);
}
void c_font::quadr(float1 x,float1 y,int col,float1 c1,float1 s1)
{
	float1 x2,y2,h=1.0;
	x*=2.0;
	y*=2.0;
//	c1*=5.0;	s1*=5.0;//test
	x2=x+c1;y2=y+s1;	lineb(x,y,x2,y2,col); x=x2;y=y2;
	x2=x-s1;y2=y+c1;	lineb(x,y,x2,y2,col); x=x2;y=y2;
	x2=x-c1;y2=y-s1;	lineb(x,y,x2,y2,col); x=x2;y=y2;
	x2=x+s1;y2=y-c1;	lineb(x,y,x2,y2,col);
}
void c_font::quadb(float1 x,float1 y,int col)//letra
{
	float1 h=1.0;
	lineb(x,y,x+h,y,col);
	lineb(x+h,y,x+h,y+h,col);
	lineb(x+h,y+h,x,y+h,col);
	lineb(x,y+h,x,y,col);
}

//#define NUMSH 7
#define NUMSH 8

void c_font::drawnum(float1 xb,float1 yb,float1 x2,float1 y2,int n,int col)
{
	if(n=='-') n=10;
	else
	if(n=='.') n=15;
	else n-='0';
	int m=n*NUMSH;

	if(xb<0) return;
	if(yb<0) return;
	if(xb+5>=xx) return;
	if(yb+NUMSH>=yy) return;
	
	loopd(y,0,NUMSH,1)
	loopd(x,0,5,1)
	{
		if(nums[(m+y)*5+x]=='o')
			quad(xb,yb,x2+x,y2+y,col);
	}
}
void c_font::drawnums(float1 xb,float1 yb,int n,int col,int op)//op=0
{
	char buf[32];
	if(op==5) sprintf(buf,"%05d",n);
	if(op==4) sprintf(buf,"%04d",n);
	if(op==3) sprintf(buf,"%03d",n);
	if(op==2) sprintf(buf,"%02d",n);
	if(op==1) sprintf(buf,"%01d",n);
	if(op==0) sprintf(buf,"%d",n);
	
	int i=0,x=0;
	while(buf[i])
	{
		drawnum(xb,yb,x,0,buf[i],col);
		i++;
		x+=6;
	}
}
void c_font::drawnumsD(float1 xb,float1 yb,int n,int col,int op)//op=0
{
	char buf[32];
	int n2=n%10;	n/=10;
	if(op==5) sprintf(buf,"%05d.%d",n,n2);
	if(op==4) sprintf(buf,"%04d.%d",n,n2);
	if(op==3) sprintf(buf,"%03d.%d",n,n2);
	if(op==2) sprintf(buf,"%02d.%d",n,n2);
	if(op==1) sprintf(buf,"%01d.%d",n,n2);
	if(op==0) sprintf(buf,"%d.%d",n,n2);
	
	int i=0,x=0;
	while(buf[i])
	{
		drawnum(xb,yb,x,0,buf[i],col);
		i++;
		x+=6;
	}
}

void c_font::drawalp(float1 xb,float1 yb,int n,int col)
{
	if(n>='0' && n<='9') n=n-'0'+'Z'-'A'+3;
	else
	{
		if(n==' ') n='Z'+1;
		else
		if(n=='/') n='Z'+2;
		n-='A';
	}
	int m=n*7;

	if(xb<0) return;
	if(yb<0) return;
	if(xb+5>=xx) return;
	if(yb+7>=yy) return;
	
	loopd(y,0,7,1)
	loopd(x,0,5,1)
	{
		if(nums3[(m+y)*5+x]=='o')
			quad(xb,yb,x,y,col);
	}
}
void c_font::drawalps(float1 xb,float1 yb,const char *buf,int col,int op)//op=0
{
	int i=0;
	while(buf[i])
	{
		drawalp(xb,yb,buf[i],col);
		i++;
		xb+=6;
	}
}

void c_font::drawnumr(float1 xb,float1 yb,int n,int col,float1 c1,float1 s1)
{
	if(n=='-') n=10;
	else 
	{
		if(n<='9') n-='0';
		else
		{
		if(n=='n') n=11;//north east west
		if(n=='e') n=12;
		if(n=='s') n=13;
		if(n=='w') n=14;
		if(n=='.') n=15;
		}
	}
	int m=n*NUMSH;

	if(xb<10) return;
	if(yb<10) return;
	if(xb+10>=xx) return;
	if(yb+10>=yy) return;
	
	loopd(y,0,NUMSH,1)
	loopd(x,0,5,1)
	{
		if(nums[(m+y)*5+x]=='o')
			quadr(xb+((float1)x*c1)-((float1)y*s1),yb+((float1)y*c1)+((float1)x*s1),col,c1,s1);//b
	}
}
void c_font::drawnumr2(float1 xb,float1 yb,int n,int col,float1 c1,float1 s1)
{
	if(n=='-') n=10;
	else 
	{
		if(n<='9') n-='0';
		else
		{
		if(n=='n') n=11;//north east west
		if(n=='e') n=12;
		if(n=='s') n=13;
		if(n=='w') n=14;
		if(n=='.') n=15;
		}
	}
	int m=n*NUMSH;

	if(xb<10) return;
	if(yb<10) return;
	if(xb+10>=xx) return;
	if(yb+10>=yy) return;
	
	loopd(y,0,NUMSH,1)
	loopd(x,0,5,1)
	{
		if(nums[(m+y)*5+x]=='o')
			quadr(xb+((float1)x*c1)-((float1)y*s1),yb+((float1)y*c1)+((float1)x*s1),col,c1,s1);
	}
}
//muhorizont csak!
void c_font::drawnumsr(float1 xb,float1 yb,int n,int col,float1 dx,float1 dy,float1 c1,float1 s1,int op)//op=0
{
	char buf[32];
	if(op==5) sprintf(buf,"%05d",n);
	if(op==4) sprintf(buf,"%04d",n);
	if(op==3) sprintf(buf,"%03d",n);
	if(op==2) sprintf(buf,"%02d",n);
	if(op==1) sprintf(buf,"%01d",n);
	if(op==0) sprintf(buf,"%d",n);
	
//	xb=hi(xb);	yb=hi(yb);
	xb+=(dx*c1-dy*s1);
	yb+=(dy*c1+dx*s1);
	
	int i=0;
	while(buf[i])
	{
		drawnumr(xb,yb,buf[i],col,c1,s1);
		i++;
		
		xb+=6.0*c1;
		yb+=6.0*s1;
	}
}
void c_font::update(float1 xtime)
{
}
//void draw(float1 xtime,int dat,int op){}
void c_font::recvcmd(s_cmd *tmp)
{
}



