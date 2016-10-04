#include "xadrez.h"

#define IMAGEFILE <xadrez/xadrez.iml>
#define IMAGECLASS XadrezImg
#include <Draw/iml_source.h>

xadrez::xadrez()
{
	Progress pi;

	Title("Xadrez Postal 1.0").Sizeable().Zoomable();
	estadoxadrezpostal = "Bem vindo ao Xadrez Postal !";
	AddFrame(menu);
	menu.Add("Ajuda", THISBACK(ajuda));
	menu.Add("Sobre Xadrez Postal", THISBACK(sobre));
	menu.Add("Sair",THISBACK(sair));
    AddFrame(estadoxadrezpostal);
	Add(arvoredeescolha.SizePos());
	arvoredeescolha.MultiSelect().NoBackground().Transparent();
    arvoredeescolha.WhenOpen = THISBACK(OpenDir);
    arvoredeescolha.WhenClose = THISBACK(CloseDir);
    arvoredeescolha.WhenCursor = THISBACK(ShowPath);
    arvoredeescolha.WhenLeftDouble = THISBACK(TratarOpcoesDeArvore);
    arvoredeescolha.SetRoot(CtrlImg::Dir(), "Entrada");
    pi.AlignText(ALIGN_LEFT);
    LoadTree(0, "Entrada", pi);
    BackPaint();
}

void xadrez::ajuda()
{
    PromptOK("Help do Xadrez Postal 1.0");
}

void xadrez::sobre()
{
	PromptOK("[A5 Xadrez Postal 1.0]&Using [*^www://upp.sf.net^ Ultimate`+`+] technology.");
}

void xadrez::sair()
{

	if(PromptOKCancel("Deseja Sair do Xadrez Postal")) Close();
}

void xadrez::configurar()
{
	PromptOK("Configuração do Sitema Xadrez Postal");
}

void xadrez::OpenDir(int id)
{
	String TextoDeFace;
	TextoDeFace = "Carregar Jogo";
    arvoredeescolha.Add(id,CtrlImg::Dir(), TextoDeFace,false);
    TextoDeFace = "Configurar Xadrez Postal";
    arvoredeescolha.Add(id,CtrlImg::Dir(), TextoDeFace,false);
    TextoDeFace = "Comunicação";
    arvoredeescolha.Add(id,CtrlImg::Dir(), TextoDeFace,false);
    TextoDeFace = "Manual de Instrução";
    arvoredeescolha.Add(id,CtrlImg::Dir(), TextoDeFace,false);
}

void xadrez::CloseDir(int id)
{
	arvoredeescolha.RemoveChildren(id);
}

void xadrez::LoadTree(int parent, const String& path, Progress& pi)
{
	if(pi.StepCanceled()) return;
}

void xadrez::TratarOpcoesDeArvore()
{
	Value raiz00 = "Carregar Jogo";
	Value raiz01 = "Configurar Xadrez Postal";
	Value raiz02 = "Comunicação";
	Value raiz03 = "Manual de Instrução";

	if(arvoredeescolha.Get() == raiz00) { PromptOK("TESTE00"); }
	if(arvoredeescolha.Get() == raiz01) { PromptOK("TESTE01"); }
	if(arvoredeescolha.Get() == raiz02) { PromptOK("TESTE02"); }
	if(arvoredeescolha.Get() == raiz03) { PromptOK("TESTE03"); }
}

void xadrez::ShowPath()
{
	estadoxadrezpostal = ~arvoredeescolha;
}

void xadrez::SetOption(int id)
{
}

GUI_APP_MAIN
{
	SetLanguage(LNG_ENGLISH);
	xadrez().Sizeable().MinimizeBox().Run();
}
