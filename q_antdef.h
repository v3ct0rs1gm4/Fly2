
#ifndef _QANTDEFH
#define _QANTDEFH

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <functional>
#include <iostream>
#include <numeric>
#include <limits>
#include <valarray>

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>

#include <assert.h>


#include <pthread.h>
//#include <zlib.h>
#include <assert.h>                                                                   


#if 0
#include <X11/Xlib.h>
#include <X11/Xatom.h>                                                             
#include <X11/keysym.h>                                                                   
#endif


#include <fcntl.h>
#include <linux/joystick.h>




#define __LINUX__
#define __USE_GLX
//#define _USEGLFW

#define border_H  40
//#define  _USEMSAA



#if 1
//1200 850        az kiigazitja 4:3 ra  y+=90   /nem a desktop miatt!!/
//#define xx 1000 #define yy 800
#define xx 1100
#define yy 650
#define xx2 1100
#define yy2 650
#else
#define xx 800
#define yy 600
#endif



#define JOY_DEV0 "/dev/input/js0"
#define JOY_DEV1 "/dev/input/js1"



#define loopd(a,b,c,d) for(int a=b;a<c;a+=d)
#define loopm(a,b,c,d) for(int a=b;a>=(c);a+=(d))

//typedef float f32;
typedef float float1;


#define IDTYPE int64_t


#define qstring std::string
//#define qvector std::vector
#define qvector zzvector
#define qmap std::map



//typedef double f32;
#define rad (M_PI/180.0)
#define iradian (M_PI/180.0)
//#define lo(n) ((n)>>12)
//#define hi(n) ((n)<<12)


#define frnd(n) (n*(float)(randint()%100000000)/100000000.0)
#define rnd(n) (rand()%n)

typedef unsigned char uchar;


#define qassert(A) {if(!A) printf("ASSERT %s %s %s %d\n",#A,__FILE__,__FUNCTION__,__LINE__);}

//#define QDUMPFNC() {printf("FNC  %s %s %d\n",__FILE__,__FUNCTION__,__LINE__);}
#define QDUMPFNC() 

//using namespace std;



//---------------------------------------------------------------------------------------
// flight symulation definitions

//need_glx_versio=3;      ewater GMAP  _USEGMAP emount       !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


#define _FLIGHTSYM

//#define _SURF_EDITOR
//#define _BUMPILLESZT
#if 1
#define _NEWPHY
//#define _NEWPHY2
//test view 
#endif

#define __USE_GLX



#if 0
//mount
#define _USEGMAP
#define _GMAP	
#define _DRAWMOUNT
//#define _USEWATER
#else
//#define __NEWMOUNT
#define _USEWATER

//water + desert
#define _USEGMAP
#define _GMAP	

//#define FASTWATER
#endif


#define __USEJOY	
//#define _LODMAP

#define knots 1.852
#define mph 1.6




//NA   #define _LOADplane_obj
#define _TEST
#define _INSTANTDIE
//#define _DEBUGVECTOR
//#define _SHOOT_TEST

#endif




