//aris: I removed it:
//#include "IdeVectorDes.h"
/* TODO:
1.Creation is a problem at the moment
2. Host problem
3.and shutdown (only from Windows Task Manager) !!!!!
4. registration data: load and save
5. where to add Main menu
6. and Load from file
7. Save to file.
8. Switch off-on the toolbars
9. Vec Data format - how to have parent-childr relatioships between images
10. Connecting (Linking?) images
11. Modules
12. Have Vector objects in QTF (and UWord)

- IMAGESPACE ?
- is this ctrl-key stored in Ide config files?
*/

#include <VectorDes/VectorDes.h>

#define IMAGESPACE IdeVecImg
#define IMAGEFILE <ArisVectorDesigner/IdeVectorDes.iml>
#include <Draw/iml.h>

//aris: this is for global config
static const char ctrl_key[] = "vectordes-ctrl-aris";

INITBLOCK
{
	RegisterGlobalConfig(ctrl_key);
}

//aris commented: #include <ide/Common/Common.h>
//aris: added public
//aris: ArisDesigner would be some kind of a universal designer..

struct ArisDesigner : public TopWindow {
	virtual String GetFileName() const = 0;
	virtual void   Save() = 0;
//aris	virtual void   SyncUsc() = 0;
	virtual void   ActivateDesigner() = 0;
	virtual void   SaveEditPos() = 0;
	virtual void   EditMenu(Bar& menu) = 0;
	virtual int    GetCharset() const                                        { return -1; }

	virtual ~ArisDesigner() {}
};


//aris: replaced Aris instead of Ide
struct ArisModule {
	virtual void         CleanUsc() {}
	virtual bool         ParseUsc(CParser&)                                  { return false; }
	virtual Image        FileIcon(const char *filename)                      { return Null; }
	virtual ArisDesigner *CreateDesigner(const char *path, byte charset)      { return NULL; }
	virtual void         Serialize(Stream& s) {}
};


void       RegisterArisModule(ArisModule& module);
int        GetArisModuleCount();
ArisModule& GetArisModule(int q);

static Vector<ArisModule *>& sM()
{
	static Vector<ArisModule *> m;
	return m;
}

void RegisterArisModule(ArisModule& module)
{
	sM().Add(&module);
}

int  GetArisModuleCount()
{
	return sM().GetCount();
}

ArisModule& GetArisModule(int q)
{
	return *sM()[q];
}

//aris FIXME or ask .... VectorDeshost is a empty class in ...VectorDes.h
class VectorDesigner : public ArisDesigner, public VectorDesHost
{
	One<VectorDes> designer;

public:
//aris FIND OUT: the host problem!!!
	VectorDesigner()                               { designer = new VectorDes(*this); Add(designer->SizePos()); }
	~VectorDesigner()                              { Shutdown(); }

	virtual void   Close()                         { designer->CloseDesigner(); }
	virtual void   ChildGotFocus()                 { if(!IsShutdown()) designer->FrameFocus(); }
	virtual bool   Key(dword key, int repcnt)      { return designer->Key(key, repcnt); }
	virtual String GetFileName() const             { return filename; }
	virtual void   Save();

//aris: do i need USC???
//	virtual void   SyncUsc()                       {}
	virtual void   ActivateDesigner()              { SetForeground(); }
	virtual void   SaveEditPos();
	virtual void   EditMenu(Bar& menu)             { designer->EditMenu(menu); }

	void           Serialize(Stream& s)            { designer->Serialize(s); }
	bool           Load(const char *fn);
	void           SetConfigName(const char *name) { cfgname = name; }

	static void    SerializeEditPos(Stream& stream);

	struct EditPos {
		Time filetime;

		void Serialize(Stream& stream);
	};

private:
	String         cfgname;
	String         filename;
};

//aris: Creation is a problem for now and shutdown!!!!!-------------------------
VectorDesigner *CreateVectorDesigner(const char *filename, const char *cfgname, bool topwindow);


//---------------------------------------
//Laod file from *.vec
bool VectorDesigner::Load(const char *fn)
{
	filename = fn;
	try {
		String data = LoadFile(filename);
		if(data.IsVoid())
			throw Exc(NFormat("Error reading file '%s'.", filename));
		CParser parser(data, filename);
		//loads the actual vec data file!!! from parser???
		designer->LoadVecFile(parser);
		return true;
	}
	catch(Exc e) {
//aris
		PromptOK(NFormat("%s: %s\n", filename, e));
//		PutConsole(NFormat("%s: %s\n", filename, e));
		return false;
	}
}

void VectorDesigner::Save()
{
	//aris: FIXME
//	if(!SaveChangedFileFinish(filename, designer->SaveVecFile()))
//		Exclamation(NFormat("Error saving file [* \1%s\1].", filename));
}


bool IsVecFile(String path) { return !CompareNoCase(GetFileExt(path), ".vec"); }


//aris: need to find out more about those globals
typedef ArrayMap<String, VectorDesigner::EditPos> EditPosMap;
GLOBAL_VAR(EditPosMap, VectorFileDataVar);


//aris: how does it work if it's empty?
void VectorDesigner::EditPos::Serialize(Stream& stream)
{
}


//aris how does it work if it's empty?
void VectorDesigner::SaveEditPos()
{
}


//aris SEE: VectorFileDataVar
void VectorDesigner::SerializeEditPos(Stream& s)
{
	s % VectorFileDataVar();
}



VectorDesigner *CreateVectorDesigner(const char *filename, const char *cfgname)
{
	One<VectorDesigner> q = new VectorDesigner;
	LoadFromGlobal(*q, cfgname);
	q->SetConfigName(cfgname);
	if(q->Load(filename))
		return -q;
	return NULL;
}


struct VectorDesModule : public ArisModule {
	virtual void CleanUsc()
	{
	}

	virtual bool ParseUsc(CParser& p)
	{
		return false;
	}

	virtual Image FileIcon(const char *path)
	{
		return IsVecFile(path) ? IdeVecImg::ide_icon() : Null;
	}

	virtual ArisDesigner *CreateDesigner(const char *path, byte cs) {
		if(IsVecFile(path)) {
			VectorDesigner *d = CreateVectorDesigner(path, ctrl_key);
			if(d)
				d->SizePos();
			return d;
		}
		return false;
	}
	virtual void Serialize(Stream& s) {
		int version = 0;
		s / version;
		VectorDesigner::SerializeEditPos(s);
	}
};

void RegisterArisVectorDes()
{
	RegisterArisModule(Single<VectorDesModule>());
//aris: what for those:
	RegisterStdVectorObjects();
	RichObject::Register(RichObjectTypeVectorImage());
}



GUI_APP_MAIN
{

	RegisterArisVectorDes();
	VectorDesigner().Sizeable().Zoomable().Run();
//aris: When to use ?-> Ctrl::EventLoop();
}
