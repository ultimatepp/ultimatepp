#ifndef _IdeVectorDes_IdeVectorDes_h_
#define _IdeVectorDes_IdeVectorDes_h_

#include <VectorDes/VectorDes.h>
#include <ide/Common/Common.h>

class VectorDesigner : public IdeDesigner, public VectorDesHost
{
	One<VectorDes> designer;

public:
	VectorDesigner()                               { designer = new VectorDes(*this); }
	~VectorDesigner()                              { designer->Shutdown(); }

	virtual String GetFileName() const             { return filename; }
	virtual void   Save();
	virtual void   SaveEditPos();
	virtual void   EditMenu(Bar& menu)             { designer->EditMenu(menu); }
	virtual Ctrl&  DesignerCtrl()                  { return *designer; }

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

VectorDesigner *CreateVectorDesigner(const char *filename, const char *cfgname, bool topwindow);
void            RegisterIdeVectorDes();

#endif
