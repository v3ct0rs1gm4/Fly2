/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "q_file.h"

qfile::qfile()
{
QDUMPFNC();
	qassert(this);
	cp=QCACHESIZE+1;
	eof=0;
	size=0;

	ff=0;
	return;
}
qfile::qfile(const char *_name,const char *mask)
{
QDUMPFNC();
	qassert(this);
	cp=QCACHESIZE+1;
	eof=0;
	size=0;
	
	qassert(_name);
	qassert(mask); 
	printf("qfile %s \n",_name);
	
	name=_name;
	ff=fopen(name.c_str(),mask);
	if(ff==NULL) {printf("ERROR FILE not found %s %s \n",name.c_str(),mask);}
	else	printf("FILE %s opened %s\n",name.c_str(),mask);
	return;
}
qfile::~qfile()
{
QDUMPFNC();
	qassert(this);
	qassert(ff);
	if(ff) {printf("ERROR FILE was not opened\n");}//possible has no name
	else	printf("FILE %s closed\n",name.c_str());

	if(ff) fclose(ff);
	ff=0;
	return;
}

int qfile::qwrite(void *buf,int n,int n2)
{
QDUMPFNC();
	qassert(this);
	if(ff==NULL) {printf("ERROR FILE not opened\n");return 0;}//possible has no name
	
	return fwrite(buf,n,n2,ff);	
}
int qfile::qread(uchar *buf,int n,int n2)
{
QDUMPFNC();
	qassert(this);
	if(ff==NULL) {printf("ERROR FILE not opened\n");return 0;}//possible has no name

	int h=fread(buf,n,n2,ff);//printf("qread hh %d %d %d %d \n",h,n,n2,ff);
	return h;	
}
	

void qfile::qwrite1i(const char *mask,int n)
{
QDUMPFNC();
	qassert(this);
	char ss[512];

	sprintf(ss,"%s %d \n\r",mask,n); 
	qwrite(ss,1,strlen(ss));
}
void qfile::qwrite1f(const char *mask,float1 v1)
{
QDUMPFNC();
	qassert(this);
	char ss[512];

	sprintf(ss,"%s %e\n\r",mask,v1); 
	qwrite(ss,1,strlen(ss));
}
void qfile::qwrite3f(const char *mask,float1 v1,float1 v2,float1 v3)
{
QDUMPFNC();
	qassert(this);
	char ss[512];

	sprintf(ss,"%s %e %e %e \n\r",mask,v1,v2,v3); 
	qwrite(ss,1,strlen(ss));
}
void qfile::qwrite3i(const char *mask,int v1,int v2,int v3)
{
QDUMPFNC();
	qassert(this);
	char ss[512];
	
	sprintf(ss,"%s %d %d %d \n\r",mask,v1,v2,v3); 
	qwrite(ss,1,strlen(ss));
}



uchar qfile::cache_next()
{
	if(cp>=QCACHESIZE) {size=qread(cache,1,QCACHESIZE);cp=0;}//BUG forditva nem jo mert lehet hogy nincs akkora a file
	if(size<=0) eof=1;
	if(cp>=size) eof=1;//printf("cp %d size %d  eof %d c %c \n",cp,size,eof,cache[cp]);

	return cache[cp];
}
int qfile::getword(uchar *word)
{
	if(eof) return 0;
	while(eof==0 && is_alpnum(cache_next())==0) cp++; 
	if(eof) return 0;
	int p=0;
	while(eof==0 && is_alpnum(cache_next())) {word[p]=cache_next();p++;cp++;}
	word[p]=0;
//printf("word: %s \n",word);
	return 1;
}


void qfile::qread1i(const char *mask,int &n)
{
	uchar ss[512];

	if(getword(ss))
	{
		if(strcmp((const char*)ss,mask)==0)//		sprintf(ss,"%s %d \n\r",mask,n); 
		{
			if(getword(ss))	n=getnumi(ss);
			else printf("ERROR qreadi %s\n",mask);
		}
	}
}
void qfile::qread1f(const char *mask,float1 &v1)
{ 
	uchar ss[512];
	if(getword(ss))
	{
		if(strcmp((const char*)ss,mask)==0)//		sprintf(ss,"%s %e %e %e \n\r",mask,v1,v2,v3); 
		{
			if(getword(ss)) v1=getnumf(ss);
		}
		else printf("ERROR qread3f %s\n",mask);
	}
}
void qfile::qread3f(const char *mask,float1 &v1,float1 &v2,float1 &v3)
{ 
	uchar ss[512];
	if(getword(ss))
	{
		if(strcmp((const char*)ss,mask)==0)//		sprintf(ss,"%s %e %e %e \n\r",mask,v1,v2,v3); 
		{
			if(getword(ss)) v1=getnumf(ss);
			if(getword(ss)) v2=getnumf(ss);
			if(getword(ss)) v3=getnumf(ss);
		}
		else printf("ERROR qread3f %s\n",mask);
	}
}
void qfile::qread3i(const char *mask,int &v1,int &v2,int &v3)
{
	uchar ss[512];
	if(getword(ss))
	{
		if(strcmp((const char*)ss,mask)==0)//		sprintf(ss,"%s %d %d %d \n\r",mask,v1,v2,v3); 
		{
			if(getword(ss)) v1=getnumi(ss);
			if(getword(ss)) v2=getnumi(ss);
			if(getword(ss)) v3=getnumi(ss);
		}
		else printf("ERROR qread3i %s\n",mask);
	}
}


void save(const char *name, void *buf,int size)
{
QDUMPFNC();
	FILE *ff=fopen(name,"wb");
	if(ff==NULL) printf("ERROR %s\n",name);
	fwrite(buf,1,size,ff);
	fclose(ff);
}
int load(const char *name, void *buf,int size)
{
QDUMPFNC();
	FILE *ff=fopen(name,"rb");
	if(ff==NULL) printf("ERROR %s\n",name);
	int h=fread(buf,1,size,ff);
	fclose(ff);
	return h;
}

