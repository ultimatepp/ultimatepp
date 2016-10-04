#include "VegaMain.h"

void Avail3(One<Ctrl>& ctrl)
{
	ctrl.Create<OptionImage>().ThreeState().SetImage(VegaImg::imgYes(), VegaImg::imgNo(), VegaImg::imgMaybe());
}

int FilterCommaSpace(int c)
{
	if (c == ',' || c ==' ') return false;
	else return c;
}

VegaTab1::VegaTab1()
{   // set the widths of each coloumn
	// arr.ColumnWidths("25 3 5 12 8 8 2 3 3 3 3");
	CtrlLayout(*this, "");
	SizePos();
	              //id, label, width
	arr.AddColumn("ID", "ID", 3).HeaderTab().WhenAction = THISBACK1(SortRecord, 0);   //0
	arr.AddColumn("Name", "Name", 21).HeaderTab().WhenAction = THISBACK1(SortRecord, 1);   //1
	arr.AddColumn("Country", "Fed", 3);         //2
	arr.AddColumn("Birthday", "Birthday", 6);   //3
	arr.AddColumn("Gender", "G", 3);       //4
	arr.AddColumn("Title", "Title", 4);         //5
	arr.AddColumn("IDFIDE", "ID FIDE", 6);      //6
	arr.AddColumn("RatFIDE", "Rtg FIDE", 5).HeaderTab().WhenAction = THISBACK1(SortRecord, 7);    //7
	arr.AddColumn("IDNat", "ID Nat", 6);        //8
	arr.AddColumn("RatNat", "Rtg Nat", 5).HeaderTab().WhenAction = THISBACK1(SortRecord, 9);      //9
	arr.AddColumn("K", "K", 3);                 //10
	arr.AddColumn("Status", "Status", 5).Ctrls(Avail3);    //11
//	arr.AddColumn("Status", "Status", 5).Ctrls(Avail3).HeaderTab().WhenAction = THISBACK1(SortRecord, 12);    //11

//	arr.AddColumn("Avail", "Avail?", 4).Ctrls(Avail3).Ctrls(Callback1<One<Ctrl>&> factory);
//	arr.AddColumn("Extra","Extra",6).Ctrls(Extra2);

    arr.NoHorzGrid();
	arr.SetLineCy(VegaImg::imgYes().GetSize().cy+2); //option image adjustment...

	//arr.WhenLeftClick = THISBACK(PromptCell);
	arr.WhenLeftClick = THISBACK(FillFieldsFromRow);

	// color even rows
	arr.EvenRowColor();

	//drop list widget
	editTitle.Add("1", "WGM");
	editTitle.Add("2", "CM");
	editTitle.Add("3", "AA");

//	btnAdd <<= THISBACK(AddPlayer);  //maybe it's better to have all callbacks from main menu?...
	btnModify <<= THISBACK(ModifyPlayer);
	btnClear <<= THISBACK(MaskDefaultValue);

    tsex.SetImage(VegaImg::female, VegaImg::male); // false state, true state
	tsex <<= 1; //set to 'male' // tsex <<= THISBACK(Tsex);
	editName.SetFilter(FilterCommaSpace).MaxChars(25); //added filter
	editIDFIDE.MaxChars(8); //set limit
	editIDNat.MaxChars(8); //set limit
	editRatFIDE.MaxChars(4); //set limit
	editRatNat.MaxChars(4); //set limit
	kcoeff.MaxChars(2); //set limit
	//fill with default value
	MaskDefaultValue();
}


void VegaTab1::SortRecord(int column)
{
	arr.Sort(column);
}


void VegaTab1::AddPlayer() // body of the callback
{
	if ( IsNull(~editName) ) {
		PromptOK("The Name field cannot be empty!");
		return;
	}
	if ( IsNull(~editCountry) ) editCountry <<= "---";
	if ( IsNull(~editBirth)  ) editBirth <<= "00.00.00";
	if ( IsNull(~editIDFIDE) ) editIDFIDE <<= 0;

	arr.Add(1, ~editName, ~editCountry, ~editBirth,  ~tsex?"m":"f", ~editTitle,
	        ~editIDFIDE, ~editRatFIDE, ~editIDNat, ~editRatNat, ~kcoeff, true); // true==1, false==0
	arr.GoEnd();
	ActiveFocus(editName);
	MaskDefaultValue();
}


void VegaTab1::ModifyPlayer() // body of the callback
{	int int_row;
	int_row=arr.GetCursor();
	if (int_row < 0) return; // prevent the crash of the application when int_row = -1)
	if ( IsNull(~editName) ) {
		Exclamation("The Name field cannot be empty!");
		return;
	}
	arr.Set( int_row, 1, ~editName);
	arr.Set( int_row, 2, ~editCountry);
	arr.Set( int_row, 3, ~editBirth);
	arr.Set( int_row, 4, ~tsex? "m" : "f" );
	arr.Set( int_row, 5, ~editTitle);
	arr.Set( int_row, 6, ~editIDFIDE);
	arr.Set( int_row, 7, ~editRatFIDE);
	arr.Set( int_row, 8, ~editIDNat);
	arr.Set( int_row, 9, ~editRatNat);
	arr.Set( int_row, 10, ~kcoeff);

	ActiveFocus(editName);
	MaskDefaultValue();
}


void VegaTab1::MaskDefaultValue() // body of the callback
{
	editName <<= Null;
	editName.SetFilter(FilterCommaSpace);
	editCountry <<= "---";
	editBirth <<= "00.00.00";
	//tsex <<= 1;
	//editTitle <<= "GM";
	editIDFIDE <<= editRatFIDE <<= editIDNat <<= editRatNat <<= 0;
	kcoeff <<= 30;
	ActiveFocus(editName);
}

void VegaTab1::FillFieldsFromRow()
{
	int int_row;
	String s;
	int_row=arr.GetCursor();

	if (int_row < 0) return; // prevent the crash of the application when int_row = -1)
	editName    <<= arr.Get(int_row, "Name");      //1
	editCountry <<= arr.Get(int_row, "Country");   //2
	editBirth   <<= arr.Get(int_row, "Birthday");  //3

 	s           =  arr.Get(int_row, "Gender")  ;    //4
	if (s=="m") tsex <<= 1; else tsex <<= 0;    //4

	editTitle   <<=                arr.Get(int_row, "Title");     //5
	editIDFIDE  <<= StrIntValue( AsString(arr.Get(int_row, "IDFIDE")) );    //6
	editRatFIDE <<= StrIntValue( AsString(arr.Get(int_row, "RatFIDE")) );   //7
	editIDNat   <<= StrIntValue( AsString(arr.Get(int_row, "IDNat")) );     //8
	editRatNat  <<= StrIntValue( AsString(arr.Get(int_row, "RatNat")) );    //9
	kcoeff      <<= StrIntValue( AsString(arr.Get(int_row, "K")) );         //10
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
	int n=0;
	for(int i=0; i<arr.GetCount(); i++) {
		if ( arr.Get(i, "Status") ) n=n+1;
	}
	return n;
}
