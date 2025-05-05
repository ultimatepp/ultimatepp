#include <RichText/RichText.h>

// Note: .pdf depends on test platform, needs to be updated
// It is in ~/autotest.qtf, needs to be copied (after checking) to autotest/Qtf

using namespace Upp;

CONSOLE_APP_MAIN
{
	TimeStop tm;

	StdLogSetup(LOG_COUT|LOG_FILE);

	String outdir = GetHomeDirFile("autotest.qtf");
	DeleteFolderDeep(outdir);
	RealizeDirectory(outdir);

	String qtf = LoadDataFile("test.qtf");

	DDUMP(qtf.GetCount());
	
	ASSERT(qtf.GetLength() > 10000);
	RichText txt = ParseQTF(qtf);
	String qtf2 = AsQTF(txt);
	ASSERT(qtf2 == qtf);
	
	String pdf = Pdf(txt);
	String pdfpath = AppendFileName(outdir, "test.pdf");
	SaveFile(pdfpath, pdf);
	
	DDUMP(pdfpath);
	
	String h = LoadDataFile("test.pdf");
	ASSERT(h.GetCount() == pdf.GetCount());
	int q = h.ReverseFind("trailer");
	ASSERT(pdf.Mid(0, q) == h.Mid(0, q));

	DeleteFolderDeep(outdir);
	
	LOG("========== OK in " << tm << " s");
}
