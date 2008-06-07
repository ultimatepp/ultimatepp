#include "Docedit.h"

String PackageDirectory(const String& name)
{
	return AppendFileName("F:/uppsrc", name);
}

String SourcePath(const String& package, const String& name)
{
	return AppendFileName(PackageDirectory(package), name);
}

String CommonPath(const String& filename)
{
	return AppendFileName("f:/theide", filename);
}

DocBase doc_base;

void DocBase::RemoveFile(const String& file)
{
	CppBase base;
	CppBase& doc_base = *this;
	for(int i = 0; i < doc_base.GetCount(); i++) {
		String m = doc_base.GetKey(i);
		CppNamespace& mm = doc_base[i];
		for(int i = 0; i < mm.GetCount(); i++) {
			String n = mm.GetKey(i);
			CppNest& nn = mm[i];
			for(int i = 0; i < nn.GetCount(); i++) {
				CppItem& q = nn[i];
				if(q.file != file)
					base.GetAdd(m).GetAdd(n).GetAdd(nn.GetKey(i)) = q;
			}
		}
	}
	doc_base = base;
}

void DocBase::ParseFile(const String& file, const String& package) throw(CParser::Error)
{
	Parse(FileIn(file), ignore, doc_base, package, file);
}

void DocBase::RefreshFile(const String& file, const String& package) throw(CParser::Error)
{
	RemoveFile(file);
	ParseFile(file, package);
}

Vector<String> DocBase::GetHeaders()
{
	Index<String> h;
	for(int i = 0; i < doc_base.GetCount(); i++) {
		String m = doc_base.GetKey(i);
		CppNamespace& mm = doc_base[i];
		for(int i = 0; i < mm.GetCount(); i++) {
			String n = mm.GetKey(i);
			CppNest& nn = mm[i];
			for(int i = 0; i < nn.GetCount(); i++) {
				h.FindAdd(nn[i].file);
			}
		}
	}
	return h.PickKeys();
}
