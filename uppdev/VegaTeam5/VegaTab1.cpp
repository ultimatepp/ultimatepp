#include "VegaMain.h"
#include "Tournament.h"

//////////////////////////// class option image three state button ///////////////////
void OptionImage::ReFocus()
{
	SetFocus();
}

OptionImage::OptionImage()
{
	WhenAction = THISBACK(ReFocus);
}


void OptionImage::Paint(Draw& w) {
	Size sz = GetSize();
	if(!IsTransparent())
		w.DrawRect(0, 0, sz.cx, sz.cy, SColorFace);
	Size isz = Vega5Img::imgYes().GetSize();  //although maybe it would be good to calc from the biggest...?
	Size tsz = GetSmartTextSize(w, label, font);
	int ty = (sz.cy - tsz.cy) / 2; //text up-down
	int iy = (tsz.cy - isz.cy) / 2 + ty;
	bool ds = !IsShowEnabled();

		Image m = (!notnull || threestate) && IsNull(option) ? ds ? Vega5Img::imgMaybe
				                      : IsPush() ? Vega5Img::imgYes
							                     : Vega5Img::imgMaybe :
			      option == 1 ? ds ? (switchimage ? Vega5Img::imgYes : Vega5Img::imgYes)
				                   : IsPush() ? (switchimage ? Vega5Img::imgYes : Vega5Img::imgYes)
							                  : (switchimage ? Vega5Img::imgYes : Vega5Img::imgYes)
				              : ds ? (switchimage ? Vega5Img::imgNo : Vega5Img::imgNo)
				                   : IsPush() ? (switchimage ? Vega5Img::imgNo : Vega5Img::imgNo)
							                  : (switchimage ? Vega5Img::imgNo : Vega5Img::imgNo);
		w.DrawImage(0, iy, m);

	DrawSmartText(w, isz.cx + 4, ty, tsz.cx, label, font, ds ? SGray : SBlack, VisibleAccessKeys() ? accesskey : 0);
	//text length problem - tsz.cx - doesn't work? - adjust *.lay!!!
}

///////////////////////////////
void Avail3(One<Ctrl>& ctrl)
{
	//ctrl.Create<OptionImage>().ThreeState().SetImage(Vega5Img::imgYes(), Vega5Img::imgNo(), Vega5Img::imgMaybe());
	ctrl.Create<OptionImage>().SetImage(Vega5Img::imgYes(), Vega5Img::imgNo(), Vega5Img::imgMaybe());
}

int FilterCommaSpace(int c)
{
	if (c == ';' || c =='\\') return false;
	else return c;
}

int FilterComma(int c)
{
	if (c == ';') return false;
	else return c;
}

VegaTab1::VegaTab1()
{   // set the widths of each coloumn
	// arr.ColumnWidths("25 3 5 12 8 8 2 3 3 3 3");
	int i;
	countID=1; //è un indice incrementale che cresce sempre
	CtrlLayout(*this);
	SizePos();
	              //id, label, width
	arr.AddColumn("ID", t_("ID"), 3).HeaderTab().WhenAction = THISBACK1(SortRecord, 0);   //0
	arr.AddColumn("Name", t_("Team Name"), 17).HeaderTab().WhenAction = THISBACK1(SortRecord, 1);   //1
	arr.AddColumn("Origin", t_("Origin"), 17).HeaderTab().WhenAction = THISBACK1(SortRecord, 2); ;   //2
	arr.AddColumn("Federation", t_("Fed"), 17).HeaderTab().WhenAction = THISBACK1(SortRecord, 3); ;         //3
	arr.AddColumn("Rating", t_("Rating"), 6).HeaderTab().WhenAction = THISBACK1(SortRecord, 4);    //4
	arr.AddColumn("Status", t_("Status"), 6); //.Ctrls(Avail3);    //5

	//drop list widget
	edtTitleP.Add("GM");
	edtTitleP.Add("WGM");
	edtTitleP.Add("IM");
	edtTitleP.Add("WIM");
	edtTitleP.Add("FM");
	edtTitleP.Add("WFM");
	edtTitleP.Add("M");
	edtTitleP.Add("CM");
	edtTitleP.Add("1N");
	edtTitleP.Add("2N");
	edtTitleP.Add("3N");
	edtTitleP.Add("A");
	edtTitleP.Add("B");
	edtTitleP.Add("C");
	edtTitleP.Add("D");
	edtTitleP.Add("E");
	edtTitleP.Add("F");
	edtTitleP.Add("G");
	edtTitleP.Add("H");
	edtTitleP.Add("NC");
	edtTitleP.Add("--");
    edtTitleP <<= "GM";

//    optGenderP.SetImage(Vega5Img::female(), Vega5Img::male()); // false state, true state
//	optGenderP <<= 1;
//	optStatusP.SetImage(Vega5Img::imgNo(), Vega5Img::imgYes()); // false state, true state
	optGenderP <<= "m";
	optStatusP <<= 1;
	arrPlayer.TabAddsRow(false);
	arrPlayer.AddColumn("Name", t_("Name"), 17).Edit(edtNameP.MaxChars(25));
	arrPlayer.AddColumn("Country", t_("Fed"), 3).Edit(edtCountryP.MaxChars(20));
	arrPlayer.AddColumn("Birthday", t_("Birthday"), 5).Edit(edtBirth.MaxChars(10));
	arrPlayer.AddColumn("Gender", t_("G"), 2).Edit(optGenderP);
	arrPlayer.AddColumn("Title", t_("Title"), 5).Edit(edtTitleP);
	arrPlayer.AddColumn("IDFIDE", t_("ID FIDE"), 5).Edit(edtIDFideP.Min(0).MaxChars(8)) ;
	arrPlayer.AddColumn("RatFIDE", t_("Rtg FIDE"), 5).Edit(edtRtgFideP.Min(0).MaxChars(4)) ;
	arrPlayer.AddColumn("IDNat", t_("ID Nat"), 5).Edit(edtIDNatP.Min(0).MaxChars(8)) ;
	arrPlayer.AddColumn("RatNat", t_("Rtg Nat"), 5).Edit(edtRtgNatP.Min(0).MaxChars(4)) ;
	arrPlayer.AddColumn("K", t_("K"), 3).Edit(edtKP.Min(0).MaxChars(2)) ;
	arrPlayer.AddColumn("Status", t_("Status"), 4).Edit(optStatusP);
//    for (i=0; i<NBoards; i++) arrPlayer.Add("0","0","0", ~optGenderP, ~edtTitleP,"0","0","0","0","0",~optStatusP);
// rendere visibili solo NBoards righe
    for (i=0; i<MAX_BOARDS; i++) arrPlayer.Add("0","0","0", ~optGenderP, ~edtTitleP,"0","0","0","0","0",~optStatusP);

    arr.NoHorzGrid();
	arr.SetLineCy(Vega5Img::imgYes().GetSize().cy+2); //option image adjustment..
	//arr.WhenLeftClick = THISBACK(PromptCell);
	arr.WhenLeftClick = THISBACK(FillFieldsFromRow);
    arr.WhenLeftDouble = THISBACK(ModifyCrossTableCB);
	// color even rows
	arr.EvenRowColor();

	btnClear <<= THISBACK(MaskDefaultValue);
//    btnRemove <<= THISBACK(RemovePlayer);

    btnAdd.SetImage(Vega5Img::add());
    btnModify.SetImage(Vega5Img::modify());
    btnClear.SetImage(CtrlImg::new_doc());
    btnRemove.SetImage(Vega5Img::cancel());
    btnPrint.SetImage(CtrlImg::print());
    btnClose.SetImage(Vega5Img::start());

	editName.SetFilter(FilterCommaSpace); //added filter
	editRatFIDE <<= 0;
	btnAvail.SetImage(Vega5Img::imgNo(), Vega5Img::imgYes());
	btnAvail <<= true;
	MaskDefaultValue();
}


void VegaTab1::SortRecord(int column)
{  if (column==4) arr.Sort(column, StdValueCompareDesc);
   else arr.Sort(column);
}

void VegaTab1::RemovePlayer()
{  int index=arr.GetCursor();
   if (index<0) return;

   String ply;
   int id = StrIntValue( AsString(arr.Get(index, 0)) );  // get ID
   String strID = AsString(id);
   ply = AsString(arr.Get(index, 1));
   if (TD.IS_CLOSED_REGISTRATION) {
      Exclamation("Now you can't remove the team.&Instead make it not available");
      return;
   }
   if (PromptYesNo(t_("Do you want to remove the team&") + ply)) {
      db.RemoveKey(strID);
      arr.Remove(index);
      MaskDefaultValue();
   }
}

void VegaTab1::AddPlayer(int modifyFLAG)
{   // Add or Modify a player
    // modifyFLAG = 0 : Add player
    // modifyFLAG = 1 : Modify player
    int i;
    // check "Name" field
	if ( TrimLeft( AsString(~editName) ).GetLength() == 0) {
		Exclamation(t_("The Team name field cannot be empty!"));
//	    MaskDefaultValue();
//	    ActiveFocus(editName);
	    return;
	}
    if( TD.IS_CLOSED_REGISTRATION && !modifyFLAG) {
//    if(!TD.IS_SWISS && TD.IS_CLOSED_REGISTRATION && !modifyFLAG) {
//		Exclamation(t_("You can't add a player in a Round Robin&after the closure of registration"));
		Exclamation(t_("You can't add a team&after the closure of registration"));
	    //MaskDefaultValue();
	    return;
	}
	// check number of players
	if (TD.NPlayer>=TD.MAX_PLAYERS && !modifyFLAG) {
         Exclamation( t_("Too much players! Stop with the registration"));
	     return;
	}
//    if( TD.IS_SWISS && TD.IS_CLOSED_REGISTRATION && !modifyFLAG) {
//		if (!PromptYesNo(t_("Do you want to add a Late-Comer to the tournament?&You will not be able to delete him again")) )
//	    return;
//	}

	//set default in case of null or empty fields
	if ( IsNull(AsString(~editOrigin)) ) editOrigin <<= "---";
	if ( IsNull(AsString(~editCountry)) ) editCountry <<= "---";
	if ( IsNull(AsString(~editRatFIDE)) ) editRatFIDE <<= 0;

    if (modifyFLAG) { //Modify
	    int int_row;
	    int_row=arr.GetCursor();
	    if (int_row < 0) return; // prevent the crash of the application when int_row = -1)
	    arr.Set( int_row, 1, ~editName);
	    arr.Set( int_row, 2, ~editOrigin);
	    arr.Set( int_row, 3, ~editCountry);
    	arr.Set( int_row, 4, ~editRatFIDE);
    	arr.Set( int_row, 5, ~btnAvail);
	    TD.SaveVegFile(true); // make active the player for the next round

	    int id = StrIntValue( AsString( arr.Get(int_row, 0)) );  // get ID
		String strID = AsString( id );

		TeamArchive& team = db.Get(strID);
		team.name = TrimRight(TrimLeft( AsString(~editName) ) );
		team.origin = AsString(~editOrigin);
		team.country = AsString(~editCountry);
		team.ratingfide = 0; //for the moment
		team.isAvailable = AsString(~btnAvail);

		for (i=0; i<MAX_BOARDS; i++) { //NBoards
	    	team.plr[i].name = AsString(arrPlayer.Get(i, 0));
	    	team.plr[i].country  = AsString(arrPlayer.Get(i, 1));
	    	team.plr[i].data  =  AsString(arrPlayer.Get(i, 2));
	    	team.plr[i].sex = AsString(arrPlayer.Get(i, 3));
	    	team.plr[i].codetitle = AsString(arrPlayer.Get(i, 4));
	    	team.plr[i].idfide = StrIntValue(AsString(arrPlayer.Get(i, 5)));
	    	team.plr[i].ratingfide = StrIntValue(AsString(arrPlayer.Get(i, 6)));
	    	team.plr[i].idnat = StrIntValue(AsString(arrPlayer.Get(i, 7)));
	    	team.plr[i].ratingnat = StrIntValue(AsString(arrPlayer.Get(i, 8)));
	    	team.plr[i].kcoeff = StrIntValue(AsString(arrPlayer.Get(i, 9)));
	    	team.plr[i].isAvailable = AsString(arrPlayer.Get(i, 10));
		}
	    TD.SaveVegFile(true); // make active the player for the next round
    }
	else {
		// add a new registered player
	    TD.NPlayer++;
		String strID = AsString(TD.NPlayer);

		TeamArchive& team = db.Add( strID );  //aggiunge la squadra all'archivio
		team.name = TrimRight(TrimLeft( AsString(~editName) ) );
		team.origin = AsString(~editOrigin);
		team.country = AsString(~editCountry);
		team.ratingfide = 0; //for the moment
		team.isAvailable = AsString(~btnAvail);

		for (i=0; i<MAX_BOARDS; i++) { //NBoards
	    	team.plr[i].name = AsString(arrPlayer.Get(i, 0));
	    	team.plr[i].country  = AsString(arrPlayer.Get(i, 1));
	    	team.plr[i].data  =  AsString(arrPlayer.Get(i, 2));
	    	team.plr[i].sex = AsString(arrPlayer.Get(i, 3));
	    	team.plr[i].codetitle = AsString(arrPlayer.Get(i, 4));
	    	team.plr[i].idfide = StrIntValue(AsString(arrPlayer.Get(i, 5)));
	    	team.plr[i].ratingfide = StrIntValue(AsString(arrPlayer.Get(i, 6)));
	    	team.plr[i].idnat = StrIntValue(AsString(arrPlayer.Get(i, 7)));
	    	team.plr[i].ratingnat = StrIntValue(AsString(arrPlayer.Get(i, 8)));
	    	team.plr[i].kcoeff = StrIntValue(AsString(arrPlayer.Get(i, 9)));
	    	team.plr[i].isAvailable = AsString(arrPlayer.Get(i, 10));
		}
	    countID++;
	    arr.Add( countID, team.name, team.origin, team.country, ~editRatFIDE, ~btnAvail);
	    // arr.Add( TD.NPlayer, team.name, team.origin, team.country, ~editRatFIDE);
	    arr.GoEnd();
	    TD.SaveVegFile(true); // make active the player for the next round
	}
	MaskDefaultValue();
    editName.SetFocus();
}

void VegaTab1::MaskDefaultValue() // body of the callback
{   int i;
	editName <<= Null;
	editOrigin  <<= "---";
	editCountry <<= "---";
	editRatFIDE <<= 0;
	btnAvail <<= true;

	for (i=0; i<MAX_BOARDS; i++) { //NBoards
	    arrPlayer.Set(i, 0, "");
	    arrPlayer.Set(i, 1, "");
	    arrPlayer.Set(i, 2,	"");
	    arrPlayer.Set(i, 3, "m");
	    arrPlayer.Set(i, 4, "--");
	    arrPlayer.Set(i, 5, 0);
	    arrPlayer.Set(i, 6, 0);
	    arrPlayer.Set(i, 7, 0);
	    arrPlayer.Set(i, 8, 0);
	    arrPlayer.Set(i, 9, 0);
	    if (i<TD.NBoards) arrPlayer.Set(i, 10, 1);
	    else arrPlayer.Set(i, 10, 0);
	}

//	ActiveFocus(editName);
}

void VegaTab1::FillFieldsFromRow()
{
	String s;

    int i = arr.GetClickRow();
	if (i<0) return;
    //int id = StrIntValue( AsString(arr.Get(i, 0)) );  // get ID
	//	String strID = AsString( id );
	String strID = db.GetKey(i); //db is zero based
	TeamArchive& team = db.Get(strID);

	editName   <<= team.name ;      //1
	editOrigin <<= team.origin ;   //2
	editCountry <<= team.country;   //3
	editRatFIDE <<= team.ratingfide;   //4
	btnAvail <<= team.isAvailable=="1" ? true : false;

	for (i=0; i<8; i++) { //NBoards
	    arrPlayer.Set(i, 0, team.plr[i].name);
	    arrPlayer.Set(i, 1, team.plr[i].country);
	    arrPlayer.Set(i, 2,	team.plr[i].data);
	    arrPlayer.Set(i, 3, team.plr[i].sex);
	    arrPlayer.Set(i, 4, team.plr[i].codetitle);
	    arrPlayer.Set(i, 5, team.plr[i].idfide);
	    arrPlayer.Set(i, 6, team.plr[i].ratingfide);
	    arrPlayer.Set(i, 7, team.plr[i].idnat);
	    arrPlayer.Set(i, 8, team.plr[i].ratingnat);
	    arrPlayer.Set(i, 9, team.plr[i].kcoeff);
	    arrPlayer.Set(i, 10, team.plr[i].isAvailable);
	}
}


void VegaTab1::PromptCell()
{
	String s;
	if (arr.GetCursor()) {
		s=AsString(arr.ReadRow(arr.GetCursor())[arr.GetClickColumn()]);
		PromptOK(s);
	}
}

int VegaTab1::GetAvail()
{
	int nbye=0, nout=0;
	for(int i=0; i<arr.GetCount(); i++) {
		if ( IsNull(AsString(arr.Get(i, "Status"))) ) nbye++; // byes
		else if ( AsString(arr.Get(i, "Status"))=="0" ) nout++;
	}
	return nbye * 1000 + nout;
}


void VegaTab1::ModifyCrossTableCB()
{ int int_row=arr.GetCursor();
  if (int_row < 0) return; // prevent the crash of the application when int_row = -1)
  if (!TD.IS_CLOSED_REGISTRATION) {
     Exclamation(t_("Option available after the closure&of the registration"));
     return;
  }
  ModifyCrossTableDlg dlg(int_row + 1);
  dlg.Run();
}

/************************* print table ***************************/

PrintTableDlg::PrintTableDlg(ArrayCtrl *array)
{ int i;
  arr = array;
  	CtrlLayout(*this, "Print Registered Players");
	for (i=0; i<=12; i++) {
	    Add(opt[i]);
	    opt[i].LeftPosZ(128, 16).TopPosZ(36 + i*20, 15);
	}
    printed = false;
    opt[1] <<= opt[5] <<= opt[7] <<= opt[9] <<= true;
 	btnClose <<= THISBACK(Close);
	btnPrint <<= THISBACK(PrintDlgCB);
	btnHtml <<= THISBACK(PrintPartecipantsHTML);
	btnHtml.Tip(t_("It will contain all the registered players"));
}


void PrintTableDlg::PrintDlgCB()
{ int i;
  FileOut out(TD.PATHDIR + "plist.txt");
  String s;
  if (!(bool)~opt[12]) { // print All registered players
  for (i=1; i<=TD.NPlayer; i++)
      { String s = "";
        if (~opt[0])  s << NFormat( "%3d ", i);
        if (~opt[1])  s << NFormat( "%-25.25s ", AsString(arr->Get(i-1, "Name")) );
        if (~opt[2])  s << NFormat( "%3.3s ", AsString(arr->Get(i-1, "Country")));
        if (~opt[3])  s << NFormat( "%8.8s ", AsString(arr->Get(i-1, "Birthday")));
        if (~opt[4])  s << NFormat( "%s ",    AsString(arr->Get(i-1, "Gender")));
        if (~opt[5])  s << NFormat( "%3.3s ", AsString(arr->Get(i-1, "Title")));
        if (~opt[6])  s << NFormat( "%8.8s ", AsString(arr->Get(i-1, "IDFIDE")));
        if (~opt[7])  s << NFormat( "%8.8s ", AsString(arr->Get(i-1, "RatFIDE")));
        if (~opt[8])  s << NFormat( "%8.8s ", AsString(arr->Get(i-1, "IDNat")));
        if (~opt[9])  s << NFormat( "%8.8s ", AsString(arr->Get(i-1, "RatNat")));
        if (~opt[10]) s << NFormat( "%2s ", AsString(arr->Get(i-1, "K")));
        out.PutLine(s);
		}
  }
  else {
    out.PutLine("\n The following players got a BYE:\n");
    for (i=1; i<=TD.NPlayer; i++)
      { String s = "";
        if ( IsNull(player[i].isAvailable) ) {
           if (~opt[0])  s << NFormat( "%3d ", i);
           if (~opt[1])  s << NFormat( "%-25.25s ", player[i].name);
           if (~opt[2])  s << NFormat( "%3.3s ", player[i].country);
           if (~opt[3])  s << NFormat( "%8.8s ", player[i].data);
           if (~opt[4])  s << NFormat( "%s ", player[i].sex);
           if (~opt[5])  s << NFormat( "%3.3s ", player[i].codetitle);
           if (~opt[6])  s << NFormat( "%8d ", player[i].idfide);
           if (~opt[7])  s << NFormat( "%5d ", player[i].ratingfide);
           if (~opt[8])  s << NFormat( "%8d ", player[i].idnat);
           if (~opt[9])  s << NFormat( "%5d ", player[i].ratingnat);
           if (~opt[10]) s << NFormat( "%2d ", player[i].kcoeff);
           if (~opt[11]) s << NFormat( "%2s", player[i].isAvailable);
           out.PutLine(s);
        }
      }
    out.PutLine("\n The following player(s) withdrew:\n");
    for (i=1; i<=TD.NPlayer; i++)
      { String s = "";
        if (player[i].isAvailable== "0") {
           if (~opt[0])  s << NFormat( "%3d ", i);
           if (~opt[1])  s << NFormat( "%-25.25s ", player[i].name);
           if (~opt[2])  s << NFormat( "%3.3s ", player[i].country);
           if (~opt[3])  s << NFormat( "%8.8s ", player[i].data);
           if (~opt[4])  s << NFormat( "%s ", player[i].sex);
           if (~opt[5])  s << NFormat( "%3.3s ", player[i].codetitle);
           if (~opt[6])  s << NFormat( "%8d ", player[i].idfide);
           if (~opt[7])  s << NFormat( "%5d ", player[i].ratingfide);
           if (~opt[8])  s << NFormat( "%8d ", player[i].idnat);
           if (~opt[9])  s << NFormat( "%5d ", player[i].ratingnat);
           if (~opt[10]) s << NFormat( "%2d ", player[i].kcoeff);
           if (~opt[11]) s << NFormat( "%2s", player[i].isAvailable);
           out.PutLine(s);
        }
     }
  }
  out.Close();
  Close();
  printed = true;
}

void PrintTableDlg::PrintPartecipantsHTML()
{ int i;
  String name[12];
  name[0] = "<th>ID</th>";
  name[1] = "<th>Name</th>";
  name[2] = "<th>Fed</th>";
  name[3] = "<th>Birthday</th>";
  name[4] = "<th>Gender</th>";
  name[5] = "<th>Title</th>";
  name[6] = "<th>ID_FIDE</th>";
  name[7] = "<th>RtgF</th>";
  name[8] = "<th>ID_Nat</th>";
  name[9] = "<th>RtgN</th>";
  name[10]= "<th>K</th>";
  name[11]= "<th>Status</th>";

  // produce HTML files
  FileOut savehtml(TD.PATHDIRWWW + "players.html");
  savehtml << TD.HTMLCommonHeader();
  savehtml << TD.HTMLNavBar();

  savehtml << "<h2>" << TD.NAMETORNEO << "</h2>\n";
  savehtml << "<h2>" << TD.PLACETORNEO << " - " <<  TD.DATATORNEO_B <<", "<<TD.DATATORNEO_E  << "</h2>\n";
  savehtml << "<h3> List of participants </h3>\n";
  savehtml << "<table bgcolor=#efefef cellpadding=4 cellspacing=0 border=0> \n";
  savehtml << "<tbody>\n";

  savehtml << "<tr>";

  for (i=0; i<=10; i++)  //exclude the Status field
      if (~opt[i]) savehtml << name[i];
  savehtml << "</tr>\n";

  for (i=1; i<=TD.NPlayer; i++)
      { if ((i+1)&1) savehtml << "<tr bgcolor=#e2e2f8> ";
        else savehtml << "<tr bgcolor=#ffffff> ";

        if (~opt[0])  savehtml << NFormat( "<td>%d</td> ", i);
        if (~opt[1])  savehtml << NFormat( "<td>%s</td>", AsString(arr->Get(i-1, "Name")) );
        if (~opt[2])  savehtml << NFormat( "<td>%s</td>", AsString(arr->Get(i-1, "Country")));
        if (~opt[3])  savehtml << NFormat( "<td>%s</td>", AsString(arr->Get(i-1, "Birthday")));
        if (~opt[4])  savehtml << NFormat( "<td>%s</td>", AsString(arr->Get(i-1, "Gender")));
        if (~opt[5])  savehtml << NFormat( "<td>%s</td>", AsString(arr->Get(i-1, "Title")));
        if (~opt[6])  savehtml << NFormat( "<td>%s</td>", AsString(arr->Get(i-1, "IDFIDE")));
        if (~opt[7])  savehtml << NFormat( "<td>%s</td>", AsString(arr->Get(i-1, "RatFIDE")));
        if (~opt[8])  savehtml << NFormat( "<td>%s</td>", AsString(arr->Get(i-1, "IDNat")));
        if (~opt[9])  savehtml << NFormat( "<td>%s</td>", AsString(arr->Get(i-1, "RatNat")));
        if (~opt[10]) savehtml << NFormat( "<td>%s</td>", AsString(arr->Get(i-1, "K")));
        savehtml << " </tr>\n";
      }
  savehtml << "</tbody>\n";
  savehtml << "</table>\n";
  savehtml << "<br> <p align=\"center\"><a href=\"http://www.vegachess.com\"> Generated by Vega </a></p> \n";
  savehtml << "</DIV></body>\n</html>\n";
  savehtml.Close();
}


/***************************** ModifyCrossTable Dialog ****************************/

ModifyCrossTableDlg::ModifyCrossTableDlg(int idplayer)
{
	CtrlLayout(*this, "Modify the Cross Table");

	id = idplayer;
	lblPlayer.SetLabel( AsString(id) + " " + player[id].name);

    edtRound <<= TD.currentRound;
    edtRound.Tip(t_("Set the round to display all other data"));

	//edtRes <<= "0";
    edtOpp.Tip(t_("Game against player 0=BYE&is considered forfeited"));

	edtRes.Add("0");
	edtRes.Add("½");
	edtRes.Add("1");
	edtRes.Add("Null");
    edtRes.Tip(t_("'Null' set the game as 'unplayed'"));

    edtCol.Add("White");
    edtCol.Add("Black");
    edtCol.Add("NOCOL");
    edtCol.Add("NO TB");
    FillEditCB();

	btnClose.WhenAction = THISBACK(Close);
	btnOk.WhenAction = THISBACK(DoneCB);
    edtRound.WhenAction = THISBACK(FillEditCB);
}


void ModifyCrossTableDlg::FillEditCB()
{ int round = StrIntValue(AsString(~edtRound));

  if (round<1 || round>TD.currentRound) {
      Exclamation(t_("Round out of range"));
      return;
   }
  if (TD.IS_SWISS) {
    int opp = TD.playerOpponent[id][round];
    int col = TD.playerColor[id][round];
    int res = TD.roundResult[id][round];

    edtOpp <<= opp;

    if (opp==0 || col==0) edtCol <<= "NOCOL";
    else if (col == 1)  edtCol <<= "White";
    else if (col == -1) edtCol <<= "Black";
    else if (res == 8)  edtCol <<= "NO TB";

    if (res == 0 || res == 4) edtRes <<= "0";
    else if (res == 1 || res == 3) edtRes <<= "1";
    else if (res == 2 || res == 6) edtRes <<= "½";
    else if (opp==0 && col==0 && res==0) edtRes <<= "Null";
  }
  else {
    int opp = TD.playerOpponentRR[id][round];
    int col = TD.playerColorRR[id][round];
    int res = TD.roundResultRR[id][round];

    edtOpp <<= opp;

    if (opp==0 || col==0) edtCol <<= "NOCOL";
    else if (col == 1)  edtCol <<= "White";
    else if (col == -1) edtCol <<= "Black";
    else if (res == 8)  edtCol <<= "NO TB";

    if (res == 0 || res == 4) edtRes <<= "0";
    else if (res == 1 || res == 3) edtRes <<= "1";
    else if (res == 2 || res == 6) edtRes <<= "½";
    else if (opp==0 && col==0 && res==0) edtRes <<= "Null";
  }
}


void ModifyCrossTableDlg::DoneCB()
{ String res, col;
  int round, idOpp;

  round = StrIntValue(AsString(~edtRound));

  if (round<1 || round>TD.currentRound) {
      Exclamation(t_("Round out of range"));
      return;
   }

  idOpp = StrIntValue(AsString(~edtOpp));
  if (idOpp<0 || idOpp>TD.NPlayer || idOpp==id) {
      Exclamation(t_("Set a valid opponent for the selected player"));
      return;
  }

 if (TD.IS_SWISS) { //swiss
   if (idOpp == 0) TD.playerOpponent[id][round] = 0;
   else TD.playerOpponent[id][round] = idOpp;

   res = AsString(~edtRes);
   if ( res=="Null" ) {TD.playerOpponent[id][round] = TD.roundResult[id][round]
                         = TD.playerColor[id][round] = 0;
                       TD.AdjournCrossTableSwiss(TD.currentRound);
                       //return;
                       Close();
      				  }
   if (idOpp==0) {
      if ( res=="1" ) TD.roundResult[id][round] = 3;
      else if ( res=="½" ) TD.roundResult[id][round] = 6;
      else if ( res=="0" ) TD.roundResult[id][round] = 4;
   }
   else {
      if ( res=="1" ) TD.roundResult[id][round] = 1;
      else if ( res=="½" ) TD.roundResult[id][round] = 2;
      else if ( res=="0" ) TD.roundResult[id][round] = 0;
   }

   col = AsString(~edtCol);
   if ( col == "NOCOL" ) TD.playerColor[id][round] = 0; //NO_COLOR;
   else if ( col == "Black" ) TD.playerColor[id][round] = -1; //BLACK_COLOR;
   else if ( col == "White" ) TD.playerColor[id][round] = 1; //WHITE_COLOR;
   else { TD.playerColor[id][round] = 0;
  	      TD.roundResult[id][round] = 8; // generic code for forfeited game with no bucholz
   }
   TD.AdjournCrossTableSwiss(TD.currentRound);
 }
 else { // round robin
   if (idOpp == 0) TD.playerOpponentRR[id][round] = 0;
   else TD.playerOpponentRR[id][round] = idOpp;

   res = AsString(~edtRes);
   if ( res=="Null" ) { TD.playerOpponentRR[id][round] = TD.roundResultRR[id][round]
                            = TD.playerColorRR[id][round] = 0;
                        TD.AdjournCrossTableRR(TD.currentRound);
                        Close();
                        //return;
      			      }
   if (idOpp==0) {
      if ( res=="1" ) TD.roundResultRR[id][round] = 3;
      else if ( res=="½" ) TD.roundResultRR[id][round] = 6;
      else if ( res=="0" ) TD.roundResultRR[id][round] = 4;
    }
   else {
      if ( res=="1" ) TD.roundResultRR[id][round] = 1;
      else if ( res=="½" ) TD.roundResultRR[id][round] = 2;
      else if ( res=="0" ) TD.roundResultRR[id][round] = 0;
   }

   col = AsString(~edtCol);
   if ( col == "NOCOL" ) TD.playerColorRR[id][round] = 0; //NO_COLOR;
   else if ( col == "Black" ) TD.playerColorRR[id][round] = -1; //BLACK_COLOR;
   else if ( col == "White" ) TD.playerColorRR[id][round] = 1; //WHITE_COLOR;
   else { TD.playerColorRR[id][round] = 0;
  	      TD.roundResultRR[id][round] = 8; // generic code for forfeited game with no bucholz
   }
   TD.AdjournCrossTableRR(TD.currentRound);
  }
  Close();
}
