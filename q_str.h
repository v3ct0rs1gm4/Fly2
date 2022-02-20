
#ifndef _QSTRH
#define _QSTRH



#include "q_engine.h"


extern int str_p,str_h;
extern char *str_buf,word2[512];



void init_str();
void search_str(const char *ss);
bool search_str2(const char *ss);
bool search_str3(const char *ss,const char last);
bool search_str_chk(const char *ss);
bool search_str_chk2(const char *ss);
void search_enter();
int search_star();
int read_command();
void search_chr(const char a);
int read_int();
float read_dd();
char *read_ss();
char *read_str();
char read_act();
bool end_ase();
int read_word();

#endif

