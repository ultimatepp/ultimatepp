#include "DockCtrl/DockCtrl.h"

#define IMAGEFILE <DockCtrl/DockableCtrl.iml>
#define IMAGECLASS DockCtrlImages
#include <Draw/iml.h>

//===============================================
// DockCtrl class:
// Actual Framework.
//===============================================

DockCtrl::DockCtrl() : DockBase()
{
	ctrlinit = false;
	AllowAll();
}

DockCtrl::~DockCtrl()
{
	ctrlinit = false;
}

void DockCtrl::State(int reason)
{
	if(reason == Ctrl::OPEN)
	{
		if(!ctrlinit)
		{
			InitFrameWork();
			InitDockCtrl ();
			InitDefaultLayout();
			InitCustomLayouts();
			InitFrameWorkSettings();
			ctrlinit = true;
		}
	}
}

//===============================================
// DockBase class:
// A dockable widget framework base ctrl.
//===============================================

Ctrl& DockBase::Dock(DockableCtrl& ctrl)
{
    int alignment = ctrl.Alignment();
    // Register ctrl.
    ctrl.SetBase(this);
	AddCtrlRecord(ctrl);
	// Dock...
     switch(alignment)
   	{
       		case DockableCtrl::DOCK_LEFT:
          			GetPaneFrame(alignment).Attach(ctrl).Vert();
  		 		break;  
            case DockableCtrl::DOCK_TOP:
                    GetPaneFrame(alignment).Attach(ctrl).Horz();
                break;
            case DockableCtrl::DOCK_RIGHT:
                    GetPaneFrame(alignment).Attach(ctrl).Vert();
                break;
            case DockableCtrl::DOCK_BOTTOM:
                    GetPaneFrame(alignment).Attach(ctrl).Horz();
                break;
             default:
             	if(ctrl.IsFloating())
             	{
             		ctrl.CenterOwner();
             		ctrl.SetSize(panesize);
             		ctrl.FloatEx(Rect(0, 0, panesize.cx / 4, panesize.cy / 3));
             		ctrl.NoCenter();
             	}
                break;
   	}
	RefreshPanel();
	return *this;
}

DockableCtrl& DockBase::Tabify(DockableCtrl& ctrl1, DockableCtrl& ctrl2)
{
	// Register Ctrls.
	AddCtrlRecord(ctrl1);
	AddCtrlRecord(ctrl2);
	// By default (if not stated otherwise) create tabwindows from Left PaneFrame. 
	TabWindow* tabwindow = GetPaneFrame(0).AddTabWindow();
	ASSERT(tabwindow);
	tabwindow->Attach(ctrl1);
	tabwindow->Attach(ctrl2);
	return (DockableCtrl&) *tabwindow;	
}

DockableCtrl& DockBase::Tabify(String groupname, DockableCtrl& ctrl1, DockableCtrl& ctrl2)
{
	return (DockableCtrl&) dynamic_cast<TabWindow&>(Tabify(ctrl1, ctrl2)).SetGroup(groupname);
}

DockableCtrl& DockBase::Tabify(DockableCtrl& ctrl1, DockableCtrl& ctrl2, DockableCtrl& ctrl3, DockableCtrl& ctrl4)
{
	return Tabify(Tabify(ctrl1, ctrl2), Tabify(ctrl3, ctrl4));
}

DockableCtrl& DockBase::Tabify(String groupname, DockableCtrl& ctrl1, DockableCtrl& ctrl2, DockableCtrl& ctrl3, DockableCtrl& ctrl4)
{
	return (DockableCtrl&) dynamic_cast<TabWindow&>(Tabify(ctrl1, ctrl2, ctrl3, ctrl4)).SetGroup(groupname);
}

void DockBase::TabifyGroupAndDock(String groupname, int alignment)
{
	Dock(TabifyGroup(groupname).DockingStyle(alignment, DockableCtrl::STATE_SHOW));	
}

void DockBase::TabifyGroupAndAutoHide(String groupname, int alignment)
{
	Dock(TabifyGroup(groupname).DockingStyle(alignment, DockableCtrl::STATE_AUTO));	
}

void DockBase::TabifyGroupAndFloat(String groupname)
{
	Dock(TabifyGroup(groupname).DockFloat());	
}

DockableCtrl& DockBase::TabifyGroup(String name)
{
	DockableCtrl* ctrl = NULL;
	Vector<int>& ids = groups.Get(name);
	int ncount = ids.GetCount();
	ASSERT(ncount);
	TabWindow* tabwindow = GetPaneFrame(0).AddTabWindow();
	ASSERT(tabwindow);
	for(int i = 0; i < ncount; i++) tabwindow->Attach(*GetCtrlRecordFromId(ids[i])->ctrl);
	tabwindow->SetGroup(name);
	return (DockableCtrl&) *tabwindow;
}

DockBase& DockBase::WidgetGroup(String name)
{
	NewWidgetGroup(predefinedgroup = name);
	return *this;
}

DockBase& DockBase::operator%(DockableCtrl& ctrl)
{
	int id = GetCtrlRecord(ctrl)->id;
	if(!FindWidgetinGroup(predefinedgroup, id))	
		AddWidgettoGroup(predefinedgroup, id);
	return *this;
}

DockBase& DockBase::WidgetLayout(String name)
{
	NewWidgetLayout(predefinedgroup = name);
	return *this;
}

DockBase& DockBase::operator/(DockableCtrl& ctrl)
{
	return *this;
}

void DockBase::DoDragAndDrop(DockableCtrl& ctrl, Point p, Size sz)
{
 	for(int i = 0; i < 4; i++)
    {   
        for(int j = 0; j < 4; j++)
        {
         		if(i != j)
        		{	 
           	 		if(!GetPaneFrame(j).HasCtrlInRange(ctrl, p)) 
           	 		{   
           		   	 	GetPaneFrame(j).DnDSourceoutofRange(); 
               	 		GetPaneFrame(j).DnDAnimate();
            		}
        		}
        }
       	if(GetPaneFrame(i).HasCtrlInRange(ctrl, p)) 
       	{
      	 	ctrl.SetDropTarget(i, ctrl.GetDropState());
          	break;
       	}
    }
}

void DockBase::Attach(DockableCtrl& ctrl)
{
    GetPaneFrame(ctrl.Alignment()).Attach(ctrl);
}

void DockBase::Attach(DockableCtrl* ctrl)
{
    Attach(*ctrl);
}

void DockBase::Detach(DockableCtrl& ctrl)
{
    GetPaneFrame(ctrl.Alignment()).Detach(ctrl);
}

void DockBase::Detach(DockableCtrl* ctrl)
{
    Detach(*ctrl);
}

void DockBase::ShowLeftPane(bool b)
{
	if(b)	GetPaneFrame(PaneFrame::LEFT).ShowFrame();
	else	GetPaneFrame(PaneFrame::LEFT).HideFrame();
}

void DockBase::ShowTopPane(bool b)
{
	if(b)	GetPaneFrame(PaneFrame::TOP).ShowFrame();
	else	GetPaneFrame(PaneFrame::TOP).HideFrame();
}

void DockBase::ShowRightPane(bool b)
{
	if(b)	GetPaneFrame(PaneFrame::RIGHT).ShowFrame();
	else	GetPaneFrame(PaneFrame::RIGHT).HideFrame();
}

void DockBase::ShowBottomPane(bool b)
{
	if(b)	GetPaneFrame(PaneFrame::BOTTOM).ShowFrame();
	else	GetPaneFrame(PaneFrame::BOTTOM).HideFrame();
}

void DockBase::AllowAll()
{
	AllowedSides[0] = true;
	AllowedSides[1] = true;
	AllowedSides[2] = true;
	AllowedSides[3] = true;
}

void DockBase::AllowLeft()
{
	AllowedSides[0] = true;
	AllowedSides[1] = false;
	AllowedSides[2] = false;
	AllowedSides[3] = false;
}

void DockBase::AllowTop()
{
	AllowedSides[1] = true;
	AllowedSides[0] = false;
	AllowedSides[2] = false;
	AllowedSides[3] = false;
}

void DockBase::AllowRight()
{
	AllowedSides[2] = true;
	AllowedSides[0] = false;
	AllowedSides[1] = false;
	AllowedSides[3] = false;
}

void DockBase::AllowBottom()
{
	AllowedSides[3] = true;
	AllowedSides[0] = false;
	AllowedSides[1] = false;
	AllowedSides[2] = false;
}

void DockBase::AllowLeftRight()
{
	AllowedSides[0] = true;
	AllowedSides[2] = true;
	AllowedSides[1] = false;
	AllowedSides[3] = false;
}

void DockBase::AllowTopBottom()
{
	AllowedSides[1] = true;
	AllowedSides[3] = true;
	AllowedSides[0] = false;
	AllowedSides[2] = false;
}

void DockBase::AllowLeftTop()
{
	AllowedSides[0] = true;
	AllowedSides[1] = true;
	AllowedSides[2] = false;
	AllowedSides[3] = false;
}

void DockBase::AllowRightBottom()
{
	AllowedSides[2] = true;
	AllowedSides[3] = true;
	AllowedSides[0] = false;
	AllowedSides[1] = false;
}

void DockBase::AllowLeftBottom()
{
	AllowedSides[0] = true;
	AllowedSides[3] = true;
	AllowedSides[1] = false;
	AllowedSides[2] = false;
}

void DockBase::AllowTopRight()
{
	AllowedSides[1] = true;
	AllowedSides[2] = true;
	AllowedSides[0] = false;
	AllowedSides[3] = false;
}

void DockBase::AllowNestedTabbing(bool b)
{
	tabsnested = b;
	controlpanel.TabOptionNest <<= tabsnested;
}

void DockBase::ControlPanel()
{
    if(controlpanel.IsOpen()) return;
    controlpanel.Open(GetTopWindow());
   	RefreshGroups();
    RefreshPanel(); 
}

void DockBase::OnPanelAction()
{
    CtrlRecord* ctrlrecord = GetCtrlRecordFromIndex(listtab.list.GetClickRow());
    if(ctrlrecord) 
    	ctrlrecord->ctrl->Menu();
}

void DockBase::RefreshPanel()
{
    if(!controlpanel.IsOpen() || !controlpanel.IsVisible()) return;
    listtab.list.Clear();
    for(int i = 0; i < GetCtrlRecords().GetCount(); i++) 
    {
        int 	alignment 	= 0;
        bool 	tabbed		= false;
        bool	floating	= false;
		DockableCtrl* ctrl 	= GetCtrlRecordFromIndex(i)->ctrl;
		if(ctrl->IsTabbed())
		{
			TabWindow* tabwindow = dynamic_cast<DockWindow*>(ctrl)->GetOwnerTab();
			if(tabwindow) 
			{
				alignment = tabwindow->GetBaseTab()->Alignment();
				floating = tabwindow->GetBaseTab()->IsAutoHidden();
				tabbed = true;
			}
		}
		else alignment = ctrl->Alignment();
		String a, s;
        switch(alignment)
        {
			case DockableCtrl::DOCK_LEFT:
				a = t_("LEFT");
                break;
			case DockableCtrl::DOCK_TOP:
                a = t_("TOP");
            	break;
            case DockableCtrl::DOCK_RIGHT:
                a = t_("RIGHT");
                break;
            case DockableCtrl::DOCK_BOTTOM:
                a = t_("BOTTOM");
                break;
            case DockableCtrl::DOCK_NONE:
                if(ctrl->IsFloating() || tabbed)
                a = t_("FLOATING");
                else
                a = t_("NONE");  	
                break;     
		}
        switch(ctrl->State())
        {
            case DockableCtrl::STATE_SHOW:
                s = t_("SHOWN");
                break;
            case DockableCtrl::STATE_HIDE:
                s = t_("HIDDEN");
                break;
            case DockableCtrl::STATE_AUTO:
                s = t_("AUTO HIDDEN");
           		break;
            case DockableCtrl::STATE_SHUT:
                s = t_("SHUT");
                break;
            case DockableCtrl::STATE_TABBED:
            	floating ?
            	s = t_("AUTO HIDDEN")
            	:
                s = t_("TABBED");
                break;
        }
  
 		listtab.list.Add(0, Format("%s",ctrl->GetLabel()), a, s, ctrl->Position());
		listtab.list.SetCtrl(i, 0, (panelicons.At(i))->SetImage(ctrl->GetIcon().IsNullInstance() ? 
                   DockCtrlImages::DefaultImage : ctrl->GetIcon()));
      }
   controlpanel.Refresh();
}

void DockBase::Serialize(Stream& s)
{
	String 	layout	= "CTRLSLAYOUTS";
	String	group	= "CTRLGROUPS";
	String 	settings = "DOCKCTRLSETTINGS";
	int version = 1;	
	int n = 0;
	
	s.Magic(1234765);
	
	if(s.IsStoring())
	{
		s / version;
		SerializePlacement(s);
		s / settings;
		s % tabsicons;
		s % tabsclose;
		s % tabsautoalign;
		s % tabsnested;
		s % autohideicons;
		s % autohideclose;
		s / layoutindex;
		s / skinindex;
		s / hotkey;

		s / layout;
		//  Write Layouts.
		n = layouts.GetCount();
		s / n;
		for(int i = 0; i < n; i++)
		{
			String key = layouts.GetKey(i);
			String lay = layouts.Get(key);
			s / key;
			s / lay;
		}
		// Write Layout List.
		n = controlpanel.layoutlist.GetCount();
		s / n;
		for(int i = 0; i < n; i++)
		{
			String lname = controlpanel.layoutlist.GetKey(i);
			s / lname; 
		}
		
		s / group;
		// Write Groups.
		//int j = predefinedgroups + 1;
		n = groups.GetCount(); 
		s / n;
		for(int i = 0; i < n; i++)
		{
			String key 	= groups.GetKey(i);
			Vector<int>& ids = groups.Get(key);
			s / key;
			ids.Serialize(s);	
		}
	}
	else
	{
		n = 0;
		s / version;
		SerializePlacement(s);
		s / settings;
		s % tabsicons;
		s % tabsclose;
		s % tabsautoalign;
		s % tabsnested;
		s % autohideicons;
		s % autohideclose;
		s / layoutindex;
		s / skinindex;
		s / hotkey;
		
		s / layout;
		// Read Layouts.
		s / n;
		for(int i = 0; i < n; i++)
		{
			String key, lay;
			s / key;
			s / lay;
			layouts.FindAdd(key, lay);
		}
		// Read Layout List.
		s / n;
		for(int i = 0; i < n; i++)
		{
			String lname;
			s / lname;
			controlpanel.layoutlist.Add(lname);
		}
		s / group;
		// Read Groups.
		s / n;
		for(int i = 0; i < n; i++)
		{
			String key;
			Vector<int> ids;
			ids.Clear();
			s / key;
			ids.Serialize(s);
			groups.FindAdd(key, ids);
		}
		// Write Ctrl values.
		controlpanel.TabOptionIcon 		<<= tabsicons;
		controlpanel.TabOptionClose 	<<= tabsclose;
		controlpanel.TabOptionNest		<<= tabsnested;
		controlpanel.TabOptionAlignment <<= tabsautoalign;
		controlpanel.AutoOptionIcon 	<<= autohideicons;
		controlpanel.AutoOptionClose 	<<= autohideclose;
		
		controlpanel.layoutlist.SetIndex(layoutindex);
		controlpanel.skinlist.SetIndex(skinindex);
	}
	s.Magic();
}

void DockBase::InitFrameWorkSettings()
{
	RefreshWidgetLayout();
	SetSkin(skinindex);
	SetWidgetLayout(layoutindex);
}

void DockBase::InitDefaultLayout()
{
	String name = t_("Default");
	if(layouts.Find(name) < 0) NewWidgetLayout(name);
	NewWidgetGroup(name);
}

void DockBase::SetSkin(int index)
{
	switch(index)
	{
		case 0: style = &(DockCtrlChStyle::StyleDefault()); 	break;
		case 1:	style = &(DockCtrlChStyle::StyleClassic());		break;
		case 2: style = &(DockCtrlChStyle::StyleEnhanced());	break;
		default: break;
	}

	for(int i = 0; i < GetCtrlRecords().GetCount(); i++)
		dynamic_cast<DockWindow*>(GetCtrlRecordFromIndex(i)->ctrl)->SetStyle(*style);        
 	for(int i = 0; i < GetTabWindowCount(); i++)
 		GetTabWindowFromIndex(i)->SetStyle(*style);
}

DockBase& DockBase::SetStyle(const DockCtrlChStyle::Style& s)
{
    style = &s; 
    Refresh(); 
    return *this;   
}


void DockBase::OnSelectSkin()
{
	int newindex = controlpanel.skinlist.GetIndex();
	if(skinindex == newindex) return;
	SetSkin(skinindex = newindex);
}

void DockBase::OnSelectLayout()
{
	int newindex = controlpanel.layoutlist.GetIndex();
	if(layoutindex == newindex) return;
	SetWidgetLayout(layoutindex = newindex);
}

void DockBase::InitFrameWork()
{
	panesize = GetSize();
	int cx = panesize.cx / 4;
	int cy = panesize.cy / 3;
	
	AddFrame(hide[0]);
	AddFrame(hide[1]);
	AddFrame(hide[2]);
	AddFrame(hide[3]);

	hide[0].SetLayout(DockCtrlCustomFrame::LAYOUT_LEFT);
	hide[2].SetLayout(DockCtrlCustomFrame::LAYOUT_RIGHT);
	hide[1].SetLayout(DockCtrlCustomFrame::LAYOUT_TOP);
	hide[3].SetLayout(DockCtrlCustomFrame::LAYOUT_BOTTOM);	

	AddFrame(pane[0]);
	AddFrame(pane[2]);
	AddFrame(pane[1]);
	AddFrame(pane[3]);
	
	pane[0].SetLayout(this, PaneFrame::LEFT, cx);
	pane[2].SetLayout(this, PaneFrame::RIGHT, cx);
	pane[1].SetLayout(this, PaneFrame::TOP, cy);
	pane[3].SetLayout(this, PaneFrame::BOTTOM, cy);
}

DockBase::DockBase()
{
	skinindex =	layoutindex = 0;
	tabsicons =	tabsclose =	tabsautoalign = true;
	tabsnested = false;
	autohideicons =	autohideclose = true;
	isready = false;
    panesize.Clear();
    LeftPos(0, 0).TopPos(0,0);
	CtrlLayout(listtab);
	CtrlLayout(grouptab);
    CtrlLayout(controlpanel);
   	controlpanel.CenterScreen().Sizeable().Zoomable().Title(t_("Settings"));
    listtab.list.AddColumn();
    listtab.list.AddColumn(t_("Window Title"));
    listtab.list.AddColumn(t_("Alignment"));
    listtab.list.AddColumn(t_("State"));
    listtab.list.AddColumn(t_("Position"));
    listtab.list.EvenRowColor();
    listtab.list.SetLineCy(16);

	grouptab.grouptree.MultiSelect();
	controlpanel.panel.Add(listtab.SizePos(), t_("Status"));
	controlpanel.panel.Add(grouptab.SizePos(), t_("Group Manager"));

    controlpanel.skinlist.Add(0, String(t_("Default"))).Add(1, String(t_("Classic"))).Add(2, String(t_("Enhanced"))).SetIndex(skinindex);
    controlpanel.skinlist.WhenAction 	= THISBACK(OnSelectSkin);
    controlpanel.layoutlist.WhenAction	= THISBACK(OnSelectLayout);
    listtab.list.WhenLeftClick 			= THISBACK(OnPanelAction); 
    grouptab.grouptree.WhenDropInsert	= THISBACK(GroupDrop);
    grouptab.grouptree.WhenDrag			= THISBACK(GroupDrag);
    grouptab.grouptree.WhenBar			= THISBACK(GroupMenu);
	grouptab.grouptree.WhenCursor		= THISBACK(GroupSelect);
	   
	grouptab.groupadd					<<= THISBACK(OnAddNewGroup);
	grouptab.groupdelete				<<= THISBACK(OnDeleteGroup);
    controlpanel.layoutbutton			<<= THISBACK(ResetWidgetLayout);
	controlpanel.layoutadd				<<= THISBACK(OnAddNewLayout);
	controlpanel.layoutdelete			<<= THISBACK(OnDeleteLayout);
    controlpanel.TabOptionAlignment		<<= THISBACK(RefreshWidgetLayout);
    controlpanel.TabOptionIcon			<<= THISBACK(RefreshWidgetLayout);
    controlpanel.TabOptionClose			<<= THISBACK(RefreshWidgetLayout);
    controlpanel.AutoOptionClose		<<= THISBACK(RefreshWidgetLayout);
    controlpanel.AutoOptionIcon			<<= THISBACK(RefreshWidgetLayout);

	controlpanel.TabOptionIcon 			<<= tabsicons;
	controlpanel.TabOptionClose 		<<= tabsclose;
	controlpanel.TabOptionNest			<<= tabsnested;
	controlpanel.TabOptionAlignment	 	<<= tabsautoalign;

	controlpanel.AutoOptionIcon 		<<= autohideicons;
	controlpanel.AutoOptionClose 		<<= autohideclose;
	
	grouptab.groupdelete.Disable();
	SetKey(K_CTRL_HOME);
}

DockBase::~DockBase()
{
 	CleanUp();
    panesize.Clear();
}

void DockBase::CleanUp()
{
	for(int i = 0; i < panelicons.GetCount(); i++)
		delete panelicons.At(i);
	for(int i = 0; i < layouts.GetCount(); i++)
		layouts.Remove(i);
	for(int i = 0; i < groups.GetCount(); i++)
		groups.Remove(i);
 	RemoveTabWindows();
    DeleteCtrlRecords();	
}

void DockBase::RefreshWidgetLayout()
{
	TabWindow* tabwindow = NULL;

	tabsicons 		= (bool) controlpanel.TabOptionIcon.Get();
	tabsclose		= (bool) controlpanel.TabOptionClose.Get();
	tabsnested		= (bool) controlpanel.TabOptionNest.Get();
	tabsautoalign	= (bool) controlpanel.TabOptionAlignment.Get();
	autohideicons	= (bool) controlpanel.AutoOptionIcon.Get();
	autohideclose	= (bool) controlpanel.AutoOptionClose.Get();	

	int n = GetTabWindowCount();
	for(int i = 0; i < n; i++)
	{
		tabwindow = GetTabWindowFromIndex(i);
		if(tabsautoalign)
		{
			switch(tabwindow->GetBaseTab()->Alignment())
			{
       			case DockableCtrl::DOCK_LEFT:
					tabwindow->SetLayout(DockCtrlCustomFrame::LAYOUT_RIGHT);  
       				break;
  	     		case DockableCtrl::DOCK_TOP:
					tabwindow->SetLayout(DockCtrlCustomFrame::LAYOUT_BOTTOM);   
       				break;
    	   		case DockableCtrl::DOCK_RIGHT:
					tabwindow->SetLayout(DockCtrlCustomFrame::LAYOUT_LEFT);  
      	 			break;
    	   		case DockableCtrl::DOCK_BOTTOM:
					tabwindow->SetLayout(DockCtrlCustomFrame::LAYOUT_TOP);  
       				break;   
			}
		}
		else tabwindow->SetLayout(DockCtrlCustomFrame::LAYOUT_BOTTOM);
		tabwindow->GetTabs().HasIcons(tabsicons);
   		tabwindow->GetTabs().HasButtons(tabsclose);		
	}
	for(int i = 0; i < 4; i++) GetHideBar(i).HasButtons(autohideclose).HasIcons(autohideicons);
}

void DockBase::OnAddNewLayout()
{
	String name;
	if(EditText(name, t_("Add new layout"), t_("Layout name:"), 32))
	{
		if(IsNull(name)) 
		{
			Exclamation(t_("You must enter a name for the new layout"));
			return;
		}
		else if(layouts.Find(name) >= 0)
		{
			if(!PromptOKCancel(Format(t_("Layout '%s' adready exists. Do you want to overwrite it?"), name)))
			return;
		}
		NewWidgetLayout(name);
		RefreshPanel();
	}
}


void DockBase::OnDeleteLayout()
{
	DelWidgetLayout(layouts.GetKey(layoutindex));	
}

void DockBase::AddCtrlRecord(DockableCtrl& ctrl)
{
	CtrlRecord* record 	= NULL;
	if(GetCtrlRecord(ctrl) || ctrl.IsTabWindow()) return;
	ctrls.Add((record = new CtrlRecord()));
	if(record) 
	{
		record->ctrl = &ctrl;
		record->id	 = ctrls.GetCount();
		ImageCtrl* imagectrl = new ImageCtrl();
    	panelicons.Add(imagectrl);
 	}
}

void DockBase::RemoveCtrlRecord(DockableCtrl& ctrl)
{
	CtrlRecord* record = GetCtrlRecord(ctrl);
	if(record) delete record;
}

void DockBase::DeleteCtrlRecords()
{
	for(int i = 0; i < ctrls.GetCount(); i++)
		delete ctrls.At(i);
}

DockBase::CtrlRecord* DockBase::GetCtrlRecord(DockableCtrl& ctrl)
{
	for(int i = 0; i < ctrls.GetCount(); i++)
	{
		CtrlRecord* record = ctrls.At(i);
		if(record->ctrl == &ctrl) return record;
	}	   
	return NULL;
}

DockBase::CtrlRecord* DockBase::GetCtrlRecordFromIndex(int index)
{
	if(index >= 0 && index < ctrls.GetCount())
		return ctrls.At(index);
	return NULL;
}

DockBase::CtrlRecord* DockBase::GetCtrlRecordFromId(int id)
{
	for(int i = 0; i < ctrls.GetCount(); i++)
	{
		CtrlRecord* record = ctrls.At(i);
		if(record->id == id) return record;
	}
	return NULL;
}

DockBase::CtrlRecord* DockBase::GetCtrlRecordFromGroup(String group)
{
	return NULL;
}

DockableCtrl* DockBase::GetDockedWindowFromIndex(int index)
{
	Vector<DockableCtrl*> docks;
	docks.Clear();
	for(int i = 0; i < 8; i++)
	{
		if(i < 4) 
		{
			int n = GetPaneFrame(i).GetCount();
			if(n) for(int j = 0; j < n; j++) docks.Add(GetPaneFrame(i).GetChild(j + 1));
		}
		else
		{
			int n = GetHideBar(i - 4).GetCount();
			if(n) for(int j = 0; j < n; j++) docks.Add(GetHideBar(i - 4).GetChild(j));
		}
	}
	return docks.GetCount() ? docks.At(index) : NULL;
}

TabWindow* DockBase::GetTabWindowFromIndex(int index)
{
	TabWindow* tabwindow = tabwindows.At(index);
	ASSERT(tabwindow);
	return tabwindow;
}

int DockBase::GetTabWindowCount()
{
	return tabwindows.GetCount();
}

int DockBase::GetDockedWindowCount()
{
	int dockcount = 0;
	for(int i = 0; i < 8; i++)
		if(i < 4) dockcount += GetPaneFrame(i).GetCount();
		else dockcount += GetHideBar(i - 4).GetCount();
	return dockcount;
}

void DockBase::RemoveTabWindows()
{
	for(int i = 0 ; i < tabwindows.GetCount(); i++)
		if(tabwindows.At(i)) delete tabwindows.At(i);	
}

void DockBase::RefreshTabWindowList()
{
	for(int i = 0; i < tabwindows.GetCount(); i++)
		if(tabwindows.At(i)->IsDestroyed())
		{
			TabWindow *t = tabwindows.At(i);
			tabwindows.Remove(i);
			delete t;
		}
}

Vector<TabWindow*>& DockBase::GetTabWindows()
{
	return tabwindows;
}

void DockBase::NewWidgetLayout(String name)
{
	StringStream s;
	SerializeLayout(s);
	int id = AddWidgetLayout(name);
	layouts[id] = (String) s;
}

int DockBase::AddWidgetLayout(String name)
{
	int id = layouts.Find(name);
	if(id < 0)
	{
		id = layouts.FindAdd(name, String());
		controlpanel.layoutlist.Add(name);
		int index = controlpanel.layoutlist.GetCount() - 1;
		controlpanel.layoutlist.SetIndex(layoutindex = index);		
	}
	return id;
}

void DockBase::AddWidgetLayout(String name, DockableCtrl& ctrl, int alignment, int state, int position)
{
	
}

void DockBase::DelWidgetLayout(String name)
{
	int id = layouts.Find(name);
	if(id < 0) return;
	if(PromptOKCancel(Format(t_("Do you really want to delete layout '%s'"), name)))
	{
		layouts.RemoveKey(name);
		controlpanel.layoutlist.Remove(id);
		id--;
		controlpanel.layoutlist.SetIndex(layoutindex = id);
	}
}

void DockBase::SetWidgetLayout(String name)
{
	int id = layouts.Find(name);
	if(id < 0) return;
	SetWidgetLayout(id);
}

void DockBase::SetWidgetLayout(int index)
{
	if(layoutindex != index) controlpanel.layoutlist.SetIndex(layoutindex = index);
	if(index == 0) 	controlpanel.layoutdelete.Disable();
	else controlpanel.layoutdelete.Enable();
	
	String key = layouts.GetKey(index);
	String& layout = layouts.Get(key);
	StringStream s(layout);
	s.SetLoading();
	SerializeLayout(s);
}

void DockBase::SerializeLayout(Stream& s, bool deflay)
{
	CtrlRecord* record	= NULL;
	DockableCtrl* ctrl	= NULL;
	TreeCtrl& tree		= grouptab.grouptree;	
	String  name;
	int		panesize	= 0;
	int 	type 		= 0;
	int 	alignment	= 0;
	int 	state		= 0;
	int 	position	= 0;
	int 	id			= 0;
	int 	childcount	= 0;
	Size	childsize;
	
		if(s.IsStoring())
		{
			// Remove unused TabWindows (BugFix).
			RefreshTabWindowList();
			// Write Tabbing mode.
			s % tabsnested;
			// Write Widgets.
			// Write Docked (shown or hidden) and AutoHidden widgets.
			childcount = GetDockedWindowCount();
			s / childcount;
			if(childcount)
				for(int i = 0; i < childcount; i++)
				{
					ctrl = GetDockedWindowFromIndex(i);
					ASSERT(ctrl);
					ctrl->Serialize(s);
				}
			// Write PaneFrame sizes.
			for(int i = 0; i < 4; i++)  
			{
				panesize = GetPaneFrame(i).GetSize();
				s / panesize;
			}
			childcount = 0;
			int ctrlscount = ctrls.GetCount();
			// Write Floating Dockwindows.
			for(int i = 0; i < ctrlscount * 2; i++)
			{
				if(i < ctrlscount) childcount += GetCtrlRecordFromIndex(i)->ctrl->IsFloating() ? 1 : 0;
				if(i >= ctrlscount) 
				{
					if(i == ctrlscount) s / childcount;
					ctrl = GetCtrlRecordFromIndex(i - ctrlscount)->ctrl;
					ASSERT(ctrl);
					if(ctrl->IsFloating()) ctrl->Serialize(s);
				}
			}
			childcount = 0;
			// Write Floating TabWindows.
			int tabwindowcount = GetTabWindowCount();
			if(tabwindowcount)
				for(int i = 0; i < tabwindowcount * 2; i++)
				{
					if(i <  tabwindowcount) childcount += GetTabWindowFromIndex(i)->IsFloating() ? 1 : 0;
					if(i >= tabwindowcount)
					{
						if(i == tabwindowcount) s / childcount;
						TabWindow* tabwindow = GetTabWindowFromIndex(i - tabwindowcount);
						ASSERT(tabwindow);
						if(tabwindow->IsFloating()) tabwindow->Serialize(s);
					}
				}
		}

		if(s.IsLoading())
		{
			childcount = 0;
			// Close All widgets.
			for(int i = 0; i < GetTabWindowCount(); i++)	GetTabWindowFromIndex(i)->DetachAll();
			for(int i = 0; i < ctrls.GetCount(); i++) 		GetCtrlRecordFromIndex(i)->ctrl->Shut();
			// Remove unused TabWindows (BugFix).
			RefreshTabWindowList();
			// Read Tabbing Mode
			s % tabsnested;
			controlpanel.TabOptionNest <<= tabsnested;
			// Read Docked (shown or hidden) and AutoHidden widgets.
			s / childcount;
			if(childcount)
				for(int i = 0; i < childcount; i++)
				{
					s / type / id;
					if(type == DockableCtrl::TYPE_DOCKWINDOW)
						GetCtrlRecordFromId(id)->ctrl->Serialize(s);
					if(type == DockableCtrl::TYPE_TABWINDOW)
						GetPaneFrame(0).AddTabWindow()->Serialize(s);
				}
			// Read PaneFrame sizes.
			for(int i = 0; i < 4; i++)
			{
				s / panesize;
				GetPaneFrame(i).SetSize(panesize);
			}
			childcount = 0;
			s / childcount;
			if(childcount)
				for(int i = 0; i < childcount; i++)
				{
					s / type / id;
					GetCtrlRecordFromId(id)->ctrl->Serialize(s);
				}
			
			childcount = 0;
			s / childcount;
			if(childcount)
				for(int j = 0; j < childcount; j++)
				{
					s / type / id;
					TabWindow* tabwindow = GetPaneFrame(0).AddTabWindow();
					tabwindow->Serialize(s);
				}
		}		
		s.Close();
}

void DockBase::ResetWidgetLayout()
{
	controlpanel.layoutlist.SetIndex(layoutindex = 0);
	SetWidgetLayout(layoutindex);
	RefreshPanel();
}

void DockBase::NewWidgetGroup(String name, bool predefined)
{
	predefined  = true;
	TreeCtrl& tree = grouptab.grouptree;
	Vector<int> ids;
	ids.Clear();
	int group = groups.FindAdd(name, ids);
	if(tree.Find(name) < 0) tree.Add(0, DockCtrlImages::DefaultImage(), Value(name));

	if(name != t_("Default")) return; 
	for(int i = 0; i < ctrls.GetCount(); i++)
	{
		DockableCtrl * ctrl = ctrls[i]->ctrl;
		int id	= ctrls[i]->id;
		ids.Add(ctrls[i]->id);
		tree.Add(tree.Find(name), ctrl->GetIcon(), Value(id), Value(ctrl->GetLabel()));
	}
	groups[group] = ids;
}

bool DockBase::AddWidgettoGroup(String name, DockableCtrl& ctrl)
{
	CtrlRecord* record = GetCtrlRecord(ctrl);
	if(!record || name == t_("Default")) return false;
	NewWidgetGroup(name);
	Vector<int>& ids = groups.Get(name);
	for(int i = 0; i < ids.GetCount(); i++)
	if(ids[i] == record->id)
	{
		Exclamation(Format(t_("Error: Widget (%s) is already in the group '%s'"), ctrl.GetLabel(), name));
		return false;
	}
	ids.Add(record->id);
	grouptab.grouptree.Add(grouptab.grouptree.Find(name), ctrl.GetIcon(), Value(record->id), Value(ctrl.GetLabel()));
	return true;
}

bool DockBase::AddWidgettoGroup(String name, int id)
{
	CtrlRecord* record = GetCtrlRecordFromId(id);
	if(!record) return false;
	return AddWidgettoGroup(name, *record->ctrl);
}

bool DockBase::DeleteWidgetfromGroup(String name, DockableCtrl& ctrl)
{
	CtrlRecord* record = GetCtrlRecord(ctrl);
	if(groups.Find(name) < 0 || name == t_("Default") || !record) return false;
	Vector<int>& ids = groups.Get(name);
	for(int i = 0; i < ids.GetCount(); i++) if(record->id == ids[i]) ids.Remove(i);
	return true;
}

bool DockBase::DeleteWidgetfromGroup(String name, int id)
{
	CtrlRecord* record = GetCtrlRecordFromId(id);
	if(!record) return false;
	return DeleteWidgetfromGroup(name, *record->ctrl);	
}

int DockBase::AddWidgetGroup(String name)
{
	return 1;
}

void DockBase::DeleteWidgetGroup(String name)
{
	TreeCtrl& tree = grouptab.grouptree;
	if(name == t_("Default")) return;
	if(PromptOKCancel(Format(t_("Do you really want to delete group '%s'"), name)))
	{
		groups.RemoveKey(name);
		int id = tree.Find(name);
		if(id >= 0) 
		{
			tree.RemoveChildren(id);
			tree.Remove(id);
		}
		tree.Refresh();
	}
	
}

Vector<int>& DockBase::FindWidgetGroup(String name)
{
	return groups.Get(name);
}

bool DockBase::FindWidgetinGroup(String name, int id)
{
	Vector<int>& ids = FindWidgetGroup(name);
	for(int i = 0; i < ids.GetCount(); i++)
		if(ids[i] == id) return true;
	return false;
}

void DockBase::OnAddNewGroup()
{
	String name;
	if(EditText(name, t_("Add new group"), t_("Group name:"), 32))
	{
		if(IsNull(name)) 
		{
			Exclamation(t_("You must enter a name for the new group"));
			return;
		}
		else if(groups.Find(name) >= 0)
		{
			if(!PromptOKCancel(Format(t_("Group '%s' adready exists. Do you want to overwrite it?"), name)))
			return;
		}
		NewWidgetGroup(name);
		RefreshPanel();
	}
}

void DockBase::OnDeleteGroup()
{
	TreeCtrl& tree = grouptab.grouptree;
	int id = tree.GetCursor();
	if(id == 0) return;
	DeleteWidgetGroup(tree.GetValue(tree.GetCursor()));
}

void DockBase::GroupDrop(int parent, int ii, PasteClip& d)
{
	if (parent == 0) return d.Reject();

	TreeCtrl& tree = grouptab.grouptree;
	int p = tree.GetParent(parent);
	if (p != 0)	parent = p;
	
	if(AcceptInternal<TreeCtrl>(d, "DockCtrlGroupTreeDrag")) 
	{
		Vector<int> sel = tree.GetSel();
		for (int i = 0; i < sel.GetCount(); i++) 
		{

			int id = tree.Get(sel[i]);
			String group = (String) tree.GetValue(parent);
			if(group == t_("Default") || FindWidgetinGroup(group, id)) 
				return d.Reject();
			AddWidgettoGroup(group, id);
			tree.Open(parent);
		}
		return;
	}
	tree.SetFocus();
}

void DockBase::GroupDrag()
{
	TreeCtrl& tree = grouptab.grouptree;
	int idc = -1;
	if (tree.GetCursor() >= 0 && tree.GetParent(tree.GetCursor()) <= 0)
		return;
	if(tree.DoDragAndDrop(InternalClip(tree, "DockCtrlGroupTreeDrag"), tree.GetDragSample()) == DND_MOVE)
	{
		int parent = tree.GetParent(tree.GetCursor());
		idc = tree.GetCursor();
		String name = tree.GetValue(idc);
		String group = (String) tree.GetValue(parent);
		int id = tree.Get(idc);
		if(DeleteWidgetfromGroup(group, id)) tree.Remove(idc);
	}
	tree.Refresh();
}

void DockBase::GroupSelect()
{
	TreeCtrl& tree = grouptab.grouptree;
	int root = tree.GetCursor();
	if(root < 0) return;
	int id = tree.GetParent(root);
	String group = (String) tree.GetValue(root);
	grouptab.groupdelete.Enable(id == 0 && group != t_("Default"));
}

void DockBase::RefreshGroups()
{
	TreeCtrl& tree = grouptab.grouptree;
	tree.Clear();
	tree.SetRoot(DockCtrlImages::DefaultImage(), t_("Available Widget Groups"));
	tree.Open(0);
	for(int i = 0; i < groups.GetCount(); i++)
	{
		String group = groups.GetKey(i);
		Vector<int>& ids = groups.Get(group);
		tree.Add(0, DockCtrlImages::DefaultImage(), group);
		for(int j = 0; j < ids.GetCount(); j++)
		{	
			CtrlRecord* record = GetCtrlRecordFromId(ids[j]);
			ASSERT(record);
			tree.Add(tree.Find(group), record->ctrl->GetIcon(), Value(record->id), Value(record->ctrl->GetLabel()));		
		}
	}
}

void DockBase::GroupMenu(Bar& bar)
{
	TreeCtrl& tree = grouptab.grouptree;
	int id = tree.GetCursor();
	if(id < 0) return;
	id = tree.GetParent(id);
	activegroup = (String) tree.GetValue(tree.GetCursor());
	if(id >= 0)
	{
		if(id == 0 && tree.GetChildCount(id))
		{
			bar.Add(Format(t_("Dock Group '%s'"), activegroup), THISBACK1(GroupDockMenu, 0));
			bar.Add(Format(t_("AutoHide Group '%s'"), activegroup), THISBACK1(GroupDockMenu, 1));
			bar.Add(Format(t_("Float Group '%s'"), activegroup), THISBACK3(GroupMenuAction, activegroup, 2, -1));
			bar.Separator();
			bar.Add(tree.GetChildCount(tree.GetCursor()) > 1, Format(t_("Tabify Group '%s' and..."), activegroup), THISBACK1(GroupTabifyMenu, 3));
			bar.Separator();
			bar.Add(activegroup != t_("Default"), Format(t_("Delete Group '%s'"), activegroup), THISBACK1(DeleteWidgetGroup, activegroup));
			bar.Separator();
			bar.Add(Format(t_("Close Group '%s'"), activegroup), DockCtrlImages::CClose(), THISBACK3(GroupMenuAction, activegroup, -1, -1));
		
		}
		if(id > 0)
		{
			Vector<int> sel = tree.GetSel();
			for(int i = 0; i < sel.GetCount(); i++)	id = tree.Get(sel[i]);
			CtrlRecord* record = GetCtrlRecordFromId(id);
			ASSERT(record);
			record->ctrl->Menu();
		}
	}
	else bar.Add(t_("Add New Group..."), THISBACK(OnAddNewGroup));
}

void DockBase::GroupDockMenu(Bar& bar, int command)
{
	if(IsSideAllowed(DockableCtrl::DOCK_TOP)) bar.Add(t_("Top"), THISBACK3(GroupMenuAction, activegroup, (int) command, (int)DockableCtrl::DOCK_TOP));
	if(IsSideAllowed(DockableCtrl::DOCK_LEFT)) bar.Add(t_("Left"), THISBACK3(GroupMenuAction, activegroup, (int) command, (int)DockableCtrl::DOCK_LEFT));
	if(IsSideAllowed(DockableCtrl::DOCK_RIGHT)) bar.Add(t_("Right"), THISBACK3(GroupMenuAction, activegroup, (int) command, (int)DockableCtrl::DOCK_RIGHT));
	if(IsSideAllowed(DockableCtrl::DOCK_BOTTOM)) bar.Add(t_("Bottom"), THISBACK3(GroupMenuAction, activegroup, (int) command, (int)DockableCtrl::DOCK_BOTTOM));
}

void DockBase::GroupTabifyMenu(Bar& bar, int command)
{
	bar.Add(t_("Dock"), THISBACK1(GroupDockMenu, 3));
	bar.Add(t_("Auto Hide"), THISBACK1(GroupDockMenu, 4));
	bar.Add(t_("Float"), THISBACK3(GroupMenuAction, activegroup, 5, -1));
}

void DockBase::GroupMenuAction(String name, int command, int alignment)
{
	DockableCtrl* ctrl = NULL;
	Vector<int>& ids = groups.Get(name);
	int ncount = ids.GetCount();

	bool aligned = (alignment >= DockableCtrl::DOCK_LEFT && alignment <= DockableCtrl::DOCK_BOTTOM);

	for(int i = 0; i < ncount * 2; i++)
	{
		if(i < ncount)
		{
			ctrl = GetCtrlRecordFromId(ids[i])->ctrl;
			ASSERT(ctrl);
			ctrl->Shut();
		}
		else
		{
			if(i == ncount) RefreshTabWindowList();
			ctrl = GetCtrlRecordFromId(ids[i - ncount])->ctrl;
			ASSERT(ctrl);

			switch(command)
			{
				case 0: Dock(ctrl->DockingStyle(alignment, DockableCtrl::STATE_SHOW)); break;
				case 1: Dock(ctrl->SetSizeHint(Size(0,0)).DockingStyle(alignment, DockableCtrl::STATE_AUTO)); break;
				case 2: Dock(ctrl->DockFloat()); break;
				case 3:	if (i - ncount == 0) TabifyGroupAndDock(name, alignment); break;
				case 4: if (i - ncount == 0) TabifyGroupAndAutoHide(name, alignment); break;
				case 5: if (i - ncount == 0) TabifyGroupAndFloat(name); break;
			}
		}
	}
}

bool DockBase::Key(dword key, int count)
{
	if(key == hotkey) 
	 	ControlPanel();
	 return TopWindow::Key(key, count);
}

CH_STYLE(DockCtrlChStyle, Style, StyleClassic)
{
    font        = StdFont();
    barmargins  = Rect(2,2,2,2);
    barheight   = font.Info().GetHeight() + 8;  
    tabheight   = font.Info().GetHeight() + 8;
    tabmargin   = 2;
    tabsel      = Rect(2, 2, 2, 2);
    tabedge     = Rect(6, 6, 6, 6);
    tabextendleft = 0;
    
    DockCtrlImgsLook(barbackground, DockCtrlImages::I_CBarBackground, 1);
    DockCtrlImgsLook(barshut, DockCtrlImages::I_CClose, 3);
    DockCtrlImgsLook(barhide, DockCtrlImages::I_CHide, 3);
    DockCtrlImgsLook(barmenu, DockCtrlImages::I_CMenu, 3);

 
    CtrlsImageLook(tabnormal, ClassicCtrlsImg::I_TAB, 4);
    CtrlsImageLook(tabfirst, ClassicCtrlsImg::I_FTAB, 4);
    CtrlsImageLook(tablast, ClassicCtrlsImg::I_LTAB, 4);
    CtrlsImageLook(tabboth, ClassicCtrlsImg::I_BTAB, 4);        
}

CH_STYLE(DockCtrlChStyle, Style, StyleEnhanced)
{
    font        = StdFont();
    barmargins  = Rect(2,2,2,2);
    barheight   = font.Info().GetHeight() + 8;  
    tabheight   = font.Info().GetHeight() + 8;
    tabmargin   = 2;
    tabsel      = Rect(2, 2, 2, 2);
    tabedge     = Rect(6, 6, 6, 6);
    tabextendleft = 0;

   	const ToolBar::Style* defguistyle = &ToolBar::StyleDefault();  
	barbackground[0] = defguistyle->arealook;

    DockCtrlImgsLook(barshut, DockCtrlImages::I_CClose, 3);
    DockCtrlImgsLook(barhide, DockCtrlImages::I_CHide, 3);
    DockCtrlImgsLook(barmenu, DockCtrlImages::I_CMenu, 3);

    CtrlsImageLook(tabnormal, CtrlsImg::I_TAB, 4);
    CtrlsImageLook(tabfirst, CtrlsImg::I_FTAB, 4);
    CtrlsImageLook(tablast, CtrlsImg::I_LTAB, 4);
    CtrlsImageLook(tabboth, CtrlsImg::I_BTAB, 4);      

	stdhighlight = 	StandardHighlight(Blend(SColorHighlight, SColorPaper, 90), SColorHighlight);
	tabhighlight = AlphaHighlight(stdhighlight, 80);     

}

CH_STYLE(DockCtrlChStyle, Style, StyleDefault)
{
    font        = StdFont();
    barmargins  = Rect(2,2,2,2);
    barheight   = font.Info().GetHeight() + 8;  
    tabheight   = font.Info().GetHeight() + 8;
    tabmargin   = 2;
    tabsel      = Rect(2, 2, 2, 2);
    tabedge     = Rect(6, 6, 6, 6);
    tabextendleft = 0;
    
    const TabCtrl::Style* defguistyle = &TabCtrl::StyleDefault();  
	barbackground[0] =  defguistyle->normal[0];

	DockCtrlImgsLook(barshut, DockCtrlImages::I_DClose, 3);
    DockCtrlImgsLook(barhide, DockCtrlImages::I_DHide, 3);
    DockCtrlImgsLook(barmenu, DockCtrlImages::I_DMenu, 3);
  
    CtrlsImageLook(tabnormal, CtrlsImg::I_TAB, 4);
    CtrlsImageLook(tabfirst, CtrlsImg::I_FTAB, 4);
    CtrlsImageLook(tablast, CtrlsImg::I_LTAB, 4);
    CtrlsImageLook(tabboth, CtrlsImg::I_BTAB, 4);  


	stdhighlight = 	StandardHighlight(Blend(SColorHighlight, SColorPaper, 90), SColorHighlight);
	tabhighlight = AlphaHighlight(stdhighlight, 80);     
	
}
