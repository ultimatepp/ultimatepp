#include <CtrlCore/CtrlCore.h>

#ifdef GUI_WINALT

NAMESPACE_UPP

#define LLOG(x) // LOG(x)

int  GetClipboardFormatCode(const char *format_id);

int ToWin32CF(const char *s)
{
	return GetClipboardFormatCode(s);
}

String FromWin32CF(int cf)
{
	GuiLock __;
	if(cf == CF_TEXT)
		return "text";
	if(cf == CF_UNICODETEXT)
		return "wtext";
	if(cf == CF_DIB)
		return "dib";
#ifndef PLATFORM_WINCE
	if(cf == CF_HDROP)
		return "files";
#endif
	char h[256];
	GetClipboardFormatNameA(cf, h, 255);
	return h;
}

FORMATETC ToFORMATETC(const char *s)
{
	FORMATETC fmtetc;
	fmtetc.cfFormat = ToWin32CF(s);
	fmtetc.dwAspect = DVASPECT_CONTENT;
	fmtetc.lindex = -1;
	fmtetc.ptd = NULL;
	fmtetc.tymed = TYMED_HGLOBAL;
	return fmtetc;
}

String AsString(POINTL p)
{
	return String().Cat() << "[" << p.x << ", " << p.y << "]";
}

struct UDropTarget : public IDropTarget {
	ULONG         rc;
	LPDATAOBJECT  data;
	Ptr<Ctrl>     ctrl;
	Index<String> fmt;

	STDMETHOD(QueryInterface)(REFIID riid, void **ppvObj);
	STDMETHOD_(ULONG, AddRef)(void)  { return ++rc; }
	STDMETHOD_(ULONG, Release)(void) { if(--rc == 0) { delete this; return 0; } return rc; }

	STDMETHOD(DragEnter)(LPDATAOBJECT pDataObj, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect);
	STDMETHOD(DragOver)(DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect);
	STDMETHOD(DragLeave)();
	STDMETHOD(Drop)(LPDATAOBJECT pDataObj, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect);

	void DnD(POINTL p, bool drop, DWORD *effect, DWORD keys);
	void FreeData();
	void Repeat();
	void EndDrag();
	String Get(const char *fmt) const;

	UDropTarget() { rc = 1; data = NULL; }
	~UDropTarget();
};

bool Has(UDropTarget *dt, const char *fmt)
{
	return dt->fmt.Find(fmt) >= 0;
}

String Get(UDropTarget *dt, const char *fmt)
{
	return dt->Get(fmt);
}

STDMETHODIMP UDropTarget::QueryInterface(REFIID iid, void ** ppv)
{
	if(iid == IID_IUnknown || iid == IID_IDropTarget) {
		*ppv = this;
		AddRef();
		return S_OK;
	}
	*ppv = NULL;
	return E_NOINTERFACE;
}

String UDropTarget::Get(const char *fmt) const
{
	FORMATETC fmtetc = ToFORMATETC(fmt);
	STGMEDIUM s;
	if(data->GetData(&fmtetc, &s) == S_OK && s.tymed == TYMED_HGLOBAL) {
		char *val = (char *)GlobalLock(s.hGlobal);
		String data(val, (int)GlobalSize(s.hGlobal));
		GlobalUnlock(s.hGlobal);
		ReleaseStgMedium(&s);
		return data;
    }
	return Null;
}

void UDropTarget::DnD(POINTL pl, bool drop, DWORD *effect, DWORD keys)
{
	GuiLock __;
	LLOG("DnD effect: " << *effect);
	dword e = *effect;
	*effect = DROPEFFECT_NONE;
	if(!ctrl)
		return;
	PasteClip d;
	d.dt = this;
	d.paste = drop;
	d.accepted = false;
	d.allowed = 0;
	d.action = 0;
	if(e & DROPEFFECT_COPY) {
		LLOG("DnD DROPEFFECT_COPY");
		d.allowed = DND_COPY;
		d.action = DND_COPY;
	}
	if(e & DROPEFFECT_MOVE) {
		LLOG("DnD DROPEFFECT_MOVE");
		d.allowed |= DND_MOVE;
		if(Ctrl::GetDragAndDropSource())
			d.action = DND_MOVE;
	}
	LLOG("DnD keys & MK_CONTROL:" << (keys & MK_CONTROL));
	if((keys & MK_CONTROL) && (d.allowed & DND_COPY))
		d.action = DND_COPY;
	if((keys & (MK_ALT|MK_SHIFT)) && (d.allowed & DND_MOVE))
		d.action = DND_MOVE;
	ctrl->DnD(Point(pl.x, pl.y), d);
	if(d.IsAccepted()) {
		LLOG("DnD accepted, action: " << (int)d.action);
		if(d.action == DND_MOVE)
			*effect = DROPEFFECT_MOVE;
		if(d.action == DND_COPY)
			*effect = DROPEFFECT_COPY;
	}
}

void UDropTarget::Repeat()
{
	Ctrl::DnDRepeat();
}

STDMETHODIMP UDropTarget::DragEnter(LPDATAOBJECT pDataObj, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)
{
	GuiLock __;
	LLOG("DragEnter " << pt);
	data = pDataObj;
	data->AddRef();
	fmt.Clear();
	IEnumFORMATETC *fe;
	if(!ctrl || pDataObj->EnumFormatEtc(DATADIR_GET, &fe) != NOERROR) {
		*pdwEffect = DROPEFFECT_NONE;
		return NOERROR;
	}
	FORMATETC fmtetc;
	while(fe->Next(1, &fmtetc, 0) == S_OK) {
		fmt.FindAdd(FromWin32CF(fmtetc.cfFormat));
		if(fmtetc.ptd)
			CoTaskMemFree(fmtetc.ptd);
	}
	fe->Release();
	DnD(pt, false, pdwEffect, grfKeyState);
	return NOERROR;
}


STDMETHODIMP UDropTarget::DragOver(DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)
{
	LLOG("DragOver " << pt << " keys: " << grfKeyState);
	DnD(pt, false, pdwEffect, grfKeyState);
	return NOERROR;
}

void UDropTarget::FreeData()
{
	if(data) {
		data->Release();
		data = NULL;
	}
}

void UDropTarget::EndDrag()
{
	Ctrl::DnDLeave();
}

STDMETHODIMP UDropTarget::DragLeave()
{
	LLOG("DragLeave");
	EndDrag();
	FreeData();
	return NOERROR;
}

STDMETHODIMP UDropTarget::Drop(LPDATAOBJECT, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)
{
	LLOG("Drop");
	if(Ctrl::GetDragAndDropSource())
		Ctrl::OverrideCursor(Null);
	DnD(pt, true, pdwEffect, grfKeyState);
	EndDrag();
	FreeData();
	return NOERROR;
}

UDropTarget::~UDropTarget()
{
	if(data) data->Release();
	EndDrag();
}

// --------------------------------------------------------------------------------------------

Ptr<Ctrl> sDnDSource;

Ctrl * Ctrl::GetDragAndDropSource()
{
	return sDnDSource;
}

struct  UDataObject : public IDataObject {
	ULONG                       rc;
	dword                       effect;
	VectorMap<String, ClipData> data;

	STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppvObj);
	STDMETHOD_(ULONG, AddRef)(void)  { return ++rc; }
	STDMETHOD_(ULONG, Release)(void) { if(--rc == 0) { delete this; return 0; } return rc; }

	STDMETHOD(GetData)(FORMATETC *fmtetc, STGMEDIUM *medium);
	STDMETHOD(GetDataHere)(FORMATETC *, STGMEDIUM *);
	STDMETHOD(QueryGetData)(FORMATETC *fmtetc);
	STDMETHOD(GetCanonicalFormatEtc)(FORMATETC *, FORMATETC *pformatetcOut);
	STDMETHOD(SetData)(FORMATETC *fmtetc, STGMEDIUM *medium, BOOL release);
	STDMETHOD(EnumFormatEtc)(DWORD dwDirection, IEnumFORMATETC **ief);
	STDMETHOD(DAdvise)(FORMATETC *, DWORD, IAdviseSink *, DWORD *);
	STDMETHOD(DUnadvise)(DWORD);
	STDMETHOD(EnumDAdvise)(LPENUMSTATDATA *);

	UDataObject() { rc = 1; effect = 0; }
};

struct UEnumFORMATETC : public IEnumFORMATETC {
	ULONG        rc;
	int          ii;
	UDataObject *data;

	STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppvObj);
	STDMETHOD_(ULONG, AddRef)(void)  { return ++rc; }
	STDMETHOD_(ULONG, Release)(void) { if(--rc == 0) { delete this; return 0; } return rc; }

	STDMETHOD(Next)(ULONG n, FORMATETC *fmtetc, ULONG *fetched);
	STDMETHOD(Skip)(ULONG n);
	STDMETHOD(Reset)(void);
	STDMETHOD(Clone)(IEnumFORMATETC **newEnum);

	UEnumFORMATETC()  { ii = 0; rc = 1; }
	~UEnumFORMATETC() { data->Release(); }
};

struct UDropSource : public IDropSource {
	ULONG rc;
	Image no, move, copy;

	STDMETHOD(QueryInterface)(REFIID riid, void ** ppvObj);
	STDMETHOD_(ULONG, AddRef)(void)  { return ++rc; }
	STDMETHOD_(ULONG, Release)(void) { if(--rc == 0) { delete this; return 0; } return rc; }

	STDMETHOD(QueryContinueDrag)(BOOL fEscapePressed, DWORD grfKeyState);
	STDMETHOD(GiveFeedback)(DWORD dwEffect);

	UDropSource() { rc = 1; }
};

STDMETHODIMP UDataObject::QueryInterface(REFIID iid, void ** ppv)
{
	if(iid == IID_IUnknown || iid == IID_IDataObject) {
		*ppv = this;
		AddRef();
		return S_OK;
	}
	*ppv = NULL;
	return E_NOINTERFACE;
}

void SetMedium(STGMEDIUM *medium, const String& data)
{
	int sz = data.GetCount();
	HGLOBAL hData = GlobalAlloc(0, sz + 4);
	if (hData) {
		char *ptr = (char *) GlobalLock(hData);
		memcpy(ptr, ~data, sz);
		memset(ptr + sz, 0, 4);
		GlobalUnlock(hData);
		medium->tymed = TYMED_HGLOBAL;
		medium->hGlobal = hData;
		medium->pUnkForRelease = 0;
	}
}

STDMETHODIMP UDataObject::GetData(FORMATETC *fmtetc, STGMEDIUM *medium)
{
	String fmt = FromWin32CF(fmtetc->cfFormat);
	ClipData *s = data.FindPtr(fmt);
	if(s) {
		String q = s->Render();
		SetMedium(medium, q.GetCount() ? q : sDnDSource ? sDnDSource->GetDropData(fmt) : String());
		return S_OK;
	}
	return DV_E_FORMATETC;
}

STDMETHODIMP UDataObject::GetDataHere(FORMATETC *, STGMEDIUM *)
{
    return DV_E_FORMATETC;
}

STDMETHODIMP UDataObject::QueryGetData(FORMATETC *fmtetc)
{
	return data.Find(FromWin32CF(fmtetc->cfFormat)) >= 0 ? S_OK : DV_E_FORMATETC;
}

STDMETHODIMP UDataObject::GetCanonicalFormatEtc(FORMATETC *, FORMATETC *pformatetcOut)
{
    pformatetcOut->ptd = NULL;
    return E_NOTIMPL;
}

#ifdef PLATFORM_WINCE
static int CF_PERFORMEDDROPEFFECT = RegisterClipboardFormat(_T("Performed DropEffect"));
#else
static int CF_PERFORMEDDROPEFFECT = RegisterClipboardFormat("Performed DropEffect");
#endif

STDMETHODIMP UDataObject::SetData(FORMATETC *fmtetc, STGMEDIUM *medium, BOOL release)
{
	if(fmtetc->cfFormat == CF_PERFORMEDDROPEFFECT && medium->tymed == TYMED_HGLOBAL) {
        DWORD *val = (DWORD*)GlobalLock(medium->hGlobal);
        effect = *val;
        GlobalUnlock(medium->hGlobal);
        if(release)
            ReleaseStgMedium(medium);
        return S_OK;
    }
	return E_NOTIMPL;
}

STDMETHODIMP UDataObject::EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC **ief)
{
	UEnumFORMATETC *ef = new UEnumFORMATETC;
	ef->data = this;
	AddRef();
	*ief = ef;
	return S_OK;
}

STDMETHODIMP UDataObject::DAdvise(FORMATETC *, DWORD, IAdviseSink *, DWORD *)
{
	return OLE_E_ADVISENOTSUPPORTED;
}


STDMETHODIMP UDataObject::DUnadvise(DWORD)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

STDMETHODIMP UDataObject::EnumDAdvise(LPENUMSTATDATA FAR*)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

STDMETHODIMP UEnumFORMATETC::QueryInterface(REFIID riid, void FAR* FAR* ppvObj)
{
	if (riid == IID_IUnknown || riid == IID_IEnumFORMATETC) {
		*ppvObj = this;
		AddRef();
		return NOERROR;
    }
	*ppvObj = NULL;
	return ResultFromScode(E_NOINTERFACE);
}

STDMETHODIMP UEnumFORMATETC::Next(ULONG n, FORMATETC *t, ULONG *fetched) {
	if(t == NULL)
		return E_INVALIDARG;
	if(fetched) *fetched = 0;
	while(ii < data->data.GetCount() && n > 0) {
		if(fetched) (*fetched)++;
		n--;
		*t++ = ToFORMATETC(data->data.GetKey(ii++));
	}
	return n ? S_FALSE : NOERROR;
}

STDMETHODIMP UEnumFORMATETC::Skip(ULONG n) {
	ii += n;
	if(ii >= data->data.GetCount())
		return S_FALSE;
	return NOERROR;
}

STDMETHODIMP UEnumFORMATETC::Reset()
{
    ii = 0;
    return NOERROR;
}

STDMETHODIMP UEnumFORMATETC::Clone(IEnumFORMATETC **newEnum)
{
	if(newEnum == NULL)
		return E_INVALIDARG;
	UEnumFORMATETC *ef = new UEnumFORMATETC;
	ef->data = data;
	data->AddRef();
	ef->ii = ii;
	*newEnum = ef;
	return NOERROR;
}

STDMETHODIMP UDropSource::QueryInterface(REFIID riid, void **ppvObj)
{
	if (riid == IID_IUnknown || riid == IID_IDropSource) {
		*ppvObj = this;
		AddRef();
		return NOERROR;
	}
	*ppvObj = NULL;
	return ResultFromScode(E_NOINTERFACE);
}

STDMETHODIMP UDropSource::QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState)
{
	if(fEscapePressed)
		return DRAGDROP_S_CANCEL;
	else
	if(!(grfKeyState & (MK_LBUTTON|MK_MBUTTON|MK_RBUTTON)))
		return DRAGDROP_S_DROP;
	Ctrl::ProcessEvents();
	return NOERROR;
}

STDMETHODIMP UDropSource::GiveFeedback(DWORD dwEffect)
{
	LLOG("GiveFeedback");
	Image m = IsNull(move) ? copy : move;
	if((dwEffect & DROPEFFECT_COPY) == DROPEFFECT_COPY) {
		LLOG("GiveFeedback COPY");
		if(!IsNull(copy)) m = copy;
	}
	else
	if((dwEffect & DROPEFFECT_MOVE) == DROPEFFECT_MOVE) {
		LLOG("GiveFeedback MOVE");
		if(!IsNull(move)) m = move;
	}
	else
		m = no;
	Ctrl::OverrideCursor(m);
	Ctrl::SetMouseCursor(m);
	return S_OK;
}

Image MakeDragImage(const Image& arrow, Image sample);

Image MakeDragImage(const Image& arrow, const Image& arrow98, Image sample)
{
	if(IsWin2K())
		return MakeDragImage(arrow, sample);
	else
		return arrow98;
}

int Ctrl::DoDragAndDrop(const char *fmts, const Image& sample, dword actions,
                        const VectorMap<String, ClipData>& data)
{
	UDataObject *obj = new UDataObject;
	obj->data <<= data;
	if(fmts) {
		Vector<String> f = Split(fmts, ';');
		for(int i = 0; i < f.GetCount(); i++)
			obj->data.GetAdd(f[i]);
	}
	UDropSource *dsrc = new UDropSource;
	DWORD result = 0;
	Image m = Ctrl::OverrideCursor(CtrlCoreImg::DndMove());
	dsrc->no = MakeDragImage(CtrlCoreImg::DndNone(), CtrlCoreImg::DndNone98(), sample);
	if(actions & DND_COPY)
		dsrc->copy = actions & DND_EXACTIMAGE ? sample : MakeDragImage(CtrlCoreImg::DndCopy(), CtrlCoreImg::DndCopy98(), sample);
	if(actions & DND_MOVE)
		dsrc->move = actions & DND_EXACTIMAGE ? sample : MakeDragImage(CtrlCoreImg::DndMove(), CtrlCoreImg::DndMove98(), sample);
	sDnDSource = this;
	int level = LeaveGMutexAll();
	HRESULT r = DoDragDrop(obj, dsrc,
	                       (actions & DND_COPY ? DROPEFFECT_COPY : 0) |
	                       (actions & DND_MOVE ? DROPEFFECT_MOVE : 0), &result);
	EnterGMutex(level);
	DWORD re = obj->effect;
	obj->Release();
	dsrc->Release();
	OverrideCursor(m);
	SyncCaret();
	CheckMouseCtrl();
	KillRepeat();
	sDnDSource = NULL;
	if(r == DRAGDROP_S_DROP) {
		if(((result | re) & DROPEFFECT_MOVE) == DROPEFFECT_MOVE && (actions & DND_MOVE))
			return DND_MOVE;
		if(((result | re) & DROPEFFECT_COPY) == DROPEFFECT_COPY && (actions & DND_COPY))
			return DND_COPY;
    }
	return DND_NONE;
}

void ReleaseUDropTarget(UDropTarget *dt)
{
	dt->Release();
}

UDropTarget *NewUDropTarget(Ctrl *ctrl)
{
	UDropTarget *dt = new UDropTarget;
	dt->ctrl = ctrl;
	return dt;
}

void Ctrl::SetSelectionSource(const char *fmts) {}

END_UPP_NAMESPACE

#endif
