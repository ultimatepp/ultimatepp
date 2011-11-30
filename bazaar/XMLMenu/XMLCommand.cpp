#include "XMLCommand.h"

NAMESPACE_UPP

////////////////////////////////////////////////////////////////////////////////////

// xml support
void XMLCommand::Xmlize(XmlIO xml)
{
	// just custom commands get streamed, so we don't need to
	// stream anything, just set values on load
	if(xml.IsLoading())
	{
		enabled = true;
		custom = true;
		control = NULL;
		ctrlSize = Size(-1, -1);
		callback.Clear();
	}
	else
	{
		ASSERT(custom == true);
	}
}

////////////////////////////////////////////////////////////////////////////////////
// adds a custom command
XMLCommands &XMLCommands::Add(String const &id)
{
	if(Has(id))
		return *this;
	XMLCommand *cmd = new XMLCommand;
	cmd->control = NULL;
	cmd->enabled = true;
	cmd->custom = true;
	cmd->callback.Clear();
	commands.Add(id, cmd);
	return *this;
}

// adds a built-in command with given callback
XMLCommands &XMLCommands::Add(String const &id, Callback cb)
{
	bool has = Has(id);
	XMLCommand *cmd;
	if(has)
		cmd = &commands.Get(id);
	else
		cmd = new XMLCommand;
	cmd->control = NULL;
	cmd->enabled = true;
	cmd->custom = false;
	cmd->callback = cb;
	if(!has)
		commands.Add(id, cmd);
	return *this;
}

// adds a control
XMLCommands &XMLCommands::Add(String const &id, Ctrl &ctrl, Size const &size)
{
	bool has = Has(id);
	XMLCommand *cmd;
	if(has)
		cmd = &commands.Get(id);
	else
		cmd = new XMLCommand;
	cmd->control = &ctrl;
	cmd->ctrlSize = size;
	cmd->enabled = true;
	cmd->custom = false;
	cmd->callback.Clear();
	if(!has)
		commands.Add(id, cmd);
	return *this;
}
XMLCommands &XMLCommands::Add(String const &id, Ctrl &ctrl)
{
	return Add(id, ctrl, Size(-1, -1));
}

// adds a custom command, allows enable/disable item
XMLCommands &XMLCommands::Add(bool enabled, String const &id)
{
	bool has = Has(id);
	XMLCommand *cmd;
	if(has)
		cmd = &commands.Get(id);
	else
		cmd = new XMLCommand;
	cmd->control = NULL;
	cmd->enabled = enabled;
	cmd->custom = true;
	cmd->callback.Clear();
	if(!has)
		commands.Add(id, cmd);
	return *this;
}

// adds a built-in command with given callback, allows enable/disable item
XMLCommands &XMLCommands::Add(bool enabled, String const &id, Callback cb)
{
	bool has = Has(id);
	XMLCommand *cmd;
	if(has)
		cmd = &commands.Get(id);
	else
		cmd = new XMLCommand;
	cmd->control = NULL;
	cmd->enabled = enabled;
	cmd->custom = false;
	cmd->callback = cb;
	if(!has)
		commands.Add(id, cmd);
	return *this;
}

// adds a control, allows enable/disable item
XMLCommands &XMLCommands::Add(bool enabled, String const &id, Ctrl &ctrl, Size const &size)
{
	bool has = Has(id);
	XMLCommand *cmd;
	if(has)
		cmd = &commands.Get(id);
	else
		cmd = new XMLCommand;
	cmd->control = &ctrl;
	cmd->ctrlSize = size;
	cmd->enabled = true;
	cmd->custom = false;
	cmd->callback.Clear();
	if(!has)
		commands.Add(id, cmd);
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
			custom.Add(commands[i]);
			customIdx.Add(commands.GetKey(i));
		}
		else
		{
			builtIn.Add(commands[i]);
			builtInIdx.Add(commands.GetKey(i));
		}
	}
	IndexSort(customIdx, custom, XMLCmdLess());
	IndexSort(builtInIdx, builtIn, XMLCmdLess());
	commands.Clear();
	for(int i = 0; i < builtIn.GetCount(); i++)
		commands.Add(builtInIdx[i], builtIn[i]);
	for(int i = 0; i < custom.GetCount(); i++)
		commands.Add(customIdx[i], custom[i]);
	
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
			commands.Add(newCmds.GetKey(i), newCmds[i]);
	}
	else
	{
		// extract custom commands from current ones
		ArrayMap<String, XMLCommand> custCmds;
		for(int i = 0; i < commands.GetCount(); i++)
			if(commands[i].GetIsCustom())
				custCmds.Add(commands.GetKey(i), commands[i]);
		
		// stream out custom commands
		xml("commands", custCmds);
	}
}
		
END_UPP_NAMESPACE