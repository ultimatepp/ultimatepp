#include "E011.h"

// keys
#define KEYGROUPNAME "E011"
#define KEYNAMESPACE E011Keys
#define KEYFILE      <E011/e011.key>
#include             <CtrlLib/key_source.h>

using namespace E011Keys;

void App::MainMenu(Bar& bar) {
	bar.Add(t_("File"), THISBACK(FileMenu));
	bar.Add(t_("Settings"), THISBACK(OptionMenu));
	bar.Add(t_("Comunication"), THISBACK(CommMenu));
	bar.Add(t_("Help"), THISBACK(helpMenu));
}

void App::FileMenu(Bar& bar) {
	bar.Add(AK_NEWFILE, commonImg::new_1_16(), THISBACK(NewFile)).Enable(!workOpen);
	bar.Add(AK_OPENFILE, commonImg::open_1_16(), THISBACK(OpenFile)).Enable(!workOpen);
	bar.Add(AK_SAVE, THISBACK(SaveFile)).Enable(workOpen);
	bar.Add(AK_CLOSEFILE, commonImg::close_1_16(), THISBACK(CloseFile)).Enable(workOpen);
	//bar.Separator();
	//bar.Add(AK_DELETEFILE, commonImg::delete_1_16(), THISBACK(DeleteFile)).Enable(!workOpen);
	bar.Separator();
	bar.Add(AK_QUIT, commonImg::quit_1_16(), THISBACK(Quit));
}

void App::OptionMenu(Bar& bar) {
	bar.Add(!port.IsOpen(), t_("Serial port"), AppImg::Port_1_16(), THISBACK(ComChange));
	bar.Separator();
	bar.Add(AK_PERCORSI, commonImg::folder_1_16(), THISBACK(SetupPath));
	bar.Add(AK_TASTIVELOCI, commonImg::keys_1_16(), callback(EditKeys));
}

void App::CommMenu(Bar& bar) {
	bar.Add(t_("On-line"), THISBACK(GoOnline)).Check(port.IsOpen());
	bar.Add(port.IsOpen(), t_("Animation"), THISBACK(Animation));
	bar.Separator();
	bar.Add(port.IsOpen(), t_("Download"), AppImg::download_16(), THISBACK(DownloadMap));
	bar.Add(port.IsOpen(), t_("Upload"), AppImg::upload_16(), THISBACK(UploadMap));
	bar.Separator();
	bar.Add(port.IsOpen(), t_("Verify"), AppImg::verify_16(), THISBACK(VerifyMap));
}

void App::helpMenu(Bar& menu) {
	//menu.Add(AK_HELPINDEX, commonImg::help_1_16(), THISBACK(helpIndex));
	//menu.Separator();
	menu.Add(AK_INFO, THISBACK(About));
}

void App::setToolsBar() {
	toolbar.Set(THISBACK(toolsBar));
	return;
}

void App::toolsBar(Bar& bar) {

	bar.Add(!workOpen, AK_OPENFILE, commonImg::open_1_16(), THISBACK(OpenFile));
	bar.Add(!workOpen, AK_NEWFILE, commonImg::new_1_16(), THISBACK(NewFile));
	bar.Add(workOpen, AK_SAVE, CtrlImg::save(), THISBACK(SaveFile));
	bar.Add(workOpen, AK_CLOSEFILE, commonImg::close_1_16(), THISBACK(CloseFile));
	bar.Separator();
	if(port.IsOpen()) bar.Add(AppImg::offline_16(), THISBACK(GoOnline));
	else bar.Add(AppImg::online_16(), THISBACK(GoOnline));
	bar.Add(AppImg::anim_16(), THISBACK(Animation));
	bar.Add(port.IsOpen(), AppImg::download_16(), THISBACK(DownloadMap));
	bar.Add(port.IsOpen(), AppImg::upload_16(), THISBACK(UploadMap));
	bar.Add(port.IsOpen(), AppImg::verify_16(), THISBACK(VerifyMap));
	//bar.Separator();
	//bar.Add(AK_HELPINDEX, commonImg::help_1_16(), THISBACK(helpIndex));
	return;
}
