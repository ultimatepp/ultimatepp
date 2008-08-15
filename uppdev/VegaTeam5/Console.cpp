#include "VegaMain.h"
#include "Tournament.h"

int filter[8][2]; // [][0]=on/off status, [][1]=start column

struct FontListDisplay : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const
	{
		Font fnt = Courier(12); //Font(q, r.Height() - 2);
	//	String txt = Font::GetFaceName(q);
		String txt = AsString(q);
	 	w.DrawRect(r, paper);
		w.DrawText(r.left + 2, r.top + (r.Height() - GetTextSize(txt, fnt).cy) / 2, txt, fnt, ink); // <- Change is here - "w." no more needed
	}
};

struct FontDisplay : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const
	{	Font fnt = Courier(13).Bold(); //Font(q, r.Height() - 2);
		String txt = AsString(q);
	 	w.DrawRect(r, paper);
		w.DrawText(r.left + 2, r.top + (r.Height() - GetTextSize(txt, fnt).cy) / 2, txt, fnt, ink); // <- Change is here - "w." no more needed
	}
};

VegaConsoleFrame::VegaConsoleFrame()
{   int i;
	CtrlLayout(*this);

	AddFrame(ThinOutsetFrame());
//	editBoardPos.SetEditable(false).LeftPosZ(272, 44).TopPosZ(4, 19);
	for (i=1; i<=MAX_BOARDS; i++)  editBoardPos.Add(AsString(i));
	editBoardPos <<= AsString(1);

//now set the window database
	slist.SetDisplay(Single<FontListDisplay>());
	editSearch.Tip(t_("Insert at least 5 characters\nspace included"));
	editSearch.WhenAction = THISBACK(EditSearchCB);
//	slist.WhenLeftDouble = THISBACK(RegisterPlayerCB);
	btnFilter.WhenAction = THISBACK(SetFilterCB);

//now set the array ctrl of the status bar
	console.AddColumn("Time", t_("Time"), 8);
	console.AddColumn("Tournament", t_("Tournament"), 5);
	console.AddColumn("System", t_("System"), 5);
	console.AddColumn("Round", t_("Round"), 3);
	console.AddColumn("Total", t_(" Total, Avail, Bye"), 9).SetDisplay(Single<FontDisplay>());
//	console.AddColumn("Total", t_(" Total, Avail, Bye"), 9);
	console.AddColumn("Action", t_("Action"), 13);
	console.AddColumn("Status", t_("Status"), 13);
	for (i=0; i<7; i++) console.HeaderTab(i).SetInk(SLtRed);
	console.Add(AsString(GetSysTime()), "...", "..." , "0", "  0,  0,  0", t_("Hello!"), t_("Open or Create a tournament!"));
}

void VegaConsoleFrame::SetConsoleDatabse(String fn, int flagDB)
{   filedbname = fn;
    IMPORT_DB = flagDB;
    console.Hide();

    String dbtype;
    if (flagDB == 1) dbtype = t_("import from FIDE db: ");
    if (flagDB == 2) dbtype = t_("import from VEG db: ");
    if (flagDB == 3) dbtype = t_("import from Fixed Length db: ");
    if (flagDB == 4) dbtype = t_("import from FSI db: ");
    if (flagDB == 5) dbtype = t_("import from USCF db (regular): ");
    if (flagDB == 6) dbtype = t_("import from FSI db (quick): ");
    rt.SetQTF( NFormat("[s0;*@6;2 [*@0 %s ]%s]", dbtype, GetFileName(filedbname)) );
	if (IMPORT_DB == 2 || IMPORT_DB == 4 || IMPORT_DB == 5 || IMPORT_DB == 6)
	        btnFilter.Disable(); //no filter for *.veg and FSI format
 #ifdef PLATFORM_WIN32
    if (IMPORT_DB == 1) {
          String s = "";
          SetFilterDlg flt(s);
          flt.LoadFilter(TD.PATHVEGAPRG + "\\filter\\fide.flt"); //default filter for FIDE DB
    }
 #endif
 #ifdef PLATFORM_POSIX
    if (IMPORT_DB == 1) {
          String s = "";
          SetFilterDlg flt(s);
          flt.LoadFilter(TD.PATHVEGAPRG + "/filter/fide.flt"); //default filter for FIDE DB
    }
 #endif
    if (TD.IS_CLOSED_REGISTRATION) PromptOK(t_("Because the registration are closed & you cannot import players & but only browse the DB"));
}
/*
bool VegaConsoleFrame::Key(dword key, int count)
{
    if(key == K_ENTER) {
          if (slist.IsCursor()) RegisterPlayerCB();
           return true; // key accepted
    }
    return TopWindow::Key(key, count);
}
*/

//search the name present in the editSearch widget
void VegaConsoleFrame::EditSearchCB()
{ String pattern = TrimLeft(AsString(~editSearch));
  // if ( IsNull(pattern) ) Exclamation(t_("Cannot find an empty string"));
  if (pattern.GetLength() <5) return;
  bool find = false;
  FileIn in(filedbname);
  slist.Clear();

	while(!in.IsEof()) {
		String line = in.GetLine();
		String s1, s2;
		s1 = ToLower(line);
		s2 = ToLower(pattern);
		if ( s1.Find(s2) >= 0 || pattern == "*****" )
		{
          slist.Add(line);
  		  find = true;
		}
	}
	in.Close();
	slist.Sort( StdValueOrder() );

	if (!find) Exclamation(t_("not found"));
}

void VegaConsoleFrame::SetFilterCB()
{ int i = 0;
  FileIn in(filedbname);
  String testdb="";
  for(i=0;i<=3;i++) { //get the first 3 lines of the batabase to be showed in the spyfilter editor
  	 testdb << "\n" << in.GetLine();
  }
  in.Close();

  SetFilterDlg flt(testdb);
  flt.Execute();//.Run();
}

void VegaConsoleFrame::RegisterPlayerCB( Vector<Value>& playerData )
{
	int i = slist.GetCursor();
	if (i<0 || slist.GetCount()==0) return;
//	if ( TD.NPlayer>TD.MAX_PLAYERS) return;
	if (TD.IS_CLOSED_REGISTRATION && !TD.IS_SWISS) {
	   Exclamation(t_("Option not available for Round Robin tournament") );
	   return;
	}
	if (TD.IS_CLOSED_REGISTRATION) Exclamation(t_("You cannot add player from the DB.&Insert it from Edit Mask"));
    String record = AsString (slist.Get(i));
    switch (IMPORT_DB) {
          case 1: ParsePlayerFIDE(record);          break;
          case 2: ParsePlayerVEG(record);           break;
          case 3: ParsePlayerFixedLenghtDB(record); break;
          case 4: ParsePlayerFSI(record);           break;
          case 5: ParsePlayerUSCFregular(record);   break;
          case 6: ParsePlayerUSCFquick(record);     break;
    }
	slist.Remove(i);
    rt2.SetQTF( NFormat("[s0;*@6;2 %d]", TD.NPlayer) );

      	//build the vestor
  	playerData.Add(play.name);
  	playerData.Add(play.country);
  	playerData.Add(play.data);
  	playerData.Add(play.sex);
  	playerData.Add(play.codetitle);
  	playerData.Add( AsString(play.idfide) );
   	playerData.Add( AsString(play.ratingfide) );
 	playerData.Add( AsString(play.idnat) );
  	playerData.Add( AsString(play.ratingnat) );
  	playerData.Add(play.kcoeff);
  	playerData.Add(play.isAvailable);
}


void VegaConsoleFrame::ParsePlayerFSI(String playerstr)
{ PlayerArchive player;
  if (TD.NPlayer < TD.MAX_PLAYERS) {
    int i = TD.NPlayer + 1;
    Vector<String> field = Split( playerstr, ';', false);
    if (field.GetCount()>9 || field.GetCount()<9) {
       Exclamation( t_("record") + playerstr + t_("& in the wrong format!&check the DB"));
       return;
    }
    play.name = TrimLeft(field[0] ); // remove leading white spaces
    play.ratingnat = StrIntValue( field[1] );
    play.codetitle = TrimLeft(field[2] );
    //play.title = player[i].TitleToCode();
    play.kcoeff = StrIntValue( field[3] );

    if (play.kcoeff != 0)  play.ratingfide = 0;
    else  play.ratingfide = play.ratingnat;

    play.country = TrimLeft(field[4] );
    play.data = TrimLeft(field[5] );
    play.idfide = StrIntValue( field[6] );
    play.idnat = StrIntValue( field[7] );
    play.sex =  TrimLeft( field[8] ) ;
    if ( play.sex!="m" && play.sex!="f") play.sex="m";
    play.isAvailable = "1" ; // by default

  	//TD.NPlayer++;
  }
  else Exclamation( t_("exceeded maximum number of players"));
}

void VegaConsoleFrame::ParsePlayerVEG(String playerstr)
{
  if (TD.NPlayer < TD.MAX_PLAYERS) {
    int i = TD.NPlayer + 1;
    Vector<String> field = Split( playerstr, ';', false);
    if ( /*field.GetCount()>10 ||*/ field.GetCount()<10) {
       Exclamation( t_("record") + playerstr + t_("& in the wrong format!&check the DB"));
       return;
    }
    // for (int j=0; j<field.GetCount(); j++) {
	    play.name = TrimLeft(field[0] ); // remove leading white spaces
 	    play.country = TrimLeft(field[1] );
	    play.data = TrimLeft(field[2] );
	    play.sex =  TrimLeft( field[3] ) ;
	    play.codetitle = TrimLeft(field[4] );
	    //play.title = play.TitleToCode();
 	    play.idfide = StrIntValue( field[5] );
	    play.ratingfide = StrIntValue( field[6] );
	    play.idnat = StrIntValue( field[7] );
	    play.ratingnat = StrIntValue( field[8] );
	    play.kcoeff = StrIntValue( field[9] );
	    play.isAvailable = "1" ; // by default
    // }
  	//TD.NPlayer++;
  }
  else Exclamation( t_("exceeded maximum number of players"));
}


void VegaConsoleFrame::ParsePlayerFIDE(String playerstr)
{
  String e2;
  if (TD.NPlayer < TD.MAX_PLAYERS) {
       int i = TD.NPlayer + 1;
       play.name = ""; //to remove the name BYE
       if ( filter[FNAME][0] ) {
           // rimuove la ',' nel nome dal DB FIDE che crea problemi al rapporto elo
           String name = TrimRight( playerstr.Mid( filter[FNAME][1], 25) );
           Vector<String> field = Split( name, ',');
           for (int j=0; j<field.GetCount(); j++) play.name << field[j];
       }
       if ( filter[FRATING][0] )
              play.ratingfide = StrIntValue(playerstr.Mid( filter[FRATING][1], 4));
       else play.ratingfide = 0;
       if ( filter[FTITLE][0] ) {
          e2 = playerstr.Mid( filter[FTITLE][1], 2); //title
          if ( e2 == "g " || e2 == " g" ) e2 = "GM";
          else if ( e2 == "wg" )  e2 == "WGM";
          else if ( e2 == " m"   ||  e2 == "m " )  e2 = "IM";
          else if ( e2 == "wm")  e2 = "WIM";
          else if ( e2 == "f " ||  e2 == " f")  e2 = "FM";
          else if ( e2 == "wf" )  e2 = "WFM";
          else if ( e2 == "c " ||  e2 == " c")  e2 = "CM";
          else e2 = "--";
          play.codetitle = e2;
       }
       else  play.codetitle = "--";
	    // play.title = play.TitleToCode();

  	   //if ( filter[FKCOEFF][0] )  //not imported in FIDE DB
  	   play.kcoeff = 0;
  	   if ( filter[FCOUNTRY][0] )
             play.country = playerstr.Mid( filter[FCOUNTRY][1], 3);
  	   if ( filter[FBIRTH][0] )
             play.data = playerstr.Mid( filter[FBIRTH][1], 4);
 	   if ( filter[FIDCODE][0] )
             play.idfide = StrIntValue(playerstr.Mid( filter[FIDCODE][1], 8));
 	   else play.idfide = 0;
 	   play.sex = "m"; //default
 	   if ( filter[FGENDER][0] ) {
          e2 = playerstr.Mid( filter[FGENDER][1], 1);
          if (e2 == " " || e2 == "m" || e2 == "M") play.sex = "m";
 	      else if (e2 == "w" || e2 == "W")	 play.sex = "f";
 	   }

   	   //default
  	   play.kcoeff = play.idnat = play.ratingnat = 0;
 	   play.isAvailable = "1" ;
 	   //TD.NPlayer++;
   }
   else Exclamation( t_("exceeded maximum number of players"));
}

void VegaConsoleFrame::ParsePlayerFixedLenghtDB(String playerstr)
{
  String e2;

  if (TD.NPlayer < TD.MAX_PLAYERS) {
         int i = TD.NPlayer + 1;
         if ( filter[FNAME][0] )  // it always true
              play.name = TrimRight( playerstr.Mid( filter[FNAME][1], 25) );

         if ( filter[FRATING][0] )
              play.ratingnat = StrIntValue(playerstr.Mid( filter[FRATING][1], 4));
         else play.ratingnat = 0;

         if ( filter[FTITLE][0] ) {
           e2 = playerstr.Mid( filter[FTITLE][1], 2); //title
           if ( e2 == "  " ) e2 = "--";
           play.codetitle = e2;
         }
         else play.codetitle = "--";
	     //play.title = play.TitleToCode();

  	 	 if ( filter[FKCOEFF][0] ) play.kcoeff = StrIntValue(playerstr.Mid( filter[FKCOEFF][1], 2));
		 else play.kcoeff = 0;

  		 if ( filter[FCOUNTRY][0] ) play.country = playerstr.Mid( filter[FCOUNTRY][1], 2);
  		 if ( filter[FBIRTH][0] ) play.data = playerstr.Mid( filter[FBIRTH][1], 8);
 		 if ( filter[FIDCODE][0] ) play.idnat = StrIntValue(playerstr.Mid( filter[FIDCODE][1], 8));
         else play.idnat = 0;
 	     play.sex = "m"; //default
 	     if ( filter[FGENDER][0] ) {
            e2 = playerstr.Mid( filter[FGENDER][1], 1);
            if (e2 == " " || e2 == "m" || e2 == "M") play.sex = "m";
 	        else if (e2 == "w" || e2 == "W")	 play.sex = "f";
 	     }
         //default
  		 play.kcoeff = play.idfide = play.ratingfide = 0;
 	     play.isAvailable = "1" ;
 	     //TD.NPlayer++;
  }
  else Exclamation( t_("exceeded maximum number of players"));
}

void VegaConsoleFrame::ParsePlayerUSCFregular(String playerstr)
{
  if (TD.NPlayer < TD.MAX_PLAYERS) {
    int i = TD.NPlayer + 1;
    Vector<String> field = Split( playerstr, '\t', false);
    if ( field.GetCount()<6) {
       Exclamation( t_("record") + playerstr + t_("& in the wrong format!&check the DB"));
       return;
    }
    // for (int j=0; j<field.GetCount(); j++) {
	    play.name = TrimLeft(field[0] ); // remove leading white spaces
 	    play.country = TrimLeft(field[3] );
	    play.data = "000000";
	    play.sex =  "m";
	    play.codetitle = "--";
	    play.title = 0;
 	    play.idfide = 0;
	    play.ratingfide = 0;
	    play.idnat = StrIntValue( field[1] );
	    play.ratingnat = StrIntValue( field[4] );
	    play.kcoeff = 0;
	    play.isAvailable = "1" ; // by default
    // }
  	//TD.NPlayer++;
  }
  else Exclamation( t_("exceeded maximum number of players"));
}

void VegaConsoleFrame::ParsePlayerUSCFquick(String playerstr)
{
  if (TD.NPlayer < TD.MAX_PLAYERS) {
    int i = TD.NPlayer + 1;
    Vector<String> field = Split( playerstr, '\t', false);
    if ( field.GetCount()<6) {
       Exclamation( t_("record") + playerstr + t_("& in the wrong format!&check the DB"));
       return;
    }
    // for (int j=0; j<field.GetCount(); j++) {
	    play.name = TrimLeft(field[0] ); // remove leading white spaces
 	    play.country = TrimLeft(field[3] );
	    play.data = "000000";
	    play.sex =  "m";
	    play.codetitle = "--";
	    play.title = 0;
 	    play.idfide = 0;
	    play.ratingfide = 0;
	    play.idnat = StrIntValue( field[1] );
	    play.ratingnat = StrIntValue( field[5] );
	    play.kcoeff = 0;
	    play.isAvailable = "1" ; // by default
    // }
  	//TD.NPlayer++;
  }
  else Exclamation( t_("exceeded maximum number of players"));
}
