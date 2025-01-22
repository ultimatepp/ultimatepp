#include <CtrlLib/CtrlLib.h>
#include <plugin/pcre/Pcre.h>

using namespace Upp;

#define LAYOUTFILE <RegExp2/RegExpExtDemoLayout.lay>
#include <CtrlCore/lay.h>

class mainwindowDlg : public Withmainwindow<TopWindow> {
	typedef mainwindowDlg CLASSNAME;
	
public:
	mainwindowDlg();
	void RunRegExp();
	void Set_Examples(int i);
	
	void match_callback_fun(Vector<String>& v) {
		for(int i=0, i1=v.GetCount(); i<i1; i++)
			v[i]=Format("Match%d",i+1);
	}
};

mainwindowDlg::mainwindowDlg()
{
	CtrlLayout(*this, "RegExp Extension Demo");
	Sizeable().Zoomable();
	HCenterPos(800).VCenterPosZ(400);
	
	
	for(int i=1; i<6; i++)
		d1.Add(i, Format("TEST-%d",i));
	
	d2.Add(1, "Disable Resolving Back-Reference")
	  .Add(2, "Enable Resolving Back-Reference")
	  .Add(3, "Use Callback Function");
	
	
	d1 << [=] { Set_Examples((int)~d1); };
	b1 << THISFN(RunRegExp);
	b2 << [=] { t4.Clear(); };

	d1.SetData(4);
	
	Set_Examples(4);
}

void mainwindowDlg::RunRegExp()
{
	RegExp regx;
	RegExp reg("\\((.*?)\\)");
	
	String t(t1.GetData());
	
	int n=-101;

	
	switch((int)d2.GetData()){
		
		case 1:	// no backref resolved
		
			regx.SetPattern(t2.GetData());
			
			if((int)o1.GetData() == 0)
				 n=regx.Replace(t, (String)t3.GetData());
			else n=regx.ReplaceGlobal(t, (String)t3.GetData());
			
		break;

		case 2:	// backref resolved
		
			regx.SetPattern(t2.GetData());
			
			if((int)o1.GetData() == 0)
				 n=regx.Replace(t, (String)t3.GetData(), true);
			else n=regx.ReplaceGlobal(t, (String)t3.GetData(), true);
			
		break;
				
		case 3: // using callback funtion
		
			regx.SetPattern(t2.GetData());
			
			if((int)o1.GetData() == 0)
				 n=regx.Replace(t, THISFN(match_callback_fun));
			else n=regx.ReplaceGlobal(t, THISFN(match_callback_fun));
			
		break;
	};
		
	
	
	String output(t4.Get());
	
	output
	      <<"Regular Expression:["<<t2.GetData()
	      <<"]\nReplacement Count="<<n
	      <<"\nInput: "<<t1.GetData()
	      <<"\nOutput: "<<t<<"\n\n\n";
	      
	t4.Set(output);
	t4.ScrollEnd();	
}


void mainwindowDlg::Set_Examples(int i)
{
	switch(i){
		
		case 1:	
			t1.SetData("A QUICK BROWN FOX JUMPS OVER THE LAZY DOG")	;
			t2.SetData("(\\w)");
			t3.SetData("(#)");
			d2.SetData(1);
		break;
					
		case 2:
			t1.SetData("31-1-2000 13-2-2010 1-12-2012");
			t2.SetData("(\\d+)-(\\d+)-(\\d+)");
			t3.SetData("(\\2)(\\1)(\\3)");		
			d2.SetData(2);
			o1.SetData(true);
		break;
		
		case 3:
			t1.SetData("A QUICK BROWN FOX JUMPS OVER THE LAZY DOG");
			t2.SetData("(QUICK)");
			t3.SetData("(SLOW)");
			d2.SetData(1);
		break;
		
		case 4:
			t1.SetData("A QUICK BROWN FOX JUMPS OVER THE LAZY DOG");
			t2.SetData("(QUICK)\\s+(BROWN)");
			t3.SetData("(\\2ISH)(\\1)");
			d2.SetData(2);
		break;
					
		
		case 5:
			t1.SetData("A QUICK BROWN FOX JUMPS OVER THE LAZY DOG");
			t2.SetData("(\\w+)");
			t3.SetData("THIS IS EXAMPLE OF THE USE OF CALLBACK FUNCTION");
			d2.SetData(3);
		break;
	}

}



GUI_APP_MAIN
{
	mainwindowDlg w;
	
	w.Run();
}
