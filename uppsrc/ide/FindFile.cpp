#include "ide.h"

static int CharFilterFindFileMask(int c)
{
	return ToUpper(ToAscii(c));
}

class FindFileWindow final : public WithFindFileLayout<TopWindow> {
private:
	struct FindFileFileDisplay : public Display {
		void Paint(Draw& w, const Rect& r, const Value& q,
				   Color ink, Color paper, dword style) const override
		{
			String txt = q;
			Image fileImage = IdeFileImage(txt, false, false);
		
			int cellHeight = r.bottom - r.top;
		
			w.DrawRect(r, paper);
			w.DrawImage(r.left, r.top + ((cellHeight - fileImage.GetHeight()) / 2), fileImage);
			w.DrawText(r.left + fileImage.GetWidth() + Zx(5), r.top +
					  ((cellHeight - StdFont().GetCy()) / 2),
					  txt, StdFont(), ink);
		}
	};

public:
	FindFileWindow(const Workspace& wspc, const String& acctualPackage,
				   const String& serachString);
	
	String GetFile() const;
	String GetPackage() const;
	
private:
	void OnSearch();
	
	bool IsFileMeetTheCriteria(
		const Package::File& file, const String& packageName,const String& query);
	bool IsAcctualPackage(const String& packageName);
	
private:
	const String     acctualPackage;
	const Workspace& wspc;
};

FindFileWindow::FindFileWindow(const Workspace& wspc, const String& acctualPackage,
							   const String& serachString)
	: wspc(wspc)
	, acctualPackage(acctualPackage)
{
	CtrlLayoutOKCancel(*this, "Find File");
	Sizeable().Zoomable();
	list.AutoHideSb();
	list.AddColumn("File").SetDisplay(Single<FindFileFileDisplay>());
	list.AddColumn("Package");
	list.WhenLeftDouble = Acceptor(IDOK);
	mask.NullText("Search");
	mask.SetText(serachString);
	mask.SelectAll();
	mask.SetFilter(CharFilterFindFileMask);
	mask << [=] { OnSearch(); };
	searchInCurrentPackage << [=] { OnSearch(); };
	
	OnSearch();
}

String FindFileWindow::GetFile() const
{
	return list.Get(0);
}

String FindFileWindow::GetPackage() const
{
	return list.Get(1);
}

void FindFileWindow::OnSearch()
{
	list.Clear();
	String maskValue = ~mask;
	for(int p = 0; p < wspc.GetCount(); p++) {
		String packageName = wspc[p];
		const Package& pack = wspc.GetPackage(p);
		for(const auto& file : pack.file) {
			if(IsFileMeetTheCriteria(file, packageName, maskValue)) {
				list.Add(file, packageName);
			}
		}
	}
	if(list.GetCount() > 0)
		list.SetCursor(0);
	
	ok.Enable(list.IsCursor());
}

bool FindFileWindow::IsFileMeetTheCriteria(
	const Package::File& file, const String& packageName, const String& query)
{
	if (searchInCurrentPackage.Get() == true && !IsAcctualPackage(packageName))
		return false;
	
	return !file.separator && (ToUpper(packageName).Find(query) >= 0 || ToUpper(file).Find(query) >= 0);
}

bool FindFileWindow::IsAcctualPackage(const String& packageName)
{
	return acctualPackage.Compare(packageName) == 0;
}

void Ide::FindFileName()
{
	FindFileWindow findFileWindow(IdeWorkspace(), actualpackage, find_file_search_string);
	if(findFileWindow.Execute() == IDOK) {
		if(findFileWindow.list.IsCursor()) {
			find_file_search_string = ~findFileWindow.mask;
			EditFile(SourcePath(findFileWindow.GetPackage(), findFileWindow.GetFile()));
		}
	}
}
