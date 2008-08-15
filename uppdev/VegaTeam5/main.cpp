#include "VegaMain.h"
#include "Tournament.h"

#define TFILE <VegaTeam5/message.t>
#include <Core/t.h>

void  ReadConfigFile()
{ VectorMap<String, String> cfg = LoadIniFile("./vega.cfg");
  String recentdir = cfg.Get("RECENTDIR", Null);
  String language  = cfg.Get("LANGUAGE", Null);
  String pathDB  = cfg.Get("PATHDB", Null);
// if ( recentdir.ReverseFind('\\') || recentdir.ReverseFind('\\') )
// recentdir.Remove( recentdir.GetLength() - 1, 1); // remove the final '\' or '/'
  TD.PATHDIR = recentdir;
  TD.LANGUAGE = language;
  TD.PATHDIRDB = pathDB;
  SetDefaultCharset(CHARSET_UTF8);
  if      ( language == "IT-IT" ) SetLanguage( LNG_('I','T','I','T') );
  else if ( language == "DE-DE" ) SetLanguage( LNG_('D','E','D','E') );
  else if ( language == "FR-FR" ) SetLanguage( LNG_('F','R','F','R') );
  else if ( language == "ES-ES" ) SetLanguage( LNG_('E','S','E','S') );
  else if ( language == "PT-PT" ) SetLanguage( LNG_('P','T','P','T') );
  else if ( language == "RU-RU" ) SetLanguage( LNG_('R','U','R','U') );
  else if ( language == "PT-PT" ) SetLanguage( LNG_('P','T','P','T') );
  else SetLanguage( LNG_('E','N','U','S') );   // default English
}

void  SaveConfigFile()
{ String cfg;
  cfg << "RECENTDIR=" << TD.PATHDIR << "\n"
  "LANGUAGE=" << TD.LANGUAGE << "\n"
  "PATHDB=" << TD.PATHDIRDB << "\n";
  if(!SaveFile("./vega.cfg", cfg)) Exclamation(t_("Error saving configuration file!"));
}

void ProcessWord(const char words[], String& str)
//void ProcessWord(const char words[], char str[])
{ char  a;
  int i, l, k;
  const char code [26][51] = {
 "VEGA456ISTEINPROGRAMMZURVERWALTUNGVONS3CHAC567HTUR",
 "RVOLLRUNDENTURN4IEREVEGAI27STSOAUFGEBAUTDASSESDIEA",
 "HRENDEINESTURNI456ERSSOWEITALSMGLICHUNTERSTTZTERFA",
 "RAUSLOSUNGFRDIENCHSTERUNDEEI456NGABED4SEFRDIELAUFE",
 "AUSGABEDERFORLQ56BELLEUNDANDERERLISTENZUMAUSHANGFR",
 "WHRENDDIE243SERSTNAUCHEINIGEZUSTZLICHEFUNKTIONENAU",
 "ZUMBEISPIELDIEBEHANLERRCKTRITTENLISTENAUSDRUCWQKED",
 "USWVEGEREITSERFOLGTENBENUTZEREINGABENAUSUNDBIETAET",
 "ENZEITZUL2SSIGENFUN2KTIONENANVEGABEISCHIEDSLTETFWW",
 "ERFISZU999SPIELERNUNDVERWAL456TENVONBISZU20RUNDENF",
 "ERFASSUNGVONBISZU24SPIELELTENVONBISZU23RUNDENFRVOL",
 "AUTOMATISCHEAUSLOSUNGGEMDENFIDEREGELNFRSCHWEIZERSV",
 "UNDUSCFSYST23EMMANUELLEPAARUNGV456ERB245ESSERUNGDQ",
 "BERCKSICHTIGUERADENTEILNEHMERZAHLERGEBNPIELERDURCH",
 "FHRENDERFORTSCHRITTSTABELLEM456GLICHKEITDERDATENND",
 "BERCKSICHTIGUNGVONTURNIERABBRECHERN345UNDWIEDERISD",
 "HEREINNAHMEVONNACHZGLERNINSBEREITSLAUFENDETURNIERA",
 "ICHTENFORTSCHRITTSTABELLENTEILNEHMERLISTENETCERZEU",
 "VEGALUFTUNTERDOSWINDOWSUNDLINUXIM32BITMODUSAUFJEDE",
 "OBWOHLVEGASOAUFGEBAUTISTDASSESSEHRINTUITIVUNDLEICH",
 "EREMPFOHLENSI456CHMITDIESEMHANDBUCHVER34534TRAUTZU",
 "RENUMALLEFUNKTIONENDESPROGRAMMSKENNENZU7658LERNENI",
 "DERANWENDERBEN3453TIGTZWINGENDDIEKENNTN435ISDER476",
 "SOWIEIHREIMPLEMENT456IERUNGINVEGAUMDESSENVERHALTEN",
 "HENODERFRDENFALLDASSEREI456546NEMANUELLEAUSLOSUNGD",
 };
 l = strlen(words);
 for (i=0; i<l; i++)
     { a = words[i];
       if (a>64 && a<90) k =  a - 64;
       else k = i;
       //str[i] =  (char) code[i][k];
       str << code[i][k];
     }
 for (i=l; i<=24; i++)
     { a = words[i];
       if (a>64 && a<90) k =  a - 64;
       else k = i;
       //str[i] = (char) code[i][k];
       str << code[i][k];
     }
 //str[25] = '\0';
}


int CheckRegistration( String& client)
{ String stringa, str1, str2, answer;

#ifdef PLATFORM_POSIX
    Exclamation("This software is registered to:\nYOU");
    client = "You";
    return 1;
#endif

  stringa = TD.PATHVEGAPRG + "\\regcode.txt";
  FileIn in(stringa);
  if (!in) {
       Exclamation(
		"[s0;=/ This software [*@6 CANNOT] be used in any official chess competition&]"
		"[s0;=*/@(28.42.200) with more than 30 players.&]"
		"[s0;=/ Please contact the author to obtain a suitable license&]"
		"[s0;= `-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-&]"
		"[s0;=/ Questo programma [*@6 NON] puo`' essere usato in alcun torneo reale&]"
		"[s0;=*/@(28.42.200) con piu`' di 30 giocatori.&]"
		"[s0;=/ Si contatti l`'autore per avere una piu`' estesa licenza d`'uso]");
       return 0;
     }
  str1 = in.GetLine(); // fgets(str1, 90, in); // skip
  str1 = in.GetLine(); //. fgets(str1, 90, in); // skip
  str1 = in.GetLine();// fgets(str1, 90, in); str1[strlen(str1)-1]='\0';
  str2 = in.GetLine(); // fgets(str2, 90, in); str2[strlen(str2)-1]='\0';
  in.Close(); // fclose(in);

  ProcessWord(str1, answer);

  if (strcmp(str2,answer)==0)
     {  PromptOK(NFormat(  "[s0;=/ This software is registered to:&] [s0;=/ [*@6 %s] ]", str1) );
        client = str1;
        return 1;
     }
  else {
      Exclamation(
		"[s0;=/ This software [*@6 CANNOT] be used in any official chess competition&]"
		"[s0;=*/@(28.42.200) with more than 30 players.&]"
		"[s0;=/ Please contact the author to obtain a suitable license&]"
		"[s0;= `-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-&]"
		"[s0;=/ Questo programma [*@6 NON] puo`' essere usato in alcun torneo reale&]"
		"[s0;=*/@(28.42.200) con piu`' di 30 giocatori.&]"
		"[s0;=/ Si contatti l`'autore per avere una piu`' estesa licenza d`'uso]");
      return 0;
  }
}

/*
int CheckRegistration( String& client)
{ char  str1[100], str2[100], answer[100];
  String stringa;
  FILE* in;
#ifdef PLATFORM_POSIX
    Exclamation("This software is registered to:\nYOU");
    client = "You";
    return 1;
#endif

//  sprintf(stringa, "%s\\regcode.txt", TD.PATHVEGAPRG);
  stringa = TD.PATHVEGAPRG + "\\regcode.txt";
  if ((in=fopen(stringa, "r"))==NULL)
     { Exclamation(
"[s0;=/ This software [*@6 CANNOT] be used in any official chess competition&]"
"[s0;=?/@(28.42.200) with more than 30 players.&]"
"[s0;=/ Please contact the author to obtain a suitable license&]"
"[s0;= `-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-&]"
"[s0;=/ Questo programma [*@6 NON] puo`' essere usato in alcun torneo reale&]"
"[s0;=?/@(28.42.200) con piu`' di 30 giocatori.&]"
"[s0;=/ Si contatti l`'autore per avere una piu`' estesa licenza d`'uso]");
       return 0;
     }
  fgets(str1, 90, in); // skip
  fgets(str1, 90, in); // skip
  fgets(str1, 90, in); str1[strlen(str1)-1]='\0';
  fgets(str2, 90, in); str2[strlen(str2)-1]='\0';
  fclose(in);
  ProcessWord(str1, answer);
  if (strcmp(str2,answer)==0)
     {  //PromptOK(NFormat( "This software is registered to:\n %s", str1));
        PromptOK(NFormat(  "[s0;=/ This software is registered to:&] [s0;=/ [*@6 %s] ]", str1) );
        client = str1;
        return 1;
     }
  else {
      Exclamation(
   //       "This software CANNOT be used in any official chess competition&with more than 30 players.&Please contact the author to obtain a suitable license.&---&Questo programma NON puo' essere usato in alcun torneo reale\ncon piu' di 30 giocatori.&Si contatti l'autore per avere una più estesa licenza d'uso");
"[s0;=/ This software [*@6 CANNOT] be used in any official chess competition&]"
"[s0;=?/@(28.42.200) with more than 30 players.&]"
"[s0;=/ Please contact the author to obtain a suitable license&]"
"[s0;= `-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-&]"
"[s0;=/ Questo programma [*@6 NON] puo`' essere usato in alcun torneo reale&]"
"[s0;=?/@(28.42.200) con piu`' di 30 giocatori.&]"
"[s0;=/ Si contatti l`'autore per avere una piu`' estesa licenza d`'uso]");
       return 0;
  }
}
*/


VegaMain::VegaMain()
{
	CtrlLayout(*this, VERSION);
	spls.Vert();
	spls.Vert(tabs, consoleFrame); // <<<<<<!!!!!
	spls.SetPos(7600);

//consoleFrame.VSizePosZ(820, 10);
//	spls.Add(tabs);
//	spls.Add(consoleFrame);
//	spls.AddFrame(NullFrame()); //FIX for 608 or add any frame

	SetBars();
	SetTabs();
	Init();
	BackPaint();
	this->WhenClose = THISBACK(Exit); // prevent the closure
}

// these classes are GLOBAL
RoundData TD; //create an instance of RoundData
TeamArchive player[N_PLAYER_MAX+1]; //create N_PLAYER_MAX+1 instances of player
VectorMap<String, TeamArchive> db;

GUI_APP_MAIN
{ // Draw::SetStdFont(Arial(11)); //set the font of the entire app
  String name_user;
  TD.PATHVEGAPRG = GetCurrentDirectory();
  if (CheckRegistration(name_user)) TD.USER_REGISTERED = name_user;
  else TD.USER_REGISTERED = "unregistered";
  ReadConfigFile();
  VegaMain().Zoomable().Run();
  SaveConfigFile();
}
