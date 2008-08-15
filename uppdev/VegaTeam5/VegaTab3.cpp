#include "VegaMain.h"
#include "Tournament.h"

struct FontDisplay2 : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const
	{
		Font fnt = Courier(14).Bold(); //Font(q, r.Height() - 2);
	//	String txt = Font::GetFaceName(q);
		String txt = AsString(q);
	 	w.DrawRect(r, paper);
		w.DrawText(r.left + 2, r.top + (r.Height() - GetTextSize(txt, fnt).cy) / 2, txt, fnt, ink); // <- Change is here - "w." no more needed
		//w.DrawText(r.left + 2+1, r.top + (r.Height() - GetTextSize(txt, fnt).cy) / 2, txt, fnt, ink); // <- Change is here - "w." no more needed
	}
};

void EditResult(One<Ctrl>& ctrl)
{
	ctrl.Create<EditInt>().MaxChars(1);
}


VegaTab3::VegaTab3()
{
	CtrlLayout(*this);
	SizePos();
	CtrlLayout(tabpair);
	CtrlLayout(tabinsert);
	tabsPairInsert.Add(tabpair, t_("New Pairing"));
	tabsPairInsert.Add(tabinsert, t_("Insert/Modify Result"));

		              //id, label, width
	arrp.AddColumn("Board", t_("Bo."), 3) ;   //0
	arrp.AddColumn("White", t_("White Player"), 19).SetDisplay(Single<FontDisplay2>());      //1
	arrp.AddColumn("Black", t_("Black Player"), 19).SetDisplay(Single<FontDisplay2>());      //2
	arrp.AddColumn("ID", t_("IDW-IDB"), 4);       //3
	arrp.AddColumn("Edit", "code", 4).Ctrls(EditResult);       //4
//	arrp.AddColumn("Edit", "code", 3).Ctrls<EditInt>();       //3
	arrp.AddColumn("Result", t_("Result"), 4);       //5
//	arrp.AddColumn("code", "code pairing", 5).HeaderTab().WhenAction = THISBACK1(HideTab, &arrp);       //6
	arrp.AddColumn("code", "code pairing", 5);      //6
    arrp.EvenRowColor();
	arrp.SetLineCy(EditField::GetStdHeight());
	arrp.WhenCtrlsAction = THISBACK(UpdateLabelResultCB);
	arrp.WhenEnterKey = THISBACK(StartArrayCursor);
	arrp.WhenLeftDouble = THISBACK(SwapColorsCB);
	HideTab(&arrp); //remove the column 5
	arrp.Tip(t_("Double click to exchange the colors"));
    arrp.HeaderObject().HideTab(4);
    arrp.HeaderObject().HideTab(6);
// tabpair
    spinRound <<= 1;
	round = TD.currentRound;
	spinRound.SetFont(StdFont(18).Bold()).WhenAction = THISBACK(SpinRoundCB);

	tabpair.optVerbose <<= true;
	tabpair.optVerbose.Tip(t_("Save the explanation pairing file"));
	tabpair.optVerbose.WhenAction  = THISBACK1(SetOptionCB, 1);
	tabpair.optThree.WhenAction    = THISBACK1(SetOptionCB, 2);
	tabpair.optThree.Tip(t_("Permit the same color\nthree time in a row"));
	tabpair.optDanubian.WhenAction = THISBACK1(SetOptionCB, 3);
	tabpair.optDanubian.Tip(t_("Enable the Danubian variant\nfor the dubov system"));

	tabpair.btnVerbose <<= THISBACK(ShowVerboseCB);
	tabpair.btnManual.SetImage(Vega5Img::manual() );
	tabpair.btnAuto.SetImage(Vega5Img::computer() );
	tabpair.btnPrint.SetImage(CtrlImg::print());
	tabpair.btnVerbose.SetImage(Vega5Img::verbose());

// tabinsert
	tabinsert.btn0.SetLabel("0 - 1 [0]");
	tabinsert.btn5.SetLabel("½ - ½ [5]");
	tabinsert.btn3.SetLabel("1F - 0 [3]");
	tabinsert.btn4.SetLabel("0 - 1F [4]");
	tabinsert.btn1.SetLabel("1 - 0 [1]");
	tabinsert.btn2.SetLabel("0F - 0F [2]");
	tabinsert.btn0 <<= THISBACK1(BtnResultCB, 0);
	tabinsert.btn1 <<= THISBACK1(BtnResultCB, 1);
	tabinsert.btn2 <<= THISBACK1(BtnResultCB, 2);
	tabinsert.btn3 <<= THISBACK1(BtnResultCB, 3);
	tabinsert.btn4 <<= THISBACK1(BtnResultCB, 4);
	tabinsert.btn5 <<= THISBACK1(BtnResultCB, 5);
	tabinsert.btn7 <<= THISBACK1(BtnResultCB, 7);
	tabinsert.btn8 <<= THISBACK1(BtnResultCB, 8);

	tabinsert.btnModify.SetImage(Vega5Img::modifypair());
	tabinsert.btnUpdate.SetImage(Vega5Img::update());
	tabinsert.btnLoad <<= THISBACK(LoadResultCB);
	tabinsert.btnLoad.SetImage(CtrlImg::open());
	//tabinsert.btnImprove.Disable();
    tabinsert.btnImprove.SetImage(Vega5Img::colors());
	tabinsert.btnUpdate <<= THISBACK1(UpdateData, 1);
}

void VegaTab3::SetOptionCB(int flag)
{
  switch (flag)
   { case 1: if (~tabpair.optVerbose) TD.IS_SET_VERBOSE = true;
             else TD.IS_SET_VERBOSE = false;
             break;
	 case 2: if (~tabpair.optThree) TD.PERMIT3COLOR = true;
             else  TD.PERMIT3COLOR = false;
             break;
	 case 3: if (~tabpair.optDanubian) TD.DROP_PLAYER = true;
             else  TD.DROP_PLAYER = false;
             break;
   }
}

void VegaTab3::SpinRoundCB()
{
	round = StrIntValue(AsString(~spinRound));
	if (round<1 || round>TD.currentRound)  {
        if (TD.IS_CLOSED_REGISTRATION) Exclamation(t_("Round not valid"));
        spinRound <<= TD.currentRound;
	    round = TD.currentRound;
	//	ShowPairingCB();
	//	return;
	}
	else if (round!=TD.currentRound && TD.STATUS_ROUND==READY_FOR_INSERTION) {
        Exclamation(t_("You cannot visualize other rounds.&First insert the results of the current round"));
        spinRound <<= TD.currentRound;
	    round = TD.currentRound;
	//	ShowPairingCB();
	//	return;
	}
	//else ShowPairingCB();
	ShowPairingCB();
}

void VegaTab3::HideTab(ArrayCtrl *x)
{
	x->HeaderObject().HideTab(6);
	x->HeaderObject().HideTab(4);
}

/*
bool VegaTab3::Key(dword key, int count)
{
    if(key == K_1) {
        Exclamation("1");
        // if (slist.IsCursor()) MoveItemCB();
        return true; // key accepted
    }
    return TopWindow::Key(key, count);
}
*/

void VegaTab3::ShowPairingCB()
{ int i, npair, code;
  int idw, idb, res;
  arrp.Clear();
  if (TD.IS_SWISS) npair = TD.pairing[0][round];
  else npair = TD.pairingRR[0][round];
  for (i=1; i<=npair; i++)
      { if (TD.IS_SWISS) code = TD.pairing[i][round];
        else code = TD.pairingRR[i][round];
        TD.DecodeResult(code, &idw, &idb, &res);
        if (!TD.IS_SWISS && (TD.NPlayer&1) && i==npair) { } //skip, do nothing
        else arrp.Add( AsString(i),
                  NFormat("%-20.20s  (%4.1f)", player[idw].name, player[idw].tiebreak[SCORE]),
                  NFormat("%-20.20s  (%4.1f)", player[idb].name, player[idb].tiebreak[SCORE]),
                  NFormat("%3d - %3d", idw, idb),
                  "",
                  TD.LabelResult( res, code, round),
                  AsString (code) );
      }
  arrp.GoBegin();
}

// calling from hided editint widget
void VegaTab3::UpdateLabelResultCB()
{ int int_row = arrp.GetCursor();
  int idw, idb, newresult, board, code, res;
  String str;
  if (int_row < 0) return; //prevent the crash of the application when int_row = -1)
  Value v = arrp.Get(int_row, 4);
  if ( IsNull(v) ) return;     //to avoid non digit key
  newresult = StrIntValue(AsString(v)); //get the inserted result
  code = StrIntValue(AsString(arrp.Get(int_row, 6)));   //get the code result in the hided column to discover if we are changing
  if (newresult==9 || newresult==6) {
    arrp.Set( int_row, 4, ""); // clear the edit field
    MoveNextPair(int_row + 1);
    return;
  }
  if ( newresult==8 ) {
    Exclamation( "If you need to insert a non standard result& please use the appropriate button");
    arrp.Set( int_row, 4, ""); // clear the edit field
    MoveNextPair(int_row + 1);
    return;
  }
  else {
    TD.DecodeResult(code, &idw, &idb, &res);  // get the old result
    if (res!=9 && newresult!=res) { // changing an old result
      if (!PromptOKCancel(t_("Are you sure do you want&to change an old result?")) ) { //ask confirmation
        arrp.Set( int_row, 4, ""); // clear
        return; // go away
      }
    }
    str = TD.LabelResult( newresult, 0, 0) ;
    arrp.Set( int_row, 5, str); // set the new label result to the right
    arrp.Set( int_row, 4, ""); // clear the edit field
    arrp.Set( int_row, 6, AsString(TD.CodifyResult(idw,idb,newresult))); //set new value
    board = int_row + 1; //the array is zero based in contrast with the board index
    if (TD.IS_SWISS) TD.pairing[board][round] = TD.CodifyResult(idw,idb,newresult); //set new result
    else TD.pairingRR[board][round] = TD.CodifyResult(idw,idb,newresult); //set new result
    MoveNextPair(int_row + 1);
  }
}

/*  insertedResult++;
  if(insertedResult%15==0) {
        IS_SAVED_ROUND=0;
        SaveFileCB(NULL, 0L, NULL); // save a new updated *.veg file
  }
*/

void VegaTab3::LoadResultCB()
{ int i, idw, idb, npair, res, round;
  String line, fn;
  FileSel fs; // for Linux and windows

  if (!TD.IS_SWISS) {
       Exclamation(t_("Option not available for Round Robin"));
       return; // no color swap
  }
  fs.Type( t_("Load result file"), "*.res").DefaultExt("res");
	if(fs.ExecuteOpen()) fn = ~fs;  // file selected
	else return; // no file selected

  FileIn in(fn);

  line = in.GetLine();
  Vector<String> field = Split( line, ' ');
  npair = StrIntValue(field[0]);
  round = StrIntValue(field[1]);
  if (round != TD.currentRound) {
       Exclamation( t_("The selected file doens't correspond to the current round!&Please choose the correct one") );
       in.Close();
       return;
  }
  for (i=1; i<=npair; i++)
     {  Vector<String> field = Split( line, ' ');
        idw = StrIntValue(field[0]);
        idb = StrIntValue(field[1]);
        res = StrIntValue(field[2]);
        TD.pairing[i][round] = TD.CodifyResult(idw,idb,res); //set result for selected round
     }
  ShowPairingCB();
  in.Close();
}

void VegaTab3::StartArrayCursor()
{ arrp.SetCursor(0);
}

void VegaTab3::MoveNextPair( int position)
{ int board, npair;
  String coderesult;
//  if (round != currentRound) return position; // there aren't missing result
  npair = arrp.GetCount();
  board = position;
  while (board<npair) {
     coderesult = AsString( arrp.Get( board, 5) );
     if (coderesult == " ... ") { arrp.SetCursor(board) ; return; }
     else board++;
  }
  // restart from 1
  board = 0;
  while (board<position) {
     coderesult = AsString( arrp.Get( board, 5) );
     if (coderesult == " ... ") { arrp.SetCursor(board) ; return; }
     else board++;
  }
  UpdateData(false); //go to save filename_round.rnd file
}


void VegaTab3::UpdateData(bool flag)
{ // flag = false if calling from MoveNextPair
  //      = true if calling from update
  int i, npair;
/*  if(TD.STATUS_ROUND == READY_FOR_INSERTION && flag) {
     Exclamation( t_("Option available after the insertion&of the results of the current round"));
     return;
  }
*/npair = arrp.GetCount();
  for (i=0; i<npair; i++) {
     String coderesult = AsString( arrp.Get( i, 5) );
     if (coderesult == " ... ") {
          Exclamation( t_("[s0;= There are still pairs without result. &] [s0;= Please insert all results or you will be not able to continue]"));
          return;
     }
  }
  TD.SetPlayersResult(round);
  TD.STATUS_ROUND = READY_FOR_PAIRING;
  if (TD.IS_SWISS) {
     TD.SaveRound(TD.currentRound);
     TD.SaveRound(0);
  } else {
     TD.SaveRoundRR(TD.currentRound);
     TD.SaveRoundRR(0);
  }
  TD.SavePairsResultFile(round);
  if (TD.IS_SWISS) TD.AdjournCrossTableSwiss(TD.currentRound);
  else TD.AdjournCrossTableRR(TD.currentRound);
  Exclamation( t_("[s0;= All results have been inserted") );
}

// set the round label without condition
void VegaTab3::SetSpinRound(int r)
{ round = r;
  spinRound <<= r;
}


// set the round label without condition
void VegaTab3::BtnOption()
{ Exclamation(t_("Option not available for Round Robin"));
}


void VegaTab3::BtnResultCB(int idresult)
{ int int_row = arrp.GetCursor();
  int idw, idb, newresult, board, code, res;

  String str;
  if (int_row < 0) return; //prevent the crash of the application when int_row = -1)
  newresult = idresult; //get the inserted result
  code = StrIntValue(AsString(arrp.Get(int_row, 6)));   //get the code result in the hided column to discover if we are changing
  TD.DecodeResult(code, &idw, &idb, &res);  // get the old result
  if (newresult==9 || newresult==6 ) {
    MoveNextPair(int_row + 1);
    return;
  }
  else if (newresult==8)  {
    NonStandardResultDlg nonstdres( idw, idb, round);
    nonstdres.Run();
    str = TD.LabelResultNonStandard( idw,  idb,  round);
    arrp.Set( int_row, 5, str); // set the new label result to the right
    arrp.Set( int_row, 6, AsString(TD.CodifyResult(idw, idb, 8) )); //set new value
    board = int_row + 1; //the array is zero based in contrast with the board index
    if (TD.IS_SWISS) TD.pairing[board][round] = TD.CodifyResult(idw,idb,8); //set new result
    else TD.pairingRR[board][round] = TD.CodifyResult(idw,idb,8); //set new result
    MoveNextPair(int_row + 1);
    return;
  }
  else {
    if (res!=9 && newresult!=res) { // changing an old result
      if (!PromptOKCancel(t_("Are you sure do you want&to change an old result?")) ) return; // go away
    }
    str = TD.LabelResult( newresult, 0, 0) ;
    arrp.Set( int_row, 5, str); // set the new label result to the right
    arrp.Set( int_row, 6, AsString(TD.CodifyResult(idw,idb,newresult))); //set new value
    board = int_row + 1; //the array is zero based in contrast with the board index
    if (TD.IS_SWISS) TD.pairing[board][round] = TD.CodifyResult(idw,idb,newresult); //set new result
    else TD.pairingRR[board][round] = TD.CodifyResult(idw,idb,newresult); //set new result
    MoveNextPair(int_row + 1);
  }
}

void VegaTab3::ShowVerboseCB()
{ ShowVerboseDlg dlg(round);
  dlg.Sizeable().Zoomable().Run();
}

void VegaTab3::ImproveColorsCB()
{ }

void VegaTab3::SwapColorsCB()
{ int board, idw, idb, res, code, position;
  String str;

  if (!TD.IS_SWISS) {
       Exclamation(t_("Option not available for Round Robin"));
       return; // no color swap
  }
  position = arrp.GetCursor();
  if (position < 0) return; // prevent the crash of the application when int_row = -1)

  board = position+1;
  code = TD.pairing[board][round];
  TD.DecodeResult(code, &idw, &idb, &res);
  if( (player[idw].canChangeDuecolor && player[idb].canChangeDuecolor) || round==1)
  {   // the change is permitted
      if ( PromptYesNo( t_("Do you want to exchange the colors&of the selected players?")) ) {
         code = TD.pairing[board][round] = TD.CodifyResult(idb,idw, InvertResult(res) ); //swap color
         // arr.Set( board, 0, ...);
	     arrp.Set( position, 1, NFormat("%-20.20s  (%4.1f)", player[idb].name, player[idb].tiebreak[SCORE]));
    	 arrp.Set( position, 2, NFormat("%-20.20s  (%4.1f)", player[idw].name, player[idw].tiebreak[SCORE]));
    	 arrp.Set( position, 3, NFormat("%3d - %3d", idb, idw) );
    	 //arr.Set( board, 4, "");
    	 arrp.Set( position, 5, TD.LabelResult( InvertResult(res), code, round));
    	 arrp.Set( position, 6, AsString (code)); // set the new result
      }
  }
  else { // the change is not permitted
     if ( PromptYesNo( t_("The exchange is not permitted!&Do you want to continue in any case?")) ) {
         code = TD.pairing[board][round] = TD.CodifyResult(idb,idw, InvertResult(res) ); //swap color
         // arr.Set( board, 0, ...);
	     arrp.Set( position, 1, NFormat("%-20.20s  (%4.1f)", player[idb].name, player[idb].tiebreak[SCORE]));
    	 arrp.Set( position, 2, NFormat("%-20.20s  (%4.1f)", player[idw].name, player[idw].tiebreak[SCORE]));
    	 arrp.Set( position, 3, NFormat("%3d - %3d", idb, idw) );
    	 //arr.Set( board, 4, "");
    	 arrp.Set( position, 5, TD.LabelResult( InvertResult(res), code, round));
    	 arrp.Set( position, 6, AsString (code)); // set the new result
     }
  }
  TD.SaveRound(0);
  if (TD.STATUS_ROUND==READY_FOR_INSERTION && round==TD.currentRound) TD.SavePairsFile(round);
  else TD.SavePairsResultFile(round);
}

int VegaTab3::InvertResult(int res)
// invert the result in a pair
{ if (res == 1) return 0;
  if (res == 0) return 1;
  if (res == 3) return 4;
  if (res == 4) return 3;
  if (res == 8) return 9; //per reiserire il risultato non standard
  return res; // default, 0 - 0, % - %, etc...
}


/////////////// class non standard result ////////////////////////

NonStandardResultDlg::NonStandardResultDlg(int idw, int idb, int rnd)
{   round = rnd;
	CtrlLayout(*this, "");

	lbl1.SetLabel(NFormat("%d %-s", idw, player[idw].name));
	dlw.Add("0");
	dlw.Add("½");
	dlw.Add("0F");
	dlw.Add("½F");
	dlw.Add("½T");
	dlw <<= "½F";

	lbl2.SetLabel(NFormat("%d %-s", idb, player[idb].name));
	dlb.Add("0");
	dlb.Add("½");
	dlb.Add("0F");
	dlb.Add("½F");
	dlb.Add("½T");
	dlb <<= "½F";
	//usare getindex
	btnClose <<= THISBACK(CloseCB);
	btnDone  <<= THISBACK2(DoneNonStandardCB, idw, idb);
}


void NonStandardResultDlg::DoneNonStandardCB(int idw, int idb)
{ String e1;

//  code = (int) ad;
//  DecodeResult(code, &idw, &idb, &res);  // get the id players
//  round = atoi(MLabelValue(labelRound)); //get the round

  if (TD.IS_SWISS) { //swiss
    e1 = AsString(~dlw);     //white player
    if ( e1 == "1" ) {
	  TD.roundResult[idw][round] = 1;
	  TD.playerColor[idw][round] = 1;  //white color
	  TD.playerOpponent[idw][round] = idb;
    }
    else if (e1 == "½") {
	  TD.roundResult[idw][round] = 2;
	  TD.playerColor[idw][round] = 1;  //white color
	  TD.playerOpponent[idw][round] = idb;
    }
    else if (e1 == "0") {
	  TD.roundResult[idw][round] = 0;
	  TD.playerColor[idw][round] = 1;  //white color
	  TD.playerOpponent[idw][round] = idb;
    }
    else if (e1 == "1F") {
	  TD.roundResult[idw][round] = 3;
      TD.playerColor[idw][round] = 0; //NO_COLOR;
	  TD.playerOpponent[idw][round] = 0;
    }
    else if (e1 == "0F") {
	  TD.roundResult[idw][round] = 4;
      TD.playerColor[idw][round] = 0; //NO_COLOR;
	  TD.playerOpponent[idw][round] = 0;
    }
    else if (e1 == "½F" ) {
	  TD.roundResult[idw][round] = 6;
      TD.playerColor[idw][round] = 0; //NO_COLOR;
	  TD.playerOpponent[idw][round] = 0;
    }
    else if (e1 == "½T" ) {
	  TD.roundResult[idw][round] = 8;
      TD.playerColor[idw][round] = 0; //NO_COLOR;
	  TD.playerOpponent[idw][round] = 0;
    }

    e1 = AsString(~dlb);  //black player
    if (e1 == "1" ) {
	  TD.roundResult[idb][round] = 1;
	  TD.playerColor[idb][round] = -1;  //black color
	  TD.playerOpponent[idb][round] = idw;
    }
    else if (e1 == "½" ) {
	  TD.roundResult[idb][round] = 2;
	  TD.playerColor[idb][round] = -1;  //black color
	  TD.playerOpponent[idb][round] = idw;
    }
    else if (e1 == "0" ) {
	  TD.roundResult[idb][round] = 0;
	  TD.playerColor[idb][round] = -1;  //black color
	  TD.playerOpponent[idb][round] = idw;
    }
    else if (e1 == "1F" ) {
	  TD.roundResult[idb][round] = 3;
      TD.playerColor[idb][round] = 0; //NO_COLOR;
	  TD.playerOpponent[idb][round] = 0;
    }
    else if (e1 == "0F" ) {
	  TD.roundResult[idb][round] = 4;
      TD.playerColor[idb][round] = 0; //NO_COLOR;
	  TD.playerOpponent[idb][round] = 0;
    }
    else if (e1 == "½F" ) {
	  TD.roundResult[idb][round] = 6;
      TD.playerColor[idb][round] = 0; //NO_COLOR;
	  TD.playerOpponent[idb][round] = 0;
    }
    else if (e1 == "½T" ) {
	  TD.roundResult[idb][round] = 8;
      TD.playerColor[idb][round] = 0; //NO_COLOR;
	  TD.playerOpponent[idb][round] = 0;
    }
  }
  else { // round robin
/*    int index = dlw.GetIndex();
    switch (index)
    {
      case 0:
	    TD.roundResultRR[idw][round] = 0;
	    TD.playerColorRR[idw][round] = 1;
	    TD.playerOpponentRR[idw][round] = idb;
        break;
      case 1:
	    TD.roundResultRR[idw][round] = 2;
	    TD.playerColorRR[idw][round] = 1;
	    TD.playerOpponentRR[idw][round] = idb;
        break;
      case 2:
	    TD.roundResultRR[idw][round] = 4;
        TD.playerColorRR[idw][round] = 0; //NO_COLOR;
	    TD.playerOpponentRR[idw][round] = 0;
        break;
      case 3:
	    TD.roundResultRR[idw][round] = 6;
        TD.playerColorRR[idw][round] = 0; //NO_COLOR;
	    TD.playerOpponentRR[idw][round] = 0;
        break;
      case 4:
	    TD.roundResultRR[idw][round] = 8;
        TD.playerColorRR[idw][round] = 0; //NO_COLOR;
	    TD.playerOpponentRR[idw][round] = 0;
        break;
    }
    index = dlb.GetIndex();
    switch (index)
    {
      case 0:
	    TD.roundResultRR[idb][round] = 0;
	    TD.playerColorRR[idb][round] = -1;
	    TD.playerOpponentRR[idb][round] = idw;
        break;
      case 1:
	    TD.roundResultRR[idb][round] = 2;
	    TD.playerColorRR[idb][round] = -1;
	    TD.playerOpponentRR[idb][round] = idw;
        break;
      case 2:
	    TD.roundResultRR[idb][round] = 4;
        TD.playerColorRR[idb][round] = 0; //NO_COLOR;
	    TD.playerOpponentRR[idb][round] = 0;
        break;
      case 3:
	    TD.roundResultRR[idb][round] = 6;
        TD.playerColorRR[idb][round] = 0; //NO_COLOR;
	    TD.playerOpponentRR[idb][round] = 0;
        break;
      case 4:
	    TD.roundResultRR[idb][round] = 8;
        TD.playerColorRR[idb][round] = 0; //NO_COLOR;
	    TD.playerOpponentRR[idb][round] = 0;
        break;
    }
  }*/

    e1 = AsString(~dlw);      //white player
    if (e1 == "1" ) {
	  TD.roundResultRR[idw][round] = 1;
	  TD.playerColorRR[idw][round] = 1;
	  TD.playerOpponentRR[idw][round] = idb;
    }
    else if (e1 == "½" ) {
	  TD.roundResultRR[idw][round] = 2;
	  TD.playerColorRR[idw][round] = 1;
	  TD.playerOpponentRR[idw][round] = idb;
    }
    else if (e1 == "0" ) {
	  TD.roundResultRR[idw][round] = 0;
	  TD.playerColorRR[idw][round] = 1;
	  TD.playerOpponentRR[idw][round] = idb;
    }
    else if (e1 == "1F" ) {
	  TD.roundResultRR[idw][round] = 3;
      TD.playerColorRR[idw][round] = 0; //NO_COLOR;
	  TD.playerOpponentRR[idw][round] = idb;
	 // TD.playerOpponentRR[idw][round] = 0;
    }
    else if (e1 == "0F" ) {
	  TD.roundResultRR[idw][round] = 4;
      TD.playerColorRR[idw][round] = 0; //NO_COLOR;
	  TD.playerOpponentRR[idw][round] = idb;
	 // TD.playerOpponentRR[idw][round] = 0;
    }
    else if (e1 == "½F" ) {
	  TD.roundResultRR[idw][round] = 6;
      TD.playerColorRR[idw][round] = 0; //NO_COLOR;
	  TD.playerOpponentRR[idw][round] = idb;
	  //TD.playerOpponentRR[idw][round] = 0;
   }
    else if (e1 == "½T" ) {
	  TD.roundResultRR[idw][round] = 8;
      TD.playerColorRR[idw][round] = 0; //NO_COLOR;
	  TD.playerOpponentRR[idw][round] = idb;
	  //TD.playerOpponentRR[idw][round] = 0;
    }

    e1 = AsString(~dlb);   //black player
      if (e1 == "1" ) {
	  TD.roundResultRR[idb][round] = 1;
	  TD.playerColorRR[idb][round] = -1;
	  TD.playerOpponentRR[idb][round] = idw;
    }
    else if (e1 == "½" ) {
	  TD.roundResultRR[idb][round] = 2;
	  TD.playerColorRR[idb][round] = -1;
	  TD.playerOpponentRR[idb][round] = idw;
    }
    else if (e1 == "0" ) {
	  TD.roundResultRR[idb][round] = 0;
	  TD.playerColorRR[idb][round] = -1;
	  TD.playerOpponentRR[idb][round] = idw;
    }
    else if (e1 == "1F" ) {
	  TD.roundResultRR[idb][round] = 3;
      TD.playerColorRR[idb][round] = 0; //NO_COLOR;
	  TD.playerOpponentRR[idb][round] = idw;
	  //TD.playerOpponentRR[idb][round] = 0;
    }
    else if (e1 == "0F" ) {
	  TD.roundResultRR[idb][round] = 4;
      TD.playerColorRR[idb][round] = 0; //NO_COLOR;
	  TD.playerOpponentRR[idb][round] = idw;
	  //TD.playerOpponentRR[idb][round] = 0;
    }
    else if (e1 == "½F" ) {
	  TD.roundResultRR[idb][round] = 6;
      TD.playerColorRR[idb][round] = 0; //NO_COLOR;
	  TD.playerOpponentRR[idb][round] = idw;
	  //TD.playerOpponentRR[idb][round] = 0;
   }
    else if (e1 == "½T" ) {
	  TD.roundResultRR[idb][round] = 8;
      TD.playerColorRR[idb][round] = 0; //NO_COLOR;
	  TD.playerOpponentRR[idb][round] = idw;
	  //TD.playerOpponentRR[idb][round] = 0;
    }
  }
  Close();
}

void NonStandardResultDlg::CloseCB()
{
	Close();
}


///////////////////////// class ShowVerbose ////////////////////////////
/*
ShowVerboseDlg::ShowVerboseDlg(int round)
{   String filename;
	CtrlLayout(*this, "Pairing explanation");
    this->Zoomable();

    filename = TD.PATHDIR + "verbose" + AsString(round) + ".txt" ;
    lineeditverbose.SetFont(Courier(11)).Set(LoadFile(filename));

    filename = TD.PATHDIR + "scgr" + AsString(round) + "-0.txt" ;
    lineeditgroup.SetFont(Courier(11)).Set(LoadFile(filename));
}
*/
ShowVerboseDlg::ShowVerboseDlg(int round)
{   String filename;
	h.Horz(a, v);
	v << b; // << c << d;
	Add(h.SizePos());

    filename = TD.PATHDIR + "verbose" + AsString(round) + ".txt" ;
    a.SetFont(Courier(11)).Set(LoadFile(filename));

    filename = TD.PATHDIR + "scgr" + AsString(round) + "-0.txt" ;
    b.SetFont(Courier(11)).Set(LoadFile(filename));
}
