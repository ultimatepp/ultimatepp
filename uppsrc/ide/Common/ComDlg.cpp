#include "Common.h"

#define IMAGECLASS IdeCommonImg
#define IMAGEFILE  <ide/Common/common.iml>
#include <Draw/iml_source.h>

void IdeFileIcon0(bool dir, const String& filename, Image& img)
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
	if(ext == ".html")
		img = IdeCommonImg::html();
	if(ext == ".css")
		img = IdeCommonImg::css();
	if(ext == ".witz")
		img = IdeCommonImg::witz();
	if(ext == ".js")
		img = IdeCommonImg::js();
	if(ext == ".json")
		img = IdeCommonImg::json();
	if(ext == ".xml")
		img == IdeCommonImg::xml();
	if(ext == ".usc")
		img = IdeCommonImg::Script();
	if(ext == ".lng" || ext == ".lngj" || ext == ".t" || ext == ".jt")
		img = IdeCommonImg::Language();
	if(ext == ".icpp")
		img = IdeCommonImg::ISource();
	if(ext == ".c" || ext == ".cpp" || ext == ".cc" || ext == ".cxx"
	               || ext == ".m" || ext == ".mm")  //should later have diff img?
		img = IdeCommonImg::Source();
	if(ext == ".sch")
		img = IdeCommonImg::Sch();
	if(ext == ".ddl")
		img = IdeCommonImg::Ddl();
	if(ext == ".sql")
		img = IdeCommonImg::Sql();
	if(filename == "Copying")
		img = IdeCommonImg::License();
	if(filename == "main.conf")
		img = IdeCommonImg::MainConf();
}

struct sImageAdd : ImageMaker {
	Image i1, i2;

	virtual String Key() const;
	virtual Image Make() const;
};

String sImageAdd::Key() const
{
	int64 a[2];
	a[0] = i1.GetSerialId();
	a[1] = i2.GetSerialId();
	return String((const char *)&a, 2 * sizeof(int64));
}

Image sImageAdd::Make() const
{
	Image dest = i1;
	Over(dest, Point(0, 0), i2, i2.GetSize());
	return dest;
}

Image ImageOver(const Image& back, const Image& over)
{
	sImageAdd h;
	h.i1 = back;
	h.i2 = over;
	return MakeImage(h);
}

Image IdeFileImage(const String& filename, bool fast, bool include_path)
{
	Image img = CtrlImg::File();
	IdeFileIcon0(false, filename, img);
	if(fast)
		img = ImageOver(img, IdeCommonImg::Fast());
	if(include_path)
		img = ImageOver(img, IdeCommonImg::IncludePath());
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
	fs.Type("C++ files (*.cpp *.h *.hpp *.c *.C *.cc *.cxx *.icpp)", "*.cpp *.h *.hpp *.c *.C *.cc *.cxx *.icpp");
	fs.Type("Layout files (*.lay)", "*.lay");
	fs.Type("Image files (*.iml)", "*.iml");
	fs.Type("Language files (*.lng)", "*.lng");
	fs.Type("Web development files (*.html *.js *.css *.witz)", "*.html *.js *.css *.witz");
	fs.Type("Other special files (*.sch *.usc *.rc *.brc *.upt)", "*.sch *.usc *.rc *.brc *.upt");
	String mask = "*.cpp *.h *.hpp *.c *.C *.cc *.cxx *.icpp *.lay *.iml *.lng *.sch *.usc *.rc *.brc *.upt *.html *.js *.css *.witz";
	fs.Type("All source files (" + mask + ")", mask);
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

void ShellOpenFolder(const String& dir)
{
	#if defined(PLATFORM_WIN32)
		LaunchWebBrowser(dir);
	#elif __APPLE__
		String tempDir = dir;
		tempDir.Replace(" ", "\\ ");

		IGNORE_RESULT(
			system("open " + dir + " &")
		);
	#else
		String tempDir = dir;
		tempDir.Replace(" ", "\\ ");
		
		IGNORE_RESULT(
			system("xdg-open " + tempDir + " &")
		);
	#endif
}
