#include "VTypes.h"

NAMESPACE_UPP

template<>
String AsString(const Ctrl::LogPos& pos)
{
	String s;
	int xa = pos.x.GetA(), xb = pos.x.GetB();
	switch(pos.x.GetAlign())
	{
		case Ctrl::LEFT:   s << "Left(mr:" << xa << ",sz:" << xb << ")"; break;
		case Ctrl::RIGHT:  s << "Right(mr:" << xa << ",sz:" << xb << ")"; break;
		case Ctrl::SIZE:   s << "HSize(mr:" << xa << ",mr:" << xb << ")"; break;
		default:
		case Ctrl::CENTER: s << "HCent(of:" << xa << ",sz:" << xb << ")"; break;
	}
	s << ".";
	int ya = pos.y.GetA(), yb = pos.y.GetB();
	switch(pos.y.GetAlign())
	{
		case Ctrl::TOP:    s << "Top(mr:" << ya << ",sz:" << yb << ")"; break;
		case Ctrl::BOTTOM: s << "Bottom(mr:" << ya << ",sz:" << yb << ")"; break;
		case Ctrl::SIZE:   s << "VSize(mr:" << ya << ",mr:" << yb << ")"; break;
		default:
		case Ctrl::CENTER: s << "VCent(of:" << ya << ",sz:" << yb << ")"; break;
	}
	return s;
}

template<>
Stream& operator%(Stream& s, Ctrl::LogPos& pos)
{
	int xal,xa,xb, yal,ya,yb;
	if(s.IsStoring())
	{
		xal = pos.x.GetAlign(); xa = pos.x.GetA(); xb = pos.x.GetB();
		yal = pos.y.GetAlign(); ya = pos.y.GetA(); yb = pos.y.GetB();
	}
	//s % pos.x. % pos.y; //that's be nice
	s % xal % xa % xb;
	s % yal % ya % yb;
	
	if(s.IsLoading())
	{
		pos.x.SetAlign(xal); pos.x.SetA(xa); pos.x.SetB(xb);
		pos.y.SetAlign(yal); pos.y.SetA(ya); pos.y.SetB(yb);
	}
	return s;
}

template<>
void Xmlize(XmlIO& xml, Ctrl::LogPos& pos)
{
	static VectorMap<String, int> mapx, mapy;
	static VectorMap<int, String> remapx, remapy;
	ONCELOCK
	{
		mapx.Add("hcenter", Ctrl::CENTER);
		remapx.Add(mapx.Top(), mapx.TopKey());
		mapx.Add("left", Ctrl::LEFT);
		remapx.Add(mapx.Top(), mapx.TopKey());
		mapx.Add("right", Ctrl::RIGHT);
		remapx.Add(mapx.Top(), mapx.TopKey());
		mapx.Add("hsize", Ctrl::SIZE);
		remapx.Add(mapx.Top(), mapx.TopKey());
		
		mapy.Add("vcenter", Ctrl::CENTER);
		remapy.Add(mapy.Top(), mapy.TopKey());
		mapy.Add("top", Ctrl::TOP);
		remapy.Add(mapy.Top(), mapy.TopKey());
		mapy.Add("bottom", Ctrl::BOTTOM);
		remapy.Add(mapy.Top(), mapy.TopKey());
		mapy.Add("vsize", Ctrl::SIZE);
		remapy.Add(mapy.Top(), mapy.TopKey());
	}

	int xal=Ctrl::CENTER,xa=0,xb=0, yal=Ctrl::CENTER,ya=0,yb=0;
	if(xml.IsStoring())
	{
		xal = pos.x.GetAlign(); xa = pos.x.GetA(); xb = pos.x.GetB();
		yal = pos.y.GetAlign(); ya = pos.y.GetA(); yb = pos.y.GetB();

		xml.Add(remapx.Get(xal)).Attr("a",xa).Attr("b",xb);
		xml.Add(remapy.Get(yal)).Attr("a",ya).Attr("b",yb);
	}

	if(xml.IsLoading())
	{
		int i = -1;
		while(++i < xml->GetCount()){
			const XmlNode & n = xml->Node(i);
			String type = n.GetTag();
			int ii = -1;

			ii = mapx.Find(type);
			if(ii>=0)
			{
				xal = mapx[ii];	
				XmlAttrLoad(xa, n.Attr("a"));
				XmlAttrLoad(xb, n.Attr("b"));
				continue;
			}
			ii = mapy.Find(type);
			if(ii>=0)
			{
				yal = mapy[ii];	
				XmlAttrLoad(ya, n.Attr("a"));
				XmlAttrLoad(yb, n.Attr("b"));
				continue;
			}

		}
		pos.x.SetAlign(xal); pos.x.SetA(xa); pos.x.SetB(xb);
		pos.y.SetAlign(yal); pos.y.SetA(ya); pos.y.SetB(yb);
	}
}

template<> void Jsonize(JsonIO& jio, Ctrl::LogPos& pos)
{
	int xal,xa,xb, yal,ya,yb;
	if(jio.IsStoring())
	{
		xal = pos.x.GetAlign(); xa = pos.x.GetA(); xb = pos.x.GetB();
		yal = pos.y.GetAlign(); ya = pos.y.GetA(); yb = pos.y.GetB();
	}
	//jio("xpos", pos.x)("ypos", pos.y); //that's be nice
	jio("xal", xal)("xa", xa)("xb", xb);
	jio("yal", yal)("ya", ya)("yb", yb);
	
	if(jio.IsLoading())
	{
		pos.x.SetAlign(xal); pos.x.SetA(xa); pos.x.SetB(xb);
		pos.y.SetAlign(yal); pos.y.SetA(ya); pos.y.SetB(yb);
	}
}

INITBLOCK {
	RichValue<Ctrl::LogPos>::Register();
}

END_UPP_NAMESPACE
