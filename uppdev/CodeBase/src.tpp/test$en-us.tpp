TITLE("class Ctrl")
REF("::Ctrl::class Ctrl")
REF("::Ctrl::enum PlacementConstants { CENTER, LEFT, RIGHT, SIZE, MINSIZE, MAXSIZE, STDSIZE }")
REF("::Ctrl::enum { TOP, BOTTOM }")
REF("::Ctrl::enum StateReason { FOCUS, ACTIVATE, DEACTIVATE, SHOW, ENABLE, EDITABLE, OPEN, CLOSE }")
REF("::Ctrl::enum MouseEvents { BUTTON, ACTION, MOUSEENTER, MOUSEMOVE, MOUSELEAVE, CURSORIMAGE, MOUSEWHEEL, DOWN, UP, DOUBLE, REPEAT, LEFTDOWN, LEFTDOUBLE, LEFTREPEAT, LEFTUP, RIGHTDOWN, RIGHTDOUBLE, RIGHTREPEAT, RIGHTUP }")
REF("::Ctrl::enum { NOBACKPAINT, FULLBACKPAINT, TRANSPARENTBACKPAINT, EXCLUDEPAINT }")
REF("::Ctrl::typedef bool (*MouseHook)(Ctrl *ctrl, bool inframe, int event, Point p, int zdelta, dword keyflags)")
REF("::Ctrl::typedef bool (*KeyHook)(Ctrl *ctrl, dword key, int count)")
REF("::Ctrl::typedef bool (*StateHook)(Ctrl *ctrl, int reason)")
REF("::Ctrl::Ctrl()")
REF("::Ctrl::~Ctrl()")
REF("::Ctrl::Accept()")
REF("::Ctrl::Reject()")
REF("::Ctrl::Serialize(Stream&)")
REF("::Ctrl::SetModify()")
REF("::Ctrl::ClearModify()")
REF("::Ctrl::IsModified()const")
REF("::Ctrl::PosLeft(int,int)")
REF("::Ctrl::PosRight(int,int)")
REF("::Ctrl::PosTop(int,int)")
REF("::Ctrl::PosBottom(int,int)")
REF("::Ctrl::PosSize(int,int)")
REF("::Ctrl::PosCenter(int,int)")
REF("::Ctrl::PosCenter(int)")
REF("::Ctrl::KEYtoK(dword)")
REF("::Ctrl::Windows()")
REF("::Ctrl::MakeWin32Happy()")
REF("::Ctrl::WndProc(HWND,UINT,WPARAM,LPARAM)")
REF("::Ctrl::GetScreenClient(HWND)")
REF("::Ctrl::Create(HWND,DWORD,DWORD,bool,int)")
REF("::Ctrl::DoMouse(int,Point,int)")
REF("::Ctrl::PreDestroy()")
REF("::Ctrl::WindowProc(UINT,WPARAM,LPARAM)")
REF("::Ctrl::GetHWND()const")
REF("::Ctrl::GetOwnerHWND()const")
REF("::Ctrl::CtrlFromHWND(HWND)")
REF("::Ctrl::Create(Ctrl*,bool,bool)")
REF("::Ctrl::SyncEvents(Window,int)")
REF("::Ctrl::SyncEvents(int)")
REF("::Ctrl::EventProc(XWindow&,XEvent*)")
REF("::Ctrl::GetWindow()const")
REF("::Ctrl::CtrlFromWindow(Window)")
REF("::Ctrl::DoRemove()")
REF("::Ctrl::GetSysWindowViewOffset()")
REF("::Ctrl::Unicode()")
REF("::Ctrl::GetTopCtrls()")
REF("::Ctrl::GetTopWindows()")
REF("::Ctrl::CloseTopCtrls()")
REF("::Ctrl::InstallMouseHook(MouseHook)")
REF("::Ctrl::DeinstallMouseHook(MouseHook)")
REF("::Ctrl::InstallKeyHook(KeyHook)")
REF("::Ctrl::DeinstallKeyHook(KeyHook)")
REF("::Ctrl::InstallStateHook(StateHook)")
REF("::Ctrl::DeinstallStateHook(StateHook)")
REF("::Ctrl::Paint(Draw&)")
REF("::Ctrl::CancelMode()")
REF("::Ctrl::Activate()")
REF("::Ctrl::Deactivate()")
REF("::Ctrl::FrameMouseEvent(int,Point,int,dword)")
REF("::Ctrl::MouseEvent(int,Point,int,dword)")
REF("::Ctrl::MouseEnter(Point,dword)")
REF("::Ctrl::MouseMove(Point,dword)")
REF("::Ctrl::LeftDown(Point,dword)")
REF("::Ctrl::LeftDouble(Point,dword)")
REF("::Ctrl::LeftRepeat(Point,dword)")
REF("::Ctrl::LeftUp(Point,dword)")
REF("::Ctrl::RightDown(Point,dword)")
REF("::Ctrl::RightDouble(Point,dword)")
REF("::Ctrl::RightRepeat(Point,dword)")
REF("::Ctrl::RightUp(Point,dword)")
REF("::Ctrl::MouseWheel(Point,int,dword)")
REF("::Ctrl::MouseLeave()")
REF("::Ctrl::CursorImage(Point,dword)")
REF("::Ctrl::Key(dword,int)")
REF("::Ctrl::WKey(dword,int)")
REF("::Ctrl::GotFocus()")
REF("::Ctrl::LostFocus()")
REF("::Ctrl::HotKey(dword)")
REF("::Ctrl::PostInput()")
REF("::Ctrl::ChildGotFocus()")
REF("::Ctrl::ChildLostFocus()")
REF("::Ctrl::ChildAdded(Ctrl*)")
REF("::Ctrl::ChildRemoved(Ctrl*)")
REF("::Ctrl::ParentChange()")
REF("::Ctrl::State(int)")
REF("::Ctrl::Layout()")
REF("::Ctrl::GetMinSize()const")
REF("::Ctrl::GetStdSize()const")
REF("::Ctrl::GetMaxSize()const")
REF("::Ctrl::IsShowEnabled()const")
REF("::Ctrl::SetLabelText(const char*)")
REF("::Ctrl::GetLabelText()const")
REF("::Ctrl::Updated()")
REF("::Ctrl::Close()")
REF("::Ctrl::IsOcxChild()")
REF("::Ctrl::AddChild(Ctrl*)")
REF("::Ctrl::AddChild(Ctrl*,Ctrl*)")
REF("::Ctrl::AddChildBefore(Ctrl*,Ctrl*)")
REF("::Ctrl::RemoveChild(Ctrl*)")
REF("::Ctrl::GetParent()const")
REF("::Ctrl::GetLastChild()const")
REF("::Ctrl::GetFirstChild()const")
REF("::Ctrl::GetPrev()const")
REF("::Ctrl::GetNext()const")
REF("::Ctrl::IsChild()const")
REF("::Ctrl::ChildFromPoint(Point&)const")
REF("::Ctrl::IsForeground()const")
REF("::Ctrl::SetForeground()")
REF("::Ctrl::GetTopCtrl()const")
REF("::Ctrl::GetTopCtrl()")
REF("::Ctrl::GetOwner()const")
REF("::Ctrl::GetOwner()")
REF("::Ctrl::GetTopCtrlOwner()const")
REF("::Ctrl::GetTopCtrlOwner()")
REF("::Ctrl::GetTopWindow()const")
REF("::Ctrl::GetTopWindow()")
REF("::Ctrl::SetFrame(int,CtrlFrame&)")
REF("::Ctrl::SetFrame(CtrlFrame&)")
REF("::Ctrl::AddFrame(CtrlFrame&)")
REF("::Ctrl::GetFrame(int)const")
REF("::Ctrl::GetFrame(int)")
REF("::Ctrl::RemoveFrame(int)")
REF("::Ctrl::RemoveFrame(CtrlFrame&)")
REF("::Ctrl::InsertFrame(int,CtrlFrame&)")
REF("::Ctrl::FindFrame(CtrlFrame&)")
REF("::Ctrl::GetFrameCount()const")
REF("::Ctrl::ClearFrames()")
REF("::Ctrl::IsOpen()const")
REF("::Ctrl::Shutdown()")
REF("::Ctrl::IsShutdown()const")
REF("::Ctrl::SetPos(LogPos,bool)")
REF("::Ctrl::SetPos(LogPos)")
REF("::Ctrl::SetPos(Logc,Logc)")
REF("::Ctrl::SetPosX(Logc)")
REF("::Ctrl::SetPosY(Logc)")
REF("::Ctrl::SetRect(const Rect&)")
REF("::Ctrl::SetRect(int,int,int,int)")
REF("::Ctrl::SetRectX(int,int)")
REF("::Ctrl::SetRectY(int,int)")
REF("::Ctrl::SetFramePos(LogPos)")
REF("::Ctrl::SetFramePos(Logc,Logc)")
REF("::Ctrl::SetFramePosX(Logc)")
REF("::Ctrl::SetFramePosY(Logc)")
REF("::Ctrl::SetFrameRect(const Rect&)")
REF("::Ctrl::SetFrameRect(int,int,int,int)")
REF("::Ctrl::SetFrameRectX(int,int)")
REF("::Ctrl::SetFrameRectY(int,int)")
REF("::Ctrl::InFrame()const")
REF("::Ctrl::InView()const")
REF("::Ctrl::GetPos()const")
REF("::Ctrl::RefreshLayout()")
REF("::Ctrl::RefreshParentLayout()")
REF("::Ctrl::LeftPos(int,int)")
REF("::Ctrl::RightPos(int,int)")
REF("::Ctrl::TopPos(int,int)")
REF("::Ctrl::BottomPos(int,int)")
REF("::Ctrl::HSizePos(int,int)")
REF("::Ctrl::VSizePos(int,int)")
REF("::Ctrl::SizePos()")
REF("::Ctrl::HCenterPos(int,int)")
REF("::Ctrl::VCenterPos(int,int)")
REF("::Ctrl::LeftPosZ(int,int)")
REF("::Ctrl::RightPosZ(int,int)")
REF("::Ctrl::TopPosZ(int,int)")
REF("::Ctrl::BottomPosZ(int,int)")
REF("::Ctrl::HSizePosZ(int,int)")
REF("::Ctrl::VSizePosZ(int,int)")
REF("::Ctrl::HCenterPosZ(int,int)")
REF("::Ctrl::VCenterPosZ(int,int)")
REF("::Ctrl::LockLayout()")
REF("::Ctrl::UnlockLayout()")
REF("::Ctrl::GetRect()const")
REF("::Ctrl::GetScreenRect()const")
REF("::Ctrl::GetView()const")
REF("::Ctrl::GetScreenView()const")
REF("::Ctrl::GetSize()const")
REF("::Ctrl::GetVisibleScreenRect()const")
REF("::Ctrl::GetVisibleScreenView()const")
REF("::Ctrl::AddFrameSize(int,int)const")
REF("::Ctrl::AddFrameSize(Size)const")
REF("::Ctrl::Refresh(const Rect&)")
REF("::Ctrl::Refresh(int,int,int,int)")
REF("::Ctrl::Refresh()")
REF("::Ctrl::IsFullRefresh()const")
REF("::Ctrl::RefreshFrame(const Rect&)")
REF("::Ctrl::RefreshFrame(int,int,int,int)")
REF("::Ctrl::RefreshFrame()")
REF("::Ctrl::ScrollView(const Rect&,int,int)")
REF("::Ctrl::ScrollView(int,int,int,int,int,int)")
REF("::Ctrl::ScrollView(int,int)")
REF("::Ctrl::ScrollView(const Rect&,Size)")
REF("::Ctrl::ScrollView(Size)")
REF("::Ctrl::Sync()")
REF("::Ctrl::Sync(const Rect&)")
REF("::Ctrl::DrawCtrl(Draw&,int,int)")
REF("::Ctrl::DrawCtrlWithParent(Draw&,int,int)")
REF("::Ctrl::HasChild(Ctrl*)const")
REF("::Ctrl::HasChildDeep(Ctrl*)const")
REF("::Ctrl::SetMnemonic(int)")
REF("::Ctrl::GetMnemonic()const")
REF("::Ctrl::IsMnemonic(int)const")
REF("::Ctrl::SetLabel(const char*)")
REF("::Ctrl::IgnoreMouse(bool)")
REF("::Ctrl::NoIgnoreMouse()")
REF("::Ctrl::HasMouse()const")
REF("::Ctrl::IsMouseInFrame()")
REF("::Ctrl::IsMouseIn()")
REF("::Ctrl::IgnoreMouseClick()")
REF("::Ctrl::IgnoreMouseUp()")
REF("::Ctrl::SetCapture()")
REF("::Ctrl::ReleaseCapture()")
REF("::Ctrl::HasCapture()const")
REF("::Ctrl::ReleaseCtrlCapture()")
REF("::Ctrl::SetFocus()")
REF("::Ctrl::HasFocus()const")
REF("::Ctrl::HasFocusDeep()const")
REF("::Ctrl::WantFocus(bool)")
REF("::Ctrl::NoWantFocus()")
REF("::Ctrl::IsWantFocus()const")
REF("::Ctrl::SetWantFocus()")
REF("::Ctrl::GetFocusChild()const")
REF("::Ctrl::GetFocusChildDeep()const")
REF("::Ctrl::CancelModeDeep()")
REF("::Ctrl::SetCaret(int,int,int,int)")
REF("::Ctrl::SetCaret(const Rect&)")
REF("::Ctrl::KillCaret()")
REF("::Ctrl::GetFocusCtrl()")
REF("::Ctrl::IterateFocusForward(Ctrl*,Ctrl*,bool)")
REF("::Ctrl::IterateFocusBackward(Ctrl*,Ctrl*,bool)")
REF("::Ctrl::Show(bool)")
REF("::Ctrl::Hide()")
REF("::Ctrl::IsShown()const")
REF("::Ctrl::IsVisible()const")
REF("::Ctrl::Enable(bool)")
REF("::Ctrl::Disable()")
REF("::Ctrl::IsEnabled()const")
REF("::Ctrl::SetEditable(bool)")
REF("::Ctrl::SetReadOnly()")
REF("::Ctrl::IsEditable()const")
REF("::Ctrl::IsReadOnly()const")
REF("::Ctrl::ResetModify()")
REF("::Ctrl::IsModifySet()const")
REF("::Ctrl::UpdateRefresh()")
REF("::Ctrl::Update()")
REF("::Ctrl::Action()")
REF("::Ctrl::UpdateAction()")
REF("::Ctrl::UpdateActionRefresh()")
REF("::Ctrl::BackPaint(int)")
REF("::Ctrl::TransparentBackPaint()")
REF("::Ctrl::NoBackPaint()")
REF("::Ctrl::GetBackPaint()")
REF("::Ctrl::Transparent(bool)")
REF("::Ctrl::NoTransparent()")
REF("::Ctrl::IsTransparent()const")
REF("::Ctrl::ActiveX(bool)")
REF("::Ctrl::NoActiveX()")
REF("::Ctrl::IsActiveX()const")
REF("::Ctrl::DesignMode(bool)")
REF("::Ctrl::IsDesignMode()const")
REF("::Ctrl::Info(const char*)")
REF("::Ctrl::GetInfo()const")
REF("::Ctrl::Tip(const char*)")
REF("::Ctrl::HelpLine(const char*)")
REF("::Ctrl::Description(const char*)")
REF("::Ctrl::HelpTopic(const char*)")
REF("::Ctrl::GetTip()const")
REF("::Ctrl::GetHelpLine()const")
REF("::Ctrl::GetDescription()const")
REF("::Ctrl::GetHelpTopic()const")
REF("::Ctrl::Add(Ctrl&)")
REF("::Ctrl::operator<<(Ctrl&)")
REF("::Ctrl::Remove()")
REF("::Ctrl::operator<<=(const Value&)")
REF("::Ctrl::operator<<=(Callback)")
REF("::Ctrl::operator<<(Callback)")
REF("::Ctrl::SetTimeCallback(int,Callback,int)")
REF("::Ctrl::KillTimeCallback(int)")
REF("::Ctrl::ExistsTimeCallback(int)const")
REF("::Ctrl::GetActiveCtrl()")
REF("::Ctrl::GetActiveWindow()")
REF("::Ctrl::GetVisibleChild(Ctrl*,Point,bool)")
REF("::Ctrl::PopUpHWND(HWND,bool,bool)")
REF("::Ctrl::PopUp(Ctrl*,bool,bool)")
REF("::Ctrl::IsWaitingEvent()")
REF("::Ctrl::ProcessEvent(bool*)")
REF("::Ctrl::ProcessEvents(bool*)")
REF("::Ctrl::IsPopUp()const")
REF("::Ctrl::EventLoop(Ctrl*)")
REF("::Ctrl::GetLoopLevel()")
REF("::Ctrl::GetLoopCtrl()")
REF("::Ctrl::EndLoop()")
REF("::Ctrl::EndLoop(int)")
REF("::Ctrl::InLoop()const")
REF("::Ctrl::GetExitCode()const")
REF("::Ctrl::SetMinSize(Size)")
REF("::Ctrl::Csizeinit()")
REF("::Ctrl::GetZoomText()")
REF("::Ctrl::SetZoomSize(Size,Size")
REF("::Ctrl::HorzLayoutZoom(int)")
REF("::Ctrl::VertLayoutZoom(int)")
REF("::Ctrl::LayoutZoom(int,int)")
REF("::Ctrl::LayoutZoom(Size)")
REF("::Ctrl::NoLayoutZoom()")
REF("::Ctrl::ClickFocus()")
REF("::Ctrl::ClickFocus(bool)")
REF("::Ctrl::GetWorkArea()")
REF("::Ctrl::GetKbdDelay()")
REF("::Ctrl::GetKbdSpeed()")
REF("::Ctrl::GetDefaultWindowRect()")
REF("::Ctrl::IsDragFullWindowMode()")
REF("::Ctrl::IsOpenEffectsMode()")
REF("::Ctrl::GetAppName()")
REF("::Ctrl::SetAppName(const String&)")
REF("::Ctrl::Name()const")
REF("::Ctrl::Dump()const")
REF("::Ctrl::Dump(Stream&)const")
REF("::Ctrl::InitWin32(HINSTANCE)")
REF("::Ctrl::InitX11(const char*)")
REF("::Ctrl::GuiFlush()")
REF("::Ctrl::StdGuiSleep(int)")
REF("::Ctrl::Callback WhenAction")
REF("::Ctrl::static HINSTANCE hInstance")
REF("::Ctrl::static HCURSOR hCursor")
REF("::Ctrl::static Atom XA_WM_PROTOCOLS")
REF("::Ctrl::static Atom XA_WM_DELETE_WINDOW")
REF("::Ctrl::static Atom XA_WM_TAKE_FOCUS")
REF("::Ctrl::static Atom XA_UPP_SYNC")
REF("::Ctrl::static Atom XA__NET_WM_CONTEXT_HELP")
REF("::Ctrl::static Atom XA__MOTIF_WM_HINTS")
REF("::Ctrl::static Atom XA_KWIN_RUNNING")
REF("::Ctrl::static Atom XA_KWM_RUNNING")
REF("::Ctrl::static Atom XA_GNOME_BACKGROUND_PROPERTIES")
REF("::Ctrl::static Atom XA__QT_SETTINGS_TIMESTAMP_")
REF("::Ctrl::static Atom XA__NET_SUPPORTED")
REF("::Ctrl::static Atom XA__NET_VIRTUAL_ROOTS")
REF("::Ctrl::static Atom XA__NET_WM_NAME")
REF("::Ctrl::static Atom XA__NET_WM_ICON_NAME")
REF("::Ctrl::static Atom XA__NET_WM_ICON")
REF("::Ctrl::static Atom XA__NET_WM_STATE")
REF("::Ctrl::static Atom XA__NET_WM_STATE_MODAL")
REF("::Ctrl::static Atom XA__NET_WM_STATE_MAXIMIZED_VERT")
REF("::Ctrl::static Atom XA__NET_WM_STATE_MAXIMIZED_HORZ")
REF("::Ctrl::static Atom XA__NET_WM_WINDOW_TYPE")
REF("::Ctrl::static Atom XA__NET_WM_WINDOW_TYPE_NORMAL")
REF("::Ctrl::static Atom XA__NET_WM_WINDOW_TYPE_DIALOG")
REF("::Ctrl::static Atom XA__NET_WM_WINDOW_TYPE_TOOLBAR")
REF("::Ctrl::static Atom XA__KDE_NET_WM_WINDOW_TYPE_OVERRIDE")
REF("::Ctrl::static Atom XA__KDE_NET_WM_FRAME_STRUT")
REF("::Ctrl::static Atom XA__NET_WM_STATE_STAYS_ON_TOP")
REF("::Ctrl::static Atom XA__NET_WM_MOVERESIZE")
REF("::Ctrl::static Atom XA__KDE_NET_USER_TIME")
REF("::Ctrl::static Atom XA_ENLIGHTENMENT_DESKTOP")
REF("::Ctrl::static Atom XA_WM_STATE")
REF("::Ctrl::static Atom XA_UTF8_STRING")
REF("::Ctrl::static Atom XA_CLIPBOARD")
REF("::Ctrl::static Atom XA_CLIPDATA")
REF("::Ctrl::static int Xeventtime")
REF("::Ctrl::static Size Dsize")
REF("::Ctrl::static Size Csize")
REF("::Ctrl::static int AutoBackPaintAreaSize")
REF("::Ctrl::static int TransparentBackPaintAreaSize")
REF("::Ctrl::static bool LogMessages")
REF("::Ctrl::static int ShowRepaint")
REF("::Ctrl::static bool MemoryCheck")
REF("::Ctrl::static void (*&GuiSleep())(int ms)")
TOPIC_TEXT(
"[ $$0,0#00000000000000000000000000000000:Default][t4167;C+117 $$1,0#37138531426314131251341829483380:class][l288;a17; "
"$$2,2#27521748481378242620020725143825:desc][ $$3,0#83433469410354161042741608181528:base][b167;a42;C "
"$$4,0#40027414424643823182269349404212:item][l288;i704;a17;O9;~~~.992; $$5,0#10431211400427159095818037425705:param][{_}%EN-US "
"[s1;:`:`:Ctrl`:`:class Ctrl:* [%00-00* class_][%00-00")
TOPIC_TEXT(
" Ctrl]&][s2; &][s3;*@(0.0.255) [%00-00/@0 Derived from][%00-00*@0  ][%00-00 Data][%00-00*@0 "
", ][%00-00 Pte<Ctrl> ]&][s4;:`:`:Ctrl`:`:enum PlacementConstants `{ CENTER`, LEFT`, "
"RIGHT`, SIZE`, MINSIZE`, MAXSIZE`, STDSIZE `}: [%00-00 enum_][* PlacementConstants][%00-00 "
"_`{_][%00-00*@3 CENTER][%00-00 , ][%00-00*@3 LEFT][%00-00 , ][%00-00*@3 RIGHT][%00-00 "
", ][%00-00*@3 SIZE][%00-00 , ][%00-00*@3 MINSIZ")
TOPIC_TEXT(
"E][%00-00 , ][%00-00*@3 MAXSIZE][%00-00 , ][%00-00*@3 STDSIZE][%00-00 _`}]&][s2; "
"&][s5; [%00-00*C@3 CENTER]-|&][s5; [%00-00*C@3 LEFT]-|&][s5; [%00-00*C@3 RIGHT]-|&][s5; "
"[%00-00*C@3 SIZE]-|&][s5; [%00-00*C@3 MINSIZE]-|&][s5; [%00-00*C@3 MAXSIZE]-|&][s5; "
"[%00-00*C@3 STDSIZE]-|&][s4;:`:`:Ctrl`:`:enum `{ TOP`, BOTTOM `}: [%00-00 enum_`{_][%00-00*@3 "
"TOP][%00-00 , ][%00-00*@3 BOTTOM][%00-00 _`}]&][s2; &")
TOPIC_TEXT(
"][s5; [%00-00*C@3 TOP]-|&][s5; [%00-00*C@3 BOTTOM]-|&][s4;:`:`:Ctrl`:`:enum StateReason "
"`{ FOCUS`, ACTIVATE`, DEACTIVATE`, SHOW`, ENABLE`, EDITABLE`, OPEN`, CLOSE `}: [%00-00 "
"enum_][%00-00* StateReason][%00-00 _`{_][%00-00*@3 FOCUS][%00-00 , ][%00-00*@3 ACTIVATE][%00-00 "
", ][%00-00*@3 DEACTIVATE][%00-00 , ][%00-00*@3 SHOW][%00-00 , ][%00-00*@3 ENABLE][%00-00 "
", ][%00-00*@3 EDITABLE][%00-00 , ][%00-0")
TOPIC_TEXT(
"0*@3 OPEN][%00-00 , ][%00-00*@3 CLOSE][%00-00 _`}]&][s2; &][s5; [%00-00*C@3 FOCUS]-|&][s5; "
"[%00-00*C@3 ACTIVATE]-|&][s5; [%00-00*C@3 DEACTIVATE]-|&][s5; [%00-00*C@3 SHOW]-|&][s5; "
"[%00-00*C@3 ENABLE]-|&][s5; [%00-00*C@3 EDITABLE]-|&][s5; [%00-00*C@3 OPEN]-|&][s5; "
"[%00-00*C@3 CLOSE]-|&][s4;:`:`:Ctrl`:`:enum MouseEvents `{ BUTTON`, ACTION`, MOUSEENTER`, "
"MOUSEMOVE`, MOUSELEAVE`, CURSORIMAGE`, MOUSEWHE")
TOPIC_TEXT(
"EL`, DOWN`, UP`, DOUBLE`, REPEAT`, LEFTDOWN`, LEFTDOUBLE`, LEFTREPEAT`, LEFTUP`, "
"RIGHTDOWN`, RIGHTDOUBLE`, RIGHTREPEAT`, RIGHTUP `}: [%00-00 enum_][%00-00* MouseEvents][%00-00 "
"_`{_][%00-00*@3 BUTTON][%00-00 , ][%00-00*@3 ACTION][%00-00 , ][%00-00*@3 MOUSEENTER][%00-00 "
", ][%00-00*@3 MOUSEMOVE][%00-00 , ][%00-00*@3 MOUSELEAVE][%00-00 , ][%00-00*@3 CURSORIMAGE][%00-00 "
", ][%00-00*@3 MOUSEWHEEL][%00-00")
TOPIC_TEXT(
" , ][%00-00*@3 DOWN][%00-00 , ][%00-00*@3 UP][%00-00 , ][%00-00*@3 DOUBLE][%00-00 "
", ][%00-00*@3 REPEAT][%00-00 , ][%00-00*@3 LEFTDOWN][%00-00 , ][%00-00*@3 LEFTDOUBLE][%00-00 "
", ][%00-00*@3 LEFTREPEAT][%00-00 , ][%00-00*@3 LEFTUP][%00-00 , ][%00-00*@3 RIGHTDOWN][%00-00 "
", ][%00-00*@3 RIGHTDOUBLE][%00-00 , ][%00-00*@3 RIGHTREPEAT][%00-00 , ][%00-00*@3 "
"RIGHTUP][%00-00 _`}]&][s2; &][s5; [%00-00*C@3 BUT")
TOPIC_TEXT(
"TON]-|&][s5; [%00-00*C@3 ACTION]-|&][s5; [%00-00*C@3 MOUSEENTER]-|&][s5; [%00-00*C@3 "
"MOUSEMOVE]-|&][s5; [%00-00*C@3 MOUSELEAVE]-|&][s5; [%00-00*C@3 CURSORIMAGE]-|&][s5; "
"[%00-00*C@3 MOUSEWHEEL]-|&][s5; [%00-00*C@3 DOWN]-|&][s5; [%00-00*C@3 UP]-|&][s5; "
"[%00-00*C@3 DOUBLE]-|&][s5; [%00-00*C@3 REPEAT]-|&][s5; [%00-00*C@3 LEFTDOWN]-|&][s5; "
"[%00-00*C@3 LEFTDOUBLE]-|&][s5; [%00-00*C@3 LEFTREPEAT]-|&][s5;")
TOPIC_TEXT(
" [%00-00*C@3 LEFTUP]-|&][s5; [%00-00*C@3 RIGHTDOWN]-|&][s5; [%00-00*C@3 RIGHTDOUBLE]-|&][s5; "
"[%00-00*C@3 RIGHTREPEAT]-|&][s5; [%00-00*C@3 RIGHTUP]-|&][s4;:`:`:Ctrl`:`:enum `{ "
"NOBACKPAINT`, FULLBACKPAINT`, TRANSPARENTBACKPAINT`, EXCLUDEPAINT `}: [%00-00 enum_`{_][%00-00*@3 "
"NOBACKPAINT][%00-00 , ][%00-00*@3 FULLBACKPAINT][%00-00 , ][%00-00*@3 TRANSPARENTBACKPAINT][%00-00 "
", ][%00-00*@3 EXCLUDEPAINT][")
TOPIC_TEXT(
"%00-00 _`}]&][s2; &][s5; [%00-00*C@3 NOBACKPAINT]-|&][s5; [%00-00*C@3 FULLBACKPAINT]-|&][s5; "
"[%00-00*C@3 TRANSPARENTBACKPAINT]-|&][s5; [%00-00*C@3 EXCLUDEPAINT]-|&][s4;:`:`:Ctrl`:`:typedef "
"bool `(`*MouseHook`)`(Ctrl `*ctrl`, bool inframe`, int event`, Point p`, int zdelta`, "
"dword keyflags`): [%00-00 typedef_bool_(`*][%00-00* MouseHook][%00-00 )(Ctrl_`*ctrl, "
"bool_inframe, int_event, Point_p, int_zd")
TOPIC_TEXT(
"elta, dword_keyflags)]&][s2; &][s4;:`:`:Ctrl`:`:typedef bool `(`*KeyHook`)`(Ctrl "
"`*ctrl`, dword key`, int count`): [%00-00 typedef_bool_(`*][%00-00* KeyHook][%00-00 "
")(Ctrl_`*ctrl, dword_key, int_count)]&][s2; &][s4;:`:`:Ctrl`:`:typedef bool `(`*StateHook`)`(Ctrl "
"`*ctrl`, int reason`): [%00-00 typedef_bool_(`*][%00-00* StateHook][%00-00 )(Ctrl_`*ctrl, "
"int_reason)]&][s2; &][s4;:`:`:Ctrl`:`:Ctrl`(`):")
TOPIC_TEXT(
" [%00-00* Ctrl][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:`~Ctrl`(`): [%00-00 `~][%00-00* "
"Ctrl][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:Accept`(`): [%00-00 virtual bool_][%00-00* "
"Accept][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:Reject`(`): "
"[%00-00 virtual void_][%00-00* Reject][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:Serialize`(Stream`&`): "
"[%00-00 virtual void_][%00-00* Serialize][%00-00 (S")
TOPIC_TEXT(
"tream`&_][%00-00*@3 s][%00-00 )]&][s2; &][s5; [%00-00*C@3 s]-|&][s4;:`:`:Ctrl`:`:SetModify`(`): "
"[%00-00 virtual void_][%00-00* SetModify][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:ClearModify`(`): "
"[%00-00 virtual void_][%00-00* ClearModify][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:IsModified`(`)const: "
"[%00-00 virtual bool_][%00-00* IsModified][%00-00 ()_const]&][s2; &][s5; [*/ Return "
"value]-|&][s4;:`:`:Ctrl`")
TOPIC_TEXT(
":`:PosLeft`(int`,int`): [%00-00 static Logc_][%00-00* PosLeft][%00-00 (int_][%00-00*@3 "
"pos][%00-00 , int_][%00-00*@3 size][%00-00 )]&][s2; &][s5; [%00-00*C@3 pos]-|&][s5; "
"[%00-00*C@3 size]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:PosRight`(int`,int`): "
"[%00-00 static Logc_][%00-00* PosRight][%00-00 (int_][%00-00*@3 pos][%00-00 , int_][%00-00*@3 "
"size][%00-00 )]&][s2; &][s5; [%00-00*C@3 pos]-|&]")
TOPIC_TEXT(
"[s5; [%00-00*C@3 size]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:PosTop`(int`,int`): "
"[%00-00 static Logc_][%00-00* PosTop][%00-00 (int_][%00-00*@3 pos][%00-00 , int_][%00-00*@3 "
"size][%00-00 )]&][s2; &][s5; [%00-00*C@3 pos]-|&][s5; [%00-00*C@3 size]-|&][s5; [*/ "
"Return value]-|&][s4;:`:`:Ctrl`:`:PosBottom`(int`,int`): [%00-00 static Logc_][%00-00* "
"PosBottom][%00-00 (int_][%00-00*@3 pos][%00-00 ,")
TOPIC_TEXT(
" int_][%00-00*@3 size][%00-00 )]&][s2; &][s5; [%00-00*C@3 pos]-|&][s5; [%00-00*C@3 "
"size]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:PosSize`(int`,int`): [%00-00 "
"static Logc_][%00-00* PosSize][%00-00 (int_][%00-00*@3 lpos][%00-00 , int_][%00-00*@3 "
"rpos][%00-00 )]&][s2; &][s5; [%00-00*C@3 lpos]-|&][s5; [%00-00*C@3 rpos]-|&][s5; "
"[*/ Return value]-|&][s4;:`:`:Ctrl`:`:PosCenter`(int`,int`): [%00-00 ")
TOPIC_TEXT(
"static Logc_][%00-00* PosCenter][%00-00 (int_][%00-00*@3 size][%00-00 , int_][%00-00*@3 "
"offset][%00-00 )]&][s2; &][s5; [%00-00*C@3 size]-|&][s5; [%00-00*C@3 offset]-|&][s5; "
"[*/ Return value]-|&][s4;:`:`:Ctrl`:`:PosCenter`(int`): [%00-00 static Logc_][%00-00* "
"PosCenter][%00-00 (int_][%00-00*@3 size][%00-00 )]&][s2; &][s5; [%00-00*C@3 size]-|&][s5; "
"[*/ Return value]-|&][s4;:`:`:Ctrl`:`:KEYtoK`(dword")
TOPIC_TEXT(
"`): [%00-00 static dword_][%00-00* KEYtoK][%00-00 (dword)]&][s2; &][s5; [*/ Return "
"value]-|&][s4;:`:`:Ctrl`:`:Windows`(`): [%00-00 static VectorMap<_HWND, Ptr<Ctrl>_>`&_][%00-00* "
"Windows][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:MakeWin32Happy`(`): "
"[%00-00 static void_][%00-00* MakeWin32Happy][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:WndProc`(HWND`,UINT`,WPARAM`,LPARAM`): "
"[%00-00 s")
TOPIC_TEXT(
"tatic LRESULT_CALLBACK_][%00-00* WndProc][%00-00 (HWND_][%00-00*@3 hWnd][%00-00 "
", UINT_][%00-00*@3 message][%00-00 , WPARAM_][%00-00*@3 wParam][%00-00 , LPARAM_][%00-00*@3 "
"lParam][%00-00 )]&][s2; &][s5; [%00-00*C@3 hWnd]-|&][s5; [%00-00*C@3 message]-|&][s5; "
"[%00-00*C@3 wParam]-|&][s5; [%00-00*C@3 lParam]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetScreenClient`(HWND`): "
"[%00-00 static Rect_][%")
TOPIC_TEXT(
"00-00* GetScreenClient][%00-00 (HWND_][%00-00*@3 hwnd][%00-00 )]&][s2; &][s5; [%00-00*C@3 "
"hwnd]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:Create`(HWND`,DWORD`,DWORD`,bool`,int`): "
"[%00-00 void_][%00-00* Create][%00-00 (HWND_][%00-00*@3 parent][%00-00 , DWORD_][%00-00*@3 "
"style][%00-00 , DWORD_][%00-00*@3 exstyle][%00-00 , bool_][%00-00*@3 savebits][%00-00 "
", int_][%00-00*@3 show][%00-00 _`=_SW`_S")
TOPIC_TEXT(
"HOW)]&][s2; &][s5; [%00-00*C@3 parent]-|&][s5; [%00-00*C@3 style]-|&][s5; [%00-00*C@3 "
"exstyle]-|&][s5; [%00-00*C@3 savebits]-|&][s5; [%00-00*C@3 show]-|&][s4;:`:`:Ctrl`:`:DoMouse`(int`,Point`,int`): "
"[%00-00 Image_][%00-00* DoMouse][%00-00 (int_][%00-00*@3 e][%00-00 , Point_][%00-00*@3 "
"p][%00-00 , int_][%00-00*@3 zd][%00-00 _`=_][%00-00@3 0][%00-00 )]&][s2; &][s5; [%00-00*C@3 "
"e]-|&][s5; [%00-00*C@3")
TOPIC_TEXT(
" p]-|&][s5; [%00-00*C@3 zd]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:PreDestroy`(`): "
"[%00-00 virtual void_][%00-00* PreDestroy][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:WindowProc`(UINT`,WPARAM`,LPARAM`): "
"[%00-00 virtual LRESULT_][%00-00* WindowProc][%00-00 (UINT_][%00-00*@3 message][%00-00 "
", WPARAM_][%00-00*@3 wParam][%00-00 , LPARAM_][%00-00*@3 lParam][%00-00 )]&][s2; "
"&][s5; [%00-00*C@3 message]")
TOPIC_TEXT(
"-|&][s5; [%00-00*C@3 wParam]-|&][s5; [%00-00*C@3 lParam]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetHWND`(`)const: "
"[%00-00 HWND_][%00-00* GetHWND][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetOwnerHWND`(`)const: "
"[%00-00 HWND_][%00-00* GetOwnerHWND][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:CtrlFromHWND`(HWND`): "
"[%00-00 static Ctrl_`*][%00-0")
TOPIC_TEXT(
"0* CtrlFromHWND][%00-00 (HWND_][%00-00*@3 hwnd][%00-00 )]&][s2; &][s5; [%00-00*C@3 "
"hwnd]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:Create`(Ctrl`*`,bool`,bool`): "
"[%00-00 void_][%00-00* Create][%00-00 (Ctrl_`*][%00-00*@3 owner][%00-00 , bool_][%00-00*@3 "
"redirect][%00-00 , bool_][%00-00*@3 savebits][%00-00 )]&][s2; &][s5; [%00-00*C@3 "
"owner]-|&][s5; [%00-00*C@3 redirect]-|&][s5; [%00-00*C@3 savebi")
TOPIC_TEXT(
"ts]-|&][s4;:`:`:Ctrl`:`:SyncEvents`(Window`,int`): [%00-00 static void_][%00-00* "
"SyncEvents][%00-00 (Window_][%00-00*@3 window][%00-00 , int_][%00-00*@3 event][%00-00 "
")]&][s2; &][s5; [%00-00*C@3 window]-|&][s5; [%00-00*C@3 event]-|&][s4;:`:`:Ctrl`:`:SyncEvents`(int`): "
"[%00-00 void_][%00-00* SyncEvents][%00-00 (int_][%00-00*@3 event][%00-00 )]&][s2; "
"&][s5; [%00-00*C@3 event]-|&][s4;:`:`:Ctrl`:`:Eve")
TOPIC_TEXT(
"ntProc`(XWindow`&`,XEvent`*`): [%00-00 virtual void_][%00-00* EventProc][%00-00 "
"(XWindow`&_][%00-00*@3 w][%00-00 , XEvent_`*][%00-00*@3 event][%00-00 )]&][s2; &][s5; "
"[%00-00*C@3 w]-|&][s5; [%00-00*C@3 event]-|&][s4;:`:`:Ctrl`:`:GetWindow`(`)const: "
"[%00-00 Window_][%00-00* GetWindow][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:CtrlFromWindow`(Window`): "
"[%00-00 static Ctrl_`*")
TOPIC_TEXT(
"][%00-00* CtrlFromWindow][%00-00 (Window_][%00-00*@3 w][%00-00 )]&][s2; &][s5; [%00-00*C@3 "
"w]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:DoRemove`(`): [%00-00 void_][%00-00* "
"DoRemove][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:GetSysWindowViewOffset`(`): [%00-00 "
"virtual Point_][%00-00* GetSysWindowViewOffset][%00-00 ()]&][s2; &][s5; [*/ Return "
"value]-|&][s4;:`:`:Ctrl`:`:Unicode`(`): [%00-00 Ctrl`&_][%")
TOPIC_TEXT(
"00-00* Unicode][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetTopCtrls`(`): "
"[%00-00 static Vector<Ctrl_`*>_][%00-00* GetTopCtrls][%00-00 ()]&][s2; &][s5; [*/ "
"Return value]-|&][s4;:`:`:Ctrl`:`:GetTopWindows`(`): [%00-00 static Vector<Ctrl_`*>_][%00-00* "
"GetTopWindows][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:CloseTopCtrls`(`): "
"[%00-00 static void_][%00-00* Clo")
TOPIC_TEXT(
"seTopCtrls][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:InstallMouseHook`(MouseHook`): [%00-00 "
"static void_][%00-00* InstallMouseHook][%00-00 (MouseHook_][%00-00*@3 hook][%00-00 "
")]&][s2; &][s5; [%00-00*C@3 hook]-|&][s4;:`:`:Ctrl`:`:DeinstallMouseHook`(MouseHook`): "
"[%00-00 static void_][%00-00* DeinstallMouseHook][%00-00 (MouseHook_][%00-00*@3 hook][%00-00 "
")]&][s2; &][s5; [%00-00*C@3 hook]-|&][s4;:`:`:Ctrl")
TOPIC_TEXT(
"`:`:InstallKeyHook`(KeyHook`): [%00-00 static void_][%00-00* InstallKeyHook][%00-00 "
"(KeyHook_][%00-00*@3 hook][%00-00 )]&][s2; &][s5; [%00-00*C@3 hook]-|&][s4;:`:`:Ctrl`:`:DeinstallKeyHook`(KeyHook`): "
"[%00-00 static void_][%00-00* DeinstallKeyHook][%00-00 (KeyHook_][%00-00*@3 hook][%00-00 "
")]&][s2; &][s5; [%00-00*C@3 hook]-|&][s4;:`:`:Ctrl`:`:InstallStateHook`(StateHook`): "
"[%00-00 static void_][%00")
TOPIC_TEXT(
"-00* InstallStateHook][%00-00 (StateHook_][%00-00*@3 hook][%00-00 )]&][s2; &][s5; "
"[%00-00*C@3 hook]-|&][s4;:`:`:Ctrl`:`:DeinstallStateHook`(StateHook`): [%00-00 static "
"void_][%00-00* DeinstallStateHook][%00-00 (StateHook_][%00-00*@3 hook][%00-00 )]&][s2; "
"&][s5; [%00-00*C@3 hook]-|&][s4;:`:`:Ctrl`:`:Paint`(Draw`&`): [%00-00 virtual void_][%00-00* "
"Paint][%00-00 (Draw`&_][%00-00*@3 draw][%00-00 )]&][")
TOPIC_TEXT(
"s2; &][s5; [%00-00*C@3 draw]-|&][s4;:`:`:Ctrl`:`:CancelMode`(`): [%00-00 virtual "
"void_][%00-00* CancelMode][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:Activate`(`): [%00-00 "
"virtual void_][%00-00* Activate][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:Deactivate`(`): "
"[%00-00 virtual void_][%00-00* Deactivate][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:FrameMouseEvent`(int`,Point`,int`,dword`): "
"[%00-00 virtual Image_][%00")
TOPIC_TEXT(
"-00* FrameMouseEvent][%00-00 (int_][%00-00*@3 event][%00-00 , Point_][%00-00*@3 "
"p][%00-00 , int_][%00-00*@3 zdelta][%00-00 , dword_][%00-00*@3 keyflags][%00-00 )]&][s2; "
"&][s5; [%00-00*C@3 event]-|&][s5; [%00-00*C@3 p]-|&][s5; [%00-00*C@3 zdelta]-|&][s5; "
"[%00-00*C@3 keyflags]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:MouseEvent`(int`,Point`,int`,dword`): "
"[%00-00 virtual Image_][%00-00* MouseEve")
TOPIC_TEXT(
"nt][%00-00 (int_][%00-00*@3 event][%00-00 , Point_][%00-00*@3 p][%00-00 , int_][%00-00*@3 "
"zdelta][%00-00 , dword_][%00-00*@3 keyflags][%00-00 )]&][s2; &][s5; [%00-00*C@3 event]-|&][s5; "
"[%00-00*C@3 p]-|&][s5; [%00-00*C@3 zdelta]-|&][s5; [%00-00*C@3 keyflags]-|&][s5; "
"[*/ Return value]-|&][s4;:`:`:Ctrl`:`:MouseEnter`(Point`,dword`): [%00-00 virtual "
"void_][%00-00* MouseEnter][%00-00 (Point_][%00-00*@3")
TOPIC_TEXT(
" p][%00-00 , dword_][%00-00*@3 keyflags][%00-00 )]&][s2; &][s5; [%00-00*C@3 p]-|&][s5; "
"[%00-00*C@3 keyflags]-|&][s4;:`:`:Ctrl`:`:MouseMove`(Point`,dword`): [%00-00 virtual "
"void_][%00-00* MouseMove][%00-00 (Point_][%00-00*@3 p][%00-00 , dword_][%00-00*@3 "
"keyflags][%00-00 )]&][s2; &][s5; [%00-00*C@3 p]-|&][s5; [%00-00*C@3 keyflags]-|&][s4;:`:`:Ctrl`:`:LeftDown`(Point`,dword`): "
"[%00-00 virtual void_]")
TOPIC_TEXT(
"[%00-00* LeftDown][%00-00 (Point_][%00-00*@3 p][%00-00 , dword_][%00-00*@3 keyflags][%00-00 "
")]&][s2; &][s5; [%00-00*C@3 p]-|&][s5; [%00-00*C@3 keyflags]-|&][s4;:`:`:Ctrl`:`:LeftDouble`(Point`,dword`): "
"[%00-00 virtual void_][%00-00* LeftDouble][%00-00 (Point_][%00-00*@3 p][%00-00 , "
"dword_][%00-00*@3 keyflags][%00-00 )]&][s2; &][s5; [%00-00*C@3 p]-|&][s5; [%00-00*C@3 "
"keyflags]-|&][s4;:`:`:Ctrl`:`:Le")
TOPIC_TEXT(
"ftRepeat`(Point`,dword`): [%00-00 virtual void_][%00-00* LeftRepeat][%00-00 (Point_][%00-00*@3 "
"p][%00-00 , dword_][%00-00*@3 keyflags][%00-00 )]&][s2; &][s5; [%00-00*C@3 p]-|&][s5; "
"[%00-00*C@3 keyflags]-|&][s4;:`:`:Ctrl`:`:LeftUp`(Point`,dword`): [%00-00 virtual "
"void_][%00-00* LeftUp][%00-00 (Point_][%00-00*@3 p][%00-00 , dword_][%00-00*@3 keyflags][%00-00 "
")]&][s2; &][s5; [%00-00*C@3 p]-|&][s5; [%")
TOPIC_TEXT(
"00-00*C@3 keyflags]-|&][s4;:`:`:Ctrl`:`:RightDown`(Point`,dword`): [%00-00 virtual "
"void_][%00-00* RightDown][%00-00 (Point_][%00-00*@3 p][%00-00 , dword_][%00-00*@3 "
"keyflags][%00-00 )]&][s2; &][s5; [%00-00*C@3 p]-|&][s5; [%00-00*C@3 keyflags]-|&][s4;:`:`:Ctrl`:`:RightDouble`(Point`,dword`): "
"[%00-00 virtual void_][%00-00* RightDouble][%00-00 (Point_][%00-00*@3 p][%00-00 , "
"dword_][%00-00*@3 keyflags")
TOPIC_TEXT(
"][%00-00 )]&][s2; &][s5; [%00-00*C@3 p]-|&][s5; [%00-00*C@3 keyflags]-|&][s4;:`:`:Ctrl`:`:RightRepeat`(Point`,dword`): "
"[%00-00 virtual void_][%00-00* RightRepeat][%00-00 (Point_][%00-00*@3 p][%00-00 , "
"dword_][%00-00*@3 keyflags][%00-00 )]&][s2; &][s5; [%00-00*C@3 p]-|&][s5; [%00-00*C@3 "
"keyflags]-|&][s4;:`:`:Ctrl`:`:RightUp`(Point`,dword`): [%00-00 virtual void_][%00-00* "
"RightUp][%00-00 (Point_][%0")
TOPIC_TEXT(
"0-00*@3 p][%00-00 , dword_][%00-00*@3 keyflags][%00-00 )]&][s2; &][s5; [%00-00*C@3 "
"p]-|&][s5; [%00-00*C@3 keyflags]-|&][s4;:`:`:Ctrl`:`:MouseWheel`(Point`,int`,dword`): "
"[%00-00 virtual void_][%00-00* MouseWheel][%00-00 (Point_][%00-00*@3 p][%00-00 , "
"int_][%00-00*@3 zdelta][%00-00 , dword_][%00-00*@3 keyflags][%00-00 )]&][s2; &][s5; "
"[%00-00*C@3 p]-|&][s5; [%00-00*C@3 zdelta]-|&][s5; [%00-00*C@3 key")
TOPIC_TEXT(
"flags]-|&][s4;:`:`:Ctrl`:`:MouseLeave`(`): [%00-00 virtual void_][%00-00* MouseLeave][%00-00 "
"()]&][s2; &][s4;:`:`:Ctrl`:`:CursorImage`(Point`,dword`): [%00-00 virtual Image_][%00-00* "
"CursorImage][%00-00 (Point_][%00-00*@3 p][%00-00 , dword_][%00-00*@3 keyflags][%00-00 "
")]&][s2; &][s5; [%00-00*C@3 p]-|&][s5; [%00-00*C@3 keyflags]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:Key`(dword`,int`): "
"[%00-")
TOPIC_TEXT(
"00 virtual bool_][%00-00* Key][%00-00 (dword_][%00-00*@3 key][%00-00 , int_][%00-00*@3 "
"count][%00-00 )]&][s2; &][s5; [%00-00*C@3 key]-|&][s5; [%00-00*C@3 count]-|&][s5; "
"[*/ Return value]-|&][s4;:`:`:Ctrl`:`:WKey`(dword`,int`): [%00-00 virtual bool_][%00-00* "
"WKey][%00-00 (dword_][%00-00*@3 key][%00-00 , int_][%00-00*@3 count][%00-00 )]&][s2; "
"&][s5; [%00-00*C@3 key]-|&][s5; [%00-00*C@3 count]-|&][s5")
TOPIC_TEXT(
"; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GotFocus`(`): [%00-00 virtual void_][%00-00* "
"GotFocus][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:LostFocus`(`): [%00-00 virtual void_][%00-00* "
"LostFocus][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:HotKey`(dword`): [%00-00 virtual bool_][%00-00* "
"HotKey][%00-00 (dword_][%00-00*@3 key][%00-00 )]&][s2; &][s5; [%00-00*C@3 key]-|&][s5; "
"[*/ Return value]-|&][s4;:`:`:Ctrl`:`:Pos")
TOPIC_TEXT(
"tInput`(`): [%00-00 virtual void_][%00-00* PostInput][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:ChildGotFocus`(`): "
"[%00-00 virtual void_][%00-00* ChildGotFocus][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:ChildLostFocus`(`): "
"[%00-00 virtual void_][%00-00* ChildLostFocus][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:ChildAdded`(Ctrl`*`): "
"[%00-00 virtual void_][%00-00* ChildAdded][%00-00 (Ctrl_`*][%00-00*@3 child][%00-00 ")
TOPIC_TEXT(
")]&][s2; &][s5; [%00-00*C@3 child]-|&][s4;:`:`:Ctrl`:`:ChildRemoved`(Ctrl`*`): [%00-00 "
"virtual void_][%00-00* ChildRemoved][%00-00 (Ctrl_`*][%00-00*@3 child][%00-00 )]&][s2; "
"&][s5; [%00-00*C@3 child]-|&][s4;:`:`:Ctrl`:`:ParentChange`(`): [%00-00 virtual void_][%00-00* "
"ParentChange][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:State`(int`): [%00-00 virtual void_][%00-00* "
"State][%00-00 (int_][%00-00*@3 reaso")
TOPIC_TEXT(
"n][%00-00 )]&][s2; &][s5; [%00-00*C@3 reason]-|&][s4;:`:`:Ctrl`:`:Layout`(`): [%00-00 "
"virtual void_][%00-00* Layout][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:GetMinSize`(`)const: "
"[%00-00 virtual Size_][%00-00* GetMinSize][%00-00 ()_const]&][s2; &][s5; [*/ Return "
"value]-|&][s4;:`:`:Ctrl`:`:GetStdSize`(`)const: [%00-00 virtual Size_][%00-00* GetStdSize][%00-00 "
"()_const]&][s2; &][s5; [*/ Return value]-|&]")
TOPIC_TEXT(
"[s4;:`:`:Ctrl`:`:GetMaxSize`(`)const: [%00-00 virtual Size_][%00-00* GetMaxSize][%00-00 "
"()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:IsShowEnabled`(`)const: "
"[%00-00 virtual bool_][%00-00* IsShowEnabled][%00-00 ()_const]&][s2; &][s5; [*/ Return "
"value]-|&][s4;:`:`:Ctrl`:`:SetLabelText`(const char`*`): [%00-00 virtual void_][%00-00* "
"SetLabelText][%00-00 (const_char_`*][%00-00*@3 text]")
TOPIC_TEXT(
"[%00-00 )]&][s2; &][s5; [%00-00*C@3 text]-|&][s4;:`:`:Ctrl`:`:GetLabelText`(`)const: "
"[%00-00 virtual String_][%00-00* GetLabelText][%00-00 ()_const]&][s2; &][s5; [*/ "
"Return value]-|&][s4;:`:`:Ctrl`:`:Updated`(`): [%00-00 virtual void_][%00-00* Updated][%00-00 "
"()]&][s2; &][s4;:`:`:Ctrl`:`:Close`(`): [%00-00 virtual void_][%00-00* Close][%00-00 "
"()]&][s2; &][s4;:`:`:Ctrl`:`:IsOcxChild`(`): [%00-00 vi")
TOPIC_TEXT(
"rtual bool_][%00-00* IsOcxChild][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:AddChild`(Ctrl`*`): "
"[%00-00 void_][%00-00* AddChild][%00-00 (Ctrl_`*][%00-00*@3 child][%00-00 )]&][s2; "
"&][s5; [%00-00*C@3 child]-|&][s4;:`:`:Ctrl`:`:AddChild`(Ctrl`*`,Ctrl`*`): [%00-00 "
"void_][%00-00* AddChild][%00-00 (Ctrl_`*][%00-00*@3 child][%00-00 , Ctrl_`*][%00-00*@3 "
"insafter][%00-00 )]&][s2; &][s5; ")
TOPIC_TEXT(
"[%00-00*C@3 child]-|&][s5; [%00-00*C@3 insafter]-|&][s4;:`:`:Ctrl`:`:AddChildBefore`(Ctrl`*`,Ctrl`*`): "
"[%00-00 void_][%00-00* AddChildBefore][%00-00 (Ctrl_`*][%00-00*@3 child][%00-00 , "
"Ctrl_`*][%00-00*@3 insbefore][%00-00 )]&][s2; &][s5; [%00-00*C@3 child]-|&][s5; [%00-00*C@3 "
"insbefore]-|&][s4;:`:`:Ctrl`:`:RemoveChild`(Ctrl`*`): [%00-00 void_][%00-00* RemoveChild][%00-00 "
"(Ctrl_`*][%00-00*@3 child]")
TOPIC_TEXT(
"[%00-00 )]&][s2; &][s5; [%00-00*C@3 child]-|&][s4;:`:`:Ctrl`:`:GetParent`(`)const: "
"[%00-00 Ctrl_`*][%00-00* GetParent][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetLastChild`(`)const: "
"[%00-00 Ctrl_`*][%00-00* GetLastChild][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetFirstChild`(`)const: "
"[%00-00 Ctrl_`*][%00-00* GetFirstChild][%00-00 ()_const]&][")
TOPIC_TEXT(
"s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetPrev`(`)const: [%00-00 Ctrl_`*][%00-00* "
"GetPrev][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetNext`(`)const: "
"[%00-00 Ctrl_`*][%00-00* GetNext][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:IsChild`(`)const: "
"[%00-00 bool_][%00-00* IsChild][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:C")
TOPIC_TEXT(
"trl`:`:ChildFromPoint`(Point`&`)const: [%00-00 Ctrl_`*][%00-00* ChildFromPoint][%00-00 "
"(Point`&_][%00-00*@3 pt][%00-00 )_const]&][s2; &][s5; [%00-00*C@3 pt]-|&][s5; [*/ "
"Return value]-|&][s4;:`:`:Ctrl`:`:IsForeground`(`)const: [%00-00 bool_][%00-00* IsForeground][%00-00 "
"()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:SetForeground`(`): [%00-00 "
"void_][%00-00* SetForeground][%00-00 ()]&]")
TOPIC_TEXT(
"[s2; &][s4;:`:`:Ctrl`:`:GetTopCtrl`(`)const: [%00-00 const_Ctrl_`*][%00-00* GetTopCtrl][%00-00 "
"()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetTopCtrl`(`): [%00-00 "
"Ctrl_`*][%00-00* GetTopCtrl][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetOwner`(`)const: "
"[%00-00 const_Ctrl_`*][%00-00* GetOwner][%00-00 ()_const]&][s2; &][s5; [*/ Return "
"value]-|&][s4;:`:`:Ctrl`:`:G")
TOPIC_TEXT(
"etOwner`(`): [%00-00 Ctrl_`*][%00-00* GetOwner][%00-00 ()]&][s2; &][s5; [*/ Return "
"value]-|&][s4;:`:`:Ctrl`:`:GetTopCtrlOwner`(`)const: [%00-00 const_Ctrl_`*][%00-00* "
"GetTopCtrlOwner][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetTopCtrlOwner`(`): "
"[%00-00 Ctrl_`*][%00-00* GetTopCtrlOwner][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetTopWindow`(`)const: ")
TOPIC_TEXT(
"[%00-00 const_TopWindow_`*][%00-00* GetTopWindow][%00-00 ()_const]&][s2; &][s5; "
"[*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetTopWindow`(`): [%00-00 TopWindow_`*][%00-00* "
"GetTopWindow][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:SetFrame`(int`,CtrlFrame`&`): "
"[%00-00 Ctrl`&_][%00-00* SetFrame][%00-00 (int_][%00-00*@3 i][%00-00 , CtrlFrame`&_][%00-00*@3 "
"frm][%00-00 )]&][s2; &][s5; [%00-")
TOPIC_TEXT(
"00*C@3 i]-|&][s5; [%00-00*C@3 frm]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:SetFrame`(CtrlFrame`&`): "
"[%00-00 Ctrl`&_][%00-00* SetFrame][%00-00 (CtrlFrame`&_][%00-00*@3 frm][%00-00 )]&][s2; "
"&][s5; [%00-00*C@3 frm]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:AddFrame`(CtrlFrame`&`): "
"[%00-00 Ctrl`&_][%00-00* AddFrame][%00-00 (CtrlFrame`&_][%00-00*@3 frm][%00-00 )]&][s2; "
"&][s5; [%00-00*C@3 frm]")
TOPIC_TEXT(
"-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetFrame`(int`)const: [%00-00 const_CtrlFrame`&_][%00-00* "
"GetFrame][%00-00 (int_][%00-00*@3 i][%00-00 _`=_][%00-00@3 0][%00-00 )_const]&][s2; "
"&][s5; [%00-00*C@3 i]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetFrame`(int`): "
"[%00-00 CtrlFrame`&_][%00-00* GetFrame][%00-00 (int_][%00-00*@3 i][%00-00 _`=_][%00-00@3 "
"0][%00-00 )]&][s2; &][s5; [%00-00*C@3")
TOPIC_TEXT(
" i]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:RemoveFrame`(int`): [%00-00 void_][%00-00* "
"RemoveFrame][%00-00 (int_][%00-00*@3 i][%00-00 )]&][s2; &][s5; [%00-00*C@3 i]-|&][s4;:`:`:Ctrl`:`:RemoveFrame`(CtrlFrame`&`): "
"[%00-00 void_][%00-00* RemoveFrame][%00-00 (CtrlFrame`&_][%00-00*@3 frm][%00-00 )]&][s2; "
"&][s5; [%00-00*C@3 frm]-|&][s4;:`:`:Ctrl`:`:InsertFrame`(int`,CtrlFrame`&`): [%00-00 "
"void_][")
TOPIC_TEXT(
"%00-00* InsertFrame][%00-00 (int_][%00-00*@3 i][%00-00 , CtrlFrame`&_][%00-00*@3 "
"frm][%00-00 )]&][s2; &][s5; [%00-00*C@3 i]-|&][s5; [%00-00*C@3 frm]-|&][s4;:`:`:Ctrl`:`:FindFrame`(CtrlFrame`&`): "
"[%00-00 int_][%00-00* FindFrame][%00-00 (CtrlFrame`&_][%00-00*@3 frm][%00-00 )]&][s2; "
"&][s5; [%00-00*C@3 frm]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetFrameCount`(`)const: "
"[%00-00 int_][%00-00* Get")
TOPIC_TEXT(
"FrameCount][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:ClearFrames`(`): "
"[%00-00 void_][%00-00* ClearFrames][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:IsOpen`(`)const: "
"[%00-00 bool_][%00-00* IsOpen][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:Shutdown`(`): "
"[%00-00 void_][%00-00* Shutdown][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:IsShutdown`(`)const: "
"[%00-00 boo")
TOPIC_TEXT(
"l_][%00-00* IsShutdown][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:SetPos`(LogPos`,bool`): "
"[%00-00 Ctrl`&_][%00-00* SetPos][%00-00 (LogPos_][%00-00*@3 p][%00-00 , bool_][%00-00*@3 "
"inframe][%00-00 )]&][s2; &][s5; [%00-00*C@3 p]-|&][s5; [%00-00*C@3 inframe]-|&][s5; "
"[*/ Return value]-|&][s4;:`:`:Ctrl`:`:SetPos`(LogPos`): [%00-00 Ctrl`&_][%00-00* "
"SetPos][%00-00 (LogPos_][%00-0")
TOPIC_TEXT(
"0*@3 p][%00-00 )]&][s2; &][s5; [%00-00*C@3 p]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:SetPos`(Logc`,Logc`): "
"[%00-00 Ctrl`&_][%00-00* SetPos][%00-00 (Logc_][%00-00*@3 x][%00-00 , Logc_][%00-00*@3 "
"y][%00-00 )]&][s2; &][s5; [%00-00*C@3 x]-|&][s5; [%00-00*C@3 y]-|&][s5; [*/ Return "
"value]-|&][s4;:`:`:Ctrl`:`:SetPosX`(Logc`): [%00-00 Ctrl`&_][%00-00* SetPosX][%00-00 "
"(Logc_][%00-00*@3 x][%00-00 )]&")
TOPIC_TEXT(
"][s2; &][s5; [%00-00*C@3 x]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:SetPosY`(Logc`): "
"[%00-00 Ctrl`&_][%00-00* SetPosY][%00-00 (Logc_][%00-00*@3 y][%00-00 )]&][s2; &][s5; "
"[%00-00*C@3 y]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:SetRect`(const Rect`&`): "
"[%00-00 void_][%00-00* SetRect][%00-00 (const_Rect`&_][%00-00*@3 r][%00-00 )]&][s2; "
"&][s5; [%00-00*C@3 r]-|&][s4;:`:`:Ctrl`:`:SetRect`(int")
TOPIC_TEXT(
"`,int`,int`,int`): [%00-00 void_][%00-00* SetRect][%00-00 (int_][%00-00*@3 x][%00-00 "
", int_][%00-00*@3 y][%00-00 , int_][%00-00*@3 cx][%00-00 , int_][%00-00*@3 cy][%00-00 "
")]&][s2; &][s5; [%00-00*C@3 x]-|&][s5; [%00-00*C@3 y]-|&][s5; [%00-00*C@3 cx]-|&][s5; "
"[%00-00*C@3 cy]-|&][s4;:`:`:Ctrl`:`:SetRectX`(int`,int`): [%00-00 void_][%00-00* "
"SetRectX][%00-00 (int_][%00-00*@3 x][%00-00 , int_][%00-00*@3 ")
TOPIC_TEXT(
"cx][%00-00 )]&][s2; &][s5; [%00-00*C@3 x]-|&][s5; [%00-00*C@3 cx]-|&][s4;:`:`:Ctrl`:`:SetRectY`(int`,int`): "
"[%00-00 void_][%00-00* SetRectY][%00-00 (int_][%00-00*@3 y][%00-00 , int_][%00-00*@3 "
"cy][%00-00 )]&][s2; &][s5; [%00-00*C@3 y]-|&][s5; [%00-00*C@3 cy]-|&][s4;:`:`:Ctrl`:`:SetFramePos`(LogPos`): "
"[%00-00 Ctrl`&_][%00-00* SetFramePos][%00-00 (LogPos_][%00-00*@3 p][%00-00 )]&][s2; "
"&][s5; [%00-00")
TOPIC_TEXT(
"*C@3 p]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:SetFramePos`(Logc`,Logc`): "
"[%00-00 Ctrl`&_][%00-00* SetFramePos][%00-00 (Logc_][%00-00*@3 x][%00-00 , Logc_][%00-00*@3 "
"y][%00-00 )]&][s2; &][s5; [%00-00*C@3 x]-|&][s5; [%00-00*C@3 y]-|&][s5; [*/ Return "
"value]-|&][s4;:`:`:Ctrl`:`:SetFramePosX`(Logc`): [%00-00 Ctrl`&_][%00-00* SetFramePosX][%00-00 "
"(Logc_][%00-00*@3 x][%00-00 )]&][s2; &][s5; [%00-")
TOPIC_TEXT(
"00*C@3 x]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:SetFramePosY`(Logc`): [%00-00 "
"Ctrl`&_][%00-00* SetFramePosY][%00-00 (Logc_][%00-00*@3 y][%00-00 )]&][s2; &][s5; "
"[%00-00*C@3 y]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:SetFrameRect`(const "
"Rect`&`): [%00-00 void_][%00-00* SetFrameRect][%00-00 (const_Rect`&_][%00-00*@3 r][%00-00 "
")]&][s2; &][s5; [%00-00*C@3 r]-|&][s4;:`:`:Ctrl`:`:SetFrameRe")
TOPIC_TEXT(
"ct`(int`,int`,int`,int`): [%00-00 void_][%00-00* SetFrameRect][%00-00 (int_][%00-00*@3 "
"x][%00-00 , int_][%00-00*@3 y][%00-00 , int_][%00-00*@3 cx][%00-00 , int_][%00-00*@3 "
"cy][%00-00 )]&][s2; &][s5; [%00-00*C@3 x]-|&][s5; [%00-00*C@3 y]-|&][s5; [%00-00*C@3 "
"cx]-|&][s5; [%00-00*C@3 cy]-|&][s4;:`:`:Ctrl`:`:SetFrameRectX`(int`,int`): [%00-00 "
"void_][%00-00* SetFrameRectX][%00-00 (int_][%00-00*@3 x][%00")
TOPIC_TEXT(
"-00 , int_][%00-00*@3 cx][%00-00 )]&][s2; &][s5; [%00-00*C@3 x]-|&][s5; [%00-00*C@3 "
"cx]-|&][s4;:`:`:Ctrl`:`:SetFrameRectY`(int`,int`): [%00-00 void_][%00-00* SetFrameRectY][%00-00 "
"(int_][%00-00*@3 y][%00-00 , int_][%00-00*@3 cy][%00-00 )]&][s2; &][s5; [%00-00*C@3 "
"y]-|&][s5; [%00-00*C@3 cy]-|&][s4;:`:`:Ctrl`:`:InFrame`(`)const: [%00-00 bool_][%00-00* "
"InFrame][%00-00 ()_const]&][s2; &][s5; [*/ Retur")
TOPIC_TEXT(
"n value]-|&][s4;:`:`:Ctrl`:`:InView`(`)const: [%00-00 bool_][%00-00* InView][%00-00 "
"()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetPos`(`)const: [%00-00 "
"LogPos_][%00-00* GetPos][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:RefreshLayout`(`): "
"[%00-00 void_][%00-00* RefreshLayout][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:RefreshParentLayout`(`): "
"[%00-00 void_][%00-")
TOPIC_TEXT(
"00* RefreshParentLayout][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:LeftPos`(int`,int`): "
"[%00-00 Ctrl`&_][%00-00* LeftPos][%00-00 (int_][%00-00*@3 a][%00-00 , int_][%00-00*@3 "
"size][%00-00 _`=_STDSIZE)]&][s2; &][s5; [%00-00*C@3 a]-|&][s5; [%00-00*C@3 size]-|&][s5; "
"[*/ Return value]-|&][s4;:`:`:Ctrl`:`:RightPos`(int`,int`): [%00-00 Ctrl`&_][%00-00* "
"RightPos][%00-00 (int_][%00-00*@3 a][%00-00 , int_][%00-00")
TOPIC_TEXT(
"*@3 size][%00-00 _`=_STDSIZE)]&][s2; &][s5; [%00-00*C@3 a]-|&][s5; [%00-00*C@3 size]-|&][s5; "
"[*/ Return value]-|&][s4;:`:`:Ctrl`:`:TopPos`(int`,int`): [%00-00 Ctrl`&_][%00-00* "
"TopPos][%00-00 (int_][%00-00*@3 a][%00-00 , int_][%00-00*@3 size][%00-00 _`=_STDSIZE)]&][s2; "
"&][s5; [%00-00*C@3 a]-|&][s5; [%00-00*C@3 size]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:BottomPos`(int`,int`): "
"[%00-00 Ctrl`&")
TOPIC_TEXT(
"_][%00-00* BottomPos][%00-00 (int_][%00-00*@3 a][%00-00 , int_][%00-00*@3 size][%00-00 "
"_`=_STDSIZE)]&][s2; &][s5; [%00-00*C@3 a]-|&][s5; [%00-00*C@3 size]-|&][s5; [*/ Return "
"value]-|&][s4;:`:`:Ctrl`:`:HSizePos`(int`,int`): [%00-00 Ctrl`&_][%00-00* HSizePos][%00-00 "
"(int_][%00-00*@3 a][%00-00 _`=_][%00-00@3 0][%00-00 , int_][%00-00*@3 b][%00-00 _`=_][%00-00@3 "
"0][%00-00 )]&][s2; &][s5; [%00-00*C@3 a]")
TOPIC_TEXT(
"-|&][s5; [%00-00*C@3 b]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:VSizePos`(int`,int`): "
"[%00-00 Ctrl`&_][%00-00* VSizePos][%00-00 (int_][%00-00*@3 a][%00-00 _`=_][%00-00@3 "
"0][%00-00 , int_][%00-00*@3 b][%00-00 _`=_][%00-00@3 0][%00-00 )]&][s2; &][s5; [%00-00*C@3 "
"a]-|&][s5; [%00-00*C@3 b]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:SizePos`(`): "
"[%00-00 Ctrl`&_][%00-00* SizePos][%00-00 ()]&][s")
TOPIC_TEXT(
"2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:HCenterPos`(int`,int`): [%00-00 "
"Ctrl`&_][%00-00* HCenterPos][%00-00 (int_][%00-00*@3 size][%00-00 _`=_STDSIZE, int_][%00-00*@3 "
"delta][%00-00 _`=_][%00-00@3 0][%00-00 )]&][s2; &][s5; [%00-00*C@3 size]-|&][s5; "
"[%00-00*C@3 delta]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:VCenterPos`(int`,int`): "
"[%00-00 Ctrl`&_][%00-00* VCenterPos][%00-00 (int_][%00-")
TOPIC_TEXT(
"00*@3 size][%00-00 _`=_STDSIZE, int_][%00-00*@3 delta][%00-00 _`=_][%00-00@3 0][%00-00 "
")]&][s2; &][s5; [%00-00*C@3 size]-|&][s5; [%00-00*C@3 delta]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:LeftPosZ`(int`,int`): "
"[%00-00 Ctrl`&_][%00-00* LeftPosZ][%00-00 (int_][%00-00*@3 a][%00-00 , int_][%00-00*@3 "
"size][%00-00 _`=_STDSIZE)]&][s2; &][s5; [%00-00*C@3 a]-|&][s5; [%00-00*C@3 size]-|&][s5; "
"[*/ Retu")
TOPIC_TEXT(
"rn value]-|&][s4;:`:`:Ctrl`:`:RightPosZ`(int`,int`): [%00-00 Ctrl`&_][%00-00* RightPosZ][%00-00 "
"(int_][%00-00*@3 a][%00-00 , int_][%00-00*@3 size][%00-00 _`=_STDSIZE)]&][s2; &][s5; "
"[%00-00*C@3 a]-|&][s5; [%00-00*C@3 size]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:TopPosZ`(int`,int`): "
"[%00-00 Ctrl`&_][%00-00* TopPosZ][%00-00 (int_][%00-00*@3 a][%00-00 , int_][%00-00*@3 "
"size][%00-00 _`=_STDSIZE)")
TOPIC_TEXT(
"]&][s2; &][s5; [%00-00*C@3 a]-|&][s5; [%00-00*C@3 size]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:BottomPosZ`(int`,int`): "
"[%00-00 Ctrl`&_][%00-00* BottomPosZ][%00-00 (int_][%00-00*@3 a][%00-00 , int_][%00-00*@3 "
"size][%00-00 _`=_STDSIZE)]&][s2; &][s5; [%00-00*C@3 a]-|&][s5; [%00-00*C@3 size]-|&][s5; "
"[*/ Return value]-|&][s4;:`:`:Ctrl`:`:HSizePosZ`(int`,int`): [%00-00 Ctrl`&_][%00-00* "
"HSizePosZ]")
TOPIC_TEXT(
"[%00-00 (int_][%00-00*@3 a][%00-00 _`=_][%00-00@3 0][%00-00 , int_][%00-00*@3 b][%00-00 "
"_`=_][%00-00@3 0][%00-00 )]&][s2; &][s5; [%00-00*C@3 a]-|&][s5; [%00-00*C@3 b]-|&][s5; "
"[*/ Return value]-|&][s4;:`:`:Ctrl`:`:VSizePosZ`(int`,int`): [%00-00 Ctrl`&_][%00-00* "
"VSizePosZ][%00-00 (int_][%00-00*@3 a][%00-00 _`=_][%00-00@3 0][%00-00 , int_][%00-00*@3 "
"b][%00-00 _`=_][%00-00@3 0][%00-00 )]&][s2; &][s5; ")
TOPIC_TEXT(
"[%00-00*C@3 a]-|&][s5; [%00-00*C@3 b]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:HCenterPosZ`(int`,int`): "
"[%00-00 Ctrl`&_][%00-00* HCenterPosZ][%00-00 (int_][%00-00*@3 size][%00-00 _`=_STDSIZE, "
"int_][%00-00*@3 delta][%00-00 _`=_][%00-00@3 0][%00-00 )]&][s2; &][s5; [%00-00*C@3 "
"size]-|&][s5; [%00-00*C@3 delta]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:VCenterPosZ`(int`,int`): "
"[%00-00 Ctrl`&_]")
TOPIC_TEXT(
"[%00-00* VCenterPosZ][%00-00 (int_][%00-00*@3 size][%00-00 _`=_STDSIZE, int_][%00-00*@3 "
"delta][%00-00 _`=_][%00-00@3 0][%00-00 )]&][s2; &][s5; [%00-00*C@3 size]-|&][s5; "
"[%00-00*C@3 delta]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:LockLayout`(`): "
"[%00-00 void_][%00-00* LockLayout][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:UnlockLayout`(`): "
"[%00-00 bool_][%00-00* UnlockLayout][%00-00 ()]&][s2; &][s5; ")
TOPIC_TEXT(
"[*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetRect`(`)const: [%00-00 Rect_][%00-00* GetRect][%00-00 "
"()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetScreenRect`(`)const: "
"[%00-00 Rect_][%00-00* GetScreenRect][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetView`(`)const: "
"[%00-00 Rect_][%00-00* GetView][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl")
TOPIC_TEXT(
"`:`:GetScreenView`(`)const: [%00-00 Rect_][%00-00* GetScreenView][%00-00 ()_const]&][s2; "
"&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetSize`(`)const: [%00-00 Size_][%00-00* "
"GetSize][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetVisibleScreenRect`(`)const: "
"[%00-00 Rect_][%00-00* GetVisibleScreenRect][%00-00 ()_const]&][s2; &][s5; [*/ Return "
"value]-|&][s4;:`:`:Ctrl`:`:GetV")
TOPIC_TEXT(
"isibleScreenView`(`)const: [%00-00 Rect_][%00-00* GetVisibleScreenView][%00-00 ()_const]&][s2; "
"&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:AddFrameSize`(int`,int`)const: [%00-00 "
"Size_][%00-00* AddFrameSize][%00-00 (int_][%00-00*@3 cx][%00-00 , int_][%00-00*@3 "
"cy][%00-00 )_const]&][s2; &][s5; [%00-00*C@3 cx]-|&][s5; [%00-00*C@3 cy]-|&][s5; "
"[*/ Return value]-|&][s4;:`:`:Ctrl`:`:AddFrameSize`(Size`)")
TOPIC_TEXT(
"const: [%00-00 Size_][%00-00* AddFrameSize][%00-00 (Size_][%00-00*@3 sz][%00-00 "
")_const]&][s2; &][s5; [%00-00*C@3 sz]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:Refresh`(const "
"Rect`&`): [%00-00 void_][%00-00* Refresh][%00-00 (const_Rect`&_][%00-00*@3 r][%00-00 "
")]&][s2; &][s5; [%00-00*C@3 r]-|&][s4;:`:`:Ctrl`:`:Refresh`(int`,int`,int`,int`): "
"[%00-00 void_][%00-00* Refresh][%00-00 (int_][%00-00*@")
TOPIC_TEXT(
"3 x][%00-00 , int_][%00-00*@3 y][%00-00 , int_][%00-00*@3 cx][%00-00 , int_][%00-00*@3 "
"cy][%00-00 )]&][s2; &][s5; [%00-00*C@3 x]-|&][s5; [%00-00*C@3 y]-|&][s5; [%00-00*C@3 "
"cx]-|&][s5; [%00-00*C@3 cy]-|&][s4;:`:`:Ctrl`:`:Refresh`(`): [%00-00 void_][%00-00* "
"Refresh][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:IsFullRefresh`(`)const: [%00-00 bool_][%00-00* "
"IsFullRefresh][%00-00 ()_const]&][s2; &][s5; [*/ Ret")
TOPIC_TEXT(
"urn value]-|&][s4;:`:`:Ctrl`:`:RefreshFrame`(const Rect`&`): [%00-00 void_][%00-00* "
"RefreshFrame][%00-00 (const_Rect`&_][%00-00*@3 r][%00-00 )]&][s2; &][s5; [%00-00*C@3 "
"r]-|&][s4;:`:`:Ctrl`:`:RefreshFrame`(int`,int`,int`,int`): [%00-00 void_][%00-00* "
"RefreshFrame][%00-00 (int_][%00-00*@3 x][%00-00 , int_][%00-00*@3 y][%00-00 , int_][%00-00*@3 "
"cx][%00-00 , int_][%00-00*@3 cy][%00-00 )]&][s2; &][s5;")
TOPIC_TEXT(
" [%00-00*C@3 x]-|&][s5; [%00-00*C@3 y]-|&][s5; [%00-00*C@3 cx]-|&][s5; [%00-00*C@3 "
"cy]-|&][s4;:`:`:Ctrl`:`:RefreshFrame`(`): [%00-00 void_][%00-00* RefreshFrame][%00-00 "
"()]&][s2; &][s4;:`:`:Ctrl`:`:ScrollView`(const Rect`&`,int`,int`): [%00-00 void_][%00-00* "
"ScrollView][%00-00 (const_Rect`&_][%00-00*@3 r][%00-00 , int_][%00-00*@3 dx][%00-00 "
", int_][%00-00*@3 dy][%00-00 )]&][s2; &][s5; [%00-00*C@3 ")
TOPIC_TEXT(
"r]-|&][s5; [%00-00*C@3 dx]-|&][s5; [%00-00*C@3 dy]-|&][s4;:`:`:Ctrl`:`:ScrollView`(int`,int`,int`,int`,int`,int`): "
"[%00-00 void_][%00-00* ScrollView][%00-00 (int_][%00-00*@3 x][%00-00 , int_][%00-00*@3 "
"y][%00-00 , int_][%00-00*@3 cx][%00-00 , int_][%00-00*@3 cy][%00-00 , int_][%00-00*@3 "
"dx][%00-00 , int_][%00-00*@3 dy][%00-00 )]&][s2; &][s5; [%00-00*C@3 x]-|&][s5; [%00-00*C@3 "
"y]-|&][s5; [%00-00*C@")
TOPIC_TEXT(
"3 cx]-|&][s5; [%00-00*C@3 cy]-|&][s5; [%00-00*C@3 dx]-|&][s5; [%00-00*C@3 dy]-|&][s4;:`:`:Ctrl`:`:ScrollView`(int`,int`): "
"[%00-00 void_][%00-00* ScrollView][%00-00 (int_][%00-00*@3 dx][%00-00 , int_][%00-00*@3 "
"dy][%00-00 )]&][s2; &][s5; [%00-00*C@3 dx]-|&][s5; [%00-00*C@3 dy]-|&][s4;:`:`:Ctrl`:`:ScrollView`(const "
"Rect`&`,Size`): [%00-00 void_][%00-00* ScrollView][%00-00 (const_Rect`&_][%00-00*@3 "
"r")
TOPIC_TEXT(
"][%00-00 , Size_][%00-00*@3 delta][%00-00 )]&][s2; &][s5; [%00-00*C@3 r]-|&][s5; "
"[%00-00*C@3 delta]-|&][s4;:`:`:Ctrl`:`:ScrollView`(Size`): [%00-00 void_][%00-00* "
"ScrollView][%00-00 (Size_][%00-00*@3 delta][%00-00 )]&][s2; &][s5; [%00-00*C@3 delta]-|&][s4;:`:`:Ctrl`:`:Sync`(`): "
"[%00-00 void_][%00-00* Sync][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:Sync`(const Rect`&`): "
"[%00-00 void_][%00-00* Sync][%00-0")
TOPIC_TEXT(
"0 (const_Rect`&_][%00-00*@3 r][%00-00 )]&][s2; &][s5; [%00-00*C@3 r]-|&][s4;:`:`:Ctrl`:`:DrawCtrl`(Draw`&`,int`,int`): "
"[%00-00 void_][%00-00* DrawCtrl][%00-00 (Draw`&_][%00-00*@3 w][%00-00 , int_][%00-00*@3 "
"x][%00-00 _`=_][%00-00@3 0][%00-00 , int_][%00-00*@3 y][%00-00 _`=_][%00-00@3 0][%00-00 "
")]&][s2; &][s5; [%00-00*C@3 w]-|&][s5; [%00-00*C@3 x]-|&][s5; [%00-00*C@3 y]-|&][s4;:`:`:Ctrl`:`:DrawCtrl")
TOPIC_TEXT(
"WithParent`(Draw`&`,int`,int`): [%00-00 void_][%00-00* DrawCtrlWithParent][%00-00 "
"(Draw`&_][%00-00*@3 w][%00-00 , int_][%00-00*@3 x][%00-00 _`=_][%00-00@3 0][%00-00 "
", int_][%00-00*@3 y][%00-00 _`=_][%00-00@3 0][%00-00 )]&][s2; &][s5; [%00-00*C@3 "
"w]-|&][s5; [%00-00*C@3 x]-|&][s5; [%00-00*C@3 y]-|&][s4;:`:`:Ctrl`:`:HasChild`(Ctrl`*`)const: "
"[%00-00 bool_][%00-00* HasChild][%00-00 (Ctrl_`*][%00-00*@3 ")
TOPIC_TEXT(
"ctrl][%00-00 )_const]&][s2; &][s5; [%00-00*C@3 ctrl]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:HasChildDeep`(Ctrl`*`)const: "
"[%00-00 bool_][%00-00* HasChildDeep][%00-00 (Ctrl_`*][%00-00*@3 ctrl][%00-00 )_const]&][s2; "
"&][s5; [%00-00*C@3 ctrl]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:SetMnemonic`(int`): "
"[%00-00 Ctrl`&_][%00-00* SetMnemonic][%00-00 (int_][%00-00*@3 hotchar][%00-00 )]&][s2; "
"&]")
TOPIC_TEXT(
"[s5; [%00-00*C@3 hotchar]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetMnemonic`(`)const: "
"[%00-00 int_][%00-00* GetMnemonic][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:IsMnemonic`(int`)const: "
"[%00-00 bool_][%00-00* IsMnemonic][%00-00 (int_][%00-00*@3 key][%00-00 )_const]&][s2; "
"&][s5; [%00-00*C@3 key]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:SetLabel`(const "
"char`*`")
TOPIC_TEXT(
"): [%00-00 Ctrl`&_][%00-00* SetLabel][%00-00 (const_char_`*][%00-00*@3 text][%00-00 "
")]&][s2; &][s5; [%00-00*C@3 text]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:IgnoreMouse`(bool`): "
"[%00-00 Ctrl`&_][%00-00* IgnoreMouse][%00-00 (bool_][%00-00*@3 b][%00-00 _`=_true)]&][s2; "
"&][s5; [%00-00*C@3 b]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:NoIgnoreMouse`(`): "
"[%00-00 Ctrl`&_][%00-00* NoIgnoreMouse")
TOPIC_TEXT(
"][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:HasMouse`(`)const: "
"[%00-00 bool_][%00-00* HasMouse][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:IsMouseInFrame`(`): "
"[%00-00 bool_][%00-00* IsMouseInFrame][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:IsMouseIn`(`): "
"[%00-00 bool_][%00-00* IsMouseIn][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][")
TOPIC_TEXT(
"s4;:`:`:Ctrl`:`:IgnoreMouseClick`(`): [%00-00 static void_][%00-00* IgnoreMouseClick][%00-00 "
"()]&][s2; &][s4;:`:`:Ctrl`:`:IgnoreMouseUp`(`): [%00-00 static void_][%00-00* IgnoreMouseUp][%00-00 "
"()]&][s2; &][s4;:`:`:Ctrl`:`:SetCapture`(`): [%00-00 bool_][%00-00* SetCapture][%00-00 "
"()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:ReleaseCapture`(`): [%00-00 "
"bool_][%00-00* ReleaseCapture][%00-00")
TOPIC_TEXT(
" ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:HasCapture`(`)const: [%00-00 "
"bool_][%00-00* HasCapture][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:ReleaseCtrlCapture`(`): "
"[%00-00 static bool_][%00-00* ReleaseCtrlCapture][%00-00 ()]&][s2; &][s5; [*/ Return "
"value]-|&][s4;:`:`:Ctrl`:`:SetFocus`(`): [%00-00 bool_][%00-00* SetFocus][%00-00 "
"()]&][s2; &][s5; [*/ Return va")
TOPIC_TEXT(
"lue]-|&][s4;:`:`:Ctrl`:`:HasFocus`(`)const: [%00-00 bool_][%00-00* HasFocus][%00-00 "
"()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:HasFocusDeep`(`)const: "
"[%00-00 bool_][%00-00* HasFocusDeep][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:WantFocus`(bool`): "
"[%00-00 Ctrl`&_][%00-00* WantFocus][%00-00 (bool_][%00-00*@3 ft][%00-00 _`=_true)]&][s2; "
"&][s5; [%00-00*C@3 ")
TOPIC_TEXT(
"ft]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:NoWantFocus`(`): [%00-00 Ctrl`&_][%00-00* "
"NoWantFocus][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:IsWantFocus`(`)const: "
"[%00-00 bool_][%00-00* IsWantFocus][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:SetWantFocus`(`): "
"[%00-00 bool_][%00-00* SetWantFocus][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:")
TOPIC_TEXT(
"`:Ctrl`:`:GetFocusChild`(`)const: [%00-00 Ctrl_`*][%00-00* GetFocusChild][%00-00 "
"()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetFocusChildDeep`(`)const: "
"[%00-00 Ctrl_`*][%00-00* GetFocusChildDeep][%00-00 ()_const]&][s2; &][s5; [*/ Return "
"value]-|&][s4;:`:`:Ctrl`:`:CancelModeDeep`(`): [%00-00 void_][%00-00* CancelModeDeep][%00-00 "
"()]&][s2; &][s4;:`:`:Ctrl`:`:SetCaret`(int`,int`,int")
TOPIC_TEXT(
"`,int`): [%00-00 void_][%00-00* SetCaret][%00-00 (int_][%00-00*@3 x][%00-00 , int_][%00-00*@3 "
"y][%00-00 , int_][%00-00*@3 cx][%00-00 , int_][%00-00*@3 cy][%00-00 )]&][s2; &][s5; "
"[%00-00*C@3 x]-|&][s5; [%00-00*C@3 y]-|&][s5; [%00-00*C@3 cx]-|&][s5; [%00-00*C@3 "
"cy]-|&][s4;:`:`:Ctrl`:`:SetCaret`(const Rect`&`): [%00-00 void_][%00-00* SetCaret][%00-00 "
"(const_Rect`&_][%00-00*@3 r][%00-00 )]&][s2; &][s5")
TOPIC_TEXT(
"; [%00-00*C@3 r]-|&][s4;:`:`:Ctrl`:`:KillCaret`(`): [%00-00 void_][%00-00* KillCaret][%00-00 "
"()]&][s2; &][s4;:`:`:Ctrl`:`:GetFocusCtrl`(`): [%00-00 static Ctrl_`*][%00-00* GetFocusCtrl][%00-00 "
"()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:IterateFocusForward`(Ctrl`*`,Ctrl`*`,bool`): "
"[%00-00 static bool_][%00-00* IterateFocusForward][%00-00 (Ctrl_`*][%00-00*@3 ctrl][%00-00 "
", Ctrl_`*][%00-0")
TOPIC_TEXT(
"0*@3 top][%00-00 , bool_][%00-00*@3 noframe][%00-00 _`=_false)]&][s2; &][s5; [%00-00*C@3 "
"ctrl]-|&][s5; [%00-00*C@3 top]-|&][s5; [%00-00*C@3 noframe]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:IterateFocusBackward`(Ctrl`*`,Ctrl`*`,bool`): "
"[%00-00 static bool_][%00-00* IterateFocusBackward][%00-00 (Ctrl_`*][%00-00*@3 ctrl][%00-00 "
", Ctrl_`*][%00-00*@3 top][%00-00 , bool_][%00-00*@3 noframe][%00-00")
TOPIC_TEXT(
" _`=_false)]&][s2; &][s5; [%00-00*C@3 ctrl]-|&][s5; [%00-00*C@3 top]-|&][s5; [%00-00*C@3 "
"noframe]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:Show`(bool`): [%00-00 void_][%00-00* "
"Show][%00-00 (bool_][%00-00*@3 show][%00-00 _`=_true)]&][s2; &][s5; [%00-00*C@3 show]-|&][s4;:`:`:Ctrl`:`:Hide`(`): "
"[%00-00 void_][%00-00* Hide][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:IsShown`(`)const: "
"[%00-00 bool_][%00-0")
TOPIC_TEXT(
"0* IsShown][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:IsVisible`(`)const: "
"[%00-00 bool_][%00-00* IsVisible][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:Enable`(bool`): "
"[%00-00 void_][%00-00* Enable][%00-00 (bool_][%00-00*@3 enable][%00-00 _`=_true)]&][s2; "
"&][s5; [%00-00*C@3 enable]-|&][s4;:`:`:Ctrl`:`:Disable`(`): [%00-00 void_][%00-00* "
"Disable][%0")
TOPIC_TEXT(
"0-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:IsEnabled`(`)const: [%00-00 bool_][%00-00* IsEnabled][%00-00 "
"()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:SetEditable`(bool`): "
"[%00-00 Ctrl`&_][%00-00* SetEditable][%00-00 (bool_][%00-00*@3 editable][%00-00 _`=_true)]&][s2; "
"&][s5; [%00-00*C@3 editable]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:SetReadOnly`(`): "
"[%00-00 Ctrl`&_][%00-00* SetReadOn")
TOPIC_TEXT(
"ly][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:IsEditable`(`)const: "
"[%00-00 bool_][%00-00* IsEditable][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:IsReadOnly`(`)const: "
"[%00-00 bool_][%00-00* IsReadOnly][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:ResetModify`(`): "
"[%00-00 void_][%00-00* ResetModify][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`")
TOPIC_TEXT(
":`:IsModifySet`(`)const: [%00-00 bool_][%00-00* IsModifySet][%00-00 ()_const]&][s2; "
"&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:UpdateRefresh`(`): [%00-00 void_][%00-00* "
"UpdateRefresh][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:Update`(`): [%00-00 void_][%00-00* "
"Update][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:Action`(`): [%00-00 void_][%00-00* Action][%00-00 "
"()]&][s2; &][s4;:`:`:Ctrl`:`:UpdateAction`(`): [%")
TOPIC_TEXT(
"00-00 void_][%00-00* UpdateAction][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:UpdateActionRefresh`(`): "
"[%00-00 void_][%00-00* UpdateActionRefresh][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:BackPaint`(int`): "
"[%00-00 Ctrl`&_][%00-00* BackPaint][%00-00 (int_][%00-00*@3 bp][%00-00 _`=_FULLBACKPAINT)]&][s2; "
"&][s5; [%00-00*C@3 bp]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:TransparentBackPaint`(`): "
"[%00-00 Ctrl`&")
TOPIC_TEXT(
"_][%00-00* TransparentBackPaint][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:NoBackPaint`(`): "
"[%00-00 Ctrl`&_][%00-00* NoBackPaint][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetBackPaint`(`): "
"[%00-00 int_][%00-00* GetBackPaint][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:Transparent`(bool`): "
"[%00-00 Ctrl`&_][%00-00* Transparent][%00-00 (bool_")
TOPIC_TEXT(
"][%00-00*@3 bp][%00-00 _`=_true)]&][s2; &][s5; [%00-00*C@3 bp]-|&][s5; [*/ Return "
"value]-|&][s4;:`:`:Ctrl`:`:NoTransparent`(`): [%00-00 Ctrl`&_][%00-00* NoTransparent][%00-00 "
"()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:IsTransparent`(`)const: [%00-00 "
"bool_][%00-00* IsTransparent][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:ActiveX`(bool`): "
"[%00-00 Ctrl`&_][%00-00")
TOPIC_TEXT(
"* ActiveX][%00-00 (bool_][%00-00*@3 ax][%00-00 _`=_true)]&][s2; &][s5; [%00-00*C@3 "
"ax]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:NoActiveX`(`): [%00-00 Ctrl`&_][%00-00* "
"NoActiveX][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:IsActiveX`(`)const: "
"[%00-00 bool_][%00-00* IsActiveX][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:DesignMode`(bool`): "
"[%00-00 Ctrl")
TOPIC_TEXT(
"`&_][%00-00* DesignMode][%00-00 (bool_][%00-00*@3 b][%00-00 _`=_true)]&][s2; &][s5; "
"[%00-00*C@3 b]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:IsDesignMode`(`)const: "
"[%00-00 bool_][%00-00* IsDesignMode][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:Info`(const "
"char`*`): [%00-00 Ctrl`&_][%00-00* Info][%00-00 (const_char_`*][%00-00*@3 txt][%00-00 "
")]&][s2; &][s5; [%00-00*C@3 t")
TOPIC_TEXT(
"xt]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetInfo`(`)const: [%00-00 String_][%00-00* "
"GetInfo][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:Tip`(const "
"char`*`): [%00-00 Ctrl`&_][%00-00* Tip][%00-00 (const_char_`*][%00-00*@3 txt][%00-00 "
")]&][s2; &][s5; [%00-00*C@3 txt]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:HelpLine`(const "
"char`*`): [%00-00 Ctrl`&_][%00-00* Help")
TOPIC_TEXT(
"Line][%00-00 (const_char_`*][%00-00*@3 txt][%00-00 )]&][s2; &][s5; [%00-00*C@3 txt]-|&][s5; "
"[*/ Return value]-|&][s4;:`:`:Ctrl`:`:Description`(const char`*`): [%00-00 Ctrl`&_][%00-00* "
"Description][%00-00 (const_char_`*][%00-00*@3 txt][%00-00 )]&][s2; &][s5; [%00-00*C@3 "
"txt]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:HelpTopic`(const char`*`): [%00-00 "
"Ctrl`&_][%00-00* HelpTopic][%00-00 (const_ch")
TOPIC_TEXT(
"ar_`*][%00-00*@3 txt][%00-00 )]&][s2; &][s5; [%00-00*C@3 txt]-|&][s5; [*/ Return "
"value]-|&][s4;:`:`:Ctrl`:`:GetTip`(`)const: [%00-00 String_][%00-00* GetTip][%00-00 "
"()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetHelpLine`(`)const: "
"[%00-00 String_][%00-00* GetHelpLine][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetDescription`(`)const: "
"[%00-00 String_][%00-")
TOPIC_TEXT(
"00* GetDescription][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetHelpTopic`(`)const: "
"[%00-00 String_][%00-00* GetHelpTopic][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:Add`(Ctrl`&`): "
"[%00-00 void_][%00-00* Add][%00-00 (Ctrl`&_][%00-00*@3 ctrl][%00-00 )]&][s2; &][s5; "
"[%00-00*C@3 ctrl]-|&][s4;:`:`:Ctrl`:`:operator`<`<`(Ctrl`&`): [%00-00 Ctrl`&_][%00-")
TOPIC_TEXT(
"00* operator<<][%00-00 (Ctrl`&_][%00-00*@3 ctrl][%00-00 )]&][s2; &][s5; [%00-00*C@3 "
"ctrl]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:Remove`(`): [%00-00 void_][%00-00* "
"Remove][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:operator`<`<`=`(const Value`&`): [%00-00 "
"const_Value`&_][%00-00* operator<<`=][%00-00 (const_Value`&_][%00-00*@3 v][%00-00 "
")]&][s2; &][s5; [%00-00*C@3 v]-|&][s5; [*/ Return value]-|&][s")
TOPIC_TEXT(
"4;:`:`:Ctrl`:`:operator`<`<`=`(Callback`): [%00-00 Callback_][%00-00* operator<<`=][%00-00 "
"(Callback_][%00-00*@3 action][%00-00 )]&][s2; &][s5; [%00-00*C@3 action]-|&][s5; "
"[*/ Return value]-|&][s4;:`:`:Ctrl`:`:operator`<`<`(Callback`): [%00-00 Callback`&_][%00-00* "
"operator<<][%00-00 (Callback_][%00-00*@3 action][%00-00 )]&][s2; &][s5; [%00-00*C@3 "
"action]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`")
TOPIC_TEXT(
":`:SetTimeCallback`(int`,Callback`,int`): [%00-00 void_][%00-00* SetTimeCallback][%00-00 "
"(int_][%00-00*@3 delay`_ms][%00-00 , Callback_][%00-00*@3 cb][%00-00 , int_][%00-00*@3 "
"id][%00-00 _`=_][%00-00@3 0][%00-00 )]&][s2; &][s5; [%00-00*C@3 delay`_ms]-|&][s5; "
"[%00-00*C@3 cb]-|&][s5; [%00-00*C@3 id]-|&][s4;:`:`:Ctrl`:`:KillTimeCallback`(int`): "
"[%00-00 void_][%00-00* KillTimeCallback][%00-00 (int_][%")
TOPIC_TEXT(
"00-00*@3 id][%00-00 _`=_][%00-00@3 0][%00-00 )]&][s2; &][s5; [%00-00*C@3 id]-|&][s4;:`:`:Ctrl`:`:ExistsTimeCallback`(int`)const: "
"[%00-00 bool_][%00-00* ExistsTimeCallback][%00-00 (int_][%00-00*@3 id][%00-00 _`=_][%00-00@3 "
"0][%00-00 )_const]&][s2; &][s5; [%00-00*C@3 id]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetActiveCtrl`(`): "
"[%00-00 static Ctrl_`*][%00-00* GetActiveCtrl][%00-00 ()]&][s2; &")
TOPIC_TEXT(
"][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetActiveWindow`(`): [%00-00 static "
"Ctrl_`*][%00-00* GetActiveWindow][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetVisibleChild`(Ctrl`*`,Point`,bool`): "
"[%00-00 static Ctrl_`*][%00-00* GetVisibleChild][%00-00 (Ctrl_`*][%00-00*@3 ctrl][%00-00 "
", Point_][%00-00*@3 p][%00-00 , bool_][%00-00*@3 pointinframe][%00-00 )]&][s2; &][s5; "
"[%00-00*C")
TOPIC_TEXT(
"@3 ctrl]-|&][s5; [%00-00*C@3 p]-|&][s5; [%00-00*C@3 pointinframe]-|&][s5; [*/ Return "
"value]-|&][s4;:`:`:Ctrl`:`:PopUpHWND`(HWND`,bool`,bool`): [%00-00 void_][%00-00* "
"PopUpHWND][%00-00 (HWND_][%00-00*@3 hwnd][%00-00 , bool_][%00-00*@3 savebits][%00-00 "
"_`=_true, bool_][%00-00*@3 activate][%00-00 _`=_true)]&][s2; &][s5; [%00-00*C@3 hwnd]-|&][s5; "
"[%00-00*C@3 savebits]-|&][s5; [%00-00*C@3 activate]-|&]")
TOPIC_TEXT(
"[s4;:`:`:Ctrl`:`:PopUp`(Ctrl`*`,bool`,bool`): [%00-00 void_][%00-00* PopUp][%00-00 "
"(Ctrl_`*][%00-00*@3 owner][%00-00 _`=_NULL, bool_][%00-00*@3 savebits][%00-00 _`=_true, "
"bool_][%00-00*@3 activate][%00-00 _`=_true)]&][s2; &][s5; [%00-00*C@3 owner]-|&][s5; "
"[%00-00*C@3 savebits]-|&][s5; [%00-00*C@3 activate]-|&][s4;:`:`:Ctrl`:`:IsWaitingEvent`(`): "
"[%00-00 static bool_][%00-00* IsWaitingEvent][%00-00")
TOPIC_TEXT(
" ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:ProcessEvent`(bool`*`): "
"[%00-00 static bool_][%00-00* ProcessEvent][%00-00 (bool_`*][%00-00*@3 quit][%00-00 "
"_`=_NULL)]&][s2; &][s5; [%00-00*C@3 quit]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:ProcessEvents`(bool`*`): "
"[%00-00 static bool_][%00-00* ProcessEvents][%00-00 (bool_`*][%00-00*@3 quit][%00-00 "
"_`=_NULL)]&][s2; &][s5; [%00-00*C@3 qu")
TOPIC_TEXT(
"it]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:IsPopUp`(`)const: [%00-00 bool_][%00-00* "
"IsPopUp][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:EventLoop`(Ctrl`*`): "
"[%00-00 static void_][%00-00* EventLoop][%00-00 (Ctrl_`*][%00-00*@3 loopctrl][%00-00 "
"_`=_NULL)]&][s2; &][s5; [%00-00*C@3 loopctrl]-|&][s4;:`:`:Ctrl`:`:GetLoopLevel`(`): "
"[%00-00 static int_][%00-00* GetLoopLevel]")
TOPIC_TEXT(
"[%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetLoopCtrl`(`): "
"[%00-00 static Ctrl_`*][%00-00* GetLoopCtrl][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:EndLoop`(`): "
"[%00-00 void_][%00-00* EndLoop][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:EndLoop`(int`): "
"[%00-00 void_][%00-00* EndLoop][%00-00 (int_][%00-00*@3 code][%00-00 )]&][s2; &][s5; "
"[%00-00*C@3 code]-|&][s4;:`:`:C")
TOPIC_TEXT(
"trl`:`:InLoop`(`)const: [%00-00 bool_][%00-00* InLoop][%00-00 ()_const]&][s2; &][s5; "
"[*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetExitCode`(`)const: [%00-00 int_][%00-00* "
"GetExitCode][%00-00 ()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:SetMinSize`(Size`): "
"[%00-00 void_][%00-00* SetMinSize][%00-00 (Size_][%00-00*@3 sz][%00-00 )]&][s2; &][s5; "
"[%00-00*C@3 sz]-|&][s4;:`:`:Ctrl`:`:Csizeinit")
TOPIC_TEXT(
"`(`): [%00-00 static void_][%00-00* Csizeinit][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:GetZoomText`(`): "
"[%00-00 static const_char_`*][%00-00* GetZoomText][%00-00 ()]&][s2; &][s5; [*/ Return "
"value]-|&][s4;:`:`:Ctrl`:`:SetZoomSize`(Size`,Size: [%00-00 static void_][%00-00* "
"SetZoomSize][%00-00 (Size_][%00-00*@3 sz][%00-00 , Size_][%00-00*@3 bsz][%00-00 _`=_Size(][%00-00@3 "
"0][%00-00 , ][%00-00@3 0][%00-00")
TOPIC_TEXT(
" ))]&][s2; &][s5; [%00-00*C@3 sz]-|&][s5; [%00-00*C@3 bsz]-|&][s4;:`:`:Ctrl`:`:HorzLayoutZoom`(int`): "
"[%00-00 static int_][%00-00* HorzLayoutZoom][%00-00 (int_][%00-00*@3 cx][%00-00 )]&][s2; "
"&][s5; [%00-00*C@3 cx]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:VertLayoutZoom`(int`): "
"[%00-00 static int_][%00-00* VertLayoutZoom][%00-00 (int_][%00-00*@3 cy][%00-00 )]&][s2; "
"&][s5; [%00-00*C@3 cy]-|&][s")
TOPIC_TEXT(
"5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:LayoutZoom`(int`,int`): [%00-00 static "
"Size_][%00-00* LayoutZoom][%00-00 (int_][%00-00*@3 cx][%00-00 , int_][%00-00*@3 cy][%00-00 "
")]&][s2; &][s5; [%00-00*C@3 cx]-|&][s5; [%00-00*C@3 cy]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:LayoutZoom`(Size`): "
"[%00-00 static Size_][%00-00* LayoutZoom][%00-00 (Size_][%00-00*@3 sz][%00-00 )]&][s2; "
"&][s5; [%00-00*C@3 s")
TOPIC_TEXT(
"z]-|&][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:NoLayoutZoom`(`): [%00-00 static "
"void_][%00-00* NoLayoutZoom][%00-00 ()]&][s2; &][s4;:`:`:Ctrl`:`:ClickFocus`(`): "
"[%00-00 static bool_][%00-00* ClickFocus][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:ClickFocus`(bool`): "
"[%00-00 static void_][%00-00* ClickFocus][%00-00 (bool_][%00-00*@3 cf][%00-00 )]&][s2; "
"&][s5; [%00-00*C@3 cf]-|&]")
TOPIC_TEXT(
"[s4;:`:`:Ctrl`:`:GetWorkArea`(`): [%00-00 static Rect_][%00-00* GetWorkArea][%00-00 "
"()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetKbdDelay`(`): [%00-00 static "
"int_][%00-00* GetKbdDelay][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetKbdSpeed`(`): "
"[%00-00 static int_][%00-00* GetKbdSpeed][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:GetDefaultWindowRec")
TOPIC_TEXT(
"t`(`): [%00-00 static Rect_][%00-00* GetDefaultWindowRect][%00-00 ()]&][s2; &][s5; "
"[*/ Return value]-|&][s4;:`:`:Ctrl`:`:IsDragFullWindowMode`(`): [%00-00 static bool_][%00-00* "
"IsDragFullWindowMode][%00-00 ()]&][s2; &][s5; [*/ Return value]-|&][s4;:`:`:Ctrl`:`:IsOpenEffectsMode`(`): "
"[%00-00 static bool_][%00-00* IsOpenEffectsMode][%00-00 ()]&][s2; &][s5; [*/ Return "
"value]-|&][s4;:`:`:Ctrl`:`:GetAp")
TOPIC_TEXT(
"pName`(`): [%00-00 static String_][%00-00* GetAppName][%00-00 ()]&][s2; &][s5; [*/ "
"Return value]-|&][s4;:`:`:Ctrl`:`:SetAppName`(const String`&`): [%00-00 static void_][%00-00* "
"SetAppName][%00-00 (const_String`&_][%00-00*@3 appname][%00-00 )]&][s2; &][s5; [%00-00*C@3 "
"appname]-|&][s4;:`:`:Ctrl`:`:Name`(`)const: [%00-00 String_][%00-00* Name][%00-00 "
"()_const]&][s2; &][s5; [*/ Return value]-|&][s4;:`")
TOPIC_TEXT(
":`:Ctrl`:`:Dump`(`)const: [%00-00 virtual void_][%00-00* Dump][%00-00 ()_const]&][s2; "
"&][s4;:`:`:Ctrl`:`:Dump`(Stream`&`)const: [%00-00 virtual void_][%00-00* Dump][%00-00 "
"(Stream`&_][%00-00*@3 s][%00-00 )_const]&][s2; &][s5; [%00-00*C@3 s]-|&][s4;:`:`:Ctrl`:`:InitWin32`(HINSTANCE`): "
"[%00-00 static void_][%00-00* InitWin32][%00-00 (HINSTANCE_][%00-00*@3 hinst][%00-00 "
")]&][s2; &][s5; [%00-00*C@3 hi")
TOPIC_TEXT(
"nst]-|&][s4;:`:`:Ctrl`:`:InitX11`(const char`*`): [%00-00 static void_][%00-00* "
"InitX11][%00-00 (const_char_`*][%00-00*@3 display][%00-00 )]&][s2; &][s5; [%00-00*C@3 "
"display]-|&][s4;:`:`:Ctrl`:`:GuiFlush`(`): [%00-00 static void_][%00-00* GuiFlush][%00-00 "
"()]&][s2; &][s4;:`:`:Ctrl`:`:StdGuiSleep`(int`): [%00-00 static void_][%00-00* StdGuiSleep][%00-00 "
"(int_][%00-00*@3 ms][%00-00 )]&][s2; &][s5; [")
TOPIC_TEXT(
"%00-00*C@3 ms]-|&][s4;:`:`:Ctrl`:`:Callback WhenAction:* [%00-00* Callback_][%00-00 "
"WhenAction]&][s2; &][s4;:`:`:Ctrl`:`:static HINSTANCE hInstance:* [%00-00* static_HINSTANCE_][%00-00 "
"hInstance]&][s2; &][s4;:`:`:Ctrl`:`:static HCURSOR hCursor:* [%00-00* static_HCURSOR_][%00-00 "
"hCursor]&][s2; &][s4;:`:`:Ctrl`:`:static Atom XA`_WM`_PROTOCOLS:* [%00-00* static_Atom_][%00-00 "
"XA`_WM`_PROTOCOLS]&][s2; ")
TOPIC_TEXT(
"&][s4;:`:`:Ctrl`:`:static Atom XA`_WM`_DELETE`_WINDOW:* [%00-00* static_Atom_][%00-00 "
"XA`_WM`_DELETE`_WINDOW]&][s2; &][s4;:`:`:Ctrl`:`:static Atom XA`_WM`_TAKE`_FOCUS:* "
"[%00-00* static_Atom_][%00-00 XA`_WM`_TAKE`_FOCUS]&][s2; &][s4;:`:`:Ctrl`:`:static "
"Atom XA`_UPP`_SYNC:* [%00-00* static_Atom_][%00-00 XA`_UPP`_SYNC]&][s2; &][s4;:`:`:Ctrl`:`:static "
"Atom XA`_`_NET`_WM`_CONTEXT`_HELP:* [%00-00* stati")
TOPIC_TEXT(
"c_Atom_][%00-00 XA`_`_NET`_WM`_CONTEXT`_HELP]&][s2; &][s4;:`:`:Ctrl`:`:static Atom "
"XA`_`_MOTIF`_WM`_HINTS:* [%00-00* static_Atom_][%00-00 XA`_`_MOTIF`_WM`_HINTS]&][s2; "
"&][s4;:`:`:Ctrl`:`:static Atom XA`_KWIN`_RUNNING:* [%00-00* static_Atom_][%00-00 "
"XA`_KWIN`_RUNNING]&][s2; &][s4;:`:`:Ctrl`:`:static Atom XA`_KWM`_RUNNING:* [%00-00* "
"static_Atom_][%00-00 XA`_KWM`_RUNNING]&][s2; &][s4;:`:`:Ctrl`:`:sta")
TOPIC_TEXT(
"tic Atom XA`_GNOME`_BACKGROUND`_PROPERTIES:* [%00-00* static_Atom_][%00-00 XA`_GNOME`_BACKGROUND`_PROPERTIES]&][s2; "
"&][s4;:`:`:Ctrl`:`:static Atom XA`_`_QT`_SETTINGS`_TIMESTAMP`_:* [%00-00* static_Atom_][%00-00 "
"XA`_`_QT`_SETTINGS`_TIMESTAMP`_]&][s2; &][s4;:`:`:Ctrl`:`:static Atom XA`_`_NET`_SUPPORTED:* "
"[%00-00* static_Atom_][%00-00 XA`_`_NET`_SUPPORTED]&][s2; &][s4;:`:`:Ctrl`:`:static "
"Atom XA`_`_N")
TOPIC_TEXT(
"ET`_VIRTUAL`_ROOTS:* [%00-00* static_Atom_][%00-00 XA`_`_NET`_VIRTUAL`_ROOTS]&][s2; "
"&][s4;:`:`:Ctrl`:`:static Atom XA`_`_NET`_WM`_NAME:* [%00-00* static_Atom_][%00-00 "
"XA`_`_NET`_WM`_NAME]&][s2; &][s4;:`:`:Ctrl`:`:static Atom XA`_`_NET`_WM`_ICON`_NAME:* "
"[%00-00* static_Atom_][%00-00 XA`_`_NET`_WM`_ICON`_NAME]&][s2; &][s4;:`:`:Ctrl`:`:static "
"Atom XA`_`_NET`_WM`_ICON:* [%00-00* static_Atom_][%00-00 X")
TOPIC_TEXT(
"A`_`_NET`_WM`_ICON]&][s2; &][s4;:`:`:Ctrl`:`:static Atom XA`_`_NET`_WM`_STATE:* "
"[%00-00* static_Atom_][%00-00 XA`_`_NET`_WM`_STATE]&][s2; &][s4;:`:`:Ctrl`:`:static "
"Atom XA`_`_NET`_WM`_STATE`_MODAL:* [%00-00* static_Atom_][%00-00 XA`_`_NET`_WM`_STATE`_MODAL]&][s2; "
"&][s4;:`:`:Ctrl`:`:static Atom XA`_`_NET`_WM`_STATE`_MAXIMIZED`_VERT:* [%00-00* static_Atom_][%00-00 "
"XA`_`_NET`_WM`_STATE`_MAXIMIZED`_VE")
TOPIC_TEXT(
"RT]&][s2; &][s4;:`:`:Ctrl`:`:static Atom XA`_`_NET`_WM`_STATE`_MAXIMIZED`_HORZ:* "
"[%00-00* static_Atom_][%00-00 XA`_`_NET`_WM`_STATE`_MAXIMIZED`_HORZ]&][s2; &][s4;:`:`:Ctrl`:`:static "
"Atom XA`_`_NET`_WM`_WINDOW`_TYPE:* [%00-00* static_Atom_][%00-00 XA`_`_NET`_WM`_WINDOW`_TYPE]&][s2; "
"&][s4;:`:`:Ctrl`:`:static Atom XA`_`_NET`_WM`_WINDOW`_TYPE`_NORMAL:* [%00-00* static_Atom_][%00-00 "
"XA`_`_NET`_WM`_WIND")
TOPIC_TEXT(
"OW`_TYPE`_NORMAL]&][s2; &][s4;:`:`:Ctrl`:`:static Atom XA`_`_NET`_WM`_WINDOW`_TYPE`_DIALOG:* "
"[%00-00* static_Atom_][%00-00 XA`_`_NET`_WM`_WINDOW`_TYPE`_DIALOG]&][s2; &][s4;:`:`:Ctrl`:`:static "
"Atom XA`_`_NET`_WM`_WINDOW`_TYPE`_TOOLBAR:* [%00-00* static_Atom_][%00-00 XA`_`_NET`_WM`_WINDOW`_TYPE`_TOOLBAR]&][s2; "
"&][s4;:`:`:Ctrl`:`:static Atom XA`_`_KDE`_NET`_WM`_WINDOW`_TYPE`_OVERRIDE:* [%00-00* "
"stati")
TOPIC_TEXT(
"c_Atom_][%00-00 XA`_`_KDE`_NET`_WM`_WINDOW`_TYPE`_OVERRIDE]&][s2; &][s4;:`:`:Ctrl`:`:static "
"Atom XA`_`_KDE`_NET`_WM`_FRAME`_STRUT:* [%00-00* static_Atom_][%00-00 XA`_`_KDE`_NET`_WM`_FRAME`_STRUT]&][s2; "
"&][s4;:`:`:Ctrl`:`:static Atom XA`_`_NET`_WM`_STATE`_STAYS`_ON`_TOP:* [%00-00* static_Atom_][%00-00 "
"XA`_`_NET`_WM`_STATE`_STAYS`_ON`_TOP]&][s2; &][s4;:`:`:Ctrl`:`:static Atom XA`_`_NET`_WM`_MOVERESI")
TOPIC_TEXT(
"ZE:* [%00-00* static_Atom_][%00-00 XA`_`_NET`_WM`_MOVERESIZE]&][s2; &][s4;:`:`:Ctrl`:`:static "
"Atom XA`_`_KDE`_NET`_USER`_TIME:* [%00-00* static_Atom_][%00-00 XA`_`_KDE`_NET`_USER`_TIME]&][s2; "
"&][s4;:`:`:Ctrl`:`:static Atom XA`_ENLIGHTENMENT`_DESKTOP:* [%00-00* static_Atom_][%00-00 "
"XA`_ENLIGHTENMENT`_DESKTOP]&][s2; &][s4;:`:`:Ctrl`:`:static Atom XA`_WM`_STATE:* "
"[%00-00* static_Atom_][%00-00 XA`_WM`")
TOPIC_TEXT(
"_STATE]&][s2; &][s4;:`:`:Ctrl`:`:static Atom XA`_UTF8`_STRING:* [%00-00* static_Atom_][%00-00 "
"XA`_UTF8`_STRING]&][s2; &][s4;:`:`:Ctrl`:`:static Atom XA`_CLIPBOARD:* [%00-00* static_Atom_][%00-00 "
"XA`_CLIPBOARD]&][s2; &][s4;:`:`:Ctrl`:`:static Atom XA`_CLIPDATA:* [%00-00* static_Atom_][%00-00 "
"XA`_CLIPDATA]&][s2; &][s4;:`:`:Ctrl`:`:static int Xeventtime:* [%00-00* static_int_][%00-00 "
"Xeventtime]&][s2")
TOPIC_TEXT(
"; &][s4;:`:`:Ctrl`:`:static Size Dsize:* [%00-00* static_Size_][%00-00 Dsize]&][s2; "
"&][s4;:`:`:Ctrl`:`:static Size Csize:* [%00-00* static_Size_][%00-00 Csize]&][s2; "
"&][s4;:`:`:Ctrl`:`:static int AutoBackPaintAreaSize:* [%00-00* static_int_][%00-00 "
"AutoBackPaintAreaSize]&][s2; &][s4;:`:`:Ctrl`:`:static int TransparentBackPaintAreaSize:* "
"[%00-00* static_int_][%00-00 TransparentBackPaintAreaSize]&][")
TOPIC_TEXT(
"s2; &][s4;:`:`:Ctrl`:`:static bool LogMessages:* [%00-00* static_bool_][%00-00 LogMessages]&][s2; "
"&][s4;:`:`:Ctrl`:`:static int ShowRepaint:* [%00-00* static_int_][%00-00 ShowRepaint]&][s2; "
"&][s4;:`:`:Ctrl`:`:static bool MemoryCheck:* [%00-00* static_bool_][%00-00 MemoryCheck]&][s2; "
"&][s4;:`:`:Ctrl`:`:static void `(`*`&GuiSleep`(`)`)`(int ms`): [%00-00 static_void_(`*`&][%00-00* "
"GuiSleep][%00-00 (")
TOPIC_TEXT(
"))(int_ms)]&][s2; &][s0; ]")
