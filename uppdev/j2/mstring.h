#ifndef MString_H
#define MString_H
#include "stdio.h"
#include "ctype.h"
#include <iostream>
#include <string>
using namespace std;

//erase(x) : löscht alle vorkommen von x aus dem Wort
//concat(x) : fügt dem wort x hinten an
//replace(x,y) : ersetzt alle x im wort durch y
//equal(x) : liefert eins wenn das wort mit x übereinstimmt, null sonst
//includes(x) : liefert eins wenn x im wort vorkommt

class MString {

private:
  int bedpos;
  char str[8000];       
  string stt;  
public:
  bool found;
public:
	MString();
	~MString();
  void set(char *s);
  void operator=(char *s){set(s);}
  void operator=(MString M){stt=M.stt;found=M.found;bedpos=M.bedpos;}
  void operator=(int x);
  void operator=(string s){stt=s;}  
  char *print(){
       if (stt.length()>=8000) printf("STRINGERROR>8000");
       for (int i=0;((i<=stt.length())&&(i<8000));i++) str[i]=stt[i];       
       return(str);
       }
  string getstring() {string x;x=stt;return(x);}       
  char stringval(int i) {if (i<stt.length()) return(stt[i]); else return('\0');};
  void erase(char *s);
  void erase(int pos,int count);
  void erase(char s);
  void concat(char s);
  void concat(char *s);
  void preconcat(char *s);  
  void concat(MString S){concat(S.print());}
  void preconcat(MString S){preconcat(S.print());}  
  int strlength() {return(stt.length());}  //oh wie schön ist Panama
  MString copy(int pos, int len); //
  MString operator+(char s);
  MString operator+(char *S);
  MString operator+=(int x);  
  MString operator+=(char *S);  
  MString operator+=(const char *S);    
  MString operator+(MString S){return(operator+(S.print()));}
  MString operator+=(MString S){return(operator+=(S.print()));}  
  void replace(char *x, char *y);
  void insert(int x, char* s);   //
  bool equal(char *x);
  bool equal(MString x){if (stt==x.stt) return(1); else return(0);}
  bool operator==(char *x) {return(equal(x));}
  bool operator==(const char *x) {if (stt==x) return(1); else return(0);}  
  bool operator!=(const char *x) {if (stt==x) return(0); else return(1);}  
  bool operator==(MString x) {return(equal(x));}
  bool operator!=(MString x) {return(!equal(x));}
  bool operator!=(char *x) {return(!equal(x));}
  bool includes(const char *s); 
  bool beginswith(const char *s);  
  bool includes(char *s);
  bool includes(MString x){return(includes(x.print()));}
  MString getbedfirst(char *a,char *e);
  MString getbednext(char *a,char *e);
  void keineumlaute(); //
  MString klein(); //
  MString gross(); //  
};

#endif
