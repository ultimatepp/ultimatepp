#include "CocoMM.h"

#ifdef PLATFORM_COCOA

#define LLOG(x)  // LOG(x)

namespace Upp {

NSString *PasteboardType(const String& fmt)
{
	return decode(fmt, "text", NSPasteboardTypeString, "image", NSPasteboardTypePNG,
	                   "files", NSFilenamesPboardType, "url", NSURLPboardType,
	                   "rtf", NSPasteboardTypeRTF,
	                   [NSString stringWithUTF8String:~fmt]);
}

};

@interface CocoClipboardOwner : NSObject {
	@public
	Upp::VectorMap<Upp::String, Upp::ClipData> data;
}
@end

@implementation CocoClipboardOwner
-(void)pasteboard:(NSPasteboard *)sender provideDataForType:(NSString *)type
{
	auto render = [&](const Upp::String& fmt) -> Upp::String {
		int q = data.Find(fmt);
		if(q < 0)
			return Upp::Null;
		return data[q].Render();
	};
	
	id pasteboard = [NSPasteboard generalPasteboard];
  
	if([type isEqualTo:NSPasteboardTypeString]) {
	    [pasteboard setString:[NSString stringWithUTF8String:~render("text")]
	                forType:type];
		return;
	}

	Upp::String raw = render([type isEqualTo:NSPasteboardTypePNG] ? "image" :
	                         [type isEqualTo:NSPasteboardTypeRTF] ? "rtf" :
	                                                                Upp::ToString(type));
	[pasteboard setData:[NSData dataWithBytes:~raw length:raw.GetCount()] forType:type];
}

- (void)pasteboardChangedOwner:(NSPasteboard *)sender
{
	LLOG("pasteboardCahandedOwner");
//	data.Clear();  // TODO: This seems to trigger on declareTypes despite owner being the same...
}

@end


namespace Upp {
	
CocoClipboardOwner *ClipboardOwner()
{
	GuiLock __;
	static CocoClipboardOwner *owner = [[CocoClipboardOwner alloc] init];
	return owner;
}

void ClearClipboard()
{
	GuiLock __;
	[[NSPasteboard generalPasteboard] clearContents];
	ClipboardOwner()->data.Clear();
}

void AppendClipboard(const char *format, const Value& value, String (*render)(const Value& data))
{
	GuiLock __;

	auto& data = ClipboardOwner()->data;

	for(String fmt : Split(format, ';'))
		data.GetAdd(fmt) = ClipData(value, render);

	NSAutoreleasePool *pool;
	pool = [[NSAutoreleasePool alloc] init];

	NSMutableSet *types = [[NSMutableSet alloc] init];
	for(auto id : data.GetKeys())
		[types addObject:PasteboardType(id)];

	[[NSPasteboard generalPasteboard] declareTypes:[types allObjects] owner:ClipboardOwner()];

	[types release];
	[pool release];
}

void AppendClipboard(const char *format, const String& data)
{
	GuiLock __;
	AppendClipboard(format, Value(data), NULL);
}

void AppendClipboard(const char *format, const byte *data, int length)
{
	GuiLock __;
	AppendClipboard(format, String(data, length));
}

bool PasteClip::IsAvailable(const char *fmt) const
{
	return IsClipboardAvailable(fmt);
}

String PasteClip::Get(const char *fmt) const
{
	return ReadClipboard(fmt);
}

void PasteClip::GuiPlatformConstruct()
{
}

bool IsClipboardAvailable(const char *id)
{
	return [[NSPasteboard generalPasteboard]
	        availableTypeFromArray:[NSArray arrayWithObjects:PasteboardType(id), nil]];
}

String ReadClipboard(const char *fmt_)
{
	String fmt = fmt_;
	if(fmt == "text")
		return ToString([[NSPasteboard generalPasteboard] stringForType:NSPasteboardTypeString]);
	NSData *data = [[NSPasteboard generalPasteboard] dataForType:PasteboardType(fmt)];
	return String((const char *)[data bytes], [data length]);
}

bool IsClipboardAvailableText()
{
	return IsClipboardAvailable("text");
}

String ReadClipboardText()
{
	if(IsClipboardAvailableText())
		return ReadClipboard("text");
	return Null;
}

WString ReadClipboardUnicodeText()
{
	return ReadClipboardText().ToWString();
}

const char *ClipFmtsText()
{
	return "text";
}

String GetString(PasteClip& clip)
{
	GuiLock __;
	return clip.Get("text");
}

WString GetWString(PasteClip& clip)
{
	GuiLock __;
	return GetString(clip).ToWString();
}

bool AcceptText(PasteClip& clip)
{
	return clip.Accept(ClipFmtsText());
}

void Append(VectorMap<String, ClipData>& data, const String& text)
{
	data.GetAdd("text", ClipData(text, NULL));
}

void Append(VectorMap<String, ClipData>& data, const WString& text)
{
	data.GetAdd("text", ClipData(text.ToString(), NULL));
}

String GetTextClip(const WString& text, const String& fmt)
{
	return text.ToString();
}

String GetTextClip(const String& text, const String& fmt)
{
	return text;
}

const char *ClipFmtsImage()
{
	return "image";
}

bool AcceptImage(PasteClip& clip)
{
	GuiLock __;
	return clip.Accept(ClipFmtsImage());
}

Image GetImage(PasteClip& clip)
{
	GuiLock __;
	return PNGRaster().LoadString(clip.Get("image"));
}

Image ReadClipboardImage()
{
	GuiLock __;
	PasteClip d = Ctrl::Clipboard();
	return GetImage(d);
}

String sImage(const Value& image)
{
	if(IsNull(image))
		return Null;
	Image img = image;
	return PNGEncoder().SaveString(img);
}

String GetImageClip(const Image& img, const String& fmt)
{
	GuiLock __;
	return sImage(img);
}

void AppendClipboardImage(const Image& img)
{
	GuiLock __;
	if(img.IsEmpty()) return;
	AppendClipboard("image", img, sImage);
}

bool AcceptFiles(PasteClip& clip)
{
	if(clip.Accept("files;url")) {
		clip.SetAction(DND_COPY);
		return true;
	}
	return false;
}

bool IsAvailableFiles(PasteClip& clip)
{
	return clip.IsAvailable("files;url");
}

Vector<String> GetFiles(PasteClip& clip)
{
	GuiLock __;
	Vector<String> f;
	String raw;
	bool files = clip.IsAvailable("files");
	if(files)
		raw = clip.Get("files");
	else
	if(clip.IsAvailable("url"))
		raw = clip.Get("url");
	XmlNode n = ParseXML(raw);
	for(const auto& e : n["plist"]["array"])
		if(e.IsTag("string")) {
			String fn = e.GatherText();
			if(files ? fn.GetCount() : fn.TrimStart("file://"))
				f.Add(fn);
		}
	return f;
}

};

#endif
