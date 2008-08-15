#include "VegaMain.h"
#include "Tournament.h"

#define IMAGECLASS    Vega5Img
#define IMAGEFILE     <VegaTeam5/Vega.iml>
#include <Draw/iml_source.h>

void VegaMain::VegaMainMenu(Bar& menu)
{
	menu.Add(t_("File"), THISBACK(File) ).Key(K_CTRL_F);
//	menu.Add(t_("Extras"), THISBACK(Extras) ).Key(K_CTRL_X);
//	menu.Add(t_("Report"), THISBACK(Report) ).Key(K_CTRL_R);
//	menu.Add(t_("Help"), THISBACK(Help) ).Key(K_CTRL_H);
}


// from file
void VegaMain::File(Bar& bar)
{
	bar.Add(t_("New Tournament"), Vega5Img::newTourn(), THISBACK(NewTournament)).Key(K_CTRL_N);
	bar.Add(t_("Open Tournament"), CtrlImg::open(), THISBACK(OpenTournament)).Key(K_CTRL_O);
	bar.Add(t_("Save Tournament"), CtrlImg::save(), THISBACK(SaveTournament)).Key(K_CTRL_S);
	bar.Separator();
//	bar.Add(t_("Export"), THISBACK(Export));
//	bar.Add(t_("Export"), THISBACK(Export));
	bar.Add(t_("Connect Database"), Vega5Img::registered(), THISBACK(SelectDbase));
	bar.Separator();
	bar.Add(t_("Dangerous Stuff!"), Vega5Img::danger(), THISBACK(DangerousStuff));
	bar.Separator();
    bar.Add(t_("Exit"), THISBACK(Exit)).Key(K_CTRL_E);
}

void VegaMain::SelectDbase(Bar& bar)
{
	bar.Add(t_("archive FIDE"), Vega5Img::fide(), THISBACK1(ConnectDatabase, 1));
	bar.Add(t_("archive FSI Italy"), Vega5Img::ita(), THISBACK1(ConnectDatabase, 4));
	bar.Add(t_("archive USCF Regular"), Vega5Img::uscf(), THISBACK1(ConnectDatabase, 5));
	bar.Add(t_("archive USCF Quick"),  Vega5Img::uscf(), THISBACK1(ConnectDatabase, 6));
	bar.Add(t_("archive VEG"), THISBACK1(ConnectDatabase, 2));
	bar.Add(t_("national archive with Fixed Length"), THISBACK1(ConnectDatabase, 3));
}


void VegaMain::Export(Bar& bar)
{
	bar.Add(t_("All Players"), THISBACK(ExportAllPlayers));
	bar.Add(t_("Tournament"), THISBACK(ExportTournament));
	bar.Add(t_("Selected Player"), THISBACK(ExportPlayer));
}


void VegaMain::DangerousStuff(Bar& bar)
{
	bar.Add(t_("Modify Tournament"), THISBACK(ModifyTournament));
	bar.Add(t_("Load Round"), THISBACK(LoadBackupRoundCB));
	bar.Add(t_("Reset Tournament"), THISBACK(ResetTournamentCB));
	bar.Add(t_("Delete Round"), THISBACK(DeleteRoundCB));
	bar.Add(t_("Reopen Registration"), THISBACK(ReopenRegistrationCB));
}


void VegaMain::NewTournament()
{   if (TD.IS_TORNEO_STARTED) {
       if (!PromptYesNo( t_("[s0;=*@6 All tournament data will be lost &(but you can keep the players).&][s0;=*@6 Do you want to continue?]") ) ) return;
    }
    TD.NEW_TORNEO = false;
    NewTournamentDlg newt(1);
	newt.Run();   // collect new information to set a new tournament
    if (TD.NEW_TORNEO) {
	    if (tab1.arr.GetCount()) {
	        if (!PromptYesNo( t_("Do you want to keep& the players currently registered?") ) )
               tab1.arr.Clear(); //remove all the previous data
  	    }
  	    tab3.arrp.Clear(); // clear roundManager
  	    SaveTournament(); //save what we have cretead
        UpdateInfo("", "", t_("Created new tournament"));
        this->Title(TD.FILETORNEO + " - " + VERSION);
//        tab1.arr.Removing(true); //players can be deleted
        tab1.arr.Moving(true); //players can be moved
	}
}

void VegaMain::ExportTournament()
{ if (TD.NPlayer<1) {
      Exclamation( t_("There are no players to export"));
      return;  // nothing to retrive
  }
  NewTournamentDlg newt(2);
  newt.Run();
}

void VegaMain::ExportAllPlayers()
{ // export in a file the players in the listPlayer
  int i;
  if (TD.NPlayer<1) {
      Exclamation( t_("There are no players to export"));
      return;  // nothing to retrive
  }
  FileSel fs; // for Linux and windows
  String fn;
  fs.Type( t_("File vega"), "*.csv").DefaultExt("csv");
//  if(fs.ExecuteOpen()) { // file selected
  if( fs.ExecuteSaveAs() ) { // file selected
	  fn = ~fs;
  } else return;
  DataFromArrayToArchive();

  FileOut out(fn);
  out.PutLine( "NAME;            COUNTRY;BIRTHDAY;G;TITLE;IDFIDE;ELOFIDE; IDNAT;ELONAT;K;ISAVAL");
  for (i=1; i<=TD.NPlayer; i++)
      { String s;
        s = NFormat( "%-25.25s;", player[i].name);
        s << NFormat( "%3.3s;", player[i].country);
        s << NFormat( "%8.8s;", player[i].data);
        s << NFormat( "%s;", player[i].sex);
        s << NFormat( "%3.3s;", player[i].codetitle);
        s << NFormat( "%8d;", player[i].idfide);
        s << NFormat( "%5d;", player[i].ratingfide);
        s << NFormat( "%8d;", player[i].idnat);
        s << NFormat( "%5d;", player[i].ratingnat);
        s << NFormat( "%2d", player[i].kcoeff);
        out.PutLine(s);
     }
  out.Close();
}

void VegaMain::ExportPlayer()
// copy the selected player from the current section to another tournament
// the player is not removed from the current section
{ String s, line;
  int i;
  i = tab1.arr.GetCursor();
  if (i<0) {
      Exclamation( t_("There are no players to export"));
      return;  // nothing to retrive
  }


  FileSel fs; // for Linux and windows
  String fn;
  // select the file *.VEG where to move the player
  fs.Set(t_("Set destination .VEG file"));
  fs.Type( t_("File vega"), "*.veg").DefaultExt("veg");
  if(fs.ExecuteOpen())  fn = ~fs;
  else return;

  // get the player
  s = AsString( tab1.arr.Get( i, 1 ) ) + ";";
  s << AsString( tab1.arr.Get( i, 2 ) ) + ";";
  s << AsString( tab1.arr.Get( i, 3 ) ) + ";";
  s << AsString( tab1.arr.Get( i, 4 ) ) + ";";
  s << AsString( tab1.arr.Get( i, 5 ) ) + ";";
  s << AsString( tab1.arr.Get( i, 6 ) ) + ";";
  s << AsString( tab1.arr.Get( i, 7 ) ) + ";";
  s << AsString( tab1.arr.Get( i, 8 ) ) + ";";
  s << AsString( tab1.arr.Get( i, 9 ) ) + ";";
  s << AsString( tab1.arr.Get( i, 10 ) ) + ";";
  s << AsString( tab1.arr.Get( i, 11 ) ) ;

  // confirm?
  //  sprintf(mess, "Are you sure to move %s\nin the VEG file [%s] ?\n\n(Close the Vega session that is eventually using\n that file before to proceed!)", str, filename);
  //  if (!QuestionMessage( mess, sss[S_NO], sss[S_YES], NULL) ) return;

  // do a backup copy of the veg file with extension vg2
  String f = fn + "b";
  FileCopy(fn, f);

  // now produce a new veg file
  FileIn in(f);
  FileOut out(fn);

  for(i=1; i<=10; i++ ) {
        line = in.GetLine();
        out.PutLine(line);
  }
  line = in.GetLine();
  int countplayers = StrIntValue(line);
  out.PutLine( NFormat("%3d", countplayers + 1) );

  for(i=1; i<=countplayers+1; i++) { // +1 refer to the headre line
       line = in.GetLine();
       out.PutLine(line);
  }
  in.Close();
  out.PutLine(s);
  out.Close();
  FileDelete(f);
  Exclamation( t_("The player has been copied") );
}


void VegaMain::SaveTournament()
{
 //  if (TD.NPlayer<1) { Exclamation( t_("Add players") ); return; } // nothing to save

   if (TD.IS_CLOSED_REGISTRATION) { // we must save both VEG and RND file
      //if (!TD.IS_VEG_SAVED) { // save the VEG file as it wasn't saved
      	tab1.SortRecord(0); //before to save sort the table for ID
  	  	DataFromArrayToArchive();
      	TD.SaveVegFile(true);
     // }
      // the RND file is saved always
      if (TD.IS_SWISS) TD.SaveRound(0);
      else TD.SaveRoundRR(0);
   }
   else { // we must save only the VEG file
     //if (!TD.IS_VEG_SAVED) {
      	tab1.SortRecord(0); //before to save sort the table for ID
  	  	DataFromArrayToArchive();
      	TD.SaveVegFile(true);
      //}
  }
  UpdateInfo("vcvx", "czzxc", t_("Saved tournament data"));
}


void VegaMain::DataFromArrayToArchive()
{ int i;
//FileOut save(TD.PATHDIR + "dump.txt");
  TD.NPlayer = tab1.arr.GetCount();
  for(i=1; i<=tab1.arr.GetCount(); i++) {
      //player[] start from 1, while the array start from 0
      //the column 'ID' is skipped
	  player[i].name        = AsString( tab1.arr.Get( i-1, 1 ) );
      player[i].country     = AsString( tab1.arr.Get( i-1, 2 ) );
      player[i].data        = AsString( tab1.arr.Get( i-1, 3 ) );
      player[i].sex         = AsString( tab1.arr.Get( i-1, 4 ) );
      player[i].codetitle   = AsString( tab1.arr.Get( i-1, 5 ) );
	  player[i].title = player[i].TitleToCode();
      player[i].idfide      = StrIntValue (AsString( tab1.arr.Get( i-1, 6 ) ) );
      player[i].ratingfide  = StrIntValue (AsString( tab1.arr.Get( i-1, 7 ) ) );
      player[i].idnat       = StrIntValue (AsString( tab1.arr.Get( i-1, 8 ) ) );
      player[i].ratingnat   = StrIntValue (AsString( tab1.arr.Get( i-1, 9 ) ) );
      player[i].kcoeff      = StrIntValue (AsString( tab1.arr.Get( i-1, 10 ) ) );
      player[i].isAvailable = AsString( tab1.arr.Get( i-1, 11 ) ) ;

//save << Format("%-20.20s -\n", player[i].name);

      // set the rating used during the tournament
      if (TD.IS_FIDE_TOURNAMENT) {
           player[i].RATING = player[i].ratingfide;
           if (player[i].ratingfide == 0) player[i].RATING = player[i].ratingnat;
      }
      else {
           player[i].RATING = player[i].ratingnat;
           if (player[i].ratingnat == 0) player[i].RATING = player[i].ratingfide;
      }
  }

//save.Close();
}


void VegaMain::DataFromArchiveToArray(int fromPlayer, int toPlayer)
{ int i;
  if (!TD.IS_CLOSED_REGISTRATION) {
      for (i=fromPlayer; i<=toPlayer; i++)
      {
	        tab1.arr.Add( 1, player[i].name , player[i].country, player[i].data, player[i].sex,
	            player[i].codetitle, player[i].idfide, player[i].ratingfide, player[i].idnat,
	            player[i].ratingnat, player[i].kcoeff, player[i].isAvailable);
      }
   }
}

void VegaMain::DataFromArchiveToArray(int board, Vector<Value>& v)
{
  if (!TD.IS_CLOSED_REGISTRATION) {
	        tab1.arrPlayer.Set(board, v);}
}

void VegaMain::DataFromArchiveToArray()
{ int i;
  tab1.arr.Clear(); //clear the arrayctrl
  if (!TD.IS_CLOSED_REGISTRATION && !TD.IS_SAVED_ROUND) {
     for (i=1; i<=TD.NPlayer; i++) {
	        tab1.arr.Add( 1, player[i].name , player[i].country, player[i].data, player[i].sex,
	            player[i].codetitle, player[i].idfide, player[i].ratingfide, player[i].idnat,
	            player[i].ratingnat, player[i].kcoeff, player[i].isAvailable);
     }
   }
   else {
      for (i=1; i<=TD.NPlayer; i++) {
	        tab1.arr.Add( i, player[i].name , player[i].country, player[i].data, player[i].sex,
	            player[i].codetitle, player[i].idfide, player[i].ratingfide, player[i].idnat,
	            player[i].ratingnat, player[i].kcoeff, player[i].isAvailable);
      }
   }
}


void VegaMain::OpenTournament()
{   //FileSelector fs; //for Windows style
	FileSel fs; // for Linux and windows
	String fn, dir;
	dir = TD.PATHDIR; // it has a final slash that should be removed
    if ( dir.Right(1) == "\\" || dir.Right(1) == "/" )
         dir.Remove( dir.GetLength() - 1, 1); // remove the final '\' or '/'
	fs.dir <<= dir;

    if (TD.IS_TORNEO_STARTED) {
       if (!PromptYesNo( t_("[s0;=*@6 All your data will be lost.&][s0;=*@6 Do you want to continue?]") ) ) return;
    }
	fs.Type( t_("File vega"), "*.veg").DefaultExt("veg");
	if(fs.ExecuteOpen()) { // file selected
			  TD.PATHDIR = fs.GetActiveDir();
 #ifdef PLATFORM_WIN32
              TD.PATHDIR << "\\";
 #endif
 #ifdef PLATFORM_POSIX
              TD.PATHDIR << "/";
 #endif
			fn = ~fs;
			//	FilePath(fn);
	}
	else return;
    ReadVegFile(fn);
 #ifdef PLATFORM_WIN32
              TD.PATHDIRWWW = TD.PATHDIR + "www" + TD.FILETORNEO + "\\"; //set the wwwdir
 #endif
 #ifdef PLATFORM_POSIX
              TD.PATHDIRWWW = TD.PATHDIR + "www" + TD.FILETORNEO + "/";  //set the wwwdir
 #endif
 //   DataFromArchiveToArray();

    if (TD.IS_SAVED_ROUND) {  // esiste il file *.rnd (le iscrizioni sono chiuse) => lo legge,
       //tab1.arr.Removing(false); //players cannot be deleted
       tab1.arr.Moving(false); //players cannot be moved
       if (TD.IS_SWISS) {
         TD.LoadRound(TD.PATHDIR + TD.FILETORNEO + ".rnd");
         TD.AdjournCrossTableSwiss(TD.currentRound);
       }
       else {
         TD.LoadRoundRR(TD.PATHDIR + TD.FILETORNEO + ".rnd"); //load .rnd file
         TD.AdjournCrossTableRR(TD.currentRound);
       }
       if (TD.STATUS_ROUND==READY_FOR_INSERTION) Exclamation("Insert missing result");
    }
    else  {   // continue with registration
       //tab1.arr.Removing(true); //players can be deleted
       tab1.arr.Moving(true); //players cannot be moved
       TD.currentRound = 0;
       TD.IS_CLOSED_REGISTRATION = false;
       TD.STATUS_ROUND = READY_FOR_PAIRING;
  }
  TD.IS_TORNEO_STARTED = 1;
  //  ResetEditMask();
  tab3.spinRound <<= TD.currentRound;
  tab3.SpinRoundCB(); //per attivare indirettamente ShowPairing() che ha bisogno di round
  UpdateInfo("vcvx", "czzxc", t_("Opened tournament ") + TD.FILETORNEO);
  this->Title(TD.FILETORNEO + " - " + VERSION);
}


// read the archive file contained in the *.veg file
void VegaMain::ReadVegFile(String filename)
{ int i, k, nrecord;
  extern  bool Contains(const String& text, const String& substr);
  String header;
  db.Clear(); //remove all the data from the database
  FileIn in(filename);
  if (in.IsError()) {
     Exclamation("Error reading the .veg FILE");
     return;
  }
  header = in.GetLine(); // skip the header file
  TD.NAMETORNEO = in.GetLine();
  header = in.GetLine();
  TD.PLACETORNEO = header.Mid(0,19);
  TD.HOSTFEDERATION = header.Mid(21,3);

  { // get date
    Vector<String> field = Split(in.GetLine(), ',');
    TD.DATATORNEO_B =  TrimLeft(field[0]) ;
    TD.DATATORNEO_E =  TrimLeft(field[1]) ;
  }
  TD.ARBITER = in.GetLine();
  { // get score system
    Vector<String> field = Split(in.GetLine(), ' ');
    TD.DRAW = StrDblValue( field[0] );
    TD.WON  = StrDblValue( field[1] );
  }
  { // get tyebreak order
    Vector<String> field = Split(in.GetLine(), ' ');
    for (i=0; i<field.GetCount(); i++) {
        TD.tyebreakorder[i+1] = StrIntValue(field[i]); //tyebreaorder start to count from 1
        //Exclamation(AsString(TD.tyebreakorder[i+1]));
    }
  }
  TD.FILETORNEO=in.GetLine();
  {  // further data
     Vector<String> field = Split(in.GetLine(), ' ');
     TD.NRounds = StrIntValue(field[0]);
     TD.PLAY_SYSTEM = StrIntValue(field[1]);
     TD.StartSystem(TD.PLAY_SYSTEM);
     TD.IS_FIDE_TOURNAMENT = StrIntValue(field[2]);
     TD.NAccRound = StrIntValue(field[3]);
     TD.NBoards = StrIntValue(field[4]);
     TD.SCORE_TYPE = StrIntValue(field[5]);
  }
  TD.IS_SAVED_ROUND = TD.IS_CLOSED_REGISTRATION = StrIntValue( in.GetLine() );

  nrecord = TD.NPlayer = StrIntValue( in.GetLine() );

  for (i=1; i<=nrecord; i++) {

		String strID = AsString( i ); //db is zero based
		TeamArchive& team = db.Add( strID );
        Vector<String> field = Split(in.GetLine(), ';');
  	    team.name = field[0];
 	    team.origin = field[1];
 	    team.country = field[2];
		team.ratingfide = 0; //StrIntValue(field[3]);
		team.isAvailable = field[4];

		for (k=0; k<MAX_BOARDS; k++) {
		    Vector<String> fields = Split(in.GetLine(), ';');
        	team.plr[k].name = TrimLeft(fields[0]);
        	team.plr[k].country = TrimLeft(fields[1]);
        	team.plr[k].data = TrimLeft(fields[2]);
        	team.plr[k].sex = TrimLeft(fields[3]);
        	team.plr[k].codetitle = TrimLeft(fields[4]);
        	team.plr[k].idfide = StrIntValue(fields[5]);
        	team.plr[k].ratingfide = StrIntValue(fields[6]);
        	team.plr[k].idnat = StrIntValue(fields[7]);
        	team.plr[k].ratingnat = StrIntValue(fields[8]);
        	team.plr[k].kcoeff = StrIntValue(fields[9]);
        	team.plr[k].isAvailable = TrimLeft(fields[10]);
            if (TD.IS_FIDE_TOURNAMENT) { // set the rating used during the tournament
               team.plr[k].RATING = team.plr[k].ratingfide;
               if (team.plr[k].ratingfide == 0) team.plr[k].RATING = team.plr[k].ratingnat;
            }
            else team.plr[k].RATING = team.plr[k].ratingnat;
     	}
        for (k=0; k<TD.NBoards; k++) team.ratingfide += team.plr[k].RATING;
        team.ratingfide = team.ratingfide/TD.NBoards;
   		tab1.arr.Add( i, team.name, team.origin, team.country, team.ratingfide, team.isAvailable);
   		//tab1.arr.Add( i, team.name, team.origin, team.country, team.ratingfide);
  }
  tab1.countID = nrecord; //l'indice riparte da qui per evitare di avere squadre con la stessa chiave
  in.Close();
}


void VegaMain::ConnectDatabase(int i)
{
    int oldNPlayer = TD.NPlayer;
 	FileSel fs; // for Linux and windows
	int type;
	String fn;
	switch (i) {
	  case 1: fs.Type( t_("archive FIDE (txt)"), "*.txt"); break; //type 1
	  case 2: fs.Type( t_("archive VEG (csv & veg)"), "*.csv;*.veg"); break; //type 2
      case 3: fs.Type( t_("national archive with Fixed Length (txt & csv)"), "*.txt;*.csv"); break; //type 3
	  case 4: fs.Type( t_("archive FSI Italy (csv)"), "*.csv"); break;  //type 4
	  case 5: fs.Type( t_("archive USCF Regular (txt)"), "*.txt"); break;  //type 5
	  case 6: fs.Type( t_("archive USCF Quick (txt)"), "*.txt"); break;  //type 6
	}
    fs.DefaultExt("txt");
    fs.dir <<= TD.PATHDIRDB; // seleziono la directory di partenza
	if(fs.ExecuteOpen())
	{ 	fn = ~fs;
		//type = 1 + fs.type.GetIndex(); //get the file type
		type = i; //1 + fs.type.GetIndex(); //get the file type
		TD.PATHDIRDB = fs.GetActiveDir();
	}
	else return;
	consoleFrame.SetConsoleDatabse(fn, type);
 //   ConnectDBDlg newdb(fn, type);
//	newdb.Run();   // collect new information to set a new tournament
// 	DataFromArchiveToArray( oldNPlayer + 1, TD.NPlayer); // show new importet players
  //  UpdateInfo("", "", t_("Imported ") + AsString(TD.NPlayer - oldNPlayer) + t_(" players"));
}


void VegaMain::ModifyTournament()
{   NewTournamentDlg newt(0);
	newt.Run();   // collect new information to set a new tournament
}


void VegaMain::LoadBackupRoundCB()
{   FileSel fs;

    if (!TD.IS_CLOSED_REGISTRATION) {
       Exclamation( t_("Nothing to load"));
       return;
    }
    fs.dir <<= TD.PATHDIR;
    if (!PromptYesNo( t_("[s0;=*@6 All your data will be lost.&][s0;=*@6 Do you want to continue?]") ) ) return;;
	String fn, dname;
	fs.Type( t_("File round"), "*.rnd").DefaultExt("rnd");
	if(fs.ExecuteOpen()) { // file selected
			fn = ~fs;
	} else return;
    if (TD.IS_SWISS) {
         TD.LoadRound(fn);
         TD.AdjournCrossTableSwiss(TD.currentRound);
    }
    else {
         TD.LoadRoundRR(fn); //load backuped .rnd file
         TD.AdjournCrossTableRR(TD.currentRound);
    }
    tab3.SetSpinRound(TD.currentRound);
    tab3.SpinRoundCB(); //per attivare indirettamente ShowPairing() che ha bisogno di round
    UpdateInfo("xxxx", "yyyy", t_("Read round file: ") + fn);
}


void VegaMain::ResetTournamentCB()
{
  if (TD.currentRound<1) {
     Exclamation( t_("Nothing to reset"));
     return;
  }
  if(!PromptYesNo( t_("[s0;=* Do you want to [@6 RESET] the Tournament at round 0?&][s0;=* The current data will be lost]") ))  return; // go back
  if(!PromptYesNo( t_("[s0;=* LAST WARNING!&][s0;=* Do you want to [@6 RESET] the Tournament at round 0?&][s0;=* The current data will be lost]") ))  return; // go back  TD.Start();
  TD.STATUS_ROUND = READY_FOR_PAIRING;
  //  TD.SaveVegFile(true);     // save what done so far
  TD.DeleteRound(1, TD.currentRound);
  tab3.arrp.Clear();
  tab3.SetSpinRound(TD.currentRound);
  UpdateInfo("vcvx", "czzxc", t_("Reset tournament"));
}

void VegaMain::DeleteRoundCB()
{
  if (TD.currentRound<1) {
     Exclamation( t_("Nothing to delete"));
     return;
  }
  if(!PromptYesNo( t_("Do you want to delete the current round?")) ) return; // go back
  TD.DeleteRound(TD.currentRound, TD.currentRound);
  TD.STATUS_ROUND = READY_FOR_PAIRING;
  tab3.SetSpinRound(TD.currentRound);
  if (TD.currentRound >= 1) tab3.SpinRoundCB();
  else tab3.arrp.Clear();
  UpdateInfo("xxxx", "yyyy", t_("Deleted Round ") + AsString(TD.currentRound+1));
}

void VegaMain::ReopenRegistrationCB()
{
  if (!TD.IS_CLOSED_REGISTRATION) {
     Exclamation( t_("The tournament is not closed!"));
     return;
  }
  if(!PromptYesNo( t_("[s0;=* Do you want to [@6 REOPEN] the Registration?&][s0;=* All your data will be lost]") ))  return; // go back
  if(!PromptYesNo( t_("[s0;=* LAST WARNING!&][s0;=* Do you want to [@6 REOPEN] the Registration?&][s0;=* All your data will be lost]") ))  return; // go back  TD.Start();
  //  TD.SaveVegFile(true);     // save what done so far
  TD.IS_TORNEO_STARTED = true;
  TD.IS_SAVED_ROUND = false;
  TD.IS_CLOSED_REGISTRATION = false;
  TD.STATUS_ROUND = READY_FOR_PAIRING;
  TD.DeleteRound(1, TD.currentRound);
  TD.currentRound = 0;
  tab3.SetSpinRound(1);
  tab3.arrp.Clear();
 //tab1.arr.Removing(true); //players can be deleted
  tab1.arr.Moving(true); //players can be moved
  UpdateInfo("vcvx", "czzxc", t_("Reopened registration"));
}

void VegaMain::Exit()
{   //if (!PromptYesNo( t_("All your data will be lost.&Do you want to continue?")) ) return;;
    int answer;
    answer = Prompt( t_("Closing Vega"), Vega5Img::exitprog(), t_("[s0;=*@6 All your data will be lost.&][s0;=*@6 Do you want to continue?]"),
     t_("Save-Exit"), t_("Exit"), t_("Cancel"), 50);
    if (answer==0) Close();
    else if (answer==1) {
        SaveTournament();
        Close();
    }
    else return;
}

//from extras
void VegaMain::Extras(Bar& bar)
{
	bar.Add(t_("Sort Cross Table"), THISBACK(SortCrossTable));
	bar.Add(t_("Rank Rating Group"), THISBACK(RankRatedGroupCB));
	bar.Add(t_("Rank Date Group"), THISBACK(RankDateGroupCB));
	bar.Add(t_("Round Robin Calendar"), THISBACK(RoundRobinCalendar));
	bar.Add(t_("Missing Point Score system"), THISBACK(MPScoreSystem));
	bar.Separator();
//	bar.Add(t_("Make small badge"), THISBACK(MakeSBadge));
//	bar.Add(t_("Make large badge"), THISBACK(MPScoreSystem));
////	bar.Add(t_("Print Receipts"), THISBACK(PrintReceipt));
////	bar.Separator();
	bar.Add(t_("Backup *.veg file"), THISBACK(BackupVegFile));
	bar.Add(t_("Find Duplicate Player"), THISBACK(FindDuplicate));
	bar.Add(t_("Select Language"), THISBACK(SelectLanguage));
//	bar.Add(t_("DB conversion utility"), THISBACK(DBConversion));
}

//from extras
void VegaMain::Report(Bar& bar)
{
	bar.Add(t_("Fide"), Vega5Img::fide(),THISBACK(FideReport));
	bar.Add("FSI", Vega5Img::ita(), THISBACK(FSIReportCB));
	bar.Add("USCF", Vega5Img::uscf(), THISBACK(USCFReportMenu));

}

void VegaMain::SelectLanguage(Bar& bar)
{
	bar.Add(t_("English"), THISBACK1(ChooseLng, 1));//.Radio(true);
	bar.Add(t_("Italian"), THISBACK1(ChooseLng, 2));//.Radio(false);
	bar.Add(t_("German"),  THISBACK1(ChooseLng, 3));//.Radio(false);
	bar.Add(t_("French"),  THISBACK1(ChooseLng, 4));//.Radio(false);
	bar.Add(t_("Spanish"), THISBACK1(ChooseLng, 5));//.Radio(false);
	bar.Add(t_("Russian"), THISBACK1(ChooseLng, 6));//.Radio(false);
	bar.Add(t_("Portuguese"), THISBACK1(ChooseLng, 7));//.Radio(false);

}

void VegaMain::MakeSBadge()
{
}

void VegaMain::PrintReceipt()
{ // produce receipt files
  int i;
  WithDialogLayout<TopWindow> dlg;
  CtrlLayoutOK(dlg, t_("Enter the registration fee"));
  dlg.Execute();
  String fee = AsString(~dlg.number);
  FileOut savehtml(TD.PATHDIR + "receipt.txt");
  for (i=1; i<=TD.NPlayer; i++)
      { savehtml << NFormat( "%-15s%-25s  |  %-15s%-25s\n", "Receipt of:", fee, "Receipt of:", fee );
        savehtml << NFormat( "%-40.40s  |  %-40.40s\n", TD.NAMETORNEO, TD.NAMETORNEO );
        savehtml << NFormat( "%-40s  |  %-40s\n", player[i].name, player[i].name);
        savehtml << NFormat( "%-10s signature..................... |  %-10s signature.....................\n", TD.DATATORNEO_B, TD.DATATORNEO_B);
        savehtml << "\n        -   -    -     -    -    -     -     -    -    -     -   -\n\n";
      }
  savehtml.Close();
}


void VegaMain::ChooseLng(int lng)
{
	switch (lng)
	{ case 1: 	TD.LANGUAGE = "EN-US"; break;	  //default English
      case 2: 	TD.LANGUAGE = "IT-IT"; break; //Italian
	  case 3: 	TD.LANGUAGE = "DE-DE"; break; //German
	  case 4: 	TD.LANGUAGE = "FR-FR"; break; //French
	  case 5: 	TD.LANGUAGE = "ES-ES"; break; //Spanish
      case 6: 	TD.LANGUAGE = "RU-RU"; break; //Russian
      case 7: 	TD.LANGUAGE = "PT-PT"; break; //Russian
	}
	PromptOK( t_("Please close and restart the program&to make active the change") );
}

void VegaMain::FideReport(Bar& bar)
{
	bar.Add(t_("Rating Report to FIDE"), THISBACK(FIDERatingReportCB));
	bar.Add(t_("Rating variation"), THISBACK(FideEloVariation));
	bar.Add(t_("History card"), THISBACK(FidePlayerCard));
	bar.Add(t_("Find Norm"), THISBACK(FindNorm));
	bar.Add(t_("FIDE norm certificate"), THISBACK(PrintFIDECertificateCB));
	bar.Add(t_("Statistics"), THISBACK(ShowStatistics));
}

void VegaMain::ShowStatistics()
{ if (!TD.IS_CLOSED_REGISTRATION) {
     Exclamation(t_("Option available after the closure&of the registration"));
     return;
  }
  TD.ShowTableStat();
  tabs.Set(2);

}

void VegaMain::FindNorm()
{ if (!TD.IS_CLOSED_REGISTRATION) {
     Exclamation(t_("Option available after the closure&of the registration"));
     return;
  }
  TD.FindNorm();
  tabs.Set(2);


}

void VegaMain::FIDERatingReportCB()
{ if (!TD.IS_CLOSED_REGISTRATION) {
     Exclamation(t_("Option available after the closure&of the registration"));
     return;
  }
  FIDERatingReport dlg;
  dlg.Execute();
}

void VegaMain::PrintFIDECertificateCB()
{ if (!TD.IS_CLOSED_REGISTRATION) {
     Exclamation(t_("Option available after the closure&of the registration"));
     return;
  }
  FIDECertificate dlg;
  dlg.Execute();
}

void VegaMain::FideEloVariation()
{ if (!TD.IS_CLOSED_REGISTRATION) {
     Exclamation(t_("Option available after the closure&of the registration"));
     return;
  }
  TD.FideEloVariation(TD.NPlayer);
  tabs.Set(2);

}

void VegaMain::FidePlayerCard()
{ if (!TD.IS_CLOSED_REGISTRATION) {
     Exclamation(t_("Option available after the closure&of the registration"));
     return;
  }
  TD.FidePlayerCard();
  tabs.Set(2);

}


void VegaMain::FSIReportCB()
{ if (!TD.IS_CLOSED_REGISTRATION) {
     Exclamation(t_("Option available after the closure&of the registration"));
     return;
  }
  TD.SaveFSIRatingReport();
//  TD.FindNormFSI();
  PromptOK( t_("DONE") );
}

void VegaMain::USCFReportMenu(Bar& bar)
{
	bar.Add(t_("Process current section"), THISBACK(USCFSectionReportCB));
	bar.Add(t_("Tournament Report (all sections) "), THISBACK(USCFTournamentReportCB));
}

void VegaMain::USCFSectionReportCB()
{ if (!TD.IS_CLOSED_REGISTRATION) {
     Exclamation(t_("Option available with a running tournament"));
     return;
  }
  USCFSectionReport dlg;
  dlg.Execute();
}

void VegaMain::USCFTournamentReportCB()
{
  USCFReport dlg;
  dlg.Execute();
}

void VegaMain::SortCrossTable()
{ if (!TD.IS_CLOSED_REGISTRATION) {
     Exclamation(t_("Option available after the closure&of the registration"));
     return;
  }
  TD.SortedCross(TD.NPlayer, 1);
  tabs.Set(2);

}


void VegaMain::RankDateGroupCB()
{ if (!TD.IS_CLOSED_REGISTRATION) {
     Exclamation(t_("Option available after the closure&of the registration"));
     return;
  }
  RankDateGroup dlg;
  dlg.Execute();
  tabs.Set(2);//  PromptOK( t_("DONE") );

}

void VegaMain::RankRatedGroupCB()
{ if (!TD.IS_CLOSED_REGISTRATION) {
     Exclamation(t_("Option available after the closure&of the registration"));
     return;
  }
  RankRatedGroup dlg;
  dlg.Execute();
  tabs.Set(2);

}

void VegaMain::RoundRobinCalendar()
{ if (!TD.IS_CLOSED_REGISTRATION) {
     Exclamation(t_("Option available after the closure&of the registration"));
     return;
  }
  if (TD.IS_SWISS) {
     Exclamation(t_("Option available only for Round Robin system!"));
     return;
  }
  TD.RoundRobinCalendar();
  tabs.Set(2);

}

void VegaMain::MPScoreSystem()
{ if (!TD.IS_CLOSED_REGISTRATION) {
     Exclamation(t_("Option available after the closure&of the registration"));
     return;
  }
  if (TD.IS_SWISS) TD.ForlanoScoreSystem(TD.NPlayer);
  else PromptOK( t_("Option not available for Round Robin") );
}

void VegaMain::BackupVegFile()
{ if (!TD.IS_CLOSED_REGISTRATION) {
     Exclamation(t_("Option available after the closure&of the registration"));
     return;
  }
  TD.SaveVegFile(false);
  Exclamation( "Saved .BAK veg file" );
}

void VegaMain::FindDuplicate()
{ int i, j; //TD.SaveVegFile(false);
  String message = "";
  String first, second;
  for (i=1; i<=TD.NPlayer; i++) {
      first = player[i].name;
      for (j=i+1; j<=TD.NPlayer; j++) {
          second = player[j].name;
          if ( first==second ) message << "[s0;*2 " + AsString(second) + "] &";
      }
  }
  if ( !IsNull(message) ) Exclamation( t_("Found the following duplicates: &") + message);
  else PromptOK( t_("There are no duplicate"));
}

void VegaMain::DBConversion()
{
	TD.ConvertUSCFDB();
}

// from Help
void VegaMain::Help(Bar& bar)
{
	bar.Add(t_("Content"), THISBACK(HelpContent));
	bar.Separator();
	bar.Add(t_("Registered to ..."), THISBACK(HelpRegistered));
	bar.Separator();
	bar.Add(t_("About"), THISBACK(HelpAbout));
}

void VegaMain::HelpContent()
{
  PromptOK( "Please refer to the user manual" );
}

void VegaMain::HelpRegistered()
{
  PromptOK( "[s0;=/ [*@6 " + TD.USER_REGISTERED  );
}

void VegaMain::HelpAbout()
{
  PromptOK("[s0;=*@(0.0.255)5 Vega 5.0.10 Beta&][s0;=C www.vegachess.com&][s0;=*/ [*/ author ]Luigi Forlano&][s0;=C2 forlano`@vegachess.com&][s0;=*+117 &][s0;= Vega uses the Crossplatform GUI library Ultimate`+`+&][s0;= http://www.ultimatepp.org]");
}

void VegaMain::SetBars()
{
  mainmenu.Set(THISBACK(VegaMainMenu));
}

void VegaMain::SetTabs()
{
	tabs.Add(tab1,t_("  Players Archive  "));
 //	tabs.Add(tab3,t_("  Round Manager "));
 //	tabs.Add(tab4,t_("  Output  "));
}

void VegaMain::CloseRegistration()
{ // preliminary check
  if(TD.IS_CLOSED_REGISTRATION) {
    String str;
    if (TD.STATUS_ROUND == READY_FOR_PAIRING) str = t_("waiting new pairing");
    else str = t_("waiting result of current round");
    Exclamation( "[s0;=~~~3424;@(0.0.255)The registration were ALREADY closed!&]"
                 "[s0;=@6+117 [@0 current tournament status: ]" + str + "]");
    return;
  }
  if(TD.NPlayer<6 && TD.IS_SWISS) {
    Exclamation( t_("Add at least 6 players for a swiss system"));
    return;
  }
  if(TD.NPlayer <= TD.NRounds && TD.IS_SWISS) {
    Exclamation( t_("Too many rounds"));
    return;
  }
  if((TD.NPlayer<3 || TD.NPlayer>24) && !TD.IS_SWISS) {
    Exclamation( t_("wrong number of players"));
    return;
  }
  if( TD.NPlayer >= (2<<TD.NRounds) &&  TD.IS_SWISS ) {
     Exclamation( t_("you could not get a clear winner... increase Rounds"));
  }
  if(PromptYesNo( t_("Do you want to close the registration?")) )
    { //tab1.btnAdd.Disable(); //no further player can enter the tournemaent for a RR torneo
      tab1.arr.Moving(false); //players cannot be moved
      //tab1.arr.Removing(false); //players cannot be deleted
      DataFromArrayToArchive();
      AssignIDtoPlayers(TD.NPlayer);  // produce the file *.veg sorted as FIDE rule
      TD.IS_CLOSED_REGISTRATION = true;
      tab1.MaskDefaultValue(); //reset the edit mask
      if (!TD.IS_SWISS) {
        TD.PrepareRRPairing(TD.NPlayer); //generate all the tournament pairing
        //TD.SaveRoundRR(0); //save the generated tournament
      }
      TD.IS_SAVED_ROUND = true;
      SaveTournament();  // save a new updated *.veg file
      tabs.Set( 1); //move focus on page 2: pairing
      DopairingCB();
  }
  UpdateInfo("", "", "Registration closed!");
}

void VegaMain::AssignIDtoPlayers(int n)
//
// PURPOSE: sort the database according to several possibility
//     1. FIDE rules: by rating, title and name
//     2. Random
//     3. no sort
//
// INPUT VARIABLE
//	n = number of players to be sorted
{
  int i, k, id;
  String fn;
  int arrid[N_PLAYER_MAX+1];
  int arrs[N_PLAYER_MAX+1];
  int type_sort;
  extern int CompareRatingTitleName(const void *pid1, const void *pid2);

  type_sort = Prompt(t_("Sort of the players"), Vega5Img::sort(), t_("Assign the ID by:"),
     t_("RtgTtlName"), t_("Random"), t_("no sort"), 50);

  if (type_sort==1) { // sort by Elo, Title, Name
      for (i=0; i<n; i++) arrid[i] = i+1; // set the id of each player in the array to be sort
      qsort(arrid, n, sizeof(int), CompareRatingTitleName);
  }
  else if (type_sort==0){ // random
      srand( GetSysTime().second );  //inizializza il generatore dei numeri casuali

      for (i=1; i<=n; i++)
      { arrs[i] = 1; // set to 1 = present all the players to be sort
      }
      for (k=0; k<n; k++)
      { i = 1 + (int) (n*rand()/(RAND_MAX +1.0)); //extract an id
        while (arrs[i] == 0) // the id was already assigned
        { i++;
          if (i>n) i = 1;
        }
        arrs[i] = 0;
        arrid[k] = i; //set the ID
      }
  }
  else { // as they appear in the player list
      for (i=0; i<n; i++) arrid[i] = i+1;
  }

  //set the player with their new ID
  tab1.arr.Clear();
  for (i=0; i<n; i++)
      { id = arrid[i];
	    tab1.arr.Add( i+1, player[id].name , player[id].country, player[id].data, player[id].sex,
	      player[id].codetitle, player[id].idfide, player[id].ratingfide, player[id].idnat,
	      player[id].ratingnat, player[id].kcoeff, player[id].isAvailable);
      }
}


void VegaMain::DopairingCB()
{ int nr;

  if(TD.STATUS_ROUND == READY_FOR_INSERTION) {
     Exclamation( t_("Please insert the result of the current round"));
     return;
  }
  if(TD.STATUS_ROUND == READY_FOR_PAIRING && !TD.IS_CLOSED_REGISTRATION) {
     Exclamation( t_("You cannot generate a new pairing. First close the Registration"));
     return;
  }
  if ( (TD.IS_SWISS) && TD.currentRound==TD.NRounds) {
     Exclamation( t_("The Tournament is finished"));
     return;
  }
  if (TD.PLAY_SYSTEM==ROUND_ROBIN ) {
     nr = TD.NPlayer-1;
     if (TD.NPlayer&1) nr = TD.NPlayer;
     if(TD.currentRound==nr) {
 	   Exclamation(  t_("The Tournament is finished"));
       return;
     }
  }
  if (TD.PLAY_SYSTEM==ROUND_ROBIN2) {
     nr = TD.NPlayer-1;
     if (TD.NPlayer&1) nr = TD.NPlayer;
     if(TD.currentRound==2*nr) {
 	   Exclamation( t_("The Tournament is finished"));
       return;
     }
  }
  TD.STATUS_ROUND = READY_FOR_INSERTION; //prevent new pairing
  if( !TD.IS_SWISS ) { // round Robin
     TD.currentRound++;
     TD.ShowRoundCard2(TD.currentRound);
  }
  else if ( TD.MakeNextPairing() ) { // SWISS
     TD.ShowRoundCard2(TD.currentRound);
  }
  tab3.spinRound <<= TD.currentRound;
  tab3.SpinRoundCB(); // attiva indirettamente ShowPairing() che ha bisogno di round
  TD.SavePairsFile(TD.currentRound);
  if (TD.IS_SWISS) SaveTournament();
  TD.EloForesee(TD.NPlayer, TD.currentRound);
  UpdateInfo("", "", "Done Pairing");
  tab3.arrp.SetCursor(0);
}


void VegaMain::DoManualPairingCB()
{ int oldCurrentRound = TD.currentRound;
  if(TD.STATUS_ROUND == READY_FOR_INSERTION) {
     Exclamation( t_("Please insert the result of the current round"));
     return;
  }
  if(TD.STATUS_ROUND == READY_FOR_PAIRING && !TD.IS_CLOSED_REGISTRATION) {
     Exclamation( t_("You cannot generate a new pairing. First close the Registration"));
     return;
  }
  if ( (TD.IS_SWISS) && TD.currentRound==TD.NRounds) {
     Exclamation( t_("The Tournament is finished"));
     return;
  }
  if (!TD.IS_SWISS) {
       Exclamation(t_("Option not available for Round Robin"));
       return;
  }
  if (oldCurrentRound != TD.currentRound) { //done new pairing
    TD.STATUS_ROUND = READY_FOR_INSERTION; //prevent new pairing
    TD.ShowRoundCard2(TD.currentRound);
    tab3.spinRound <<= TD.currentRound;
    tab3.SpinRoundCB(); // attiva indirettamente ShowPairing() che ha bisogno di round
    TD.SavePairsFile(TD.currentRound);
    SaveTournament();
    TD.EloForesee(TD.NPlayer, TD.currentRound);
    UpdateInfo("", "", "Done Pairing");
  }
}

void VegaMain::ModifyPairingCB()
{
  if(TD.STATUS_ROUND == READY_FOR_PAIRING && !TD.IS_CLOSED_REGISTRATION) {
     Exclamation( t_("Option not available.&First close the Registration"));
     return;
  }
  if (!TD.IS_SWISS) {
       Exclamation(t_("Option not available for Round Robin"));
       return;
  }
  if (tab3.round != TD.currentRound) {
       Exclamation(t_("Option available only for the current round"));
       return;
  }

    TD.ShowRoundCard2(TD.currentRound);
    tab3.spinRound <<= TD.currentRound;
    tab3.SpinRoundCB(); // attiva indirettamente ShowPairing() che ha bisogno di round
    TD.SavePairsFile(TD.currentRound);
    SaveTournament();
    TD.EloForesee(TD.NPlayer, TD.currentRound);
}

void VegaMain::PrintTable()
{
}

void VegaMain::PrintPairingCB()
{
}

void VegaMain::UpdateInfo(const String& _who, const String& _action, const String& _what)
{	String status, psystem;
	int nTotal = TD.NPlayer = tab1.arr.GetCount();
	int ninfo = tab1.GetAvail();
	int nbye = ninfo/1000;
	int nout = ninfo%1000;
	int nAvail  = nTotal - nout;
	String	info = NFormat("%3d,%3d,%3d", nTotal, nAvail, nbye);
	String time = AsString(GetSysTime());
	consoleFrame.console.ScrollInto(consoleFrame.console.GetCount());

	if (!TD.IS_CLOSED_REGISTRATION && !TD.IS_TORNEO_STARTED)
	        status = t_("Open or Create a tournament!");
	else if (!TD.IS_CLOSED_REGISTRATION && TD.IS_TORNEO_STARTED)
	    	status = t_("Registration of players");
	else if (TD.STATUS_ROUND==READY_FOR_INSERTION) status = t_("Waiting insert results!");
	else if (TD.STATUS_ROUND==READY_FOR_PAIRING && TD.currentRound==TD.NRounds)
	        status = t_("Tournament finished!");
	else status = t_("Waiting new pairing");
    if (TD.currentRound==TD.NRounds && TD.IS_SWISS )   status = t_("LAST ROUND!");

	if (TD.PLAY_SYSTEM == DUBOV) psystem = t_("swiss Dubov");
	else if (TD.PLAY_SYSTEM == ROUND_ROBIN)  psystem = t_("single RR");
	else if (TD.PLAY_SYSTEM == ROUND_ROBIN2) psystem = t_("double RR");
	else if (TD.PLAY_SYSTEM == VEGA) psystem = t_("swiss vega");
	else if (TD.PLAY_SYSTEM == USCF) psystem = t_("swiss USCF");
	else if (TD.PLAY_SYSTEM == LIM) psystem = t_("swiss Lim");

	consoleFrame.console.Add(AsString(GetSysTime()), TD.FILETORNEO, psystem , TD.currentRound, info, _what, status);
}


String VegaMain::InfoCurrentPlayerAndStatus()
{   // from the status option button
	int m = tab1.arr.GetCursor();
	if (m < 0) return "--"; //prevent crash in case of null array
	String player =  TrimRight( AsString(tab1.arr.Get( m, "Name") ));
	String s;
	Value v = tab1.arr.Get( m, "Status");
	if (IsNull(v))
		s = t_(" got a BYE!");
	else
		s = v? t_(" is available") : t_(" is NOT available!");
	return (player + s);
}


void VegaMain::arrCtrlsAction() // to be removed
{
    //	SaveTournament(); // each modification is saved immediately
	if (TD.NPlayer != tab1.arr.GetCount()) {
	   	if (TD.IS_CLOSED_REGISTRATION) tab1.SortRecord(0); //before to save sort the table for ID
     	DataFromArrayToArchive();
        TD.SaveVegFile(true); // save only veg file
	}
	UpdateInfo("optCtrl", "made", InfoCurrentPlayerAndStatus());
}

void VegaMain::ThreeButtonAction() // to be removed
{

	int m = tab1.arr.GetCursor();
	//if (m < 0) return "--"; //prevent crash in case of null array
	Value vplayer =  tab1.arr.Get( m, "Name");
	String player =  TrimRight( AsString(vplayer) );
	String s;
	Value v = tab1.arr.Get( m, "Status");
	if (IsNull(v))
		s = t_(" got a BYE!");
	else
		s = v? t_(" is available") : t_(" is NOT available!");
	UpdateInfo("optCtrl", "made", player + s);
  	if (TD.IS_CLOSED_REGISTRATION) tab1.SortRecord(0); //before to save sort the table for ID
   	DataFromArrayToArchive();
    TD.SaveVegFile(true); // save only veg file
//	tab1.arr.FindSetCursor(vplayer, "Name", 0); // set the cursor

/*	UpdateInfo("optCtrl", "made", InfoCurrentPlayerAndStatus());
  	if (TD.IS_CLOSED_REGISTRATION) tab1.SortRecord(0); //before to save sort the table for ID
   	DataFromArrayToArchive();
    TD.SaveVegFile(true); // save only veg file
*/
}

void VegaMain::btnAddAction()
{
	tab1.AddPlayer(0); //Add player
	UpdateInfo("btnAdd", "added", InfoCurrentPlayerAndStatus());
	SaveTournament(); // each modification is saved immediately
}

void VegaMain::btnModifyAction()
{
	tab1.AddPlayer(1); //Modify player
	//UpdateInfo("btnAdd", "added", InfoCurrentPlayerAndStatus());
	SaveTournament(); // each modification is saved immediately
}

void VegaMain::btnRemoveAction()
{
	tab1.RemovePlayer(); //Modify player
	SaveTournament(); // each modification is saved immediately
}

void VegaMain::ShowStatusBar()
{
	consoleFrame.console.Show(); //close the window search DB and show the status bar
	UpdateInfo("btnAdd", "added", InfoCurrentPlayerAndStatus());
}

void VegaMain::AddImportedPlayer()
{  	if ( TD.NPlayer>=TD.MAX_PLAYERS) {
	      Exclamation(t_("exceeded maximum number of players"));
	      return;
	}
	else {
		  Vector<Value> pdata;
		  int board = StrIntValue(AsString(~consoleFrame.editBoardPos));
		  consoleFrame.RegisterPlayerCB(pdata);
		  tab1.arrPlayer.Set(board-1, pdata);
		  board++; if (board>MAX_BOARDS) board = 1;
		  consoleFrame.editBoardPos <<= AsString(board);
	}
}

bool VegaMain::Key(dword key, int count)
{
    if(key == K_ENTER) {
           if (consoleFrame.slist.IsCursor()) {
				if ( TD.NPlayer>=TD.MAX_PLAYERS) {
	      			Exclamation(t_("exceeded maximum number of players"));
				}
				else {
					Vector<Value> pdata;
		  			int board = StrIntValue(AsString(~consoleFrame.editBoardPos));
					consoleFrame.RegisterPlayerCB(pdata);
		  			tab1.arrPlayer.Set(board-1, pdata);
				    board++; if (board>MAX_BOARDS) board = 1;
		  			consoleFrame.editBoardPos <<= AsString(board);
				}
           }
           return true; // key accepted
    }
    return TopWindow::Key(key, count);
}

void VegaMain::Init()
{
	tab1.btnAdd.WhenAction = THISBACK(btnAddAction);
	tab1.btnModify.WhenAction = THISBACK(btnModifyAction);
	tab1.btnRemove.WhenAction = THISBACK(btnRemoveAction);
	//need for other layout buttons...
	tab1.arr.WhenCtrlsAction = THISBACK(ThreeButtonAction); // get signal from threebutton widget
    //tab1.arr.WhenArrayAction = THISBACK(arrCtrlsAction); //update after a delete row//to improve because update two times
    tab1.btnPrint.WhenAction = THISBACK(PrintTable);

    tab3.tabpair.btnAuto.WhenAction = THISBACK(DopairingCB);
    tab3.tabpair.btnManual.WhenAction = THISBACK(DoManualPairingCB);
    tab3.tabinsert.btnModify.WhenAction = THISBACK(ModifyPairingCB);
    tab3.tabpair.btnPrint.WhenAction = THISBACK(PrintPairingCB);
	tab1.btnClose.WhenAction = THISBACK(CloseRegistration);
	consoleFrame.btnClose.WhenAction = THISBACK(ShowStatusBar);
	consoleFrame.slist.WhenLeftDouble = THISBACK(AddImportedPlayer);
}
