#include "Builders.h"

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

void BlitzFile(String& blitz, const String& sourceFile, const Vector<String>& defines, int index)
{
	blitz << "\r\n"
	      << "#define BLITZ_INDEX__ F" << index << "\r\n"
	      << "#include \"" << sourceFile << "\"\r\n";
	for(String s : defines) {
		int q = s.Find('(');
		if(q >= 0) s.Trim(q);
		blitz << "#ifdef " << s << "\r\n"
		      << "#undef " << s << "\r\n"
		      << "#endif\r\n";
	}
	blitz << "#undef BLITZ_INDEX__\r\n";
}

BlitzBuilderComponent::BlitzBuilderComponent(Builder* builder)
	: BuilderComponent(builder)
	, outDir(builder->outdir)
	, blitzFileName("$blitz")
{
}

Blitz BlitzBuilderComponent::MakeBlitzStep(
	Builder& builder,
	Vector<String>& sourceFiles, Vector<String>& soptions,
	Vector<String>& obj, Vector<String>& immfile,
	const char *objext, const Index<String>& noblitz,
	const String& package)
{
	Blitz b;
	b.count = 0;
	b.build = false;

	if(!IsBuilder())
		return b;
	
	if(package.GetCount())
		blitzFileName = package + "$blitz";

	Vector<String> excluded;
	Vector<String> excludedoptions;
	b.object = CatAnyPath(outDir, blitzFileName + String(objext));
	Time blitztime = GetFileTime(b.object);
	String blitz;
	if(!IdeGetOneFile().IsEmpty())
		return b;
	String build_info_path = NormalizePath(builder.GetBuildInfoPath());
	for(int i = 0; i < sourceFiles.GetCount(); i++) {
		String sourceFile = sourceFiles[i];
		String ext = ToLower(GetFileExt(sourceFile));
		String objfile = CatAnyPath(outDir, GetFileTitle(sourceFile) + objext);
		Time sourceFileTime = GetFileTime(sourceFile);
		Time htime = builder.HdependFileTime(sourceFile);
		if((ext == ".cpp" || ext == ".cc" || ext == ".cxx" || ext == ".icpp")
		   && HdependBlitzApproved(sourceFile) && IsNull(soptions[i])
		   && sourceFileTime < blitz_base_time
		   && noblitz.Find(sourceFile) < 0) {
			if(htime > blitztime)
				b.build = true;
			BlitzFile(blitz, sourceFile, HdependGetDefines(sourceFile), i);
			b.info << ' ' << GetFileName(sourceFile);
			b.count++;
		}
		else {
			excluded.Add(sourceFile);
			excludedoptions.Add(soptions[i]);
		}
	}
	
	b.path = CatAnyPath(outDir, blitzFileName + ".cpp");
	if(b.count > 1) {
		sourceFiles = pick(excluded);
		soptions = pick(excludedoptions);
		if(LoadFile(b.path) != blitz) {
			RealizeDirectory(GetFileDirectory(b.path));
			SaveFile(b.path, blitz);
			b.build = true;
		}
		obj.Add(b.object);
		immfile.Add(b.object);
	}
	else {
		DeleteFile(b.path);
		b.build = false;
	}
	return b;
}
