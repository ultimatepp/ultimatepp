#include "PullText.h"



PullText::PullText()
{
	CtrlLayout(*this, "Window title");
	
	bt1 <<= THISBACK(OnButton1);
	bt2 <<= THISBACK(OnButton2);
}


	
void PullText::OnButton1(void)
{
	String qtf;
	qtf << txText1.GetQTF(CHARSET_ISO8859_1);
	//qtf << "[a NRE PARAGRAPH: &";
	DUMP(qtf);
	txText1.SetQTF(qtf);

}

void PullText::OnButton2(void)
{
	RichText clip=txText2.CopyText(0,10000);
	txText2.Clear();
	txText2.PasteText(clip);
}

GUI_APP_MAIN
{
	PullText().Run();
}

