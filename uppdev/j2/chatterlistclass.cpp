#include "ChatterListClass.h"
#include <math.h>

ChatterListClass::ChatterListClass() {
}

void ChatterListClass::clearall() {
     chatter.clear();
}

void ChatterListClass::printchatter() {
    MString k;
    k="ChatterinSepList: ";
    vector<chatterstruct>::iterator it;    
    for(it = chatter.begin(); it != chatter.end(); it++) {       
      chatterstruct wle;
      wle=*it;
      k=k+wle.Chatter;
      k=k+"(";
      if(wle.abgemeldet==1)k+="abgemeldet";else k+="angemeldet";
      k=k+") ";      
    }      
//  printf("%s\n",k.print());    
}


int ChatterListClass::ChatterAngemeldet() {
    int num=0;
    vector<chatterstruct>::iterator it;    
    for(it = chatter.begin(); it != chatter.end(); it++) {       
      chatterstruct wle;
      wle=*it;
      if (wle.abgemeldet==0) num++;
    }      
//    printf("%i Chatter angemeldet im Sep\n",num);    
    return(num);
}  
  
void ChatterListClass::anmelden(MString Chatter) {
    printf("|%s|\n",Chatter.print());     
    vector<chatterstruct>::iterator it;    
    for(it = chatter.begin(); it != chatter.end(); it++) {       
      chatterstruct wle;
      wle=*it;
      if (wle.Chatter==Chatter) {
         it->abgemeldet=0;  //fehler hier
         printf("%s jetzt angemeldet\n",Chatter.print());    
      }
    }      
}

void ChatterListClass::abmelden(MString Chatter) {
    printf("|%s|\n",Chatter.print());
    vector<chatterstruct>::iterator it;    
    for(it = chatter.begin(); it != chatter.end(); it++) {       
      chatterstruct wle;
      wle=*it;
      if (wle.Chatter==Chatter) {
         it->abgemeldet=1;
         printf("%s jetzt abgemeldet\n",Chatter.print());    
      }
    }      
}

bool ChatterListClass::AddChatter(MString Chatter) {
    vector<chatterstruct>::iterator it;    
    for(it = chatter.begin(); it != chatter.end(); it++) {       
      chatterstruct wle;
      wle=*it;
      if (wle.Chatter==Chatter) return(0);
    }      
    chatterstruct wle;    
    wle.Chatter = Chatter.print();
    wle.abgemeldet=0;
    chatter.push_back(wle);           
    printf("%s zur Chatterliste hinzugefügt\n",Chatter.print());    
    return(1);
}

bool ChatterListClass::RemoveChatter(MString Chatter) {
    vector<chatterstruct>::iterator it;    
    for(it = chatter.begin(); it != chatter.end(); it++) {       
      chatterstruct wle;
      wle=*it;
      if (wle.Chatter==Chatter) {
         chatter.erase(it);           
         printf("%s aus der Chatterliste gelöscht\n",Chatter.print());    
         return(1);
      }
    }      
    printf("%s nicht in der Chatterliste vorhanden, konnte nicht gelöscht werden\n",Chatter.print());    
    return(0);
}

/*
int ChatterListClass::AddUserZahlenraten(MString Chatter, float Antwort, float Zielwert) {
    if (Antwort == 0) return(0);    
    printf("auswerten zaoh\n");	
    vector<chatterstruct>::iterator it;    
    for(it = chatter.begin(); it != chatter.end(); it++) {       
      chatterstruct wle;
      wle=*it;
      if (wle.Chatter==Chatter) return(0);
    }      
    chatterstruct wle;    
    wle.Chatter = Chatter.print();
    wle.Pkt = Antwort;  
    chatter.push_back(wle);           
    printf("%f %f\n",Antwort,Zielwert);    
    if (Antwort==Zielwert) return(1);
    return(0);
}

*/
