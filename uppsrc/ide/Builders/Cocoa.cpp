#include "Builders.h"

#ifdef PLATFORM_OSX

#include <Draw/Draw.h>
#include <plugin/png/png.h>

void GccBuilder::CocoaAppBundle()
{
	if(!HasFlag("OSX") || !HasFlag("GUI"))
		return;

	String icns = GetFileFolder(GetFileFolder(target)) + "/Resources/icons.icns";
	RealizePath(icns);
	Time icns_tm = Nvl(GetFileTime(icns), Time::Low());
	bool convert_icons = false;

	SortedVectorMap<int, Image> imgs;
	for(FindFile ff(PackageDirectory(mainpackage) + "/icon*.png"); ff; ff.Next()) {
		Image m = StreamRaster::LoadFileAny(ff.GetPath());
		Size sz = m.GetSize();
		if(sz.cx == sz.cy) {
			imgs.Add(sz.cx, m);
			PutVerbose("Found icon " << ff.GetName());
			if((Time)ff.GetLastWriteTime() >= icns_tm)
				convert_icons = true;
		}
	}
	if(imgs.GetCount() && convert_icons) {
		String icons = AppendFileName(outdir, "icons.iconset");
		RealizeDirectory(icons);
		PutConsole("Exporting bundle icons to " + icons);
		
		for(String fn : {
			"icon_16x16.png",
			"icon_16x16@2x.png",
			"icon_32x32.png",
			"icon_32x32@2x.png",
			"icon_128x128.png",
			"icon_128x128@2x.png",
			"icon_256x256.png",
			"icon_256x256@2x.png",
			"icon_512x512.png",
			"icon_512x512@2x.png",
		}) {
			int n = atoi(~fn + strlen("icon_"));
			if(fn.Find("@2x") >= 0)
				n *= 2;
			int q = imgs.FindLowerBound(n);
			Image img = q >= 0 && q < imgs.GetCount() ? imgs[q] : imgs[imgs.GetCount() - 1];
			PutVerbose(String() << "Exporting " << fn << " from "
			                    << img.GetSize().cx << "x" << img.GetSize().cx);
			PNGEncoder().SaveFile(AppendFileName(icons, fn), Rescale(img, n, n));
		}

		String exec = String() << "iconutil --convert icns --output \"" << icns << "\" \"" << icons << "\"";
		Execute(exec);
	}

	if(IsNull(Info_plist)) {
		Info_plist
			<< "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			<< "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
			<< "<plist version=\"1.0\">\n"
			<< "<dict>\n"
			<< "    <key>CFBundleExecutable</key>\n"
			<< "    <string>" << GetFileName(target) << "</string>\n"
			<< "    <key>NSHighResolutionCapable</key>\n"
		    << "    <string>True</string>\n"
		    << "	<key>LSMinimumSystemVersion</key>\n"
			<< "    <string>10.13.0</string>\n"
		;
		if(imgs.GetCount())
			Info_plist
				<< "    <key>CFBundleIconFile</key>\n"
				<< "	<string>icons.icns</string>\n"
			;
		Info_plist
			<< "</dict>\n"
			<< "</plist>\n"
		;
	}
	String Info_plist_path = GetFileFolder(GetFileFolder(target)) + "/Info.plist";
	if(LoadFile(Info_plist_path) != Info_plist) {
		if(FileExists(Info_plist_path))
			Execute("defaults delete " + Info_plist_path); // Force MacOS to reload plist
		SaveFile(Info_plist_path, Info_plist);
		PutConsole("Saving " << Info_plist_path);
	}
}

#endif