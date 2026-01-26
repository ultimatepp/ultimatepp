#include "Designers.h"

#define LAYOUTFILE <ide/Designers/Designers.lay>
#include <CtrlCore/lay.h>

struct ExportMD : WithExportMDLayout<TopWindow> {
	String             md;
	Vector<RichObject> img;

	void Export(const RichPara& p);
	void Export(const RichTable& table, const RichStyles& styles);
	void Export(const RichText& txt);
	
	static bool IsPreformatted(const RichPara& p);
	
	void Do(const char *qtf);
	
	ExportMD();
};

ExportMD::ExportMD()
{
	CtrlLayoutExit(*this, "Export as GitHub Markdown");
}

void ExportMD::Export(const RichPara& p)
{
	if(p.format.ruler)
		md << "---";
	if(p.format.bullet)
		md << "- ";
	for(int i = 0; i < p.part.GetCount(); i++) {
		const RichPara::Part& part = p.part[i];
		int q;
		if(part.object) {
			md << "IMAGE:" << img.GetCount();
			img << part.object;
		}
		else {
			const wchar *s = part.text;
			
			while(*s == ' ') {
				md << ' ';
				s++;
			}
			
			String endtag;
			if(part.format.sscript == 1) {
				md << "<sup>";
				endtag = "</sup>" + endtag;
			}
			if(part.format.sscript == 2) {
				md << "<sub>";
				endtag = "</sub>" + endtag;
			}
			if(part.format.IsUnderline()) {
				md << "<ins>";
				endtag = "</ins>" + endtag;
			}
			if(part.format.IsBold()) {
				if(!md.TrimEnd("**"))
					md << "**";
				endtag = "**" + endtag;
			}
			if(part.format.IsItalic()) {
				md << "_";
				endtag = "_" + endtag;
			}
			if(part.format.IsStrikeout()) {
				md << "~~";
				endtag = "~~" + endtag;
			}

			while(*s) {
				auto NeedsEscape = [](int c) { return strchr("|[]{}^*_<>~-`'\"", c); };
				if(*s == '\\' && NeedsEscape(s[1]))
					md << "\\\\" << (char)*++s;
				else
				if(NeedsEscape(*s))
					md << "\\" << (char)*s;
				else
					md << ToUtf8(*s);
				s++;
			}
			
			if(endtag.GetCount()) {
				String t;
				while(md.TrimEnd(" "))
					t << ' ';
				md << endtag << t;
			}
		}
	}
}

void ExportMD::Export(const RichTable& table, const RichStyles& styles)
{
	for(int i = 0; i < table.GetRows(); i++) {
		for(int j = 0; j < table.GetColumns(); j++) {
			md << "|";
			const RichTxt& txt = table.Get(i, j);
			for(int i = 0; i < txt.GetPartCount(); i++)
				if(txt.IsPara(i))
					Export(txt.Get(i, styles));
		}
		md << "|\n";
		if(i == 0) {
			for(int j = 0; j < table.GetColumns(); j++)
				md << "|-";
			md << "|\n";
		}
	}
	md << "\n\n";
}

bool ExportMD::IsPreformatted(const RichPara& p)
{
	bool b = false;
	for(int i = 0; i < p.part.GetCount(); i++) {
		const RichPara::Part& part = p.part[i];
		if(part.object || !(part.format.GetFaceInfo() & Font::FIXEDPITCH))
			return false;
		b = b || part.text.GetCount();
	}
	return b;
}

void ExportMD::Export(const RichText& txt)
{
	int i = 0;
	while(i < txt.GetPartCount())
		if(txt.IsPara(i)) {
			const RichPara& p = txt.Get(i++);
			if(IsPreformatted(p)) {
				md << "```\n";
				md << ToUtf8(p.GetText()) << '\n';
				while(i < txt.GetPartCount() && txt.IsPara(i)) {
					const RichPara& p = txt.Get(i);
					if(!IsPreformatted(p))
						break;
					i++;
					md << ToUtf8(p.GetText()) << '\n';
				}
				md << "```\n";
			}
			else {
				Export(p);
				md << "\n\n";
			}
		}
		else
		if(txt.IsTable(i))
			Export(txt.GetTable(i++), txt.GetStyles());
		else
			i++;
}

void ExportMD::Do(const char *qtf)
{
	Export(ParseQTF(qtf));
	WriteClipboardText(md);
	if(img.GetCount()) {
		list.SetLineCy(DPI(28));
		list.AddColumn("Image").Ctrls([&](int ii, One<Ctrl>& ctrl) {
			Button& b = ctrl.Create<Button>();
			b.SetImage(CtrlImg::copy());
			b.SetLabel("Copy IMAGE:" + AsString(ii));
			b << [=] {
				if(ii >= 0 && ii < img.GetCount()) {
					RichObjectPaintInfo pi;
					pi.ink = SBlack();
					WriteClipboardImage(img[ii].ToImage(img[ii].GetPixelSize(), pi));
				}
			};
		});
		for(int i = 0; i < img.GetCount(); i++)
			list.Add();
		Run();
	}
	else
		PromptOK("Markdown copied to clipboard.");
}

void ExportMarkdown(const char *qtf)
{
	ExportMD md;
	md.Do(qtf);
}
