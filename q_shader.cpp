/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "q_engine.h"


const char *shader_path="shaders";

//#define _DUMPSH			

std::unordered_map<int64_t,GLuint>  program_object; 
std::unordered_map<qstring,GLuint>  vertex_shader;  
std::unordered_map<qstring,GLuint>  fragment_shader;
qstring selshader;


void dumpshader(char *ss);


void initshader(qstring nm,GLchar *vertex_source,GLchar *fragment_source)
{
QDUMPFNC();	
//printf("initshader %s %s\n",vertex_source,fragment_source);	
   GLuint tmp= ERR2(glCreateProgram());    // creating a program object
   program_object[qhash64(nm)]=tmp;
   vertex_shader[nm]   = ERR2(glCreateShader(GL_VERTEX_SHADER));   // creating a vertex shader object
   fragment_shader[nm] = ERR2(glCreateShader(GL_FRAGMENT_SHADER)); // creating a fragment shader object
   printProgramInfoLog(tmp);
   
   ERR(glShaderSource(vertex_shader[nm], 1, &vertex_source, NULL)); // assigning the vertex source
   ERR(glShaderSource(fragment_shader[nm], 1, &fragment_source, NULL)); // assigning the fragment source
   printProgramInfoLog(tmp);   // verifies if all this is ok so far
   
   // compiling and attaching the vertex shader onto program
   ERR(glCompileShader(vertex_shader[nm]));
   ERR(glAttachShader(tmp, vertex_shader[nm])); 
   printProgramInfoLog(tmp);   // verifies if all this is ok so far

			GLint status;

			glGetShaderiv(vertex_shader[nm], GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE)
			{
                GLint logLength;
                glGetShaderiv(vertex_shader[nm], GL_INFO_LOG_LENGTH, &logLength);
                if (logLength > 0)
                {
                    GLchar *log = (GLchar *)malloc(logLength);
                    glGetShaderInfoLog(vertex_shader[nm], logLength, &logLength, log);
					printf("Shader (%s)vs compile log: %s  \n",nm.c_str(),log);
					dumpshader((char*)vertex_source);

                    free(log);
                }
				glDeleteShader(vertex_shader[nm]);
			}
#ifdef _DUMPSH			
		printf("vshader ______________________________________________________\n");			
		dumpshader((char*)vertex_source);
#endif
   
   // compiling and attaching the fragment shader onto program
   ERR(glCompileShader(fragment_shader[nm]));
   ERR(glAttachShader(tmp, fragment_shader[nm])); 
   printProgramInfoLog(tmp);   // verifies if all this is ok so far

			

			glGetShaderiv(fragment_shader[nm], GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE)
			{
                GLint logLength;
                glGetShaderiv(fragment_shader[nm], GL_INFO_LOG_LENGTH, &logLength);
                if (logLength > 0)
                {
                    GLchar *log = (GLchar *)malloc(logLength);
                    glGetShaderInfoLog(fragment_shader[nm], logLength, &logLength, log);
					printf("Shader (%s)fs compile log: %s \n",nm.c_str(),log);
					dumpshader((char*)fragment_source);

                    free(log);
                }
				glDeleteShader(fragment_shader[nm]);
			}
			
#ifdef _DUMPSH			
		printf("fshader ______________________________________________________\n");			
		dumpshader((char*)fragment_source);
		printf(" ______________________________________________________\n");			
#endif   
   // Link the shaders into a complete GLSL program.
   ERR(glLinkProgram(tmp));
   printProgramInfoLog(tmp);   // verifies if all this is ok so far

   
   // some extra code for checking if all this initialization is ok
   GLint prog_link_success;
   ERR(glGetObjectParameterivARB(tmp, GL_OBJECT_LINK_STATUS_ARB, &prog_link_success));
   if (!prog_link_success) {
      printf( "The shaders could not be linked\n");
      exit(1);
   }
}
void loadshader(qstring nm)
{
QDUMPFNC();
	GLchar *vertex_source=new GLchar[120100];
	GLchar *fragment_source=new GLchar[120100];
	memset(vertex_source,0,120000);
	memset(fragment_source,0,120000);
	printf("loadshader %s\n",nm.c_str());

	char ss[255];	
	sprintf(ss,"%s/%s.vsh",shader_path,nm.c_str());	qload(ss,vertex_source,120000);
	sprintf(ss,"%s/%s.fsh",shader_path,nm.c_str());	qload(ss,fragment_source,120000);
   
   initshader(nm,vertex_source,fragment_source);
   
   
	delete [] vertex_source;	
	delete [] fragment_source;
	vertex_source=0;	
	fragment_source=0;
}
void shader_setparm32(qstring2 &name,void *parm_ptr,int h)
{
//	if(name.id==-1)
	{
		//GLint 
		name.id = ERR2(glGetUniformLocation(program_object[qhash64(selshader)],name.c_str()));
//		if(id==0) printf("ERROR %s %s\n",selshader.c_str(),name);
	}

	float1 *v1=(float1 *)parm_ptr;
	if(h==1) {ERR(glUniform1f(name.id, v1[0]));}
	if(h==2) {ERR(glUniform2f(name.id, v1[0], v1[1]));}
	if(h==3) {ERR(glUniform3f(name.id, v1[0], v1[1], v1[2]));}
	if(h==4) {ERR(glUniform4f(name.id, v1[0], v1[1], v1[2], v1[3]));}

	if(h==16) {ERR(glUniformMatrix4fv(name.id, 1, GL_FALSE, v1));}//GL_FALSE
}
void qshader_setparm3(const char *name,void *parm_ptr,int h,IDTYPE &id)
{
//	if(id==-1)//TODO
	{	
		//GLint 
		id = ERR2(glGetUniformLocation(program_object[qhash64(selshader)],name));
//		if(id==0) printf("ERROR %s %s\n",selshader.c_str(),name);
	}

	float1 *v1=(float1 *)parm_ptr;
	if(h==1) {ERR(glUniform1f(id, v1[0]));}
	if(h==2) {ERR(glUniform2f(id, v1[0], v1[1]));}
	if(h==3) {ERR(glUniform3f(id, v1[0], v1[1], v1[2]));}
	if(h==4) {ERR(glUniform4f(id, v1[0], v1[1], v1[2], v1[3]));}

	if(h==16) {ERR(glUniformMatrix4fv(id, 1, GL_FALSE, v1));}//GL_FALSE
}
void qshader_setparm2(const char *name,int parm_ptr,IDTYPE &id)  // int
{
//	if(id==-1)//  fuggvenybol nem jo! ha a shader mindig mas 
	{
		//GLint 
		id = ERR2(glGetUniformLocation(program_object[qhash64(selshader)],name));
//		if(id==0) 	printf("ERROR %s %s\n",selshader.c_str(),name);
	}
	ERR(glUniform1i(id, parm_ptr));
}
void qsetshader(qstring n,IDTYPE &id)
{
	if(id==-1)//TODO
	{
		printf("USED qsetshader: %s \n",n.c_str());
		id=program_object[qhash64(n)];
		if(id==0) printf("ERROR %s\n",n.c_str());
	}
	ERR(glUseProgram(id));
	selshader=n;
}

void loadshaders(qstring name)
{
QDUMPFNC();
	char fname[92]={0};
	sprintf(fname,"%s/%s",shader_path,name.c_str());
	
	GLchar *mem=new GLchar[200000];
	int h=qload(fname,mem,200000);
	if(h<=0) printf("ERROR %s\n",fname);
	
	GLchar *vertex_source=new GLchar[120100];
	GLchar *fragment_source=new GLchar[120100];
	int p=0;
	char nm[92]; nm[0]=0;
	
	while(mem[p]!='~')	 p++;
	while(p<h)
	{
		int ok=0;
		memset(vertex_source,0,120000);
		memset(fragment_source,0,120000);
		
		if(p>=h) break;
		while(p<h && mem[p]=='~')	 p++;
		if(p>=h) break;

		while(p<h && mem[p]!='#')	 p++;
		if(mem[p+1]=='s')
		if(mem[p+2]=='h')
		if(mem[p+3]=='a')
		if(mem[p+4]=='d')
		if(mem[p+5]=='e')
		if(mem[p+6]=='r')
		if(mem[p+7]=='n')
		if(mem[p+8]=='a')
		if(mem[p+9]=='m')
		if(mem[p+10]=='e')
		{
			p+=12;
			int c2=0;
			while(isletter(mem[p]))
			{
				nm[c2]=mem[p];
				c2++;
				p++;
			}
			nm[c2]=0;
			printf("%s\n",nm);
			ok|=1;
		}
		else printf("ERROR shader name missing\n");
		if(p>=h) break;
		
		int c=0;
		while(mem[p]!='~')	 
		{
			vertex_source[c]=mem[p];
			p++;
			c++;
		}
		if(c) 	ok|=2;

		if(p>=h) break;
		while(mem[p]=='~' )	 p++;
		if(p>=h) break;
		c=0;
		while(mem[p]!='~')	 
		{
			fragment_source[c]=mem[p];
			p++;
			c++;
		}
		if(c) 	ok|=4;
		
		if(ok==(1|2|4))
			initshader(qstring(nm),vertex_source,fragment_source);
	}
   
    delete [] mem;mem=0;
	delete [] vertex_source;	vertex_source=0;
	delete [] fragment_source;fragment_source=0;
}

//new ubershader -----------------------------------------------

char word[100000]={0};
qvector<qstring> shaderfnclist;
qvector<int> shaderposlist;
int *deepbuf=0;
char *filebuf5=0;
enum
{
stype_funct=0,
stype_struct=6,
stype_define=1,
stype_varying=3,
stype_uniform=5,
stype_vshader=2,
stype_fshader=4
};

void dumpshader(char *ss)
{
QDUMPFNC();
	if(ss==0) return;
	
	int p=0,line=1;
	
	printf("%03d ",line);
	while(ss[p])
	{
		printf("%c",ss[p]);
		if(ss[p]==10) 
		{
			line++;
			printf("%03d ",line);
		}
		p++;
	}
	printf("\n");
}
bool isseparator(char a)
{
	if(a>='a')	if(a<='z') return false;
	if(a>='A')	if(a<='Z') return false;
	if(a>='0')	if(a<='9') return false;
	if(a=='_') return false;
	return true;
}
void getname4word(char *shadername,char *ss1)
{
	int w=0;
	int p=0;	
	while(isseparator(ss1[p])) p++;// space vagy tab
	while(!isseparator(ss1[p])) p++;//1 word
	p++;//skip space
	while(!isseparator(ss1[p])) {shadername[w]=ss1[p]; p++;w++;}		
	shadername[w]=0;
	if(w>91) printf("WARNING w>91\n");
}
void getname4word2(char *shadername,char *ss1)
{
	int w=0;
	int p=0;	
	while(isseparator(ss1[p])) p++;// space vagy tab
	while(!isseparator(ss1[p])) p++;//1 word
	while(isseparator(ss1[p])) p++;// space vagy tab
	while(!isseparator(ss1[p])) p++;//2 word
	p++;//skip space
	
	while(!isseparator(ss1[p])) 
		{shadername[w]=ss1[p]; p++;w++;}		
	shadername[w]=0;
	if(w>91) 
	{
		printf("WARNING w>91 2 %s\n",ss1);
	}
}
//kiszedi a hozza tartozo blockot  /function/struct define uniform varying vshader fshader/
void getmodule4buf(int i,int type)
{
	int p=shaderposlist[i],w=0;
	
	if(type&1)//egy sor
	{
		while(filebuf5[p]!=10) {word[w++]=filebuf5[p++];}//enterig megy
		word[w++]=10;
		word[w++]=0;
//		printf("%s",word);//dump
	}
	else// {} vagy unknown
	{
		while(1)
		{
			word[w]=filebuf5[p];
			if(deepbuf[p]<2)//megall, ha az elso/deep==1/ block vege {}  
			if(filebuf5[p]=='}') //block vegeig megy
			{
				if(type==stype_struct)  //struct }; !
					word[++w]=filebuf5[++p];//ha struct volt , kell a pontosvesszo is

				break;
			}
			p++;
			w++;
		}
		w++;
		word[w++]=10;			//  +enter
		word[w++]=0;
//		printf("%s",word);//dump
	}
}
int getmoduletype(char *ss2,char *modulname)		
{
	int type=stype_funct;//undef -> functions 
	if(strstr(ss2,"struct"))  type=stype_struct;
	if(strstr(ss2,"#define")) type=stype_define;
	if(strstr(ss2,"varying")) type=stype_varying;
	if(strstr(ss2,"uniform")) type=stype_uniform;
	if(strstr(ss2,"vshader")) type=stype_vshader;
	if(strstr(ss2,"fshader")) type=stype_fshader;

	if(type==stype_varying ||type==stype_uniform)	getname4word2(modulname,ss2);//a typus van a nev elott
	else					getname4word(modulname,ss2);//csak az azonosito a nev elott

	return type;
}
void buildvary(qstring &source, qstring module_base,int ii)
{
	qvector<qstring> module_ready;
//variables	in reverse 
	for(int i=ii;i>=0;i--)//az elotte levok kellhetnek csak     varying/uniform
	{
		char modulname[92]={0};
		char *ss2=(char *)shaderfnclist[i].c_str();
		int type=getmoduletype(ss2,modulname);

		if(type==stype_varying ||type==stype_uniform)//csak az  undef, varying, uniform
		if(strstr(module_base.c_str(),modulname) ||
  		   strstr(source.c_str(),modulname))//vagy a mar meglevo define
		{
			int ok=1;
			qstring ss=qstring(modulname);
			for(int j=0;j<module_ready.size();j++)
				if(module_ready[j]==ss) {ok=0;break;}

			if(ok)
			{
				getmodule4buf(i,type);
				source+=qstring(word);
				module_ready.push_back(modulname);
			}
		}
	}

//functions	
	//eloszor csak kigyujti , mert majd sorba kell
	qstring source_tmp;
	qvector<qstring> module_funct;
	for(int i=0;i<shaderfnclist.size();i++)//stype_funct  sorba!
	{
		char modulname[92]={0};
		char *ss2=(char *)shaderfnclist[i].c_str();
		int type=getmoduletype(ss2,modulname);

		if(type==stype_funct || type==stype_struct)//  function or struct!
		if(strstr(module_base.c_str(),modulname) ||//csak a root code check
  		   strstr(source.c_str(),modulname))//vagy a mar meglevo define
		{
			int ok=1;
			qstring ss=qstring(modulname);
			for(int j=0;j<module_ready.size();j++)
				if(module_ready[j]==ss) {ok=0;break;}

			if(ok)
			{
				getmodule4buf(i,type);// structnal };  !
				source_tmp+=qstring(word);
				module_ready.push_back(modulname);
				module_funct.push_back(modulname);
			}
		}
	}
	//mivel function hivhat funtiont, es sorba kell, ezert akkor rakja ossze, ha mar mind megvan
	for(int i=0;i<shaderfnclist.size();i++)//stype_funct in funct /    modul hivas modulbol!!
	{
		char modulname[92]={0};
		char *ss2=(char *)shaderfnclist[i].c_str();
		int type=getmoduletype(ss2,modulname);

//printf("modulname %s \n",modulname);
		if(type==stype_funct || type==stype_struct)//  function or struct!
		if(strstr(source_tmp.c_str(),modulname))//teljes source check
		{
			int ok=1;
			qstring ss=qstring(modulname);
			for(int j=0;j<module_ready.size();j++)
				if(module_ready[j]==ss) {ok=0;break;}

			if(ok)
			{
				getmodule4buf(i,type);// structnal };  !
				source_tmp+=qstring(word);
				module_ready.push_back(modulname);
				module_funct.push_back(modulname);
			}
		}
	}


//final variables	
	for(int i=ii;i>=0;i--)//az elotte levok kellhetnek csak     varying/uniform
	{
		char modulname[92]={0};
		char *ss2=(char *)shaderfnclist[i].c_str();
		int type=getmoduletype(ss2,modulname);

		if(type==stype_varying ||type==stype_uniform)//csak az  undef, varying, uniform
		if(strstr(source_tmp.c_str(),modulname))//az osszes functiont nezi!
		{
			int ok=1;
			qstring ss=qstring(modulname);
			for(int j=0;j<module_ready.size();j++)
				if(module_ready[j]==ss) {ok=0;break;}

			if(ok)
			{
				getmodule4buf(i,type);
				source+=qstring(word);
				module_ready.push_back(modulname);
			}
		}
	}

//final functions
	for(int i=0;i<shaderfnclist.size();i++)//stype_funct in funct /    modul hivas modulbol!!
	{
		char modulname[92]={0};
		char *ss2=(char *)shaderfnclist[i].c_str();
		int type=getmoduletype(ss2,modulname);

		if(type==stype_funct || type==stype_struct)//  function or struct!
		{
			int ok=0;
			qstring ss=qstring(modulname);
			for(int j=0;j<module_funct.size();j++)//listabol sorba ujra
				if(module_funct[j]==ss) {ok=1;break;}

			if(ok)
			{
				getmodule4buf(i,type);// structnal };  !
				source+=qstring(word);
				module_ready.push_back(modulname);
			}
		}
	}
}
void buildshader(char *shadername,	qstring &vertex_source, qstring &fragment_source)
{
	for(int i=0;i<shaderfnclist.size();i++)// osszerakja a shadereket paronkent
	{
		char modulname[92]={0};
		char *ss2=(char *)shaderfnclist[i].c_str();
		int type=getmoduletype(ss2,modulname);

		if(strcmp(shadername,modulname)==0)
		{
			getmodule4buf(i,type);
			qstring word2=qstring(word);//store

			if(type==stype_vshader) {buildvary(vertex_source,word2,i); 
				vertex_source+=qstring("void main(void) //");vertex_source+=word2;}
			if(type==stype_fshader) {buildvary(fragment_source,word2,i); 
				fragment_source+=qstring("void main(void) //");fragment_source+=word2;}
		}
		if(type==stype_define)// define biztosan hozza tartozik, ez lesz elorebb!
		{
			getmodule4buf(i,type);
			vertex_source+=qstring(word);
			fragment_source+=qstring(word);
		}
	}
}
void loadshadersC(qstring fname)
{
QDUMPFNC();
	char *buf=0;
	int h=qload(fname.c_str(),(void **)&buf);
	filebuf5=buf;
	printf("shaderC %s %d ---------------------\n",fname.c_str(),h);
	deepbuf=new int[h+1];
	for(int i=0;i<h;i++) deepbuf[i]=0;
//iiddee

	shaderfnclist.clear();
	shaderposlist.clear();

	int p=0,deep=0,rem=0,w=0,base=0;
	while(p<h)
	{
		if(buf[p]=='{') deep++;
		if(buf[p]=='/') if(buf[p+1]=='/')rem=1;//remark
		if(buf[p]==13) rem=0;//remark end
		if(buf[p]==10) rem=0;
		
		if(deep==0)//nulla szintuek
		if(rem==0)//nem megjegyzes
		if(buf[p]!=';')//nem sorveg vagy enter
		if(buf[p]!=10)
		if(buf[p]!=13)
		{
			if(w==0) base=p;//first letter position in buf
			word[w]=buf[p];//store valid string
			w++;
		}
		deepbuf[p]=deep;
		
		if(buf[p]==10) //enter
		{
			word[w]=0;
			if(strlen(word)>0)	//van string
			{
				shaderfnclist.push_back(qstring(word));//printf("%s \n",word);
				shaderposlist.push_back(base);//innen kezdodik
			}
			w=0;
		}
		if(buf[p]=='}') deep--;
		p++;
	}


//build shaders
	
	for(int j=0;j<shaderfnclist.size();j++)
	{
		qstring vertex_source, fragment_source;
		char *ss1=(char *)shaderfnclist[j].c_str();
		if(strstr(ss1,"vshader"))//shader par
		{
			char shadername[92]={0};
			getname4word(shadername,ss1);
			printf("%s \n",shadername);

//			if(strcmp(shadername,"coriol")==0)		//test
			{
				buildshader(shadername,vertex_source,fragment_source);
				initshader(shadername,(GLchar*)vertex_source.c_str(),(GLchar*)fragment_source.c_str());
			}
		}
	}

	printf("END ---------------------\n");
	
	delete [] deepbuf; deepbuf=0;
}


