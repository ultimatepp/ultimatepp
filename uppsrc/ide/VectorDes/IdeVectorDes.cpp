#include "IdeVectorDes.h"

#define IMAGECLASS IdeVecImg
#define IMAGEFILE  <ide/VectorDes/IdeVectorDes.iml>
#include <Draw/iml.h>

static const char ctrl_key[] = "vectordes-ctrl";

INITBLOCK
{
	RegisterGlobalConfig(ctrl_key);
}

bool VectorDesigner::Load(const char *fn)
{
	filename = fn;
	try {
		String data = LoadFile(filename);
		if(data.IsVoid())
			throw Exc(NFormat("Error reading file '%s'.", filename));
		CParser parser(data, filename);
		designer->LoadVecFile(parser);
		return true;
	}
	catch(Exc e) {
		PutConsole(NFormat("%s: %s\n", filename, e));
		return false;
	}
}

void VectorDesigner::Save()
{
	if(!SaveChangedFileFinish(filename, designer->SaveVecFile()))
		Exclamation(NFormat("Error saving file [* \1%s\1].", filename));
}

bool IsVecFile(String path) { return !CompareNoCase(GetFileExt(path), ".vec"); }

typedef ArrayMap<String, VectorDesigner::EditPos> EditPosMap;
GLOBAL_VAR(EditPosMap, VectorFileDataVar);

void VectorDesigner::EditPos::Serialize(Stream& stream)
{
}

void VectorDesigner::SaveEditPos()
{
}

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

struct VectorDesModule : public IdeModule {
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

	virtual IdeDesigner *CreateDesigner(const char *path, byte cs) {
		if(IsVecFile(path)) {
			VectorDesigner *d = CreateVectorDesigner(path, ctrl_key);
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

void RegisterIdeVectorDes()
{
	RegisterIdeModule(Single<VectorDesModule>());
	RegisterStdVectorObjects();
}

/*
GUI_APP_MAIN
{
	void AppMain___();
	void RegisterVectorDes(); RegisterIdeVectorDes();
	RegisterStdVectorObjects();
	RichObject::Register(RichObjectTypeVectorImage());
	AppMain___();
}
*/
