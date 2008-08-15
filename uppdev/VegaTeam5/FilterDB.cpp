#include "VegaMain.h"

extern int filter[FIELDS][2]; // [][0]=on/off status, [][1]=start column

SetFilterDlg::SetFilterDlg(String& testdb)
{   int i;
	String name[] = { "Name", "Rating", "Title", "K coeff", "Federation", "Birthday", "ID code", "Gender" };
	String lenght[] = { "25", "4", "2", "2", "3", "4", "8", "1"};

	CtrlLayout(*this, "Set Database Filter");

	for (i=0; i<FIELDS; i++) {

	    Add(tbutton[i]);
	    tbutton[i].SetLabel( name[i] ).LeftPosZ(55, 68).TopPosZ(61+i*24, 15);
    	if  (filter[i][0])  tbutton[i] <<= true;

		Add(edit[i]);
		edit[i].Min(0).LeftPosZ(147, 32).TopPosZ(60+i*24, 19);

        Add(label[i]);
        label[i].SetLabel( lenght[i] ).SetAlign(ALIGN_CENTER).SetFrame(ThinInsetFrame()).LeftPosZ(215, 64).TopPosZ(60+i*24, 19);
	}
    tbutton[0] <<= true;  tbutton[0].Disable(); // name always ON
	spyfilter.Set("012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\n");
	spyfilter.Insert(142, testdb);

    //set current value
	for (i=0; i<FIELDS; i++) {
	   	if  (filter[i][0])  {
	   	     tbutton[i] <<= true;
	   	     edit[i] <<= AsString(filter[i][1]);
	   	}
	   	else edit[i] <<= AsString(0);
 	    edit[i].WhenAction = THISBACK2(SetSelection, i, StrIntValue(lenght[i]) );
	}
	btnDone <<= THISBACK(DoneFilterCB);
	btnLoad <<= THISBACK(LoadFilterCB);
	btnSave <<= THISBACK(SaveFilterCB);
	btnClose.WhenAction = THISBACK(Close);
}


void SetFilterDlg::LoadFilterCB()
{
	FileSel fs; // for Linux and windows
	String fn;
	fs.Type( t_("File Filter"), "*.flt").DefaultExt("flt");
/*
  fs.dir <<= "C:\\Vega\\dbase"; // seleziono la directory di partenza
*/
	if(fs.ExecuteOpen())
	{
	  fn = ~fs;
      FileIn in(fn);
      LoadFilter(fn);
	}
}

void SetFilterDlg::LoadFilter(String fn)
{  int i, status, c;
   FileIn in(fn);
   if (in) {
    for (i=0; i<FIELDS; i++)
      { // get date
        Vector<String> field = Split(in.GetLine(), ' ');
        status =  StrIntValue( field[0] ); // tbutton on/off
        c =  StrIntValue( field[1] );
        if (status) tbutton[i] <<= true;
        else  tbutton[i] <<= false;
        edit[i] <<= AsString (c);

        filter[i][0] = status;
        filter[i][1] = c;
	  }
	  in.Close();
   } else Exclamation(t_("The default FIDE filter was not found. & Please set it to retrieve the players"));

}


void SetFilterDlg::SaveFilterCB()
{   int i;
	String fn, s;
    FileSel fs;
    fs.Type( t_("File Filter"), "*.flt").DefaultExt("flt").ExecuteSaveAs("Save as...");
    fn = ~fs;
    if ( IsNull(fn) ) return;
    FileOut out(fn);

    for (i=0; i<FIELDS; i++) {
        if ( tbutton[i] ) {
            filter[i][0] = 1;
            filter[i][1] = StrIntValue( "0" + AsString(~edit[i]));
        }
        else {
            filter[i][0] = 0;
            filter[i][1] = 0;
        }
        out.PutLine( NFormat("%d  %d", filter[i][0], filter[i][1]) );
    }
    out.Close();
}

void SetFilterDlg::DoneFilterCB()
{ int i;
  for (i=0; i<FIELDS; i++)
      { if ( tbutton[i] ) {
            filter[i][0] = 1;
            filter[i][1] = StrIntValue( "0" + AsString(~edit[i] ));
        }
        else {
            filter[i][0] = 0;
            filter[i][1] = 0;
        }
      }
  Close();
}

void SetFilterDlg::SetSelection(int i, int l)
{ int start = StrIntValue( AsString(~edit[i]) );
  spyfilter.SetSelection(start, start+l);
}
