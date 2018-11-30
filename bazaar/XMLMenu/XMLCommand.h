#ifndef _XMLCommand_h_
#define _XMLCommand_h_

#include <CtrlCore/CtrlCore.h>

NAMESPACE_UPP

////////////////////////////////////////////////////////////////////////////////////
// a single command, i.e. available entry for menu and bars
class XMLToolBar;
class XMLCommand : DeepCopyOption<XMLCommand>
{
	friend class XMLCommands;
	private:
		// embedded control, if any
		Ptr<Ctrl> control;
		Size ctrlSize;
		
		// associated callback, if any
		Function<void()> callback;
		
		// generated menu, if any
		Function<void(XMLToolBar &)> menuCallback;
		
		// a plece to store the generated menu toolbar
		// it MUJST be persistent, as system require it after generation
		One<XMLToolBar> menuTb;
		
		// enabled flag
		bool enabled;
		
		// custom command flag
		bool custom;
		
		// custom command string to be sent
		// to custom command handler
		String commandString;

	public:
		// constructor
		XMLCommand() {}
		
		// copy constructor
		XMLCommand(XMLCommand rval_ c);
		XMLCommand(XMLCommand const &c, int);
		
		Ctrl *GetCtrl(void) const										{ return control;		}
		Size const &GetCtrlSize(void) const								{ return ctrlSize;		}
		Function<void()> const &GetCallback(void) const					{ return callback;		}
		Function<void(XMLToolBar &)> const &GetMenuCallback(void) const	{ return menuCallback;	}
		XMLToolBar const &GetMenuTb(void);

		bool GetIsEnabled(void) const									{ return enabled;		}
		bool GetIsCustom(void) const									{ return custom;		}

		String const &GetCommandString(void) const						{ return commandString; }
		XMLCommand &SetCommandString(String const &s)					{ commandString = s; return *this; }
		
		bool operator==(XMLCommand &other) const;
		
		// xml support
		void Xmlize(XmlIO xml);
};

////////////////////////////////////////////////////////////////////////////////////
// an array of available commands
class XMLCommands : DeepCopyOption<XMLCommands>
{

	private:
		ArrayMap<String, XMLCommand> commands;

	public:
		rval_default(XMLCommands);

		// default constructor
		XMLCommands() {}
		
		// copy constructor
		XMLCommands(XMLCommands const &cmds, int dummy) : commands(cmds.commands, 0) {}
		
		// adds a custom command
		XMLCommands &Add(String const &id, String const &cmd);
		XMLCommands &Add(const char *id, const char *cmd)
			{ return Add(String(id), String(cmd)); }
		
		// adds a built-in command with given callback
		XMLCommands &Add(String const &id, Function<void()> cb);
		XMLCommands &Add(const char *id, Function<void()> cb)
			{ return Add(String(id), cb); }
		
		// adds a generated submenu "command"
		XMLCommands &Add(String const &id, Function<void(XMLToolBar &)> mc);
		XMLCommands &Add(const char *id, Function<void(XMLToolBar &)> mc)
			{ return Add(String(id), mc); }
		
		// adds a control
		XMLCommands &Add(String const &id, Ctrl &ctrl);
		XMLCommands &Add(const char *id, Ctrl &ctrl)
			{ return Add(String(id), ctrl); }
		XMLCommands &Add(String const &id, Ctrl &ctrl, Size const &size);
		XMLCommands &Add(const char *id, Ctrl &ctrl, Size const &size)
			{ return Add(String(id), ctrl, size); }
		
		// adds a custom command, allows enable/disable item
		XMLCommands &Add(bool enabled, String const &id, String const &cmd);
		XMLCommands &Add(bool enabled, const char *id, String const &cmd)
			{ return Add(enabled, String(id), cmd); }
		XMLCommands &Add(bool enabled, String const &id, const char *cmd)
			{ return Add(enabled, id, String(cmd)); }
		
		// adds a built-in command with given callback, allows enable/disable item
		XMLCommands &Add(bool enabled, String const &id, Function<void()> cb);
		XMLCommands &Add(bool enabled, const char *id, Function<void()> cb)
			{ return Add(enabled, String(id), cb); }
		
		// adds a generated submenu "command", allows enable/disable item
		XMLCommands &Add(bool enabled, String const &id, Function<void(XMLToolBar &)> mc);
		XMLCommands &Add(bool enabled, const char *id, Function<void(XMLToolBar &)> mc)
			{ return Add(enabled, String(id), mc); }
		
		// adds a control, allows enable/disable item
		XMLCommands &Add(bool enabled, String const &id, Ctrl &ctrl);
		XMLCommands &Add(bool enabled, const char *id, Ctrl &ctrl)
			{ return Add(enabled, String(id), ctrl); }

		XMLCommands &Add(bool enabled, String const &id, Ctrl &ctrl, Size const &size);
		XMLCommands &Add(bool enabled, const char *id, Ctrl &ctrl, Size const &size)
			{ return Add(enabled, String(id), ctrl, size); }
		
		// removes a command
		XMLCommands &Remove(int idx) { commands.Remove(idx); return *this; }

		// get all available command IDs
		Vector<String> const &GetIds(void) const;
		
		// get a command for a given id
		XMLCommand const &Get(String const &id) const { return commands.Get(ToUpper(id)); }
		XMLCommand &Get(String const &id) { return commands.Get(ToUpper(id)); }

		// find a command given its name
		int Find(String const &id) const { return commands.Find(ToUpper(id)); }
		
		// sets the commands by a callback
		void Set(Function<void(XMLCommands &)> commands);
		
		// checks wether a command is present given its id
		bool Has(String const &id) const { return commands.Find(ToUpper(id)) >= 0; }
		
		// array access
		XMLCommand const &operator[](int idx) const { return commands[idx]; }
		XMLCommand &operator[](int idx) { return commands[idx]; }
		
		// sort items - alphabetically, but first built-in commands, then custom ones
		XMLCommands &Sort(void);
		
		// xml support
		void Xmlize(XmlIO xml);
};

END_UPP_NAMESPACE

#endif
