#include "VegaMain.h"
#include "Tournament.h"

////////////////// SECTION USCF Rating report ////////////////////////////////

USCFSectionReport::USCFSectionReport()
{   String str;
	CtrlLayout(*this, "");

	edit0 <<= TD.NAMETORNEO;
	edit0.Tip("optional\n(can be empty)");

	edit1 <<= "F";
	edit1.Add("F");
	edit1.Add("H");
	edit1.Add("Q");
 	edit1.Tip("F full\nH half\nQ quick");

	edit2 <<= "R";
	edit2.Add("R");
	edit2.Add("D");
	edit2.Add("Q");
 	edit2.Tip("R regular\nD dual\nQ quick");

	edit3 <<= "";
  	edit3.Tip("optional\n(can be empty)");

	edit4 <<= "";
 	edit4.Tip("optional\n(can be empty)");

	if (TD.PLAY_SYSTEM==ROUND_ROBIN ) str = "R";
//	else if (PLAY_SYSTEM==ROUND_ROBIN2) strcpy(str, "D");
	else str = "S";
    edit5 <<= str;
	edit5.Add("S");
	edit5.Add("M");
	edit5.Add("R");
	edit5.Add("C");
 	edit5.Tip("S swiss system\nM match\nR round robin\nC cum swiss");

	edit6 <<= AsString(TD.currentRound);
	edit6.Disable();

	edit7 <<= AsString(TD.NPlayer);
	edit7.Disable();

	edit8 <<= "VC";
	edit8.Disable();

	edit9 <<= "#";
	edit9.Disable();

   	btnClose <<= THISBACK(Close);
	btnDone <<= THISBACK(DoneCB);
}


void USCFSectionReport::DoneCB()
{ int i, j, index, rec_seq=1;
  String stringa;
  int *roundRes;
//  short int *playerCol;
  int *playerOpp;

  FileOut out(TD.PATHDIR + "uscfsctn.txt");

  out << NFormat("%d  %d\n", TD.NPlayer, TD.currentRound);

  // section file record
  out << NFormat("%s\t", AsString(~edit0));
  out << NFormat("%s\t", AsString(~edit1));
  out << NFormat("%s\t", AsString(~edit2));
  out << NFormat("%s\t", AsString(~edit3));
  out << NFormat("%s\t", AsString(~edit4));
  out << NFormat("%s\t", AsString(~edit5));
  out << NFormat("%s\t", AsString(~edit6));
  out << NFormat("%s\t", AsString(~edit7));
  out <<  "???????\t";
  out << NFormat("%s\t", AsString(~edit8));
  out << NFormat("%s\t", AsString(~edit9));
  out << "\n";  // close the section record

  // detail file records
  if (TD.IS_SWISS) {
     playerOpp = &TD.playerOpponent[0][0];
     roundRes = &TD.roundResult[0][0];
  }
  else { playerOpp = &TD.playerOpponentRR[0][0];
         roundRes = &TD.roundResultRR[0][0];
       }

  if (TD.currentRound>10) rec_seq=2;
  for (i=1; i<=TD.NPlayer; i++)
      {
       out << NFormat("\t%d\t%d\t%d", i, rec_seq, player[i].idnat);
	   for(j=1; j<=TD.currentRound; j++)
	   { if (TD.IS_SWISS) index = i*(N_ROUND_MAX+1) + j;
	     else index = i*(N_ROUND_MAX_RR+1) + j;

         if (playerOpp[index]==0 && roundRes[index]==3) { //BYE: 1F - 0
        	 out <<  "\tB   0";
	     }
	     else if (playerOpp[index]==0 && roundRes[index]==4) { //lost unplayed game: BYE: 0 - 1F
        	 out << "\tF   0";
	     }
	     else if (playerOpp[index]==0 && (roundRes[index]==6 || roundRes[index]==8)) { //for BYE: XF
        	 out << "\tH   0";
	     }
	     else if (playerOpp[index]!=0 ) {
		     if (roundRes[index]==0)        out << NFormat("\tL%4d", playerOpp[index]);
		     else if (roundRes[index]==4)   out << "\tF   0";
             else if (roundRes[index]==1)   out << NFormat("\tW%4d", playerOpp[index]);
             else if (roundRes[index]==3)   out << "\tX   0";
	         else if (roundRes[index]==2)   out << NFormat( "\tD%4d", playerOpp[index]);
	         else if (roundRes[index]==6 || roundRes[index]==8)  out << "\tZ   0";
	       }
          else out << "\tU   0"; //default case: unplayed
	    }
        out << "\n";
      }
  out.Close();
  Close();
}
////////////////// TOURNAMENT USCF Rating report collect sections ////////////////////////////////

USCFReport::USCFReport()
{
	CtrlLayout(*this, "Tournament Report");
    Ndirectory = max_rounds = 0;

    btnOpen.Tip("Read the list of the sections");
	btnSave.Tip("Save the list of the sections");
    btnAdd.Tip("Select a tournament section");
	btnRemove.Tip("Remove the selected section");

	btnAdd.WhenAction = THISBACK(AddCB);
	btnRemove.WhenAction = THISBACK(RemoveCB);
	btnOpen.WhenAction = THISBACK(OpenFileCB);
	btnSave.WhenAction = THISBACK(SaveFileCB);
	btnClose.WhenAction = THISBACK(Close);
	btnDone.WhenAction = THISBACK(DoneCB);
}


void USCFReport::OpenFileCB()
{ String line, name;
  int i;

#ifdef PLATFORM_WIN32
   name = TD.PATHVEGAPRG + "\\" + "savedpath.txt";
#endif
#ifdef PLATFORM_POSIX
   name = TD.PATHVEGAPRG + "/" + "savedpath.txt";
#endif

  FileIn in(name);
  if (in) {
     list.Clear();
     line = in.GetLine();
     Ndirectory = StrIntValue(line);
     for(i=1; i<=Ndirectory; i++)
        { line = in.GetLine();
          list.Add(line);
        }
	 in.Close();
  }
  else Exclamation("I can't read the file");
}

void USCFReport::SaveFileCB()
{ String name;
  int i;

  if (Ndirectory<1) {
      Exclamation("There aren't Sections to save");
      return;  // nothing to retrive
  }

#ifdef PLATFORM_WIN32
   name = TD.PATHVEGAPRG + "\\" + "savedpath.txt";
#endif
#ifdef PLATFORM_POSIX
   name = TD.PATHVEGAPRG + "/" + "savedpath.txt";
#endif

  FileOut out(name);
  out << Ndirectory << "\n";
  for(i=1; i<=Ndirectory; i++) out << AsString( list.Get(i-1) ) << "\n";
  out.Close();
}

void USCFReport::RemoveCB()
{
  int ans, i=list.GetCursor();

  if (Ndirectory < 1 || i < 0) return;
  ans = PromptYesNo("Do you want to remove the selected section?");
  if (ans == 1 ) list.Remove( i );
  Ndirectory--;
}


void USCFReport::AddCB()
{ String stringa, directory;
  FileSel fs;
  if (Ndirectory) fs.dir <<= AsString(list.Get(Ndirectory - 1)); //start with the last directory
  fs.ExecuteSelectDir();
  directory = ~fs;
  if ( !IsNull(directory) ) {
     list.Add(directory);
     Ndirectory++;
  }
  else Exclamation( t_("You have not selected a directory"));
}

void USCFReport::DoneCB()
{ String s, str, name, stringa;
  int i, n, rounds;
  String bdate, edate, listfile[20];

 // Ndirectory = list.GetColumnItems();
  if (Ndirectory<1)
     { Exclamation("Please select at least one section!");
       return;
     }
  for(i=1; i<=Ndirectory; i++) { // legge i file dei singoli tornei
#ifdef PLATFORM_WIN32
     name = AsString(list.Get(i-1)) + "\\" + "uscfsctn.txt";
#endif
#ifdef PLATFORM_POSIX
     name = AsString(list.Get(i-1)) + "/" + "uscfsctn.txt";
#endif
     listfile[i] = name; // remind the section files

     FileIn in(name);
     if (in) {
        Vector<String> field = Split( in.GetLine(), ' ', true);
        n = StrIntValue(field[0]);
        rounds = StrIntValue(field[1]);
        if (rounds>max_rounds) max_rounds=rounds;
        in.Close();
     }
     else Exclamation("I can't read the &" + name + " &Please generate it");
 }
 Close();
 USCFTournamentReport USCFreport(Ndirectory, max_rounds, listfile);
 USCFreport.Execute();
}

////////////////// TOURNAMENT USCF Rating report generator ////////////////////////////////


USCFTournamentReport::USCFTournamentReport(int SectionNumber, int max_r, String listfile[])
{   int i;
	CtrlLayout(*this, "");
    Ndirectory = SectionNumber;
    max_rounds = max_r;
    String str, strdate;

    for (i=1; i<=SectionNumber; i++) filesection[i] = listfile[i];

    edit0.AutoFormat(false);
    edit0.Tip("enter Year, Month and Day:\nyymmdd (six digit!)");
    edit2 <<= Ndirectory;
	edit3.Tip("yyyymmdd");
    edit4.Tip("yyyymmdd");
	edit5 <<= 0;
	edit9 <<= "USA";

	edit10 <<= "T";
	edit10.Add("T");
	edit10.Add("A");
	edit10.Add("N");
    edit10.Tip("T tournament director\nA affiliate\nN do not send cross tables");

	edit11 <<= "N";
	edit11.Add("Y");
	edit11.Add("N");
    edit11.Tip("Y scholastic\nN non scholastic");

	btnDone.WhenAction = THISBACK(DoneCB);
	btnClose.WhenAction = THISBACK(Close);
}


void USCFTournamentReport::DoneCB()
{ int i, j, n, rounds, k, S_DTLREC01=1;
  String bdate, edate, stringa, s, f1;
  String currdir, string;

#ifdef PLATFORM_WIN32
   currdir = TD.PATHVEGAPRG + "\\";
#endif
#ifdef PLATFORM_POSIX
   currdir = TD.PATHVEGAPRG + "/";
#endif

  FileOut out(currdir + "thexport.txt");
  if (out.IsError())
     { Exclamation("Cannot save the Tournament Header File");
       return;
     }
/*
H_EVENT_ID,C,9			yymmddccc
H_NAME,C,35
H_TOT_SECT,N,2,0		# sections
H_BEG_DATE,D			MM/DD/YYYY
H_END_DATE,D			MM/DD/YYYY
H_RCV_DATE,D			N/A
H_ENT_DATE,D			N/A
H_AFF_ID,C,8			affiliate
H_CITY,C,21
H_STATE,C,2
H_ZIPCODE,C,10			nnnnn or nnnnn-nnnn
H_COUNTRY,C,21			"USA" (optional?)
H_SENDCROS,C,1			T, A, N (TD, Aff, none)
H_SCHOLAST,C,1			Y, N (scholastic, non)
H_SECREC01,N,7,0		"1" section record number if more than one event
*/

  out << "15\n";
  out << "H_EVENT_ID    C    9\n";
  out << "H_NAME        C   35\n";
  out << "H_TOT_SECT    N    2\n";
  out << "H_BEG_DATE    D    8\n";
  out << "H_END_DATE    D    8\n";
  out << "H_RCV_DATE    D    8\n";
  out << "H_ENT_DATE    D    8\n";
  out << "H_AFF_ID      C    8\n";
  out << "H_CITY        C   21\n";
  out << "H_STATE       C    2\n";
  out << "H_ZIPCODE     C   10\n";
  out << "H_COUNTRY     C   21\n";
  out << "H_SENDCROS    C    1\n";
  out << "H_SCHOLAST    C    1\n";
  out << "H_SECREC01    N    7\n";

  string = NFormat("%06d", StrIntValue(AsString( ~edit0 )));
  if ( string.GetLength() < 6) {
         Exclamation( "Please fills the Event ID field with 6 digits" );
         out.Close();
         return;
  } else out << string + "001\t";

  string = TrimLeft(AsString(~edit1));
  if ( string.GetLength() < 1) {
         Exclamation( "There can't be empty fields!&Please fill all fields with some value");
         out.Close();
         return;
  } else out << string + "\t";

  string = TrimLeft(AsString(~edit2));
  if ( string.GetLength() < 1) {
         Exclamation( "There can't be empty fields!&Please fill all fields with some value");
         out.Close();
         return;
  } else out << string + "\t";

  string = TrimLeft(AsString(~edit3));
  if ( string.GetLength() < 8) {
         Exclamation( "Set a correct begin date");
         out.Close();
         return;
  } else out << string + "\t";

  string = TrimLeft(AsString(~edit4));
  if ( string.GetLength() < 8) {
         Exclamation( "Set a correct end date");
         out.Close();
         return;
  } else out << string + "\t";

  out << "\t\t"; // RCV_DATE e ENT_DATE

  string = TrimLeft(AsString(~edit5));
  if ( string.GetLength() < 1) {
         Exclamation( "There can't be empty fields!&Please fill all fields with some value");
         out.Close();
         return;
  } else out << string + "\t";

  string = TrimLeft(AsString(~edit6));
  if ( string.GetLength() < 1) {
         Exclamation( "There can't be empty fields!&Please fill all fields with some value");
         out.Close();
         return;
  } else out << string + "\t";

  string = TrimLeft(AsString(~edit7));
  if ( string.GetLength() < 1) {
         Exclamation( "There can't be empty fields!&Please fill all fields with some value");
         out.Close();
         return;
  } else out << string + "\t";

  string = TrimLeft(AsString(~edit8));
  if ( string.GetLength() < 1) {
         Exclamation( "There can't be empty fields!&Please fill all fields with some value");
         out.Close();
         return;
  } else out << string + "\t";

  string = TrimLeft(AsString(~edit9));
  if ( string.GetLength() < 1) {
         Exclamation( "There can't be empty fields!&Please fill all fields with some value");
         out.Close();
         return;
  } else out << string + "\t";

  string = AsString(~edit10);
  out << string + "\t";

  string = AsString(~edit11);
  out << string + "\t";

  out << "1\n";    //SECREC01
  out.Close();

  // generate Tournament Section File
/*
S_EVENT_ID,C,9			same as H_EVENT_ID
S_SEC_NUM,C,2			1-n
S_SEC_NAME,C,10			(optional)
S_K_FACTOR,C,1			F, Q (full, quick) ### test H (half)
S_R_SYSTEM,C,1			R, Q (regular, quick) ### test D (dual)
S_CTD_ID,C,8			(optional)
S_ATD_ID,C,8			(optional)
S_TRN_TYPE,C,1			S, R, M, C (swiss, round robin, match, cum swiss)
S_TOT_RNDS,N,2,0		Max 20. For R, enter # players
S_LST_PAIR,N,4,0		Last pairing number for section
S_DTLREC01,N,7,0		"1" for section 1, previous section's S_LST_PAIR + S_DTLREC01
S_OPER,C,2			XX (disk) ### test UU (??)
S_STATUS,C,1			"#"
*/

  out.Open(currdir + "tsexport.txt");
  if (out.IsError())
     { Exclamation("Cannot save the Tournament Section File");
       return;
     }
  out << "13\n";
  out << "S_EVENT_ID    C    9\n";
  out << "S_SEC_NUM     C    2\n";
  out << "S_SEC_NAME    C   10\n";
  out << "S_K_FACTOR    C    1\n";
  out << "S_R_SYSTEM    C    1\n";
  out << "S_CTD_ID      C    8\n";
  out << "S_ATD_ID      C    8\n";
  out << "S_TRN_TYPE    C    1\n";
  out << "S_TOT_RNDS    N    2\n";
  out << "S_LST_PAIR    N    4\n";
  out << "S_DTLREC01    N    7\n";
  out << "S_OPER        C    2\n";
  out << "S_STATUS      C    1\n";

  for(i=1; i<=Ndirectory; i++) {
     String s, s1, s2, sss;
     int l, lsss;
     FileIn in(filesection[i]);
     if (in) {
       Vector<String> field = Split( in.GetLine(), ' ', true);
       n = StrIntValue(field[0]);
       rounds = StrIntValue(field[1]);
       sss = in.GetLine();
       lsss = sss.GetLength();
	   l = sss.Find("???????");
       s1 = sss.Mid(0,l);
       s2 = sss.Mid(l+7,lsss-l+7);

       s = NFormat("%s%d%s", s1, S_DTLREC01, s2); // remove '???????'
       out << NFormat("%s001\t%d\t", NFormat("%06d", StrIntValue(AsString( ~edit0 ))), i);
       out << s + "\n";

       in.Close();
       S_DTLREC01 += n;
     }
     else { Exclamation("I can't read the uscfsctn.txt file");
            return;
          }
  }
  out.Close();

  // generate Tournament Detail File

/*
D_EVENT_ID,C,9			same as H_EVENT_ID
D_SEC_NUM,C,2
D_PAIR_NUM,C,4			1-n
D_REC_SEQ,C,1			if rds 1-10 "1", 11-20 "2"
D_MEM_ID,C,8			(optional)
D_RND01,C,5			W=win D=draw L=loss
D_RND02,C,5			X= Forfeit win Z= Forfeit draw F= forfeit loss
D_RND03,C,5			B= 1 pt bye  H= 1/2 pt bye U= unplayed game
D_RND04,C,5			Cnnnn, where n is pair num left-padded with space
D_RND05,C,5			Unplayed C...0
D_RND06,C,5			No round ....0
D_RND07,C,5			(all optional)
D_RND08,C,5
D_RND09,C,5
D_RND10,C,5
*/

  stringa = currdir + "tdexport.txt";
  out.Open(stringa);
  if (out.IsError())
     { Exclamation("Cannot save the Tournament Detail File");
       return;
     }
  out << 5 + max_rounds << "\n";
  out << "D_EVENT_ID    C    9\n";
  out << "D_SEC_NUM     C    2\n";
  out << "D_PAIR_NUM    C    4\n";
  out << "D_REC_SEQ     C    1\n";
  out << "D_MEM_ID      C    8\n";
  for (k=1; k<=max_rounds; k++) out << NFormat("D_RND%02d       C    5\n", k);

  for(i=1; i<=Ndirectory; i++) {
     //stringa = PATHDIR[i] + "uscfsctn.txt"; //built the whole file name
     FileIn in(filesection[i]);
     if ( in ) {
       Vector<String> field = Split( in.GetLine(), ' ', true);
       n = StrIntValue(field[0]);
       rounds = StrIntValue(field[1]);
       in.GetLine(); // skip second row
       for (j=1; j<=n; j++) {
           out << NFormat("%06d", StrIntValue(AsString( ~edit0 ))) + "001\t" ;
                       //TrimLeft(AsString(~edit0)) + "001\t";
           out << i;
           s = in.GetLine();
           out << s;
           for (k=rounds+1; k<=max_rounds; k++) out << "\t    0";
           out << "\n";
       }
       in.Close();
     }
     else { Exclamation("I can't read the uscfsctn.txt file");
            return;
          }
  }
  out.Close();

 f1 = currdir + "convert.exe";
 system(f1);
 Close();
}
