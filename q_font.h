
#ifndef _QFONTH
#define _QFONTH


#include "q_engine.h"

extern float1 fontscale5;
extern float1 fontsize5;


class c_font:public component //<c_font>
{
const char *nums22=
".ooo."
"o...o"
"o...o"
"o...o"
"o...o"
"o...o"
".ooo."

"....o"
"....o"
"....o"
"....o"
"....o"
"....o"
"....o"

".ooo."
"....o"
"....o"
".ooo."
"o...."
"o...."
"ooooo"

".ooo."
"....o"
"....o"
".ooo."
"....o"
"....o"
".ooo."

"o...."
"o...o"
"o...o"
".ooo."
"....o"
"....o"
"....o"

".ooo."
"o...."
"o...."
".ooo."
"....o"
"....o"
".ooo."

".ooo."
"o...."
"o...."
".ooo."
"o...o"
"o...o"
".ooo."

".ooo."
"....o"
"....o"
"....o"
"....o"
"....o"
"....o"

".ooo."
"o...o"
"o...o"
".ooo."
"o...o"
"o...o"
".ooo."

".ooo."
"o...o"
"o...o"
".ooo."
"....o"
"....o"
".ooo."

"....."
"....."
"....."
"ooooo"
"....."
"....."
"....."

"....."
"....."
"....."
"....."
"....."
"....."
"..o.."
;




const char *nums=
".ooo."
"o...o"
"o...o"
"o...o"
"o...o"
"o...o"
"o...o"
".ooo."

"....o"
"..ooo"
"....o"
"....o"
"....o"
"....o"
"....o"
"....o"

".ooo."
"o...o"
"o...o"
"...o."
"..o.."
".o..."
"o...."
"ooooo"

".ooo."
"o...o"
"....o"
"..oo."
"....o"
"....o"
"o...o"
".ooo."

"...oo"
"..o.o"
".o..o"
"o...o"
"ooooo"
"....o"
"....o"
"....o"

"ooooo"
"o...."
"o...."
"oooo."
"o...o"
"....o"
"o...o"
".ooo."

".ooo."
"o...o"
"o...."
"o.oo."
"oo..o"
"o...o"
"o...o"
".ooo."

"ooooo"
"....o"
"....o"
"...o."
"..o.."
".o..."
"o...."
"o...."

".ooo."
"o...o"
"o...o"
".ooo."
"o...o"
"o...o"
"o...o"
".ooo."

".ooo."
"o...o"
"o...o"
"o..oo"
".oo.o"
"....o"
"o...o"
".ooo."

"....."
"....."
"....."
"ooooo"
"....."
"....."
"....."
"....."

"o...o"
"oo..o"
"oo..o"
"o.o.o"
"o.o.o"
"o..oo"
"o..oo"
"o...o"

"ooooo"
"o...."
"o...."
"ooo.."
"o...."
"o...."
"o...o"
"ooooo"

".ooo."
"o...o"
"o...."
".ooo."
"....o"
"....o"
"o...o"
".ooo."

"o...o"
"o...o"
"o...o"
"o...o"
"o...o"
"o.o.o"
"o.o.o"
".o.o."

"....."
"....."
"....."
"....."
"....."
"....."
"....."
"..o.."


;

const char *nums3=
"..o.."
".o.o."
"o...o"
"ooooo"
"o...o"
"o...o"
"o...o"

"oooo."
"o...o"
"o...o"
"oooo."
"o...o"
"o...o"
"oooo."

".ooo."
"o...o"
"o...."
"o...."
"o...."
"o...o"
".ooo."

"oooo."
"o...o"
"o...o"
"o...o"
"o...o"
"o...o"
"oooo."

"ooooo"
"o...."
"o...."
"ooo.."
"o...."
"o...."
"ooooo"

"ooooo"
"o...."
"o...."
"ooo.."
"o...."
"o...."
"o...."

".ooo."
"o...o"
"o...."
"o..oo"
"o...o"
"o...o"
".ooo."

"o...o"
"o...o"
"o...o"
"ooooo"
"o...o"
"o...o"
"o...o"

"..o.."
"..o.."
"..o.."
"..o.."
"..o.."
"..o.."
"..o.."

"ooooo"
"....o"
"....o"
"....o"
"....o"
"o...o"
".ooo."

"o...o"
"o..o."
"o.o.."
"oo..."
"o.o.."
"o..o."
"o...o"

"o...."
"o...."
"o...."
"o...."
"o...."
"o...."
"ooooo"

"o...o"
"oo.oo"
"o.o.o"
"o...o"
"o...o"
"o...o"
"o...o"

"o...o"
"oo..o"
"o.o.o"
"o.o.o"
"o.o.o"
"o..oo"
"o...o"

".ooo."
"o...o"
"o...o"
"o...o"
"o...o"
"o...o"
".ooo."

"oooo."
"o...o"
"o...o"
"oooo."
"o...."
"o...."
"o...."

".ooo."
"o...o"
"o...o"
"o...o"
"o.o.o"
"o..oo"
".oo.o"

"oooo."
"o...o"
"o...o"
"oooo."
"o.o.."
"o..o."
"o...o"

".ooo."
"o...."
"o...."
".ooo."
"....o"
"....o"
".ooo."

"ooooo"
"..o.."
"..o.."
"..o.."
"..o.."
"..o.."
"..o.."

"o...o"
"o...o"
"o...o"
"o...o"
"o...o"
"o...o"
".ooo."

"o...o"
"o...o"
"o...o"
".o.o."
".o.o."
".o.o."
"..o.."

"o...o"
"o...o"
"o...o"
"o.o.o"
"o.o.o"
"o.o.o"
".o.o."

"o...o"
"o...o"
".o.o."
"..o.."
".o.o."
"o...o"
"o...o"

"o...o"
"o...o"
".o.o."
"..o.."
"..o.."
"..o.."
"..o.."

"ooooo"
"....o"
"...o."
"..o.."
".o..."
"o...."
"ooooo"

"....."
"....."
"....."
"....."
"....."
"....."
"....."

"....."
"....o"
"...o."
"..o.."
".o..."
"o...."
"....."

".ooo."
"o...o"
"o...o"
"o...o"
"o...o"
"o...o"
".ooo."

"....o"
"....o"
"....o"
"....o"
"....o"
"....o"
"....o"

".ooo."
"....o"
"....o"
".ooo."
"o...."
"o...."
"ooooo"

".ooo."
"....o"
"....o"
".ooo."
"....o"
"....o"
".ooo."

"o...."
"o...o"
"o...o"
".ooo."
"....o"
"....o"
"....o"

".ooo."
"o...."
"o...."
".ooo."
"....o"
"....o"
".ooo."

".ooo."
"o...."
"o...."
".ooo."
"o...o"
"o...o"
".ooo."

".ooo."
"....o"
"....o"
"....o"
"....o"
"....o"
"....o"

".ooo."
"o...o"
"o...o"
".ooo."
"o...o"
"o...o"
".ooo."

".ooo."
"o...o"
"o...o"
".ooo."
"....o"
"....o"
".ooo."

"....."
"....."
"....."
"ooooo"
"....."
"....."
"....."
;

public:
//component<c_scene> *scene;
c_font(component *_root)//_base
{
	//scene=static_cast<component<c_scene> *>(_root);
}

#ifdef _FLIGHTSYM
int conv3dchk2(vec3 v1,int &xc,int &yc);
int conv3dchk2b(vec3 v1,int &xc,int &yc);
int conv3dchk3(vec3 v1,int &xc,int &yc);
int conv3dchk3c(vec3 v1,int &xc,int &yc,int &ok);

#endif

void quad(float1 xb,float1 yb,float1 x,float1 y,int col);
void quadr(float1 x,float1 y,int col,float1 c1,float1 s1);
void quadb(float1 x,float1 y,int col);//letra

//#define NUMSH 7
#define NUMSH 8

void drawnum(float1 xb,float1 yb,float1 x2,float1 y2,int n,int col);
void drawnums(float1 xb,float1 yb,int n,int col,int op=0);
void drawnumsD(float1 xb,float1 yb,int n,int col,int op=0);
void drawalp(float1 xb,float1 yb,int n,int col);
void drawalps(float1 xb,float1 yb,const char *buf,int col,int op=0);
void drawnumr(float1 xb,float1 yb,int n,int col,float1 c1,float1 s1);
void drawnumr2(float1 xb,float1 yb,int n,int col,float1 c1,float1 s1);
//muhorizont csak!
void drawnumsr(float1 xb,float1 yb,int n,int col,float1 dx,float1 dy,float1 c1,float1 s1,int op=0);
void update(float1 xtime);
void recvcmd(s_cmd *tmp);
};
#endif



