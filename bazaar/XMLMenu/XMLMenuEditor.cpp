#include "XMLMenuEditor.h"

#include <plugin/bmp/bmp.h>

NAMESPACE_UPP

// colored Display
class ColDisp : public Display
{
		Color color;
	public :
		ColDisp(Color const &c) { color = c; }
		virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
			{ Display::Paint(w, r, q, color, paper, style); }
};
static ColDisp RedDisp(Red());

////////////////////////////////////////////////////////////////////////////////
// constructor
XMLBarEditor::XMLBarEditor()
{
	bar = NULL;
	curIcon = Null;
	itemSize = itemPane.GetLayoutSize();
	CtrlLayout(itemPane);
	Add(vertSplitter);
	vertSplitter.Vert(barTree, itemPane);
	
	// drag'n drop
	barTree.WhenDropInsert = THISBACK(dropInsertCb);
	barTree.WhenDrag = THISBACK(dragCb);
	barTree.WhenSel  = THISBACK(itemSelCb);
	barTree.WhenBar = THISBACK(treeContextCb);
	
	itemPane.cmdId		<<= THISBACK(fieldsModCb);
	itemPane.label		<<= THISBACK(fieldsModCb);
	itemPane.tooltip	<<= THISBACK(fieldsModCb);
	
	itemPane.icon		<<= THISBACK(imageSelCb);
	
	itemPane.Disable();
}

// gets minimum size of bar editor
Size XMLBarEditor::GetMinSize(void)
{
	return Size(itemSize.cx, itemSize.cy * 2);
}

// layouts control
void XMLBarEditor::Layout(void)
{
	int h = GetSize().cy;
	if(h)
		vertSplitter.SetPos(10000 * (h - itemSize.cy) / h);
}
		
// refresh current bar
void XMLBarEditor::RefreshBar(int treeRoot, XMLToolBar *subBar)
{
	if(!subBar)
		subBar = bar;
	if(!subBar)
		return;
	subBar->items.Clear();
	for(int iChild = 0; iChild < barTree.GetChildCount(treeRoot); iChild++)
	{
		int iNode = barTree.GetChild(treeRoot, iChild);
		XMLToolBarItem const &item = ValueTo<XMLToolBarItem>(barTree.Get(iNode));
		subBar->items.Add(new XMLToolBarItem(item, 0));
		if(barTree.GetChildCount(iNode))
		{
			subBar->items.Top().subMenu = new XMLToolBar;
			RefreshBar(iNode, ~subBar->items.Top().subMenu);
		}
		else
			subBar->items.Top().subMenu.Clear();
	}
}

// sets bar being edited
void XMLBarEditor::SetBar(XMLToolBar *_bar)
{
	// if changing bar, update current one if not null
	// before changing it
	if(bar && bar != _bar)
		RefreshBar();
	
	bar = _bar;
	barTree.Clear();
	if(!bar)
		return;
	buildTree(0, bar);
	barTree.SetRoot(Null, bar->GetName());
	barTree.OpenDeep(0);
}

// builds bar tree
void XMLBarEditor::buildTree(int root, XMLToolBar const *bar)
{
	Array<XMLToolBarItem> const &items = bar->GetItems();
	for(int iItem = 0; iItem < items.GetCount(); iItem++)
	{
		XMLToolBarItem const &item = items[iItem];
		TreeCtrl::Node node(item.GetIcon(), RawDeepToValue(item), (item.IsSeparator() ? String(t_("<SEPARATOR>")) : item.GetLabel()));
		int iNode = barTree.Add(root, node);
		if(items[iItem].IsSubMenu())
			buildTree(iNode, &items[iItem].GetSubMenu());
	}
}

// dragging element
void XMLBarEditor::dragCb(void)
{
	if(barTree.DoDragAndDrop(InternalClip(barTree, "mytreedrag"), barTree.GetDragSample()) == DND_MOVE)
		barTree.RemoveSelection();
}

// dropping between elements (inserts between)
void XMLBarEditor::dropInsertCb(int parent, int ii, PasteClip& d)
{
	barTree.AdjustAction(parent, d);
	if(AcceptInternal<TreeCtrl>(d, "mytreedrag"))
	{
		barTree.InsertDrop(parent, ii, d);
		barTree.SetFocus();
		return;
	}
	if(AcceptText(d))
	{
		barTree.SetCursor(barTree.Insert(parent, ii, Image(), GetString(d)));
		barTree.SetFocus();
		return;
	}
}
		
// item selection callback
void XMLBarEditor::itemSelCb()
{
	// get selected node
	int i = barTree.GetCursor();

	// if none, just disable input fields and return
	if(i < 0)
	{
		itemPane.Disable();
		return;
	}

	// enable input pane
	itemPane.Enable();
	
	// enable label and tooltip fields
	itemPane.label.Enable();

	// disable command field, it'll be filled
	// by command bar anyways
	itemPane.cmdId.Disable();
	
	// if not the root, treat as an item
	// otherwise allow just to edit bar name
	if(i > 0)
	{
		// fetch node data
		XMLToolBarItem const &item = ValueTo<XMLToolBarItem>(barTree.Get(i));
		
		if(item.IsSeparator())
		{
			itemPane.icon.Disable();
			itemPane.tooltip.Disable();
			itemPane.label.Disable();
			itemPane.cmdId.Clear();
			itemPane.labelName = t_("Type :");;
			itemPane.label	= String(t_("<SEPARATOR>"));
			itemPane.tooltip.Clear();
			itemPane.icon.SetImage(Null);
			curIcon = Null;
		}
		else
		{
			itemPane.icon.Enable();
			itemPane.tooltip.Enable();
			itemPane.label.Enable();
			
			bool hasChilds = barTree.GetChildCount(i);
		
			// fill fields
			if(hasChilds)
				itemPane.cmdId.Clear();
			else
				itemPane.cmdId		= item.GetId();
			itemPane.labelName = t_("Label :");
			itemPane.label		= item.GetLabel();
			itemPane.tooltip	= item.GetTooltip();
			itemPane.icon.SetImage(item.GetIcon());
			curIcon = item.GetIcon();
		}
	}
	else
	{
		itemPane.icon.Disable();
		itemPane.tooltip.Disable();
		itemPane.label.Enable();
		itemPane.cmdId.Clear();
		itemPane.labelName = t_("Bar name :");
		itemPane.label		= String(barTree.GetValue(0));
		itemPane.tooltip.Clear();
		itemPane.icon.SetImage(Null);
		curIcon = Null;
	}
}

// fields modified callback
void XMLBarEditor::fieldsModCb(void)
{
	// get selected node
	int i = barTree.GetCursor();
	if( i < 0 || bar == 0)
		return;
	if(i == 0)
	{
		bar->SetName(~itemPane.label);
		barTree.SetRoot(Null, itemPane.label);
		return;
	}

	XMLToolBarItem item(ValueTo<XMLToolBarItem>(barTree.Get(i)), 0);
	item.label		= ~itemPane.label;
	item.commandId	= ~itemPane.cmdId;
	item.tooltip	= ~itemPane.tooltip;
	item.icon		= curIcon;
	TreeCtrl::Node node(item.GetIcon(), RawDeepToValue(item), item.GetLabel());
	barTree.SetNode(i, node);
}
		
// image selection callback
void XMLBarEditor::imageSelCb(void)
{
	static String lastPath = "";
	
	// opens a file selector, allows selection of some
	// kind of image formats
	FileSel fs;
	fs.ActiveDir(lastPath);
	fs.Type(t_("Image files"), "*.png,*.jpg,*.ico,*.bmp");
	if(!fs.ExecuteOpen(t_("Select icon file")))
		return;
	String path = fs.Get();
	lastPath = GetFileFolder(path);
	String ext = ToUpper(GetFileExt(path));
	Image img;
	if(ext != ".ICO")
		img = StreamRaster::LoadFileAny(path);
	else
	{
		String data = LoadFile(path);
		Vector<Image> imgVec;
		try
		{
		 	imgVec = ReadIcon(data);
		}
		catch(...)
		{
		}
		if(imgVec.GetCount())
			img = imgVec[0];
		else
			img = Null;
	}
	curIcon = img;
	itemPane.icon.SetImage(img);
	fieldsModCb();
}

// barTree context menu
void XMLBarEditor::treeContextCb(Bar &b)
{
	// if no toolbar selected in, no menu
	if(!bar)
		return;
	
	int i = barTree.GetCursor();
	if(i < 0)
		return;
	if(i > 0)
	{
		b.Add(t_("Insert new item before current"), THISBACK1(treeContextAddCb, 1));
		b.Add(t_("Insert new item after current"), THISBACK1(treeContextAddCb, 2));
		b.Separator();
		b.Add(t_("Insert new separator before current item"), THISBACK1(treeContextAddCb, 5));
		b.Add(t_("Insert new separator after current item"), THISBACK1(treeContextAddCb, 6));
	}
	if(barTree.GetChildCount(i))
	{
		b.Separator();
		b.Add(t_("Insert new child item at top"), THISBACK1(treeContextAddCb, 3));
		b.Add(t_("Append new child item at bottom"), THISBACK1(treeContextAddCb, 4));
	}
	else
	{
		b.Separator();
		b.Add(t_("Insert new child item"), THISBACK1(treeContextAddCb, 4));
	}
	if(i > 0)
	{
		b.Separator();
		b.Add(t_("Remove item"), THISBACK(treeContextRemoveCb));
	}
}

void XMLBarEditor::treeContextAddCb(int mode)
{
	int id = barTree.GetCursor();
	int parentId = 0;
	int childIdx;
	int newId = 0;
	XMLToolBarItem item;
	if(mode > 4)
	{
		item.isSeparator = true;
		mode -= 4;
	}
	String lbl = (item.IsSeparator() ? t_("<SEPARATOR>") : "");
	Value v = RawPickToValue(pick(item));
	switch(mode)
	{
		case 1:
			parentId = barTree.GetParent(id);
			childIdx = barTree.GetChildIndex(parentId, id);
			newId = barTree.Insert(parentId, childIdx, Null, v, lbl);
			break;

		case 2:
			parentId = barTree.GetParent(id);
			childIdx = barTree.GetChildIndex(parentId, id);
			newId = barTree.Insert(parentId, childIdx + 1, Null, v, lbl);
			break;

		case 3:
			newId = barTree.Insert(id, 0, Null, v, "");
			break;

		case 4:
			newId = barTree.Add(id, Null, v, "");
			break;
		
		default:
			NEVER();
	}
	RefreshBar();
	barTree.SetCursor(newId);
	itemPane.label.SetFocus();
}

void XMLBarEditor::treeContextRemoveCb(void)
{
	int id = barTree.GetCursor();
	if(id > 0)
	{
		int line = barTree.GetCursorLine();
		barTree.Remove(id);
		RefreshBar();
		barTree.SetCursorLine(line);
		itemPane.label.SetFocus();
	}
}

////////////////////////////////////////////////////////////////////////////////
// constructor
XMLBarsEditor::XMLBarsEditor()
{
	// get toolbar/menu selector pane sizes, it's width will be fixed too
	selectorSize = barListPane.GetLayoutSize();
	
	CtrlLayout(barListPane);

	Add(horzSplitter.SizePos());
	horzSplitter.Horz(barListPane, barEditor);
	
	barListPane.barList.WhenSel << THISBACK(barSelCb);
	barListPane.barList.WhenBar << THISBACK(barContextCb);
}

// gets minimum size of bar editor
Size XMLBarsEditor::GetMinSize(void)
{
	return Size(
		selectorSize.cx + barEditor.GetMinSize().cx,
		max(selectorSize.cy, barEditor.GetMinSize().cy)
	);
}

// adjust layout on win changes
void XMLBarsEditor::Layout(void)
{
	if(!GetSize().cx)
		return;
	horzSplitter.SetPos(10000 * selectorSize.cx / GetSize().cx);
}

// set title
void XMLBarsEditor::SetTitle(const char *s)
{
	barListPane.title = s;
}

// sets the local copy of toolbars
void XMLBarsEditor::SetToolBars(XMLToolBars const &tb)
{
	toolBars <<= tb;
	barListPane.barList.Clear();
	for(int iBar = 0; iBar < toolBars.GetCount(); iBar++)
		barListPane.barList.Add(toolBars[iBar].GetName());
	
	// select first item
	if(barListPane.barList.GetCount())
		barListPane.barList.SetCursor(0);
}

// gets the local copy of toolbars
XMLToolBars &XMLBarsEditor::GetToolBars(void)
{
	barEditor.RefreshBar();
	for(int i = 0; i < toolBars.GetCount(); i++)
		toolBars.SetKey(i, toolBars[i].GetName());
	return toolBars;
}

// bar selection callback
void XMLBarsEditor::barSelCb(void)
{
	
	int idx = barListPane.barList.GetCursor();
	if(idx < 0)
		barEditor.SetBar(NULL);
	else
		barEditor.SetBar(&toolBars[idx]);
}

// query if a command is in use by a toolbar
bool XMLBarsEditor::IsUsingCommand(String const &cmdId) const
{
	for(int iBar = 0; iBar < toolBars.GetCount(); iBar++)
	{
		Array<XMLToolBarItem> const &items = toolBars[iBar].GetItems();
		for(int iItem = 0; iItem < items.GetCount(); iItem++)
			if(items[iItem].GetId() == cmdId)
				return true;
	}
	return false;
}
	
// bar list context menu
void XMLBarsEditor::barContextCb(Bar &bar)
{
	// get current selection
	bar.Add(t_("Add new bar"), THISBACK(barContextAddCb));
	int i = barListPane.barList.GetCursor();
	if(i < 0)
		return;
	String barName = barListPane.barList.Get(i);
	bar.Add(Format(t_("Remove bar '%s'"), barName), THISBACK(barContextRemoveCb));
}

void XMLBarsEditor::barContextAddCb(void)
{
	XMLBarAdd add(toolBars);
	int res = add.RunAppModal();
	add.Close();
	if(res == IDOK)
	{
		toolBars.AddPick(~add.barName, XMLToolBar());
		barListPane.barList.Add(~add.barName);
		barListPane.barList.SetCursor(barListPane.barList.GetCount() - 1);
	}
}

void XMLBarsEditor::barContextRemoveCb(void)
{
	int i = barListPane.barList.GetCursor();
	if(i < 0)
		return;

	// the double SetBar(NULL) is needed because, when deleting
	// row from barList, the WhenSel event is triggered and app
	// try to refresh a deleted bar, otherwise
	barEditor.SetBar(NULL);
	barListPane.barList.Remove(i);
	barEditor.SetBar(NULL);
	toolBars.Remove(i);

	if(barListPane.barList.GetCount() > i)
		barListPane.barList.SetCursor(i);
	else if(barListPane.barList.GetCount())
		barListPane.barList.SetCursor(barListPane.barList.GetCount()-1);
}

////////////////////////////////////////////////////////////////////////////////
void XMLMenuEditor::cancelCb(void)
{
	Break(IDCANCEL);
	Close();
}

void XMLMenuEditor::okCb(void)
{
	// updates edited toolbars
	if(iFace)
	{
		iFace->SetCommands(commands);
		iFace->SetMenuBars(menusEditor.GetToolBars());
		iFace->SetToolBars(barsEditor.GetToolBars());
	}
	Break(IDOK);
	Close();
}

XMLMenuEditor::XMLMenuEditor(XMLMenuInterface *_iFace) : iFace(_iFace)
{
	CtrlLayout(*this);
	
	// setup ok and cancel handlers
	okButton.Ok() <<= THISBACK(okCb);
	cancelButton.Cancel() <<= THISBACK(cancelCb);
	
	// get command pane size, that one will be fixed in width
	cmdSize = cmdPane.GetLayoutSize();
	
	// gets menu/bar editor minimum sizes
	editorSize = menusEditor.GetMinSize();
	
	// now calculate tabctrl minimum sizes
	int tx = editorSize.cx;
	int ty = cmdSize.cy;
	tabCtrlSize = Size(tx, ty);
	
	// gets button vertical neede size
	buttonVertSize = GetLayoutSize().cy - horzSplitter.GetSize().cy;
	
	// adds items to horz splitter
	horzSplitter.Horz(cmdPane, tabCtrl);
	
	// adds items to tabctrl
	tabCtrl.Add(menusEditor.SizePos(), t_("Menus"));
	tabCtrl.Add(barsEditor.SizePos(), t_("ToolBars"));

	// adds layout to all ctrls
	CtrlLayout(cmdPane);
	
	// set title for selector panes
	menusEditor.SetTitle(t_("Available menus"));
	barsEditor.SetTitle(t_("Available toolbars"));
	
	// adjust window width to accomodate at least itemPane and cmdPane
	minWidth = cmdSize.cx + tabCtrlSize.cx;
	
	// adjust window height to accomodate cmdPane
	minHeight = cmdSize.cy;
	
	SetMinSize(Size(minWidth, minHeight));
	
	int w = max(GetSize().cx, minWidth);
	int h = max(GetSize().cy - buttonVertSize, minHeight);
	int wr = w;
	int hr = h + buttonVertSize;
	SetRect(GetRect().left, GetRect().top, wr, hr);
	
	Sizeable();
	
	// reads the commands and put them into list
	commands <<= iFace->GetCommands();
	FillCmdList();
	
	// sets toolbars in bars editors
	menusEditor.SetToolBars(iFace->GetMenuBars());
	barsEditor.SetToolBars(iFace->GetToolBars());
	
	// setup handlers for command pane
	cmdPane.commandList.WhenLeftDouble << THISBACK(cmdDblClickCb);
	cmdPane.commandList.WhenBar << THISBACK(cmdContextCb);
}

XMLMenuEditor::~XMLMenuEditor()
{
}

// fills (or updates) command list
void XMLMenuEditor::FillCmdList(void)
{
	commands.Sort();
	Vector<String> const &ids = commands.GetIds();
	cmdPane.commandList.Clear();
	for(int i = 0; i < ids.GetCount(); i++)
	{
		// built in commands (un-modifiable) are in red color
		// custom ones in normal color
		XMLCommand const &cmd = commands[i];
		if(cmd.GetIsCustom())
			cmdPane.commandList.Add(ids[i]);
		else
			cmdPane.commandList.Add(ids[i], RedDisp);
	}
}

// adjust layout on win changes
void XMLMenuEditor::Layout(void)
{
	// adjust splitter sizes
	horzSplitter.SetPos(10000 * cmdSize.cx / GetSize().cx);
}

// check wether a command is used in a menu
bool XMLMenuEditor::cmdInUse(String const &cmdId) const
{
	if(menusEditor.IsUsingCommand(cmdId))
		return true;
	if(barsEditor.IsUsingCommand(cmdId))
		return true;
	return false;
}
		
// commandlist context menu
void XMLMenuEditor::cmdContextCb(Bar &bar)
{
	// get current selection
	int i = cmdPane.commandList.GetCursor();
	if(i < 0)
		return;
	String cmdId = cmdPane.commandList.Get(i);
	XMLCommand const &cmd = commands[i]; 
	bar.Add(t_("Add custom command"), THISBACK(cmdContextAddCb));
	if(cmd.GetIsCustom())
		bar.Add(t_("Edit custom command"), THISBACK(cmdContextEditCb));
	if(!cmd.GetIsCustom())
		bar.Add(false, t_("Can't remove built-in command"), THISBACK(cmdContextRemoveCb));
	else if(cmdInUse(cmdId))
		bar.Add(false, t_("Can't remove - command in use"), THISBACK(cmdContextRemoveCb));
	else
		bar.Add(t_("Remove custom command"), THISBACK(cmdContextRemoveCb));
}

void XMLMenuEditor::cmdContextAddCb(void)
{
	XMLCmdEdit edit(commands, true);
	int res = edit.RunAppModal();
	edit.Close();
	if(res == IDOK)
	{
		commands.Add(~edit.cmdId, ~edit.cmdStr);
		FillCmdList();
		cmdPane.commandList.SetCursor(cmdPane.commandList.Find(~edit.cmdId));
	}
}

void XMLMenuEditor::cmdContextEditCb(void)
{
	XMLCmdEdit edit(commands, false);
	edit.cmdId <<= cmdPane.commandList.GetData();
	edit.cmdStr <<= commands.Get(cmdPane.commandList.GetData()).GetCommandString();
	int res = edit.RunAppModal();
	edit.Close();
	if(res == IDOK)
	{
		int idx = commands.Find(~edit.cmdId);
		commands[idx].SetCommandString(~edit.cmdStr);
		FillCmdList();
		cmdPane.commandList.SetCursor(cmdPane.commandList.Find(~edit.cmdId));
	}
}

void XMLMenuEditor::cmdContextRemoveCb(void)
{
	int i = cmdPane.commandList.GetCursor();
	if(i < 0)
		return;
	cmdPane.commandList.Remove(i);
	commands.Remove(i);
	FillCmdList();
}

// commandlist double-click handler
void XMLMenuEditor::cmdDblClickCb(void)
{
	int iCmd = cmdPane.commandList.GetCursor();
	if(iCmd < 0)
		return;
	String cmdId = cmdPane.commandList.Get(iCmd);
	int iTab = tabCtrl.Get();
	if(iTab == 0)
		menusEditor.SetCommandId(cmdId);
	else
		barsEditor.SetCommandId(cmdId);
}
		
END_UPP_NAMESPACE