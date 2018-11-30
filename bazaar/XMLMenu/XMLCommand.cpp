#include "XMLCommand.h"
#include "XMLToolBar.h"

NAMESPACE_UPP

////////////////////////////////////////////////////////////////////////////////////

// copy constructors
XMLCommand::XMLCommand(XMLCommand const &c, int)
{
	control					= c.control;
	ctrlSize				= c.ctrlSize;
	callback				= c.callback;
	menuCallback			= c.menuCallback;
	if(!c.menuTb.IsEmpty())
		menuTb				= One<XMLToolBar>(new XMLToolBar(*c.menuTb, 1)); // @@ was deep copy operator <<=
	else
		menuTb.Clear();
	enabled					= c.enabled;
	custom					= c.custom;
	commandString			= c.commandString;
}

XMLCommand::XMLCommand(XMLCommand rval_ c)
{
	control					= c.control;
	ctrlSize				= c.ctrlSize;
	callback				= c.callback;
	menuCallback			= c.menuCallback;
	menuTb					= pick(c.menuTb);
	enabled					= c.enabled;
	custom					= c.custom;
	commandString			= c.commandString;
}

// xml support
void XMLCommand::Xmlize(XmlIO xml)
{
	// just custom commands get streamed
	// only command string gets streamed, other values are just set
	if(xml.IsLoading())
	{
		enabled = true;
		custom = true;
		control = NULL;
		ctrlSize = Size(-1, -1);
		callback.Clear();
		menuCallback.Clear();
	}
	else
	{
		ASSERT(custom == true);
	}
	xml("CommandString", commandString);
}

XMLToolBar const &XMLCommand::GetMenuTb(void)
{
	menuTb = new XMLToolBar;
	menuCallback(*menuTb);
	return *menuTb;
}

////////////////////////////////////////////////////////////////////////////////////
// adds a custom command
XMLCommands &XMLCommands::Add(String const &id, String const &cmdStr)
{
	if(Has(id))
		return *this;
	XMLCommand *cmd = new XMLCommand;
	cmd->control = NULL;
	cmd->enabled = true;
	cmd->custom = true;
	cmd->callback.Clear();
	cmd->menuCallback.Clear();
	cmd->commandString = cmdStr;
	commands.Add(ToUpper(id), cmd);
	return *this;
}

// adds a built-in command with given callback
XMLCommands &XMLCommands::Add(String const &id, Function<void()> cb)
{
	bool has = Has(id);
	XMLCommand *cmd;
	if(has)
		cmd = &commands.Get(ToUpper(id));
	else
		cmd = new XMLCommand;
	cmd->control = NULL;
	cmd->enabled = true;
	cmd->custom = false;
	cmd->callback = cb;
	cmd->menuCallback.Clear();
	if(!has)
		commands.Add(ToUpper(id), cmd);
	return *this;
}

// adds a generated submenu "command"
XMLCommands &XMLCommands::Add(String const &id, Function<void(XMLToolBar &)> mc)
{
	bool has = Has(id);
	XMLCommand *cmd;
	if(has)
		cmd = &commands.Get(ToUpper(id));
	else
		cmd = new XMLCommand;
	cmd->control = NULL;
	cmd->enabled = true;
	cmd->custom = false;
	cmd->callback.Clear();
	cmd->menuCallback = mc;
	if(!has)
		commands.Add(ToUpper(id), cmd);
	return *this;
}

// adds a control
XMLCommands &XMLCommands::Add(String const &id, Ctrl &ctrl, Size const &size)
{
	bool has = Has(id);
	XMLCommand *cmd;
	if(has)
		cmd = &commands.Get(ToUpper(id));
	else
		cmd = new XMLCommand;
	cmd->control = &ctrl;
	cmd->ctrlSize = size;
	cmd->enabled = true;
	cmd->custom = false;
	cmd->callback.Clear();
	cmd->menuCallback.Clear();
	if(!has)
		commands.Add(ToUpper(id), cmd);
	return *this;
}
XMLCommands &XMLCommands::Add(String const &id, Ctrl &ctrl)
{
	return Add(id, ctrl, Size(-1, -1));
}

// adds a custom command, allows enable/disable item
XMLCommands &XMLCommands::Add(bool enabled, String const &id, String const &cmdStr)
{
	bool has = Has(id);
	XMLCommand *cmd;
	if(has)
		cmd = &commands.Get(ToUpper(id));
	else
		cmd = new XMLCommand;
	cmd->control = NULL;
	cmd->enabled = enabled;
	cmd->custom = true;
	cmd->callback.Clear();
	cmd->menuCallback.Clear();
	cmd->commandString = cmdStr;
	if(!has)
		commands.Add(ToUpper(id), cmd);
	return *this;
}

// adds a built-in command with given callback, allows enable/disable item
XMLCommands &XMLCommands::Add(bool enabled, String const &id, Function<void()> cb)
{
	bool has = Has(id);
	XMLCommand *cmd;
	if(has)
		cmd = &commands.Get(ToUpper(id));
	else
		cmd = new XMLCommand;
	cmd->control = NULL;
	cmd->enabled = enabled;
	cmd->custom = false;
	cmd->callback = cb;
	cmd->menuCallback.Clear();
	if(!has)
		commands.Add(ToUpper(id), cmd);
	return *this;
}

// adds a generated submenu "command", allows enable/disable item
XMLCommands &XMLCommands::Add(bool enabled, String const &id, Function<void(XMLToolBar &)> mc)
{
	bool has = Has(id);
	XMLCommand *cmd;
	if(has)
		cmd = &commands.Get(ToUpper(id));
	else
		cmd = new XMLCommand;
	cmd->control = NULL;
	cmd->enabled = enabled;
	cmd->custom = false;
	cmd->callback.Clear();
	cmd->menuCallback = mc;
	if(!has)
		commands.Add(ToUpper(id), cmd);
	return *this;
}
		
// adds a control, allows enable/disable item
XMLCommands &XMLCommands::Add(bool enabled, String const &id, Ctrl &ctrl, Size const &size)
{
	bool has = Has(id);
	XMLCommand *cmd;
	if(has)
		cmd = &commands.Get(ToUpper(id));
	else
		cmd = new XMLCommand;
	cmd->control = &ctrl;
	cmd->ctrlSize = size;
	cmd->enabled = true;
	cmd->custom = false;
	cmd->callback.Clear();
	cmd->menuCallback.Clear();
	if(!has)
		commands.Add(ToUpper(id), cmd);
	return *this;
}
XMLCommands &XMLCommands::Add(bool enabled, String const &id, Ctrl &ctrl)
{
	return Add(enabled, id, ctrl, Size(-1, -1));
}

// get all available command IDs
Vector<String> const &XMLCommands::GetIds(void) const
{
	return commands.GetKeys();
}

struct XMLCmdLess
{
	bool operator()(String const &a, String const &b) const
		{ return ToUpper(a) < ToUpper(b); }
};
	
// sort items - alphabetically, but first built-in commands, then custom ones
XMLCommands &XMLCommands::Sort(void)
{
	Array<XMLCommand> builtIn, custom;
	Array<String> builtInIdx, customIdx;
	for(int i = 0; i < commands.GetCount(); i++)
	{
		if(commands[i].GetIsCustom())
		{
			custom.AddPick(pick(commands[i]));
			customIdx.Add(commands.GetKey(i));
		}
		else
		{
			builtIn.AddPick(pick(commands[i]));
			builtInIdx.Add(commands.GetKey(i));
		}
	}
	IndexSort(customIdx, custom, XMLCmdLess());
	IndexSort(builtInIdx, builtIn, XMLCmdLess());
	commands.Clear();
	for(int i = 0; i < builtIn.GetCount(); i++)
		commands.AddPick(builtInIdx[i], pick(builtIn[i]));
	for(int i = 0; i < custom.GetCount(); i++)
		commands.AddPick(customIdx[i], pick(custom[i]));
	
	return *this;
}

// xml support
void XMLCommands::Xmlize(XmlIO xml)
{
	// just stream CUSTOM commands; embedded ones are defined
	// by application and can't be edited
	if(xml.IsLoading())
	{
		// wipe all custom commands currently present
		for(int i = commands.GetCount() - 1; i >= 0; i--)
			if(commands[i].GetIsCustom())
				commands.Remove(i);

		// stream in new commands
		ArrayMap<String, XMLCommand> newCmds;
		xml("commands", newCmds);
		
		// appends new commands to current list
		for(int i = 0; i < newCmds.GetCount(); i++)
			commands.AddPick(newCmds.GetKey(i), pick(newCmds[i]));
	}
	else
	{
		// extract custom commands from current ones
		ArrayMap<String, XMLCommand> custCmds;
		for(int i = 0; i < commands.GetCount(); i++)
			if(commands[i].GetIsCustom())
				custCmds.Add(commands.GetKey(i), new XMLCommand(commands[i], 1));
		
		// stream out custom commands
		xml("commands", custCmds);
	}
}
		
END_UPP_NAMESPACE