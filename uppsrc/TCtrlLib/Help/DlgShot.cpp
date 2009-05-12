#include <TCtrlLib/Help/TCtrlLibHelp.h>
#pragma hdrstop

#include <TCtrlLib/OldTreeCtrl.h>

NAMESPACE_UPP

#define LAYOUTFILE <TCtrlLib/Help/DlgShot.lay>
#include <CtrlCore/lay.h>

// Smart Text Formatting
// '\n' - left aligned line
// '\t' - left align, dont leave line
// '\r' - right aligned line
// '\2' - right align, dont leave line
// '\v' - centered line
// '\1' - center align, dont leave line
// '\a' - underline switch
// '\b' - bold switch
// '\f' - italic switch

String StripSmartText(const char *smart)
{
	String out;
	for(; *smart; smart++)
		if(*smart != '\n' && *smart != '\t' && *smart != '\r' && *smart != '\2'
		&& *smart != '\v' && *smart != '\1' && *smart != '\a' && *smart != '\b' && *smart != '\f')
			out.Cat(*smart);
	return out;
}

class ImageBrowseCtrl : public DragDropCtrl
{
public:
	typedef ImageBrowseCtrl CLASSNAME;
	ImageBrowseCtrl();

	void         Set(Image dwg);

	virtual void Layout();
	virtual void Paint(Draw& draw);

	virtual void DropRect(const Rect& rc, dword keyflags);
	virtual void Click(Point pt);

	bool         IsSelection() const          { return !IsNull(selection); }
	Rect         GetSelection() const         { return selection; }
	void         ClearSelection()             { selection = Null; Refresh(); }
	void         SetSelection(const Rect& rc) { selection = rc; Refresh(); }

	TOOL_LOCAL(View)
		TOOL_LOCAL(ViewZoomIn)
		TOOL_LOCAL(ViewZoomOut)
		TOOL_LOCAL(View1to1)

private:
	void         OnScroll() { Refresh(); }
	Size         GetZoomSize() const;

private:
	int          zoom;
	Image        image;
	ScrollBars   scrollbars;
	Rect         selection;

	enum {
		GAP = 10,
	};
};

ImageBrowseCtrl::ImageBrowseCtrl()
{
	BackPaint();
	SetFrame(InsetFrame());
	AddFrame(scrollbars);
	scrollbars.WhenScroll = THISBACK(OnScroll);
	scrollbars.AutoHide();
	zoom = 1024;
}

void ImageBrowseCtrl::Set(Image im)
{
	image = im;
	Layout();
}

Size ImageBrowseCtrl::GetZoomSize() const
{
	return (image.GetSize() * zoom) >> 10;
}

void ImageBrowseCtrl::Layout()
{
	scrollbars.Set(scrollbars, scrollbars.GetReducedViewSize(), GetZoomSize() + 2 * GAP);
	Refresh();
}

void ImageBrowseCtrl::Paint(Draw& draw)
{
	draw.DrawRect(GetSize(), Color(224, 255, 255));
	Size org_size = image.GetSize();
	Size out_size = GetZoomSize();
	if(org_size.cx <= 0 || org_size.cy <= 0
	|| out_size.cx <= 0 || out_size.cy <= 0)
		return;
	Rect out_rc = Rect(out_size).Offseted(-scrollbars.Get() + GAP);
	if(!IsNull(image))
		draw.DrawImage(out_rc, image);
	if(!IsNull(selection)) {
		Rect area = Transform(selection, Rect(org_size), out_rc);

	}
}

void ImageBrowseCtrl::DropRect(const Rect& rc, dword keyflags)
{

}

void ImageBrowseCtrl::Click(Point pt)
{
}

void ImageBrowseCtrl::ToolView(Bar& bar, bool local)
{
}

void ImageBrowseCtrl::ToolViewZoomIn(Bar& bar, bool local)
{
}

void ImageBrowseCtrl::OnViewZoomIn()
{
}

void ImageBrowseCtrl::ToolViewZoomOut(Bar& bar, bool local)
{
}

void ImageBrowseCtrl::OnViewZoomOut()
{
}

void ImageBrowseCtrl::ToolView1to1(Bar& bar, bool local)
{
}

void ImageBrowseCtrl::OnView1to1()
{
}

class DlgCtrlAttr : public WithDlgShotCtrlAttrLayout<TopWindow>
{
public:
	typedef DlgCtrlAttr CLASSNAME;
	DlgCtrlAttr(Ctrl& ctrl, Ctrl *focus);

	void Run();
};

DlgCtrlAttr::DlgCtrlAttr(Ctrl& ctrl, Ctrl *focus)
{
	CtrlLayout(*this, "Control properties");
	object_type <<= typeid(ctrl).name();
	Rect pos = ctrl.GetRect();
	Rect scr = ctrl.GetScreenRect();
	Rect top = scr.Offseted(-ctrl.GetTopCtrl() -> GetScreenView().TopLeft());
	Rect view = ctrl.GetView();
	Rect frm(view.left, view.top, pos.Width() - view.right, pos.Height() - view.bottom);
	pos_cx <<= pos.Width();
	pos_cy <<= pos.Height();
	pos_l <<= pos.left; pos_t <<= pos.top; pos_r <<= pos.right; pos_b <<= pos.bottom;
	scr_l <<= scr.left; scr_t <<= scr.top; scr_r <<= scr.right; scr_b <<= scr.bottom;
	top_l <<= top.left; top_t <<= top.top; top_r <<= top.right; top_b <<= top.bottom;
	frm_l <<= frm.left; frm_t <<= frm.top; frm_r <<= frm.right; frm_b <<= frm.bottom;
	LogPos logpos = ctrl.GetPos();
	switch(logpos.x.GetAlign()) {
	case Ctrl::CENTER:
		logpos_x <<= NFormat("HCenterPos(%d, %d)", logpos.x.GetA(), logpos.x.GetB());
		break;
	case Ctrl::LEFT:
		logpos_x <<= NFormat("LeftPos(%d, %d)", logpos.x.GetA(), logpos.x.GetB());
		break;
	case Ctrl::RIGHT:
		logpos_x <<= NFormat("RightPos(%d, %d)", logpos.x.GetA(), logpos.x.GetB());
		break;
	case Ctrl::SIZE:
		logpos_x <<= NFormat("HSizePos(%d, %d)", logpos.x.GetA(), logpos.x.GetB());
		break;
	default:
		logpos_x <<= NFormat("Logc(%d, %d, %d)", logpos.x.GetAlign(), logpos.x.GetA(), logpos.x.GetB());
		break;
	}
	switch(logpos.y.GetAlign()) {
	case Ctrl::CENTER:
		logpos_y <<= NFormat("VCenterPos(%d, %d)", logpos.y.GetA(), logpos.y.GetB());
		break;
	case Ctrl::TOP:
		logpos_y <<= NFormat("TopPos(%d, %d)", logpos.y.GetA(), logpos.y.GetB());
		break;
	case Ctrl::BOTTOM:
		logpos_y <<= NFormat("BottomPos(%d, %d)", logpos.y.GetA(), logpos.y.GetB());
		break;
	case Ctrl::SIZE:
		logpos_y <<= NFormat("VSizePos(%d, %d)", logpos.y.GetA(), logpos.y.GetB());
		break;
	default:
		logpos_y <<= NFormat("Logc(%d, %d, %d)", logpos.y.GetAlign(), logpos.y.GetA(), logpos.y.GetB());
		break;
	}
	view_cx <<= view.Width();
	view_cy <<= view.Height();
	is_enabled = ctrl.IsEnabled();
	is_visible = ctrl.IsVisible();
	is_shown = ctrl.IsShown();
	has_focus = (focus == &ctrl);
	while(focus && focus != &ctrl)
		focus = focus -> GetParent();
	has_focus_deep = (focus == &ctrl);
	is_transparent = ctrl.IsTransparent();
	switch(ctrl.GetBackPaint()) {
	case Ctrl::NOBACKPAINT:          get_backpaint.SetLabel("NOBACKPAINT"); break;
	case Ctrl::TRANSPARENTBACKPAINT: get_backpaint.SetLabel("TRANSPARENTBACKPAINT"); break;
	case Ctrl::FULLBACKPAINT:        get_backpaint.SetLabel("FULLBACKPAINT"); break;
	default:                         get_backpaint.SetLabel(NFormat("BackPaint(%d)", (int)ctrl.GetBackPaint())); break;
	}
	is_in_frame = ctrl.InFrame();
	is_editable = ctrl.IsEditable();
	is_modified = ctrl.IsModified();
	is_modify_set = ctrl.IsModifySet();
	object_label = "label"; //AsCString(ctrl.GetLabelText()).ToWString();
	object_value = StdFormat(ctrl.GetData()).ToWString();

	for(Ctrl *c = GetFirstChild(); c; c = c -> GetNext())
		c -> SetReadOnly();
}

void DlgCtrlAttr::Run()
{
	TopWindow::Run();
}

class DlgShot : public TopWindow
{
public:
	typedef DlgShot CLASSNAME;
	DlgShot(Ctrl *ctrl, const Image& still);

	void        Run();

	Value       GetData() const;

private:
	Image       GetTreeImage() const;
	void        OnTree();
	void        SyncTree();
	void        SyncTreeDeep(OldTreeItem& dest, Ctrl *src, bool visible);

	void        Rescan() { menubar.Set(THISBACK(ToolMain)); toolbar.Set(THISBACK(ToolMain)); }

	TOOL(Main)
	TOOL(Edit)
		TOOL(EditCopy)
		TOOL(EditCtrlAttr)

	TOOL(Local)

private:
	Ctrl       *ctrl;
	Ctrl       *top_ctrl;
	Image       still;
	Rect        top_frame;
	Ptr<Ctrl>   focus_ctrl;

	MenuBar     menubar;
	ToolBar     toolbar;

	Splitter    splitter;
	OldTreeCtrl    tree;
	ImageBrowseCtrl view;
};

//One<Ctrl> GetDlgShot(Ctrl *master, const Image& still) { return new DlgShot(master, still); }
void      RunDlgShot(Ctrl *master, const Image& still) { DlgShot(master, still).Run(); }

DlgShot::DlgShot(Ctrl *ctrl, const Image& still)
: ctrl(ctrl), still(still)
{
	top_frame = Rect(0, 0, 0, 0);
#ifdef PLATFORM_WIN32
	HWND hwnd = 0;
	for(top_ctrl = ctrl; top_ctrl && !(hwnd = top_ctrl -> GetHWND()); top_ctrl = top_ctrl -> GetParent())
		;
	if(hwnd) {
		Size sz = top_ctrl -> GetSize();
		top_frame = Rect(sz);
		::AdjustWindowRectEx(top_frame, GetWindowLong(hwnd, GWL_STYLE),
			FALSE, GetWindowLong(hwnd, GWL_EXSTYLE));
		top_frame = Rect(-top_frame.left, -top_frame.top, top_frame.right - sz.cx, top_frame.bottom - sz.cy);
	}
#endif
	focus_ctrl = GetFocusCtrl();
	Title("Snímek dialogu / okna");
	Sizeable().Zoomable();
	AddFrame(menubar);
	AddFrame(toolbar);
	Add(splitter.SizePos());
	splitter.Horz(tree, view);
	splitter.SetPos(3000);
	tree <<= THISBACK(OnTree);
	tree.WhenLeftDouble = THISBACK(OnEditCopy);
	tree.WhenBar = THISBACK(ToolLocal);
	view.SetFrame(InsetFrame());
	SyncTree();
	Rescan();
}

void DlgShot::Run()
{
	TopWindow::Run();
}

void DlgShot::SyncTree()
{
	tree.Clear();
	if(ctrl)
		SyncTreeDeep(tree, ctrl, true);
	tree.OldTreeItem::Open();
	tree.SetCursor(&tree);
	OnTree();
}

void DlgShot::SyncTreeDeep(OldTreeItem& dest, Ctrl *src, bool visible)
{
	if(!src -> IsVisible())
		visible = false;
	String qtf = "\xFF";
	if(!visible)
		qtf << "[@1";
	qtf << " \1" << GetTypeName(typeid(*src)) << "\1";
	String lbl = "lbl"; // StripSmartText(src -> GetLabelText());
	if(!IsNull(lbl))
		qtf << ": [* \1" << lbl;
	dest.Text(qtf);
	dest <<= PtrToValue(src);
	dest.SetImage(src == focus_ctrl ? CtrlImg::Home() : visible
		? src -> GetFirstChild() ? CtrlImg::Dir() : CtrlImg::File() : CtrlImg::remove());

	for(Ctrl *child = src -> GetFirstChild(); child; child = child -> GetNext())
		SyncTreeDeep(dest.Add(), child, visible);
}

Value DlgShot::GetData() const
{
	return GetTreeImage();
}

static Image ImageCrop(Image src, Rect rc)
{
	rc &= Rect(src.GetSize());
	if(rc.IsEmpty())
		return Null;
	if(rc.Size() == src.GetSize())
		return src;
	Size sz = rc.Size();
	ImageBuffer buffer(sz);
	for(int y = 0; y < sz.cy; y++)
		memcpy(buffer[y], &src[y + rc.top][rc.left], sz.cx * sizeof(RGBA));
	return Image(buffer);
}

Image DlgShot::GetTreeImage() const
{
	Image img;
	if(!IsNull(~tree)) {
		Ctrl *sel_ctrl = ValueTo<Ctrl *>(~tree);
		Rect rc;
		if(sel_ctrl == ctrl)
			rc = Rect(still.GetSize());
		else
			rc = sel_ctrl -> GetScreenRect() - ctrl -> GetScreenRect().TopLeft() + top_frame.TopLeft();
		img = ImageCrop(still, rc);
//		DrawingDraw ddraw(rc.Size());
//		ddraw.DrawImage(0, 0, ImageCrop(still, rc));
//		img = ddraw;
	}
	return img;
}

void DlgShot::OnTree()
{
	view.Set(GetTreeImage());
}

void DlgShot::ToolMain(Bar& bar)
{
	bar.Add("Edit", THISBACK(ToolEdit));
}

void DlgShot::ToolEdit(Bar& bar)
{
	ToolEditCopy(bar);
	ToolEditCtrlAttr(bar);
}

void DlgShot::ToolEditCopy(Bar& bar)
{
	bar.Add("Kopírovat", CtrlImg::copy(), THISBACK(OnEditCopy))
		.Key(K_CTRL_C)
		.Help("Kopírovat vybraný obrázek do schránky");
}

void DlgShot::OnEditCopy()
{
	if(!IsNull(~tree)) {
		Image img = GetTreeImage();
		Size dot_size = iscale(img.GetSize(), 1024, 170);
/*
		RichObject object = CreateImageObject(img, dot_size, iscale(dot_size, 80, 100));
		RichPara para;
		para.Cat(object, RichPara::CharFormat());
		RichText text;
		text.Cat(para);
		WriteClipboard(GetClipboardFormatCode("QTF"), BodyAsQTF(text));
*/
		WriteClipboardImage(img); //TODO, White(), true);
		BeepInformation();
	}
}

void DlgShot::ToolLocal(Bar& bar)
{
	tree.StdBar(bar);
	bar.Separator();
	ToolEdit(bar);
}

void DlgShot::ToolEditCtrlAttr(Bar& bar)
{
	bar.Add("Properties", THISBACK(OnEditCtrlAttr));
}

void DlgShot::OnEditCtrlAttr()
{
	if(!IsNull(~tree)) {
		Ctrl *sel_ctrl = ValueTo<Ctrl *>(~tree);
		DlgCtrlAttr(*sel_ctrl, focus_ctrl).Run();
	}
}

END_UPP_NAMESPACE
