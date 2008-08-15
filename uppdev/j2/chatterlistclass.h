#ifndef ChatterListClass_H
#define ChatterListClass_H
#include "mstring.h"
#include <vector>   
using namespace std;

  typedef struct {
	MString Chatter;
	int abgemeldet;
  } chatterstruct;

class ChatterListClass
{
private:
        
public:
  vector<chatterstruct> chatter;     
  ChatterListClass(); 
  void printchatter();
  void anmelden(MString name);  
  void abmelden(MString name);    
  bool AddChatter(MString name);
  bool RemoveChatter(MString name);  
  void clearall();
  int ChatterInSep(){return(chatter.size());}
  int ChatterAngemeldet();
};

#endif 
