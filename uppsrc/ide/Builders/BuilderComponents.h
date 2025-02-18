#ifndef _ide_Builders_Components_h_
#define _ide_Builders_Components_h_

#include <Core/Core.h>

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

void BlitzFile(String& blitz, const String& sourceFile, const Vector<String>& defines, int index);

class BlitzBuilderComponent : public BuilderComponent {
public:
	BlitzBuilderComponent(Builder *builder);
	
	void SetWorkingDir(const String& workingDir)       { this->outDir = workingDir; }
	void SetBlitzFileName(const String& blitzFileName) { this->blitzFileName = blitzFileName; }
	
	Blitz MakeBlitzStep(
		Builder& b,
		Vector<String>& sfile, Vector<String>& soptions,
		Vector<String>& obj, Vector<String>& immfile,
		const char *objext, const Index<String>& noblitz,
		const String& package = Null);
	                    
private:
	String outDir;
	String blitzFileName;
};

#endif
