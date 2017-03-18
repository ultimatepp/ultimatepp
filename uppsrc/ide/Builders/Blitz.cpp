#include "Builders.h"

namespace Upp {

String BlitzBaseFile()
{
	return ConfigFile("blitzbase");
}

void ResetBlitz()
{
	SaveFile(BlitzBaseFile(), "");
}

Time blitz_base_time;

void InitBlitz()
{
	Time ltm = Time::High();

	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) { // find lowest file time
		const Package& pk = wspc.GetPackage(i);
		String n = wspc[i];
		for(int i = 0; i < pk.GetCount(); i++) {
			String path = SourcePath(n, pk.file[i]);
			if(FileExists(path))
				ltm = min(ltm, FileGetTime(path));
		}
	}
	
	blitz_base_time = max(GetSysTime() - 3600, Time(GetFileTime(BlitzBaseFile())));
	if(ltm != Time::High())
		blitz_base_time = max(blitz_base_time, ltm + 3 * 60); // should solve first build after install/checkout
}

BlitzBuilderComponent::BlitzBuilderComponent(Builder* builder)
	: BuilderComponent(builder)
	, workingDir(builder->outdir)
	, blitzFileName("$blitz")
{
}

Blitz BlitzBuilderComponent::MakeBlitzStep(
	Vector<String>& sourceFiles, Vector<String>& soptions,
	Vector<String>& obj, Vector<String>& immfile,
	const char *objext, const Index<String>& noblitz)
{
	Blitz b;
	b.count = 0;
	b.build = false;

	if(!IsBuilder())
		return b;
	
	Vector<String> excluded;
	Vector<String> excludedoptions;
	b.object = CatAnyPath(workingDir, blitzFileName + String(objext));
	Time blitztime = GetFileTime(b.object);
	String blitz;
	if(!IdeGetOneFile().IsEmpty())
		return b;
	for(int i = 0; i < sourceFiles.GetCount(); i++) {
		String sourceFile = sourceFiles[i];
		String ext = ToLower(GetFileExt(sourceFile));
		String objfile = CatAnyPath(workingDir, GetFileTitle(sourceFile) + objext);
		Time sourceFileTime = GetFileTime(sourceFile);
		if((ext == ".cpp" || ext == ".cc" || ext == ".cxx" || ext == ".icpp")
		   && HdependBlitzApproved(sourceFile) && IsNull(soptions[i])
		   && sourceFileTime < blitz_base_time
		   && noblitz.Find(sourceFile) < 0) {
			if(HdependFileTime(sourceFile) > blitztime)
				b.build = true;
			blitz << "\r\n"
			      << "#define BLITZ_INDEX__ F" << i << "\r\n"
			      << "#include \"" << builder->GetHostPath(sourceFile) << "\"\r\n";
			b.info << ' ' << GetFileName(sourceFile);
			const Vector<String>& d = HdependGetDefines(sourceFile);
			for(int i = 0; i < d.GetCount(); i++)
				blitz << "#ifdef " << d[i] << "\r\n"
				      << "#undef " << d[i] << "\r\n"
				      << "#endif\r\n";
			blitz << "#undef BLITZ_INDEX__\r\n";
			b.count++;
		}
		else {
			excluded.Add(sourceFile);
			excludedoptions.Add(soptions[i]);
		}
	}
	
	b.path = CatAnyPath(workingDir, blitzFileName + ".cpp");
	if(b.count > 1) {
		sourceFiles = pick(excluded);
		soptions = pick(excludedoptions);
		if(builder->LoadFile(b.path) != blitz) {
			builder->RealizeDir(GetFileDirectory(b.path));
			builder->SaveFile(b.path, blitz);
			b.build = true;
		}
		obj.Add(b.object);
		immfile.Add(b.object);
	}
	else {
		builder->DeleteFile(b.path);
		b.build = false;
	}
	return b;
}

}
