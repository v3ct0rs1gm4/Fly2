
#ifndef _QOPENGLH
#define _QOPENGLH

// int function()        must have RETURN       BUG egyebkent a hulye tovabb megy!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


#define GL_GLEXT_PROTOTYPES

#include <GL/glx.h>
#include <GL/gl.h>
#include <unistd.h>
#include <iostream>
 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

 
#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);


#ifdef _USEGLFW
//#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif

#include "q_engine.h"


//#define glBegin2(A) if(0){
//#define glEnd2() }
#define glBegin2(A)  enaGLerr=0;ERR(glBegin(A))
#define glEnd2()     ERR(glEnd()); enaGLerr=1;
//#define glEnd2()    glEnd()

//#define glBegin2(A)  enaGLerr=0;(glBegin(A))
//#define glEnd2()     (glEnd()); enaGLerr=1;



extern int enaGLerr;

void GetErrorWS(const char *func,const char *file,int line, int res);



#ifdef _USEGLFW
static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void glxinit5();


#else
void glxinit5();//int argc, char ** argv)

#endif



extern Display *display5;
extern Window win;
extern int need_glx_versio;




void GetErrorWS(const char *func,const char *file,int line, int res);

#define GL_EXPR_STR5(expr) #expr
#define ERR(expr) {(expr);  GetErrorWS(GL_EXPR_STR5(expr),__FILE__,__LINE__,-1); }
#define ERR2(expr) (expr);  GetErrorWS(GL_EXPR_STR5(expr),__FILE__,__LINE__,-1);
#define ERR3(expr) enaGLerr=0;ERR(expr); enaGLerr=1;

//(expr);




void glxflush();
void glxend();
void glxclear(int op);
void glxclearzbuf();
void setdrawbuf(int n);


#endif



