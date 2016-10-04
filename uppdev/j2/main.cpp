#include "j2.h"

#include "IniReader.h"
#include "MString.h"
#include "ChatBot.h"

j2 HG;
String qtfx;
ChatBotClass ChatBot1;
void (*pfi) (int fl, String colG, String SenderG, String MessageG);
char *inibotName;
char *inibotPass;
char *inibotInitialChannel;


j2::j2() {
	qtfx = "Joachim [@4 Green text]{@40}.1&";	
	CtrlLayout(*this, "Joachim 0.1");
	SetLanguage( LNG_('D','E','D','E') );
	SetDefaultCharset(CHARSET_UTF8);
    e.SetQTF(qtfx);
    mbu <<= THISBACK(Quit);
    a <<  THISBACK(DragEnter);  	
    a <<  THISBACK(GotFocus);  	
}

void j2::Quit() {
	qtfx<<"test&";
	e.SetQTF(qtfx);
	ChatBot1.login(inibotName,inibotPass,inibotInitialChannel);
	ChatBot1.readchatline2(pfi);
}

void j2::GotFocus() {	
		e.SetQTF(qtfx);	
		e.ScrollEnd();	
}
void j2::DragEnter() {		
	
	String x;
	char *t;
	t = new char[2000];
	x=~a;
	if (strlen(x)<1) return;
	if (x[1]==char(10)) {a.Clear();return;}
	if (x[(int)strlen(x)-1]==char(10)) { 
		int i=0;
		do {			
			t[i]=x[i];		
			i++;
		} while (x[i]!=char(10));	
		ChatBot1.say(t);
	//	qtfx<< ~a<<"&";
	//	e.SetQTF(qtfx);
		e.SetQTF(qtfx);	
		e.ScrollEnd();
		a.Clear();
	}

}

void readinifile() {
 CIniReader iniReader(".\\joachim.ini"); 
 inibotName = iniReader.ReadString("GeneralSetting", "BotName", "PowerMops"); 
 inibotPass = iniReader.ReadString("GeneralSetting", "BotPass", "mdykoz"); 
 inibotInitialChannel = iniReader.ReadString("GeneralSetting", "InitialChannel", "Halle1");   
}
int size;

void textin_cb(int x,String farbe,String Sender,String text) {
	size++;	
	if (farbe=="") farbe="000000";
	
		int col[6];
		for (int i=0;i<6;i++) {
			if (farbe[i]=='0') col[i]=0;			
			if (farbe[i]=='1') col[i]=1;			
			if (farbe[i]=='2') col[i]=2;			
			if (farbe[i]=='3') col[i]=3;			
			if (farbe[i]=='4') col[i]=4;			
			if (farbe[i]=='5') col[i]=5;			
			if (farbe[i]=='6') col[i]=6;			
			if (farbe[i]=='7') col[i]=7;			
			if (farbe[i]=='8') col[i]=8;			
			if (farbe[i]=='9') col[i]=9;						
			if (farbe[i]=='a') col[i]=10;						
			if (farbe[i]=='b') col[i]=11;			
			if (farbe[i]=='c') col[i]=12;			
			if (farbe[i]=='d') col[i]=13;			
			if (farbe[i]=='e') col[i]=14;			
			if (farbe[i]=='f') col[i]=15;
			if (farbe[i]=='A') col[i]=10;						
			if (farbe[i]=='B') col[i]=11;			
			if (farbe[i]=='C') col[i]=12;			
			if (farbe[i]=='D') col[i]=13;			
			if (farbe[i]=='E') col[i]=14;			
			if (farbe[i]=='F') col[i]=15;			
		}
		int num[3];
		for (int i=0;i<3;i++) {
			num[i]=col[i*2]*16+col[1+i*2];
		}
		qtfx<<"[@("<<num[0]<<"."<<num[1]<<"."<<num[2]<<") ";
			
	qtfx<<"\1"<<Sender<<":"<<text<<"\1]&";
	MString v;
	v=qtfx;
	printf("%s\n",v.print());
	HG.e.SetQTF(qtfx);	
	HG.e.ScrollEnd();
	printf("xo\n");
}

void *readchat (void *arg){
    neuverbinden:
	do {
       ChatBot1.errorcode=0;        
	   ChatBot1.login(inibotName,inibotPass,inibotInitialChannel);        
	   Sleep(1000);
       if (ChatBot1.errorcode) textin_cb(0,"FF0000","System",ChatBot1.ErrorMsg.print());         
       if (ChatBot1.ErrorMsg.includes("Chat verwiesen worden")) {
          textin_cb(0,"FF0000","System","Chatverweis, neuer Verbindungsversuch in 60 Sekunden");             
          Sleep(60000);          
       }                                     
        if (ChatBot1.ErrorMsg.includes("Zerolengthreturn")) {
          textin_cb(0,"FF0000","System","Nulllängenantwort vom Sever, verbinde neu in 10 Sekunden");             
          Sleep(10000);
       }                                    
       if ((ChatBot1.ErrorMsg.includes("Call to connect()"))&&(ChatBot1.errorcode!=10061)) {
          textin_cb(0,"FF0000","System","Neuer Verbindungsversuch in 100 Sekunden");             
          Sleep(100000);
       }
       if ((ChatBot1.ErrorMsg.includes("Call to connect()"))&&(ChatBot1.errorcode==10061)) {
          textin_cb(0,"FF0000","System","Host ist Down, Neuer Verbindungsversuch in 10 Minuten");             
          Sleep(600000);
       }
    } while (ChatBot1.errorcode);
    textin_cb(0,"FF0000","System","Verbunden zu ChatCity");                 
    int g=1;
	while (g != 0)
	{   
		//printf("%i hier\n",ChatBot1.errorcode);       
        Sleep(100);       
		ChatBot1.readchatline2(pfi); 
		//printf("%i da\n",ChatBot1.errorcode);    
		if (ChatBot1.errorcode) goto neuverbinden;                                  		
	}
}

void ParseChatLine (int flag, String col, String Sender, String Message)
{     
	textin_cb(flag,col,Sender,Message);
}

GUI_APP_MAIN
{
	readinifile();
	int ids = 1;	
	Thread t;
 	pfi = &ParseChatLine;
 	t.Start(
 	
	pthread_create (&threads, NULL, readchat, &ids);
	HG.Run();
}
