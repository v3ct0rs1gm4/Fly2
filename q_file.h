
#ifndef _QFILEH
#define _QFILEH

#include "q_tools.h"

#define QCACHESIZE (64*1024)


class qfile
{
//private:
public:
	qchar name;
	uchar cache[QCACHESIZE+2];
	int cp;
	int eof;
	int size;


	FILE *ff;
	qfile();
	qfile(const char *_name,const char *mask);
	~qfile();
	int qwrite(void *buf,int n,int n2);
	int qread(uchar *buf,int n,int n2);

	void qwrite1i(const char *mask,int n);
	void qwrite1f(const char *mask,float1 v1);
	void qwrite3f(const char *mask,float1 v1,float1 v2,float1 v3);
	void qwrite3i(const char *mask,int v1,int v2,int v3);
	
	uchar cache_next();
	int getword(uchar *ss);
	
	void qread1i(const char *mask,int &n);
	void qread1f(const char *mask,float1 &v1);
	void qread3f(const char *mask,float1 &v1,float1 &v2,float1 &v3);
	void qread3i(const char *mask,int &v1,int &v2,int &v3);

	
};
void save(const char *name, void *buf,int size);
int load(const char *name, void *buf,int size);

#endif

