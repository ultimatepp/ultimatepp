#include "Common.h"

void IdeFileIcon0(bool dir, const String& filename, Image& img, bool fast = false)
{
	if(dir) return;
	String ext = ToLower(GetFileExt(filename));
	if(ext == ".h" || ext == ".hpp" || ext == ".hh" || ext == ".hxx")
		img = IdeCommonImg::Header();
	for(int i = 0; i < GetIdeModuleCount(); i++) {
		Image m = GetIdeModule(i).FileIcon(filename);
		if(!IsNull(m)) {
			img = m;
			break;
		}
	}
	if(ext == ".usc")
		img = IdeCommonImg::Script();
	if(ext == ".lng" || ext == ".lngj" || ext == ".t" || ext == ".jt")
		img = IdeCommonImg::Language();
	if(fast)
		img = IdeCommonImg::Fast();
	if(ext == ".icpp")
		img = fast ? IdeCommonImg::FastISource() : IdeCommonImg::ISource();
	if(ext == ".c" || ext == ".cpp" || ext == ".cc" || ext == ".cxx")
		img = fast ? IdeCommonImg::FastSource() : IdeCommonImg::Source();
	if(ext == ".sch")
		img = IdeCommonImg::Sch();
	if(filename == "Copying")
		img = IdeCommonImg::License();
}

Image IdeFileImage(const String& filename, bool fast)
{
	Image img = CtrlImg::File();
	IdeFileIcon0(false, filename, img, fast);
	return img;
}

void IdeFileIcon(bool dir, const String& filename, Image& img)
{
	IdeFileIcon0(dir, filename, img);
}

void IdeFs(FileSel& fs) {
	fs.WhenIcon = callback(IdeFileIcon);
	fs.AllFilesType();
	fs.Multi();
	fs.NoAsking();
	fs.ReadOnlyOption();
}

void SourceFs(FileSel& fs) {
	String mask = "*.cpp *.h *.hpp *.c *.C *.cc *.cxx *.lay *.iml *.lng *.sch *.usc *.rc *.brc *.upt";
	fs.Type("Source files (" + mask + ")", mask);
	IdeFs(fs);
}

FileSel& AnySourceFs() {
	static FileSel *fsp;
	if(!fsp) {
		static FileSel fs;
		SourceFs(fs);
		fsp = &fs;
	}
	fsp->Multi();
	return *fsp;
}

FileSel& AnyPackageFs() {
	static FileSel fs;
	static bool b;
	if(!b) {
		fs.Type("Ultimate++ package (*.upp)", "*.upp");
		fs.AllFilesType();
		b = true;
	}
	return fs;
}

FileSel& BasedSourceFs() {
	static FileSel *fsp;
	if(!fsp) {
		static FileSel fs;
		SourceFs(fs);
		fsp = &fs;
	}
	fsp->Multi();
	return *fsp;
}

FileSel& OutputFs() {
	static FileSel *fsp;
	if(!fsp) {
		static FileSel fs;
		fs.AllFilesType();
		fs.Type("Various (*.log *.map *.ini *.sql)", "*.log *.map *.ini *.sql");
		fs.Type("Log files (*.log)", "*.log");
		fs.Type("Map files (*.map)", "*.map");
		fs.Type("Ini files (*.ini)", "*.ini");
		fs.Type("SQL scripts (*.sql)", "*.sql");
		IdeFs(fs);
		fsp = &fs;
	}
	fsp->Multi();
	return *fsp;
}
