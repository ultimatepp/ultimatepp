#include <CtrlLib/CtrlLib.h>

#include "windows.h"
#include "ChatBot.h" 
//#include <ftplib.h>
#include <iostream>
#include "IniReader.h"

using namespace Upp;

void ChatBotClass::checksendlog() {
     vector<MStruct>::iterator it;    
     time_t timenow=time(NULL);
     for(it = sendlog.begin(); it != sendlog.end(); it++) { 
            MStruct msg;
            msg=*it;
            if ((timenow-msg.timestamp)>3) {
                printf("resend: (%s)(%i)\n",msg.message.print(),msg.timestamp);                
                resendcheck=0;
                MString k;
                k=msg.message.print();
                k+="(R)";
                say(k.print());
                resendcheck=1;                
                sendlog.erase(it);
                return;
            } 
     }
}

void ChatBotClass::addtosendlog (MString message) {
     if (message.beginswith("/")) return;
     MStruct msg;
     msg.nick="";
     msg.message=message;
     msg.timestamp=time(NULL);
     sendlog.push_back(msg); 
//     printf("addtosend (%s)(%i)\n",message.print(),msg.timestamp);
}  
void ChatBotClass::delfromsendlog (MString message) {
//     printf("delfr:(%s)\n",message.print());
     vector<MStruct>::iterator it;    
     int counter=0;
     message.erase(" ");
     message.erase("_");     
     message.erase("*");          
//   printf("chx:[(%s)]\n",message.print());                     
     for(it = sendlog.begin(); it != sendlog.end(); it++) { 
            MStruct msg;
            msg=*it;
            MString k;
            k=msg.message.print();
     k.erase("&rlm;");  
     k.erase("&lrm;"); 
     k.erase("&rArr;"); 
//     k.erase("&uArr;"); 
//     k.erase("&dArr;"); 
     k.replace("&quot;","\"");
     k.replace("&szlig;","ß");      
     k.replace("&lt;","<");
     k.replace("&gt;",">");            

     k.replace("é","e");                                      
     k.replace("á","a");                                           
     k.replace("í","i");                                                
     k.replace("ó","o");                                                
     k.replace("ú","u");                                                     
     k.replace("&auml;","ä");
     k.replace("&uuml;","ü");
     k.replace("&ouml;","ö");
     k.replace("&Auml;","ä");  //wg. umlaute werden mit lowercase ncht in klein buchstaben verwandelt
     k.replace("&Uuml;","ü");
     k.replace("&Ouml;","ö");

     k.erase("+");               
     k.erase(" ");
     k.erase("_");     
     k.erase("*");          
//              printf("chs:[(%s)]\n",k.print());                
            if (k==message) {
//              printf("delfromsendlog: (%s)(%i)\n",message.print(),msg.timestamp);                     
                sendlog.erase(it);
                return;
            } 
     }
}  

void ChatBotClass::addtomessagelog(MString sender, MString message) {
     MStruct msg;
     msg.nick=sender;
     msg.message=message;
     msg.timestamp=time(NULL);
     messagelog.push_back(msg); 
     
    MString zielnick;
    MString zielstring;
    int zc=1;int sc=1;
    int firsttimestamp;int lasttimestamp;int lasttimestamp2;
    vector<MStruct>::iterator it;    
    int counter=0;
    for(it = messagelog.begin(); it != messagelog.end(); it++) { 
      MStruct o;
      o=*it;
      if (counter==0) {zielnick=o.nick;zielstring=o.message;firsttimestamp=o.timestamp;}
      if (counter!=0) if (o.nick==zielnick) {zc++;lasttimestamp=o.timestamp;}
      if (counter!=0) if (o.nick==zielnick&&o.message==zielstring) {sc++;lasttimestamp2=o.timestamp;}
//      printf("%s %i\n",o.nick.print(),o.timestamp);
      counter++;
    }
    if (zc>3) if ((lasttimestamp-firsttimestamp)<2) {addtosperrlist(zielnick);messagelog.clear();}
    if (sc>3) if ((lasttimestamp2-firsttimestamp)<7) {addtosperrlist(zielnick);messagelog.clear();}
    if (messagelog.size()>4) messagelog.erase(messagelog.begin());                   
}
        
bool ChatBotClass::onsperrliste(MString nick) {
  vector<MString>::iterator it;    
    for(it = sperrnicks.begin(); it != sperrnicks.end(); it++) { 
      MString o;
      o=*it;
//      printf("%s %s\n",nick.print(),o.print());
      if (o==nick) return(1);
    }        
    return(0);
}

void ChatBotClass::addtosperrlist(MString nick) {
  if (nick!="Wenlok") {
  FILE *fd;
  if (NULL == (fd = fopen (".\\sperrliste.ini", "a+"))) printf("error: can't open file sperrliste.ini\n");
  fprintf (fd, "%s\n",nick.print());
  fclose(fd);
  sperrnicks.push_back(nick);     
  cout<<nick.print()<<" addet to spamlist\n"<<endl;
  if (autokicknicksonsperrlist) {MString k;k="/sepkick ";k+=nick;say(k.print());}
  }
}

void ChatBotClass::readsperrliste() {
 printf("Sperrliste Einlesen... ");
 FILE *fd;
 if (NULL == (fd = fopen (".\\sperrliste.ini", "r"))) {printf("error: can't open file sperrliste.ini\n");return;}
  MString s11;	    
  char s1[20];
  int cvar=0;
  while (fscanf (fd, "%s\n",&s1)!=EOF) {
  cvar++;        
  s11=s1;
//  printf("%s\n",s11.print());
  sperrnicks.push_back(s11);     
 }           
  fclose(fd);                 
  printf("%i Einträge gefunden\n",cvar);              
  vector<MString>::iterator it;    
    for(it = sperrnicks.begin(); it != sperrnicks.end(); it++) { 
      MString o;
      o=*it;
//      cout<<o.print()<<endl;
    }        
}

void ChatBotClass::readschlussel() {
 CIniReader iniReader(".\\schlussel.ini"); 
 schlussel[0].schl = atol(iniReader.ReadString("Schlussel", "A", "")); 
 schlussel[0].zeichen="A";
 schlussel[0].width=41;
 schlussel[1].schl = atol(iniReader.ReadString("Schlussel", "B", "")); 
 schlussel[1].zeichen="B";
 schlussel[1].width=41; 
 schlussel[2].schl = atol(iniReader.ReadString("Schlussel", "C", "")); 
 schlussel[2].zeichen="C";  
 schlussel[2].width=42; 
 schlussel[3].schl = atol(iniReader.ReadString("Schlussel", "D", "")); 
 schlussel[3].zeichen="D";  
 schlussel[3].width=45; 
 schlussel[4].schl = atol(iniReader.ReadString("Schlussel", "E", "")); 
 schlussel[4].zeichen="E";  
 schlussel[4].width=37.032;  
 schlussel[5].schl = atol(iniReader.ReadString("Schlussel", "F", "")); 
 schlussel[5].zeichen="F";  
 schlussel[5].width=32,8;  
 schlussel[6].schl = atol(iniReader.ReadString("Schlussel", "G", "")); 
 schlussel[6].zeichen="G";  
 schlussel[6].width=47;  
 schlussel[7].schl = atol(iniReader.ReadString("Schlussel", "H", "")); 
 schlussel[7].zeichen="H";  
 schlussel[7].width=41;  
 schlussel[8].schl = atol(iniReader.ReadString("Schlussel", "I", "")); 
 schlussel[8].zeichen="I";  
 schlussel[8].width=22,62;  
 schlussel[9].schl = atol(iniReader.ReadString("Schlussel", "J", "")); 
 schlussel[9].zeichen="J";  
 schlussel[9].width=27;  
 schlussel[10].schl = atol(iniReader.ReadString("Schlussel", "K", "")); 
 schlussel[10].zeichen="K";  
 schlussel[10].width=41;  
 schlussel[11].schl = atol(iniReader.ReadString("Schlussel", "L", "")); 
 schlussel[11].zeichen="L";  
 schlussel[11].width=33;   
 schlussel[12].schl = atol(iniReader.ReadString("Schlussel", "M", "")); 
 schlussel[12].zeichen="M";  
 schlussel[12].width=51;   
 schlussel[13].schl = atol(iniReader.ReadString("Schlussel", "N", "")); 
 schlussel[13].zeichen="N";  
 schlussel[13].width=41;   
 schlussel[14].schl = atol(iniReader.ReadString("Schlussel", "O", "")); 
 schlussel[14].zeichen="O";  
 schlussel[14].width=45;   
 schlussel[15].schl = atol(iniReader.ReadString("Schlussel", "P", "")); 
 schlussel[15].zeichen="P";  
 schlussel[15].width=36;   
 schlussel[16].schl = atol(iniReader.ReadString("Schlussel", "Q", "")); 
 schlussel[16].zeichen="Q";  
 schlussel[16].width=47;   
 schlussel[17].schl = atol(iniReader.ReadString("Schlussel", "R", "")); 
 schlussel[17].zeichen="R";  
 schlussel[17].width=41;   
 schlussel[18].schl = atol(iniReader.ReadString("Schlussel", "S", "")); 
 schlussel[18].zeichen="S";  
 schlussel[18].width=41;   
 schlussel[19].schl = atol(iniReader.ReadString("Schlussel", "T", "")); 
 schlussel[19].zeichen="T";  
 schlussel[19].width=37,032;   
 schlussel[20].schl = atol(iniReader.ReadString("Schlussel", "U", "")); 
 schlussel[20].zeichen="U";  
 schlussel[20].width=41;   
 schlussel[21].schl = atol(iniReader.ReadString("Schlussel", "V", "")); 
 schlussel[21].zeichen="V";  
 schlussel[21].width=41;   
 schlussel[22].schl = atol(iniReader.ReadString("Schlussel", "W", "")); 
 schlussel[22].zeichen="W";  
 schlussel[22].width=59.088;   
 schlussel[23].schl = atol(iniReader.ReadString("Schlussel", "X", "")); 
 schlussel[23].zeichen="X";  
 schlussel[23].width=41;   
 schlussel[24].schl = atol(iniReader.ReadString("Schlussel", "Y", "")); 
 schlussel[24].zeichen="Y";  
 schlussel[24].width=37,51;   
 schlussel[25].schl = atol(iniReader.ReadString("Schlussel", "Z", "")); 
 schlussel[25].zeichen="Z";  
 schlussel[25].width=41;   
 schlussel[26].schl = atol(iniReader.ReadString("Schlussel", "1", "")); 
 schlussel[26].zeichen="1";  
 schlussel[26].width=32.458;   
 schlussel[27].schl = atol(iniReader.ReadString("Schlussel", "2", "")); 
 schlussel[27].zeichen="2";  
 schlussel[27].width=32.458;   
 schlussel[28].schl = atol(iniReader.ReadString("Schlussel", "3", "")); 
 schlussel[28].zeichen="3";
 schlussel[28].width=32.458;      
 schlussel[29].schl = atol(iniReader.ReadString("Schlussel", "4", "")); 
 schlussel[29].zeichen="4";  
 schlussel[29].width=32.458;    
 schlussel[30].schl = atol(iniReader.ReadString("Schlussel", "5", "")); 
 schlussel[30].zeichen="5";  
 schlussel[30].width=32.458;    
 schlussel[31].schl = atol(iniReader.ReadString("Schlussel", "6", "")); 
 schlussel[31].zeichen="6";  
 schlussel[31].width=32.458;    
 schlussel[32].schl = atol(iniReader.ReadString("Schlussel", "7", "")); 
 schlussel[32].zeichen="7";  
 schlussel[32].width=32.458;    
 schlussel[33].schl = atol(iniReader.ReadString("Schlussel", "8", "")); 
 schlussel[33].zeichen="8";  
 schlussel[33].width=32.458;    
 schlussel[34].schl = atol(iniReader.ReadString("Schlussel", "9", "")); 
 schlussel[34].zeichen="9";  
 schlussel[34].width=32.458;    
 schlussel[35].schl = atol(iniReader.ReadString("Schlussel", "0", "")); 
 schlussel[35].zeichen="0";  
 schlussel[35].width=32.458;    
 schlussel[36].schl = atol(iniReader.ReadString("Schlussel", ":", "")); 
 schlussel[36].zeichen=":";  
 schlussel[36].width=24.296;    
 

}
/*
unsigned int ChatBotClass::CalcHash(SDL_Surface *surface, int xoff)
{	unsigned int ret = 0;
	unsigned char r,b,g;
	for (int y = 1; y < 18; y++)
	{	for (int x = 0; x < 16; x++)
		{	r = *((unsigned char*)surface->pixels + (xoff+x)*3 + y*surface->pitch);
			g = *((unsigned char*)surface->pixels + (xoff+x)*3 + y*surface->pitch + 1);
			b = *((unsigned char*)surface->pixels + (xoff+x)*3 + y*surface->pitch + 2);
			bool first = ret&0x40000000;
			ret<<=1;
			ret |= first;
			ret^=(r < 128 && g < 128 && b < 128)?1:0;
			if (r < 128 && g < 128 && b < 128) printf("x"); else printf(".");			
		}
		printf("\n");
	}
	return ret;
}
*/
int ChatBotClass::LadeDatei(char *datei,char *speichername) {
     printf("Lade Datei: %s\n",datei);

   SOCKET passSocket;                 
   WORD sockVersion;
   WSADATA wsaData;
   int nret;   
   sockVersion = MAKEWORD(1, 1);
   WSAStartup(sockVersion, &wsaData);
   LPHOSTENT hostEntry;
//   hostEntry = gethostbyname("chatcity.de");     // Specifying the server by its name;
   if (!anonymize) hostEntry = gethostbyname("213.203.200.84");
   else hostEntry = gethostbyname("http://www.waythru.com/index.php?q=213.203.200.84");
   
                                                    // another option is gethostbyaddr()                                                    
   if (!hostEntry) {
      nret = WSAGetLastError();
      ReportError(nret, "gethostbyname()");         // Report the error as before      
      WSACleanup();
      return NETWORK_ERROR;
   }
   passSocket = socket(AF_INET,                      // Go over TCP/IP
                      SOCK_STREAM,                  // This is a stream-oriented socket
                      IPPROTO_TCP);                 // Use TCP rather than UDP                      
   if (passSocket == INVALID_SOCKET) {
      nret = WSAGetLastError();
      ReportError(nret, "socket()");      
      WSACleanup();
      return NETWORK_ERROR;
   }   
   
   SOCKADDR_IN serverInfo;   
   serverInfo.sin_family = AF_INET;
   serverInfo.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);   // See the explanation in the tutorial
   serverInfo.sin_port = htons(80);                 // Change to network-byte order and
   nret = connect(passSocket,
                  (LPSOCKADDR)&serverInfo,
                  sizeof(struct sockaddr));                  
   if (nret == SOCKET_ERROR) {
      nret = WSAGetLastError();
      ReportError(nret, "connect()");      
      WSACleanup();
      return NETWORK_ERROR;
   }
   // Send/receive, then cleanup:  
   
  char s[2000];     
  strcpy(s,"GET /securitycode/index.php?0.42204000+1194905347 HTTP/1.1\r\n");
  strcat(s,"User-Agent: Opera/9.10 (Windows NT 5.1; U; en)\r\n");
  strcat(s,"Host: chats.kunden.abacho.net\r\n");
  strcat(s,"Accept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\n");
  strcat(s,"Accept-Language: de-DE,de;q=0.9,en;q=0.8\r\n");
  strcat(s,"Accept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\n");
  strcat(s,"Accept-Encoding: deflate, gzip, x-gzip, identity, *;q=0\r\n");
  strcat(s,"Referer: http://home.chatcity.de/registrierung/\r\n");
  strcat(s,"Connection: Keep-Alive, TE\r\n");
  strcat(s,"TE: deflate, gzip, chunked, identity, trailers\r\n");            
  strcat(s,"\r\n");              
  int nReadAmount=send(passSocket,s,strlen(s),0);
  
  
    FILE *fd2;
  fd2 = fopen (speichername, "w");

  MString x;
  char pBuffer[1024];

  int y;
/*  y=recv(passSocket,pBuffer,1024,0) {
  fprintf (fd2,pBuffer);
  printf("[%i][]\n",y);                                
  Sleep(100);  
//     x+=pBuffer;
}
*/
/*
  int v=1;
  while(v!=15) {
     for (int i=0;i<6048;i++) pBuffer[i]=0;
     recv(passSocket,pBuffer,1024,0);   
     x+=pBuffer;
     v++;
     printf("%s\n",x.print());
     printf("%i, %i\n",v,x.strlength());
  }
  */
/*
  FILE *fd2;
  fd2 = fopen (speichername, "w");
  fprintf (fd2,x.print());
  */
  fclose(fd2);	
  
     closesocket(passSocket);
}
  /*
ChatBotClass::ChatBotClass()
{
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
    resendcheck=1;
}
*/

void ChatBotClass::Write2Log(char *name,char *text) {
  time_t rawtime;
  time ( &rawtime );
 
  FILE *fd2;
  fd2 = fopen (name, "a+");
//  fprintf (fd2,ctime (&rawtime));
  fprintf (fd2,text);
  fprintf (fd2,"\n");
  fclose(fd2);	
}

/*
ChatBotClass::~ChatBotClass()
{
}
*/

void ChatBotClass::webpage() {
  if (errorcode) return;
  char *name;
  FILE *fd2;
  MString p;      
  name=(char*)malloc(255);    
  sprintf(name,"channel.htm");
  if (NULL == (fd2 = fopen (name, "w")))
	{
		printf ("Can't open it ejecting!\n");
	}
	else
	{
		printf ("file erzeugt\n");
	}
  
  	fprintf (fd2, "<HTML>\n %i\n",Nicknum);
	fprintf (fd2, "<TABLE WIDTH=600>");
	printf("ok\n");
    for (int i=0;i<Nicknum;i++) {	
	 	if (i % 2 == 0)
			fprintf (fd2, "<TR BGCOLOR=\"#FFE6FF\">");
		else
			fprintf (fd2, "<TR>");
		    fprintf (fd2,"<TD>name: %s<BR></TD><TD><IMG SRC=""http://www.chatcity.de/cgi-bin/cc1/eximg/wwb.cgi?NAME=%s""></TD></TR>\n",Nicks[i].print(),Nicks[i].print());
	}
	fprintf (fd2, "</TABLE>");	
	fprintf (fd2, "</HTML>\n");	
    fclose (fd2);
    
    printf ("lade hoch\n");
/*
	netbuf *nControl;
	FtpInit ();
	FtpConnect ("web7.webmailer.de", &nControl);
	FtpLogin ("www.ph2.net", "abulafia", nControl);
//	FtpChdir("Fragen", nControl);		
	FtpPut (name, name, FTPLIB_ASCII, nControl);
	FtpQuit (nControl);
    printf ("hochgeladen\n");	
  */  
}


int ChatBotClass::login(const char *name, char *pwd, char *chan) {
    Write2Log("connectionlog.txt","Login\n");
    isinsep=0;
    peopleinsep=0;
    seconds1=time(NULL);    
    aktivechannel=chan;
    UserName=name;
    Passwort=pwd;
    GetIDandSID();
    GetChatOut();
}

int ChatBotClass::login() {
    peopleinsep=0;
    isinsep=0;
    GetIDandSID();
    GetChatOut();
}

void ChatBotClass::Exit() {
   WSACleanup();
}

void ChatBotClass::ReportError(int errorCodex, const char *whichFunc) {
   Write2Log("connectionlog.txt","Error:\n");     
   sendlog.clear();
   printf("sendlog cleared\n");   
   char errorMsg[92];                              
   ZeroMemory(errorMsg, 92);                        // automatically NULL-terminate the string
   sprintf(errorMsg, "SOCKETERROR:  Call to %s returned error %d!", (char *)whichFunc, errorCodex);
   Write2Log("connectionlog.txt",errorMsg);           
   ErrorMsg=errorMsg;
   errorcode=errorCodex;
}
    
void ChatBotClass::readHTMLout(void (*pfi)(MString Text)) { 
  if (errorcode) return;     
  char pBuffer[6048];
  MString Sender[40];
  MString message[40];
  MString col[40];
  int flag[40];
  MString packet;
  for (int i=0;i<6048;i++) pBuffer[i]=0;
  int len=recv(sendHTMLSocket,pBuffer,6048,0);
  for (int i=0;((pBuffer!=""));i++) packet.concat(pBuffer[i]);
}   

struct list_entry {
    int sock;
    struct list_entry *next;
};

struct list_type {
    struct list_entry *data;
    unsigned count;
};


void ChatBotClass::readchatline2(void (*pfi)(int fl, String col, String Sender, String Message)) {
printf("inreadchat\n");

printf("inreadchat\n");
  if (errorcode) return;
  printf("inreadchat\n");
  char pBuffer[8000];
  String Sender[40];
  String message[40];
  String col[40];
  int flag[40];
  MString packet;
  
  for (int i=0;i<40;i++) {
  col[i]="000000";
  }
  
  for (int i=0;i<8000;i++) pBuffer[i]=0;

  unsigned long a[1];
  a[1]=1;
  int e = ioctlsocket(sendSocket, FIONBIO, a);
  if (e == -1) {
    errorcode=1;         
    printf("ioctlsocket failed\n");
  }

int len = recv(sendSocket,pBuffer,8000,0);
if (len<0) {checksendlog();return;}
  if (len==0) {
     errorcode=1; 
     printf("len=0\n");     
     ReportError(999, "Zerolengthreturn");
  }

  for (int i=0;((pBuffer[i]!=(int)0));i++) packet.concat(pBuffer[i]);
  printf("RAW[%s]\n",packet.print());
  int mc=0;    

if (packet.includes("Du bist eine gewisse Zeit des Chats verwiesen worden!</span></B>")) {
      errorcode=1;ReportError(1000, "Du bist eine gewisse Zeit des Chats verwiesen worden");
  }

  packet.erase("<img src=\"http://213.203.200.54/cc1/images/cc_icon_female.gif\" width=\"15\" height=\"15\" border='0' title=\"ID-Card des Chatters\"></a>");  
  packet.erase("<img src=\"http://213.203.200.54/cc1/images/cc_icon_nosex.gif\" width=\"15\" height=\"15\" border='0' title=\"ID-Card des Chatters\"></a>");
  packet.erase("<img src=\"http://213.203.200.54/cc1/images/cc_icon_male.gif\" width=\"15\" height=\"15\" border='0' title=\"ID-Card des Chatters\"></a>");
  packet.erase("<img src=\"http://213.203.200.54/cc1/images/cc_icon_female.gif\" width=\"15\" height=\"15\" border=\"0\" title=\"ID-Card des Chatters\"></a>");  
  packet.erase("<img src=\"http://213.203.200.54/cc1/images/cc_icon_nosex.gif\" width=\"15\" height=\"15\" border=\"0\" title=\"ID-Card des Chatters\"></a>");
  packet.erase("<img src=\"http://213.203.200.54/cc1/images/cc_icon_male.gif\" width=\"15\" height=\"15\" border=\"0\" title=\"ID-Card des Chatters\"></a>");  
  packet.erase("<A HREF=\"#\" OnClick=\"oid('");
  packet.erase("'); return false;\">");
  
  packet.preconcat("<br>");
  if (rawtext) { col[0]="000000";flag[0]=0;message[0]=packet.print();Sender[0]="RAW"; mc+=1;}

	if ((!packet.includes("***newpage***"))) {
		packet.replace("é","e");                                      
     	packet.replace("á","a");                                           
     	packet.replace("í","i");                                                
		packet.replace("ó","o");                                                
     	packet.replace("ú","u");                                                     
	    packet.replace("&auml;","ä");
	    packet.replace("&uuml;","ü");
	    packet.replace("&ouml;","ö");
	    packet.replace("&Auml;","ä");  //wg. umlaute werden mit lowercase ncht in klein buchstaben verwandelt
	    packet.replace("&Uuml;","ü");
	    packet.replace("&Ouml;","ö");
	    packet.erase("&#0160;");                
	    packet.replace("<FONT SIZE=-1><BR>","<FONT SIZE=-1>");
	    packet.replace("<BR>\n<LI>","<LI>");	 
	    for (int i=0;i<40;i++) flag[i]=0;
     	int x=1;
     	MString ppacket;
		do {
        	x++; 
	        ppacket=packet.getbedfirst("<br>","<br>");
	        if (ppacket.found) goto ok;
	        ppacket=packet.getbedfirst("<BR>","<br>");
	        if (ppacket.found) goto ok;
	        ppacket=packet.getbedfirst("<br>","<BR>");
	        if (ppacket.found) goto ok;
	        ppacket=packet.getbedfirst("<BR>","<BR>");
	        if (ppacket.found) goto ok;
	        goto ok2;
	        ok:
	        
	        packet.erase(0,4);   
	        packet.erase(ppacket.print());                
	        ppacket.concat("<br>");
	        //Normaler Chatschrieb 	  
	        printf("%s\n",ppacket.print());
	        
	        if (ppacket.includes("<!--91-->")) { 
	           MString k;	           
	           k=ppacket.getbedfirst("<FONT COLOR=\"#","\"><B>");
			   col[mc]=k.print();
	           k=ppacket.getbedfirst("<B><!--91-->","</B></FONT>:");    
	           Sender[mc]=k.print();
	           k=ppacket.getbedfirst("</B></FONT>: ","<br>");                                    
	           message[mc]=k.print();
	           mc++;
	        }                         

	        //Du flüsterst zu ...
	        if (ppacket.includes("<!--94-->")) {
	           MString k;
	           col[mc]="000000";
	           k=ppacket.getbedfirst("<!--94-->","</I>:");                                                 
	           Sender[mc]=k.print();	           
	           k="Du fluesterst zu ";
	           k+=Sender[mc];
	           k+=":";      
	           k+=ppacket.getbedfirst("</I>: ","<br>");                                 
	           message[mc]=k.print();
	           Sender[mc]="---";
	           flag[mc]=4;      
	           mc++;          
	        }	        
	        //... flüstert ...
	        if (ppacket.includes("<!--93-->")) {
	           MString k;
	           col[mc]="000000";
	           k=ppacket.getbedfirst("<!--93-->","</B></font>");
	           Sender[mc]=k.print();
	           k=ppacket.getbedfirst("</u></I></FONT> ","<br>");                                                    
	           message[mc]=k.print();
	           flag[mc]=3;                                                     
	           mc++;           
	        }

	        //me
	        if (ppacket.includes("<!--95-->")) {
	           MString k;
	           col[mc]="000000";
	           k=ppacket.getbedfirst("<!--95-->","</FONT><br>");        
	           message[mc]=k.print();
	           Sender[mc]="...";
	           flag[mc]=4;      
	           mc++;        
	        }

	        printf("xx1\n");

	        //Bob Nachrichten, gemischtes
	        if (ppacket.includes("<!--0-->")) {
	            MString k;
	           k=ppacket.getbedfirst("<!--0-->MrBob: ","<br>");    


	           if (k.includes("Du gehst in das Separee")) {	               
	              sepname=k.getbedfirst("Du gehst in das Separee ","<SCRIPT");
	              isinsep=1;
	              ChatterSepList.clearall();
	           }

	           if (k.includes("Du gehst in den Channel")) aktivechannel=k.getbedfirst("Du gehst in den Channel ","<SCRIPT");
	                               
	           if (k.includes("verlässt uns")) {
	                if (isinsep) {
	                             peopleinsep--;
	                         MString k;k=message[mc];k.erase(" verlässt uns");                                           
	                         ChatterSepList.RemoveChatter(k);                                                                                                   
	                }
	           }        
	           if (k.includes("verlässt das Separee")) 
	                         if (autokicknicksonsperrlist) {
	                         MString k;k=message[mc];k.erase(" verlässt das Separee");
	                         if  (onsperrliste(k)) {message[mc]="Nick on Sperrlist left";}                                                       
	                         ChatterSepList.RemoveChatter(k);                         
	                         peopleinsep--;
	                         } else {
	                                MString k;k=message[mc];k.erase(" verlässt das Separee");                                
	                                ChatterSepList.RemoveChatter(k);                                                         
	                                peopleinsep--;                                
	                         }
	          if (k.includes("meldet sich kurz ab")) {
	                         MString k;k=message[mc];k.erase(" meldet sich kurz ab</B>");                                           
	                         k.erase("<B>");
	                         if (isinsep) ChatterSepList.abmelden(k);                                                                      
	          }
	          if (k.includes("ist wieder da.")) {
	                         MString k;k=message[mc];k.erase(" ist wieder da.</B>");                                           
	                         k.erase("<B>");
	                         if (isinsep) ChatterSepList.anmelden(k);                                                                                   
	          }                                        	
	          if (k.includes("ist im Augenblick abgemeldet")) {
	             MString k;k=message[mc];k.erase(" ist im Augenblick abgemeldet");
	             if (isinsep) ChatterSepList.abmelden(k);                                                                      
	          } 
	           if (k.includes("betritt das Separee")) {              
	              if (autokicknicksonsperrlist) {
	                      MString k;k=message[mc];k.erase(" betritt das Separee");
	                      if  (onsperrliste(k)) {MString x;x="/sepkick ";x+=k;say(x.print());message[mc]="Nick on Sperrlist joined";}
	                      peopleinsep++;
	                      ChatterSepList.AddChatter(k);
	              } else {
	                     MString k;k=message[mc];k.erase(" betritt das Separee");                     
	                     ChatterSepList.AddChatter(k);
	                     peopleinsep++;
	              }
	           }
	           if (k.includes("Schwupp und raus aus dem Separee")) {
	              isinsep=0;
	              peopleinsep=0;
	              ChatterSepList.clearall();
	           }
	           k.erase("<SCRIPT LANGUAGE=JavaScript>setbgcol('#FFFFFF','#000000');</SCRIPT>");
	           k.erase("<SCRIPT LANGUAGE=JavaScript>setbgcol('#ffffff','#000000');</SCRIPT>");
	           k.erase("<SCRIPT LANGUAGE=JavaScript>setbgcol('#008000','#ffffff');</SCRIPT>");
	           k.erase("<!--72--><b><font color=\"#D24B4F\">");
	           k.erase("</font><script>fenster=window.open('http://home.chatcity.de/pages/gast.php',\"Gaeste\",'width=600,height=350');fenster.focus();</script>");
	           
	           message[mc]=k.print();
	           Sender[mc]="Wenlok";                                             
	           flag[mc]=1;                 
	           mc++;
	           
	        }      
	        printf("xx1\n");    
	        //Userlist anzeigen
	        
	        if (ppacket.includes("<!--92-->")) {	           
	           MString k;
	           k=ppacket.getbedfirst("<!--92-->","</B></FONT>");    
	           k+=":\n";
	           k+=ppacket.getbedfirst("<FONT SIZE=-1>","</FONT></B><br>");
	           k.replace("<FONT COLOR=\"#800000\">","<B>");
	           k.replace("</FONT>","</B>");                                         
	           k.replace("<LI>","\n");                                         
	//           message[mc].replace("offen...","offen...\n");                                                    
	           Sender[mc]="Wenlok";                                             
	           Nicks[0]=k.getbedfirst("[","]");
	           Nicknum=1;                                      
	           do { 
	              Nicks[Nicknum]=k.getbednext("[","]");
	              Nicknum++;                                          
	           } while ((Nicks[Nicknum-1]!="")&&Nicknum<200);           
	           message[mc]=k.print();
	           Nicknum--;
	           mc++;                 	                
	           //webpage();	        
	        }          
	        
		} while (x<50);
		printf("xx2\n");		
		ok2:  	
		printf("HI");		

  for (int o=0;o<mc;o++) {

      seconds1=time(NULL);
      MString logp;
      logp=Sender[o];
      logp+=" ";
      logp+=message[o];
      Write2Log("ChatBotClasslog.txt",logp.print());      

/*        
      message[o].replace("<b>","_");
      message[o].replace("</b>","_");
      message[o].replace("<B>","_");
      message[o].replace("</B>","_");
      message[o].replace("<I>","*");
      message[o].replace("</I>","*");        
      message[o].replace("&quot;","\"");
      message[o].replace("&szlig;","ß");      
      message[o].replace("&lt;","<");
      message[o].replace("&gt;",">");            
      message[o].erase("&rArr;");
      message[o].erase("&lArr;");
      message[o].erase("&lrm;");
      message[o].erase("&rlm;");           
      
      
      if (message[o].beginswith("!who")) {
         ChatterSepList.printchatter();
      }      
      if (message[o].beginswith("!botdata")) {
         MString p;
         p="conf:: ";
         p+="isinsep:";
         if (isinsep) {p+="1";p+=" sepname:";p+=sepname;} else p+="0";
         if (isinsep) {p+=" peopleinsep:";p+=peopleinsep;}
         p+=" channel:";
         p+=aktivechannel;
         p+=" isonline:";
         if (isonline()) p+="1"; else p+="0";
         p+=" name:";
         p+=UserName;
         p+=" autoaddtosperrlistonspaming:"; 
         p+=autoaddtosperrlistonspaming;     
         p+=" autokicknicksonsperrlist:";
         p+=autokicknicksonsperrlist;
         say(p.print());
      }
*/
      if (message[o]=="!autoaddtosperrlistonspaming") autoaddtosperrlistonspaming=!autoaddtosperrlistonspaming;
      if (message[o]=="!autokicknicksonsperrlist") autokicknicksonsperrlist=!autokicknicksonsperrlist;
/*      
      if (message[o].includes("automatisch") and Sender[o].includes("Wenlok") ){
        say("/alive");
      } else {         
             
        if (beepmyname) if (
             (message[o].includes(UserName))&&
            ((!Sender[o].includes("Wenlok"))&&(!Sender[o].includes("RAW")))
            ) Beep(500,100);                         
                    
        //if ((Sender[o]==UserName)) delfromsendlog(message[o]);
  */
  
        
//        if (!onsperrliste(Sender[o])) {
//           if (autoaddtosperrlistonspaming) if ((Sender[o]!=UserName)) addtomessagelog(Sender[o],message[o]);        
//           printf("GOT[%i|%s|%s|%s]\n",flag[o],col[o].print(),Sender[o].print(),message[o].print());        
           if (!leseeigenenschrieb) { if ((Sender[o]!=UserName)) pfi(flag[o],col[o],Sender[o],message[o]);}
           else pfi(flag[o],col[o],Sender[o],message[o]);         
//		  }
    
    	//pfi(flag[o],col[o],Sender[o],message[o]);         
         
       
      }
    
   }
  // checksendlog();
		

}

void ChatBotClass::readchatline(void (*pfi)(int fl, MString col, MString Sender, MString Message)) {
printf("inreadchat\n");

printf("inreadchat\n");
  if (errorcode) return;
  printf("inreadchat\n");
  char pBuffer[8000];
  MString Sender[40];
  MString message[40];
  MString col[40];
  int flag[40];
  MString packet;
    
  for (int i=0;i<8000;i++) pBuffer[i]=0;

  unsigned long a[1];
  a[1]=1;
  int e = ioctlsocket(sendSocket, FIONBIO, a);
  if (e == -1) {
    errorcode=1;         
    printf("ioctlsocket failed\n");
  }

int len = recv(sendSocket,pBuffer,8000,0);
if (len<0) {checksendlog();return;}
  if (len==0) {
     errorcode=1; 
     printf("len=0\n");     
     ReportError(999, "Zerolengthreturn");
  }

  for (int i=0;((pBuffer[i]!=(int)0));i++) packet.concat(pBuffer[i]);
  printf("RAW[%s]\n",packet.print());
  int mc=0;    

  if (packet.includes("Du bist eine gewisse Zeit des Chats verwiesen worden!</span></B>")) {
      errorcode=1;ReportError(1000, "Du bist eine gewisse Zeit des Chats verwiesen worden");
  }

  packet.erase("<img src=\"http://213.203.200.54/cc1/images/cc_icon_female.gif\" width=\"15\" height=\"15\" border='0' title=\"ID-Card des Chatters\"></a>");  
  packet.erase("<img src=\"http://213.203.200.54/cc1/images/cc_icon_nosex.gif\" width=\"15\" height=\"15\" border='0' title=\"ID-Card des Chatters\"></a>");
  packet.erase("<img src=\"http://213.203.200.54/cc1/images/cc_icon_male.gif\" width=\"15\" height=\"15\" border='0' title=\"ID-Card des Chatters\"></a>");
  packet.erase("<img src=\"http://213.203.200.54/cc1/images/cc_icon_female.gif\" width=\"15\" height=\"15\" border=\"0\" title=\"ID-Card des Chatters\"></a>");  
  packet.erase("<img src=\"http://213.203.200.54/cc1/images/cc_icon_nosex.gif\" width=\"15\" height=\"15\" border=\"0\" title=\"ID-Card des Chatters\"></a>");
  packet.erase("<img src=\"http://213.203.200.54/cc1/images/cc_icon_male.gif\" width=\"15\" height=\"15\" border=\"0\" title=\"ID-Card des Chatters\"></a>");  
  packet.erase("<A HREF=\"#\" OnClick=\"oid('");
  packet.erase("'); return false;\">");
  
//  for (int i=0;i<len;i++) printf("%c",packet.str[i]);
  packet.preconcat("<br>");
  if (rawtext) { col[0]="000000";flag[0]=0;message[0]=packet;Sender[0]="RAW"; mc+=1;  
  }

  if ((!packet.includes("***newpage***"))) {
     packet.replace("é","e");                                      
     packet.replace("á","a");                                           
     packet.replace("í","i");                                                
     packet.replace("ó","o");                                                
     packet.replace("ú","u");                                                     
     packet.replace("&auml;","ä");
     packet.replace("&uuml;","ü");
     packet.replace("&ouml;","ö");
     packet.replace("&Auml;","ä");  //wg. umlaute werden mit lowercase ncht in klein buchstaben verwandelt
     packet.replace("&Uuml;","ü");
     packet.replace("&Ouml;","ö");
     packet.erase("&#0160;");                
     packet.replace("<FONT SIZE=-1><BR>","<FONT SIZE=-1>");
     packet.replace("<BR>\n<LI>","<LI>");
 
     for (int i=0;i<40;i++) flag[i]=0;


     int x=1;
     MString ppacket;
  
     /*
     do {
        x++; 
        ppacket=packet.getbedfirst("<br>","<br>");
        if (ppacket.found) goto ok;
        ppacket=packet.getbedfirst("<BR>","<br>");
        if (ppacket.found) goto ok;
        ppacket=packet.getbedfirst("<br>","<BR>");
        if (ppacket.found) goto ok;
        ppacket=packet.getbedfirst("<BR>","<BR>");
        if (ppacket.found) goto ok;
        goto ok2;
        ok:
        packet.erase(0,4);   
        packet.erase(ppacket.print());                
        ppacket.concat("<br>");
        //Normaler Chatschrieb 
        
        if (ppacket.includes("<!--91-->")) { 
//           printf("habs %s\n",packet.print());
           col[mc]=ppacket.getbedfirst("<FONT COLOR=\"#","\"><B>");
           Sender[mc]=ppacket.getbedfirst("<B><!--91-->","</B></FONT>:");    
           message[mc]=ppacket.getbedfirst("</B></FONT>: ","<br>");                                    
           mc++;
        }                         
        //Du flüsterst zu ...
        if (ppacket.includes("<!--94-->")) {
           col[mc]="000000";
           Sender[mc]=ppacket.getbedfirst("<!--94-->","</I>:");                                                 
           message[mc]="Du flüsterst zu ";
           message[mc]+=Sender[mc].print();
           message[mc]+=":";      
           message[mc]+=ppacket.getbedfirst("</I>: ","<br>");                                 
           Sender[mc]="---";
           flag[mc]=4;      
           mc++;          
        }
        //... flüstert ...
        if (ppacket.includes("<!--93-->")) {
           Sender[mc]=ppacket.getbedfirst("<!--93-->","</B></font>");
           message[mc]=ppacket.getbedfirst("</u></I></FONT> ","<br>");                                                    
           flag[mc]=3;                                                     
           mc++;           
        }
        //me
        if (ppacket.includes("<!--95-->")) {
           message[mc]=ppacket.getbedfirst("<!--95-->","</FONT><br>");        
           Sender[mc]="...";
           flag[mc]=4;      
           mc++;        
        }
        //Bob Nachrichten, gemischtes
        if (ppacket.includes("<!--0-->")) {
           message[mc]=ppacket.getbedfirst("<!--0-->MrBob: ","<br>");    
           if (message[mc].includes("Du gehst in das Separee")) {
              sepname=message[mc].getbedfirst("Du gehst in das Separee ","<SCRIPT");
              isinsep=1;
              ChatterSepList.clearall();
           }
           if (message[mc].includes("Du gehst in den Channel")) aktivechannel=message[mc].getbedfirst("Du gehst in den Channel ","<SCRIPT");
                               
           if (message[mc].includes("verlässt uns")) {
                if (isinsep) {
                             peopleinsep--;
                         MString k;k=message[mc];k.erase(" verlässt uns");                                           
                         ChatterSepList.RemoveChatter(k);                                                                                                   
                }
           }        
           if (message[mc].includes("verlässt das Separee")) 
                         if (autokicknicksonsperrlist) {
                         MString k;k=message[mc];k.erase(" verlässt das Separee");
                         if  (onsperrliste(k)) {message[mc]="Nick on Sperrlist left";}                                                       
                         ChatterSepList.RemoveChatter(k);                         
                         peopleinsep--;
                         } else {
                                MString k;k=message[mc];k.erase(" verlässt das Separee");                                
                                ChatterSepList.RemoveChatter(k);                                                         
                                peopleinsep--;                                
                         }
          if (message[mc].includes("meldet sich kurz ab")) {
                         MString k;k=message[mc];k.erase(" meldet sich kurz ab</B>");                                           
                         k.erase("<B>");
                         if (isinsep) ChatterSepList.abmelden(k);                                                                      
          }
          if (message[mc].includes("ist wieder da.")) {
                         MString k;k=message[mc];k.erase(" ist wieder da.</B>");                                           
                         k.erase("<B>");
                         if (isinsep) ChatterSepList.anmelden(k);                                                                                   
          }                                        

          if (message[mc].includes("ist im Augenblick abgemeldet")) {
             MString k;k=message[mc];k.erase(" ist im Augenblick abgemeldet");
             if (isinsep) ChatterSepList.abmelden(k);                                                                      
          } 
           if (message[mc].includes("betritt das Separee")) {              
              if (autokicknicksonsperrlist) {
                      MString k;k=message[mc];k.erase(" betritt das Separee");
                      if  (onsperrliste(k)) {MString x;x="/sepkick ";x+=k;say(x.print());message[mc]="Nick on Sperrlist joined";}
                      peopleinsep++;
                      ChatterSepList.AddChatter(k);
              } else {
                     MString k;k=message[mc];k.erase(" betritt das Separee");                     
                     ChatterSepList.AddChatter(k);
                     peopleinsep++;
              }
           }
           if (message[mc].includes("Schwupp und raus aus dem Separee")) {
              isinsep=0;
              peopleinsep=0;
              ChatterSepList.clearall();
           }
           message[mc].erase("<SCRIPT LANGUAGE=JavaScript>setbgcol('#FFFFFF','#000000');</SCRIPT>");
           message[mc].erase("<SCRIPT LANGUAGE=JavaScript>setbgcol('#ffffff','#000000');</SCRIPT>");
           message[mc].erase("<SCRIPT LANGUAGE=JavaScript>setbgcol('#008000','#ffffff');</SCRIPT>");
           message[mc].erase("<!--72--><b><font color=\"#D24B4F\">");
           message[mc].erase("</font><script>fenster=window.open('http://home.chatcity.de/pages/gast.php',\"Gaeste\",'width=600,height=350');fenster.focus();</script>");
           Sender[mc]="Wenlok";                                             
           flag[mc]=1;                 
           mc++;
        }          
        //Userlist anzeigen
        if (ppacket.includes("<!--92-->")) {
           message[mc]=ppacket.getbedfirst("<!--92-->","</B></FONT>");    
           message[mc]+=":\n";
           message[mc]+=ppacket.getbedfirst("<FONT SIZE=-1>","</FONT></B><br>");
           message[mc].replace("<FONT COLOR=\"#800000\">","<B>");
           message[mc].replace("</FONT>","</B>");                                         
           message[mc].replace("<LI>","\n");                                         
//           message[mc].replace("offen...","offen...\n");                                                    
           Sender[mc]="Wenlok";                                             
           Nicks[0]=message[mc].getbedfirst("[","]");
           Nicknum=1;                                      
           do { 
              Nicks[Nicknum]=message[mc].getbednext("[","]");
              Nicknum++;                                          
           } while ((Nicks[Nicknum-1]!="")&&Nicknum<200);           
           Nicknum--;
           mc++;                      
           webpage();
        }          
     } while (x<50);
     
     if (x>20) Write2Log("debuglog.txt","x>20 in function readchatline\n");
     */
     ok2:
  for (int o=0;o<mc;o++) {
      seconds1=time(NULL);
      MString logp;
      logp=Sender[o];
      logp+=" ";
      logp+=message[o];
      Write2Log("ChatBotClasslog.txt",logp.print());      
        
      message[o].replace("<b>","_");
      message[o].replace("</b>","_");
      message[o].replace("<B>","_");
      message[o].replace("</B>","_");
      message[o].replace("<I>","*");
      message[o].replace("</I>","*");        
      message[o].replace("&quot;","\"");
      message[o].replace("&szlig;","ß");      
      message[o].replace("&lt;","<");
      message[o].replace("&gt;",">");            
      message[o].erase("&rArr;");
      message[o].erase("&lArr;");
      message[o].erase("&lrm;");
      message[o].erase("&rlm;");           
      
      
      if (message[o].beginswith("!who")) {
         ChatterSepList.printchatter();
      }      
      if (message[o].beginswith("!botdata")) {
         MString p;
         p="conf:: ";
         p+="isinsep:";
         if (isinsep) {p+="1";p+=" sepname:";p+=sepname;} else p+="0";
         if (isinsep) {p+=" peopleinsep:";p+=peopleinsep;}
         p+=" channel:";
         p+=aktivechannel;
         p+=" isonline:";
         if (isonline()) p+="1"; else p+="0";
         p+=" name:";
         p+=UserName;
         p+=" autoaddtosperrlistonspaming:"; 
         p+=autoaddtosperrlistonspaming;     
         p+=" autokicknicksonsperrlist:";
         p+=autokicknicksonsperrlist;
         say(p.print());
      }

      if (message[o]=="!autoaddtosperrlistonspaming") autoaddtosperrlistonspaming=!autoaddtosperrlistonspaming;
      if (message[o]=="!autokicknicksonsperrlist") autokicknicksonsperrlist=!autokicknicksonsperrlist;
      
      if (message[o].includes("automatisch") && Sender[o].includes("Wenlok") ){
        say("/alive");
      } else {         
             
        if (beepmyname) if (
             (message[o].includes(UserName))&&
            ((!Sender[o].includes("Wenlok"))&&(!Sender[o].includes("RAW")))
            ) Beep(500,100);                         
                    
        //if ((Sender[o]==UserName)) delfromsendlog(message[o]);
  /*      
        if (!onsperrliste(Sender[o])) {
           if (autoaddtosperrlistonspaming) if ((Sender[o]!=UserName)) addtomessagelog(Sender[o],message[o]);        
           printf("GOT[%i|%s|%s|%s]\n",flag[o],col[o].print(),Sender[o].print(),message[o].print());        
           if (!leseeigenenschrieb) { if ((Sender[o]!=UserName)) pfi(flag[o],col[o],Sender[o],message[o]);}
           else pfi(flag[o],col[o],Sender[o],message[o]);         
        }
        */
        }  
        
      }
    
   }
  // checksendlog();

}




int ChatBotClass::GetChatOut() {
   if(errorcode) return(1);
   WORD sockVersion;
   WSADATA wsaData;
   int nret;
   sockVersion = MAKEWORD(1, 1);
   WSAStartup(sockVersion, &wsaData);
   LPHOSTENT hostEntry;
   printf("gettin chatout\n");
   if (!anonymize) hostEntry = gethostbyname("213.203.200.72");
   else hostEntry = gethostbyname("http://www.waythru.com/index.php?q=http://213.203.200.72");
printf("endgetcahtout\n");
   if (!hostEntry) {
      nret = WSAGetLastError();
      ReportError(nret, "gethostbyname()");         // Report the error as before
      printf("ERROR\n");
      WSACleanup();
      return NETWORK_ERROR;
   }   
   sendSocket = socket(AF_INET,                      // Go over TCP/IP
                      SOCK_STREAM,                  // This is a stream-oriented socket
                      IPPROTO_TCP);                 // Use TCP rather than UDP
                      
   if (sendSocket == INVALID_SOCKET) {
      nret = WSAGetLastError();
      ReportError(nret, "socket()");      
      WSACleanup();   
      return NETWORK_ERROR;
   }
printf("endgetcahtout\n");   
   SOCKADDR_IN serverInfo;   
   serverInfo.sin_family = AF_INET;
   serverInfo.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);   // See the explanation in the tutorial
   serverInfo.sin_port = htons(80);                 // Change to network-byte order and
   nret = connect(sendSocket,
                  (LPSOCKADDR)&serverInfo,
                  sizeof(struct sockaddr));
               
   if (nret == SOCKET_ERROR) {
      nret = WSAGetLastError();
      ReportError(nret, "connect()");
      WSACleanup();
      return NETWORK_ERROR;
   }
   
  char s[3000];     
  strcpy(s,"GET /chatout?MODE=1048576&CHANNEL=");
  strcat(s,aktivechannel.print());  
  strcat(s,"&PROVIDER=REG&NICKNAME=");
  strcat(s,UserName);  
  strcat(s,"&PW=");    
  strcat(s,PW.print());
  strcat(s,"&SID=");
  strcat(s,Sid.print());  
  strcat(s,"&ID=");
  strcat(s,ID.print());  
  strcat(s,"&KEY=");
  strcat(s,"258");    
  strcat(s,"&SBG=1 HTTP/1.1\n");
  strcat(s,"User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; en) Opera 8.51\n");
  strcat(s,"Host: 213.203.200.72\n");
  strcat(s,"Accept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\n");
  strcat(s,"Accept-Language: en\n");
  strcat(s,"Accept-Charset: windows-1252, utf-8, utf-16, iso-8859-1;q=0.6, *;q=0.1\n");
  strcat(s,"Accept-Encoding: deflate, gzip, x-gzip, identity, *;q=0\n");
  strcat(s,"Referer: http://www.chatcity.de/cgi-bin/cc2005/wwb/wwb.cgi\n");
  strcat(s,"Connection: Keep-Alive, TE\n");
  strcat(s,"TE: deflate, gzip, chunked, identity, trailers\n\n\n");
  int nReadAmount=send(sendSocket,s,strlen(s),0);
  printf("endgetcahtout\n");
}

int ChatBotClass::say(char *textin) {
    if (textin!="") {
    //Write2Log("ChatBotClasslog.txt",textin);
//   printf("in say\n");
   if (errorcode) return(1);    
   //split long messages
   MString msgs[20];
   MString buf;
   buf=textin;

   int counter=0;       
   for (int i=0;((i<1+(int)(buf.strlength()/MessageSplitLength))&&(counter<20));i++) {
//       printf("%i\n",i);
       msgs[i]=buf.copy(MessageSplitLength,i*MessageSplitLength);
//       printf("%s\n",msgs[i].print());       
       counter++;
   }
//   printf("ok\n");
   for (int u=0;u<counter;u++) {
   MString st;
   st=msgs[u].print();
   
   if ((st==floodpref1)&&(st==floodpref2)&&(!floodpref2.includes("/alive"))) {
      printf("flooding: %s\n",st.print());
      floodpref2=floodpref1;
      floodpref1=st;
      st+=" ";
      //goto fertig;
   } else {
      floodpref2=floodpref1;
      floodpref1=st;
   }

   printf("SEND[%s]\n",st.print());   
   if (resendcheck) addtosendlog(st);   
   st.replace("%","%25");	     
   st.replace(" ","+");
   st.replace("?","%3F");
   st.replace("&","%26");
   st.replace(":","%3A");	  
//   st.replace("PROZENTZEICHEN","%25");	     
   char s[3000]="";

  
   strcpy(s,"GET /chatin?ID=");
   strcat(s,ID.print());
   strcat(s,"&SID=");
   strcat(s,Sid.print());
   strcat(s,"&OUT=");
   strcat(s,st.print());
   strcat(s," HTTP/1.1\n");
   strcat(s,"User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; en) Opera 8.51\n"); 
   strcat(s,"Host: 213.203.200.72\n");
   strcat(s,"Accept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\n");
   strcat(s,"Accept-Language: en\n");
   strcat(s,"Accept-Charset: windows-1252, utf-8, utf-16, iso-8859-1;q=0.6, *;q=0.1\n");
   strcat(s,"Accept-Encoding: deflate, gzip, x-gzip, identity, *;q=0\n");
   strcat(s,"Referer: http://213.203.200.72:80/html?PAGE=chat_in1.html&NICKNAME=");
   strcat(s,UserName);
   strcat(s,"&SID=");
   strcat(s,Sid.print());
   strcat(s,"&ID=");
   strcat(s,ID.print());
   strcat(s,"&PW=");
   strcat(s,PW.print());
   strcat(s,"\n");
   strcat(s,"Connection: Keep-Alive, TE\n");
   strcat(s,"TE: deflate, gzip, chunked, identity, trailers\n\n\n");  
   strcat(s,UserName);
   strcat(s,"&SID=");
   strcat(s,Sid.print());
   strcat(s,"&ID=");
   strcat(s,ID.print());

   WORD sockVersion;
   WSADATA wsaData;
   int nret;   
   sockVersion = MAKEWORD(1, 1);
   WSAStartup(sockVersion, &wsaData);
   LPHOSTENT hostEntry;

   if (!anonymize) hostEntry = gethostbyname("213.203.200.72");
   else hostEntry = gethostbyname("http://www.waythru.com/index.php?q=http://213.203.200.72");

   if (!hostEntry) {
      nret = WSAGetLastError();
      ReportError(nret, "gethostbyname()");         // Report the error as before      
      WSACleanup();
      return NETWORK_ERROR;
   }

   SOCKET sendSocket;   
   sendSocket = socket(AF_INET,                      // Go over TCP/IP
                      SOCK_STREAM,                  // This is a stream-oriented socket
                      IPPROTO_TCP);                 // Use TCP rather than UDP                      
   if (sendSocket == INVALID_SOCKET) {
      nret = WSAGetLastError();
      ReportError(nret, "socket()");      
      WSACleanup();
      return NETWORK_ERROR;
   }
   SOCKADDR_IN serverInfo;   
   serverInfo.sin_family = AF_INET;
   serverInfo.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);   // See the explanation in the tutorial
   serverInfo.sin_port = htons(80);                 // Change to network-byte order and
   nret = connect(sendSocket,
                  (LPSOCKADDR)&serverInfo,
                  sizeof(struct sockaddr));
                  
   if (nret == SOCKET_ERROR) {
      nret = WSAGetLastError();
      ReportError(nret, "connect()");      
      WSACleanup();
      return NETWORK_ERROR;
   }  
  
  send(sendSocket,s,strlen(s),0);
  closesocket(sendSocket);   
  fertig:
  int v=0;
  }
  }
//   printf(" say raus\n");  
}


int ChatBotClass::GetIDandSID() {
   if (errorcode) return(1);    
   SOCKET passSocket;                 
   WORD sockVersion;
   WSADATA wsaData;
   int nret;   
   sockVersion = MAKEWORD(1, 1);
   WSAStartup(sockVersion, &wsaData);
   LPHOSTENT hostEntry;
   printf("gettin id and sid\n");
//   hostEntry = gethostbyname("chatcity.de");     // Specifying the server by its name;

   if (!anonymize) hostEntry = gethostbyname("213.203.200.70");
   else hostEntry = gethostbyname("http://www.waythru.com/index.php?q=http://213.203.200.70");

                                                    // another option is gethostbyaddr()                                                    
   if (!hostEntry) {
      nret = WSAGetLastError();
      ReportError(nret, "gethostbyname()");         // Report the error as before      
      WSACleanup();
      return NETWORK_ERROR;
   }
   passSocket = socket(AF_INET,                      // Go over TCP/IP
                      SOCK_STREAM,                  // This is a stream-oriented socket
                      IPPROTO_TCP);                 // Use TCP rather than UDP                      
   if (passSocket == INVALID_SOCKET) {
      nret = WSAGetLastError();
      ReportError(nret, "socket()");      
      WSACleanup();
      return NETWORK_ERROR;
   }   
   
   SOCKADDR_IN serverInfo;   
   serverInfo.sin_family = AF_INET;
   serverInfo.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);   // See the explanation in the tutorial
   serverInfo.sin_port = htons(80);                 // Change to network-byte order and
   nret = connect(passSocket,
                  (LPSOCKADDR)&serverInfo,
                  sizeof(struct sockaddr));                  
   if (nret == SOCKET_ERROR) {
      nret = WSAGetLastError();
      ReportError(nret, "connect()");      
      WSACleanup();
      return NETWORK_ERROR;
   }
   // Send/receive, then cleanup:  
  MString contentlenght;
  contentlenght+=113-20+strlen(UserName)+aktivechannel.strlength();
   
  char s[2000];     
  strcpy(s,"POST /cgi-bin/cc2005/wwb/wwb.cgi HTTP/1.1\n");
  strcat(s,"User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; en) Opera 8.51\n");
  strcat(s,"Host: www.chatcity.de\n");
  strcat(s,"Accept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\n");
  strcat(s,"Accept-Language: en\n");
  strcat(s,"Accept-Charset: windows-1252, utf-8, utf-16, iso-8859-1;q=0.6, *;q=0.1\n");
  strcat(s,"Accept-Encoding: deflate, gzip, x-gzip, identity, *;q=0\n");
  strcat(s,"Referer: http://home.chatcity.de/pages/bereich3.php\n");
  strcat(s,"Connection: Keep-Alive, TE\n");
  strcat(s,"TE: deflate, gzip, chunked, identity, trailers\n");
  strcat(s,"Content-Type: application/x-www-form-urlencoded\n");
  strcat(s,"Content-Length: ");
  strcat(s,contentlenght.print());
  strcat(s,"\n\n");
  strcat(s,"_CID=login%2Flo&PROVIDER=REG&KEY=258&MODE=1048576");
  strcat(s,"&NAME=");
  strcat(s,UserName);        
  strcat(s,"&PW=");               
  strcat(s,Passwort);
  strcat(s,"&CHANNEL=");  
  strcat(s,aktivechannel.print());
  strcat(s,"&Chat=+++++Chatten+++++");      

  int nReadAmount=send(passSocket,s,strlen(s),0);
  MString x;
  MString XSID;
  MString XID;
  MString XPW;
  MString XKEY;
  char pBuffer[6048];
  int v=1;
  while(v!=15) {
     for (int i=0;i<6048;i++) pBuffer[i]=0;
     recv(passSocket,pBuffer,1048,0);   
     x=pBuffer;
     XSID="";XID="";
     XSID=x.getbedfirst("&SID=","&ID=");
     XID=x.getbedfirst("&ID=","&KEY=");
     XPW=x.getbedfirst("&PW=","&SID=");
     XKEY=x.getbedfirst("&KEY=","&SBG=");
     if ((XSID!="")&&(XID!="")&&(XPW!="")) break;
     v++;
  }
  if ((XSID!="")&&(XID!="")&&(XPW!="")) {
     printf("Erfolg: SID=%s ID=%s PW=%s KEY=%s\n",XSID.print(),XID.print(),XPW.print(),XKEY.print());
  } else {
     printf("Konnte keine ID/SID bekommen\n");}
     Sid=XSID.print();
     ID=XID.print();
     PW=XPW.print(); 
     KEY=XKEY.print();
     closesocket(passSocket);
}

int ChatBotClass::getregistrationform() {
   SOCKET passSocket;                 
   WORD sockVersion;
   WSADATA wsaData;
   int nret;   
   sockVersion = MAKEWORD(1, 1);
   WSAStartup(sockVersion, &wsaData);
   LPHOSTENT hostEntry;
   printf("gettin regform\n");

   if (!anonymize) hostEntry = gethostbyname("213.203.200.2");
   else hostEntry = gethostbyname("http://www.waythru.com/index.php?q=http://213.203.200.2");

   if (!hostEntry) {
      nret = WSAGetLastError();
      ReportError(nret, "gethostbyname()");         // Report the error as before      
      WSACleanup();
      return NETWORK_ERROR;
   }
   passSocket = socket(AF_INET,                      // Go over TCP/IP
                      SOCK_STREAM,                  // This is a stream-oriented socket
                      IPPROTO_TCP);                 // Use TCP rather than UDP                      
   if (passSocket == INVALID_SOCKET) {
      nret = WSAGetLastError();
      ReportError(nret, "socket()");      
      WSACleanup();
      return NETWORK_ERROR;
   }   
   
   SOCKADDR_IN serverInfo;   
   serverInfo.sin_family = AF_INET;
   serverInfo.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);   // See the explanation in the tutorial
   serverInfo.sin_port = htons(80);                 // Change to network-byte order and
   nret = connect(passSocket,
                  (LPSOCKADDR)&serverInfo,
                  sizeof(struct sockaddr));                  
   if (nret == SOCKET_ERROR) {
      nret = WSAGetLastError();
      ReportError(nret, "connect()");      
      WSACleanup();
      return NETWORK_ERROR;
   }
   // Send/receive, then cleanup:  
   
  char s[2000];     
  
  strcpy(s,"GET /registrierung/ HTTP/1.1\r\n");
  strcat(s,"User-Agent: Opera/9.10 (Windows NT 5.1; U; en)\r\n");
  strcat(s,"Host: home.chatcity.de\r\n");
  strcat(s,"Accept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\n");
  strcat(s,"Accept-Language: de-DE,de;q=0.9,en;q=0.8\r\n");
  strcat(s,"Accept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\n");
  strcat(s,"Accept-Encoding: deflate, gzip, x-gzip, identity, *;q=0\r\n");
  strcat(s,"Cookie: POPUPCHECK=1194890296737\r\n");
  strcat(s,"Cookie2: $Version=1\r\n");
  strcat(s,"Cache-Control: no-cache\r\n");
  strcat(s,"Connection: Keep-Alive, TE\r\n");
  strcat(s,"TE: deflate, gzip, chunked, identity, trailers\r\n");
  strcat(s,"\r\n");
  
  int nReadAmount=send(passSocket,s,strlen(s),0);

  MString x;
  MString Bild;
  char pBuffer[26048];
  int v=1;
  while(v!=55) {
     for (int i=0;i<6048;i++) pBuffer[i]=0;
     recv(passSocket,pBuffer,1412,0);   
     x=pBuffer;
     if (x.includes("securitycode")) {
        Bild=x.getbedfirst("securitycode/index.php?","\" width=\"80\"");
        printf("gefunden\n");goto fertig;
     }         
     v++;
  }
  fertig:
     MString Bildadr;
     Bildadr="wget -nd -r http://chats.kunden.abacho.net/securitycode/index.php?";
     Bildadr+=Bild.print();
     Bildadr+=" -O index.php";
     closesocket(passSocket);
     printf("%s\n",Bildadr.print());
     system(Bildadr.print()); 
//     SDL_Surface *image;
//     image=IMG_Load("index.php");
     unsigned int hash;

     for (int i=0;i<5;i++) {
//        c2[i]=CalcHash(image,i*16);        
     }
     for (int i=0;i<5;i++) printf(">>%u\n",c2[i]);
    
    system("del index.php");               
//    SDL_UnlockSurface(image);    
}

int ChatBotClass::registername(char *nickname, char *pwd, char *email) {
   readschlussel();     
     for (int i=0;i<5;i++) {
        for (int l=0;l<35;l++) {
            if (c2[i]==schlussel[l].schl) {
              printf("%s",schlussel[l].zeichen.print());
              checkcode=checkcode+schlussel[l].zeichen.print();
              }
        }
     }   
     printf("\n");
     
   SOCKET passSocket;                 
   WORD sockVersion;
   WSADATA wsaData;
   int nret;   
   sockVersion = MAKEWORD(1, 1);
   WSAStartup(sockVersion, &wsaData);
   LPHOSTENT hostEntry;
   printf("register name\n");

   if (!anonymize) hostEntry = gethostbyname("213.203.200.2");
   else hostEntry = gethostbyname("http://www.waythru.com/index.php?q=http://213.203.200.2");

                                                    // another option is gethostbyaddr()                                                    
   if (!hostEntry) {
      nret = WSAGetLastError();
      ReportError(nret, "gethostbyname()");         // Report the error as before      
      WSACleanup();
      return NETWORK_ERROR;
   }
   passSocket = socket(AF_INET,                      // Go over TCP/IP
                      SOCK_STREAM,                  // This is a stream-oriented socket
                      IPPROTO_TCP);                 // Use TCP rather than UDP                      
   if (passSocket == INVALID_SOCKET) {
      nret = WSAGetLastError();
      ReportError(nret, "socket()");      
      WSACleanup();
      return NETWORK_ERROR;
   }   
   
   SOCKADDR_IN serverInfo;   
   serverInfo.sin_family = AF_INET;
   serverInfo.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);   // See the explanation in the tutorial
   serverInfo.sin_port = htons(80);                 // Change to network-byte order and
   nret = connect(passSocket,
                  (LPSOCKADDR)&serverInfo,
                  sizeof(struct sockaddr));                  
   if (nret == SOCKET_ERROR) {
      nret = WSAGetLastError();
      ReportError(nret, "connect()");      
      WSACleanup();
      return NETWORK_ERROR;
   }
   // Send/receive, then cleanup:  

   
  char s[20000];     

  strcpy(s,"POST /registrierung/ HTTP/1.1\r\n");
  strcat(s,"User-Agent: Opera/9.10 (Windows NT 5.1; U; en)\r\n");
  strcat(s,"Host: home.chatcity.de\r\n");  
  strcat(s,"Accept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\n");  
  strcat(s,"Accept-Language: de-DE,de;q=0.9,en;q=0.8\r\n");  
  strcat(s,"Accept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\n");  
  strcat(s,"Accept-Encoding: deflate, gzip, x-gzip, identity, *;q=0\r\n");  
  strcat(s,"Referer: http://home.chatcity.de/registrierung/\r\n");  
  strcat(s,"Cookie: POPUPCHECK=1194890296737\r\n");                
  strcat(s,"Cookie2: $Version=1\r\n");  
  strcat(s,"Connection: Keep-Alive, TE\r\n");  
  strcat(s,"TE: deflate, gzip, chunked, identity, trailers\r\n");        
  strcat(s,"Content-Length: 468\r\n");        
  strcat(s,"Content-Type: application/x-www-form-urlencoded\r\n");        
  strcat(s,"\r\n");          

MString securecode;
securecode=checkcode.print();;

//  cmd=login&nickname=Schnee8876&pwd1=mdykoz&pwd2=mdykoz&email1=chochmah%40gmx.net&email2=chochmah%40gmx.net&securecode=NG979&newsletter=1&gender=1&showgender=1&get_datum_tag=09&get_datum_monat=05&get_datum_jahr=1981&plz=&wohnort=&id_card=1&m               
  strcat(s,"cmd=login&nickname=");   
  strcat(s,nickname);
  strcat(s,"&pwd1=");
  strcat(s,pwd);
  strcat(s,"&pwd2=");
  strcat(s,pwd);
  strcat(s,"&email1=");
  strcat(s,email);  
//  email  //email @ als %40
  strcat(s,"&email2=");
  strcat(s,email);  
  strcat(s,"&securecode=");  
  strcat(s,securecode.print());  
  strcat(s,"&newsletter=1&gender=1&showgender=1&get_datum_tag=09&get_datum_monat=05&get_datum_jahr=1981&plz=&wohnort=&id_card=1&motto=&img_idcard=http%3A%2F%2F&hobby=&interessen=&buecher=&filme=&fernsehsendungen=&musik=&sprachen=&ichsuche=&homepage=&essen=&reiseziel=&getraenke=&tier=&cc=%23000000&sc=%23FFFFFF&system_msg=1&agb=1&agbsubmit=+++registrieren+++");            


  int nReadAmount=send(passSocket,s,strlen(s),0);  
  printf("%s %i\n",s,nReadAmount);


  strcpy(s,"GET /registrierung/register.php HTTP/1.1\r\n");
  strcat(s,"User-Agent: Opera/9.10 (Windows NT 5.1; U; en)\r\n");
  strcat(s,"Host: home.chatcity.de\r\n");  
  strcat(s,"Accept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\n");  
  strcat(s,"Accept-Language: de-DE,de;q=0.9,en;q=0.8\r\n");  
  strcat(s,"Accept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\n");  
  strcat(s,"Accept-Encoding: deflate, gzip, x-gzip, identity, *;q=0\r\n");  
  strcat(s,"Referer: http://home.chatcity.de/registrierung/\r\n");  
  strcat(s,"Cookie: POPUPCHECK=1194890296737\r\n");                
  strcat(s,"Cookie2: $Version=1\r\n");  
  strcat(s,"Connection: Keep-Alive, TE\r\n");  
  strcat(s,"TE: deflate, gzip, chunked, identity, trailers\r\n");        
  strcat(s,"\r\n");          

  nReadAmount=send(passSocket,s,strlen(s),0);  
  printf("%s %i\n",s,nReadAmount);

  printf("registrierung fertig\n");
  
  char pBuffer[1024];
  int y;
  MString x;  
  do {
  y=recv(passSocket,pBuffer,1024,0);
  Sleep(100);  
  } while (y>0);
  closesocket(passSocket);  
}

void ChatBotClass::registernickname(char *nickname, char *pwd, char *email) {
    getregistrationform();
    registername(nickname,pwd,email);
}

