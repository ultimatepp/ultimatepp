#include "mstring.h"

MString::MString ()
{
    stt.clear();                 
    found=0;                          
	bedpos = 0;
}

MString::~MString ()
{
}

void
MString::set (char *s)
{      
    if (s) {
    stt=s;  
    }
}

void
MString::erase (char *s)
{
    if (strlen(s)==0) return;            
    int z=0;
    do {
       z = stt.find(s,0);           
       if(z!= string::npos) stt.erase(z,strlen(s));           
    } while (z!= string::npos);
}

void 
MString::erase(int pos,int count) {
    stt.erase(pos,count);
}

void
MString::erase (char s)
{
    int z=0;
    do {
    z = stt.find(s,0);           
    if(z!= string::npos) stt.erase(z,1);           
    } while (z!= string::npos);
}


void
MString::concat (char *s)
{
    stt+=s;            
}

void
MString::preconcat (char *s)
{
    stt=s+stt;            
}

void
MString::concat (char s)
{
    stt+=s;     
}

MString
MString::operator+ (char s)
{
    MString x;               
    x.stt=stt+=s;   
    return(x);
                        
}

MString
MString::copy(int pos, int len) {
    char buffer[8000];                  
    MString x;
    int leng=stt.copy(buffer,pos,len);
    buffer[leng]='\0';
    x=buffer;
    return(x);
}


MString
MString::operator+ (char *S) {
    MString x;               
    x.stt=stt+=S;   
    return(x);                                   
}

void
MString::operator=(int x) {  
    MString u;
    char yIntString[32];
    sprintf (yIntString, "%d", x);				
    stt=yIntString;    
}  
  
MString 
MString::operator+= (int x) {
    MString u;
    char yIntString[32];
    sprintf (yIntString, "%d", x);				
    u=stt+=yIntString;
    return(u);    
}

MString
MString::operator+= (char *S) {
    MString x;
    x.stt=stt+=S;      
    return(x);
}

MString
MString::operator+= (const char *S) {
    MString x;
    x.stt=stt+=S;      
    return(x);
}

void

MString::insert (int x, char *s)
{
    stt.insert(x-1,s);            
}

void
MString::replace (char *s, char *y)
{
    if (s==y) return;                  
    int z=0;
    do {
    z = stt.find(s,z);           
    if(z!= string::npos) 
    {
           stt.erase(z,strlen(s));           
           stt.insert(z,y);           
           z+=strlen(y);
    }
    } while (z!= string::npos);
}

bool
MString::equal (char *x)
{
    if (stt==x) return (1); else return(0);
}

bool
MString::includes (char *s)
{
    if (stt.find(s,0)!=string::npos) return(1); else return(0);              
}
bool 
MString::beginswith(const char *s) {
   string::size_type loc = stt.find(s, 0 );   
   if(( loc != string::npos )&&(loc==0)) return(1); else return(0);
}

bool
MString::includes (const char *s)
{
    if (stt.find(s,0)!=string::npos) return(1); else return(0);              
}


MString
MString::getbedfirst (char *a, char *e)
{                   
	MString rstring;
    int start,end;                 
    start=stt.find(a,0);
    if (start!=string::npos) {
       start=start+strlen(a);
       end=stt.find(e,start);                             
       if (end!=string::npos) {       
       rstring.stt=stt.substr(start,end-start);                       
       bedpos=end-1;
       rstring.found=1;
       return(rstring);                              
       }
    }                     
    return(rstring);      
}


MString
MString::getbednext (char *a, char *e)
{
	MString rstring;
    int start,end;                 
    start=stt.find(a,bedpos);
    if (start!=string::npos) {
       start=start+strlen(a);
       end=stt.find(e,start);                             
       if (end!=string::npos) {       
       rstring.stt=stt.substr(start,end-start);                       
       bedpos=end-1;
       rstring.found=1;       
       return(rstring);                              
       }
    }               
    bedpos=0;
    return(rstring);      
}

void
MString::keineumlaute ()
{
	replace ("ä", "ae");
	replace ("ö", "oe");
	replace ("ü", "ue");
	replace ("Ä", "Ae");
	replace ("Ö", "Oe");
	replace ("Ü", "Ue");
	replace ("ß", "ss");
	replace ("á", "a");
	replace ("é", "e");	
	replace ("ú", "u");		
	replace ("ó", "o");	
    replace ("í", "i");		
	replace ("À", "A");	
	replace ("Ó", "O");	
	replace ("Ù", "U");	
	replace ("È", "E");	  	
	replace ("Í", "i");	  	
}

MString
MString::klein ()
{
    MString x;               
    x.stt=stt;
    for (int i=0;i<x.stt.length();i++) x.stt[i]=tolower(x.stt[i]);               
    return(x);    
}

MString
MString::gross ()
{
    MString x;               
    x.stt=stt;
    for (int i=0;i<x.stt.length();i++) x.stt[i]=toupper(x.stt[i]);               
    return(x);    
}


