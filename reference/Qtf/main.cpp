#include <RichEdit/RichEdit.h>

using namespace Upp;

const char *examples[] = {
	"Normal [* bold] [/ italic] [_ underline] [` superscript] [, subscript]",
	"`[ `] \1[escaped]\1 [* bold]",
	"[A Arial (Sans-Serif)] [R Times New Roman (Serif)] [C Courier (Monospace)]",
	"[0 6pt ][1 8pt ][2 10pt ][3 12pt ][4 16pt ][5 20pt ][6 24pt ][7 28pt ][8 36pt ][9 48pt ]",
	"[!Tahoma! Tahoma]",
	"[+500 500dots]",
	"[@4 Green text] [$(255.220.200) Pink background]",
	"[%EN-US English language] [%CS-CZ Czech language]",
	"[^upp.sf.net^ Hyperlink] [Icompiler, linker; Index entry]",
	"[:label: Labeled paragraph]",
	"[< Left paragraph alignment]",
	"[= Center paragraph alignment]",
	"[> Right paragraph alignment]",
	"[# Justify alignment. Just some text to demosntrate it... "
	"Just some text to demonstrate it... Just some text to demonstrate it...]",
	"[l1000 Left margin 1000dots]",
	"[i1000 Indent 1000 dots."
	"Just some text to demonstrate it... Just some text to demonstrate it...]",
	"[r1000 Right margin 1000 dots."
	"Just some text to demonstrate it... Just some text to demonstrate it...]",
	"Paragraph&[b200 Before 200dots]",
	"[a200 After 200dots]&Paragraph",
	"[i200 [O0 bullet&][O1 bullet&][O2 bullet&][O3 bullet]]",
	"[O9i500 text bullet-|"
	"Just some text to demonstrate it... Just some text to demonstrate it..."
	"Just some text to demonstrate it... Just some text to demonstrate it...]",
	"[O9i200 [N1m.; -|level 1&][N1a -|level 2&][N1a -|level 2&][N1 -|level 1&][N1a -|level 2]]",
	"[~300~=.2000~>-3000 -|Normal tab-|Centered tab-|Right tab]",
	"{{1:2 A1:: A2:: B1:: B2}}",
	"{{2:1G4g100F5f50 A1:: A2:: B1:: B2}}",
	"{{1:2 A1::l40/60R6@3 A2::! B1:: B2}}",
	"{{1:1:1|2 A1::-2 A2:: A3:: B1:: B2:: B3}}",
	"{{1:2 A1:: A2:: B1:: {{1:2 a1:: a2:: a1:: a2}}}}",
	"{{1~a30/50o Round border}}",
	NULL 
};

GUI_APP_MAIN
{
	SetLanguage("ru-ru");

	String qtf = "&";
	for(int i = 0; examples[i]; i++)
		qtf << "{{1 [@3C1* " << DeQtf(AsCString(examples[i])) << "]:: " << examples[i] << "}}&&";
	RichEditWithToolBar e;
	e.SetQTF(qtf);
	TopWindow w;
	w.Sizeable().Zoomable();
	w.Add(e.SizePos());
	w.Run();
}
