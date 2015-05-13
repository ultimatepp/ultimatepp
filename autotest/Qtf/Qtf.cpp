#include <RichText/RichText.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	TimeStop tm;

	StdLogSetup(LOG_COUT|LOG_FILE);

	String outdir = GetHomeDirFile("autotest.qtf");
	DeleteFolderDeep(outdir);
	RealizeDirectory(outdir);
	
	// to properly test PDF on various hosts, we need the same fonts to be installed/used
	Index<String> face;
	for(int i = 0; i < Font::GetFaceCount(); i++)
		face.Add(Font::GetFaceName(i));
//	DDUMPC(face);
	ASSERT(face.Find("Droid Sans") >= 0);
	ASSERT(face.Find("Droid Sans Mono") >= 0);
	ASSERT(face.Find("Droid Serif") >= 0);
	
	Font::SetFace(Font::SERIF, "Droid Serif");
	Font::SetFace(Font::SANSSERIF, "Droid Sans");
	Font::SetFace(Font::MONOSPACE, "Droid Sans Mono");

	for(FindFile ff(GetDataFile("*.qtf")); ff; ff.Next()) {
		String qtf = LoadFile(ff.GetPath());
		LOG(ff.GetName() << ' ' << qtf.GetCount());
		
		ASSERT(qtf.GetLength() > 10000);
		RichText txt = ParseQTF(qtf);
		String qtf2 = AsQTF(txt);
		ASSERT(qtf2 == qtf);
		SaveFile(AppendFileName(outdir, ff.GetName()), qtf2);
		
		String pdfname = ForceExt(ff.GetName(), ".pdf");
		String pdf = Pdf(txt);
		SaveFile(AppendFileName(outdir, pdfname), pdf);
		
		String h = LoadDataFile(pdfname);
		ASSERT(h.GetCount() == pdf.GetCount());
		int q = h.ReverseFind("trailer");
		ASSERT(pdf.Mid(0, q) == h.Mid(0, q));
	}

	DeleteFolderDeep(outdir);
	
	LOG("========== OK in " << tm << " s");
}
