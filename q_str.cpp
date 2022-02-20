/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include "q_str.h"



int str_p=0,str_h=0;
char *str_buf=0,word2[512];

int is_number2[256],is_alfa2[256],is_mix2[256];


void init_str()
{
	is_number2['0']=1;
	is_number2['1']=1;
	is_number2['2']=1;
	is_number2['3']=1;
	is_number2['4']=1;
	is_number2['5']=1;
	is_number2['6']=1;
	is_number2['7']=1;
	is_number2['8']=1;
	is_number2['9']=1;

	is_number2['.']=1;//?
	is_number2['-']=1;

	loopd(i,0,26,1)
	{
		is_alfa2['a'+i]=1;
		is_alfa2['A'+i]=1;
	}
	is_alfa2['_']=1;
	is_alfa2['*']=1;

	loopd(i,0,256,1) is_mix2[i]=is_alfa2[i] | is_number2[i];
}
void search_str(const char *ss)
{
	int ok=1,i=0;

	//      debugi("p  ",str_p);      debugi("h  ",str_h);

	while(ok&&str_p<str_h)
	{
		i=0;
		while(ss[i]!=0&&ss[i]==str_buf[str_p+i]) {i++;}
		if(ss[i]==0&&str_buf[str_p+i]==' ') ok=0;
		str_p++;
	}
	str_p+=i;
	if(str_p>=str_h) error2("search str overflow ",ss);
	//      debugi("qwe  ",str_p);
	//      errori("search ",str_p);
}
bool search_str2(const char *ss)
{
	int ok=1,i,p=str_p;

	while(ok&&p<str_h&&str_buf[p]!='}')
	{
		i=0;
		while(ss[i]!=0&&ss[i]==str_buf[p+i]&&str_buf[p+i]!='}') {i++;}
		if(ss[i]==0&&str_buf[p+i]==' ') ok=0;
		p++;
	}
	p++;
	if(ok==0) {str_p=p;return(1);}
	else return(0);
}
bool search_str3(const char *ss,const char last)
{
	int ok=1,i,p=str_p;

	while(ok&&p<str_h&&str_buf[p]!='}')
	{
		i=0;
		while(ss[i]!=0&&ss[i]==str_buf[p+i]&&str_buf[p+i]!='}') {i++;}
		if(ss[i]==0&&str_buf[p+i]==last) ok=0;
		p++;
	}
	if(ok==0) {str_p=p;return(1);}
	else return(0);
}
bool search_str_chk(const char *ss)
{
	int ok=1,i,p=str_p;

	while(ok&&p<str_h)
	{
		i=0;
		while(ss[i]!=0&&ss[i]==str_buf[p+i]) {i++;}
		if(ss[i]==0&&str_buf[p+i]==' ') ok=0;
		p++;
	}
	return(p<str_h);
}
bool search_str_chk2(const char *ss)
{
	int ok=1,i,p=str_p;

	while(ok&&p<str_h)
	{
		i=0;
		while(ss[i]!=0&&ss[i]==str_buf[p+i]) {i++;}
		if(ss[i]==0&&str_buf[p+i]==' ') ok=0;
		p++;
	}
	p++;
	if(ok==0) {str_p=p;return(1);}
	else return(0);
}
void search_enter()
{
	while(str_buf[str_p]!=13&&str_p<str_h)
	{
		str_p++;
	}
}
int search_star()
{
	while(str_buf[str_p]!='*'&&str_p<str_h)
	{
		str_p++;
	}
	if(str_p<str_h) return 1;
	return 0;
}
int read_command()
{
	int i=0;
	while((is_mix2[str_buf[str_p]] ) && str_p<str_h)
	{
		word2[i]=str_buf[str_p];
		str_p++;
		i++;
	}
	word2[i]=0;
	
	if(str_p<str_h) return 1;
	return 0;
}
void search_chr(const char a)
{
	while(str_buf[str_p]!=a&&str_p<str_h)
	{
		str_p++;
	}
	str_p++;
}
int read_int()
{
	int n=0,sg=1;

	if(str_p>=str_h) return(0);

	while(!is_number2[str_buf[str_p]]) str_p++;
	if(str_buf[str_p]=='-') {sg=-1;str_p++;}
	while(is_number2[str_buf[str_p]])
	{
		n*=10;
		n+=(str_buf[str_p]-'0');
		str_p++;
	}
	return(n*sg);
}
float read_dd()
{
	float n=0.0,sg=1.0,exp=0.0;

	if(str_p>=str_h) return(0.0);

	while(!is_number2[str_buf[str_p]]) str_p++;
	if(str_buf[str_p]=='-') {sg=-1.0;str_p++;}

	while(is_number2[str_buf[str_p]])
	{
		if(str_buf[str_p]=='.')
		{
			exp=1.0;
		}
		else
		{
			n*=10.0;
			n+=(float)(str_buf[str_p]-'0');
			if(exp!=0.0) exp*=10.0;
		}
		str_p++;
	}
	//      if(exp==0.0) error("no point in float !");
	if(exp==0.0) exp=1.0;
	if(n==0.0) return(0.0);
	return((n*sg)/exp);
}
char *read_ss()
{
	int p,i;
	char *ss=new char[512];
	ss[0]=0;

	if(str_p>=str_h) return(0);

	while(str_buf[str_p]!='"') str_p++;
	str_p++;
	while(str_buf[str_p]!='"') str_p++;
	p=str_p-1;
	while(str_buf[p]!='\\'&&str_buf[p]!='"') p--;
	//      while(str_buf[p]!='\\') p--;
	p++;
	i=0;
	while(str_buf[p]!='"') {ss[i]=str_buf[p];i++;p++;}
	ss[i]=0;
/*	if((ss[0]=='a')&&(ss[1]=='_'))//alfas
	{
		ss[i-3]='t';
		ss[i-2]='g';
		ss[i-1]='a';
	}
	else
	{
		if(ss[i-3]=='b'||ss[i-3]=='B') // csak bmp-t
		{
			ss[i-3]='j';
			ss[i-2]='p';
			ss[i-1]='g';
		}
	}*/

	return(ss);
}
char *read_str()
{
	int p,i;
	char *ss=new char[512];
	ss[0]=0;

	if(str_p>=str_h) return(0);

	while(str_buf[str_p]!='"') str_p++;
	str_p++;
	p=str_p;
	i=0;
	while(str_buf[p]!='"'&&i<512) {ss[i]=str_buf[p];i++;p++;}
	if(i>=512) error2("read str overflow ","");
	ss[i]=0;

	return(ss);
}
char read_act()
{
	return(str_buf[str_p]);
}
bool end_ase()
{
	return(str_p>=str_h);
}

int read_word()
{
	int i=0;
	while((!is_mix2[str_buf[str_p]] ) && str_p<str_h) str_p++;
	
	while((is_mix2[str_buf[str_p]] ) && str_p<str_h)
	{
		word2[i]=str_buf[str_p];
		str_p++;
		i++;
	}
	word2[i]=0;
	
	if(str_p<str_h) return 1;
	return 0;
}


