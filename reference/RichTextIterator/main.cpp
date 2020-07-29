#include <CtrlLib/CtrlLib.h>

using namespace Upp;

Vector<Image> ExtractImages(const RichText& txt)
{
	struct Iter : RichText::Iterator {
		Vector<Image> m;
		virtual bool operator()(int, const RichPara& para) {
			for(int i = 0; i < para.GetCount(); i++) {
				const RichPara::Part& p = para[i];
				if(p.object)
					m.Add(p.object.ToImage(p.object.GetPixelSize()));
			}
			return false;
		}
	} iter;

	txt.Iterate(iter);
	
	return pick(iter.m);
}

GUI_APP_MAIN
{
	int i = 0;
	for(Image m : ExtractImages(ParseQTF(LoadDataFile("Test.qtf"))))
		PNGEncoder().SaveFile(GetHomeDirFile(String() << "ExtractedImage_" << ++i << ".png"), m);
}
