#ifndef _ide_Builders_Components_h_
#define _ide_Builders_Components_h_

#include <Core/Core.h>

NAMESPACE_UPP

class BuilderComponent {
public:
	BuilderComponent() { this->builder = NULL; }
	BuilderComponent(Builder *builder) : builder(builder) {}
	
	void SetBuilder(Builder *builder) { this->builder = builder; }
	
	bool IsBuilder() const { return builder != NULL; }
	
protected:
	Builder *builder;
};

struct Blitz {
	bool   build;
	int    count;
	String path;
	String object;
	String info;
};

String BlitzBaseFile();
void   ResetBlitz();
void   InitBlitz();

class BlitzBuilderComponent : public BuilderComponent {
public:
	BlitzBuilderComponent(Builder *builder) : BuilderComponent(builder) {}
	
	Blitz MakeBlitzStep(Vector<String>& sfile, Vector<String>& soptions,
	                    Vector<String>& obj, Vector<String>& immfile,
	                    const char *objext, Vector<bool>& optimize,
	                    const Index<String>& noblitz);
	
};

END_UPP_NAMESPACE

#endif
