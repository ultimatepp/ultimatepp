#ifndef flagSKELETON // replaces GUI host with empty VirtualGui (e.g. for converting GUI app to console)

#include <CtrlLib/CtrlLib.h>
#include <CtrlCore/CocoMM.h>

namespace Upp {

#ifdef GUI_COCOA

FileSelNative::FileSelNative()
{
	confirm = true;
	multi = hidden = false;
	activetype = 0;
}

FileSelNative& FileSelNative::AllFilesType()
{
	return Type(t_("All files"), "*.*");
}

bool FileSelNative::Execute0(int open, const char *title)
{
	path.Clear();
	Ctrl::ReleaseCtrlCapture();
	if(!title)
		title = open ? t_("Open") : t_("Save as");
	CFRef<CFStringRef> mmtitle = CFStringCreateWithCString(NULL, title, kCFStringEncodingUTF8);

	if(open) {
		NSOpenPanel *panel = [NSOpenPanel openPanel];
		[panel setAllowsMultipleSelection: multi ? YES : NO];
		[panel setMessage:(NSString *)~mmtitle];
		if(open == 2) {
			[panel setCanChooseDirectories: YES];
			[panel setCanChooseFiles: NO];
		}
		else {
			[panel setCanChooseDirectories: NO];
			[panel setCanChooseFiles: YES];
		}
		if([panel runModal] == NSModalResponseOK) {
			NSArray* urls = [panel URLs];
			for(int i = 0; i < urls.count; i++)
				path.Add([[urls objectAtIndex:i] fileSystemRepresentation]);
		}
	}
	else {
		NSSavePanel *panel = [NSSavePanel savePanel];
		[panel setMessage:(NSString *)~mmtitle];
		if([panel runModal] == NSModalResponseOK)
			path.Add([[panel URL] fileSystemRepresentation]);
	}
	return path.GetCount();
}

#endif

}
#endif