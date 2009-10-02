#include "hiddisk.h"
#include <cstdlib>

int PromptYesNo2(const char *msg, const char *title){
	BeepQuestion();
	return Prompt(callback(LaunchWebBrowser),
	              title, CtrlImg::question(), msg, false,
	              t_("是(&Y)"), t_("否(&N)"), NULL);
}

void PromptOK2(const char *msg, const char *title) {
	BeepInformation();
	Prompt(title, CtrlImg::information(), msg, t_("OK"));
}

hiddisk::hiddisk()
{
	CtrlLayout(*this, "隐藏盘管理器");
	this->btn_start <<= THISBACK(start);
	this->btn_stop <<= THISBACK(stop);
	this->btn_setup <<= THISBACK(setup);
	this->btn_uninst <<= THISBACK(uninst);
}

GUI_APP_MAIN
{
	hiddisk().Run();
}

void hiddisk::start(){
	std::system("if exist d:\\recycler\\hiddisk\\nul subst.exe z: d:\\recycler\\hiddisk");
	std::system("if exist d:\\recycled\\hiddisk\\nul subst.exe z: d:\\recycled\\hiddisk");
	std::system("if exist d:\\$RECYCLE.BIN\\hiddisk\\nul subst.exe z: d:\\$RECYCLE.BIN\\hiddisk");
}

void hiddisk::stop(){
	std::system("subst z: /d");
}

void hiddisk::setup(){
	if(PromptYesNo2("你确定要安装吗？", "提示")){
		std::system("if exist d:\\recycler\\nul mkdir d:\\recycler\\hiddisk");
		std::system("if exist d:\\recycled\\nul mkdir d:\\recycled\\hiddisk");
		std::system("if exist d:\\$RECYCLE.BIN\\nul mkdir d:\\$RECYCLE.BIN\\hiddisk");
	}else{
		PromptOK2("安装已经被您取消", "提示");
	}
}

void hiddisk::uninst(){
	if(PromptYesNo2("此操作将会删除隐藏盘里面的所有数据，你确定吗", "提示")){
		stop();
		std::system("if exist d:\\recycler\\hiddisk rd /s /q d:\\recycler\\hiddisk");
		std::system("if exist d:\\recycled\\hiddisk rd /s /q d:\\recycled\\hiddisk");
		std::system("if exist d:\\$RECYCLE.BIN\\hiddisk rd /s /q d:\\$RECYCLE.BIN\\hiddisk");
	}else{
		PromptOK2("卸载操作已经被您取消", "提示");
	}
}
