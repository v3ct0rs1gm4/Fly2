/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

// int function()        must have RETURN       BUG egyebkent a hulye tovabb megy!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#include "q_opengl.h"


#ifdef _USEGLFW    
GLFWwindow* window5=0;
#endif


Window win=0;
GLXContext ctx=0;//GLint                   att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
Display *display5;
int need_glx_versio=2;


int enaGLerr=1;

void GetErrorWS(const char *func,const char *file,int line, int res)
{
	GLenum ErrorCode = glGetError();
	if(enaGLerr==0) return;//skip glbegin glend
	
	if(ErrorCode != GL_NO_ERROR)
	{
			int needstop=1;
			printf("GetErrorWS:\n	func:%s\n	file:%s:%d\n",func,file,line);

			switch (ErrorCode)
			{
				case GL_INVALID_ENUM:
					printf("GL_INVALID_ENUM\n");
					break;
				case GL_INVALID_VALUE:
					printf("GL_INVALID_VALUE\n");
					break;
				case GL_INVALID_OPERATION:
					printf("GL_INVALID_OPERATION\n");
					break;
				case GL_OUT_OF_MEMORY:
					printf("GL_OUT_OF_MEMORY\n");
					break;
//				#ifdef __OPENGL__
#if 1
                    case GL_STACK_OVERFLOW:
                        printf("GL_STACK_OVERFLOW\n");
                        break;
                    case GL_STACK_UNDERFLOW:
                        printf("GL_STACK_UNDERFLOW\n");
                        break;
					case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
						printf("GL_INVALID_FRAMEBUFFER_OPERATION_EXT\n");
						break;
				#endif
				
				default:
					printf("Unknow Error GL Error: 0x%x  \n",ErrorCode);
					needstop=0;
					break;
			}
			if(needstop) assert(false);//        exit(3);
	}

}


#ifdef _USEGLFW
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


void glxinit5()
{
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, need_glx_versio);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window5 = glfwCreateWindow(xx, yy, "ANT engine", NULL, NULL);
    if (!window5)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window5, key_callback);
    glfwMakeContextCurrent(window5);
    
//    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);
   

}


#else
void glxinit5()//int argc, char ** argv)
{
QDUMPFNC();
        display5 = XOpenDisplay(0);
        if(display5==0) printf("ERROR XOpenDisplay(0) \n");
  	
#if 1        
  
        glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;
     
        const char *extensions = glXQueryExtensionsString(display5, DefaultScreen(display5));
        std::cout << extensions << std::endl;
     
    static int visual_attribs[] =
    {
      //GLX_X_RENDERABLE    , True,            GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_DEPTH_SIZE, 24,//24
//        GLX_STENCIL_SIZE    , 8,
        GLX_DOUBLEBUFFER, true,
        GLX_RED_SIZE, 1,
        GLX_GREEN_SIZE, 1,
        GLX_BLUE_SIZE, 1,
//        GLX_AUX_BUFFERS, 0, 
        None
     };     
//#ifndef  _USEMSAA
#if 0
        static int visual_attribs[] =
        {
            GLX_RENDER_TYPE, GLX_RGBA_BIT,//GLX_RGBA_BIT
            GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
            GLX_DEPTH_SIZE, 24, 
            GLX_DOUBLEBUFFER, true,//true,
            GLX_RED_SIZE, 1,
            GLX_GREEN_SIZE, 1,
            GLX_BLUE_SIZE, 1,
            None
         };

 // --- Find a MSAA FBConfig ---
  static const int visual_attribs[] =
    {
      GLX_X_RENDERABLE    , True,
      GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
      GLX_RENDER_TYPE     , GLX_RGBA_BIT,
      GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
      GLX_RED_SIZE        , 8,
      GLX_GREEN_SIZE      , 8,
      GLX_BLUE_SIZE       , 8,
      GLX_ALPHA_SIZE      , 8,
      GLX_DEPTH_SIZE      , 24,
      GLX_STENCIL_SIZE    , 8,
      GLX_DOUBLEBUFFER    , True,//True,
      GLX_SAMPLE_BUFFERS  , 1,            // <-- MSAA
      GLX_SAMPLES         , 4,            // <-- MSAA
      None
    };
#endif



        std::cout << "Getting framebuffer config" << std::endl;
        int fbcount;
        GLXFBConfig *fbc = glXChooseFBConfig(display5, DefaultScreen(display5), visual_attribs, &fbcount);
        if (!fbc)
        {
            std::cout << "Failed to retrieve a framebuffer config" << std::endl;
            return ;
        }
     
        std::cout << "Getting XVisualInfo" << std::endl;
        XVisualInfo *vi = glXGetVisualFromFBConfig(display5, fbc[0]);
/*		for(int i=0;i<fbcount;i++)
		{
	        XVisualInfo *vi2 = glXGetVisualFromFBConfig(display5, fbc[i]);
				printf("%d  %d %p\n",fbcount,(int)vi2->screen,vi2->visual);
		}*/
     
        XSetWindowAttributes swa;
        std::cout << "Creating colormap" << std::endl;
        swa.colormap = XCreateColormap(display5, RootWindow(display5, vi->screen), vi->visual, AllocNone);
//  swa.background_pixmap = None ;//new
//  swa.background_pixel  = 0    ;
          swa.border_pixel = 0;
        swa.event_mask = StructureNotifyMask;
   
        std::cout << "Creating window" << std::endl;//yy-90
    win = XCreateWindow(display5, RootWindow(display5, vi->screen), 0, 0, xx, yy, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel|CWColormap|CWEventMask, &swa);        
//        win = XCreateWindow(display5, RootWindow(display5, vi->screen), 0, 0, xx, yy, 0, vi->depth, InputOutput, vi->visual, CWColormap|CWEventMask, &swa);//CWBorderPixel|
        if (!win)
        {
            std::cout << "Failed to create window." << std::endl;
            return ;
        }
     
        std::cout << "Mapping window" << std::endl;
        XMapWindow(display5, win);

//	for(;;) { XEvent e; XNextEvent(display5, &e); if (e.type == MapNotify) break; }	qsleep(1.0);

        // Create an oldstyle context first, to get the correct function pointer for glXCreateContextAttribsARB
        GLXContext ctx_old = glXCreateContext(display5, vi, 0, GL_TRUE);//TRUE
        glXCreateContextAttribsARB =  (glXCreateContextAttribsARBProc)glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
        glXMakeCurrent(display5, 0, 0);
        glXDestroyContext(display5, ctx_old);

        if (glXCreateContextAttribsARB == NULL)
        {
            std::cout << "glXCreateContextAttribsARB entry point not found. Aborting." << std::endl;
            return ;
        }
     
        static int context_attribs[] =
        {
            GLX_CONTEXT_MAJOR_VERSION_ARB, need_glx_versio,//3
            GLX_CONTEXT_MINOR_VERSION_ARB, 0,//0
            None
        };

        std::cout << "Creating context" << std::endl;
        ctx = glXCreateContextAttribsARB(display5, fbc[0], NULL, true, context_attribs);
        if (!ctx)
        {
            std::cout << "Failed to create GL3 context." << std::endl;
            return ;
        }

        std::cout << "Making context current" << std::endl;
        glXMakeCurrent(display5, win, ctx);

	XSelectInput(display5, win, StructureNotifyMask| 
			ButtonPressMask| ButtonReleaseMask| 
			KeyPressMask |KeyReleaseMask |
			PointerMotionMask);


		

        std::cout << "Making context OK" << std::endl;

            glClearColor (0.0, 0.5, 1.0, 1.0);
            glClear (GL_COLOR_BUFFER_BIT);
            glXSwapBuffers (display5, win);
       
//             sleep(1);           
#endif            
/*     
            sleep(1);
     
            glClearColor (1, 0.5, 0, 1);
            glClear (GL_COLOR_BUFFER_BIT);
            glXSwapBuffers (display5, win);
  
  */
 
//#ifdef  _USEMSAA
#if 0
 glEnable( GL_MULTISAMPLE ); 
glEnable(GL_POLYGON_SMOOTH); 
glEnable(GL_LINE_SMOOTH);
glEnable(GL_POINT_SMOOTH) ;
#endif



        std::cout << "init OK" << std::endl;

}
#endif









void GetErrorWS(const char *func,const char *file,int line, int res);

#define GL_EXPR_STR5(expr) #expr
#define ERR(expr) {(expr);  GetErrorWS(GL_EXPR_STR5(expr),__FILE__,__LINE__,-1); }
#define ERR2(expr) (expr);  GetErrorWS(GL_EXPR_STR5(expr),__FILE__,__LINE__,-1);
#define ERR3(expr) enaGLerr=0;ERR(expr); enaGLerr=1;

//(expr);




void glxflush()
{
QDUMPFNC();
	// bind the GLSL program	
	// this means that from here the GLSL program attends all OpenGL operations
	// painting a quad
	/*
	glBegin(GL_QUADS);//Multi
		glTexCoord2f(0.0,0.0);	   glVertex3f(-1.0, -1.0, 0.0);
		glTexCoord2f(1.0,0.0);	   glVertex3f( 1.0, -1.0, 0.0);
		glTexCoord2f(1.0,1.0);	   glVertex3f( 1.0,  1.0, 0.0);
		glTexCoord2f(0.0,1.0);	   glVertex3f(-1.0,  1.0, 0.0);
	glEnd();
	*/
	// unbind the GLSL program
	// this means that from here the OpenGL fixed functionality is used
     
    //ERR(glXSwapBuffers (display5, win));
//    ERR(glXSwapBuffers (display5, win));//TODO
#ifdef _USEGLFW
	if(window5)
	{
        glfwSwapBuffers(window5);
        glfwPollEvents();
    }
#else    
    glXSwapBuffers (display5, win);
#endif        


//    printf("%d\n",cc++);
}

void glxend()
{
	ERR(glUseProgram(0));


#ifdef _USEGLFW
    glfwDestroyWindow(window5);
    glfwTerminate();
#else
//	sleep(3);
    ctx = glXGetCurrentContext();
    glXMakeCurrent(display5, 0, 0);
    glXDestroyContext(display5, ctx);
#endif
}


void glxclear(int op)
{
	if(op==0) ERR(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	if(op==1) ERR(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));	// Black Background
	if(op==2) ERR(glClearColor(0.1f, 0.05f, 0.0f, 1.0f));
	if(op==4) ERR(glClearColor(0.03f, 0.03f, 0.03f, 1.0f));
	if(op==6) ERR(glClearColor(0.9f, 0.0f, 0.0f, 1.0f));

	if(op==5) ERR(glClearColor(0.0f, 0.0f, 0.0f, 0.5f));	// Black Background
	//op==11 undef external
	ERR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));	// Clear Screen And Depth Buffer
	ERR(glLoadIdentity());									// Reset The Current Modelview Matrix
	
//	glUseProgram(program_object[0]);
}		
void glxclearzbuf()
{
	glClear( GL_DEPTH_BUFFER_BIT);	
}		
	
void setdrawbuf(int n)
{
	GLenum drbuf[]={
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4,
		GL_NONE,GL_NONE};
	ERR(glDrawBuffers(n,drbuf));
}



