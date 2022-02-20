/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _QTOOLSH
#define _QTOOLSH

#include "q_antdef.h"


inline int randint()
{
	int a=0;
	a=rand()%256; a<<=8;
	a+=rand()%256; a<<=8;
	a+=rand()%256; a<<=8;
	a+=rand()%256; 
	if(a<0) a=-a;
	return a;
}
inline float frnd5(float n)  // +- NA!
{
	float q=(n*(float)(randint()%10000000)/10000000.0);
	if((rand()%10000)<5000) q=-q;
	return q;
}


inline int is_alpha(uchar s)
{
	if(s>='a')	if(s<='z') return 1;
	if(s>='A')	if(s<='Z') return 1;
	if(s=='_') return 1;
	return 0;
}
inline int is_number09(uchar s)
{
	if(s>='0')	if(s<='9') return 1;
	return 0;
}
inline int is_number(uchar s)
{
	if(s>='0')	if(s<='9') return 1;
	if(s=='+') return 1;//?
	if(s=='-') return 1;
	if(s=='.') return 1;//	if(s=='e') return 1;BUG
	return 0;
}
inline int is_alpnum(uchar s)
{
	if(s>='a')	if(s<='z') return 1;
	if(s>='A')	if(s<='Z') return 1;
	if(s>='0')	if(s<='9') return 1;
	if(s=='_') return 1;
	if(s=='+') return 1;
	if(s=='-') return 1;
	if(s=='.') return 1;

	return 0;
}
inline int getnumi(uchar *buf)
{
	int n=0,sg=1,p=0;

	if (buf[p]==0) return(0.0);
	
	while (buf[p] && !is_number(buf[p])) p++;
	if (buf[p] == '-') { sg = -1;	p++;}

	if (buf[p]==0) return(0);
		
	while (buf[p] && is_number(buf[p]))
	{
		if (buf[p] == '.')
		{
		}
		else
		{
			n *= 10;
			n += (int)(buf[p] - '0');
		}
		p++;
	}
	return (n*sg);
}

inline float getnum(uchar *buf,int &p)
{
	int expinited=0;
	float n=0,exp=-999.0,sg=1.0;

	if (buf[p]==0) return(0.0);
	
	while (buf[p] && !is_number(buf[p])) p++;
	if (buf[p] == '-') { sg = -1.0;	p++;}

	if (buf[p]==0) return(0.0);
		
	while (buf[p] && is_number(buf[p]))
	{
		if (buf[p] == '.')
		{
			exp = 1.0;
			expinited=1;
		}
		else
		if (is_number09(buf[p]))/// +e nem!   BUG
		{
			n *= 10.0;
			n += (float)(buf[p] - '0');
			if (expinited) exp *= 10.0;
		}
		p++;
	}
//printf("<%e %e %e> \n",n,sg,exp);	
	if(expinited==0) return n*sg;
	return ((n*sg) / exp);
}
inline float getnumf(uchar *buf)
{
	int p=0;
	float ex=0.0;
	float n=getnum(buf,p);

	if (buf[p] == 'e') 
	{
		p++;
		ex=getnum(buf,p);//			printf("<! %e %e>",n,ex);
	}
	return n*pow(10.0,ex);
}



template<typename T,int N>
class fix_array
{
public:
	T ptr[N];
	T p;
	
    fix_array()        {clr();return;};
    ~fix_array()       {return;};
	void clr() 			{for(int i=0;i<N;i++) ptr[i]=-1;}//do not touch!!!!!!!!!!
	void dirty() 		{clr();}
    T &operator [](int indx)
    {
		if(indx<0 || indx>=N)
		{
			printf("ERROR fixarray[int index] %d/%d \n",indx,N);
			int *n=0;		printf("%d \n",n[345]);
			//DebugBreak();
			return p;
		}
		return ptr[indx];
	}
	operator T *()
	{
		return (T *)ptr;
	}
	int size()
	{
		return N;
	}
};
template<typename T,int N>
class fix_array2
{
public:
	T ptr[N];
	T p;
	
    fix_array2()        {};
    ~fix_array2()       {};
    T &operator [](int indx)
    {
		if(indx<0 || indx>=N)
		{
			printf("ERROR fixarray[int index] %d/%d \n",indx,N);
			int *n=0;		printf("%d \n",n[345]);
			//DebugBreak();
			return p;
		}
		return ptr[indx];
	}
	operator T *()
	{
		return (T *)ptr;
	}
	int size()
	{
		return N;
	}
};

/*
int64_t hash64(const char *ss)
{
	int64_t hash=0x0;
	int n=0;
	
	while(*ss) 
	{
		hash^=(((int64_t)ss[0])<<n);
		ss++;
		n+=2;
//		if(n>24) n=0;//32-8
		if(n>56) n=0;//64-8
	}
	return hash;
}
inline int64_t qhash64(const char *ss)
{
	int64_t hash=0;

	//Jenkins' One-at-a-Time hash for strings should look something like this:
	while(ss[0])
	{
		hash += (unsigned char)ss[0];
    	hash += (hash << 10);
    	hash ^= (hash >> 6);
		ss++;
	}

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
	return hash;
}
*/
inline int64_t qhash64(const char *ss)
{
	int64_t hash=0;// 2x64 128/6 =21 char
	int64_t hash2=0;
	int cc=0;
	while(ss[0])
	{
		hash += (unsigned char)ss[0];
		hash<<=6;
		ss++;
		cc++;
		if(cc>10) break;
	}
	cc=0;
	while(ss[0])
	{
		hash2 += (unsigned char)ss[0];
		hash2<<=6;
		ss++;
		cc++;
		if(cc>10) break;
	}
	return hash^hash2;
}
inline int64_t qhash64tuti(const char *ss)
{
	int64_t hash=0;// 2x64 128/6 =21 char
	int64_t hash2=0;
	int cc=0;
	while(ss[0])
	{
		hash += (unsigned char)ss[0];
		hash<<=6;
		ss++;
		cc++;
		if(cc>10) break;
	}
	cc=0;
	while(ss[0])
	{
		hash2 += (unsigned char)ss[0];
		hash2<<=6;
		ss++;
		cc++;
		if(cc>10) break;
	}
	return hash^hash2;
}

inline int64_t qhash64(qstring ss)
{
	return qhash64((const char *)ss.c_str());
}
inline int64_t qhash64(char *ss)
{
	return qhash64((const char *)ss);
}
struct qchar
{
	char *ptr;
	int size;
	int capacity;

	qchar() {size=0;capacity=0;ptr=0;};
	qchar(int _capacity)	{size=0;ptr=0;resize(_capacity);}
	qchar(char *ss) 		{size=strlen(ss);resize(size);strcpy(ptr,ss);}
	qchar(const char *ss)	{size=strlen(ss);resize(size);strcpy(ptr,ss);}
	~qchar() {free();}

	void resize(int _capacity) {free();capacity=_capacity+2;ptr=new char[capacity];};
	void free() {clr();}
	void clr() {if(ptr) {delete [] ptr;ptr=0;size=0;capacity=0;};}
	char& operator [](int indx)
	{
		qassert(indx>=0 && indx<size);
		return ptr[indx];
	}
	qchar& operator =(const char *ss)
	{
		resize(strlen(ss));
		strcpy(ptr,ss);
		setlen();
		return *this;
	}
	qchar& operator +=(const char *ss)
	{
		qchar tmp(size+strlen(ss));// +2 ok
		strcpy(tmp.ptr,ptr);
		strcat(tmp.ptr,ss);
		tmp.setlen();
		swap(tmp);
		return *this;
	}
	void swap(qchar &tmp)
	{
		char *ptr2=ptr;
		int size2=size;
		
		ptr=tmp.ptr;
		size=tmp.size;
		
		tmp.ptr=ptr2;
		tmp.size=size2;
	}
	int len()
	{
		return size;
	}
	void setlen()
	{
		size=strlen(ptr);
	}
/*	operator char *()	{		return (char *)ptr;	}*/
	char *c_str()	
	{		
		qassert(ptr);
		return (char *)ptr;	
	}
};
struct s_node
{
	int ival;
	float fval;
	qchar sval;
	int type;

	s_node() {ival=0;fval=0;type=0;};//sval=0;
};

struct s_qscript
{
	char tmpstr[512];
	//map<string,s_node *> nodes;
	qmap<int64_t,s_node *> nodes;

	void defi(qstring name,int n)
	{
		s_node *tmp=new s_node;
		nodes[qhash64(name)]=tmp;
		tmp->ival=n;
		tmp->type=1;
	}
	void deff(qstring name,float n)
	{
		s_node *tmp=new s_node;
		nodes[qhash64(name)]=tmp;
		tmp->fval=n;
		tmp->type=2;
	}
	void defs(qstring name,qstring data)
	{
		s_node *tmp=new s_node;
		nodes[qhash64(name)]=tmp;
		tmp->sval.resize(strlen(data.c_str())+2);
		strcpy(tmp->sval.c_str(),data.c_str());
		tmp->type=3;
	}
	void defs(qstring name,int n,qstring data)
	{
		sprintf(tmpstr,"%s_%d",name.c_str(),n);
		defs(qstring(tmpstr),data);
	}
	void defs(qstring name,int n,int n2,qstring data)
	{
		sprintf(tmpstr,"%s_%d_%d",name.c_str(),n,n2);
		defs(qstring(tmpstr),data);
	}


	int geti(qstring name)
	{
		s_node *tmp=nodes[qhash64(name)];
		qassert(tmp->type==1);
		return tmp->ival;
	}
	float getf(qstring name)
	{
		s_node *tmp=nodes[qhash64(name)];
		qassert(tmp->type==2);
		return tmp->fval;
	}
	qchar& gets(qstring name)
	{
		s_node *tmp=nodes[qhash64(name)];
		qassert(tmp->type==3);
		return tmp->sval;
	}
	qchar& gets(qstring name,int n)
	{
		sprintf(tmpstr,"%s_%d",name.c_str(),n);
		return gets(qstring(tmpstr));
	}
	qchar& gets(qstring name,int n,int n2)
	{
		sprintf(tmpstr,"%s_%d_%d",name.c_str(),n,n2);
		return gets(qstring(tmpstr));
	}
};

/*
template<typename T,int N>
class fix_array
{
public:
	T   ptr[N];

	fix_array()       {s_clear<T>::clear(ptr,N);}// {for(int i=0;i<N;i++) ptr[i]=0;};
    ~fix_array()       {};
    T &operator [](int indx)
    {
		if(indx<0 || indx>=N)
		{
			printf("ERROR fixarray[index] \n");
			qassert((indx>=0 && indx<N));
			//DebugBreak();
			return ptr[0];
		}
		return ptr[indx];
	}
    T &operator [](char indx)
    {
		if(indx<0 || (int)indx>=N)
		{
			printf("ERROR fixarray[index] \n");
			qassert((indx>=0 && indx<N));
			//DebugBreak();
			return ptr[0];
		}
		return ptr[indx];
	}
	operator T *()
	{
		return (T *)ptr;
	}
	int size()
	{
		return N;
	}
};
*/

//#define zvector vector
#define zvector zzvector


template <typename T>
class zzvector : public std::vector<T>
{
//	T dummy;
public:
	
	T operator [](int i) const
	{
		if(i<0 || i>=this->size()) { printf("ERROR qvector::operator[] indx %d/%d\n",i,this->size());assert(false);return this->at(0);}//-1?
		return this->at(i);
	}
	T& operator [](int i)
	{
		if(i<0 || i>=this->size()) {printf("ERROR qvector::operator[] indx %d/%d\n",i,this->size());assert(false);return this->at(0);}//return dummy
		return this->at(i);
	}
};


struct qstate
{
	int state;
	int state_old;

	qstate() {state=0;state_old=-1;};
	int ischange()
	{
		if(state_old!=state)
		{
			state_old=state;
			return 1;
		}
		return 0;
	}
	bool operator ==(int val)
	{
		return (val==state);
	}
	int &operator =(int val)
	{
		state=val;
		return state;
	}
};


struct s_cmd
{
	const char *s[32];
	int i[32];
	float1 f[32];
	
	s_cmd *init(const char *s1,const char *s2)
	{
		s[0]=s1;
		s[1]=s2;
		s[2]=0;
		
		return this;
	}
};


class component //_base
{
public:	
	virtual void update(float1 xtime) 	{printf("WARN:  EMPTY UPDATE \n");		};
	virtual void recvcmd(s_cmd *tmp) 	{printf("WARN:  EMPTY RECV \n");		};

	qmap<int64_t,int> components_id;
	qvector<component*> components_list;//_base
	qvector<qstring> components_name;


	component()
	{
		components_list.push_back(this);
		components_name.push_back("empty");
		return;
	}
	void sendcmd(s_cmd *cmd)
	{
	QDUMPFNC();
		const char* name=cmd->s[1];

		loopd(i,1,components_list.size(),1) 
		{
			if(strstr(components_name[i].c_str(),name))	components_list[i]->recvcmd(cmd);
		}
	}
	template<typename T2>
	void addcomponent(const char *name)
	{
QDUMPFNC();	
		int id=components_list.size();
		
		T2 *comp=new T2((this));//_base  static_cast<component*>
		components_list.push_back(comp);
		components_name.push_back(qstring(name));
		components_id[qhash64(name)]=id;
	}
	template<typename T2>
	T2 *getcomponent(const char *name)
	{
		int64_t hash=qhash64(name);
		if(components_id.find(hash)==components_id.end()) printf("ERROR %s \n",name);
		int id=components_id[hash];
		return static_cast<T2*>(components_list[id]);//(T2*)
	}
	template<typename T2>
	T2 *qgetcomponent(const char *name,IDTYPE &id) //  LID
	{
		if(id==-1)
		{
			int64_t hash=qhash64(name);
			if(components_id.find(hash)==components_id.end()) printf("ERROR %s \n",name);
			id=components_id[hash];
			id=(IDTYPE)components_list[id];
		}
//		return static_cast<T2*>(components_list[id]);//(T2*)
		return (T2*)id;//(T2*)
	}
	
};

#define LID idtabl[__COUNTER__]


/*
sany comp sys --------------------------------------
problem: nincs nev

struct ComponentBase
{
public:
	virtual ~ComponentBase() {}
protected:
	static Family m_family_counter; //egy!
};

template <typename Derived>
struct Component : public ComponentBase
{
public:
	static Family family()
	{
		static Family family = ComponentBase::m_family_counter++; //a typename miatt mind masik static
		return family;
	}
};

class Entity : public util::handleAble, public util::EventSet
{
public:
	::wm::ecs::id_type getComponentID(Family family) const
	{
		if(family < (Family)m_componentID.size())
		{
			return m_componentID[family];
		} else return empty_component;
	}
};

ComponentBase* ComponentManager::getComponent(const Entity* p, Family family) const
{
	//wm_assert(p != 0);
	if(p == 0 || family >= (Family)m_familyDatas.size()) return 0;
	//Entity* p = e.lock().get();
	id_type cid = p->getComponentID(family);
	const FamilyData& d = m_familyDatas[family];
	wm_assert(d.m_components[cid] != 0);
	return d.m_components[cid];
}
ComponentBase* ComponentManager::addComponent(EntityHandle e, Family family, ComponentBase* c)
{
	FamilyData& d = m_familyDatas[family];
	wm_assert(d.m_handles.size() == d.m_components.size());
	{
		d.m_components.push_back(c);
		d.m_handles.push_back(e);
		e->setComponentID(family, id_type(d.m_components.size() - 1));
	}
	return c;
}
 --------------------------------------
*/

//nem jo,  bonyolult a definicio
class qstring2
{
public:
	const char *str;
	int id;
	
	qstring2(const char *ss) {id=-1;str=ss;return;}
	const char *c_str() {return str;};
};



#if 0


class component_base
{
public:	
	virtual void update(f32 xtime) 
	{
printf("WARN:  EMPTY UPDATE \n");	
	};
/*	virtual void draw(f32 xtime,int dat) 
	{
	};*/
	virtual void getchild(vector<component_base*> &tmp) 
	{
	};
	virtual void recvcmd(s_cmd *tmp) 
	{
	};
};

template<typename T>
class component:public component_base
{
public:
	map<int64_t,int> components_id;
	vector<component_base*> components_list;
	vector<string> components_name;
	component_base *root;


	component()
	{
		components_list.push_back(this);
		components_name.push_back("empty");
	}
	void getchild(vector<component_base*> *tmp) 
	{
		tmp=&components_list;
	};


	void update(f32 xtime) //vitrual
	{		
		((T*)this)->update(xtime,1);	//class member
	};
	void update_childs(f32 xtime)
	{
		loopd(i,1,components_list.size(),1) components_list[i]->update(xtime);
	}

/*
	void draw(f32 xtime,int dat) //vitrual
	{		
		((T*)this)->draw(xtime,dat,1);	//class member
	};
	void draw_childs(f32 xtime,int dat)
	{
		loopd(i,1,components_list.size(),1) components_list[i]->draw(xtime,dat);
	}*/
	void sendcmd(s_cmd *cmd)
	{
		char* name=cmd->s[1];

		loopd(i,1,components_list.size(),1) 
		{
			if(strstr(components_name[i].c_str(),name))	components_list[i]->recvcmd(cmd);
		}
	}
	
	template<typename T2>
	void addcomponent(char *name)
	{
		int id=components_list.size();
		
		T2 *comp=new T2(static_cast<component_base*>(this));
		components_list.push_back(comp);
		components_name.push_back(string(name));
		components_id[qhash64(name)]=id;
	}
	template<typename T2>
	T2 *getcomponent(char *name)
	{
		int64_t hash=qhash64(name);
		if(components_id.find(hash)==components_id.end()) printf("ERROR %s \n",name);
		int id=components_id[hash];
		return (T2*)components_list[id];
	}
	
};










typedef int family_t;
family_t m_family=0;

class component_base
{
public:	
//	static family_t m_family;
};

template<typename T>
class component:public component_base
{
public:
	fix_array<int,100> components_id;
	vector<component_base*> components_list;

	static family_t family()
	{
		static family_t _family=m_family++;//component_base::m_family
		return _family;
	}
	template<typename T2>
	void addcomponent()
	{
		int id=components_list.size();
		
		T2 *comp=new T2;
//		family_t fam=comp->family();
		family_t fam=component<T2>::family();
		components_list.push_back(comp);
		components_id[fam]=id;
//		printf("fam id %d %d \n",fam,id);
	}
	template<typename T2>
	T2 *getcomponent()
	{
		family_t fam=component<T2>::family();
		
		int id=components_id[fam];
		return (T2*)components_list[id];
	}
};
#endif

#endif

