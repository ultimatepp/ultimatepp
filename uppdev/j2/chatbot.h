#ifndef CHATBOT_H
#define CHATBOT_H
using namespace std;
#include <windows.h>
#include <winsock.h>
#include <vector>        
#include "chatterlistclass.h"
//#include <sdl/sdl_image.h>
using namespace Upp;

#include <stdio.h>
#include "MString.h"
#include <time.h>
#include <CtrlLib/CtrlLib.h>

#define NETWORK_ERROR -1
#define NETWORK_OK     0

class ChatBotClass
{
    private:    
                      
      typedef struct {
        unsigned int schl;
        MString zeichen;
        float width;
      } schlusselstruct;

      typedef struct {
        MString nick;
        MString message;
        time_t timestamp;
      } MStruct;


        int                  Nicknum,
                             Sepnum;
        MString              Nicks[100]; //depr
        MString              SepNick[100];     //depr   
      
        unsigned int c2[5];                
        MString checkcode;        
        static int const            MessageSplitLength=255;
        MString ID;
        MString Sid;
        MString PW;
        MString KEY;
        SOCKET sendSocket,sendHTMLSocket;      
        const char *UserName, *Passwort, *Channel;
        MString floodpref1,floodpref2; 
        void ReportError(int errorCode, const char *whichFunc);
        time_t seconds1;        
        void Write2Log(char *name,char *text);        
        void readsperrliste();
        vector<MString> sperrnicks;     
        vector<MStruct> messagelog;                     
        vector<MStruct> sendlog;
        bool onsperrliste(MString nick);                          
        void addtomessagelog(MString sender, MString message);
        void addtosperrlist(MString nick);
        bool resendcheck;
	public:                 
        schlusselstruct schlussel[40];                            
        time_t               timeoflastrec() {return(seconds1);}                  
        void                 updatetimeoflastrec() {seconds1=time(NULL);};
        bool isonline() {if (errorcode) return(0); else return(1);}
        bool                 isinsep;
        int                  peopleinsep;
        MString              sepname;
        MString              aktivechannel;        
        bool                 leseeigenenschrieb;        
        bool                 rawtext;
        bool                 beepmyname;
        bool                 autokicknicksonsperrlist;
        bool                 autoaddtosperrlistonspaming;
        int                  errorcode;
        MString              ErrorMsg;
        bool                 anonymize;
        ChatterListClass ChatterSepList;
		
		ChatBotClass(){
		    autokicknicksonsperrlist=0;                  
    autoaddtosperrlistonspaming=0;
    anonymize=0;                  
    leseeigenenschrieb=1;
    beepmyname=1;
    rawtext=0;                  
    floodpref1="";
    floodpref2="";
    seconds1=time(NULL);    
    readsperrliste();
    readschlussel();
    resendcheck=1;};
		~ChatBotClass(){};		
        void webpage();
        void botdata();
		int login(const char *name, char *pwd, char *chan);
		int login();		
        int GetIDandSID();
        int GetChatOut();
        int say(char *text);
        void readchatline(void (*pfi)(int fl, MString col, MString Sender, MString Message));
        void readchatline2(void (*pfi)(int fl, String colG, String SenderG, String MessageG));
        void readHTMLout(void (*pfi)(MString Text));        
        void Exit();
        int GetIDCard(MString m,MString *Statement);
        int getregistrationform();
        int registername(char *nickname, char *pwd, char *email);
        int LadeDatei(char *datei,char *speichername);
//        unsigned int CalcHash(SDL_Surface *surface, int xoff);
        void readschlussel();
        void registernickname(char *nickname, char *pwd, char *email);    
        void addtosendlog (MString message);
        void delfromsendlog (MString message);
        void checksendlog();
 };

#endif // CHATBOT_H

