#include "Theming.h"

String Themed::GetClass() const
{
	CParser p(typeid(*this).name());
	String n;
	while(!p.IsEof()) {
		if(p.IsId())
			n = p.ReadId();
		else
			p.SkipTerm();
	}
	return n;
}

struct ThemeData : Moveable<ThemeData> {
	VectorMap<String, String> property;
	VectorMap<String, Image>  image;
};

struct ChildThemeData : ThemeData, Moveable<ChildThemeData> {
	Ctrl::LogPos              pos;
};

struct ClassTheme : Moveable<ClassTheme> {
	ThemeData                         data;
	VectorMap<String, ChildThemeData> child;
	
	void Load(const char *dir, const char *cls);
};

Point ReadNums(CParser& p) {
	Point pt;
	pt.x = p.ReadInt();
	p.PassChar(',');
	pt.y = p.ReadInt();
	return pt;
}

void LoadProp(FileIn& in, VectorMap<String, String>& prop)
{
	while(!in.IsEof()) {
		String ln = in.GetLine();
		CParser p(ln);
		if(p.Char(';'))
			break;
		if(p.IsId()) {
			String& d = prop.GetAdd(p.ReadId());
			p.SkipSpaces();
			d = p.GetPtr();
		}
	}
}

void ClassTheme::Load(const char *dir, const char *cls)
{
	String imgdir = AppendFileName(dir, String(cls) + ".image");
	FindFile ff(AppendFileName(imgdir, "*.png"));
	while(ff) {
		Vector<String> v = Split(ff.GetName(), '.');
		if(v.GetCount() == 6) {
			Image m = StreamRaster::LoadFileAny(AppendFileName(imgdir, ff.GetName()));
			ImageBuffer b(m);
			b.SetHotSpot(Point(atoi(v[1]), atoi(v[2])));
			b.Set2ndSpot(Point(atoi(v[3]), atoi(v[4])));
			m = b;
			int q = v[0].Find('-');
			if(q >= 0)
				child.GetAdd(v[0].Mid(0, q)).image.GetAdd(v[0].Mid(q + 1)) = m;
			else
				data.image.GetAdd(v[0]) = m;
		}
		ff.Next();
	}
	FileIn in(AppendFileName(dir, String(cls) + ".class"));
	if(!in) return;
	LoadProp(in, data.property);
	while(!in.IsEof()) {
		String ln = in.GetLine();
		CParser p(ln);
		if(p.IsId()) {
			String id = p.ReadId();
			ChildThemeData& m = child.GetAdd(id);
			while(!p.IsEof()) {
				if(p.Id("left")) {
					Point pt = ReadNums(p);
					m.pos.x = Ctrl::PosLeft(pt.x, pt.y);
				}
				else
				if(p.Id("right")) {
					Point pt = ReadNums(p);
					m.pos.x = Ctrl::PosRight(pt.x, pt.y);
				}
				else
				if(p.Id("hsize")) {
					Point pt = ReadNums(p);
					m.pos.x = Ctrl::PosSize(pt.x, pt.y);
				}
				else
				if(p.Id("hcenter")) {
					Point pt = ReadNums(p);
					m.pos.x = Ctrl::PosCenter(pt.x, pt.y);
				}
				else
				if(p.Id("top")) {
					Point pt = ReadNums(p);
					m.pos.y = Ctrl::PosLeft(pt.x, pt.y);
				}
				else
				if(p.Id("bottom")) {
					Point pt = ReadNums(p);
					m.pos.y = Ctrl::PosRight(pt.x, pt.y);
				}
				else
				if(p.Id("vsize")) {
					Point pt = ReadNums(p);
					m.pos.y = Ctrl::PosSize(pt.x, pt.y);
				}
				else
				if(p.Id("vcenter")) {
					Point pt = ReadNums(p);
					m.pos.y = Ctrl::PosCenter(pt.x, pt.y);
				}
			}
			LoadProp(in, m.property);
		}
	}
}

VectorMap<String, ClassTheme> theme;

void Themed::LoadTheme(const char *dir)
{
	theme.Clear();
	FindFile ff(AppendFileName(dir, "*.class"));
	while(ff) {
		String cls = ToLower(GetFileTitle(ff.GetName()));
		theme.GetAdd(cls).Load(dir, cls);
		ff.Next();
	}
}

void Themed::ReadTheme()
{
	int q = theme.Find(ToLower(GetClass()));
	if(q < 0) return;
	ClassTheme& cm = theme[q];
	ThemeProperties tm(cm.data.property, cm.data.image, true);
	Properties(tm);
	Ctrl *p = dynamic_cast<Ctrl *>(this);
	if(p)
		for(Ctrl *q = p->GetFirstChild(); q; q = q->GetNext()) {
			Themed *t = dynamic_cast<Themed *>(q);
			if(t) {
				int i = cm.child.Find(t->GetClass());
				if(i >= 0) {
					ThemeProperties tm(cm.child[i].property, cm.child[i].image, true);
					t->Properties(tm);
				}
				i = cm.child.Find(t->id);
				if(i >= 0) {
					q->SetPos(cm.child[i].pos);
					ThemeProperties tm(cm.child[i].property, cm.child[i].image, true);
					t->Properties(tm);
				}
			}
		}
}

String FillRight(const String& text, int n)
{
	int q = n - text.GetLength();
	return q > 0 ? text + String(' ', q) : text;
}

void SaveProp(FileOut& out, VectorMap<String, String>& prop, const char *tab)
{
	for(int i = 0; i < prop.GetCount(); i++)
		out << tab << FillRight(prop.GetKey(i), 8) << " " << prop[i] << "\r\n";
	out << ";\r\n\r\n";
}

void SaveImages(const char *dir, const char *prefix, const VectorMap<String, Image>& img)
{
	for(int i = 0; i < img.GetCount(); i++) {
		String fn;
		if(prefix)
			fn << prefix;
		Point p1 = img[i].GetHotSpot();
		Point p2 = img[i].Get2ndSpot();
		fn << img.GetKey(i) << '.' << p1.x << '.' << p1.y << '.' << p2.x << '.' << p2.y << ".png";
		PNGEncoder().SaveFile(AppendFileName(dir, fn), img[i]);
	}
}

static String sWriteDir;

void Themed::SetWrite(const char *writedir)
{
	sWriteDir = writedir;
}

void Themed::WriteTheme()
{
	if(IsNull(sWriteDir))
		return;
	RealizeDirectory(sWriteDir);
	String imgdir = AppendFileName(sWriteDir, GetClass() + ".image");
	FileOut out(AppendFileName(sWriteDir, GetClass() + ".class"));
	VectorMap<String, String> prop;
	VectorMap<String, Image> img;
	ThemeProperties tm(prop, img, false);
	Properties(tm);
	RealizeDirectory(imgdir);
	SaveImages(imgdir, NULL, img);
	SaveProp(out, prop, "");
	Ctrl *p = dynamic_cast<Ctrl *>(this);
	if(p)
		for(Ctrl *q = p->GetFirstChild(); q; q = q->GetNext()) {
			Themed *t = dynamic_cast<Themed *>(q);
			if(t && t->id.GetCount()) {
//				prop.Clear();
//				ThemeProperties tm(prop, img, false);
//				t->Properties(tm);
				out << t->id;
				Ctrl::LogPos pos = q->GetPos();
				switch(pos.x.GetAlign()) {
				case Ctrl::LEFT:   out << Format(" left %d, %d", pos.x.GetA(), pos.x.GetB()); break;
				case Ctrl::RIGHT:  out << Format(" right %d, %d", pos.x.GetA(), pos.x.GetB()); break;
				case Ctrl::SIZE:   out << Format(" hsize %d, %d", pos.x.GetA(), pos.x.GetB()); break;
				case Ctrl::CENTER: out << Format(" hcenter %d, %d", pos.x.GetB(), pos.x.GetA()); break;
				}
				switch(pos.y.GetAlign()) {
				case Ctrl::TOP:    out << Format(" top %d, %d", pos.y.GetA(), pos.y.GetB()); break;
				case Ctrl::BOTTOM: out << Format(" bottom %d, %d", pos.y.GetA(), pos.y.GetB()); break;
				case Ctrl::SIZE:   out << Format(" vsize %d, %d", pos.y.GetA(), pos.y.GetB()); break;
				case Ctrl::CENTER: out << Format(" vcenter %d, %d", pos.y.GetB(), pos.y.GetA()); break;
				}
				out << "\r\n";
//				SaveProp(out, prop, "   ");
			}
		}
}

void Themed::Properties(ThemeProperties& x) {}
