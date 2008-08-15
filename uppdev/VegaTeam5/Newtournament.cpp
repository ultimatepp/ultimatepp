#include "VegaMain.h"
#include "Tournament.h"

extern RoundData TD;
extern TeamArchive player[N_PLAYER_MAX+1];
extern VectorMap<String, TeamArchive> db;

bool Contains(const String& text, const String& substr)
{
	for(const char *s = text; s <= text.End() - substr.GetLength(); s++)
		if(strncmp(s, substr, substr.GetLength()) == 0)
			return true;
	return false;
}


int FilterSpace(int c)
{
	if (c == ';' || c ==' ' || c == '.' || c == ',') return false;
	else return c;
}


// check once more the inserted data and create a new tournament
// flag = 1 for a new torneo
// flag = 0 to modify it
// flag = 2 to export tournament
NewTournamentDlg::NewTournamentDlg(int option)
{   int i, j;
	String dir;
//	Label dv___30, dv___32, dv___34;
    flag = option;
	switch (flag)
    { case 0: CtrlLayout(*this, t_("Modify Tournament")); break;
      case 1: CtrlLayout(*this, t_("New Tournament")); break;
      case 2: CtrlLayout(*this, t_("Export Tournament")); break;
    }
    editTourn <<= TD.NAMETORNEO;
    editPlace <<= TD.PLACETORNEO;
    editFed   <<= TD.HOSTFEDERATION;
    editFed.Tip(t_("Federation FIDE code"));
    editBeginDate <<= TD.DATATORNEO_B;
    editBeginDate.Tip(t_("ddmmyyyy for FIDE\nyyyymmdd for USCF"));
    editEndDate <<= TD.DATATORNEO_E;
    editEndDate.Tip(t_("ddmmyyyy for FIDE\nyyyymmdd for USCF"));
    btnIsFIDE <<= TD.IS_FIDE_TOURNAMENT;
    btnIsFIDE.Tip(t_("Check it if pairing (swiss system)\nand rating calculation are made with\nthe FIDE rating"));
    editArbiter <<= TD.ARBITER;
    editBoards <<= 4;
    editBoards.Tip( t_("Total number of players for each team") );

    editSystem.Add("Round Robin Single");
	editSystem.Add("Round Robin Double");
    editSystem.Add("Swiss Dubov FIDE");
	editSystem.Add("Swiss Vega");
    editSystem.Add("Swiss USCF");
	editSystem.Add("Swiss Lim FIDE");
    switch (TD.PLAY_SYSTEM) {
        case DUBOV: editSystem        <<= "Swiss Dubov FIDE"; break;
        case ROUND_ROBIN2: editSystem <<= "Round Robin Double"; break;
        case ROUND_ROBIN: editSystem <<= "Round Robin Single"; break;
        case VEGA: editSystem <<= "Swiss Vega"; break;
        case USCF: editSystem <<= "Swiss USCF"; break;
        case LIM: editSystem <<= "Swiss Lim FIDE"; break;
    }

	editFirstScore.Add("Sum points");
	editFirstScore.Add("Match points");
    if (TD.SCORE_TYPE == 1) editFirstScore <<= "Sum points";
    else editFirstScore <<= "Match points";
    editFirstScore.Tip("Sum points: sum of all points made by players\nMatch points: made by the team");

    editScore.Add("0 - ½ - 1");
    editScore.Add("0 - 1 - 3");
  //  editScore.Add("0 -0.4- 1");
    editScore.Add("0 - 2 - 5");
    if ( (int) (10*TD.DRAW)==4)   editScore <<= "0 -0.4- 1";
    else if ( (int) TD.WON==1 )   editScore <<= "0 - ½ - 1";
    else if ( (int) TD.WON==3 )   editScore <<= "0 - 1 - 3";
    else editScore <<= "0 - 2 - 5";

    editRound <<= TD.NRounds;
    editRound.Tip(t_("Max 20 rounds for Swiss System\nCalculated by Vega for round robin"));
    editAccRound.Add("0");
    editAccRound.Add("1");
    editAccRound.Add("2");
    editAccRound <<= AsString(TD.NAccRound);

    //set the option button
    for (i=1; i<=NUM_BTN; i++) {
        Add(opt[i]);
        opt[i].LeftPosZ(80, 130).TopPosZ(192 + (i-1)*20, 15);
        opt[i].WhenAction = THISBACK1(AddTyebreak, i);
    }
    opt[1].SetLabel("Bucholz Cut 1");
    opt[2].SetLabel("Bucholz Total");
    opt[3].SetLabel("Bucholz Median");
    opt[4].SetLabel("Sonneborn-Berger");
    opt[5].SetLabel("Cumulative");
    opt[6].SetLabel("Average Rating Opp.");
    opt[7].SetLabel("Most Blacks");
    opt[8].SetLabel("Most Wins");
    opt[9].SetLabel("Av. Perf. Rat. Opp.");
    for (j=1; j<=NUM_BTN; j++) {
         if ( TD.tyebreakorder[j] ) opt[ TD.tyebreakorder[j] ] <<= true;
    }
    //set the arrayctrl with the selected tyebreak
	collist.AddColumn("TB", t_("Selected Tie Breaks"), 18);   //0

    //current setting
    for (j=1; j<=NUM_BTN; j++) {
       if      ( TD.tyebreakorder[j] == 1 ) collist.Add("1: Buccholz Cut 1");
       else if ( TD.tyebreakorder[j] == 2 ) collist.Add("2: Buccholz Total");
       else if ( TD.tyebreakorder[j] == 3 ) collist.Add("3: Buccholz Median");
       else if ( TD.tyebreakorder[j] == 4 ) collist.Add("4: Sonneborn-Berger");
       else if ( TD.tyebreakorder[j] == 5 ) collist.Add("5: Cumulative");
       else if ( TD.tyebreakorder[j] == 6 ) collist.Add("6: Average Rat. Opp.");
       else if ( TD.tyebreakorder[j] == 7 ) collist.Add("7: Most Blacks");
       else if ( TD.tyebreakorder[j] == 8 ) collist.Add("8: Most Wins");
       else if ( TD.tyebreakorder[j] == 9 ) collist.Add("9: Av. Perf. Rat. Opp.");
    }
    editTFN <<= TD.FILETORNEO;
    editTFN.SetFilter(FilterSpace).MaxChars(8); //added filter

   	dir = TD.PATHDIR; // it has a final slash that should be removed
    if ( dir.Right(1) == "\\" || dir.Right(1) == "/" )
         dir.Remove( dir.GetLength() - 1, 1); // remove the final '\' or '/'
    editDir <<= dir;

	if (flag == 2) { // export tournament
	   Add(dv___30);
	   dv___30.SetLabel(t_("Export players in this")).SetAlign(ALIGN_RIGHT).SetInk(SLtRed).HSizePosZ(12, 368).TopPosZ(435, 19);
	   Add(dropRating);
	   dropRating.LeftPosZ(140, 88).TopPosZ(435, 19);
	   dropRating.Add(t_("FIDE"));
	   dropRating.Add(t_("National"));
	   dropRating.Add(t_("FIDE & Nat"));
	   dropRating <<= "FIDE";
	   Add(dv___32);
	   dv___32.SetLabel(t_("rating interval: Min")).SetAlign(ALIGN_CENTER).HSizePosZ(236, 168).TopPosZ(435, 19);
	   Add(edtmin);
	   edtmin.Min(0).Max(9999).LeftPosZ(340, 48).TopPosZ(435, 19);
   	   Add(dv___34);
   	   dv___34.SetLabel(t_("- Max ")).SetAlign(ALIGN_CENTER).HSizePosZ(396, 72).TopPosZ(435, 19);
	   Add(edtmax);
	   edtmax.Min(0).Max(9999).LeftPosZ(436, 48).TopPosZ(435, 19);
	}

    btnDir.WhenAction = THISBACK1(SetTournamentDir, dir);
    btnClose <<= Breaker(999); //.WhenAction = THISBACK(Close);

	if (flag == 2) btnDone.WhenAction = THISBACK(ExportDlg); // export tournament
    else btnDone.WhenAction = THISBACK(DoneDlg);
}



void NewTournamentDlg::DoneDlg()
{ String e0, e1, ehf, e2, e2end, e6, es, e3, e4, e5, f1, oe;
  int aux, nitem, a, j;
  bool fd;

  e0 = AsString(~editTourn);
  if ( IsNull(TrimLeft( e0 )) ) {
      PromptOK( t_("Set a name for the tournament") );
      editTourn.SetFocus();
      return;
  }

  e1 = AsString(~editPlace);
  if ( IsNull(TrimLeft( e1 )) ) {
      PromptOK( t_("Set the place/city of the tournament") );
      editPlace.SetFocus();
      return;
  }

  ehf = AsString(~editFed);
  if ( IsNull(TrimLeft( ehf )) ) {
      PromptOK( t_("Set the Federation FIDE code") );
      editFed.SetFocus();
      return;
  }

  e2 = AsString(~editBeginDate);
  if ( IsNull(TrimLeft( e2 )) ) {
      PromptOK( t_("Set the begin date") );
      return;
  }

  e2end = AsString(~editEndDate);
  if ( IsNull(TrimLeft( e2end )) ) {
      PromptOK( t_("Set the end date") );
      editEndDate.SetFocus();
      return;
  }

  e6 = AsString(~editArbiter);
  if ( IsNull(TrimLeft( e6 ) ) ) {
      PromptOK( t_("Set the name of the arbiter") );
      editArbiter.SetFocus();
      return;
  }

   es = AsString(~editSystem);
   if      ( Contains(es, "Dubov"))   aux = DUBOV; //1;
   else if ( Contains(es, "Robin S")) aux = ROUND_ROBIN; //2;
   else if ( Contains(es, "Vega"))    aux = VEGA; //3;
   else if ( Contains(es, "USCF"))    aux = USCF; //4;
   else if ( Contains(es, "Robin D")) aux = ROUND_ROBIN2; //5;
   else if ( Contains(es, "Lim"))     aux = LIM;

  if (flag==0 && !TD.IS_SWISS && TD.PLAY_SYSTEM != aux && TD.IS_CLOSED_REGISTRATION) { // cannot change a round robin with another
      Exclamation( t_("You cannot change Play system") );
      return;
   }
   else if (flag==0 && TD.IS_SWISS
      && (aux == ROUND_ROBIN || aux == ROUND_ROBIN2) && TD.IS_CLOSED_REGISTRATION) { // cannot change a swiss with a round robin
      Exclamation( t_("You cannot change Play system") );
      return;
   }
   else TD.PLAY_SYSTEM = aux; //new tournament or trying to modify its setting

   a = StrIntValue(AsString(~editRound));

   oe = AsString(~editScore);
   if ( oe == "0 - ½ - 1")  { TD.WON = 1.0; TD.DRAW = 0.5;}
   else if ( oe == "0 - 2 - 5") { TD.WON = 5.0; TD.DRAW = 2.0;}
   else if ( oe == "0 - 1 - 3") { TD.WON = 3.0; TD.DRAW = 1.0;}
   else  { TD.WON = 1.0; TD.DRAW = 0.4;} // "0 -0.4- 5"

   e4 = AsString(~editDir);
   if (  IsNull(e4) ) {
      PromptOK( t_("Set the tournament directory") );
      btnDir.SetFocus();
      return;
   }

   e5 = AsString(~editTFN);
   if ( IsNull(TrimLeft( e5 )) ) {
      PromptOK( t_("Set the Tournament File Name") );
      editTFN.SetFocus();
      return;
   }

   // set the tye break sort priority
   nitem = collist.GetCount();
   for (j=1; j<=nitem; j++) {
       TD.tyebreakorder[j] = StrIntValue ( AsString(collist.Get(j-1, "TB")) );
   }
   for (j=nitem+1; j<=NUM_BTN; j++) TD.tyebreakorder[j] = 0; // set default value ATTENTO AL 10=nitemmax

   fd = ~btnIsFIDE;
   if (flag==0 && TD.IS_FIDE_TOURNAMENT != fd && TD.IS_CLOSED_REGISTRATION) { // avoid change of system when the tourn is started
      Exclamation( t_("Change not permitted") );
      return;
   } else

   TD.IS_FIDE_TOURNAMENT = fd;

   //set values
   TD.NAMETORNEO = e0;
   TD.PLACETORNEO = e1;
   TD.HOSTFEDERATION = ehf;
   TD.DATATORNEO_B = e2;
   TD.DATATORNEO_E = e2end;
   TD.NRounds = a;
   TD.NAccRound = StrIntValue(AsString(~editAccRound));
   TD.PATHDIR = AsString(~editDir);
 			#ifdef PLATFORM_WIN32
            TD.PATHDIR << "\\";
            #endif
            #ifdef PLATFORM_POSIX
            TD.PATHDIR << "/";
		    #endif
   TD.FILETORNEO = e5;
   TD.ARBITER = e6;
   TD.NEW_TORNEO = true;
#ifdef PLATFORM_WIN32
   f1 = TD.PATHDIR + "www" + TD.FILETORNEO;
   //   mkdir(f1);
   CreateDirectory (f1,NULL);
   TD.PATHDIRWWW = TD.PATHDIR + "www" + TD.FILETORNEO + "\\";
#endif
#ifdef PLATFORM_POSIX
    f1 = "mkdir " + TD.PATHDIR + "www" + TD.FILETORNEO;
    system( f1 );
    TD.PATHDIRWWW = TD.PATHDIR + "www" + TD.FILETORNEO + "/";
#endif
   if (flag == 1) { // new tournament
     TD.Reset();
     TD.StartSystem(aux);
     TD.PLAY_SYSTEM = aux;
   }
//Exclamation( AsString(aux) );
   Close();
}


void NewTournamentDlg::ExportDlg()
{ String e0, e1, ehf, e2, e2end, e6, es, e3, e4, e5, f1, oe, eb;
  int aux, nitem, acc, a, i, j, min, max, flagrat, count=0, pl[N_PLAYER_MAX], tb[15];
  double w, d;
  bool fd;

  e0 = AsString(~editTourn);
  if ( IsNull(TrimLeft( e0 )) ) {
      PromptOK( t_("Set a name for the tournament") );
      return;
  }

  e1 = AsString(~editPlace);
  if ( IsNull(TrimLeft( e1 )) ) {
      PromptOK( t_("Set the place/city of the tournament") );
      return;
  }

  ehf = AsString(~editFed);
  if ( IsNull(TrimLeft( ehf )) ) {
      PromptOK( t_("Set the Federation FIDE code") );
      return;
  }

  e2 = AsString(~editBeginDate);
  if ( IsNull(TrimLeft( e2 )) ) {
      PromptOK( t_("Set the begin date") );
      return;
  }

  e2end = AsString(~editEndDate);
  if ( IsNull(TrimLeft( e2end )) ) {
      PromptOK( t_("Set the end date") );
      return;
  }

  e6 = AsString(~editArbiter);
  if ( IsNull(TrimLeft( e6 )) ) {
      PromptOK( t_("Set the name of the arbiter") );
      return;
  }

  es = AsString(~editSystem);
   if      ( Contains(es, "Dubov"))   aux = DUBOV; //1;
   else if ( Contains(es, "Robin S")) aux = ROUND_ROBIN; //2;
   else if ( Contains(es, "Vega"))    aux = VEGA; //3;
   else if ( Contains(es, "USCF"))    aux = USCF; //4;
   else if ( Contains(es, "Robin D")) aux = ROUND_ROBIN2; //5;
   else if ( Contains(es, "Lim"))     aux = LIM;

  es = AsString( ~editFirstScore );
   if      ( Contains(es, "Sum"))   TD.SCORE_TYPE = 1;
   else if ( Contains(es, "Match")) TD.SCORE_TYPE = 2;

  a = StrIntValue(AsString(~editRound));

  oe = AsString(~editScore);
   if ( oe == "0 - ½ - 1")  { w = 1.0; d = 0.5;}
   else if ( oe == "0 - 2 - 5") { w = 5.0; d = 2.0;}
   else if ( oe == "0 - 1 - 3") { w = 3.0; d = 1.0;}
   else    { w = 1.0; d = 0.4;} // "0 -0.4- 5"

  eb = AsString(~editBoards);
   if ( IsNull(eb) )  {
      PromptOK( t_("Set the number of player for each team") );
      return;
  }

  e4 = AsString(~editDir);
   if (  IsNull(e4) ) {
      PromptOK( t_("Set the tournament directory") );
      return;
   }

  e5 = AsString(~editTFN);
   if ( IsNull(TrimLeft( e5 )) ) {
      PromptOK( t_("Set the Tournament File Name") );
      return;
   }

   // set the tye break sort priority
   nitem = collist.GetCount();
   for (j=1; j<=nitem; j++) {
       TD.tyebreakorder[j] = StrIntValue ( AsString(collist.Get(j-1, "TB")) );
   }
   for (j=nitem+1; j<=NUM_BTN; j++) TD.tyebreakorder[j] = 0; // set default value ATTENTO AL 10=nitemmax

   fd = ~btnIsFIDE;
   acc = StrIntValue(AsString(~editAccRound));

#ifdef PLATFORM_WIN32
   e4 << "\\";
#endif
#ifdef PLATFORM_POSIX
   e4 << "/";
#endif

    // retrieve information about what to export
    min = StrIntValue(AsString ( ~edtmin) );
    max = StrIntValue(AsString ( ~edtmax) );
    if (min>max) {
       Exclamation( t_("Set the rating interval") );
       return;
    }
    oe = AsString(~dropRating);
    if ( oe == "National" ) flagrat = 1;
    else if (oe == "FIDE" ) flagrat = 2;
    else flagrat = 3;

   // now starts the export tournament
   String stringa = e4 + e5 + ".veg";
   FileIn in(stringa);
   if ( in.IsOpen() ) {
      in.Close();
      Exclamation( t_("A file with the same name already exist!&Choose another name") );
      return;
  }
#ifdef PLATFORM_WIN32
   f1 = e4 + "www" + TD.FILETORNEO;
   //   mkdir(f1);
   CreateDirectory (f1,NULL);
#endif
#ifdef PLATFORM_POSIX
    f1 = "mkdir " + e4 + "www" + TD.FILETORNEO;
    system( f1 );
#endif

  FileOut out(stringa);
  String s;
  s = VERSION; s <<  " # Do not edit this file!" ;
  out.PutLine( s );
  out.PutLine( e0 );
//  out.PutLine( e1 );
//  out.PutLine( ehf );

  out.PutLine( NFormat("%-20.20s %s", e1, ehf ) );

  out.PutLine( e2 + ", " + e2end);
  out.PutLine( e6 );
  out.PutLine( NFormat("%3.1f  %3.1f", d, w ) );
   // set the tye break sort priority
   nitem = collist.GetCount();
   for (j=1; j<=nitem; j++) {
       tb[j] = StrIntValue ( AsString(collist.Get(j-1, "TB")) );
   }
   for (j=nitem+1; j<=NUM_BTN+1; j++) tb[j] = 0;
   out.PutLine( NFormat(" %d %d %d %d %d %d %d %d %d %d",
         tb[1], tb[2], tb[3], tb[4], tb[5], tb[6], tb[7], tb[8], tb[9], tb[10]) );

  out.PutLine( e5 );
  out.PutLine( NFormat("%d  %d  %d  %d", a, aux, fd, acc) );
  out.PutLine( NFormat("%d", 0) );

  for (i=1; i<=TD.NPlayer; i++) // count player to export
      { if (flagrat==1 && (player[i].ratingnat>=min && player[i].ratingnat<=max) )
            { count++; pl[count]=i; }
        if (flagrat==2 && (player[i].ratingfide>=min && player[i].ratingfide<=max) )
            { count++; pl[count]=i; }
        if ( flagrat==3 &&
             ( (player[i].ratingfide>=min && player[i].ratingfide<=max) ||
               (player[i].ratingnat>=min && player[i].ratingnat<=max)
             ) )
            { count++; pl[count]=i; }
      }
  out.PutLine( NFormat("%d", count) );
  out.PutLine( "NAME;            COUNTRY;BIRTHDAY;G;TITLE;IDFIDE;ELOFIDE; IDNAT;ELONAT;K;ISAVAL");
  for (j=1; j<=count; j++)
      { i = pl[j];
        String s;
        s = NFormat( "%-25.25s;", player[i].name);
        s << NFormat( "%3.3s;", player[i].country);
        s << NFormat( "%8.8s;", player[i].data);
        s << NFormat( "%s;", player[i].sex);
        s << NFormat( "%3.3s;", player[i].codetitle);
        s << NFormat( "%8d;", player[i].idfide);
        s << NFormat( "%5d;", player[i].ratingfide);
        s << NFormat( "%8d;", player[i].idnat);
        s << NFormat( "%5d;", player[i].ratingnat);
        s << NFormat( "%2d;", player[i].kcoeff);
        s << NFormat( "%2s", player[i].isAvailable);
        out.PutLine(s);
  }
  out.Close();
  s = NFormat("Exported %d players", count);
  Exclamation( s );

  Close();
}


void NewTournamentDlg::SetTournamentDir( String dir)
{   String directory;
	FileSel fs;
	fs.dir <<= dir;
	fs.ExecuteSelectDir();
	directory = ~fs;
	if ( !IsNull(directory) ) editDir <<= directory;
	else if ( IsNull(~editDir) ) Exclamation( t_("You have not selected a directory"));
}


void NewTournamentDlg::AddTyebreak(int idbtn)
{  int checkstatus;
   checkstatus = ~opt[idbtn];
   if (checkstatus) { //  button checked: ON
     switch (idbtn)
     { case 1: 	collist.Add("1: Buccholz Cut 1"); break;
       case 2:  collist.Add("2: Buccholz Total"); break;
       case 3: 	collist.Add("3: Buccholz Median"); break;
       case 4: 	collist.Add("4: Sonneborn-Berger"); break;
       case 5: 	collist.Add("5: Cumulative"); break;
       case 6: 	collist.Add("6: Average Rat. Opp."); break;
       case 7: 	collist.Add("7: Most Blacks"); break;
       case 8: 	collist.Add("8: Most Wins"); break;
       case 9: 	collist.Add("9: Av. Perf. Rat. Opp."); break;
     }
   }
   else { // button uncked: OFF
	       for(int i = 0; i < collist.GetCount(); i++)
		       if( Contains(collist.Get(i, 0), AsString(idbtn) ) ) collist.Remove( i );
   }
}
