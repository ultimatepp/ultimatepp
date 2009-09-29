#include <Ole/Ctrl/OleCtrl.h>

#ifdef PLATFORM_WIN32

NAMESPACE_UPP

LPOLESTR AllocString(String s)
{
	LPOLESTR p = reinterpret_cast<LPOLESTR>(CoTaskMemAlloc(sizeof(OLECHAR) * (s.GetLength() + 1))), d = p;
	const char *f = s;
	while(*d++ = *f++)
		;
	return p;
}

OcxControl::OcxControl()
{
	ActiveX();
	view_sink_aspect = DVASPECT_CONTENT;
	view_sink_advf = 0;
	status = CLOSED;
	stream_inited = false;
	timer_shutdown = false;
	extent_size = GetStdSize();
}

OcxControl::~OcxControl()
{
	LOGMETHOD("OcxControl::~OcxControl");
}

bool OcxControl::IsOcxChild()
{
	return true;
}

LRESULT OcxControl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LOGMETHOD("OcxControl::WindowProc");
//	RLOG("message = " << FormatIntHex(message) << ", wParam = " << FormatIntHex(wParam) << ", lParam = " << FormatIntHex(lParam));
	if(message == WM_GETDLGCODE)
		return DLGC_WANTALLKEYS | DLGC_WANTARROWS | DLGC_WANTMESSAGE;
	LRESULT res = Ctrl::WindowProc(message, wParam, lParam);
	return res;
}

bool OcxControl::Open(HWND parent) // parent = 0 - create forged control
{
	LOGMETHOD("OcxControl::Open");
//		if(!IsForged())
	ctrl_rect = GetRect();
	if(IsOpen())
		Ctrl::Close();
	if(parent && IsChild())
		Remove();
	status = CLOSED;

//		noproc = true;
//		visible |= !parent;
	Rect rc = ctrl_rect;
	if(IsNull(rc))
		rc = Rect(0, 0, 0, 0);
	if(!parent)
	{ // move window away from screen in forged mode
		rc.Offset(-30000 - rc.left, -30000 - rc.top);
		SetRect(rc);
	}
//		view.SetRectEmpty();

	Create(parent, (IsEnabled() ? 0 : WS_DISABLED) | (parent ? WS_CHILD : WS_POPUP) | WS_VISIBLE,
		(parent ? 0 : WS_EX_TOOLWINDOW), false, SW_HIDE, false);

	Show();

/*		wnd = &stdwnd;
	fullrefresh = false;
	dword style = parent ? WS_CHILD : WS_POPUP, exstyle = WS_EX_TOOLWINDOW;
	if(!enabled) style |= WS_DISABLED;
	noproc = true;
	visible |= !parent;
	Rect rc = GetRect();
	if(!parent)
	{ // move window away from screen in forged mode
		rc.OffsetRect(-30000 - rc.left, -30000 - rc.top);
		SetRect(rc);
	}
	view.SetRectEmpty();
	wnd->CreateEx(exstyle, CtrlWindowClass(),
#ifdef _DEBUG
		Name(),
#else
		NULL,
#endif
		style,
		rc.left, rc.top, rc.Width(), rc.Height(),
		parent ? parent : 0, 0, 0);

	if(!wnd->m_hWnd)
		return false;

	type = POPUP;
	noproc = false;

	for(Ctrl *q = GetFirstChild(); q; q = q->GetNext())
		reinterpret_cast<CtrlHook *>(q)->_CreateChild();

	if(visible)
		ShowWindow(GetHWND(), SW_SHOWNA);
//		SetMnemonic((byte) hotchar);

	RefreshLayout();
	RefreshPos();
	Refresh();
	CancelMode();
	CreateInit();
	move = true;
*/
	status = ACTIVE; //(parent ? ACTIVE : FORGED);
	return true;
}

void OcxControl::Serialize(Stream& stream)
{
	int version = 1;
	SerializePos(stream);
}

void OcxControl::SerializePos(Stream& stream)
{
	int version = 1;
	stream / version;
	Rect rc;
	if(stream.IsStoring())
		rc = GetRect();
	stream % rc;
	if(stream.IsLoading())
		SetRect(rc);
}

const WCHAR OcxControl::data_stream_name[] = L"data";

HRESULT OcxControl::SetClientSite(IOleClientSite *site)
{
	LOGMETHOD("IOleObject::SetClientSite()");
	in_place_site = site;
	client_site = site;
	if(GetRect().IsEmpty()) {
		LOG("Setting up default control rect...");
		SetRect(GetStdSize());
	}
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::GetClientSite(IOleClientSite **res)
{
	LOGMETHOD("IOleObject::GetClientSite");
	if(*res = ~client_site)
		client_site->AddRef();
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::SetHostNames(LPCOLESTR app_name, LPCOLESTR doc_name)
{
	LOGMETHOD("IOleObject::SetHostNames");
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::Close(dword save_option)
{
	LOGMETHOD("IOleObject::Close");
	InPlaceDeactivate();
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::SetMoniker(dword which, IMoniker *moniker)
{
	LOGMETHOD("IOleObject::SetMoniker");
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::GetMoniker(dword assign, dword which, IMoniker **moniker)
{
	LOGMETHOD("IOleObject::GetMoniker");
	*moniker = 0;
	return LOGRESULT(E_NOTIMPL);
}

HRESULT OcxControl::InitFromData(IDataObject *data, BOOL creation, dword)
{
	LOGMETHOD("IOleObject::InitFromData");
	return LOGRESULT(S_FALSE);
}

HRESULT OcxControl::GetClipboardData(dword, IDataObject **data)
{
	LOGMETHOD("IOleObject::GetClipboardData");
	*data = 0;
	return LOGRESULT(E_NOTIMPL);
}

HRESULT OcxControl::DoVerb(long verb, MSG *msg, IOleClientSite *active_site,
	long index, HWND parent, const RECT *rc)
{
	LOGMETHOD("IOleObject::DoVerb");
	switch(verb)
	{
	case OLEIVERB_PRIMARY:          return DoVerbPrimary();
	case OLEIVERB_SHOW:             return DoVerbShow();
	case OLEIVERB_INPLACEACTIVATE:  return DoVerbInPlaceActivate();
	case OLEIVERB_UIACTIVATE:       return DoVerbUIActivate();
	case OLEIVERB_HIDE:             return DoVerbHide();
	case OLEIVERB_OPEN:             return DoVerbOpen();
	case OLEIVERB_DISCARDUNDOSTATE: return DoVerbDiscardUndo();
	case OLEIVERB_PROPERTIES:       return DoVerbProperties();
	default:                        return OLEOBJ_S_INVALIDVERB;
	}
}

HRESULT OcxControl::EnumVerbs(IEnumOLEVERB **verbs)
{
	LOGMETHOD("IOleObject::EnumVerbs");
	*verbs = 0;
	return LOGRESULT(OLEOBJ_E_NOVERBS);
}

HRESULT OcxControl::Update()
{
	LOGMETHOD("IOleObject::Update");
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::IsUpToDate()
{
	LOGMETHOD("IOleObject::IsUpToDate");
	return LOGRESULT(MK_E_UNAVAILABLE);
}

HRESULT OcxControl::GetUserClassID(GUID *guid)
{
	LOGMETHOD("IOleObject::GetUserClassID");
	ASSERT(ocx_info);
	*guid = ocx_info->GetCoClassGUID();
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::GetUserType(dword form, LPOLESTR *type)
{
//	__asm int 3
	LOGMETHOD("IOleObject::GetUserType");
	ASSERT(ocx_info);
	*type = AllocString(ocx_info->GetName());
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::SetExtent(dword aspect, SIZEL *hi_size)
{
	LOGMETHOD("IOleObject::SetExtent(" << hi_size->cx << ", " << hi_size->cy << ")");
	if(aspect == DVASPECT_CONTENT) {
		extent_size = min(FromHiMetric(*hi_size), GetMaxSize());
		OCXLOG("Ctrl size = " << extent_size);
		SetRect(Rect(GetRect().TopLeft(), extent_size));
	}
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::GetExtent(dword aspect, SIZEL *size)
{
	LOGMETHOD("IOleObject::GetExtent");
	if(aspect == DVASPECT_CONTENT) {
		*size = ToHiMetric(extent_size);
		return LOGRESULT(S_OK);
	}
	return LOGRESULT(E_NOTIMPL);
}

HRESULT OcxControl::Advise(IAdviseSink *sink, dword *connection)
{
	LOGMETHOD("IOleObject::Advise");
	if(!sink || !connection)
		return E_INVALIDARG;
	*connection = advise_sinks.Add(sink);
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::Unadvise(dword connection)
{
	LOGMETHOD("IOleObject::Unadvise");
	return LOGRESULT(advise_sinks.Remove(connection) ? S_OK : E_INVALIDARG);
}

HRESULT OcxControl::EnumAdvise(IEnumSTATDATA **sink_enum)
{
	LOGMETHOD("IOleObject::EnumAdvise");
	*sink_enum = 0;
	return LOGRESULT(E_NOTIMPL);
}

HRESULT OcxControl::GetMiscStatus(dword aspect, dword *st)
{
	LOGMETHOD("IOleObject::GetMiscStatus");
	*st = OLEMISC_RECOMPOSEONRESIZE
		| OLEMISC_INSIDEOUT
		| OLEMISC_ACTIVATEWHENVISIBLE;
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::SetColorScheme(LOGPALETTE *logpal)
{
	LOGMETHOD("IOleObject::SetColorScheme");
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::GetClassID(GUID *guid)
{
	LOGMETHOD("IPersist::GetClassID");
	ASSERT(ocx_info);
	*guid = ocx_info->GetCoClassGUID();
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::IsDirty()
{
	LOGMETHOD("IPersistStorage::IsDirty");
	return LOGRESULT(IsModified() ? S_OK : S_FALSE);
}

HRESULT OcxControl::InitNew(IStorage *stg)
{
	LOGMETHOD("IPersistStorage::InitNew");
	if(!stg)
		return LOGRESULT(E_INVALIDARG);
	HRESULT hr;
	IStreamPtr stream;
	if(FAILED(hr = stg->CreateStream(data_stream_name,
		STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 0, 0, &stream)))
		return LOGRESULT(hr);
	return InitNew(); // call IPersistStream::InitNew
}

HRESULT OcxControl::Load(IStorage *stg)
{
	LOGMETHOD("IPersistStorage::Load");
	if(!stg)
		return LOGRESULT(E_INVALIDARG);
	HRESULT hr;
	IStreamPtr stream;
	if(FAILED(hr = stg->OpenStream(data_stream_name, 0,
		STGM_DIRECT | STGM_SHARE_EXCLUSIVE, 0, &stream)))
		return LOGRESULT(hr);
	return Load(stream); // delegate load to IPersistStream::Load
}

HRESULT OcxControl::Save(IStorage *stg, BOOL same_as_load)
{
	LOGMETHOD("IPersistStorage::Save(" << (same_as_load ? "same as load" : "full save") << ')');
	if(!stg)
		return LOGRESULT(E_INVALIDARG);
	HRESULT hr;
#if LOG_PERSIST >= 1
	STATSTG stat;
	Zero(stat);
	if(FAILED(hr = stg->Stat(&stat, STATFLAG_NONAME)))
		return LOGRESULT(hr);
	OCXLOG(NFormat("Storage flags = %08x\r\n", (int)stat.grfMode));
	OCXLOG("CLSID = " << GetCoClassName(stat.clsid) << " - " << Format(stat.clsid));
#endif
	IStreamPtr stream;
	if(FAILED(hr = stg->CreateStream(data_stream_name,
		STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 0, 0, &stream)))
		return LOGRESULT(hr);

	if(FAILED(hr = Save(stream, true)))
		return hr;

#if LOG_PERSIST >= 1
	ULARGE_INTEGER ul;
	if(FAILED(hr = stream->Seek(AsLarge(0), STREAM_SEEK_END, &ul)))
		return hr;
	OCXLOG("- stream size = " << ul.LowPart);
#endif

	return S_OK;
}

HRESULT OcxControl::SaveCompleted(IStorage *stg)
{
	LOGMETHOD("IPersistStorage::SaveCompleted");
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::HandsOffStorage()
{
	LOGMETHOD("IPersistStorage::HandsOffStorage");
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::Load(IStream *stream)
{
	LOGMETHOD("IPersistStreamInit::Load");
	if(stream_inited)
		return LOGRESULT(E_UNEXPECTED);
#if LOG_PERSIST >= 1
	ULARGE_INTEGER old_pos, len;
	HRESULT hr;
	if(FAILED(hr = stream->Seek(AsLarge(0), STREAM_SEEK_CUR, &old_pos))
	|| FAILED(hr = stream->Seek(AsLarge(0), STREAM_SEEK_END, &len))
	|| FAILED(hr = stream->Seek(reinterpret_cast<const LARGE_INTEGER&>(old_pos), STREAM_SEEK_SET, 0)))
		return hr;
	OCXLOG("- stream size = " << len.LowPart);
#endif
	OleStream os(stream);
	if(!os.IsOpen() || os.IsError())
	{
		LOGPERSIST("- internal error creating OleStream");
		return LOGRESULT(E_FAIL);
	}
	os.SetLoading();
	Serialize(os);
	ClearModify();
	os.Close();
	stream_inited = true;
	if(os.IsError())
		return LOGRESULT(os.GetError() >= 0 ? os.GetError() : E_FAIL);
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::Save(IStream *stream, BOOL clear_dirty)
{
	LOGMETHOD("IPersistStreamInit::Save");
	OleStream os(stream);
	if(!os.IsOpen() || os.IsError())
	{
		LOGPERSIST("- internal error creating OleStream");
		return LOGRESULT(E_FAIL);
	}
	os.SetStoring();
	Serialize(os);
	os.Close();
	if(os.IsError())
		return LOGRESULT(os.GetError() >= 0 ? os.GetError() : E_FAIL);
	if(clear_dirty)
		ClearModify();
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::GetSizeMax(ULARGE_INTEGER *size)
{
	LOGMETHOD("IPersistStreamInit::GetSizeMax");
	if(!size)
		return LOGRESULT(E_INVALIDARG);
	SizeStream ss;
	Serialize(ss);
	*size = AsULarge(ss.GetSize());
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::InitNew()
{
	LOGMETHOD("IPersistStreamInit::InitNew");
	if(stream_inited)
		return LOGRESULT(E_UNEXPECTED);
	SetRect(Rect(Point(0, 0), GetStdSize()));
	SetData(Value()); // default clear algorithm
	ClearModify();
	stream_inited = true;
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::GetData(FORMATETC *format, STGMEDIUM *medium)
{
	LOGMETHOD("IDataObject::GetData");
	return LOGRESULT(E_NOTIMPL);
}

HRESULT OcxControl::GetDataHere(FORMATETC *format, STGMEDIUM *medium)
{
	LOGMETHOD("IDataObject::GetDataHere");
	return LOGRESULT(E_NOTIMPL);
}

HRESULT OcxControl::QueryGetData(FORMATETC *format)
{
	LOGMETHOD("IDataObject::QueryGetData");
	return LOGRESULT(E_NOTIMPL);
}

HRESULT OcxControl::GetCanonicalFormatEtc(FORMATETC *in, FORMATETC *out)
{
	LOGMETHOD("IDataObject::GetCanonicalFormatEtc");
	*out = *in;
	out->ptd = 0;
	return LOGRESULT(DATA_S_SAMEFORMATETC);
}

HRESULT OcxControl::SetData(FORMATETC *format, STGMEDIUM *medium, BOOL free)
{
	LOGMETHOD("IDataObject::SetData");
	if(free)
		ReleaseStgMedium(medium);
	return LOGRESULT(E_NOTIMPL);
}

HRESULT OcxControl::EnumFormatEtc(dword dir, IEnumFORMATETC **enumerator)
{
	LOGMETHOD("IDataObject::EnumFormatEtc");
	*enumerator = 0;
	return LOGRESULT(E_NOTIMPL);
}

HRESULT OcxControl::DAdvise(FORMATETC *format, dword advf, IAdviseSink *sink, dword *connection)
{
	LOGMETHOD("IDataObject::DAdvise");
	*connection = advise_sinks.Add(sink);
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::DUnadvise(dword connection)
{
	LOGMETHOD("IDataObject::DUnadvise");
	return LOGRESULT(advise_sinks.Remove(connection) ? S_OK : E_FAIL);
}

HRESULT OcxControl::EnumDAdvise(IEnumSTATDATA **enumerator)
{
	LOGMETHOD("IDataObject::EnumDAdvise");
	*enumerator = 0;
	return LOGRESULT(E_NOTIMPL);
}

HRESULT OcxControl::Draw(dword aspect, long index, void *aspectinfo,
	DVTARGETDEVICE *device, HDC target_dc, HDC draw_dc,
	const RECTL *bounds, const RECTL *wbounds,
	BOOL (STDMETHODCALLTYPE *progress)(dword arg), dword arg)
{
	LOGMETHOD("IViewObject::Draw");
	if(aspect != DVASPECT_CONTENT)
		return DV_E_DVASPECT;
//	if(IsClosed() && !CreateControl(0)) // create forged control to make paint routines work
//		return E_FAIL;
	Rect rc(GetSize());
	if(bounds)
	{
		rc.left   = bounds->left;
		rc.top    = bounds->top;
		rc.right  = bounds->right;
		rc.bottom = bounds->bottom;
		LPtoDP(draw_dc, reinterpret_cast<POINT *>(&rc), 2);
	}
//	if(IsForged())
//		SetCtrlRect(rc);

	Point worg, vorg;
	Size wext, vext;
	SetWindowOrgEx(draw_dc, 0, 0, worg);
	SetWindowExtEx(draw_dc, 1, 1, wext);
	SetViewportOrgEx(draw_dc, 0, 0, vorg);
	SetViewportExtEx(draw_dc, 1, 1, vext);
	int old_mode = SetMapMode(draw_dc, MM_TEXT);

	SystemDraw draw(draw_dc);
	draw.Offset(rc.TopLeft());
	DrawCtrl(draw);
	draw.End();

	SetMapMode(draw_dc, old_mode);
	SetWindowOrgEx(draw_dc, worg.x, worg.y, 0);
	SetWindowExtEx(draw_dc, wext.cx, wext.cy, 0);
	SetViewportOrgEx(draw_dc, vorg.x, vorg.y, 0);
	SetViewportExtEx(draw_dc, vext.cx, vext.cy, 0);

//	DUMP(GetMapMode(draw_dc));
//	DUMP(::GetWindow(draw_dc));
//	DUMP(GetViewport(draw_dc));
//	::Draw draw(draw_dc);
//	draw.DrawRect(rc, Yellow);
//	draw.Clipoff(rc);
//	CtrlPaint(draw);
//	draw.End();

	return LOGRESULT(S_OK);
}

HRESULT OcxControl::GetColorSet(dword aspect, long index, void *aspectinfo,
	DVTARGETDEVICE *device, HDC target, LOGPALETTE **palette)
{
	LOGMETHOD("IViewObject::GetColorSet");
	return LOGRESULT(E_NOTIMPL);
}

HRESULT OcxControl::Freeze(dword aspect, long index, void *aspectinfo, dword *freeze_key)
{
	LOGMETHOD("IViewObject::Freeze");
	return LOGRESULT(E_NOTIMPL);
}

HRESULT OcxControl::Unfreeze(dword freeze_key)
{
	LOGMETHOD("IViewObject::Unfreeze");
	return LOGRESULT(E_NOTIMPL);
}

HRESULT OcxControl::SetAdvise(dword aspect, dword advf, IAdviseSink *sink)
{
	LOGMETHOD("IViewObject::SetAdvise(aspect = " << aspect << ", advf = " << advf << ")");
	view_sink_aspect = aspect;
	view_sink_advf = advf;
	view_sink = sink;
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::GetAdvise(dword *aspect, dword *advf, IAdviseSink **sink)
{
	LOGMETHOD("IViewObject::GetAdvise");
	if(aspect) *aspect = view_sink_aspect;
	if(advf)   *advf   = view_sink_advf;
	if(sink && (*sink = ~view_sink))
		(**sink).AddRef();
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::GetExtent(dword aspect, long index, DVTARGETDEVICE *device, SIZEL *size)
{
	LOGMETHOD("IViewObject2::GetExtent");
	*size = GetRect().Size();
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::GetRect(dword aspect, RECTL *rc)
{
	LOGMETHOD("IViewObjectEx::GetRect(aspect = " << aspect << ")");
	if(aspect != DVASPECT_CONTENT)
		return E_NOTIMPL;
	if(!rc)
		return E_INVALIDARG;
	*rc = ToRectL(GetRect());
	return S_OK;
}

HRESULT OcxControl::GetViewStatus(dword *status)
{
	LOGMETHOD("IViewObjectEx::GetViewStatus");
	*status = (IsTransparent() ? 0 : DVASPECT_OPAQUE);
	return S_OK;
}

HRESULT OcxControl::QueryHitPoint(dword aspect, const RECT *bounds, POINT pos, long close_hint, dword *result)
{
	LOGMETHOD("IViewObjectEx::QueryHitPoint");
	if(!result)
		return E_INVALIDARG;
	if(aspect != DVASPECT_CONTENT)
		return E_FAIL;
	*result = PtInRect(bounds, pos) ? HITRESULT_HIT : HITRESULT_OUTSIDE;
	return S_OK;
}

HRESULT OcxControl::QueryHitRect(dword aspect, const RECT *bounds, const RECT *rc, long close_hint, dword *result)
{
	LOGMETHOD("IViewObjectEx::QueryHitRect");
	if(!result)
		return E_INVALIDARG;
	if(aspect != DVASPECT_CONTENT)
		return E_FAIL;
	*result = Rect(*bounds).Contains(Rect(*rc)) ? HITRESULT_HIT : HITRESULT_OUTSIDE;
	return S_OK;
}

HRESULT OcxControl::GetNaturalExtent(dword aspect, long index, DVTARGETDEVICE *ptd, HDC tdc, DVEXTENTINFO *extinfo, SIZEL *psize)
{
	LOGMETHOD("IViewObjectEx::GetNaturalExtent");
	if(aspect != DVASPECT_CONTENT)
		return E_FAIL;
	if(psize)
		*psize = GetRect().Size();
	return S_OK;
}

HRESULT OcxControl::GetWindow(HWND *hwnd)
{
	LOGMETHOD("IOleWindow::GetWindow");
	if(!hwnd)
		return LOGRESULT(E_INVALIDARG);
	return LOGRESULT((*hwnd = GetHWND()) ? S_OK : E_FAIL);
}

HRESULT OcxControl::ContextSensitiveHelp(BOOL enter_mode)
{
	LOGMETHOD("IOleWindow::ContextSensitiveHelp");
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::InPlaceDeactivate()
{
	LOGMETHOD("IOleInPlaceObject::InPlaceDeactivate");
	if(!IsActive())
		return LOGRESULT(S_OK);
	UIDeactivate();
	if(!!in_place_site)
		in_place_site->OnInPlaceDeactivate();
	if(IsOpen() && !IsChild())
		Ctrl::Close();
	in_place_site.Clear();
	client_site.Clear();
	status = CLOSED;
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::UIDeactivate()
{
	LOGMETHOD("IOleInPlaceObject::UIDeactivate");

	if(!IsUIActive())
		return LOGRESULT(S_OK); // no work needed when not UI active

	if(!in_place_site)
		return LOGRESULT(E_FAIL);

	SetActiveObject(false);
	return LOGRESULT(in_place_site->OnUIDeactivate(false));
}

HRESULT OcxControl::SetObjectRects(const RECT *pos, const RECT *clip)
{
	LOGMETHOD("IOleInPlaceObject::SetObjectRects(pos = " << Rect(*pos) << ", clip = " << Rect(*clip) << ")");
	if(!pos || !clip)
		return LOGRESULT(E_INVALIDARG);
	Rect rc(Point(pos->left, pos->top), extent_size);
	OCXLOG("-> Ctrl rc = " << rc << " (extent size = " << extent_size << ")");
	SetRect(rc);
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::ReactivateAndUndo()
{
	LOGMETHOD("IOleInPlaceObject::ReactivateAndUndo");
	if(!IsActive())
		return OLE_E_NOT_INPLACEACTIVE;
	return DoVerbUIActivate();
}

HRESULT OcxControl::TranslateAccelerator(MSG *msg)
{
	LOGMETHOD("IOleInPlaceObject::TranslateAccelerator");
	dword keycode = 0;
	if(msg->message == WM_KEYDOWN)
		keycode = KEYtoK(msg->wParam);
	else if(msg->message == WM_KEYUP)
		keycode = KEYtoK(msg->wParam) | K_KEYUP;
	else if(msg->message == WM_SYSKEYDOWN && (msg->lParam & 0x20000000))
		keycode = KEYtoK(msg->wParam);
	else if(msg->message == WM_SYSKEYUP && (msg->lParam & 0x20000000))
		keycode = KEYtoK(msg->wParam) | K_KEYUP;
	else if(msg->message == WM_CHAR)
		keycode = msg->wParam;
	if(keycode == 0 || !IsActive() || !IsEnabled() || !IsVisible())
		return LOGRESULT(S_FALSE);

	return LOGRESULT(HotKey(keycode) ? S_OK : S_FALSE);
}

HRESULT OcxControl::OnFrameWindowActivate(BOOL activate)
{
	LOGMETHOD("IOleInPlaceObject::OnFrameWindowActivate");
	Refresh();
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::OnDocWindowActivate(BOOL activate)
{
	LOGMETHOD("IOleInPlaceObject::OnDocWindowActivate");
	Refresh();
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::ResizeBorder(const RECT *border, IOleInPlaceUIWindow *ui, BOOL frame)
{
	LOGMETHOD("IOleInPlaceObject::ResizeBorder");
	Refresh();
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::EnableModeless(BOOL enable)
{
	LOGMETHOD("IOleInPlaceObject::EnableModeless");
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::GetControlInfo(CONTROLINFO *cinfo)
{
	LOGMETHOD("IOleControl::GetControlInfo");
	if(!cinfo)
		return LOGRESULT(E_INVALIDARG);
	cinfo->hAccel = 0;
	cinfo->cAccel = 0;
	cinfo->dwFlags = 0; // by default eat neither Return nor Escape
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::OnMnemonic(MSG *msg)
{
	LOGMETHOD("IOleControl::OnMnemonic");
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::OnAmbientPropertyChange(DISPID dispid)
{
	LOGMETHOD("IOleControl::OnAmbientPropertyChange");
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::FreezeEvents(BOOL freeze)
{
	LOGMETHOD("IOleControl::FreezeEvents(" << (freeze ? "on" : "off") << ")");
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::DoVerbPrimary()
{
	LOGMETHOD("DoVerbPrimary");
	return LOGRESULT(ShowControl(true));
}

HRESULT OcxControl::DoVerbShow()
{
	LOGMETHOD("DoVerbShow");
	return LOGRESULT(ShowControl(false));
}

HRESULT OcxControl::ShowControl(bool ui_activate)
{
	LOGMETHOD("OcxControl::ShowControl(" << (ui_activate ? "UI activate" : "") << ")");

	if(!in_place_site)
		return LOGRESULT(E_FAIL);

	HRESULT hr;

	if(!IsActive())
	{
		HWND parent = 0;

		if((hr = in_place_site->CanInPlaceActivate()) != S_OK
		|| FAILED(hr = in_place_site->OnInPlaceActivate())
		|| FAILED(hr = in_place_site->GetWindow(&parent)))
			return LOGRESULT(hr);

		if(!parent || !::IsWindow(parent))
			return E_FAIL;

		UpdateControlRect();

		if(!Open(parent))
			return LOGRESULT(E_FAIL);

		Show();
		Enable();

// todo: fire view change
//		if(view_sink)
//		{
//			view_sink->OnViewChange(DVASPECT_CONTENT, -1);
//			if(view_sink_advf & ADVF_ONLYONCE)
//				view_sink = 0;
//		}
//
	}

	if(ui_activate && !IsUIActive() && SUCCEEDED(in_place_site->OnUIActivate()))
		status = UIACTIVE;

	Show();
	Enable();
	if(!HasFocusDeep() && !SetWantFocus() && GetFirstChild())
		IterateFocusForward(GetFirstChild(), this);
	SetActiveObject(true);

	return LOGRESULT(client_site->ShowObject());
}

HRESULT OcxControl::DoVerbInPlaceActivate()
{
	LOGMETHOD("DoVerbInPlaceActivate");
	return LOGRESULT(ShowControl(false));
}

HRESULT OcxControl::DoVerbUIActivate()
{
	LOGMETHOD("DoVerbUIActivate");
	return LOGRESULT(ShowControl(true));
}

HRESULT OcxControl::DoVerbHide()
{
	LOGMETHOD("DoVerbHide");
	Hide();
//	Close(false);
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::DoVerbOpen()
{
	LOGMETHOD("DoVerbOpen");
	return LOGRESULT(ShowControl(false));
}

HRESULT OcxControl::DoVerbDiscardUndo()
{
	LOGMETHOD("DoVerbDiscardUndo");
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::DoVerbProperties()
{
	LOGMETHOD("DoVerbProperties");
	return LOGRESULT(S_OK);
}

void OcxControl::UpdateControlRect()
{
	LOGMETHOD("OcxControl::UpdateControlRect");

	if(!in_place_site)
		return;

	IOleInPlaceFramePtr frame;
	IOleInPlaceUIWindowPtr ui_window;
	OLEINPLACEFRAMEINFO frame_info = { sizeof(frame_info) };
	Rect rc, clip;
	if(FAILED(in_place_site->GetWindowContext(&frame, &ui_window, rc, clip, &frame_info)))
		return;

	OCXLOG("Control rect: " << rc << ", clip = " << clip);
	SetRect(rc);
}

void OcxControl::SetActiveObject(bool set)
{
	LOGMETHOD("OcxControl::SetActiveObject");
	if(!in_place_site)
		return;

	IRef<IOleInPlaceFrame> frame;
	IRef<IOleInPlaceUIWindow> ui_window;
	OLEINPLACEFRAMEINFO frame_info = { sizeof(frame_info) };
	Rect rc, clip;
	if(FAILED(in_place_site->GetWindowContext(frame.Set(), ui_window.Set(), rc, clip, &frame_info)))
		return;

	if(!!frame) {
		frame->SetActiveObject(set ? this : 0, 0);
		if(set)
			frame->SetBorderSpace(0);
	}
	if(!!ui_window) {
		ui_window->SetActiveObject(set ? this : 0, 0);
		if(set)
			ui_window->SetBorderSpace(0);
	}
}

void OcxControl::RefreshSink()
{
	LOGMETHOD("OcxControl::RefreshSink");
	if(!!view_sink)
		view_sink->OnViewChange(DVASPECT_CONTENT, -1);
}

HRESULT OcxControl::GetActivationPolicy(DWORD *pdwPolicy)
{
	LOGMETHOD("IPointerInactive::GetActivationPolicy");
	*pdwPolicy = POINTERINACTIVE_ACTIVATEONENTRY;
	return LOGRESULT(S_OK);
}

HRESULT OcxControl::OnInactiveMouseMove(LPCRECT pRectBounds, LONG x, LONG y, DWORD grfKeyState)
{
	LOGMETHOD("IPointerInactive::OnInactiveMouseMove");
	return S_OK;
}

HRESULT OcxControl::OnInactiveSetCursor(LPCRECT pRectBounds, LONG x, LONG y, DWORD dwMouseMsg, BOOL fSetAlways)
{
	LOGMETHOD("IPointerInactive::InactiveSetCursor");
	return S_OK;
}

HRESULT OcxRunnableControl::GetRunningClass(GUID *clsid)
{
	LOGMETHOD("IRunnableObject::GetRunningClass");
	*clsid = __uuidof(0);
	return LOGRESULT(E_UNEXPECTED);
}

HRESULT OcxRunnableControl::Run(LPBC ctx)
{
	LOGMETHOD("IRunnableObject::Run");
	return LOGRESULT(S_OK);
}

BOOL OcxRunnableControl::IsRunning()
{
	LOGMETHOD("IRunnableObject::IsRunning");
	return true;
}

HRESULT OcxRunnableControl::LockRunning(BOOL lock, BOOL last_unlock_closes)
{
	LOGMETHOD("IRunnableObject::LockRunning");
	return LOGRESULT(S_OK);
}

HRESULT OcxRunnableControl::SetContainedObject(BOOL contained)
{
	LOGMETHOD("IRunnableObject::SetContainedObject");
	return LOGRESULT(S_OK);
}

HRESULT STDMETHODCALLTYPE OcxConnectionPoint::Enumerator::Next(unsigned long count, CONNECTDATA *data, unsigned long *fetched)
{
	LOGMETHOD("IConnectionPoint::IEnumConnections::Next(" << (int)count << ")");
	if(!point)
		return LOGRESULT(E_FAIL);
	if(count == 0 || !data || !fetched)
		return LOGRESULT(E_INVALIDARG);
	int avail = min<int>(count, point->conn_map.GetCount() - index);
	if(avail <= 0)
	{
		*fetched = 0;
		return LOGRESULT(S_OK);
	}
	*fetched = avail;
	while(--avail >= 0)
	{
		data->pUnk = point->conn_map[index].AddRef();
		data->dwCookie = point->conn_map.GetKey(index);
		index++;
		data++;
	}
	return LOGRESULT(S_OK);
}

HRESULT STDMETHODCALLTYPE OcxConnectionPoint::Enumerator::Skip(unsigned long connections)
{
	LOGMETHOD("IConnectionPoint::IEnumConnections::Skip(" << (int)connections << ")");
	index += connections;
	return LOGRESULT(S_OK);
}

HRESULT STDMETHODCALLTYPE OcxConnectionPoint::Enumerator::Reset()
{
	LOGMETHOD("IConnectionPoint::IEnumConnections::Reset()");
	index = 0;
	return LOGRESULT(S_OK);
}

HRESULT STDMETHODCALLTYPE OcxConnectionPoint::Enumerator::Clone(IEnumConnections **enum_conn)
{
	LOGMETHOD("IConnectionPoint::IEnumConnections::Clone()");
	Enumerator *enumerator = new OcxObjectWrapper<Enumerator>;
	enumerator->Attach(~point);
	*enum_conn = enumerator;
	return LOGRESULT(S_OK);
}

HRESULT STDMETHODCALLTYPE OcxConnectionPoint::GetConnectionInterface(IID *iid)
{
	LOGMETHOD("IConnectionPoint::GetConnectionInterface()");
	*iid = guid;
	return LOGRESULT(S_OK);
}

HRESULT STDMETHODCALLTYPE OcxConnectionPoint::GetConnectionPointContainer(IConnectionPointContainer **cpc)
{
	LOGMETHOD("IConnectionPoint::GetConnectionPointContainer()");
	*cpc = container;
	if(container)
	{
		container->AddRef();
		return LOGRESULT(S_OK);
	}
	return LOGRESULT(E_FAIL);
}

HRESULT STDMETHODCALLTYPE OcxConnectionPoint::Advise(IUnknown *ptr, DWORD *cookie)
{
	LOGMETHOD("IConnectionPoint::Advise()");
	IRef<IDispatch> pif;
	if(!cookie || !ptr || FAILED(ptr->QueryInterface(guid, pif.SetVoid())))
		return LOGRESULT(E_INVALIDARG); // required interface not found
	*cookie = (free_id.IsEmpty() ? conn_map.GetCount() : free_id.Pop());
	conn_map.Add(*cookie) = pif;
	return LOGRESULT(S_OK);
}

HRESULT STDMETHODCALLTYPE OcxConnectionPoint::Unadvise(DWORD cookie)
{
	LOGMETHOD("IConnectionPoint::Unadvise(" << (int)cookie << ")");
	int p = conn_map.Find(cookie);
	if(p < 0)
		return LOGRESULT(E_INVALIDARG);
	conn_map.Remove(p);
	free_id.Add(cookie);
	return LOGRESULT(S_OK);
}

HRESULT STDMETHODCALLTYPE OcxConnectionPoint::EnumConnections(IEnumConnections **enum_conn)
{
	LOGMETHOD("IConnectionPoint::EnumConnections()");
	Enumerator *enumerator = new OcxObjectWrapper<Enumerator>;
	enumerator->Attach(this);
	*enum_conn = enumerator;
	return LOGRESULT(S_OK);
}

void OcxConnectionPoint::FireEvent(int method_id, const Vector<Value>& values)
{
	LOGMETHOD("OcxConnectionPoint::FireEvent");
	int i, nargs = values.GetCount();
	Buffer<VARIANTARG> args(nargs);
	for(i = 0; i < nargs; i++)
	{
		VARIANTARG& out = args[i];
		out.vt = VT_EMPTY;
		VariantCopy(&out, &ValueToVariant(values[nargs - i - 1]));
	}
	DISPPARAMS disp = { args, 0, nargs, 0 };
	for(i = 0; i < conn_map.GetCount(); i++)
		conn_map[i]->Invoke(method_id, IID_NULL, 0, DISPATCH_METHOD, &disp, NULL,  NULL, NULL);
	for(i = 0; i < nargs; i++)
		VariantClear(&args[i]);
}

HRESULT STDMETHODCALLTYPE OcxConnectionPointContainer::Enumerator::Next(unsigned long count, IConnectionPoint **data, unsigned long *fetched)
{
	LOGMETHOD("IConnectionPointContainer::IEnumConnectionPoints::Next(" << (int)count << ")");
	if(!point)
		return LOGRESULT(E_FAIL);
	if(count == 0 || !data || !fetched)
		return LOGRESULT(E_INVALIDARG);
	int avail = min<int>(count, point->point_map.GetCount() - index);
	if(avail <= 0)
	{
		*fetched = 0;
		return LOGRESULT(S_OK);
	}
	*fetched = avail;
	while(--avail >= 0)
	{
		*data = point->point_map[index];
		(*data)->AddRef();
		index++;
		data++;
	}
	return LOGRESULT(S_OK);
}

HRESULT STDMETHODCALLTYPE OcxConnectionPointContainer::Enumerator::Skip(unsigned long connections)
{
	LOGMETHOD("IConnectionPointContainer::IEnumConnectionPoints::Skip(" << (int)connections << ")");
	index += connections;
	return LOGRESULT(S_OK);
}

HRESULT STDMETHODCALLTYPE OcxConnectionPointContainer::Enumerator::Reset()
{
	LOGMETHOD("IConnectionPointContainer::IEnumConnectionPoints::Reset()");
	index = 0;
	return LOGRESULT(S_OK);
}

HRESULT STDMETHODCALLTYPE OcxConnectionPointContainer::Enumerator::Clone(IEnumConnectionPoints **enum_conn)
{
	LOGMETHOD("IConnectionPointContainer::IEnumConnectionPoints::Clone()");
	Enumerator *enumerator = new OcxObjectWrapper<Enumerator>;
	enumerator->Attach(~point);
	*enum_conn = enumerator;
	return LOGRESULT(S_OK);
}

OcxConnectionPointContainer::OcxConnectionPointContainer()
{
}

HRESULT OcxConnectionPointContainer::EnumConnectionPoints(IEnumConnectionPoints **ppEnum)
{
	LOGMETHOD("IConnectionPointContainer::EnumConnectionPoints()");
	Enumerator *enumerator = new OcxObjectWrapper<Enumerator>;
	enumerator->Attach(this);
	*ppEnum = enumerator;
	return LOGRESULT(S_OK);
}

HRESULT OcxConnectionPointContainer::FindConnectionPoint(REFIID riid, IConnectionPoint **ppCP)
{
	Guid guid(riid);
	LOGMETHOD("IConnectionPointContainer::FindConnectionPoint(" << guid << ", ppCP = " << FormatIntHex(ppCP) << ")");
	int i = point_map.Find(guid);
	RLOG("point_map.Find(guid) = " << i);
	if(i < 0) {
		*ppCP = 0;
		return LOGRESULT(CONNECT_E_NOCONNECTION);
	}
	(*ppCP = point_map[i])->AddRef();
	return LOGRESULT(S_OK);
}

END_UPP_NAMESPACE

#endif
