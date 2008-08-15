#include "VegaMain.h"
#include "Tournament.h"
#include "SwissUSCF.h"


extern int  CanPlayWith(int player1, int player2, int round);
//============= Aris ScrollArea ===========================
//====== can move to your lib ScrollArea.h ================
/*class ScrollArea : public StaticRect {
public:
	ScrollBars sc;
	StaticRect work;
	void SetWorkSize(Size sz);
	virtual void Scroll();

	typedef ScrollArea CLASSNAME;
	ScrollArea();
	~ScrollArea() {;}
};
*/
//====== can move to your lib ScrollArea.cpp ================
void ScrollArea::Scroll()
{
	Size ws = work.GetSize();  // WhenResize callback is needed?! or?...
	work.SetRect(-sc.x, -sc.y, ws.cx, ws.cy);
}


void ScrollArea::SetWorkSize(Size ws)
{
	work.SetRect(0,0,ws.cx,ws.cy);
	sc.SetTotal(ws.cx,ws.cy);  //can be adjusted depending on GetRect...
	sc.SetPage(100,100);  //can be calculated... let's keep for a while
}


ScrollArea::ScrollArea()
{
	SetWorkSize(Size(500,1000));
	AddFrame(sc);
	Add(work);
	//sc.HideX();  //
//	sc.HideY();  // ADDED uncomment to hide...

	sc.WhenScroll=THISBACK(Scroll);
//	AddFrame(ThinOutsetFrame());
	AddFrame(ThinInsetFrame());
}

/*
class ImageButton : public Button {
private:
  int align;
public:
  ImageButton() {align = ALIGN_CENTER; }; //default alignment is center
  virtual void Paint(Draw& draw);
  //void SetAlignment(int _align) {align = _align;}; //method to set the Alignment
  void SetAlignment(int _align) {align=_align; Refresh();};
  int GetAlignment() {return align;}; //method to get the alignment
};

void ImageButton::Paint(Draw& w)
{
	Size sz = GetSize(); //this is the beginning of the original Paint method
	bool ds = !IsShowEnabled();
	DrawLabel dl;
	///////////////////
	dl.align = align; //this is the only line to be added
	///////////////////
	dl.text = label;
	dl.font = Nvl(font, ButtonFont());
	dl.limg = img;
	dl.disabled = ds;
	dl.lspc = !label.IsEmpty() && !img.IsEmpty() ? 4 : 0;
	if(VisibleAccessKeys())
		dl.accesskey = accesskey;
	if(monoimg)
		dl.lcolor = SColorText;
	Value (*st)(int) = look;
	if(look == ButtonLook) {
		if(InFrame())
			st = EdgeButtonLook;
		if(type == OK)
			st = OkButtonLook;
	}
	int i = GetVisualState();
	ChPaint(w, sz, (*st)(i));
	dl.ink = ButtonTextColor(i);
	if(monoimg)
		dl.lcolor = ButtonMonoColor(i);
	int m = IsPush() && ButtonPressOffsetFlag();
	dl.Paint(w, 3 + m, 3 + m, sz.cx - 6, sz.cy - 6);
};
*/



/*
enum {WY=0, WN, BY, BN, NG};

class ColoredRow : public WithRowLayout<ParentCtrl> {
public:
    int wid, bid, board;
    int wrtg, brtg;
    float wscr, bscr;
    int wflag, bflag;
	typedef ColoredRow CLASSNAME;

	ColoredRow();
	void Set(int board, int wid, int bid, int wrtg, int brtg, float wscr, float bscr, int wflag, int bflag);
	void SetSide(int side, int id, int rtg, float scr, int flag);
	void GetSide(int side, int& id, int& rtg, float& scr, int& flag);
};
*/

ColoredRow::ColoredRow()
{
	CtrlLayout(*this);
    rect.Color(White);
	wid = bid = board = 0;
	wimg.SetImage(Vega5Img::whiteY());
	bimg.SetImage(Vega5Img::blackN());
}

void ColoredRow::Set(int indexboard, int whiteid, int blackid, int whitertg, int blackrtg,
                    float whitescr, float blackscr, int whiteflag, int blackflag)
{  	board = indexboard;
	wid = whiteid;
	bid = blackid;
	wrtg = whitertg;
	brtg = blackrtg;
	wscr = whitescr;
	bscr = blackscr;
	wflag = whiteflag;
	bflag = blackflag;

	lblBoard.SetLabel(AsString(board));
	lblwrtg.SetLabel(AsString(wrtg));
	lblbrtg.SetLabel(AsString(brtg));
	lblwscr.SetLabel(AsString(wscr));
	lblbscr.SetLabel(AsString(bscr));
	btnwid.SetLabel(AsString(wid));
	btnbid.SetLabel(AsString(bid));
	switch (wflag) {
		case WY: wimg.SetImage(Vega5Img::whiteY());
				 break;
		case WN: wimg.SetImage(Vega5Img::whiteN());
				 break;
		case BY: wimg.SetImage(Vega5Img::blackY());
				 break;
		case BN: wimg.SetImage(Vega5Img::blackN());
				 break;
		case NG: wimg.SetImage(Vega5Img::neutral());
				 break;
	}
	switch (bflag) {
		case WY: bimg.SetImage(Vega5Img::whiteY());
				 break;
		case WN: bimg.SetImage(Vega5Img::whiteN());
				 break;
		case BY: bimg.SetImage(Vega5Img::blackY());
				 break;
		case BN: bimg.SetImage(Vega5Img::blackN());
				 break;
		case NG: bimg.SetImage(Vega5Img::neutral());
				 break;
	}
}

void ColoredRow::Get(int& whiteid, int& blackid, int& whitertg, int& blackrtg)
{
	whiteid = wid;
	blackid = bid;
	whitertg = wrtg;
	blackrtg = brtg;
}

void ColoredRow::SetSide(int side, int id, int rtg, float scr, int flag)
{
  if (side == 1) { //right
	bid = id;
	brtg = rtg;
	bscr = scr;
	bflag = flag;

	lblbrtg.SetLabel(AsString(rtg));
	lblbscr.SetLabel(AsString(scr));
	btnbid.SetLabel(AsString(id));

	switch (flag) {
		case WY: bimg.SetImage(Vega5Img::whiteY());
				 break;
		case WN: bimg.SetImage(Vega5Img::whiteN());
				 break;
		case BY: bimg.SetImage(Vega5Img::blackY());
				 break;
		case BN: bimg.SetImage(Vega5Img::blackN());
				 break;
		case NG: bimg.SetImage(Vega5Img::neutral());
				 break;
	}
  }
  else { //left
	wid = id;
	wrtg = rtg;
	wscr = scr;
	wflag = flag;

	lblwrtg.SetLabel(AsString(rtg));
	lblwscr.SetLabel(AsString(scr));
	btnwid.SetLabel(AsString(id));
	switch (flag) {
		case WY: wimg.SetImage(Vega5Img::whiteY());
				 break;
		case WN: wimg.SetImage(Vega5Img::whiteN());
				 break;
		case BY: wimg.SetImage(Vega5Img::blackY());
				 break;
		case BN: wimg.SetImage(Vega5Img::blackN());
				 break;
		case NG: wimg.SetImage(Vega5Img::neutral());
				 break;
	}
  }
}


void ColoredRow::GetSide(int side, int& id, int& rtg, float& scr, int& flag)
{
  if (side == 0) { //left
	id = wid;
	rtg = wrtg;
	scr = wscr;
	flag = wflag;
  }
  else { //right
	id = bid;
	rtg = brtg;
	scr = bscr;
	flag = bflag;
  }
}

//-----------------------
/*
class Abbinamento : public WithAbbinamento<TopWindow> {
	ColoredRow a[3];
//	ScrollArea scrollarea;
    int lastlabel, numlabel;
    int id1, id2; // remind the two ids has been selected
    int iboard[2], side[2]; // remind the board (1...npair) and the side(0=left, 1=right)
	void FillEdit(int board, int id);
    void ExchangePlayersCB();
public:

	typedef Abbinamento CLASSNAME;
	Abbinamento();
};
*/
Abbinamento::Abbinamento()
{
	CtrlLayout(*this, "Modify Colors");

//	Add(scrollarea);
//	scrollarea.LeftPosZ(14, 499).TopPosZ(7, 307);
//	scrollarea.SetRect(50,50,490,400);
//	scrollarea.work.Color(SRed); //not necessary... just to play with total...
    int i, npair, code, idw, idb, res, wf, bf;
    lastlabel = 1;
    numlabel = 0;
    id1 = id2 = 0;
    iboard[0] = iboard[1] = 0;
    side[0] = side[1] = 0;

    npair = TD.pairing[0][TD.currentRound];
    //for (i=1; i<=npair; i++) pairingNew[i]=pairing[i][currentRound];//do a copy
  	scrollarea.SetWorkSize( Size(485, 26*npair) );
    scrollarea.sc.HideX();
    for (i=1; i<=npair; i++)
      { //code = pairingNew[i];
        code = TD.pairing[i][TD.currentRound];
        TD.DecodeResult(code, &idw, &idb, &res);

        if      (player[idw].canChangeDuecolor && player[idw].duecolor==WHITE) wf = WY;
        else if (player[idw].canChangeDuecolor && player[idw].duecolor==BLACK) wf = BY;
        else if (!player[idw].canChangeDuecolor && player[idw].duecolor==BLACK) wf = BN;
        else if (!player[idw].canChangeDuecolor && player[idw].duecolor==WHITE) wf = WN;
        else wf = NG;

        if      (player[idb].canChangeDuecolor && player[idb].duecolor==WHITE) bf = WY;
        else if (player[idb].canChangeDuecolor && player[idb].duecolor==BLACK) bf = BY;
        else if (!player[idb].canChangeDuecolor && player[idb].duecolor==BLACK) bf = BN;
        else if (!player[idb].canChangeDuecolor && player[idb].duecolor==WHITE) bf = WN;
        else bf = NG;

	    scrollarea.work.Add(a[i]);
  	    a[i].Set(i, idw, idb, player[idw].RATING, player[idb].RATING,
  	         player[idw].tiebreak[SCORE], player[idb].tiebreak[SCORE], wf, bf);
	    a[i].LeftPosZ(1, 478).TopPosZ(1 + (i-1)*26, 26);

	    a[i].btnwid.WhenAction = THISBACK2(FillEdit, a[i].board * 10 + 0, a[i].wid);
	    a[i].btnbid.WhenAction = THISBACK2(FillEdit, a[i].board * 10 + 1, a[i].bid);
     }

	edtw.SetLabel(""); //<<= "";
    edtb.SetLabel(""); // <<= "";
    btnExchange.WhenAction = THISBACK(ExchangePlayersCB);
    btnVerify.WhenAction = THISBACK(VerifyPairCB);
    btnDone.WhenAction = THISBACK(DoneCB);
    btnClose.WhenAction = THISBACK(CloseCB);
//	ImageButton btn;
//	Add(btn);
//	btn.LeftPosZ(24, 73).TopPosZ(231, 27);
//	btn.SetImage(Vega5Img::blackN());
}

void Abbinamento::FillEdit(int board, int id)
{ // flaglabel = 0 left, 1 right
  int flaglabel;
  String str;
  flaglabel = board % 10; // extract the side
  board = board/10;
//  code = pairingNew[board];
//  DecodeResult(code, &idw, &idb, &res);

//  str = NFormat("%3d (%3d)", id, board);
  str = NFormat("%d", id);
  numlabel++;

  if (lastlabel) { // force to write not in the last label
     edtw.SetLabel(str); // <<= str;
     id1 = id;
     if (numlabel > 2) a[ iboard[0] ].rect.Color(White);
     lastlabel = 0;
     iboard[lastlabel] = board;
     side[lastlabel] = flaglabel;
     a[board].rect.Color(Color(175,255,84)); // old return White
  }
  else { // try to write in right label
     edtb.SetLabel(str); // <<= str;
     id2 = id;
     if (numlabel > 2) a[ iboard[1] ].rect.Color(White);
     lastlabel = 1;
     iboard[lastlabel] = board;
     side[lastlabel] = flaglabel;
     a[board].rect.Color(Color(175,255,84)); // old return White
  }
 if (numlabel>2) numlabel = 2; // count the written label
 //String a=NFormat("(%3d-%d) (%3d-%d)", iboard[0], side[0], iboard[1], side[1]);
 //lbl.SetLabel(a);
 if (numlabel == 2) ShowInfo();

}


void Abbinamento::VerifyPairCB()
{
  String str1, str2;

  str1 = edtw.GetText(); // AsString(~edtw);
  str2 = edtb.GetText(); // AsString(~edtb);
  if ( IsNull(str1) || IsNull(str2) ) {
       Exclamation(t_("Please select two players"));
       return;
  }
  if ( CanPlayWith(id1, id2, TD.currentRound) )  Exclamation(t_("The selected players are compatible"));
  else Exclamation(t_("The selected players are NON compatible"));
}

void Abbinamento::DoneCB()
{
  String str;
  int i, idw, idb, rtgw, rtgb, npair;
  extern void SortPairs(int round);
  npair = TD.pairing[0][TD.currentRound];
  for (i=1; i<=npair; i++) {
    a[i].Get(idw, idb, rtgw, rtgb);
    TD.pairing[i][TD.currentRound] = TD.CodifyResult( idw, idb, 9);
  }
  if (TD.currentRound>1) TD.VerifyColor(TD.currentRound);  // check only for round>1
  SortPairs(TD.currentRound);
  TD.SavePairsFile(TD.currentRound); // save on a file the pairing
  Close();
}


bool Abbinamento::ShowInfo()
{ String str1, str2;
  SwissUSCF USCF;
  int b1, b2;
  int idw1, idb1, idw2, idb2, rtgw, rtgb;
  int d;

  str1 = edtw.GetText(); // AsString(~edtw);
  str2 = edtb.GetText(); // AsString(~edtb);
  if ( IsNull(str1) || IsNull(str2) ) {
       Exclamation(t_("Please select two players"));
       return false;
  }
  b1 = iboard[0];
  b2 = iboard[1];

  a[b1].Get(idw1, idb1, rtgw, rtgb);
  a[b2].Get(idw2, idb2, rtgw, rtgb);

  if (b1==b2) {
      lbl.SetLabel("0");
      return false;
  }
  if (id1==idw1 && id2==idw2) {  // transposition
     d = USCF.CompareTranspose(idw1, idb1, idw2, idb2);
     if (d == 100000) {
       lbl.SetLabel(t_("FORBID!"));
       return false;
     }
     lbl.SetLabel( NFormat("%d trans.", d) );
  }
  else if (id1==idw1 && id2==idb2) {
     d = USCF.CompareInterchange(idw1, idb1, idw2, idb2, 1);
     if ( d == 100000 ) {
       lbl.SetLabel( t_("FORBID!"));
       return false;
     }
     lbl.SetLabel( NFormat("%d inter.", d) );
  }
  else if (id1==idb1 && id2==idw2) {
     d = USCF.CompareInterchange(idw1, idb1, idw2, idb2, 0);
     if (d == 100000) {
       lbl.SetLabel( "FORBID!");
       return false;
     }
     lbl.SetLabel( NFormat("%d inter.", d) );
  }
  else if (id1==idb1 && id2==idb2) {
     d = USCF.CompareTranspose(idw1, idb1, idw2, idb2);
     if (d == 100000) {
       lbl.SetLabel( t_("FORBID!"));
       return false;
     }
     lbl.SetLabel( NFormat("%d trans.", d) );
  }
  return true;
}

void Abbinamento::CloseCB()
{
  if (!PromptYesNo(t_("The current data will be lost.&Do you want to continue?"))) return;
  Close();
}


void Abbinamento::ExchangePlayersCB()
{ String str1, str2;
//  int id1, b1, id2, b2, flagalign1, flagalign2;
//  int code1, code2, idw1, idb1, idw2, idb2, res;
//  int ans, d;
//  int p1, p2, s1, s2, r80, r200;

  str1 = edtw.GetText(); // AsString(~edtw);
  str2 = edtb.GetText(); // AsString(~edtb);
  if ( IsNull(str1) || IsNull(str2) ) {
       Exclamation(t_("Choose two players to exchange"));
       return;
  }
  if ( !ShowInfo() ) {
       Exclamation(t_("This exchange is ILLEGAL"));
       return;
  }
/*
  sscanf(str1, "%d (%d", &id1, &b1);
  code1 = pairingNew[b1];
  DecodeResult(code1, &idw1, &idb1, &res);

  sscanf(str2, "%d (%d", &id2, &b2);
  code2 = pairingNew[b2];
  DecodeResult(code2, &idw2, &idb2, &res);

  if (id1==id2) {
      Exclamation("This exchange looks rather stupid.&Do you agree?");
      return;
  }
  if (b1==b2) {
      Exclamation("This exchange will be performed by Vega if necessary");
      return;
  }
  // check 200 points rule
  // WW & BB, oppure BB & WW, in ogni caso i duecolor
  // sono diversi nelle due coppie
  p1 = playerState[idw1].colorDiff * playerState[idb1].colorDiff;
  p2 = playerState[idw2].colorDiff * playerState[idb2].colorDiff;
  s1 = playerState[idw1].duecolor + playerState[idb1].duecolor;
  s2 = playerState[idw2].duecolor + playerState[idb2].duecolor;
  if ( (p1>0 || p2>0) && (s1*s2<0) ) r200 = 1;
  else r200 = 0;

  // check 80 points rule
  // WW & BB, oppure BB & WW, con diff=0 e duecolor
  // diversi nelle due coppie
  p1 = playerState[idw1].colorDiff + playerState[idb1].colorDiff;
  p2 = playerState[idw2].colorDiff + playerState[idb2].colorDiff;
  s1 = playerState[idw1].duecolor + playerState[idb1].duecolor;
  s2 = playerState[idw2].duecolor + playerState[idb2].duecolor;
  if ( (p1==0 && p2==0) && (s1*s2<0) ) r80 = 1;
  else r80 = 0;

  if (id1==idw1 && id2==idw2) {  // transposition
     d = CompareTranspose(idw1, idb1, idw2, idb2);
     if (d == 100000)
     { Exclamation("The exchange is illegal");
       return;
     }
     if (d>200 && r200) {
       sprintf(str, "D = %d", d);
       ans = WarningDialog(str, "Contrast with 200 points rule. Do you want to proceede anyway?");
       if (ans == 0) return;
	 }
	 else if (d>80 && r80) {
       sprintf(str, "D = %d", d);
       ans = WarningDialog(str, "Contrast with 80 points rule. Do you want to proceede anyway?");
       if (ans == 0) return;
	 }
     flagalign1 = 0;
     flagalign2 = 0;
     pairingNew[b1] = CodifyResult(id2, idb1, 9);
     pairingNew[b2] = CodifyResult(id1, idb2, 9);
  }
  else if (id1==idw1 && id2==idb2) {
     d = CompareInterchange(idw1, idb1, idw2, idb2, 1);
     if (d==100000)
     {  WarningMessage(  sss[S_PLAYNOTC] , sss[S_CLOSE]);
        return;
     }
     if (d>200 && r200) {
       sprintf(str, "D = %d", d);
       ans = WarningDialog(str, "This interchange do not met the 200 points rule\nDo you want to proceede anyway?");
       if (ans == 0) return;
	 }
	 else if (d>80 && r80) {
       sprintf(str, "D = %d", d);
       ans = WarningDialog(str, "This interchange do not met the 80 points rule\nDo you want to proceede anyway?");
       if (ans == 0) return;
	 }
     flagalign1 = 1;
     flagalign2 = 0;
     pairingNew[b1] = CodifyResult(id2, idb1, 9);
     pairingNew[b2] = CodifyResult(idw2, id1, 9);
  }
  else if (id1==idb1 && id2==idw2) {
     d = CompareInterchange(idw1, idb1, idw2, idb2, 0);
     if (d == 100000)
     { WarningMessage( sss[S_PLAYNOTC] , sss[S_CLOSE]);
       return;
     }
     if (d>200 && r200) {
       sprintf(str, "D = %d", d);
       ans = WarningDialog(str, "This interchange do not met the 200 points rule\nDo you want to proceede anyway?");
       if (ans == 0) return;
	 }
	 else if (d>80 && r80) {
       sprintf(str, "D = %d", d);
       ans = WarningDialog(str, "This interchange do not met the 80 points rule\nDo you want to proceede anyway?");
       if (ans == 0) return;
	 }
     flagalign1 = 0;
     flagalign2 = 1;
     pairingNew[b1] = CodifyResult(idw1, id2, 9);
     pairingNew[b2] = CodifyResult(id1, idb2, 9);
  }
  else if (id1==idb1 && id2==idb2) {
     d = CompareTranspose(idw1, idb1, idw2, idb2);
     if (d==100000)
     { WarningMessage( sss[S_PLAYNOTC] , sss[S_CLOSE]);
       return;
     }
     if (d>200 && r200) {
       sprintf(str, "D = %d", d);
       ans = WarningDialog(str, "This transposition do not met the 200 points rule\nDo you want to proceede anyway?");
       if (ans == 0) return;
	 }
	 else if (d>80 && r80) {
       sprintf(str, "D = %d", d);
       ans = WarningDialog(str, "This transposition do not met the 80 points rule\nDo you want to proceede anyway?");
       if (ans == 0) return;
	 }
     flagalign1 = flagalign2 = 1;
     pairingNew[b1] = CodifyResult(idw1, id2, 9);
     pairingNew[b2] = CodifyResult(idw2, id1, 9);
  }
*/

     int tid, trtg, tflag, xid, xrtg, xflag;
     float tscr, xscr;
     int bb0, ss0, bb1, ss1;
     bb0 = iboard[0];   ss0 = side[0];
     bb1 = iboard[1];   ss1 = side[1];
     a[ bb0 ].GetSide(ss0, tid, trtg, tscr, tflag); // get bb0-ss0 data
     a[ bb1 ].GetSide(ss1, xid, xrtg, xscr, xflag); // get bb1-ss1 data
     a[ bb0 ].SetSide(ss0, xid, xrtg, xscr, xflag); // set bb1-ss1 data in bb0-ss0
     a[ bb1 ].SetSide(ss1, tid, trtg, tscr, tflag); // set bb0-ss0 data in bb1-ss1

     // set white colors both selected board
     edtw.SetLabel(""); // <<= "";
     edtb.SetLabel(""); // <<= "";
     a[ bb0 ].rect.Color(White);
     a[ bb1 ].rect.Color(White);

     numlabel = 0;
}
