
#ifndef _QSHADERH
#define _QSHADERH

#include "q_engine.h"

extern const char *shader_path;

//#define _DUMPSH			

extern std::unordered_map<int64_t,GLuint>  program_object; 
extern std::unordered_map<qstring,GLuint>  vertex_shader;  
extern std::unordered_map<qstring,GLuint>  fragment_shader;
extern qstring selshader;


void dumpshader(char *ss);


void initshader(qstring nm,GLchar *vertex_source,GLchar *fragment_source);
void loadshader(qstring nm);
void shader_setparm32(qstring2 &name,void *parm_ptr,int h);
void qshader_setparm3(const char *name,void *parm_ptr,int h,IDTYPE &id);
void qshader_setparm2(const char *name,int parm_ptr,IDTYPE &id);  // int
void qsetshader(qstring n,IDTYPE &id);
void loadshaders(qstring name);
void dumpshader(char *ss);
bool isseparator(char a);
void getname4word(char *shadername,char *ss1);
void getname4word2(char *shadername,char *ss1);
void getmodule4buf(int i,int type);
int getmoduletype(char *ss2,char *modulname)		;
void buildvary(qstring &source, qstring module_base,int ii);
void buildshader(char *shadername,	qstring &vertex_source, qstring &fragment_source);
void loadshadersC(qstring fname);


#endif



