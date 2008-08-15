#include "VegaMain.h"
#include "Tournament.h"

extern int filter[8][2]; // [][0]=on/off status, [][1]=start column

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


ConnectDBDlg::ConnectDBDlg(String fn, int flagDB)
{
 	CtrlLayout(*this, t_("Import Player from Database"));
    filedbname = fn;

    IMPORT_DB = flagDB;
    String dbtype;
    if (flagDB == 1) dbtype = t_("import from FIDE db: ");
    if (flagDB == 2) dbtype = t_("import from VEG db: ");
    if (flagDB == 3) dbtype = t_("import from Fixed Length db: ");
    if (flagDB == 4) dbtype = t_("import from FSI db: ");
    if (flagDB == 5) dbtype = t_("import from USCF db (regular): ");
    if (flagDB == 6) dbtype = t_("import from FSI db (quick): ");
    rt.SetQTF( NFormat("[s0;*@6;2 [*@0 %s ]%s]", dbtype, GetFileName(filedbname)) );
//	lblbox.SetLabel(dbtype);
	slist.SetDisplay(Single<FontListDisplay>());
	slist2.SetDisplay(Single<FontListDisplay>());
	btnExit <<= THISBACK(CloseDlgCB);
	editSearch.Tip(t_("Insert at least 5 characters\nspace included"));
	editSearch.WhenAction = THISBACK(EditSearchCB);
	slist.WhenLeftDouble = THISBACK(MoveItemCB);
	btnRemove.WhenAction = THISBACK(RemoveItemCB);
	btnReg.WhenAction = THISBACK(RegisterPlayerCB);
	btnFilter.WhenAction = THISBACK(SetFilterCB);
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


bool ConnectDBDlg::Key(dword key, int count)
{
    if(key == K_ENTER) {
           if (slist.IsCursor()) MoveItemCB();
           return true; // key accepted
    }
    return TopWindow::Key(key, count);
}


//search the name present in the editSearch widget
void ConnectDBDlg::EditSearchCB()
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

void ConnectDBDlg::SetFilterCB()
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

void ConnectDBDlg::RegisterPlayerCB()
{   int i;
	if (TD.IS_CLOSED_REGISTRATION && !TD.IS_SWISS) {
	   Exclamation(t_("Option not available for Round Robin tournament") );
	   return;
	}
	if (TD.IS_CLOSED_REGISTRATION) Exclamation(t_("You cannot add player from the DB.&Insert it from Edit Mask"));
	if (slist2.GetCount() + TD.NPlayer>TD.MAX_PLAYERS) {
	      //String s = NFormat("exceded maximum number of players");
	      Exclamation(t_("exceeded maximum number of players.&Please remove at least ") +
	                    AsString(slist2.GetCount() + TD.NPlayer - TD.MAX_PLAYERS ) + t_(" players"));
	      return;
	}
    for (i=0; i< slist2.GetCount(); i++) {
        String record = AsString (slist2.Get(i));
        switch (IMPORT_DB)
        {
          case 1: ParsePlayerFIDE(record);          break;
          case 2: ParsePlayerVEG(record);           break;
          case 3: ParsePlayerFixedLenghtDB(record); break;
          case 4: ParsePlayerFSI(record);           break;
          case 5: ParsePlayerUSCFregular(record);   break;
          case 6: ParsePlayerUSCFquick(record);     break;
        }
    }
    CloseDlgCB();
}

//move item from slist to slist2
void ConnectDBDlg::MoveItemCB()
{   int n = slist.GetCursor();
	if (n<0) return;
	slist2.Add( slist.Get(n));
	slist.Remove(n);
	lbl.SetLabel( AsString(slist2.GetCount()) );
}

//remove an item from slist2
void ConnectDBDlg::RemoveItemCB()
{   int n = slist2.GetCursor();
	if (n<0) return;
	slist2.Remove(n);
	lbl.SetLabel( AsString(slist2.GetCount()) );
}

void ConnectDBDlg::ParsePlayerFSI(String playerstr)
{
  if (TD.NPlayer < TD.MAX_PLAYERS) {
    int i = TD.NPlayer + 1;
    Vector<String> field = Split( playerstr, ';', false);
    if (field.GetCount()>9 || field.GetCount()<9) {
       Exclamation( t_("record") + playerstr + t_("& in the wrong format!&check the DB"));
       return;
    }
    player[i].name = TrimLeft(field[0] ); // remove leading white spaces
    player[i].ratingnat = StrIntValue( field[1] );
    player[i].codetitle = TrimLeft(field[2] );
    player[i].title = player[i].TitleToCode();
    player[i].kcoeff = StrIntValue( field[3] );

    if (player[i].kcoeff != 0)  player[i].ratingfide = 0;
    else  player[i].ratingfide = player[i].ratingnat;

    player[i].country = TrimLeft(field[4] );
    player[i].data = TrimLeft(field[5] );
    player[i].idfide = StrIntValue( field[6] );
    player[i].idnat = StrIntValue( field[7] );
    player[i].sex =  TrimLeft( field[8] ) ;
    if ( player[i].sex!="m" && player[i].sex!="f") player[i].sex="m";
    player[i].isAvailable = "1" ; // by default
  	TD.NPlayer++;
  }
  else Exclamation( t_("exceeded maximum number of players"));
}

void ConnectDBDlg::ParsePlayerVEG(String playerstr)
{
  if (TD.NPlayer < TD.MAX_PLAYERS) {
    int i = TD.NPlayer + 1;
    Vector<String> field = Split( playerstr, ';', false);
    if ( /*field.GetCount()>10 ||*/ field.GetCount()<10) {
       Exclamation( t_("record") + playerstr + t_("& in the wrong format!&check the DB"));
       return;
    }
    // for (int j=0; j<field.GetCount(); j++) {
	    player[i].name = TrimLeft(field[0] ); // remove leading white spaces
 	    player[i].country = TrimLeft(field[1] );
	    player[i].data = TrimLeft(field[2] );
	    player[i].sex =  TrimLeft( field[3] ) ;
	    player[i].codetitle = TrimLeft(field[4] );
	    player[i].title = player[i].TitleToCode();
 	    player[i].idfide = StrIntValue( field[5] );
	    player[i].ratingfide = StrIntValue( field[6] );
	    player[i].idnat = StrIntValue( field[7] );
	    player[i].ratingnat = StrIntValue( field[8] );
	    player[i].kcoeff = StrIntValue( field[9] );
	    player[i].isAvailable = "1" ; // by default
    // }
  	TD.NPlayer++;
  }
  else Exclamation( t_("exceeded maximum number of players"));
}


void ConnectDBDlg::ParsePlayerFIDE(String playerstr)
{
  String e2;
  if (TD.NPlayer < TD.MAX_PLAYERS) {
       int i = TD.NPlayer + 1;
       player[i].name = ""; //to remove the name BYE
       if ( filter[FNAME][0] ) {
           // rimuove la ',' nel nome dal DB FIDE che crea problemi al rapporto elo
           String name = TrimRight( playerstr.Mid( filter[FNAME][1], 25) );
           Vector<String> field = Split( name, ',');
           for (int j=0; j<field.GetCount(); j++) player[i].name << field[j];
       }
       if ( filter[FRATING][0] )
              player[i].ratingfide = StrIntValue(playerstr.Mid( filter[FRATING][1], 4));
       else player[i].ratingfide = 0;
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
          player[i].codetitle = e2;
       }
       else  player[i].codetitle = "--";
	     player[i].title = player[i].TitleToCode();

  	   //if ( filter[FKCOEFF][0] )  //not imported in FIDE DB
  	   player[i].kcoeff = 0;
  	   if ( filter[FCOUNTRY][0] )
             player[i].country = playerstr.Mid( filter[FCOUNTRY][1], 3);
  	   if ( filter[FBIRTH][0] )
             player[i].data = playerstr.Mid( filter[FBIRTH][1], 4);
 	   if ( filter[FIDCODE][0] )
             player[i].idfide = StrIntValue(playerstr.Mid( filter[FIDCODE][1], 8));
 	   else player[i].idfide = 0;
 	   player[i].sex = "m"; //default
 	   if ( filter[FGENDER][0] ) {
          e2 = playerstr.Mid( filter[FGENDER][1], 1);
          if (e2 == " " || e2 == "m" || e2 == "M") player[i].sex = "m";
 	      else if (e2 == "w" || e2 == "W")	 player[i].sex = "f";
 	   }

   	   //default
  	   player[i].kcoeff = player[i].idnat = player[i].ratingnat = 0;
 	   player[i].isAvailable = "1" ;
 	   TD.NPlayer++;
   }
   else Exclamation( t_("exceeded maximum number of players"));
}

void ConnectDBDlg::ParsePlayerFixedLenghtDB(String playerstr)
{
  String e2;

  if (TD.NPlayer < TD.MAX_PLAYERS) {
         int i = TD.NPlayer + 1;
         if ( filter[FNAME][0] )  // it always true
              player[i].name = TrimRight( playerstr.Mid( filter[FNAME][1], 25) );

         if ( filter[FRATING][0] )
              player[i].ratingnat = StrIntValue(playerstr.Mid( filter[FRATING][1], 4));
         else player[i].ratingnat = 0;

         if ( filter[FTITLE][0] ) {
           e2 = playerstr.Mid( filter[FTITLE][1], 2); //title
           if ( e2 == "  " ) e2 = "--";
           player[i].codetitle = e2;
         }
         else player[i].codetitle = "--";
	     player[i].title = player[i].TitleToCode();

  	 	 if ( filter[FKCOEFF][0] ) player[i].kcoeff = StrIntValue(playerstr.Mid( filter[FKCOEFF][1], 2));
		 else player[i].kcoeff = 0;

  		 if ( filter[FCOUNTRY][0] ) player[i].country = playerstr.Mid( filter[FCOUNTRY][1], 2);
  		 if ( filter[FBIRTH][0] ) player[i].data = playerstr.Mid( filter[FBIRTH][1], 8);
 		 if ( filter[FIDCODE][0] ) player[i].idnat = StrIntValue(playerstr.Mid( filter[FIDCODE][1], 8));
         else player[i].idnat = 0;
 	     player[i].sex = "m"; //default
 	     if ( filter[FGENDER][0] ) {
            e2 = playerstr.Mid( filter[FGENDER][1], 1);
            if (e2 == " " || e2 == "m" || e2 == "M") player[i].sex = "m";
 	        else if (e2 == "w" || e2 == "W")	 player[i].sex = "f";
 	     }
         //default
  		 player[i].kcoeff = player[i].idfide = player[i].ratingfide = 0;
 	     player[i].isAvailable = "1" ;
 	     TD.NPlayer++;
  }
  else Exclamation( t_("exceeded maximum number of players"));
}

void ConnectDBDlg::ParsePlayerUSCFregular(String playerstr)
{
  if (TD.NPlayer < TD.MAX_PLAYERS) {
    int i = TD.NPlayer + 1;
    Vector<String> field = Split( playerstr, '\t', false);
    if ( field.GetCount()<6) {
       Exclamation( t_("record") + playerstr + t_("& in the wrong format!&check the DB"));
       return;
    }
    // for (int j=0; j<field.GetCount(); j++) {
	    player[i].name = TrimLeft(field[0] ); // remove leading white spaces
 	    player[i].country = TrimLeft(field[3] );
	    player[i].data = "000000";
	    player[i].sex =  "m";
	    player[i].codetitle = "--";
	    player[i].title = 0;
 	    player[i].idfide = 0;
	    player[i].ratingfide = 0;
	    player[i].idnat = StrIntValue( field[1] );
	    player[i].ratingnat = StrIntValue( field[4] );
	    player[i].kcoeff = 0;
	    player[i].isAvailable = "1" ; // by default
    // }
  	TD.NPlayer++;
  }
  else Exclamation( t_("exceeded maximum number of players"));
}

void ConnectDBDlg::ParsePlayerUSCFquick(String playerstr)
{
  if (TD.NPlayer < TD.MAX_PLAYERS) {
    int i = TD.NPlayer + 1;
    Vector<String> field = Split( playerstr, '\t', false);
    if ( field.GetCount()<6) {
       Exclamation( t_("record") + playerstr + t_("& in the wrong format!&check the DB"));
       return;
    }
    // for (int j=0; j<field.GetCount(); j++) {
	    player[i].name = TrimLeft(field[0] ); // remove leading white spaces
 	    player[i].country = TrimLeft(field[3] );
	    player[i].data = "000000";
	    player[i].sex =  "m";
	    player[i].codetitle = "--";
	    player[i].title = 0;
 	    player[i].idfide = 0;
	    player[i].ratingfide = 0;
	    player[i].idnat = StrIntValue( field[1] );
	    player[i].ratingnat = StrIntValue( field[5] );
	    player[i].kcoeff = 0;
	    player[i].isAvailable = "1" ; // by default
    // }
  	TD.NPlayer++;
  }
  else Exclamation( t_("exceeded maximum number of players"));
}

void ConnectDBDlg::CloseDlgCB()
{
	Close();
}
