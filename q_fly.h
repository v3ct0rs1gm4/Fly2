
#ifndef _QFLYH
#define _QFLYH



#include "q_engine.h"



extern float1 skypowerval;
extern int selsky;// 31; 2 5
extern float1 nightvalue;
extern int ewater;
extern int enight;



extern vec3 lookb[16],upb[16],cam_eyeb[16];
extern axis cam_axs[16];
extern qmat cam_matb[16];
extern float1 cam_alfa[16];
extern float1 cam_zoom[16];


extern int selcammode;//0
extern int selcam;
extern int selcam2;//3;//HUD
extern int selcam3;//kicsi
extern float delta_center;
extern int ehelp,ewind;


//extern asefile f18ase,carrierASE;
extern objfile carrierOBJ,f18obj,su35obj,f16obj; // f18obj minden repcsihez kulon,itt preload


//--------------------------------------------------------------------------------
//float1 zoom=yy*2;
extern int xx22,yy22;
extern vec3 camera_trg;

extern s_texture *tx40;
extern s_texture *tx50;

extern int physics;
extern int selils;
extern int ecut;
extern int bumptest;
extern int emount;


extern float1 mapzoom;
extern vec3 gforcecolor;



extern vec3 wind;
extern vec3 carrier_pos,carrier_spd;
extern vec3 carrier_min,carrier_max;
extern axis carrier_ax;
extern vec3 carrier_drawpos;


extern vec3 dumpvec1,dumpvec2,dumpvec3;


extern vec3 clip1,clip2,clip3,clip4;
extern int ccc;
//void draw_forces(int scam);

extern float1 slowtime;
extern int ddx,ddy;

extern float1 waterlevel;
extern int facecc;
extern vec3 dcloud;


extern GLint gscene2[32];
extern GLint gterrain[16];
extern vec3 gcol;


#endif

