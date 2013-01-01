#include <CtrlCore/CtrlCore.h>
#include <plugin/bmp/bmp.h>

#ifdef GUI_GTK

NAMESPACE_UPP

#define LLOG(x)  // DLOG(x)

void Ctrl::GtkSelectionDataSet(GtkSelectionData *selection_data, const String& fmt, const String& data)
{
	if(fmt == "text") {
		String s = data;
		gtk_selection_data_set_text(selection_data, (const gchar*)~s, s.GetCount());
	}
	else
	if(fmt == "image") {
		Image img;
		if(IsString(data))
			LoadFromString(img, data);
		if(!IsNull(img)) {
			ImageGdk m(img);
			gtk_selection_data_set_pixbuf(selection_data, m);
		}
	}
	else {
		String s = data;
		gtk_selection_data_set(selection_data, GAtom(fmt), 8, (const guchar*)~s, s.GetCount());
	}
}

void Ctrl::GtkGetClipData(GtkClipboard *clipboard, GtkSelectionData *selection_data,
                          guint info, gpointer user_data)
{
	ArrayMap<String, ClipData>& target = *(ArrayMap<String, ClipData> *)user_data;
	LLOG("GtkGetClipData for " << target.GetKey(info));
	GtkSelectionDataSet(selection_data, target.GetKey(info), target[info].Render());	
}

void ClearClipData(GtkClipboard *clipboard, gpointer) {}

Ctrl::Gclipboard::Gclipboard(GdkAtom type)
{
	clipboard = gtk_clipboard_get(type);
}

void Ctrl::AddFmt(GtkTargetList *list, const String& fmt, int info)
{
	if(fmt == "text")
		gtk_target_list_add_text_targets(list, info);
	else
	if(fmt == "image")
		gtk_target_list_add_image_targets(list, info, TRUE);
	else
		gtk_target_list_add(list, GAtom(fmt), 0, info);
}

GtkTargetList *Ctrl::CreateTargetList(const VectorMap<String, ClipData>& target)
{
	GtkTargetList *list = gtk_target_list_new(NULL, 0);
	for(int i = 0; i < target.GetCount(); i++)
		AddFmt(list, target.GetKey(i), i);
	return list;
}

void Ctrl::Gclipboard::Put(const String& fmt, const ClipData& data)
{
	GuiLock __; 
	LLOG("Gclipboard::Put " << fmt);

	target.GetAdd(fmt) = data;

	GtkTargetList *list = CreateTargetList(target);
	
	gint n;
	GtkTargetEntry *targets = gtk_target_table_new_from_list(list, &n);
	
	gtk_clipboard_set_with_data(clipboard, targets, n, GtkGetClipData, ClearClipData, &target);
	gtk_clipboard_set_can_store(clipboard, NULL, 0);
	
	gtk_target_table_free(targets, n);
	gtk_target_list_unref(list);
}

String Ctrl::GtkDataGet(GtkSelectionData *s)
{
	if(!s)
		return Null;
	const guchar *b = gtk_selection_data_get_data(s);
	int n = gtk_selection_data_get_length(s);
	return n >= 0 && b ? String(b, n) : String();
}

String Ctrl::Gclipboard::Get(const String& fmt)
{
	if(fmt == "text") {
		gchar *s = gtk_clipboard_wait_for_text(clipboard);
		if(s) {
			String h = s;
			g_free(s);
			return h;
		}
		return Null;
	}
	else
	if(fmt == "image") {
		Image img = ImageFromPixbufUnref(gtk_clipboard_wait_for_image(clipboard));
		return StoreAsString(img); // Not very optimal...
	}
	else
		return GtkDataGet(gtk_clipboard_wait_for_contents(clipboard, GAtom(fmt)));
}

bool Ctrl::Gclipboard::IsAvailable(const String& fmt)
{
	if(fmt == "text")
		return gtk_clipboard_wait_is_text_available(clipboard);
	if(fmt == "image")
		return gtk_clipboard_wait_is_image_available(clipboard);
	return gtk_clipboard_wait_is_target_available(clipboard, GAtom(fmt));
}

bool PasteClip::IsAvailable(const char *fmt) const
{
	LLOG("PasteClip::IsAvailable " << fmt << ", type: " << type);
	if(type == 1)
		return Ctrl::IsDragAvailable(fmt);
	return (type == 0 ? Ctrl::gclipboard() : Ctrl::gselection()).IsAvailable(fmt);
}

String PasteClip::Get(const char *fmt) const
{
	LLOG("PasteClip::Get " << fmt << ", type: " << type);
	if(type == 1)
		return Ctrl::DragGet(fmt);
	return (type == 0 ? Ctrl::gclipboard() : Ctrl::gselection()).Get(fmt);
}

void PasteClip::GuiPlatformConstruct()
{
	type = 0;
}

void Ctrl::Gclipboard::Clear()
{
	gtk_clipboard_clear(clipboard);
	target.Clear();
}

void ClearClipboard()
{
	Ctrl::gclipboard().Clear();
}

Ctrl::Gclipboard& Ctrl::gclipboard()
{
	GuiLock __; 
	static Gclipboard c(GDK_SELECTION_CLIPBOARD);
	return c;
}

Ctrl::Gclipboard& Ctrl::gselection()
{
	GuiLock __; 
	static Gclipboard c(GDK_SELECTION_PRIMARY);
	return c;
}

void AppendClipboard(const char *format, const Value& data, String (*render)(const Value& data))
{
	GuiLock __; 
	LLOG("AppendClipboard " << format);
	Vector<String> s = Split(format, ';');
	for(int i = 0; i < s.GetCount(); i++)
		Ctrl::gclipboard().Put(s[i], ClipData(data, render));
}

void AppendClipboard(const char *format, const byte *data, int length)
{
	GuiLock __;
	AppendClipboard(format, String(data, length));
}

void AppendClipboard(const char *format, const String& data)
{
	GuiLock __;
	LLOG("AppendClipboard " << format);
	Vector<String> s = Split(format, ';');
	for(int i = 0; i < s.GetCount(); i++)
		Ctrl::gclipboard().Put(s[i], ClipData(data));
}

String ReadClipboard(const char *format)
{
	GuiLock __;
	return Null;
}

const char *ClipFmtsText()
{
	GuiLock __;
	return "text";
}

void AppendClipboardUnicodeText(const WString& s)
{
	AppendClipboard(ClipFmtsText(), Value(ToUtf8(s)), NULL);
}

void AppendClipboardText(const String& s)
{
	AppendClipboard(ClipFmtsText(), Value(ToCharset(CHARSET_UTF8, s)), NULL);
}

String GetString(PasteClip& clip)
{
	GuiLock __;
	if(clip.IsAvailable("text"))
		return ToCharset(CHARSET_DEFAULT, clip.Get("text"), CHARSET_UTF8);
	return Null;
}

WString GetWString(PasteClip& clip)
{
	GuiLock __;
	if(clip.IsAvailable("text"))
		return FromUtf8(clip.Get("text"));
	return Null;
}


bool AcceptText(PasteClip& clip)
{
	return clip.Accept(ClipFmtsText());
}

static String sText(const Value& data)
{
	return data;
}

void Append(VectorMap<String, ClipData>& data, const String& text)
{
	data.GetAdd("text", ClipData(ToCharset(CHARSET_UTF8, text), sText));
}

void Append(VectorMap<String, ClipData>& data, const WString& text)
{
	data.GetAdd("text", ClipData(ToUtf8(text), sText));
}

String GetTextClip(const WString& text, const String& fmt)
{
	if(fmt == "text")
		return ToUtf8(text);
	return Null;
}

String GetTextClip(const String& text, const String& fmt)
{
	if(fmt == "text")
		return ToCharset(CHARSET_UTF8, text);
	return Null;
}

String ReadClipboardText()
{
	return ToCharset(CHARSET_DEFAULT, Ctrl::gclipboard().Get("text"), CHARSET_UTF8);
}

WString ReadClipboardUnicodeText()
{
	return FromUtf8(Ctrl::gclipboard().Get("text"));
}

bool IsClipboardAvailable(const char *id)
{
	return Ctrl::gclipboard().IsAvailable(id);
}

bool IsClipboardAvailableText()
{
	return Ctrl::gclipboard().IsAvailable("text");
}

const char *ClipFmtsImage()
{
	return "image";
}

bool AcceptImage(PasteClip& clip)
{
	GuiLock __;
	return clip.Accept("image");
}

Image GetImage(PasteClip& clip)
{
	GuiLock __;
	Image m;
	LoadFromString(m, clip.Get("image"));
	return m;
}

Image ReadClipboardImage()
{
	GuiLock __;
	Image m;
	LoadFromString(m, Ctrl::gclipboard().Get("image"));
	return m;
}

String GetImageClip(const Image& m, const String& fmt)
{
	Image h = m;
	if(fmt == "image")
		return StoreAsString(h);
	return Null;
}

void AppendClipboardImage(const Image& img)
{
	GuiLock __;
	if(img.IsEmpty()) return;
	Image h = img;
	AppendClipboard("image", StoreAsString(h));
}

bool AcceptFiles(PasteClip& clip)
{
	if(clip.Accept("files")) {
		clip.SetAction(DND_COPY);
		return true;
	}
	return false;
}

bool IsAvailableFiles(PasteClip& clip)
{
	return clip.IsAvailable("files");
}

// TODO:
Vector<String> GetFiles(PasteClip& clip)
{
	GuiLock __;
	Vector<String> f;
	return f;
}

Ptr<Ctrl> Ctrl::sel_ctrl;

void Ctrl::GuiPlatformSelection(PasteClip& d)
{
	d.fmt.Clear();
	d.type = 2;
}

String Ctrl::RenderPrimarySelection(const Value& fmt)
{
	return sel_ctrl->GetSelectionData(fmt);
}

void Ctrl::SetSelectionSource(const char *fmts)
{
	GuiLock __; 
	LLOG("SetSelectionSource " << UPP::Name(this) << ": " << fmts);
	Vector<String> s = Split(fmts, ';');
	sel_ctrl = this;
	for(int i = 0; i < s.GetCount(); i++)
		gselection().Put(s[i], ClipData(s[i], RenderPrimarySelection));
}

END_UPP_NAMESPACE

#endif
