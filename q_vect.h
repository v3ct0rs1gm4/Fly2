/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _QVECTH
#define _QVECTH


#include "q_antdef.h"


float1 fsgn(float1 n);

#define DIVLIMIT 1e-5

inline void chklim(float &n)
{
	if(n==0.0) n=DIVLIMIT;
	if(fabs(n)<DIVLIMIT) n=DIVLIMIT*fsgn(n);
}


struct mat2
{
	float ax,ay,bx,by;
	mat2() {ax=1;ay=0;bx=0;by=1;}
	mat2(double a1,double a2,double a3,double a4) {ax=(float1)a1;ay=(float1)a2;bx=(float1)a3;by=(float1)a4;}
};



class vec3
{
public:
	float1 x,y,z;
	
	vec3() {x=0;y=0;z=0;}
	vec3(int x2,int y2,int z2) {x=x2;y=y2;z=z2;}
	vec3(float1 x2,float1 y2,float1 z2) {x=x2;y=y2;z=z2;}
//	vec3(float x2,float y2,float z2) {x=(float1)x2;y=(float1)y2;z=(float1)z2;}
	vec3(double x2,double y2,double z2) {x=(float1)x2;y=(float1)y2;z=(float1)z2;}

/*	vec3(float1 x2,double y2,double z2) {x=(float1)x2;y=(float1)y2;z=(float1)z2;}
	vec3(double x2,float1 y2,double z2) {x=(float1)x2;y=(float1)y2;z=(float1)z2;}
	vec3(double x2,double y2,float1 z2) {x=(float1)x2;y=(float1)y2;z=(float1)z2;}
	vec3(float1 x2,float1 y2,double z2) {x=(float1)x2;y=(float1)y2;z=(float1)z2;}
	vec3(double x2,float1 y2,float1 z2) {x=(float1)x2;y=(float1)y2;z=(float1)z2;}
	vec3(float1 x2,double y2,float1 z2) {x=(float1)x2;y=(float1)y2;z=(float1)z2;}
	*/
	vec3 operator+(float1 s) {vec3 e;e.x=x+s;e.y=y+s;e.z=z+s;return e;}
	vec3 operator-(float1 s) {vec3 e;e.x=x-s;e.y=y-s;e.z=z-s;return e;}
	vec3 operator*(float1 s) {vec3 e;e.x=x*s;e.y=y*s;e.z=z*s;return e;}
	vec3 operator/(float1 s) {chklim(s);vec3 e; e.x=x/s;e.y=y/s;e.z=z/s;return e; }

	vec3 operator+=(float1 s) {x+=s;y+=s;z+=s;return *this;}
	vec3 operator-=(float1 s) {x-=s;y-=s;z-=s;return *this;}
	vec3 operator*=(float1 s) {x*=s;y*=s;z*=s;return *this;}
	vec3 operator/=(float1 s) {chklim(s); x/=s;y/=s;z/=s;return *this;}

	vec3 operator+(vec3 v) {vec3 e;e.x=x+v.x;e.y=y+v.y;e.z=z+v.z;return e;}
	vec3 operator-(vec3 v) {vec3 e;e.x=x-v.x;e.y=y-v.y;e.z=z-v.z;return e;}
	vec3 operator*(vec3 v) {vec3 e;e.x=x*v.x;e.y=y*v.y;e.z=z*v.z;return e;}
	vec3 operator/(vec3 v) {chklim(v.x);chklim(v.y);chklim(v.z);vec3 e;e.x=x/v.x;e.y=y/v.y;e.z=z/v.z;return e;}

	vec3 operator+=(vec3 v) {x+=v.x;y+=v.y;z+=v.z;return *this;}
	vec3 operator-=(vec3 v) {x-=v.x;y-=v.y;z-=v.z;return *this;}
	vec3 operator*=(vec3 v) {x*=v.x;y*=v.y;z*=v.z;return *this;}
	vec3 operator/=(vec3 v) {chklim(v.x);chklim(v.y);chklim(v.z);x/=v.x;y/=v.y;z/=v.z;return *this;}

	vec3 operator*=(mat2 v) {x=v.ax*x+v.ay*y;y=v.bx*x+v.by*y;return *this;}//vec2 !!
	
//	vec3 operator -() {x=-x;y=-y;z=-z;return *this;};
	vec3 operator -() {vec3 e;e.x=-x;e.y=-y;e.z=-z;return e;}
	
//	vec3 operator-() {x=-x;y=-y;z=-z;return *this;}
};
class vec4
{
public:
	float1 x,y,z,w;
	
	vec4() {x=0;y=0;z=0;w=0;}
	vec4(int x2,int y2,int z2,int w2) {x=x2;y=y2;z=z2;w=w2;}
	vec4(float1 x2,float1 y2,float1 z2,float1 w2) {x=x2;y=y2;z=z2;w=w2;}
//	vec4(float x2,float y2,float z2,float w2) {x=x2;y=y2;z=z2;w=w2;}
	vec4(double x2,double y2,double z2,double w2) {x=x2;y=y2;z=z2;w=w2;}
	
	vec4 operator+(float1 v) {vec4 e;e.x=x+v;e.y=y+v;e.z=z+v;e.w=w+v;return e;}
	vec4 operator-(float1 v) {vec4 e;e.x=x-v;e.y=y-v;e.z=z-v;e.w=w-v;return e;}
	vec4 operator*(float1 v) {vec4 e;e.x=x*v;e.y=y*v;e.z=z*v;e.w=w*v;return e;}
	vec4 operator/(float1 v) {if(fabs(v)<1e-16) v=fsgn(v)*1e-16;vec4 e;e.x=x/v;e.y=y/v;e.z=z/v;e.w=w/v;return e;}

	vec4 operator+(vec4 v) {vec4 e;e.x=x+v.x;e.y=y+v.y;e.z=z+v.z;e.w=w+v.w;return e;}
	vec4 operator-(vec4 v) {vec4 e;e.x=x-v.x;e.y=y-v.y;e.z=z-v.z;e.w=w-v.w;return e;}
	vec4 operator*(vec4 v) {vec4 e;e.x=x*v.x;e.y=y*v.y;e.z=z*v.z;e.w=w*v.w;return e;}
	vec4 operator/(vec4 v) {vec4 e;e.x=x/v.x;e.y=y/v.y;e.z=z/v.z;e.w=w/v.w;return e;}

	vec4 operator =(vec3 v3)
	{
		x=v3.x;		
		y=v3.y;		
		z=v3.z;		
		w=0.0;
		return *this;// BUG volt
	}
};



inline float1 dot(vec3 v1,vec3 v2) {return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z ;}
inline float1 dot4(vec3 v1,vec4 v2) {return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z ;}
inline float1 dot4w(vec3 v1,vec4 v2) {return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z  + v2.w ;}
inline float1 dotxy(vec3 v1,vec3 v2) {return v1.x*v2.x + v1.y*v2.y;}
inline float1 dotxz(vec3 v1,vec3 v2) {return v1.x*v2.x+ v1.z*v2.z ;}
inline float1 length(vec3 v1) {float1 d=dot(v1,v1);if(d<1e-16) return 0.0;else return sqrt(d);};
inline float1 length2(vec3 v1) {float1 d=dot(v1,v1);return (d);};
inline float1 lengthxy(vec3 v1) {float1 d=dotxy(v1,v1);if(d<1e-16) return 1e-16;else return sqrt(d);};
inline float1 lengthxz(vec3 v1) {float1 d=dotxz(v1,v1);if(d<1e-16) return 1e-16;else return sqrt(d);};

inline vec3 normalize(vec3 v1) {float1 d=length(v1);if(d<1e-16) return v1; else return v1/d;}
inline vec3 cross(vec3 v1,vec3 v2) 
{
	vec3 v3;
	v3.x=v1.y*v2.z - v1.z*v2.y;
	v3.y=v1.z*v2.x - v1.x*v2.z;
	v3.z=v1.x*v2.y - v1.y*v2.x;
	
	return v3;
}

inline float1 clamp(float1 n,float1 l1,float1 l2)
{
	if(n<l1) n=l1;
	if(n>l2) n=l2;
	
	return n;
}
inline float1 saturate(float1 n)
{
	if(n<0.0) n=0.0;
	if(n>1.0) n=1.0;
	
	return n;
}
inline vec3 saturate(vec3 v1)
{
	v1.x=saturate(v1.x);
	v1.y=saturate(v1.y);
	v1.z=saturate(v1.z);
	
	return v1;
}
inline float1 lerp(float1 v1,float1 v2,float1 t)
{
	return v1+(v2-v1)*t;
}
inline vec3 lerp(vec3 v1,vec3 v2,float1 t)
{
	v1.x=lerp(v1.x,v2.x,t);
	v1.y=lerp(v1.y,v2.y,t);
	v1.z=lerp(v1.z,v2.z,t);
	
	return v1;
}
inline vec3 lerp(vec3 v1,vec3 v2,vec3 t)
{
	v1.x=lerp(v1.x,v2.x,t.x);
	v1.y=lerp(v1.y,v2.y,t.y);
	v1.z=lerp(v1.z,v2.z,t.z);
	
	return v1;
}
inline vec3 floor(vec3 v1)
{
	v1.x=floor(v1.x);
	v1.y=floor(v1.y);
	v1.z=floor(v1.z);
	
	return v1;
}
inline vec3 fmod(vec3 v1,float n)
{
	v1.x=fmod(v1.x,n);
	v1.y=fmod(v1.y,n);
	v1.z=fmod(v1.z,n);
	
	return v1;
}





struct axis
{
	vec3 x;
	vec3 y;
	vec3 z;
	vec3 w;
	
	axis()
	{
		x=vec3(1,0,0);
		y=vec3(0,1,0);
		z=vec3(0,0,1);
		w=vec3(0,0,0);
	}
};

struct axis4
{
	union{
		vec4 x;
		vec3 vec3_x; float1 q1;
	};
	union{
		vec4 y;
		vec3 vec3_y; float1 q2;
	};
	union{
		vec4 z;
		vec3 vec3_z; float1 q3;
	};
	union{
		vec4 w;
		vec3 vec3_w; float1 q4;
	};
	
	axis4()
	{
		x=vec4(1,0,0,0);
		y=vec4(0,1,0,0);
		z=vec4(0,0,1,0);
		w=vec4(0,0,0,1);
	}
};
inline axis axis_mul(axis &m1,axis &m2)//3x3 mat multiplication
{
	axis m3;
	
	m3.x=m2.x*m1.x.x +m2.y*m1.x.y +m2.z*m1.x.z;
	m3.y=m2.x*m1.y.x +m2.y*m1.y.y +m2.z*m1.y.z;
	m3.z=m2.x*m1.z.x +m2.y*m1.z.y +m2.z*m1.z.z;
		
	return m3;
}
inline axis4 axis4_mul(axis4 &m1,axis4 &m2)//4x4 mat multiplication
{
	axis4 m3;
	
	m3.x=m2.x*m1.x.x +m2.y*m1.x.y +m2.z*m1.x.z +m2.w*m1.x.w;
	m3.y=m2.x*m1.y.x +m2.y*m1.y.y +m2.z*m1.y.z +m2.w*m1.y.w;
	m3.z=m2.x*m1.z.x +m2.y*m1.z.y +m2.z*m1.z.z +m2.w*m1.z.w;
	m3.w=m2.x*m1.w.x +m2.y*m1.w.y +m2.z*m1.w.z +m2.w*m1.w.w;
		
	return m3;
}

inline float determinant(axis4 &m)
{
	return
		 m.x.w * m.y.z * m.z.y * m.w.x
		-m.x.z * m.y.w * m.z.y * m.w.x
		-m.x.w * m.y.y * m.z.z * m.w.x
		+m.x.y * m.y.w * m.z.z * m.w.x+
		
   		 m.x.z * m.y.y * m.z.w * m.w.x
   		-m.x.y * m.y.z * m.z.w * m.w.x
   		-m.x.w * m.y.z * m.z.x * m.w.y
   		+m.x.z * m.y.w * m.z.x * m.w.y+
   		
   		 m.x.w * m.y.x * m.z.z * m.w.y
   		-m.x.x * m.y.w * m.z.z * m.w.y
   		-m.x.z * m.y.x * m.z.w * m.w.y
   		+m.x.x * m.y.z * m.z.w * m.w.y+
   		
   		 m.x.w * m.y.y * m.z.x * m.w.z
   		-m.x.y * m.y.w * m.z.x * m.w.z
   		-m.x.w * m.y.x * m.z.y * m.w.z
   		+m.x.x * m.y.w * m.z.y * m.w.z+
   		
   		 m.x.y * m.y.x * m.z.w * m.w.z
   		-m.x.x * m.y.y * m.z.w * m.w.z
   		-m.x.z * m.y.y * m.z.x * m.w.w
   		+m.x.y * m.y.z * m.z.x * m.w.w+
   		
   		 m.x.z * m.y.x * m.z.y * m.w.w
   		-m.x.x * m.y.z * m.z.y * m.w.w
   		-m.x.y * m.y.x * m.z.z * m.w.w
   		+m.x.x * m.y.y * m.z.z * m.w.w;
}
inline axis4 invmatrix(axis4 &m)//perfect!
{
	axis4 n;

	n.x.x = m.y.z*m.z.w*m.w.y - m.y.w*m.z.z*m.w.y + m.y.w*m.z.y*m.w.z - m.y.y*m.z.w*m.w.z - m.y.z*m.z.y*m.w.w + m.y.y*m.z.z*m.w.w;
	n.x.y = m.x.w*m.z.z*m.w.y - m.x.z*m.z.w*m.w.y - m.x.w*m.z.y*m.w.z + m.x.y*m.z.w*m.w.z + m.x.z*m.z.y*m.w.w - m.x.y*m.z.z*m.w.w;
	n.x.z = m.x.z*m.y.w*m.w.y - m.x.w*m.y.z*m.w.y + m.x.w*m.y.y*m.w.z - m.x.y*m.y.w*m.w.z - m.x.z*m.y.y*m.w.w + m.x.y*m.y.z*m.w.w;
	n.x.w = m.x.w*m.y.z*m.z.y - m.x.z*m.y.w*m.z.y - m.x.w*m.y.y*m.z.z + m.x.y*m.y.w*m.z.z + m.x.z*m.y.y*m.z.w - m.x.y*m.y.z*m.z.w;
	n.y.x = m.y.w*m.z.z*m.w.x - m.y.z*m.z.w*m.w.x - m.y.w*m.z.x*m.w.z + m.y.x*m.z.w*m.w.z + m.y.z*m.z.x*m.w.w - m.y.x*m.z.z*m.w.w;
	n.y.y = m.x.z*m.z.w*m.w.x - m.x.w*m.z.z*m.w.x + m.x.w*m.z.x*m.w.z - m.x.x*m.z.w*m.w.z - m.x.z*m.z.x*m.w.w + m.x.x*m.z.z*m.w.w;
	n.y.z = m.x.w*m.y.z*m.w.x - m.x.z*m.y.w*m.w.x - m.x.w*m.y.x*m.w.z + m.x.x*m.y.w*m.w.z + m.x.z*m.y.x*m.w.w - m.x.x*m.y.z*m.w.w;
	n.y.w = m.x.z*m.y.w*m.z.x - m.x.w*m.y.z*m.z.x + m.x.w*m.y.x*m.z.z - m.x.x*m.y.w*m.z.z - m.x.z*m.y.x*m.z.w + m.x.x*m.y.z*m.z.w;
	n.z.x = m.y.y*m.z.w*m.w.x - m.y.w*m.z.y*m.w.x + m.y.w*m.z.x*m.w.y - m.y.x*m.z.w*m.w.y - m.y.y*m.z.x*m.w.w + m.y.x*m.z.y*m.w.w;
	n.z.y = m.x.w*m.z.y*m.w.x - m.x.y*m.z.w*m.w.x - m.x.w*m.z.x*m.w.y + m.x.x*m.z.w*m.w.y + m.x.y*m.z.x*m.w.w - m.x.x*m.z.y*m.w.w;
	n.z.z = m.x.y*m.y.w*m.w.x - m.x.w*m.y.y*m.w.x + m.x.w*m.y.x*m.w.y - m.x.x*m.y.w*m.w.y - m.x.y*m.y.x*m.w.w + m.x.x*m.y.y*m.w.w;
	n.z.w = m.x.w*m.y.y*m.z.x - m.x.y*m.y.w*m.z.x - m.x.w*m.y.x*m.z.y + m.x.x*m.y.w*m.z.y + m.x.y*m.y.x*m.z.w - m.x.x*m.y.y*m.z.w;
	n.w.x = m.y.z*m.z.y*m.w.x - m.y.y*m.z.z*m.w.x - m.y.z*m.z.x*m.w.y + m.y.x*m.z.z*m.w.y + m.y.y*m.z.x*m.w.z - m.y.x*m.z.y*m.w.z;
	n.w.y = m.x.y*m.z.z*m.w.x - m.x.z*m.z.y*m.w.x + m.x.z*m.z.x*m.w.y - m.x.x*m.z.z*m.w.y - m.x.y*m.z.x*m.w.z + m.x.x*m.z.y*m.w.z;
	n.w.z = m.x.z*m.y.y*m.w.x - m.x.y*m.y.z*m.w.x - m.x.z*m.y.x*m.w.y + m.x.x*m.y.z*m.w.y + m.x.y*m.y.x*m.w.z - m.x.x*m.y.y*m.w.z;
	n.w.w = m.x.y*m.y.z*m.z.x - m.x.z*m.y.y*m.z.x + m.x.z*m.y.x*m.z.y - m.x.x*m.y.z*m.z.y - m.x.y*m.y.x*m.z.z + m.x.x*m.y.y*m.z.z;

    float det=determinant(m);

	float scale=1.0;
    if(det!=0.0) scale/=det;
    
	n.x.x*=scale;
	n.x.y*=scale;
	n.x.z*=scale;
	n.x.w*=scale;
	n.y.x*=scale;
	n.y.y*=scale;
	n.y.z*=scale;
	n.y.w*=scale;
	n.z.x*=scale;
	n.z.y*=scale;
	n.z.z*=scale;
	n.z.w*=scale;
	n.w.x*=scale;
	n.w.y*=scale;
	n.w.z*=scale;
	n.w.w*=scale;

	return n;	//	memcpy(&m1,&n,sizeof(axis4));
}

inline void copyaxis4(axis4 &m1,axis &m2)
{
	m1.x=m2.x;
	m1.y=m2.y;
	m1.z=m2.z;
}
inline void axis4ident(axis4 &m1)
{
	m1.x=vec4(1,0,0,0);
	m1.y=vec4(0,1,0,0);
	m1.z=vec4(0,0,1,0);
	m1.w=vec4(0,0,0,1);
}
inline void axis4rotx(axis4 &m1,float1 alf)
{
	axis4ident(m1);

    m1.y.y= cosf(alf);  m1.y.z=sinf(alf);//++ 22 23
    m1.z.y=-sinf(alf);  m1.z.z=cosf(alf);//-+ 32 33
}
inline void axis4roty(axis4 &m1,float1 alf)
{
	axis4ident(m1);

    m1.x.x=cosf(alf);  m1.x.z=-sinf(alf);//+- 11 13
    m1.z.x=sinf(alf);  m1.z.z= cosf(alf);//++ 31 33
}
inline void axis4rotz(axis4 &m1,float1 alf)
{
	axis4ident(m1);

    m1.x.x= cosf(alf);  m1.x.y=sinf(alf);//++ 11 12
    m1.y.x=-sinf(alf);  m1.y.y=cosf(alf);//-+ 21 22
}
inline void axis4translate(axis4 &m1,float1 x,float1 y,float1 z)
{
	axis4ident(m1);

	m1.w.x=x;
	m1.w.y=y;
	m1.w.z=z;
}
inline void axis4scale(axis4 &m1,float1 x,float1 y,float1 z)
{
	axis4ident(m1);

	m1.x.x=x;
	m1.y.y=y;
	m1.z.z=z;
}
inline axis4 vec2axis(vec3 axi)
{
	axis w1;
	w1.x=axi;
	
	w1.y=vec3(axi.z,axi.x,axi.y); // old nem jo ha 1,1,1
	w1.z=cross(w1.y,w1.x);
	w1.y=cross(w1.x,w1.z); // re

	w1.x=normalize(w1.x);
	w1.y=normalize(w1.y);
	w1.z=normalize(w1.z);

	axis4 w4;
	copyaxis4(w4,w1);
	return w4;
}
inline axis4 vec2axisB(vec3 axi)
{
	axis w1;
	w1.x=axi;
/*	//if(fabs(axi.x)>=fabs(axi.z) && fabs(axi.y)>=fabs(axi.z))	
	{	w1.y=vec3(axi.y,-axi.x,axi.z);}// XY 2drot 90
	if(fabs(axi.x)>=fabs(axi.y) && fabs(axi.z)>=fabs(axi.y)) 	{	w1.y=vec3(axi.z,axi.y,-axi.x);}// XZ 2drot 90
	if(fabs(axi.y)>=fabs(axi.x) && fabs(axi.z)>=fabs(axi.x)) 	{	w1.y=vec3(axi.x,-axi.z,axi.y);}// YZ 2drot 90
	if(fabs(axi.x)>=fabs(axi.z) && fabs(axi.y)>=fabs(axi.z))	{	w1.y=vec3(0,0,1);}
	if(fabs(axi.x)>=fabs(axi.y) && fabs(axi.z)>=fabs(axi.y)) 	{	w1.y=vec3(0,1,0);}
	if(fabs(axi.y)>=fabs(axi.x) && fabs(axi.z)>=fabs(axi.x)) 	{	w1.y=vec3(1,0,0);}
	*/
	
	w1.y=vec3(-axi.z,-axi.x,axi.y); // old nem jo ha 1,1,1
	w1.z=cross(w1.y,w1.x); 
	w1.y=cross(w1.x,w1.z); // re

	w1.x=normalize(w1.x);
	w1.y=normalize(w1.y);
	w1.z=normalize(w1.z);

	axis4 w4;
	copyaxis4(w4,w1);
	return w4;
}
inline axis4 getrotation2axis4(vec3 rotcent,vec3 rotaxis,float1 ang1)
{
	axis4 ax5,axw;
	axis4 axrot=vec2axisB(rotaxis);
	axis4 axrotinv=invmatrix(axrot);
	
	axis4translate(ax5,-rotcent.x,-rotcent.y,-rotcent.z);//rot pos
	ax5=axis4_mul(ax5,axrotinv);//to rot ax

	axis4rotx(axw,ang1);//rot
	ax5=axis4_mul(ax5,axw);

	ax5=axis4_mul(ax5,axrot);//back to rot ax

	axis4translate(axw,rotcent.x,rotcent.y,rotcent.z);//rot pos
	ax5=axis4_mul(ax5,axw);
	
	return ax5;
}	



inline float1 fsgn(float1 n) {if(n<0.0) return -1.0;if(n>0.0) return 1.0;return 0.0;}

inline float1 qabs(float1 n)
{
	if(n<float1(0.0)) return -n;
	return n;
}
inline float sqr(float n) {return n*n;}





struct s_qmat
{
	float1 _11,_12,_13,_14;
	float1 _21,_22,_23,_24;
	float1 _31,_32,_33,_34;
	float1 _41,_42,_43,_44;
};
#define qmat struct s_qmat



inline void qMatrixIdentity(qmat *m1)
{
	memset(m1,0,sizeof(qmat));

    m1->_11=1.0;
    m1->_22=1.0;
    m1->_33=1.0;
    m1->_44=1.0;
}
inline void qMatrixMultiply(qmat *m1,qmat *m2,qmat *m3)
{
	qmat mo;

	mo._11=m2->_11*m3->_11 + m2->_12*m3->_21 + m2->_13*m3->_31 + m2->_14*m3->_41 ;
	mo._12=m2->_11*m3->_12 + m2->_12*m3->_22 + m2->_13*m3->_32 + m2->_14*m3->_42 ;
	mo._13=m2->_11*m3->_13 + m2->_12*m3->_23 + m2->_13*m3->_33 + m2->_14*m3->_43 ;
	mo._14=m2->_11*m3->_14 + m2->_12*m3->_24 + m2->_13*m3->_34 + m2->_14*m3->_44 ;

	mo._21=m2->_21*m3->_11 + m2->_22*m3->_21 + m2->_23*m3->_31 + m2->_24*m3->_41 ;
	mo._22=m2->_21*m3->_12 + m2->_22*m3->_22 + m2->_23*m3->_32 + m2->_24*m3->_42 ;
	mo._23=m2->_21*m3->_13 + m2->_22*m3->_23 + m2->_23*m3->_33 + m2->_24*m3->_43 ;
	mo._24=m2->_21*m3->_14 + m2->_22*m3->_24 + m2->_23*m3->_34 + m2->_24*m3->_44 ;

	mo._31=m2->_31*m3->_11 + m2->_32*m3->_21 + m2->_33*m3->_31 + m2->_34*m3->_41 ;
	mo._32=m2->_31*m3->_12 + m2->_32*m3->_22 + m2->_33*m3->_32 + m2->_34*m3->_42 ;
	mo._33=m2->_31*m3->_13 + m2->_32*m3->_23 + m2->_33*m3->_33 + m2->_34*m3->_43 ;
	mo._34=m2->_31*m3->_14 + m2->_32*m3->_24 + m2->_33*m3->_34 + m2->_34*m3->_44 ;

	mo._41=m2->_41*m3->_11 + m2->_42*m3->_21 + m2->_43*m3->_31 + m2->_44*m3->_41 ;
	mo._42=m2->_41*m3->_12 + m2->_42*m3->_22 + m2->_43*m3->_32 + m2->_44*m3->_42 ;
	mo._43=m2->_41*m3->_13 + m2->_42*m3->_23 + m2->_43*m3->_33 + m2->_44*m3->_43 ;
	mo._44=m2->_41*m3->_14 + m2->_42*m3->_24 + m2->_43*m3->_34 + m2->_44*m3->_44 ;

    *m1=mo;
}
inline void qMatrixScaling(qmat *m1,float1 x,float1 y,float1 z)
{
	qMatrixIdentity(m1);

    m1->_11=x;
    m1->_22=y;
    m1->_33=z;
}
inline void qMatrixTranslation(qmat *m1,float1 x,float1 y,float1 z)
{
	qMatrixIdentity(m1);

    m1->_41=x;
    m1->_42=y;
    m1->_43=z;
}
inline void qMatrixRotationX(qmat *m1,float1 alf)
{
	qMatrixIdentity(m1);

    m1->_22= cosf(alf);  m1->_23=sinf(alf);//++
    m1->_32=-sinf(alf);  m1->_33=cosf(alf);//-+
}
inline void qMatrixRotationY(qmat *m1,float1 alf)
{
	qMatrixIdentity(m1);

    m1->_11=cosf(alf);  m1->_13=-sinf(alf);//+-
    m1->_31=sinf(alf);  m1->_33= cosf(alf);//++
}
inline void qMatrixRotationZ(qmat *m1,float1 alf)
{
	qMatrixIdentity(m1);

    m1->_11= cosf(alf);  m1->_12=sinf(alf);//++
    m1->_21=-sinf(alf);  m1->_22=cosf(alf);//-+
}


inline void qVec3Transform(vec4 *v1,vec3 *v2,qmat *m1)
{
	v1[0].x=v2[0].x*m1->_11 + v2[0].y*m1->_21 + v2[0].z*m1->_31 + m1->_41;
	v1[0].y=v2[0].x*m1->_12 + v2[0].y*m1->_22 + v2[0].z*m1->_32 + m1->_42;
	v1[0].z=v2[0].x*m1->_13 + v2[0].y*m1->_23 + v2[0].z*m1->_33 + m1->_43;
    v1[0].w=1.0;
}
inline void qMatrixTranspose(qmat *m1,qmat *m2)
{
	m1->_11=m2->_11;
	m1->_12=m2->_21;
	m1->_13=m2->_31;
	m1->_14=m2->_41;

	m1->_21=m2->_12;
	m1->_22=m2->_22;
	m1->_23=m2->_32;
	m1->_24=m2->_42;

	m1->_31=m2->_13;
	m1->_32=m2->_23;
	m1->_33=m2->_33;
	m1->_34=m2->_43;

	m1->_41=m2->_14;
	m1->_42=m2->_24;
	m1->_43=m2->_34;
	m1->_44=m2->_44;
}

inline void qMatrixLookAtLH(qmat *m1,vec3 *pEye,vec3 *pAt,vec3 *pUp)
{

	vec3 zaxis = normalize( pAt[0]-pEye[0] );
	vec3 xaxis = normalize(cross(pUp[0],zaxis));
	vec3 yaxis = cross(zaxis,xaxis);
	
	m1->_11=xaxis.x;//forditva, mert nem gl matrix!
	m1->_21=xaxis.y;
	m1->_31=xaxis.z;
	m1->_41=-dot(xaxis, pEye[0]);
	
	m1->_12=yaxis.x;
	m1->_22=yaxis.y;
	m1->_32=yaxis.z;
	m1->_42=-dot(yaxis, pEye[0]);
	
	m1->_13=-zaxis.x;//+?
	m1->_23=-zaxis.y;
	m1->_33=-zaxis.z;
	m1->_43=dot(zaxis, pEye[0]);//-?
	
	m1->_14=0.0;
	m1->_24=0.0;
	m1->_34=0.0;
	m1->_44=1.0;
}
inline void qMatrixPerspectiveFovLH(qmat *m1,float fieldOfView,float aspectRatio,float zNear,float zFar)
{
	float f = 1.0f / tanf(fieldOfView * (M_PI/360.0f));
	float *m = &m1->_11;
	
	m[0] = f  /aspectRatio;
	m[1] = 0.0;
	m[2] = 0.0;
	m[3] = 0.0;
	
	m[4] = 0.0;
	m[5] = f;
	m[6] = 0.0;
	m[7] = 0.0;
	
	m[8] = 0.0;
	m[9] = 0.0;
	m[10] = (zFar + zNear) / (zNear - zFar);
//	m[10] = (zFar-zNear) / (zFar-zNear );
//	m[10] = (zFar) / (zFar-zNear );//????
	m[11] = -1.0;//+
	
	m[12] = 0.0;
	m[13] = 0.0;
	m[14] = 2.0f * zFar * zNear / (zNear - zFar);
//	m[14] = -2.0f * zFar * zNear / (zFar - zNear);
//	m[14] = -zFar * zNear / (zFar - zNear);//-    ????
	m[15] = 0.0;
}
inline void qMatrixOrtho(qmat *m1,float1 l,float1 r,float1 b,float1 t,float1 n,float1 f)//szar
{
	m1->_11=2.0/(r-l);
	m1->_12=0.0;
	m1->_13=0.0;
	m1->_14=-(r+l)/(r-l);

	m1->_21=0.0;
	m1->_22=2.0/(t-b);
	m1->_23=0.0;
	m1->_24=-(t+b)/(t-b);

	m1->_31=0.0;
	m1->_32=0.0;
	m1->_33=-2.0*((f-n)/(f-n));
	m1->_34=(f+n)/(f-n);

	m1->_41=0.0;
	m1->_42=0.0;
	m1->_43=0.0;
	m1->_44=1.0;
}
inline void qMatrixOrthoLH(qmat *m1,  const float w,   const float h,    const float zn,    const float zf)
{
    qMatrixIdentity(m1);
    
    m1->_11 = 2.0 / w;
    m1->_22 = 2.0 / h;
    m1->_33 = 1.0 / (zf - zn);
    m1->_43 = zn / (zn - zf);
}





inline void circumcircle(vec3 &v1,vec3 &v2,vec3 &v3,vec3 &circle_center,float1 &ra,vec3 &nor)
{
//3 pont kore irhato kor, ket felezomeroleges metszespontja
		vec3 v12=(v1+v2)*0.5;
		vec3 v23=(v2+v3)*0.5;

		vec3 nor12=normalize(v2-v1);//minden pont egy sikban van
		vec3 ray23=normalize(cross(v3-v2,nor));//gnorm

        float1 q=dot(ray23,nor12);
        if(q==0.0) {ra=1e12;return;};//q=1.0;
		float1 t1=dot(v12-v23,nor12)/q;// -eye!
		
		circle_center=v23 +ray23*t1;
		ra=length(circle_center-v1);
	}

inline float1 tri_area(vec3 p1,vec3 p2,vec3 p3)//triangle area
{
	vec3 e1=(p2-p1);//also el
	float1 a=length(e1);//base
	vec3 n1=e1;//normalize(e1);
	if(a!=0.0) n1/=a;

	vec3 e3=(p3-p1);//masik el, p3 a legmagasabb pont
	vec3 w1=e3 - n1*dot(n1,e3);//n1 re meroleges lesz w1
	float1 h=length(w1);//w1 hossza a high
	
	return h*a/2.0f;//ket-ket egyforma haromszogre osztja a h/w1/
}
inline float1 tri_area2(vec3 p1,vec3 p2,vec3 p3)//triangle area
{
	vec3 e1=(p2-p1);
	vec3 e2=(p3-p1);
	
	return length(cross(e1,e2))/2.0f;
}
inline vec3 qbezier(vec3 v1,vec3 v2,vec3 v3,vec3 v4,float1 t1)
{
	float1 t2=t1*t1;
	float1 t3=t2*t1;
	float1 u1=1.0-t1;
	float1 u2=u1*u1;
	float1 u3=u2*u1;
	
	return v1*u3 + v2*t1*u2*3.0 + v3*t2*u1*3.0 + v4*t3;
}

inline void rotatev(vec3 &v1,vec3 rotz,float1 alf)
{
	alf*=rad;

	vec3 rotx=v1;
	rotz=normalize(rotz);

	vec3 maradek=rotz*dot(rotz,rotx);
	rotx-=maradek;
	vec3 roty=cross(rotx,rotz);//roty nem normalt, de rotx sem az!!!
	vec3 w1=rotx*cos(alf) + roty*sin(alf);
	v1=maradek+w1;
}
inline vec3 rotatevrad(vec3 v11,vec3 rotz,float1 alf)
{
	vec3 v1=v11;//	alf*=rad;

	vec3 rotx=v1;
	rotz=normalize(rotz);

	vec3 maradek=rotz*dot(rotz,rotx);
	rotx-=maradek;
	vec3 roty=cross(rotx,rotz);//roty nem normalt, de rotx sem az!!!
	vec3 w1=rotx*cos(alf) + roty*sin(alf);
	v1=maradek+w1;
	
	return v1;
}
inline vec3 rotatevradcent(vec3 v11,vec3 rotz,float1 alf,vec3 cent)
{
	v11-=cent;
	v11=rotatevrad(v11,rotz,alf);
	v11+=cent;
	return v11;
}

inline void rotax(axis &ax,float1 alf)
{
	alf*=rad;
	float1 c1=cos(alf);
	float1 s1=sin(alf);

	vec3 x=ax.z*c1 + ax.y*s1;
	vec3 y=ax.y*c1 - ax.z*s1;
	ax.z=x;
	ax.y=y;
}
inline void rotay(axis &ax,float1 alf)
{
	alf*=rad;
	float1 c1=cos(alf);
	float1 s1=sin(alf);

	vec3 x=ax.x*c1 + ax.z*s1;
	vec3 y=ax.z*c1 - ax.x*s1;
	ax.x=x;
	ax.z=y;
}
inline void rotaz(axis &ax,float1 alf)
{
	alf*=rad;
	float1 c1=cos(alf);
	float1 s1=sin(alf);

	vec3 x=ax.x*c1 + ax.y*s1;
	vec3 y=ax.y*c1 - ax.x*s1;
	ax.x=x;
	ax.y=y;
}
inline void rotvy(vec3 &v1,float1 alf)
{
	alf*=rad;
	float1 c1=cos(alf);
	float1 s1=sin(alf);

	float x=v1.x*c1 + v1.z*s1;
	float y=v1.z*c1 - v1.x*s1;
	v1.x=x;
	v1.z=y;
}
inline void rotvz(vec3 &v1,float1 alf)
{
	alf*=rad;
	float1 c1=cos(alf);
	float1 s1=sin(alf);

	float x=v1.x*c1 + v1.y*s1;
	float y=v1.y*c1 - v1.x*s1;
	v1.x=x;
	v1.y=y;
}
inline void rotvx(vec3 &v1,float1 alf)
{
	alf*=rad;
	float1 c1=cos(alf);
	float1 s1=sin(alf);

	float x=v1.y*c1 + v1.z*s1;
	float y=v1.z*c1 - v1.y*s1;
	v1.y=x;
	v1.z=y;
}



inline void rotvec(vec3 &vx,vec3 ax1,float1 c1,float1 s1)
{
	float1 z=dot(vx,ax1);
	vx-=ax1*z;

	vec3 vy=cross(vx,ax1);
	vx=vx*c1 + vy*s1;

	vx+=ax1*z;//	vx=normalize(vx);
}
inline void rotaxis(axis &ax,vec3 ax1,float1 scl)
{
	float1 alf=length(ax1)*scl;
	ax1=normalize(ax1);
	
	float1 c1=cos(alf);
	float1 s1=sin(alf);

	rotvec(ax.x,ax1,c1,s1);
	rotvec(ax.y,ax1,c1,s1);
	rotvec(ax.z,ax1,c1,s1);
}
inline void rotaxis2(axis &ax,vec3 ax1,float1 scl,int op)
{
	float1 alf=length(ax1)*scl;
	ax1=normalize(ax1);
	
	float1 c1=cos(alf);
	float1 s1=sin(alf);

	if(op!=0) rotvec(ax.x,ax1,c1,s1);
	if(op!=1) rotvec(ax.y,ax1,c1,s1);
	if(op!=2) rotvec(ax.z,ax1,c1,s1);
}

#endif

