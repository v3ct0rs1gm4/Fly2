/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

// int function()        must have RETURN       BUG egyebkent a hulye tovabb megy a fizikailag utanna levo fuggvenyre !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	

/*
SUSE-n NEM JO!  FEDORAN ok 
vonalasan mar jo! TODO
  glGenTextures( 1, &tex );
  glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, tex );
  glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, num_samples, GL_RGBA8, width, height, false );

  glGenFramebuffers( 1, &fbo );
  glBindFramebuffer( GL_FRAMEBUFFER, fbo );
  glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, tex, 0 );

  GLenum status = glCheckFramebufferStatus( target );
*/

#include "q_engine.h"


void error(const char *ss) {printf("ERROR %s  \n",ss);};
void error2(const char *ss,const char *ss2) {printf("ERROR %s %s \n",ss,ss2);};



int joy_fd0;
int joy_fd1;
struct JS_DATA_TYPE js0;
struct JS_DATA_TYPE js1;
int joyx0,joyy0, 	joyxb0=-1234,joyyb0=-1234;
int joyx1,joyy1, 	joyxb1=-1234,joyyb1=-1234,
	isjoy=0,ejoy=0,
	jbutt0=0,jbutt1=0,jbutt0_old=0,jbutt1_old=0,
	jbutpress0=0,jbutpress1=0;

int joyx0_min=-1,joyx0_max=1,
	joyy0_min=-1,joyy0_max=1,
	joyx1_min=-1,joyx1_max=1,
	joyy1_min=-1,joyy1_max=1;



     


int needfloatlinear=0;

//--------------------------------------------------------------------------------



s_qscript qscript;

//--------------------------------------------------------------------------------
vec3 look,up(0,1,0),eye,light;
qmat cam_mat;
qmat cam_mat_shad;
qmat cam_mat_shad2;
axis cam;



int hmap=0;
int xmap=920;
//#define xmap 920
//#define xmap 2800



    


struct s_rtg
{
	GLuint id_fbo;
	GLuint id_rbo;
	int type;
};
std::unordered_map<int64_t,s_rtg> rtgs;
uchar *mem=0;
uchar *mem2=0;
uchar *mem3=0;

float1 gametime=0;
//float1 xtime;


std::unordered_map<int64_t,GLint> gscene;
fix_array<IDTYPE,5000> idtabl;



unsigned short head[256];
vec3 *mapv;

std::unordered_map<int64_t,s_texture> textures;

//--------------------------------------------------------------------------------



void qsleep(float1 dt)
{
	timespec ti1,ti2;

	ti1.tv_sec=0;
	ti1.tv_nsec=(int)(1e9*dt);

	nanosleep(&ti1,&ti2);
}
double q_getcpuclock()
{
#ifdef __LINUX__
        timespec tim;
        timespec timres;
        clockid_t cid=0;
        
//      clock_getcpuclockid(0,&cid);
//      clock_gettime(cid,&tim);
//      clock_getres(CLOCK_REALTIME,&timres);

        clock_gettime(CLOCK_MONOTONIC,&tim);    
//      clock_gettime(CLOCK_REALTIME,&tim);

        double ti=(double)tim.tv_sec + (double)tim.tv_nsec/1e9;
//printf("%d %d \n",timres.tv_sec,timres.tv_nsec);
        return ti;
#else
//	return (dd)GetTickCount()*0.001f;
        LARGE_INTEGER   freq;
        LARGE_INTEGER   counter;

        QueryPerformanceFrequency( &freq );
        QueryPerformanceCounter( &counter );
        
        return ( static_cast< double >( counter.QuadPart ) / 
                        static_cast< double >( freq.QuadPart ) );// * 100.0f
#endif
}
double ti22=0;
int averfpscc=0;
double averfps=3.0;

double xtimer(const char *ss)
{
	double ti1=q_getcpuclock();//zsirtimer
	double deltaSeconds=ti1-ti22;
	ti22=ti1;

	if(deltaSeconds==0) deltaSeconds=0.000001;
	if(ss) printf("%s : %f sec %f fps\n",ss,deltaSeconds,1.0/deltaSeconds);
	
	averfps+=(deltaSeconds);
	averfpscc++;
	if(averfps>5.0)
	{
		printf("%d FPS\n",(int)((float)averfpscc/averfps));
		averfpscc=0;
		averfps=0;
	}
	return deltaSeconds;
}
double xtimer(const char *ss,float scl)
{
	double ti1=q_getcpuclock();//zsirtimer
	double deltaSeconds=ti1-ti22;
	ti22=ti1;

	if(deltaSeconds==0) deltaSeconds=0.000001;
	if(ss) printf("%s : %f sec \n",ss,deltaSeconds*scl);
	
	averfps+=(deltaSeconds);
	averfpscc++;
	if(averfps>5.0)
	{
		printf("%d FPS\n",(int)((float)averfpscc/averfps));
		averfpscc=0;
		averfps=0;
	}
	return deltaSeconds;
}
#if 0
void qprintf(int x,int y,char *format, ...)
{
  va_list args;
  char    buffer[255];

  va_start(args,format);
  strcpy(buffer + vsprintf(buffer,format,args), "\r\n");
//  sprintf(buffer,format,args);
}
#endif

int isletter(char u)
{
	if(u>='0')	if(u<='9') return 1;
	if(u>='a')	if(u<='z') return 1;
	if(u>='A')	if(u<='Z') return 1;
	return 0;
}
//--------------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//--------------------------------------------------------------------------------
	
int64_t filesize(FILE *ff)
{
	int64_t size=0;
	int64_t pos = ftell(ff);
	fseek(ff,0,SEEK_END);
	size = ftell(ff);
	fseek(ff,pos,SEEK_SET);

	return size;
}

int qload(const char *ss,void *buf,int n)
{
QDUMPFNC();
	FILE *ff=fopen(ss,"rb");
	if(ff==NULL) {printf("ERRORL:%s\n",ss);return 0;}
	int h=fread(buf,1,n,ff);
	fclose(ff);
	printf("%s loaded %d\n",ss,h);
	
	return h;
}
int qload(const char *ss,void **buf)
{
QDUMPFNC();
	FILE *ff=fopen(ss,"rb");
	if(ff==NULL) {printf("ERRORL:%s\n",ss);return 0;}
	int size=filesize(ff);
	*buf=new char[size+1];
	int h=fread(*buf,1,size,ff);
	fclose(ff);
	printf("%s loaded %d\n",ss,h);
	
	return h;
}

void printProgramInfoLog(GLuint obj)
{
	GLint infologLength = 0, charsWritten = 0;
	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 2) {
		GLchar* infoLog = new GLchar [infologLength];
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		std::cerr << infoLog << std::endl;
		delete infoLog;
	}
}




//------------------------------------------------------------------------------



int cc=0;




GLuint glxtexture(void *mem,int xc,int yc,int col,int wrap,int needmip)//rtg wrap=1,int needmip=1
{
QDUMPFNC();
    GLuint texture=0;
printf("glxtexture %d %d %d \n",xc,yc,col);

	ERR(glActiveTexture(GL_TEXTURE0));
    ERR(glGenTextures( 1, &texture ));
     if(col==9)    	{ERR(glBindTexture( GL_TEXTURE_3D, texture ));}
     else    		{ERR(glBindTexture( GL_TEXTURE_2D, texture ));}
    ERR(glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL ));//MODULATE     DECAL MODULATE  

    
//                     GL_LINEAR_MIPMAP_NEAREST );   //NEAREST
//NA    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//NEAREST);
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	if(col>4)
	{
    	if(needfloatlinear && col!=5)
    	{
		    ERR(glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));//NEAREST);
		    ERR(glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));//GL_NONE ); //LINEAR NEAREST

		    ERR(glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));//NEAREST);
		    ERR(glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));//GL_NONE ); //LINEAR NEAREST
	    }
	    else
	    {
	    	ERR(glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	    	ERR(glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	    	ERR(glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	    	ERR(glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	    }

//    	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NONE);
//    	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NONE);
    }
    else
    {
//nomipmap    
//	    ERR(glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
//	    ERR(glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST )); //LINEAR NEAREST  NONE?
//mipmap	    
	    ERR(glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));//NEAREST);
	    ERR(glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));//GL_NONE ); //LINEAR NEAREST
	    ERR(glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));//NEAREST);
	    ERR(glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));//GL_NONE ); //LINEAR NEAREST
    }
    ERR(glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  wrap ? GL_REPEAT : GL_CLAMP ));
    ERR(glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  wrap ? GL_REPEAT : GL_CLAMP ));
    ERR(glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S,  wrap ? GL_REPEAT : GL_CLAMP ));
    ERR(glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T,  wrap ? GL_REPEAT : GL_CLAMP ));

  //	gluBuild2DMipmaps( GL_TEXTURE_2D, 4, xc, yc,GL_RGBA, GL_UNSIGNED_BYTE, mem );
// 	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    if(col==1) {ERR(glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,xc,yc,0,GL_LUMINANCE, GL_UNSIGNED_BYTE,mem));}
    if(col==3) {ERR(glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,xc,yc,0,GL_RGB, GL_UNSIGNED_BYTE,mem));}
    if(col==4) {ERR(glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,xc,yc,0,GL_RGBA, GL_UNSIGNED_BYTE,mem));}
    
//    if(col==5) glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT16,xc,yc,0,GL_DEPTH_COMPONENT, GL_SHORT,mem);
    if(col==5) 
    {
#if 1
    ERR(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,GL_COMPARE_R_TO_TEXTURE ));	    
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,GL_COMPARE_REF_TO_TEXTURE_EXT );	    
    
    ERR(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_FUNC,GL_LEQUAL ));	    
    ERR(glTexParameteri(GL_TEXTURE_2D,GL_DEPTH_TEXTURE_MODE,GL_INTENSITY ));	    
#endif    

//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);//GL_LEQUAL
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);//sample2DShadow
#if 1
    ERR(glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT32F,xc,yc,0,GL_DEPTH_COMPONENT, 
//    glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT24,xc,yc,0,GL_DEPTH_COMPONENT, 
    GL_FLOAT
//    GL_UNSIGNED_INT
    ,mem));//32F 24
#else    
    ERR(glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT16,xc,yc,0,GL_DEPTH_COMPONENT, 
    GL_UNSIGNED_SHORT
    ,mem));//32F 24
#endif    
//(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT32F,xc,yc,0,GL_DEPTH_COMPONENT, GL_FLOAT,mem);
//	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,GL_COMPARE_REF_TO_TEXTURE );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,   GL_CLAMP_TO_EDGE );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,   GL_CLAMP_TO_EDGE );
	    //GL_NONE ); //
	    

	}

//
    if(col==6) {ERR(glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,xc,yc,0,GL_RGBA, GL_FLOAT,mem));}//LUMINANCE  vec3 kell!
    if(col==7) {ERR(glTexImage2D(GL_TEXTURE_2D,0,GL_RGB32F,xc,yc,0,GL_RGB, GL_FLOAT,mem));}
    if(col==77) {ERR(glTexImage2D(GL_TEXTURE_2D,0,GL_RGB16F,xc,yc,0,GL_RGB, GL_HALF_FLOAT,mem));}    //rayt5
    
//    if(col==6) glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,xc,yc,0,GL_RGB, GL_SHORT,mem);
//    if(col==6) glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,xc,yc,0,GL_RGB, GL_UNSIGNED_BYTE,mem);
    if(col==8) {ERR(glTexImage2D(GL_TEXTURE_2D,0,GL_R32F,xc,yc,0,GL_LUMINANCE,   GL_FLOAT   ,mem));}//32F 24      ???????//
//    if(col==8) glTexImage2D(GL_TEXTURE_2D,0,GL_R32F,xc,yc,0,GL_RGB,   GL_FLOAT   ,mem);//32F 24      ???????//

    if(col==9) {ERR(glTexImage3D(GL_TEXTURE_3D,0,GL_RGBA32F,xc,yc,xc,0,GL_RGBA, GL_FLOAT,mem));} // oct
    
    if(needmip && col<=4)	    
    {
   //ERR(glGenerateMipmap(texture));//??????? TODO

#if 0
//mip    
	int lev=0;
    uchar *mem1=new uchar[xc*yc*4];
    memcpy(mem1,mem,xc*yc*3);
    uchar *mem2=new uchar[xc*yc*4];
    
    while(xc>1 && yc>1)
    {
	    xc/=2; yc/=2; lev++;
	    if(xc>1 && yc>1)
	    {
	    loopd(y,0,yc,1)
	    loopd(x,0,xc,1)
	    {
	    	int m1=(y*xc*2+x*2)*3;
	    	int m2=(y*xc+x)*3;
	    	
	    	loopd(i,0,3,1)
		    	mem2[m2+i]=(mem1[m1+i]+mem1[m1+3+i]+mem1[m1+3+xc*3+i]+mem1[m1+xc*3+i])/4;
	    }
	    ERR(glTexImage2D(GL_TEXTURE_2D,lev,GL_RGB,xc,yc,0,GL_RGB, GL_UNSIGNED_BYTE,mem2));
//	    glTexSubImage2D(GL_TEXTURE_2D,lev,0,0,xc,yc,GL_RGB, GL_UNSIGNED_BYTE,mem2);
	    }

	    xc/=2; yc/=2; lev++;
	    if(xc>1 && yc>1)
	    {
	    loopd(y,0,yc,1)
	    loopd(x,0,xc,1)
	    {
	    	int m1=(y*xc*2+x*2)*3;
	    	int m2=(y*xc+x)*3;
	    	
	    	loopd(i,0,3,1)
		    	mem1[m2+i]=(mem2[m1+i]+mem2[m1+3+i]+mem2[m1+3+xc*3+i]+mem2[m1+xc*3+i])/4;
	    }
	    ERR(glTexImage2D(GL_TEXTURE_2D,lev,GL_RGB,xc,yc,0,GL_RGB, GL_UNSIGNED_BYTE,mem2));
//	    glTexSubImage2D(GL_TEXTURE_2D,lev,0,0,xc,yc,GL_RGB, GL_UNSIGNED_BYTE,mem1);
	    }
    }
    
    delete [] mem2;
#endif
   }
   
   if(col==9)   glBindTexture( GL_TEXTURE_3D, 0 );
    else    	glBindTexture( GL_TEXTURE_2D, 0 );
    
    return texture;
}

GLuint glxtextureb(void *mem,int xc,int yc,int col,int wrap)//wrap=1
{
QDUMPFNC();
    GLuint texture=0;

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );//MODULATE     DECAL MODULATE  

    
//                     GL_LINEAR_MIPMAP_NEAREST );   //NEAREST
//NA    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//NEAREST);
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//NEAREST);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NONE ); //LINEAR NEAREST
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  wrap ? GL_REPEAT : GL_CLAMP );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  wrap ? GL_REPEAT : GL_CLAMP );

  //	gluBuild2DMipmaps( GL_TEXTURE_2D, 4, xc, yc,GL_RGBA, GL_UNSIGNED_BYTE, mem );
// 	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    if(col==3) glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,xc,yc,0,GL_RGB, GL_UNSIGNED_BYTE,mem);
    if(col==4) glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,xc,yc,0,GL_RGBA, GL_UNSIGNED_BYTE,mem);
//    glGenerateMipmap(texture);

#if 0
//mip    
	int lev=0;
    uchar *mem1=new uchar[xc*yc*4];
    memcpy(mem1,mem,xc*yc*3);
    uchar *mem2=new uchar[xc*yc*4];
    
    while(xc>1 && yc>1)
    {
	    xc/=2; yc/=2; lev++;
	    if(xc>1 && yc>1)
	    {
	    loopd(y,0,yc,1)
	    loopd(x,0,xc,1)
	    {
	    	int m1=(y*xc*2+x*2)*3;
	    	int m2=(y*xc+x)*3;
	    	
	    	loopd(i,0,3,1)
		    	mem2[m2+i]=(mem1[m1+i]+mem1[m1+3+i]+mem1[m1+3+xc*3+i]+mem1[m1+xc*3+i])/4;
	    }
	    glTexImage2D(GL_TEXTURE_2D,lev,GL_RGB,xc,yc,0,GL_RGB, GL_UNSIGNED_BYTE,mem2);
//	    glTexSubImage2D(GL_TEXTURE_2D,lev,0,0,xc,yc,GL_RGB, GL_UNSIGNED_BYTE,mem2);
	    }

	    xc/=2; yc/=2; lev++;
	    if(xc>1 && yc>1)
	    {
	    loopd(y,0,yc,1)
	    loopd(x,0,xc,1)
	    {
	    	int m1=(y*xc*2+x*2)*3;
	    	int m2=(y*xc+x)*3;
	    	
	    	loopd(i,0,3,1)
		    	mem1[m2+i]=(mem2[m1+i]+mem2[m1+3+i]+mem2[m1+3+xc*3+i]+mem2[m1+xc*3+i])/4;
	    }
	    glTexImage2D(GL_TEXTURE_2D,lev,GL_RGB,xc,yc,0,GL_RGB, GL_UNSIGNED_BYTE,mem2);
//	    glTexSubImage2D(GL_TEXTURE_2D,lev,0,0,xc,yc,GL_RGB, GL_UNSIGNED_BYTE,mem1);
	    }
    }
    
    delete [] mem2;
#endif
    glBindTexture( GL_TEXTURE_2D, 0 );
    
    return texture;
}

qstring int2string(int a)
{
	char ss[92];
	sprintf(ss,"%d",a);
	return qstring(ss);
}
void qsetrtg(qstring name,IDTYPE &id)
{
	int type=0;
QDUMPFNC();
	if(name=="display" ||name=="DISPLAY" )
	{
		ERR(glBindFramebuffer(GL_FRAMEBUFFER, NULL));
	}
	else
	{
//		if(id==-1)
		{
			id=rtgs[qhash64(name)].id_fbo;
			type=rtgs[qhash64(name)].type;
			if(id==0) printf("ERROR %s \n",name.c_str());
//			printf("!!!!!\n");//test OK
		}

		if(type==9)		{ERR(glBindImageTexture(0, id, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA32F));}// oct  
		else			{ERR(glBindFramebuffer(GL_FRAMEBUFFER, id));}//rtgs[name].id_fbo);
	}
}
//nem lehet 20 nel hoszabb nev!!!!!!!!!!!!
void create_rtg(qstring nm,int floatrtg,int nattach,int x2,int y2,int needDeep)//tx +1 +2 attachment!!!!!!!!11 needDeep=1
{	
QDUMPFNC();
	s_rtg tmp;
	qstring tx;
	int u=0;
	
	ERR(glGenFramebuffers(1, &tmp.id_fbo)); 
	ERR(glBindFramebuffer(GL_FRAMEBUFFER, tmp.id_fbo));
printf("create_rtg %s / %d ",nm.c_str(),floatrtg);

	if(mem==0) mem=new uchar[x2*y2*4];
	if(floatrtg==1)if(mem2==0) {mem2=new uchar[x2*y2*4*4];memset(mem2,0,x2*y2*4*4);}
	if(floatrtg==9)if(mem3==0) {mem3=new uchar[x2*y2*x2*4*4];memset(mem2,0,x2*y2*x2*4*4);}
	
	tx=nm+int2string(u); u++;
	int64_t txi=qhash64(tx);
	if(floatrtg==1) textures[txi  ].id=glxtexture(mem2,x2,y2,6,0,0);//RGBA32f
	else//!!!!! BUG
	if(floatrtg==2) textures[txi  ].id=glxtexture(mem2,x2,y2,8,0,0);//deepth32f
	else        
	if(floatrtg==9) textures[txi  ].id=glxtexture(mem3,x2,y2,9,0,0);//3d?
	else         textures[txi  ].id=glxtexture(mem,x2,y2,4);
	
	if(floatrtg==9)	{ERR(glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 ,GL_TEXTURE_3D,textures[txi  ].id ,0,0)); }
	else			{ERR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 ,GL_TEXTURE_2D,textures[txi  ].id ,0)); }

	if(nattach>=2)
	{//elso nem float, a tobbi az
		tx=nm+int2string(u); u++;
		int64_t txi=qhash64(tx);
		textures[txi].id=glxtexture(mem2,x2,y2,6);
		ERR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1 ,GL_TEXTURE_2D,textures[txi].id ,0)); 
	}
	if(nattach>=3)
	{
		tx=nm+int2string(u); u++;
		int64_t txi=qhash64(tx);
		textures[txi].id=glxtexture(mem2,x2,y2,6);
		ERR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2 ,GL_TEXTURE_2D,textures[txi].id ,0)); 
	}
	tx=nm+int2string(u); u++;
	txi=qhash64(tx);
	if(needDeep)
	{
		textures[txi].id=glxtexture(mem,x2,y2,5);//3!
		ERR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT  ,GL_TEXTURE_2D,textures[txi].id ,0 )); 
	}


					
	ERR(glGenRenderbuffers(1, &tmp.id_rbo));
	ERR(glBindRenderbuffer(GL_RENDERBUFFER, tmp.id_rbo));//GL_DEPTH_COMPONENT16

	ERR(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, x2,y2));//24 nem kell
	ERR(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT , GL_RENDERBUFFER, tmp.id_rbo));


	GLenum status;
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE)
	{
		switch(status)
		{
					case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");					
						//An attached render buffer or texture is not complete.
						break;

					case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
						//There is not at least one attached image.
printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n");					
						break;

							case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
							//All attached images do not have the same width and height.
printf("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT\n");					
							break;
						case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
							//All images attached to the attachment points GL_COLOR_ATTACHMENT0_EXT through GL_COLOR_ATTACHMENTn_EXT do not have the same internal format.
printf("GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT\n");					
							break;

					case GL_FRAMEBUFFER_UNSUPPORTED:
printf("GL_FRAMEBUFFER_UNSUPPORTED\n");					
						break;
			};
//			glDeleteFramebuffers(1,&tmp.id_fbo);
	}

    ERR(glBindFramebuffer(GL_FRAMEBUFFER, 0));//tmp.id_fbo); 
    tmp.type=floatrtg;
	rtgs[qhash64(nm)]=tmp;
	

/*		glBindFramebuffer(GL_READ_FRAMEBUFFER,  src->id_fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->id_fbo);
		glBlitFramebuffer(0, 0, src->Width, src->Height,
						  0, 0, this->Width, this->Height,
						  GL_DEPTH_BUFFER_BIT,
						  GL_NEAREST);//GL_LINEAR
*/
}
void create_rtg(qstring nm,int floatrtg,int nattach,int needDeep)//tx +1 +2 attachment!!!!!!!!11 needDeep=1
{
QDUMPFNC();
	create_rtg(nm,floatrtg,nattach,xx2,yy2,needDeep);
}

#if 0
#include <GL/glx.h>
#include <GL/gl.h>
#include <unistd.h>
#include <iostream>
 
#define GLX_CONTEXT_MAJOR_VERSION_ARB		0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB		0x2092
typedef GLXContext (*GLXCREATECONTEXTATTRIBSARBPROC)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
 
 
int main (int argc, char ** argv){
	Display *display5 = XOpenDisplay(0);
 
	int nelements;
	GLXFBConfig *fbc = glXChooseFBConfig(display5, DefaultScreen(display5), 0, &nelements);
 
	static int attributeList[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, None };
	XVisualInfo *vi = glXChooseVisual(display5, DefaultScreen(display5),attributeList);
 
	XSetWindowAttributes swa;
	swa.colormap = XCreateColormap(display5, RootWindow(display5, vi->screen), vi->visual, AllocNone);
	swa.border_pixel = 0;
	swa.event_mask = StructureNotifyMask;
	Window win = XCreateWindow(display5, RootWindow(display5, vi->screen), 0, 0, 100, 100, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel|CWColormap|CWEventMask, &swa);
 
	XMapWindow (display5, win);
 
	//oldstyle context:
	//	GLXContext ctx = glXCreateContext(display5, vi, 0, GL_TRUE);
 
	std::cout << "glXCreateContextAttribsARB " << (void*) glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB") << std::endl;
	GLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = (GLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
 
	int attribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
		GLX_CONTEXT_MINOR_VERSION_ARB, 0,
		0};
 
	GLXContext ctx = glXCreateContextAttribsARB(display5, *fbc, 0, true, attribs);
 
	glXMakeCurrent (display5, win, ctx);
 
		glClearColor (0, 0.5, 1, 1);
		glClear (GL_COLOR_BUFFER_BIT);
		glXSwapBuffers (display5, win);
 
		sleep(1);
 
		glClearColor (1, 0.5, 0, 1);
		glClear (GL_COLOR_BUFFER_BIT);
		glXSwapBuffers (display5, win);
 
		sleep(1);
 
	ctx = glXGetCurrentContext(); 
	glXDestroyContext(display5, ctx); 
	}
	
	
	
	
	
	
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
vec3 qbezier(vec3 v1,vec3 v2,vec3 v3,vec3 v4,float1 t1)
{
	float1 t2=t1*t1;
	float1 t3=t2*t1;
	float1 u1=1.0-t1;
	float1 u2=u1*u1;
	float1 u3=u2*u1;
	
	return v1*u3 + v2*t1*u2*3.0 + v3*t2*u1*3.0 + v4*t3;
}

void rotatev(vec3 &v1,vec3 rotz,float1 alf)
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



template<typename T>
class qarray
{
public:
	vector<T*> ptr;
	int gsize,fullsize,addr;
	
#define	SLICE 64
#define SLICE_SHIFT 6
	
	qarray() {ptr.clear();gsize=0;fullsize=0;};

	~qarray() {	clear();};
	int size() {return gsize;};

	void push_back(T v) 
	{
		int indx=gsize;

		gsize++;
		extend(gsize);
		
		addr=indx>>SLICE_SHIFT;
		indx=indx&(SLICE-1);
		
		ptr[addr][indx]=v; 
	};
	void extend(int newsize)
	{
		newsize/=SLICE;
		newsize+=1;
		newsize*=SLICE;

		if(newsize>fullsize)
		{
			int ii=fullsize>>6;
			addr=newsize>>6;
			
			loopd(i,ii,addr,1)
			{
				ptr.push_back(new T[SLICE]);
				memset(ptr.back(),0,sizeof(T)*SLICE);
			}

			fullsize=newsize;
		}
	}
	void clear() 
	{
		loopd(i,0,ptr.size(),1)
			if(ptr[i]) delete [] ptr[i];

		ptr.clear();
		gsize=0;
		fullsize=0;
	};


	T& operator[](int indx) 
	{
		if(indx<0) {printf("indx<0\n");exit(3);}
		if(indx>10000) {printf("indx>10000\n");exit(3);}
		
		if(indx>=gsize) {extend(indx+1);gsize=indx+1;}//printf("operator [] big indx\n");

		addr=indx>>SLICE_SHIFT;
		indx=indx&(SLICE-1);
		
		return ptr[addr][indx];
	};
};



vec3 gettxctang(vec3 w2,vec3 *tang)
{
	vec3 stxc;
    
	stxc.x=tang[0].x + dot(tang[1],w2);//tangent space
	stxc.y=tang[0].y + dot(tang[2],w2);
    stxc.z=0;

    return stxc;
}
vec3 gettxctang3(vec3 w2,vec3 *tang)
{
	vec3 stxc;
    
	stxc.x=tang[0].x + dot(tang[1],w2);//tangent space
	stxc.y=tang[0].y + dot(tang[2],w2);
	stxc.z=tang[0].z + dot(tang[3],w2);

//	stxc=tang[0] + tang[3]*dot(tang[1],w2) + tang[4]*dot(tang[2],w2);

    return stxc;
}
void calc_tangent(vec3 &v1,vec3 &v2,vec3 &v3,vec3 &txc1,vec3 &txc2,vec3 &txc3, vec3 *_tang)
{
	vec3 nx=v2-v1;//keyword in shader tsw
	vec3 ny=v3-v1;
	vec3 nnx=normalize(nx);

	vec3 ux=txc2-txc1;
	vec3 uy=txc3-txc1;
//	vec3 uux=normalize(ux);

	vec3 nyd=nnx*dot(nnx,ny);
	ny=ny- nyd;//levagja a kilogo Y reszt!
//	ny=ny- nnx*dot(nnx,ny);;//levagja a kilogo Y reszt!
//	uy=uy- uux*dot(uux,uy);//2d ben ugyanugy!  BUG ha nem uniform mapping!!
	uy-=ux*length(nyd)/length(nx);//nx tavnal ux vonodik le!!!!!!!!!
	
/*
//	nx=normalize(nx)/length(nx);//ha nx 10, akkor 0.1el kell szorozni, hogy 1 legyen!
//	ny=normalize(ny)/length(ny);
*/
	nx=nx/dot(nx,nx);//ugyan az!
	ny=ny/dot(ny,ny);
	vec3 nxx=nx*ux.x + ny*uy.x;//a kesobbi stxc= bol w1 kiemelve, egyszerusites!
	vec3 nyy=nx*ux.y + ny*uy.y;
	//mivel stxc.x=ux.x*( nx.x*w1.x + nx.y*w1.y + nx.z*w1.z) +uy.x*( ny.x*w1.x + ny.y*w1.y + ny.z*w1.z) ;
/*
//	vec3 w1=sobj - v1;
//	stxc=txc1 + ux*dot(nx,w1) + uy*dot(ny,w1);//tangent space witout matrix ! OKE
    */

	_tang[0]=txc1;
	_tang[1]=nxx;
	_tang[2]=nyy;


/*
tang[i]=txc1;//test
tang[i+1]=txc2;
tang[i+2]=txc3;
*/
}
void calc_tangent3(vec3 &v1,vec3 &v2,vec3 &v3,vec3 &txc1,vec3 &txc2,vec3 &txc3, vec3 *_tang)
{
	vec3 nx=v2-v1;//keyword in shader tsw
	vec3 ny=v3-v1;
	vec3 nnx=normalize(nx);

	vec3 ux=txc2-txc1;
	vec3 uy=txc3-txc1;
	vec3 uux=normalize(ux);

	ny=ny- nnx*dot(nnx,ny);;//levagja a kilogo Y reszt!
	uy=uy- uux*dot(uux,uy);//2d ben ugyanugy!
//	uy=uy- nnx*dot(nnx,uy);//2d ben ugyanugy!

/*
//	nx=normalize(nx)/length(nx);//ha nx 10, akkor 0.1el kell szorozni, hogy 1 legyen!
//	ny=normalize(ny)/length(ny);
*/
	nx=nx/dot(nx,nx);//ugyan az!
	ny=ny/dot(ny,ny);
	vec3 nxx=nx*ux.x + ny*uy.x ;//a kesobbi stxc= bol w1 kiemelve, egyszerusites!
	vec3 nyy=nx*ux.y + ny*uy.y;
	vec3 nzz=nx*ux.z + ny*uy.z;
//mivel	
//stxc.x=txc1.x+ux.x*( nx.x*w1.x + nx.y*w1.y + nx.z*w1.z) +uy.x*( ny.x*w1.x + ny.y*w1.y + ny.z*w1.z) ;
//stxc.x=txc1.x+ux.x*nx.x*w1.x + ux.x*nx.y*w1.y + ux.x*nx.z*w1.z +uy.x*ny.x*w1.x + uy.x*ny.y*w1.y + uy.x*ny.z*w1.z ;
//stxc.x=txc1.x+(ux.x*nx.x+uy.x*ny.x)*w1.x + (ux.x*nx.y+ uy.x*ny.y)*w1.y + (ux.x*nx.z+ uy.x*ny.z)*w1.z ;
/*
x axis!
1=dot(nxx,w1)
0=dot(nyy,w1)
0=dot(nzz,w1)

1=nxx.x*w1.x + nxx.y*w1.y + nxx.z*w1.z
0=nyy.x*w1.x + nyy.y*w1.y + nyy.z*w1.z
0=nzz.x*w1.x + nzz.y*w1.y + nzz.z*w1.z

w1.x=nxx.x*1 + nyy.x*0 + nzz.x*0
w1.y=nxx.y*1 + nyy.y*0 + nzz.y*0
w1.z=nxx.z*1 + nyy.z*0 + nzz.z*0


//	vec3 w1=sobj - v1;
//	stxc=txc1 + ux*dot(nx,w1) + uy*dot(ny,w1);//tangent space witout matrix ! OKE
    */

	_tang[0]=txc1;
	_tang[1]=nxx;
	_tang[2]=nyy;
	_tang[3]=nzz;
/*
	_tang[1]=nx;
	_tang[2]=ny;
	_tang[3]=ux;
	_tang[4]=uy;*/

/*
tang[i]=txc1;//test
tang[i+1]=txc2;
tang[i+2]=txc3;
*/
}



void qsleep(float1 dt)
{
	timespec ti1,ti2;

	ti1.tv_sec=0;
	ti1.tv_nsec=(int)(1e9*dt);

	nanosleep(&ti1,&ti2);
}
double q_getcpuclock()
{
#ifdef __LINUX__
        timespec tim;
        timespec timres;
        clockid_t cid=0;
        
//      clock_getcpuclockid(0,&cid);
//      clock_gettime(cid,&tim);
//      clock_getres(CLOCK_REALTIME,&timres);

        clock_gettime(CLOCK_MONOTONIC,&tim);    
//      clock_gettime(CLOCK_REALTIME,&tim);

        double ti=(double)tim.tv_sec + (double)tim.tv_nsec/1e9;
//printf("%d %d \n",timres.tv_sec,timres.tv_nsec);
        return ti;
#else
//	return (dd)GetTickCount()*0.001f;
        LARGE_INTEGER   freq;
        LARGE_INTEGER   counter;

        QueryPerformanceFrequency( &freq );
        QueryPerformanceCounter( &counter );
        
        return ( static_cast< double >( counter.QuadPart ) / 
                        static_cast< double >( freq.QuadPart ) );// * 100.0f
#endif
}
double ti22=0;

double xtimer(const char *ss)
{
	double ti1=q_getcpuclock();//zsirtimer
	double deltaSeconds=ti1-ti22;
	ti22=ti1;

	if(deltaSeconds==0) deltaSeconds=0.000001;
	if(ss) printf("%s : %f sec %f fps\n",ss,deltaSeconds,1.0/deltaSeconds);
	return deltaSeconds;
}
/*
void load(const char *name, void *buf,int size)
{
	FILE *ff=fopen(name,"rb");
	if(ff<=0) printf("ERROR %s \n",name);
	else
	{
		fread(buf,1,size,ff);
		fclose(ff);
	}
}
*/

	
#endif	
	

GC gc;
int mbutpress[8],mbut[8],mbute[8],mx,my,dmx,dmy,key,qwheel=0,mx32=0,my32=0;
short tkey[100000]; 
XEvent event;              


void qevent(void (*fnc)())
{
QDUMPFNC();
#ifndef _USEGLFW
	XButtonEvent *xbe=(XButtonEvent *)&event;
	
	loopd(i,0,8,1) mbute[i]=mbut[i];
	
	dmx=0;
	dmy=0;
	
	//mbut=0;
	int u;
#ifdef __USEJOY	
//		 read( joy_fd1, &js1, JS_RETURN ) ;//xchange  0,1

		if( read( joy_fd0, &js0, JS_RETURN ) != JS_RETURN )
		{
			isjoy=0;
//			printf( "\nFailed read\n" );
		}
		else
		{
			isjoy=1;//  ON-OFF

//printf("%x \n",js1.buttons);
			if(joyxb0==-1234) joyxb0=js0.x;
			if(joyyb0==-1234) joyyb0=js0.y;
			if(joyxb1==-1234) joyxb1=js1.x;
			if(joyyb1==-1234) joyyb1=js1.y;

			joyx0=js0.x-joyxb0;
			joyy0=js0.y-joyyb0;
			joyx1=js1.x-joyxb1;
			joyy1=js1.y-joyyb1;
			int inf=0;
			if(joyx0<joyx0_min) {joyx0_min=joyx0;inf=1;}
			if(joyx0>joyx0_max) {joyx0_max=joyx0;inf=1;}
			if(joyy0<joyy0_min) {joyy0_min=joyy0;inf=1;}
			if(joyy0>joyy0_max) {joyy0_max=joyy0;inf=1;}
			if(joyx1<joyx1_min) {joyx1_min=joyx1;inf=1;}
			if(joyx1>joyx1_max) {joyx1_max=joyx1;inf=1;}
			if(joyy1<joyy1_min) {joyy1_min=joyy1;inf=1;}
			if(joyy1>joyy1_max) {joyy1_max=joyy1;inf=1;}
			
			if(inf)
			{
				printf("joyx0 %d \n",joyx0);
				printf("joyu0 %d \n",joyy0);
				printf("joyx0_min %d \n",joyx0_min);
				printf("joyx0_max %d \n",joyx0_max);
				printf("joyy0_min %d \n",joyy0_min);
				printf("joyy0_max %d \n",joyy0_max);
			}
			if(joyx0<0) joyx0=1000*joyx0/abs(joyx0_min);
			else        joyx0=1000*joyx0/abs(joyx0_max);
			if(joyy0<0) joyy0=1000*joyy0/abs(joyy0_min);
			else        joyy0=1000*joyy0/abs(joyy0_max);
			if(joyx1<0) joyx1=1000*joyx1/abs(joyx1_min);
			else        joyx1=1000*joyx1/abs(joyx1_max);
			if(joyy1<0) joyy1=1000*joyy1/abs(joyy1_min);
			else        joyy1=1000*joyy1/abs(joyy1_max);
			
			
//printf("%d %d %d    %d %d %d   \n",joyx0,js0.x,joyxb0,joyy0,js0.y,joyyb0);
//printf("%d %d %d    %d %d %d   \n",joyx1,js1.x,joyxb1,joyy1,js1.y,joyyb1);
			#if 0
			if(joyx0>0) {if(joyx0<16)  joyx0=0;}//dead zone
			else 		{if(joyx0>-16)  joyx0=0;}
			if(joyy0>0) {if(joyy0<16)  joyy0=0;}
			else 		{if(joyy0>-16)  joyy0=0;}
			if(joyx1>0) {if(joyx1<16)  joyx1=0;}//trust nincs
			else 		{if(joyx1>-16)  joyx1=0;}
			if(joyy1>0) {if(joyy1<16)  joyy1=0;}
			else 		{if(joyy1>-16)  joyy1=0;}
#endif
			
			jbutt0=js0.buttons;//butt
			jbutt1=js1.buttons;
			
			jbutpress0=jbutt0 & (jbutt0_old^0xffffffff);
			jbutpress1=jbutt1 & (jbutt1_old^0xffffffff);
			
			jbutt0_old=jbutt0;
			jbutt1_old=jbutt1;
			
			if(jbutpress0)
			{
#if 0			
				printf("jbutt0 %d \n",jbutt0);
				printf("jbutt1 %d \n",jbutt1);
				printf("joyx0 %d \n",joyx0);
				printf("joyu0 %d \n",joyy0);
				printf("joyx0_min %d \n",joyx0_min);
				printf("joyx0_max %d \n",joyx0_max);
				printf("joyy0_min %d \n",joyy0_min);
				printf("joyy0_max %d \n",joyy0_max);
#endif				
			}
#if 0
			/* print the results */
			printf("X: % 4d  Y: % 4d  B1: %1d  B2: %1d  \r"
				,js.x	/* X axis */
				,js.y	/* Y axis */
				,(js.buttons & 1) ? 1 : 0	/* button 1 */
				,(js.buttons & 2) ? 1 : 0 );	/* button 2 */
#endif
		}
#endif


        while (XPending(display5) > 0)        
        {                                    
            XNextEvent(display5, &event);     
//            XPeekEvent(disp, &event);     
            switch (event.type)              
            {                                
                case Expose:                 
                    break;                       
                case ButtonRelease:
//                printf("%d \n",xbe->state);                printf("%d \n",xbe->button);
					if(xbe->button>=0)
					if(xbe->button<=7)
                	mbut[xbe->button]=0;
                    break;                       
                case ButtonPress:
//                printf("%d \n",xbe->state);                printf("%d \n",xbe->button);                	
                	mx=event.xbutton.x;//4-5 wheel
	                my=event.xbutton.y+border_H;
//	                printf("%d %d \n",xm,ym);
					if(xbe->button>=0)
					if(xbe->button<=7)
                	mbut[xbe->button]=1;
                	if(xbe->button==4) qwheel++;// enginers
                	if(xbe->button==5) qwheel--;
                    break;
                case MotionNotify:
                	dmx=mx-event.xmotion.x;               
                	dmy=my-(event.xmotion.y+border_H);
                	mx=event.xmotion.x;               
                	my=event.xmotion.y+border_H;
                    break;
                case KeyRelease:
                	u=XLookupKeysym(&event.xkey, 0);
                	if(u>=0)
                	if(u<=0xffff)
						tkey[u]=0;
                    break;
                case KeyPress:
                	u=XLookupKeysym(&event.xkey, 0);
                	if(u>=0)
                	if(u<=0xffff)
						tkey[u]=1;
//	                printf("> %d \n",u);

					key=u;					
                    if (u == XK_Escape)
                    {
	                    glxend();
	                    XDestroyWindow(display5,win);                    	
	                    if(fnc) (*fnc)();
	                    //loopd(i,0,100,1) printf("  %d %d \n",i,idtabl[i]);
	                    exit(3);
                    }
                    break;
                case ClientMessage:
    //                if (strcmp(XGetAtomName(disp, event.xclient.message_type),
  //                             "WM_PROTOCOLS") == 0)
                    {
                    }
                    break;
                default:
                    break;
            }
        }
        
/*	loopd(i,0,8,1) 
		if(mbute[i]==2)
		if(mbut[i]==2) mbut[i]=1;//clr press
	*/	
	loopd(i,0,8,1) 
	{
		mbutpress[i]=0;
		
		if(mbute[i]==0)
		if(mbut[i]==1) mbutpress[i]=2;//press

		if(mbute[i]==1)
		if(mbut[i]==0) mbutpress[i]=3;//release
	}
#endif	

}



	
   
float1 linewidth=1.0;
float1 win_scale=1.0;
vec3 win_pos;





void glxline(int x1,int y1,int x2,int y2,int color)
{
	float1 r=(float1)((color>>16)&255)/256.0;
	float1 g=(float1)((color>>8)&255)/256.0;
	float1 b=(float1)((color)&255)/256.0;
	glColor3f(r,g,b);
	
	x1*=2;
	y1*=2;
	x2*=2;
	y2*=2;
	
	vec3 w1(x1,y1,0);
	vec3 w2(x2,y2,0);
	
	w1*=win_scale;
	w2*=win_scale;
	w1+=win_pos;
	w2+=win_pos;
	
	
	vec3 p1,p2,p3,p4,n1,n2;
	n1=normalize(w2-w1);
	n2.x= n1.y;
	n2.y=-n1.x;
	n2.z=0;
	
	float1 q=linewidth*win_scale;
	p1=w1-n1*q-n2*q;
	p2=w1-n1*q+n2*q;
	p3=w2+n1*q+n2*q;
	p4=w2+n1*q-n2*q;
	
//	glBegin(GL_QUADS);
		glMultiTexCoord2f(0,0.0,0.0);	   
		glMultiTexCoord4f(1,w1.x,w1.y,w2.x,w2.y);	   
		glMultiTexCoord2f(2,n2.x,n2.y);	   
		glVertex3f(p1.x,p1.y , 0.0);
		
		glMultiTexCoord2f(0,0.99,0.0);	   
		glMultiTexCoord4f(1,w1.x,w1.y,w2.x,w2.y);	   
		glMultiTexCoord2f(2,n2.x,n2.y);	   
		glVertex3f(p2.x,p2.y , 0.0);
		
		glMultiTexCoord2f(0,0.99,0.99);	   
		glMultiTexCoord4f(1,w1.x,w1.y,w2.x,w2.y);	   
		glMultiTexCoord2f(2,n2.x,n2.y);	   
		glVertex3f(p3.x,p3.y , 0.0);
		
		glMultiTexCoord2f(0,0.0,0.99);	   
		glMultiTexCoord4f(1,w1.x,w1.y,w2.x,w2.y);	   
		glMultiTexCoord2f(2,n2.x,n2.y);	   
		glVertex3f(p4.x,p4.y , 0.0);
//	glEnd();
}
void glxlineb(float1 x1,float1 y1,float1 x2,float1 y2,int color)
{
	float1 r=(float1)((color>>16)&255)/256.0;
	float1 g=(float1)((color>>8)&255)/256.0;
	float1 b=(float1)((color)&255)/256.0;
	glColor3f(r,g,b);

	vec3 w1(x1,y1,0.0f);
	vec3 w2(x2,y2,0.0f);
	
	w1*=win_scale;
	w2*=win_scale;
	w1+=win_pos;
	w2+=win_pos;

	vec3 p1,p2,p3,p4,n1,n2;
	n1=normalize(w2-w1);
	n2.x= n1.y;
	n2.y=-n1.x;
	n2.z=0;
	
	float1 q=linewidth*win_scale;
	p1=w1-n1*q-n2*q;
	p2=w1-n1*q+n2*q;
	p3=w2+n1*q+n2*q;
	p4=w2+n1*q-n2*q;
	
//	glBegin(GL_QUADS);
		glMultiTexCoord2f(0,0.0,0.0);	   
		glMultiTexCoord4f(1,w1.x,w1.y,w2.x,w2.y);	   
		glMultiTexCoord2f(2,n2.x,n2.y);	   
		glVertex3f(p1.x,p1.y , 0.0);
		
		glMultiTexCoord2f(0,0.99,0.0);	   
		glMultiTexCoord4f(1,w1.x,w1.y,w2.x,w2.y);	   
		glMultiTexCoord2f(2,n2.x,n2.y);	   
		glVertex3f(p2.x,p2.y , 0.0);
		
		glMultiTexCoord2f(0,0.99,0.99);	   
		glMultiTexCoord4f(1,w1.x,w1.y,w2.x,w2.y);	   
		glMultiTexCoord2f(2,n2.x,n2.y);	   
		glVertex3f(p3.x,p3.y , 0.0);
		
		glMultiTexCoord2f(0,0.0,0.99);	   
		glMultiTexCoord4f(1,w1.x,w1.y,w2.x,w2.y);	   
		glMultiTexCoord2f(2,n2.x,n2.y);	   
		glVertex3f(p4.x,p4.y , 0.0);
//	glEnd();
}
#define TSINSIZE 0x004000

float1 tsin[TSINSIZE*2];
float1 tcos[TSINSIZE*2];

void initsintab()
{
	loopd(x,0,TSINSIZE*2-1,1)		
	{
		tsin[x]=sin((float1)x*M_PI*2.0/TSINSIZE);
		tcos[x]=cos((float1)x*M_PI*2.0/TSINSIZE);
	}
}
float qsin(float n)//quick sin()
{
	float ii=TSINSIZE*n/(M_PI*2.0);
	int indx=((int)ii)&(TSINSIZE-1);
	
	return tsin[indx];
}
float qcos(float n)
{
	float ii=TSINSIZE*n/(M_PI*2.0);
	int indx=((int)ii)&(TSINSIZE-1);
	
	return tcos[indx];
}





void pixel(int x,int y,int col)
{
#ifdef __USE_GLX
#else
    XSetForeground(display5,gc,col);
    XDrawPoint(display5, win, gc, x,y);
#endif
}
void line(int x1,int y1,int x2,int y2,int color)
{
#ifdef __USE_GLX
	glxline(x1,y1,x2,y2,color);
#else
    XSetForeground(display5,gc,color);
    XDrawLine(display5, win, gc, x1,y1, x2,y2);
#endif
}

void lineb(float1 x1,float1 y1,float1 x2,float1 y2,int color)
//void lineb(int x1,int y1,int x2,int y2,int color)
{
#ifdef __USE_GLX
	glxlineb(x1,y1,x2,y2,color);
#else
    XSetForeground(display5,gc,color);
    XDrawLine(display5, win, gc, x1,y1, x2,y2);
#endif
}



void init_system5()
{
QDUMPFNC();	


#ifdef __USE_GLX
	glxinit5();


#else
	display5 = XOpenDisplay(0);
	win = XCreateSimpleWindow(display5, DefaultRootWindow(display5), 0,0, xx, yy, 0,0,0);

	XSelectInput(display5, win, StructureNotifyMask| 
			ButtonPressMask| ButtonReleaseMask| 
			KeyPressMask |KeyReleaseMask |
			PointerMotionMask);
	XMapWindow(display5, win);
	gc = XCreateGC(display5, win, 0, 0);

	for(;;) { XEvent e; XNextEvent(display5, &e); if (e.type == MapNotify) break; }
	qsleep(1.0);
#endif	

#ifdef __USEJOY
		/* open the joystick */
	if( ( joy_fd0 = open( JOY_DEV0, O_RDONLY ) ) < 0 )
	{
		printf( "Couldn't open joystick device %s\n", JOY_DEV0 );
	}
	if( ( joy_fd1 = open( JOY_DEV1, O_RDONLY ) ) < 0 )
	{
		printf( "Couldn't open joystick device %s\n", JOY_DEV1 );
	}
#endif	
printf("init_system 2 \n");
	initsintab();


	qclear(4);	
	qflush();
printf("init_system OK \n");

}

void wait()
{
//	XFlush(display5);
	getchar();
}
void qclear(int op)
{
#ifdef __USE_GLX
	 glxclear(op);
#else
	XClearWindow(display5,win);
#endif
}
void qflush()
{
#ifdef __USE_GLX
	glxflush();
#else
	XFlush(display5);
#endif
}
//--------------------------------------------------------------------------------

int lerpcol(int col1,int col2,float1 t)
{
	int r1=(col1>>16)&255;
	int g1=(col1>>8)&255;
	int b1=(col1)&255;

	int r2=(col2>>16)&255;
	int g2=(col2>>8)&255;
	int b2=(col2)&255;
	
	int t1=(int)(t*1000.0);
	if(t1<0) t1=-t1;
	if(t1>1000) t1=1000;
	
	r1=r1+(r2-r1)*t1/1000;
	g1=g1+(g2-g1)*t1/1000;
	b1=b1+(b2-b1)*t1/1000;
	
	return (r1<<16)+(g1<<8)+b1;
}


void create_texture(qstring name,int _x,int _y,int _c,uchar *buf)
{
QDUMPFNC();	
	s_texture tmp;
	
	tmp.xh=_x;
	tmp.yh=_y;
	tmp.ch=_c;
	tmp.tgabuf=new uchar[tmp.xh*tmp.yh*_c];
	tmp.scan=tmp.xh*tmp.ch;
	
	memcpy(tmp.tgabuf,buf,_x*_y*_c);
	tmp.id=glxtexture(tmp.tgabuf,tmp.xh,tmp.yh,_c);
	
	int64_t txi=qhash64(name);
	textures[txi]=tmp;
}

void loadbmp(qstring name,qstring fname,int opac)
{
QDUMPFNC();	
	s_texture tmp;
	tmp.xh=0;
	
	FILE *ff=fopen(fname.c_str(),"rb");
	if(ff)
	{
		fread(head,1,12,ff);
		int headerlength=head[5];//kua 2 gimpwriter
		fread(&head[6],1,headerlength-12,ff);
		if(headerlength>128) {printf("headerlength %s\n",fname.c_str());exit(3);}
		tmp.xh=head[9];
		tmp.yh=head[11];
		tmp.ch=head[14]/8;//8-24
//printf("%s ",name);	loopd(i,9,16,1) printf("%d ",head[i]);		
printf("%s %d %d %d , %d ",fname.c_str(),tmp.xh,tmp.yh,tmp.ch,headerlength);		
		tmp.tgabuf=new uchar[tmp.xh*tmp.yh*4];
		uchar *buf3=new uchar[tmp.xh*tmp.yh*4];
		fread(buf3,1,tmp.xh*tmp.yh*tmp.ch,ff);
		fclose(ff);

		tmp.scan=tmp.xh*tmp.ch;
		tmp.scan+=2;		
		tmp.scan/=4;		
		tmp.scan*=4;//div by 4

printf("scan %d %d ",tmp.scan,tmp.xh*tmp.ch);
printf("\n");

//	if(tmp.ch==3)
	loopd(y,0,tmp.yh,1)//rtg
	loopd(x,0,tmp.xh,1)
	{
		int m=y*tmp.scan+x*tmp.ch;
		int m4=y*tmp.xh*4+x*4;//will be 4
		
		uchar r1=buf3[m+2];
		uchar g1=buf3[m+1];
		uchar b1=buf3[m+0];
		uchar a1=255;
	
		if(opac)
		if(r1>=170)//white opacity 170
		if(g1>=170)
		if(b1>=170) a1=0;

		tmp.tgabuf[m4+0]=r1;
		tmp.tgabuf[m4+1]=g1;
		tmp.tgabuf[m4+2]=b1;
		tmp.tgabuf[m4+3]=a1;
	}
	tmp.id=glxtexture(tmp.tgabuf,tmp.xh,tmp.yh,4);//3
	
	int64_t txi=qhash64(name);
	textures[txi]=tmp;

	}
	else printf("FILE ERROR %s\n",fname.c_str());
}


void loadtga(qstring name,qstring fname,int colxchg)//colxchg=0
{
QDUMPFNC();	
	s_texture tmp;
	tmp.xh=0;
	FILE *ff=fopen(fname.c_str(),"rb");
	if(ff)
	{
		fread(head,1,18,ff);
		tmp.xh=head[6];
		tmp.yh=head[7];
		tmp.ch=(head[8]&0xff)/8;//8-24
//printf("%s ",name);	loopd(i,6,16,1) printf("%d ",head[i]);		
printf("%s %d %d %d ",fname.c_str(),tmp.xh,tmp.yh,tmp.ch);		printf("\n");
		tmp.tgabuf=new uchar[tmp.xh*tmp.yh*4];
		fread(tmp.tgabuf,1,tmp.xh*tmp.yh*tmp.ch,ff);
		fclose(ff);


		tmp.scan=tmp.xh*tmp.ch;
		tmp.scan+=2;
		tmp.scan/=4;
		tmp.scan*=4;//div by 4


	if(colxchg)
	if(tmp.ch==3)
	loopd(y,0,tmp.yh,1)
	loopd(x,0,tmp.xh,1)
	{
		int m=y*tmp.scan+x*tmp.ch;
	
		uchar q=tmp.tgabuf[m+0];
		tmp.tgabuf[m+0]=tmp.tgabuf[m+2];
		tmp.tgabuf[m+2]=q;//r<->g
	}


	
/*	uchar *buf3=new uchar[scan[tx]*yh[tx]];//a
	loopd(y,0,yh[tx],1)	memcpy(&buf3[y*scan[tx]],&tgabuf[tx][y*xh[tx]*ch[tx]],xh[tx]*ch[tx]);
	yh[tx]=scan[tx]/ch[tx];
	textures[tx]=glxtexture(buf3,xh[tx],yh[tx],ch[tx]);
	delete [] buf3;*/
	tmp.id=glxtexture(tmp.tgabuf,tmp.xh,tmp.yh,tmp.ch);
	int64_t txi=qhash64(name);
	textures[txi]=tmp;
	}
	else printf("FILE ERROR %s\n",fname.c_str());
}



void loadbmpb(qstring name,qstring fname,int ecut,int colxchg)//ecut=0 colxchg=1
{
QDUMPFNC();	
	s_texture tmp;
	tmp.xh=0;
	FILE *ff=fopen(fname.c_str(),"rb");
	if(ff)
	{
		fread(head,1,12,ff);
		int headerlength=head[5];//kua 2 gimpwriter
		fread(&head[6],1,headerlength-12,ff);
		if(headerlength>128) {printf("headerlength %s\n",fname.c_str());exit(3);}
		tmp.xh=head[9];
		tmp.yh=head[11];
		tmp.ch=head[14]/8;//8-24
//printf("%s ",name);	loopd(i,9,16,1) printf("%d ",head[i]);		
printf("%s %d %d %d , %d %d",fname.c_str(),tmp.xh,tmp.yh,tmp.ch,headerlength,ecut);		printf("\n");
		tmp.tgabuf=new uchar[tmp.xh*tmp.yh*4];
		fread(tmp.tgabuf,1,tmp.xh*tmp.yh*tmp.ch,ff);
		fclose(ff);

		tmp.scan=tmp.xh*tmp.ch;
		tmp.scan+=2;
		tmp.scan/=4;
		tmp.scan*=4;//div by 4

#if 0
	if(ch[tx]==3)
	loopd(y,0,yh[tx],1)
	loopd(x,0,xh[tx],1)
	{
		int m=(y*xh[tx]+x)*ch[tx];
	
		if(colxchg)
		{
			uchar q=buf[tx][m+1];
			buf[tx][m+1]=buf[tx][m+2];
			buf[tx][m+2]=q;//r<->g
		}
		else
		{
			uchar q=buf[tx][m+0];//wtf?? lol gimp   header bug
			buf[tx][m+0]=buf[tx][m+2];
			buf[tx][m+2]=q;//r<->g
		}
	}
#endif
//ecut=0;colxchg=0;//cheat

	if(colxchg)
	if(tmp.ch==3)
	loopd(y,0,tmp.yh,1)
	loopd(x,0,tmp.xh,1)
	{
		int m=y*tmp.scan+x*tmp.ch;
	
		uchar q=tmp.tgabuf[m+0];
		tmp.tgabuf[m+0]=tmp.tgabuf[m+2];
		tmp.tgabuf[m+2]=q;//r<->g
	}

	if(ecut)
	{
//1280/1024./2=640/512    center= 640/542    542-70=472  944x944
		int xh2=xmap,yh2=xmap;//944
		uchar *buf2=new uchar[xh2*yh2*4];
printf("      scaled %d %d \n",xh2,yh2);

		int dx=tmp.xh/2-xh2/2;//472
		int dy=tmp.yh/2-yh2/2 -30;//  30=542-512  fix
		
		loopd(y,0,yh2,1)
		loopd(x,0,xh2,1)
		{
			int m1=(y+dy)*tmp.scan+(x+dx)*tmp.ch;
			int m2=(y*xh2+x)*tmp.ch;
	
			loopd(i,0,tmp.ch,1)		
				buf2[m2+i]=tmp.tgabuf[m1+i];
		}
		

		tmp.xh=xh2;
		tmp.yh=yh2;
		delete [] tmp.tgabuf;//xchg
		tmp.tgabuf=buf2;
		buf2=0;
	}
	else
	{
#if 0	
		int xh2=xh[tx]&0xfffffff0,yh2=yh[tx]&0xfffffff0;//cut div16
		uchar *buf2=new uchar[xh2*yh2*4];

		loopd(y,0,yh2,1)
		loopd(x,0,xh2,1)
		{
			int m1=(y*xh[tx] +x)*3;
			int m2=(y*xh2+x)*3;
			
			buf2[m2  ]=buf[tx][m1];
			buf2[m2+1]=buf[tx][m1+1];
			buf2[m2+2]=buf[tx][m1+2];
		}
		

		xh[tx]=xh2;
		yh[tx]=yh2;
		delete [] buf[tx];//xchg
		buf[tx]=buf2;
		buf2=0;
#endif
	}
	
	{
	uchar *buf3=new uchar[tmp.xh*tmp.yh*4];//anyad
	memcpy(buf3,tmp.tgabuf,tmp.xh*tmp.yh*4);
	tmp.id=glxtextureb(buf3,tmp.xh,tmp.yh,3);
	int64_t txi=qhash64(name);
	textures[txi]=tmp;
	delete [] buf3;
	}
	
	}
	else printf("FILE ERROR %s\n",fname.c_str());
}

#if 0
void loadbmpgz(int tx,const char *name,int ecut=0,int colxchg=1)
{
	xh[tx]=0;
	gzFile ff=gzopen(name,"rb");
	if(ff!=0)
	{
		gzread(ff,head,12);
		int headerlength=head[5];//kua 2 gimpwriter
		gzread(ff,&head[6],headerlength-12);
		if(headerlength>128) {printf("headerlength %s\n",name);exit(3);}
		xh[tx]=head[9];
		yh[tx]=head[11];
		ch[tx]=head[14]/8;//8-24
printf("%s ",name);	loopd(i,0,16,1) printf("%d ",head[i]);		
printf("%s %d %d %d , %d %d",name,xh[tx],yh[tx],ch[tx],headerlength,ecut);		printf("\n");
		tgabuf[tx]=new uchar[xh[tx]*yh[tx]*4];
		gzread(ff,tgabuf[tx],xh[tx]*yh[tx]*ch[tx]);
		gzclose(ff);

		scan[tx]=xh[tx]*ch[tx];
		scan[tx]+=2;
		scan[tx]/=4;
		scan[tx]*=4;//div by 4


#if 0
	if(ch[tx]==3)
	loopd(y,0,yh[tx],1)
	loopd(x,0,xh[tx],1)
	{
		int m=(y*xh[tx]+x)*ch[tx];
	
		if(colxchg)
		{
			uchar q=buf[tx][m+1];
			buf[tx][m+1]=buf[tx][m+2];
			buf[tx][m+2]=q;//r<->g
		}
		else
		{
			uchar q=buf[tx][m+0];//wtf?? lol gimp   header bug
			buf[tx][m+0]=buf[tx][m+2];
			buf[tx][m+2]=q;//r<->g
		}
	}
#endif

	if(colxchg)
	if(ch[tx]==3)
	loopd(y,0,yh[tx],1)
	loopd(x,0,xh[tx],1)
	{
		int m=y*scan[tx]+x*ch[tx];
	
		uchar q=tgabuf[tx][m+0];
		tgabuf[tx][m+0]=tgabuf[tx][m+2];
		tgabuf[tx][m+2]=q;//r<->g
	}

	if(ecut)
	{
//1280/1024./2=640/512    center= 640/542    542-70=472  944x944
		int xh2=xmap,yh2=xmap;//944
		uchar *buf2=new uchar[xh2*yh2*4];
printf("      scaled %d %d \n",xh2,yh2);

		int dx=xh[tx]/2-xh2/2;//472
		int dy=yh[tx]/2-yh2/2 -30;//  30=542-512  fix
		
		loopd(y,0,yh2,1)
		loopd(x,0,xh2,1)
		{
			int m1=(y+dy)*scan[tx]+(x+dx)*ch[tx];
			int m2=(y*xh2+x)*ch[tx];
	
			loopd(i,0,ch[tx],1)		
				buf2[m2+i]=tgabuf[tx][m1+i];
		}
		

		xh[tx]=xh2;
		yh[tx]=yh2;
		delete [] tgabuf[tx];//xchg
		tgabuf[tx]=buf2;
		buf2=0;
	}
	else
	{
#if 0	
		int xh2=xh[tx]&0xfffffff0,yh2=yh[tx]&0xfffffff0;//cut div16
		uchar *buf2=new uchar[xh2*yh2*4];

		loopd(y,0,yh2,1)
		loopd(x,0,xh2,1)
		{
			int m1=(y*xh[tx] +x)*3;
			int m2=(y*xh2+x)*3;
			
			buf2[m2  ]=buf[tx][m1];
			buf2[m2+1]=buf[tx][m1+1];
			buf2[m2+2]=buf[tx][m1+2];
		}
		

		xh[tx]=xh2;
		yh[tx]=yh2;
		delete [] buf[tx];//xchg
		buf[tx]=buf2;
		buf2=0;
#endif
	}
	
	{
	uchar *buf3=new uchar[xh[tx]*yh[tx]*4];//anyad
	memcpy(buf3,tgabuf[tx],xh[tx]*yh[tx]*4);
	textures[tx]=glxtextureb(buf3,xh[tx],yh[tx],3);
	delete [] buf3;
	}
	
	}
	else printf("FILE ERROR %s\n",name);
}
#endif

void loadtgab(qstring name,qstring fname,int colxchg)//colxchg=0
{
QDUMPFNC();	
	s_texture tmp;
	tmp.xh=0;
	FILE *ff=fopen(fname.c_str(),"rb");
	if(ff)
	{
		fread(head,1,18,ff);
		tmp.xh=head[6];
		tmp.yh=head[7];
		tmp.ch=(head[8]&0xff)/8;//8-24
//printf("%s ",name);	loopd(i,6,16,1) printf("%d ",head[i]);		
printf("%s %d %d %d ",fname.c_str(),tmp.xh,tmp.yh,tmp.ch);		printf("\n");
		tmp.tgabuf=new uchar[tmp.xh*tmp.yh*4];
		fread(tmp.tgabuf,1,tmp.xh*tmp.yh*tmp.ch,ff);
		fclose(ff);


		tmp.scan=tmp.xh*tmp.ch;
		tmp.scan+=2;
		tmp.scan/=4;
		tmp.scan*=4;//div by 4


	if(colxchg)
	if(tmp.ch==3)
	loopd(y,0,tmp.yh,1)
	loopd(x,0,tmp.xh,1)
	{
		int m=y*tmp.scan+x*tmp.ch;
	
		uchar q=tmp.tgabuf[m+0];
		tmp.tgabuf[m+0]=tmp.tgabuf[m+2];
		tmp.tgabuf[m+2]=q;//r<->g
	}


	
/*	uchar *buf3=new uchar[scan[tx]*yh[tx]];//anyad
	loopd(y,0,yh[tx],1)	memcpy(&buf3[y*scan[tx]],&tgabuf[tx][y*xh[tx]*ch[tx]],xh[tx]*ch[tx]);
	yh[tx]=scan[tx]/ch[tx];
	textures[tx]=glxtexture(buf3,xh[tx],yh[tx],ch[tx]);
	delete [] buf3;*/
	tmp.id=glxtextureb(tmp.tgabuf,tmp.xh,tmp.yh,tmp.ch);
	int64_t txi=qhash64(name);
	textures[txi]=tmp;
	}
	else printf("FILE ERROR %s\n",fname.c_str());
}

void savetga(std::string fname,uchar *mem,int xh,int yh,int ch)
{
	FILE *ff=fopen(fname.c_str(),"wb");
	if(ff)
	{
		memset(head,0,64);
		head[1]=2;
		head[6]=xh;
		head[7]=yh;
		head[8]=ch*8;
		fwrite(head,1,18,ff);
		
		for(int y=0;y<yh;y++)
		for(int x=0;x<xh;x++)
		{
			fwrite(&mem[(y*xh+x)*ch],1,ch,ff);
		}
		fclose(ff);
	}
}

#if 0
vector<vect> verts;
vector<vect> txc;
vector<int> faces;

vector<int> txlist_facestart;
vector<string> txlist;


void loadACfile(char *name,float1 scl)
{
	char word[256];
	char word2[256];
	char word3[256];
	int base=0;
	float1 scale=1.0;
	vec3 pos;
	
	FILE *ff=fopen(name,"r");
	if(ff>0)
	{
		while(feof(ff)==0)
		{
			fscanf(ff,"%s",word);				//printf("%s \n",word);
			

			if(strcmp(word,"texture")==0)
			{
				fscanf(ff,"%s",word2);//	texture "Liveries/topbottom1.checkmates.png"		
//printf("%s \n",word2);
				
				int l=strlen(word2)-2;// from "
				if(l>0)	
				{
				while(word2[l]!='.' && word2[l]!='"') l--;
				word2[l+1]='b';
				word2[l+2]='m';
				word2[l+3]='p';
				word2[l+4]=0; // "
				if(l>0)	
				{
					while(word2[l]!='/' && word2[l]!='"') l--;
					l++;
				}
				if(l<0) l=0;
				
				sprintf(word3,"plane/%s",&word2[l]);
				string ss(word3);

				int sel=-1;
				loopd(i,0,txlist.size(),1)
					if(txlist[i]==ss) {sel=i;break;}

				if(sel==-1)
				{
					sel=txlist.size();
					loadbmp(50+sel,word3);//"plane/common2.bmp");
printf("%s\n",word3);

					txlist.push_back(ss);
					txlist_facestart.push_back(faces.size());
				}
				}

			}
			if(strcmp(word,"loc")==0)//pivot
			{
				fscanf(ff,"%f %f %f",&pos.x,&pos.y,&pos.z);
			}
			if(strcmp(word,"crease")==0)//??
			{
				fscanf(ff,"%f",&scale);
			}
			
			if(strcmp(word,"numvert")==0)
			{
				int nv=0;
				
				base=verts.size();
				fscanf(ff,"%d",&nv);	//				printf("%d \n",nv);
				
				loopd(i,0,nv,1)
				{
					vec3 w1;
					fscanf(ff,"%f %f %f",&w1.x,&w1.y,&w1.z);
//					printf("%f %f %f \n",w1.x,w1.y,w1.z);	
					verts.push_back((w1)*scl);//+pos
				}
			}
			if(strcmp(word,"numsurf")==0)
			{
				int nf=0;
				fscanf(ff,"%d",&nf);

				loopd(j,0,nf,1)
				{
				int a=0,b=0,c=0;				
				fscanf(ff,"%s",word2);	fscanf(ff,"%x",&a);
				fscanf(ff,"%s",word2);	fscanf(ff,"%d",&b);
				fscanf(ff,"%s",word2);	fscanf(ff,"%d",&c);
//					if((a&0x20)) 2side
//					if((a&0x10)) shaded
				
//				printf("%d %d %d\n",a,b,c);
				int ids[16];
				vec3 txcr[16];

				loopd(i,0,c,1)
				{
					int id=0;
					
					fscanf(ff,"%d %f %f",&id,&txcr[i].x,&txcr[i].y);
					txcr[i].z=0;
	
					ids[i]=id+base;
				
//					printf("%d %f %f \n",id,txcr[i].x,txcr[i].y);
				}
				if(c==3)
				{
					faces.push_back(ids[0]);
					faces.push_back(ids[1]);
					faces.push_back(ids[2]);
					
					txc.push_back(txcr[0]);
					txc.push_back(txcr[1]);
					txc.push_back(txcr[2]);
				}
				else
				{
				if(c==4)
				{
					faces.push_back(ids[0]);
					faces.push_back(ids[1]);
					faces.push_back(ids[2]);

					faces.push_back(ids[0]);
					faces.push_back(ids[2]);
					faces.push_back(ids[3]);


					txc.push_back(txcr[0]);
					txc.push_back(txcr[1]);
					txc.push_back(txcr[2]);
					
					txc.push_back(txcr[0]);
					txc.push_back(txcr[2]);
					txc.push_back(txcr[3]);
				}
				else
					printf("c==%d\n",c);
				}
				}
			}
		}
		fclose(ff);
		
		txlist_facestart.push_back(faces.size());//close
		printf("faces:%d  verts:%d \n",faces.size(),verts.size());
	}
}



void drawplane(vec3 pos)
{
return;
	vec3 resol(xx,yy,0);

	glDisable (GL_BLEND);
	//glDisable (GL_DEPTH);//	
	glEnable(GL_DEPTH);
	glEnable (GL_DEPTH_TEST);

	setshader(4);
	shader_setparm("camx",&cam2.x,3);//uniform
	shader_setparm("camy",&cam2.y,3);
	shader_setparm("camz",&cam2.z,3);
	shader_setparm("eye",&eye,3);
	shader_setparm("resol",&resol,2);
	


	loopd(j,0,txlist_facestart.size()-1,1)
	{
		glActiveTexture(GL_TEXTURE0);	glBindTexture(GL_TEXTURE_2D, textures[50+j]);//sorba
		shader_setparm("tx2",0);

		glBegin(GL_TRIANGLES);
		//loopd(i,0,faces.size(),3)
		loopd(i,txlist_facestart[j],txlist_facestart[j+1],3)
		{
			vec3 v1=verts[faces[i]];
			vec3 v2=verts[faces[i+1]];
			vec3 v3=verts[faces[i+2]];
			
			glMultiTexCoord2f(0,txc[i  ].x,txc[i  ].y);	  glVertex3f(v1.x,v1.y,v1.z);
			glMultiTexCoord2f(0,txc[i+1].x,txc[i+1].y);	  glVertex3f(v2.x,v2.y,v2.z);
			glMultiTexCoord2f(0,txc[i+2].x,txc[i+2].y);	  glVertex3f(v3.x,v3.y,v3.z);
		}
		glEnd();
	}
}
#endif

//--------------------------------------------------------------------------------
//nem lehet 20 nel hoszabb nev!!!!!!!!!!!!
void qsettx(int tx,qstring nm,IDTYPE &id,IDTYPE &id2)
{
QDUMPFNC();	
	qstring tmp=qstring("tx")+int2string(tx);
	if(id==-1)
	{
		id=textures[qhash64(nm)].id;
//	if(textures.find(nm)==textures.end()) printf("ERROR %s \n",nm.c_str());
		if(id==0) printf("ERROR %s \n",nm.c_str());
	}
	

	switch(tx) 
	{
		default:
		case 0:ERR(glActiveTexture(GL_TEXTURE0));	break;
		case 1:ERR(glActiveTexture(GL_TEXTURE1));	break;
		case 2:ERR(glActiveTexture(GL_TEXTURE2));	break;
		case 3:ERR(glActiveTexture(GL_TEXTURE3));	break;
		case 4:ERR(glActiveTexture(GL_TEXTURE4));	break;
		case 5:ERR(glActiveTexture(GL_TEXTURE5));	break;
		case 6:ERR(glActiveTexture(GL_TEXTURE6));	break;
		case 7:ERR(glActiveTexture(GL_TEXTURE7));	break;
		case 8:ERR(glActiveTexture(GL_TEXTURE8));	break;
		case 9:ERR(glActiveTexture(GL_TEXTURE9));	break;
		case 10:ERR(glActiveTexture(GL_TEXTURE10));	break;
		case 11:ERR(glActiveTexture(GL_TEXTURE11));	break;
		case 12:ERR(glActiveTexture(GL_TEXTURE12));	break;
		case 13:ERR(glActiveTexture(GL_TEXTURE13));	break;
		case 14:ERR(glActiveTexture(GL_TEXTURE14));	break;
		case 15:ERR(glActiveTexture(GL_TEXTURE15));	break;
	}
	ERR(glBindTexture(GL_TEXTURE_2D, id));//textures[nm].
	qshader_setparm2(tmp.c_str(),tx,id2);
}
void qsettx3d(int tx,qstring nm,IDTYPE &id,IDTYPE &id2)
{
QDUMPFNC();	
	qstring tmp=qstring("tx")+int2string(tx);
	if(id==-1)
	{
		id=textures[qhash64(nm)].id;
//	if(textures.find(nm)==textures.end()) printf("ERROR %s \n",nm.c_str());
		if(id==0) printf("ERROR %s \n",nm.c_str());
	}
	

	switch(tx) 
	{
		default:
		case 0:ERR(glActiveTexture(GL_TEXTURE0));	break;
		case 1:ERR(glActiveTexture(GL_TEXTURE1));	break;
		case 2:ERR(glActiveTexture(GL_TEXTURE2));	break;
		case 3:ERR(glActiveTexture(GL_TEXTURE3));	break;
		case 4:ERR(glActiveTexture(GL_TEXTURE4));	break;
		case 5:ERR(glActiveTexture(GL_TEXTURE5));	break;
		case 6:ERR(glActiveTexture(GL_TEXTURE6));	break;
		case 7:ERR(glActiveTexture(GL_TEXTURE7));	break;
		case 8:ERR(glActiveTexture(GL_TEXTURE8));	break;
		case 9:ERR(glActiveTexture(GL_TEXTURE9));	break;
		case 10:ERR(glActiveTexture(GL_TEXTURE10));	break;
		case 11:ERR(glActiveTexture(GL_TEXTURE11));	break;
		case 12:ERR(glActiveTexture(GL_TEXTURE12));	break;
		case 13:ERR(glActiveTexture(GL_TEXTURE13));	break;
		case 14:ERR(glActiveTexture(GL_TEXTURE14));	break;
		case 15:ERR(glActiveTexture(GL_TEXTURE15));	break;
	}
	ERR(glBindTexture(GL_TEXTURE_3D, id));//textures[nm].
	qshader_setparm2(tmp.c_str(),tx,id2);
}

//nem lehet 20 nel hoszabb nev!!!!!!!!!!!!
// NOS=no shader param
void qsettxNOS(int tx,qstring nm,IDTYPE &id,IDTYPE &id2)
{
QDUMPFNC();	
	qstring tmp=qstring("tx")+int2string(tx);
	if(id==-1)
	{
		id=textures[qhash64(nm)].id;
//	if(textures.find(nm)==textures.end()) printf("ERROR %s \n",nm.c_str());
		if(id==0) printf("ERROR %s \n",nm.c_str());
	}
	

	switch(tx) 
	{
		default:
		case 0:ERR(glActiveTexture(GL_TEXTURE0));	break;
		case 1:ERR(glActiveTexture(GL_TEXTURE1));	break;
		case 2:ERR(glActiveTexture(GL_TEXTURE2));	break;
		case 3:ERR(glActiveTexture(GL_TEXTURE3));	break;
		case 4:ERR(glActiveTexture(GL_TEXTURE4));	break;
		case 5:ERR(glActiveTexture(GL_TEXTURE5));	break;
		case 6:ERR(glActiveTexture(GL_TEXTURE6));	break;
		case 7:ERR(glActiveTexture(GL_TEXTURE7));	break;
		case 8:ERR(glActiveTexture(GL_TEXTURE8));	break;
		case 9:ERR(glActiveTexture(GL_TEXTURE9));	break;
		case 10:ERR(glActiveTexture(GL_TEXTURE10));	break;
		case 11:ERR(glActiveTexture(GL_TEXTURE11));	break;
		case 12:ERR(glActiveTexture(GL_TEXTURE12));	break;
		case 13:ERR(glActiveTexture(GL_TEXTURE13));	break;
		case 14:ERR(glActiveTexture(GL_TEXTURE14));	break;
		case 15:ERR(glActiveTexture(GL_TEXTURE15));	break;
	}
	ERR(glBindTexture(GL_TEXTURE_2D, id));//textures[nm].
//	qshader_setparm2(tmp.c_str(),tx,id2);
}
void qsettxbyid(int tx,int id)
{
QDUMPFNC();	
	qstring tmp=qstring("tx")+int2string(tx);

	switch(tx) 
	{
		default:
		case 0:ERR(glActiveTexture(GL_TEXTURE0));	break;
		case 1:ERR(glActiveTexture(GL_TEXTURE1));	break;
		case 2:ERR(glActiveTexture(GL_TEXTURE2));	break;
		case 3:ERR(glActiveTexture(GL_TEXTURE3));	break;
		case 4:ERR(glActiveTexture(GL_TEXTURE4));	break;
		case 5:ERR(glActiveTexture(GL_TEXTURE5));	break;
		case 6:ERR(glActiveTexture(GL_TEXTURE6));	break;
		case 7:ERR(glActiveTexture(GL_TEXTURE7));	break;
		case 8:ERR(glActiveTexture(GL_TEXTURE8));	break;
		case 9:ERR(glActiveTexture(GL_TEXTURE9));	break;
		case 10:ERR(glActiveTexture(GL_TEXTURE10));	break;
		case 11:ERR(glActiveTexture(GL_TEXTURE11));	break;
		case 12:ERR(glActiveTexture(GL_TEXTURE12));	break;
		case 13:ERR(glActiveTexture(GL_TEXTURE13));	break;
		case 14:ERR(glActiveTexture(GL_TEXTURE14));	break;
		case 15:ERR(glActiveTexture(GL_TEXTURE15));	break;
	}
	IDTYPE id2=0;
	ERR(glBindTexture(GL_TEXTURE_2D, id));//textures[nm].
	qshader_setparm2(tmp.c_str(),tx,id2);// id2 nem lenyeg, shader ujratolti
}
s_texture *qgettexture(const char *name,IDTYPE &id)
{
	if(id==-1)
	{
		id=(IDTYPE)&textures[qhash64(name)];
	}
	return (s_texture *)id;
}

void defmesh(qstring meshname,qstring type)
{
QDUMPFNC();	
	gscene[qhash64(meshname)]=glGenLists(1);
	ERR(glNewList(gscene[qhash64(meshname)],GL_COMPILE));
	if(type=="quads") glBegin2(GL_QUADS);
	if(type=="points") glBegin2(GL_POINTS);
	if(type=="triangles") glBegin2(GL_TRIANGLES);
	if(type=="lines") glBegin2(GL_LINES);
}
void endmesh()
{
QDUMPFNC();	
    glEnd2();//ERR
	ERR(glEndList());
}
void qdrawmesh(qstring name,IDTYPE &id)
{
QDUMPFNC();	
//	if(id==-1)
	{
		id=gscene[qhash64(name)];
		if(id==0) printf("ERROR %s \n",name.c_str());
	}
	ERR3(glCallList(id));//gscene[name]);//1   ERR3?
}
void delmesh(qstring name)
{
QDUMPFNC();	
	int id=gscene[qhash64(name)];
	if(id==0) printf("ERROR %s \n",name.c_str());
	
	ERR(glCallList(id));//gscene[name]);//1
}


int gcc=0;


void quadtx(
	vec3 v1,vec3 v2,vec3 v3,vec3 v4,
	vec3 n1,vec3 n2,vec3 n3,vec3 n4,
	vec3 col,
	vec3 tx1,vec3 tx2,vec3 tx3,vec3 tx4)
{
//render
	//glBegin(GL_TRIANGLES);	//
//	glBegin(GL_QUADS);
#if 0
		glMultiTexCoord2fv(0,&tx1.x);glMultiTexCoord3fv(1,&n1.x);	  glMultiTexCoord3fv(2,&col.x); glVertex3fv(&v1.x);
		glMultiTexCoord2fv(0,&tx2.x);glMultiTexCoord3fv(1,&n2.x);	  glMultiTexCoord3fv(2,&col.x); glVertex3fv(&v2.x);
		glMultiTexCoord2fv(0,&tx3.x);glMultiTexCoord3fv(1,&n3.x);	  glMultiTexCoord3fv(2,&col.x); glVertex3fv(&v3.x);
		glMultiTexCoord2fv(0,&tx4.x);glMultiTexCoord3fv(1,&n4.x);	  glMultiTexCoord3fv(2,&col.x); glVertex3fv(&v4.x);
#else		
glMultiTexCoord2f(0,tx1.x,tx1.y);glMultiTexCoord3f(1,n1.x,n1.y,n1.z);  glMultiTexCoord3f(2,col.x,col.y,col.z); glVertex3f(v1.x,v1.y,v1.z);
glMultiTexCoord2f(0,tx2.x,tx2.y);glMultiTexCoord3f(1,n2.x,n2.y,n2.z);  glMultiTexCoord3f(2,col.x,col.y,col.z); glVertex3f(v2.x,v2.y,v2.z);
glMultiTexCoord2f(0,tx3.x,tx3.y);glMultiTexCoord3f(1,n3.x,n3.y,n3.z);  glMultiTexCoord3f(2,col.x,col.y,col.z); glVertex3f(v3.x,v3.y,v3.z);
glMultiTexCoord2f(0,tx4.x,tx4.y);glMultiTexCoord3f(1,n4.x,n4.y,n4.z);  glMultiTexCoord3f(2,col.x,col.y,col.z); glVertex3f(v4.x,v4.y,v4.z);
#endif

//	glEnd();
	gcc+=2;
}
void quadn(
	vec3 v1,vec3 v2,vec3 v3,vec3 v4,
	vec3 n1,vec3 n2,vec3 n3,vec3 n4,
	vec3 col)
{
//render
	//glBegin(GL_TRIANGLES);	//
//	glBegin(GL_QUADS);
		glMultiTexCoord2fv(0,&col.x);glMultiTexCoord2fv(1,&n1.x);	   glVertex3fv(&v1.x);
		glMultiTexCoord2fv(0,&col.x);glMultiTexCoord2fv(1,&n2.x);	   glVertex3fv(&v2.x);
		glMultiTexCoord2fv(0,&col.x);glMultiTexCoord2fv(1,&n3.x);	   glVertex3fv(&v3.x);
		glMultiTexCoord2fv(0,&col.x);glMultiTexCoord2fv(1,&n4.x);	   glVertex3fv(&v4.x);
//	glEnd();
	gcc+=2;
}

#include <execinfo.h>
/* Obtain a backtrace and print it to stdout. */
void print_trace(void)
{
      void *array[10];
       size_t size;
       char **strings;
       size_t i;
     
       size = backtrace (array, 10);
       strings = backtrace_symbols (array, size);
     
       printf ("Obtained %zd stack frames.\n", size);
     
       for (i = 0; i < size; i++)
          printf ("%s\n", strings[i]);
     
       free (strings);
}

void quad3d(vec3 v1,vec3 v2,vec3 v3,vec3 v4,vec3 col)
{
	glMultiTexCoord3fv(0,&col.x);		glVertex3fv(&v1.x);
	glMultiTexCoord3fv(0,&col.x);		glVertex3fv(&v2.x);
	glMultiTexCoord3fv(0,&col.x);		glVertex3fv(&v3.x);
	glMultiTexCoord3fv(0,&col.x);		glVertex3fv(&v4.x);
}
/*
void conv3d(vec3 v1,vec3 &w2) 
{
	vec3 w1=v1-eye;
	
	float1 x=dot(w1,cam.x);
	float1 y=dot(w1,cam.y);
	float1 z=dot(w1,cam.z);
	if(z<0.0) return ;

	x=xx/2+ x*zoom*yy/z+xx/4;
	y=yy/2- y*zoom*yy/z;

	w2.x=x;
	w2.y=y;
	w2.z=z;
}
void pixel3d(vec3 v1,int col)
{
	vec3 w1;
	conv3d(v1,w1);
	
	int xc=(int)w1.x;
	int yc=(int)w1.y;
	
	if(xc<0) return ;
	if(yc<0) return ;
	if(xc>=xx) return ;
	if(yc>=yy) return ;
	
	loopd(y2,0,3,1)	
	loopd(x2,0,3,1)
		pixel(xc+x2,yc+y2,col);
}


void quad3d(int indx,int r,int g,int b)
{
	int col=(r<<16)+(g<<8)+b;
	vec3 v1,v2,v3,v4;
	
	conv3d(pos[indx],v1);
	conv3d(pos[indx+1],v2);
	conv3d(pos[indx+hh],v3);
	conv3d(pos[indx+1+hh],v4);
	
	loopd(y,0,8,1)
	loopd(x,0,8,1)
	{
		vec3 wa=v1+(v2-v1)*(float1)x/8.0;
		vec3 wb=v3+(v4-v3)*(float1)x/8.0;
		vec3 w1=wa+(wb-wa)*(float1)y/8.0;
		int xc=(int)w1.x;
		int yc=(int)w1.y;

		pixel(xc,yc,col);
	}
}
*/








#if 0
#include <GL/glx.h>
#include <GL/gl.h>
#include <unistd.h>
#include <iostream>
 
#define GLX_CONTEXT_MAJOR_VERSION_ARB		0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB		0x2092
typedef GLXContext (*GLXCREATECONTEXTATTRIBSARBPROC)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
 
 
int main (int argc, char ** argv){
	Display *display5 = XOpenDisplay(0);
 
	int nelements;
	GLXFBConfig *fbc = glXChooseFBConfig(display5, DefaultScreen(display5), 0, &nelements);
 
	static int attributeList[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, None };
	XVisualInfo *vi = glXChooseVisual(display5, DefaultScreen(display5),attributeList);
 
	XSetWindowAttributes swa;
	swa.colormap = XCreateColormap(display5, RootWindow(display5, vi->screen), vi->visual, AllocNone);
	swa.border_pixel = 0;
	swa.event_mask = StructureNotifyMask;
	Window win = XCreateWindow(display5, RootWindow(display5, vi->screen), 0, 0, 100, 100, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel|CWColormap|CWEventMask, &swa);
 
	XMapWindow (display5, win);
 
	//oldstyle context:
	//	GLXContext ctx = glXCreateContext(display5, vi, 0, GL_TRUE);
 
	std::cout << "glXCreateContextAttribsARB " << (void*) glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB") << std::endl;
	GLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = (GLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
 
	int attribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
		GLX_CONTEXT_MINOR_VERSION_ARB, 0,
		0};
 
	GLXContext ctx = glXCreateContextAttribsARB(display5, *fbc, 0, true, attribs);
 
	glXMakeCurrent (display5, win, ctx);
 
		glClearColor (0, 0.5, 1, 1);
		glClear (GL_COLOR_BUFFER_BIT);
		glXSwapBuffers (display5, win);
 
		sleep(1);
 
		glClearColor (1, 0.5, 0, 1);
		glClear (GL_COLOR_BUFFER_BIT);
		glXSwapBuffers (display5, win);
 
		sleep(1);
 
	ctx = glXGetCurrentContext(); 
	glXDestroyContext(display5, ctx); 
	}
	
	
	
	
	
	
	
	
	
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
vec3 qbezier(vec3 v1,vec3 v2,vec3 v3,vec3 v4,float1 t1)
{
	float1 t2=t1*t1;
	float1 t3=t2*t1;
	float1 u1=1.0-t1;
	float1 u2=u1*u1;
	float1 u3=u2*u1;
	
	return v1*u3 + v2*t1*u2*3.0 + v3*t2*u1*3.0 + v4*t3;
}

void rotatev(vec3 &v1,vec3 rotz,float1 alf)
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



template<typename T>
class qarray
{
public:
	vector<T*> ptr;
	int gsize,fullsize,addr;
	
#define	SLICE 64
#define SLICE_SHIFT 6
	
	qarray() {ptr.clear();gsize=0;fullsize=0;};

	~qarray() {	clear();};
	int size() {return gsize;};

	void push_back(T v) 
	{
		int indx=gsize;

		gsize++;
		extend(gsize);
		
		addr=indx>>SLICE_SHIFT;
		indx=indx&(SLICE-1);
		
		ptr[addr][indx]=v; 
	};
	void extend(int newsize)
	{
		newsize/=SLICE;
		newsize+=1;
		newsize*=SLICE;

		if(newsize>fullsize)
		{
			int ii=fullsize>>6;
			addr=newsize>>6;
			
			loopd(i,ii,addr,1)
			{
				ptr.push_back(new T[SLICE]);
				memset(ptr.back(),0,sizeof(T)*SLICE);
			}

			fullsize=newsize;
		}
	}
	void clear() 
	{
		loopd(i,0,ptr.size(),1)
			if(ptr[i]) delete [] ptr[i];

		ptr.clear();
		gsize=0;
		fullsize=0;
	};


	T& operator[](int indx) 
	{
		if(indx<0) {printf("indx<0\n");exit(3);}
		if(indx>10000) {printf("indx>10000\n");exit(3);}
		
		if(indx>=gsize) {extend(indx+1);gsize=indx+1;}//printf("operator [] big indx\n");

		addr=indx>>SLICE_SHIFT;
		indx=indx&(SLICE-1);
		
		return ptr[addr][indx];
	};
};



vec3 gettxctang(vec3 w2,vec3 *tang)
{
	vec3 stxc;
    
	stxc.x=tang[0].x + dot(tang[1],w2);//tangent space
	stxc.y=tang[0].y + dot(tang[2],w2);
    stxc.z=0;

    return stxc;
}
vec3 gettxctang3(vec3 w2,vec3 *tang)
{
	vec3 stxc;
    
	stxc.x=tang[0].x + dot(tang[1],w2);//tangent space
	stxc.y=tang[0].y + dot(tang[2],w2);
	stxc.z=tang[0].z + dot(tang[3],w2);

//	stxc=tang[0] + tang[3]*dot(tang[1],w2) + tang[4]*dot(tang[2],w2);

    return stxc;
}
void calc_tangent(vec3 &v1,vec3 &v2,vec3 &v3,vec3 &txc1,vec3 &txc2,vec3 &txc3, vec3 *_tang)
{
	vec3 nx=v2-v1;//keyword in shader tsw
	vec3 ny=v3-v1;
	vec3 nnx=normalize(nx);

	vec3 ux=txc2-txc1;
	vec3 uy=txc3-txc1;
	vec3 uux=normalize(ux);

	ny=ny- nnx*dot(nnx,ny);;//levagja a kilogo Y reszt!
	uy=uy- uux*dot(uux,uy);//2d ben ugyanugy!
/*
//	nx=normalize(nx)/length(nx);//ha nx 10, akkor 0.1el kell szorozni, hogy 1 legyen!
//	ny=normalize(ny)/length(ny);
*/
	nx=nx/dot(nx,nx);//ugyan az!
	ny=ny/dot(ny,ny);
	vec3 nxx=nx*ux.x + ny*uy.x;//a kesobbi stxc= bol w1 kiemelve, egyszerusites!
	vec3 nyy=nx*ux.y + ny*uy.y;
	//mivel stxc.x=ux.x*( nx.x*w1.x + nx.y*w1.y + nx.z*w1.z) +uy.x*( ny.x*w1.x + ny.y*w1.y + ny.z*w1.z) ;
/*
//	vec3 w1=sobj - v1;
//	stxc=txc1 + ux*dot(nx,w1) + uy*dot(ny,w1);//tangent space witout matrix ! OKE
    */

	_tang[0]=txc1;
	_tang[1]=nxx;
	_tang[2]=nyy;


/*
tang[i]=txc1;//test
tang[i+1]=txc2;
tang[i+2]=txc3;
*/
}
void calc_tangent3(vec3 &v1,vec3 &v2,vec3 &v3,vec3 &txc1,vec3 &txc2,vec3 &txc3, vec3 *_tang)
{
	vec3 nx=v2-v1;//keyword in shader tsw
	vec3 ny=v3-v1;
	vec3 nnx=normalize(nx);

	vec3 ux=txc2-txc1;
	vec3 uy=txc3-txc1;
	vec3 uux=normalize(ux);

	ny=ny- nnx*dot(nnx,ny);;//levagja a kilogo Y reszt!
	uy=uy- uux*dot(uux,uy);//2d ben ugyanugy!
//	uy=uy- nnx*dot(nnx,uy);//2d ben ugyanugy!

/*
//	nx=normalize(nx)/length(nx);//ha nx 10, akkor 0.1el kell szorozni, hogy 1 legyen!
//	ny=normalize(ny)/length(ny);
*/
	nx=nx/dot(nx,nx);//ugyan az!
	ny=ny/dot(ny,ny);
	vec3 nxx=nx*ux.x + ny*uy.x ;//a kesobbi stxc= bol w1 kiemelve, egyszerusites!
	vec3 nyy=nx*ux.y + ny*uy.y;
	vec3 nzz=nx*ux.z + ny*uy.z;
//mivel	
//stxc.x=txc1.x+ux.x*( nx.x*w1.x + nx.y*w1.y + nx.z*w1.z) +uy.x*( ny.x*w1.x + ny.y*w1.y + ny.z*w1.z) ;
//stxc.x=txc1.x+ux.x*nx.x*w1.x + ux.x*nx.y*w1.y + ux.x*nx.z*w1.z +uy.x*ny.x*w1.x + uy.x*ny.y*w1.y + uy.x*ny.z*w1.z ;
//stxc.x=txc1.x+(ux.x*nx.x+uy.x*ny.x)*w1.x + (ux.x*nx.y+ uy.x*ny.y)*w1.y + (ux.x*nx.z+ uy.x*ny.z)*w1.z ;
/*
x axis!
1=dot(nxx,w1)
0=dot(nyy,w1)
0=dot(nzz,w1)

1=nxx.x*w1.x + nxx.y*w1.y + nxx.z*w1.z
0=nyy.x*w1.x + nyy.y*w1.y + nyy.z*w1.z
0=nzz.x*w1.x + nzz.y*w1.y + nzz.z*w1.z

w1.x=nxx.x*1 + nyy.x*0 + nzz.x*0
w1.y=nxx.y*1 + nyy.y*0 + nzz.y*0
w1.z=nxx.z*1 + nyy.z*0 + nzz.z*0


//	vec3 w1=sobj - v1;
//	stxc=txc1 + ux*dot(nx,w1) + uy*dot(ny,w1);//tangent space witout matrix ! OKE
    */

	_tang[0]=txc1;
	_tang[1]=nxx;
	_tang[2]=nyy;
	_tang[3]=nzz;
/*
	_tang[1]=nx;
	_tang[2]=ny;
	_tang[3]=ux;
	_tang[4]=uy;*/

/*
tang[i]=txc1;//test
tang[i+1]=txc2;
tang[i+2]=txc3;
*/
}



void qsleep(float1 dt)
{
	timespec ti1,ti2;

	ti1.tv_sec=0;
	ti1.tv_nsec=(int)(1e9*dt);

	nanosleep(&ti1,&ti2);
}
double q_getcpuclock()
{
#ifdef __LINUX__
        timespec tim;
        timespec timres;
        clockid_t cid=0;
        
//      clock_getcpuclockid(0,&cid);
//      clock_gettime(cid,&tim);
//      clock_getres(CLOCK_REALTIME,&timres);

        clock_gettime(CLOCK_MONOTONIC,&tim);    
//      clock_gettime(CLOCK_REALTIME,&tim);

        double ti=(double)tim.tv_sec + (double)tim.tv_nsec/1e9;
//printf("%d %d \n",timres.tv_sec,timres.tv_nsec);
        return ti;
#else
//	return (dd)GetTickCount()*0.001f;
        LARGE_INTEGER   freq;
        LARGE_INTEGER   counter;

        QueryPerformanceFrequency( &freq );
        QueryPerformanceCounter( &counter );
        
        return ( static_cast< double >( counter.QuadPart ) / 
                        static_cast< double >( freq.QuadPart ) );// * 100.0f
#endif
}
double ti22=0;

double xtimer(const char *ss)
{
	double ti1=q_getcpuclock();//zsirtimer
	double deltaSeconds=ti1-ti22;
	ti22=ti1;

	if(deltaSeconds==0) deltaSeconds=0.000001;
	if(ss) printf("%s : %f sec %f fps\n",ss,deltaSeconds,1.0/deltaSeconds);
	return deltaSeconds;
}
/*
void load(const char *name, void *buf,int size)
{
	FILE *ff=fopen(name,"rb");
	if(ff<=0) printf("ERROR %s \n",name);
	else
	{
		fread(buf,1,size,ff);
		fclose(ff);
	}
}
*/
#endif




/*	
			(glGenFramebuffers(1, &texFBOID)); 
			(glBindFramebuffer(GL_FRAMEBUFFER, texFBOID));

			(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 ,GL_TEXTURE_2D, l_pColor->mTexture->Get_Texture()->GetTexID(), 0));
					(glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,l_pDepth->mTexture->Get_Texture()->GetTexID(),0 ));
					
                    (glGenRenderbuffers(1, &texRBOID));
                    (glBindRenderbuffer(GL_RENDERBUFFER, texRBOID));//GL_DEPTH_COMPONENT16

                        (glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, l_pColor->mTexture->Width(), l_pColor->mTexture->Height()));
//                    #else
                        (glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                                                  l_pColor->mTexture->Width() * scalex,
                                                  l_pColor->mTexture->Height() * scaley
                                                  ));                
  //                  #endif

					(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT , GL_RENDERBUFFER, texRBOID));



				GLenum status;
				(status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
				if(status != GL_FRAMEBUFFER_COMPLETE)
				{
					switch(status)
					{
					case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
						//An attached render buffer or texture is not complete.
						XAssert(false);
						break;

					case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
						//There is not at least one attached image.
						XAssert(false);
						break;

							case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
							//All attached images do not have the same width and height.
							XAssert(false);
							break;
						case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
							//All images attached to the attachment points GL_COLOR_ATTACHMENT0_EXT through GL_COLOR_ATTACHMENTn_EXT do not have the same internal format.
							XAssert(false);
							break;

					case GL_FRAMEBUFFER_UNSUPPORTED:
						XAssert(false);
						break;
					};
			glDeleteFramebuffers(1,&texFBOID);


     (glBindFramebuffer(GL_FRAMEBUFFER, texFBOID)); 




//copy
		glBindFramebuffer(GL_READ_FRAMEBUFFER,  src->texFBOID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->texFBOID);

    
		glBlitFramebuffer(0, 0, src->Width, src->Height,
						  0, 0, this->Width, this->Height,
						  GL_DEPTH_BUFFER_BIT,
						  GL_NEAREST);//GL_LINEAR




I test


printf("%e %e %e \n",I.x,I.y,I.z);

float1 m=mass/(20.0*20.0*20.0);
I=vec3(0,0,0);
loopd(z,-10,11,1)
loopd(y,-10,11,1)
loopd(x,-10,11,1)
{
	vec3 p1=half_size*vec3(x,y,z)/10.0;
	vec3 p2;
	float1 r;
	
	p2=p1; p2.x=0;	r=length(p2);	I.x+=m*r*r;
	p2=p1; p2.y=0;	r=length(p2);	I.y+=m*r*r;
	p2=p1; p2.z=0;	r=length(p2);	I.z+=m*r*r;
}
printf("%e %e %e \n",I.x,I.y,I.z);
exit(3);


#define	add_force()
	vel_v=vec3(0,0,0) - spd - cross(R,omega);
	vel=dot(vel_v,normal);
	F=normal*A*vel*fabs(vel)*ro*C*0.5;
	F_all+=F;
	M_all+=cross(F,R);

*/

void rainbow4(int &r,int &g,int &b,int t,int h,
	int r1,int g1,int b1,
	int r2,int g2,int b2,
	int r3,int g3,int b3,
	int r4,int g4,int b4,
	int r5,int g5,int b5)
{
	if(t>h) t=h;
	int o=t*4/h;
	h/=4;
	t%=h;	
	float1 t2=(float1)t/(float1)h;
	
	if(o==0)
	{
		r=r1 +(r2-r1)*t2;
		g=g1 +(g2-g1)*t2;
		b=b1 +(b2-b1)*t2;
	}
	if(o==1)
	{
		r=r2 +(r3-r2)*t2;
		g=g2 +(g3-g2)*t2;
		b=b2 +(b3-b2)*t2;
	}
	if(o==2)
	{
		r=r3 +(r4-r3)*t2;
		g=g3 +(g4-g3)*t2;
		b=b3 +(b4-b3)*t2;
	}
	if(o==3)
	{
		r=r4 +(r5-r4)*t2;
		g=g4 +(g5-g4)*t2;
		b=b4 +(b5-b4)*t2;
	}
}











