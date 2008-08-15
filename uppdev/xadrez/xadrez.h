#ifndef _xadrez_xadrez_h
#define _xadrez_xadrez_h

#include <CtrlLib/CtrlLib.h>

#define IMAGEFILE <xadrez/xadrez.iml>
#define IMAGECLASS XadrezImg
#include <Draw/iml_header.h>

using namespace Upp;

#define LAYOUTFILE <xadrez/xadrez.lay>
#include <CtrlCore/lay.h>

#define           MAXESC    10

class xadrez : public TopWindow {
public:
	typedef xadrez CLASSNAME;
	// Variaveis do Programa
	MenuBar                   menu;
	StatusBar   estadoxadrezpostal;
	TreeCtrl       arvoredeescolha;
	String         OpcoesDeEscolha;
	Image                      img;

	xadrez();

	void Paint(Draw& w)
	{
     	Size sz = GetSize();
		w.DrawRect(sz, SWhite);
		Size isz = XadrezImg::vacat().GetSize();
		w.DrawImage((sz.cx - isz.cx) / 2, (sz.cy - isz.cy) / 2, XadrezImg::vacat());
		//Icon(Xadrez::vulpix());
	    //Tip("This is U++ tray icon");
	}

	virtual void MouseMove(Point pos, dword flags)
	{
		Refresh();
	}

	virtual void LeftDown(Point pos, dword flags)
	{
		Refresh();
	}

	void LocalMenu(Bar& bar) {
		bar.Add("Sair", THISBACK(Exit));
	}

	void RightDown(Point, dword) {
		MenuBar::Execute(THISBACK(LocalMenu));
	}

	void Exit()
	{
		if(PromptOKCancel("Sair do Xadrez Postal?"))
			Break();
	}

	void ajuda();
	void sobre();
	void configurar();
	void sair();
	void OpenDir(int id);
	void CloseDir(int id);
	void LoadTree(int parent, const String& path, Progress& pi);
	void ShowPath();
	void TratarOpcoesDeArvore();
	void SetOption(int id);
};

#endif
