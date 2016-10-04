#include "VegaMain.h"


void VegaMain::CheckRegistration()
{ char stringa[100], str1[100], str2[100], answer[100];
  FileIn in;

#ifdef LINUX
    Exclamation("This software is registered to:\nYOU", " OK ");
    return;
#endif

  sprintf(stringa, "%sregcode.txt", PATHVEGAPRG);
  if ((in=fopen(stringa, "r"))==NULL)
     { WarningMessage(
       "This software CANNOT be used in any official chess competition\nwith more than 30 players.\nPlease contact the author to obtain a suitable license.\n---\nQuesto programma NON puo' essere usato in alcun torneo reale\ncon piu' di 30 giocatori.\nSi contatti l'autore per avere una più estesa licenza d'uso",
//       "This software can be used only for evaluation purpose\nand CANNOT be used in any official chess competition.\nContact the author to obtain a suitable license.\n---\nQuesto programma puo' essere usato solo allo scopo di valutarlo\ne NON puo' essere usato in alcun torneo reale.\nContattare l'autore per avere una più estesa licenza d'uso",
       "CLOSE");
       return;
     }
  fgets(str1, 90, in); // skip
  fgets(str1, 90, in); // skip
  fgets(str1, 90, in); str1[strlen(str1)-1]='\0';
  fgets(str2, 90, in); str2[strlen(str2)-1]='\0';
  fclose(in);
  ProcessWord(str1, answer);
  if (strcmp(str2,answer)==0)
     { sprintf(stringa, "This software is registered to:\n %s", str1);
       WarningMessage(stringa, " OK ");
     }
  else  WarningMessage(
       "This software CANNOT be used in any official chess competition\nwith more than 30 players.\nPlease contact the author to obtain a suitable license.\n---\nQuesto programma NON puo' essere usato in alcun torneo reale\ncon piu' di 30 giocatori.\nSi contatti l'autore per avere una più estesa licenza d'uso",
//       "This software can be used only for evaluation purpose\nand CANNOT be used in any official chess competition.\nContact the author to obtain a suitable license.\n---\nQuesto programma puo' essere usato solo per valutarlo\ne NON puo' essere usato in alcun torneo reale.\nContattare l'autore per avere una più estesa licenza d'uso",
       " OK ");
}


void VegaMain::ProcessWord(const char word[], char str[])
{ char  a;
  int i, l, k;
  char code [26][51] = {
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
 l = strlen(word);
 for (i=0; i<l; i++)
     { //if (word[i]>='A' && word[i]<='Z')  str[i] = str[i] - 32;
       a = word[i];
       if (a>64 && a<90) k =  a - 64;
       else k = i;
       str[i] =  (char) code[i][k];
     }
 for (i=l; i<=24; i++)
     { a = word[i];
       if (a>64 && a<90) k =  a - 64;
       else k = i;
       str[i] = (char) code[i][k];
     }
 str[25] = '\0';
}
