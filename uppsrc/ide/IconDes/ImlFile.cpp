#include "IconDes.h"

class AlphaImageInfo
{
public:
	enum ENCODING { COLOR_RLE, MONO_RLE, MONO_PACKED };

	AlphaImageInfo(Size size = Size(0, 0), int encoding = COLOR_RLE, Point hotspot = Point(0, 0))
		: encoding(encoding), size(size), hotspot(hotspot) {}

	void               Serialize(Stream& stream);

public:
	int                encoding;
	Size               size;
	Point              hotspot;
};

String AlphaToRLE(const Image& aa)
{
	String result;
	for(int y = 0; y < aa.GetHeight(); y++) {
		result.Cat(PackRLE(aa[y], aa.GetWidth()));
		result.Cat(0x80);
	}
	return result;
}

Image RLEToAlpha(const String& rle, Size sz)
{
	ImageBuffer ib(sz);
	const byte *s = rle;
	for(int y = 0; y < sz.cy; y++)
		if((const char *)s < rle.End())
			s = UnpackRLE(ib[y], s, sz.cx) + 1;
		else
			memset(ib[y], 0, sz.cx * sizeof(RGBA));
	return Image(ib);
}

void AlphaImageInfo::Serialize(Stream& stream)
{
	int version = 1;
	stream / version;
	if(version >= 1)
		stream % size % hotspot % encoding;
}

void ScanIML(CParser& parser, Array<ImlImage>& out_images,
             VectorMap<String, String>& out_settings)
{
	String name, bid;
	bool exp = false;
	while(!parser.IsEof())
	{
		if((bid = parser.ReadId()) == "IMAGE_META")
		{
			parser.Char('(');
			if(parser.IsString())
				name = parser.ReadString();
			else
				name = parser.ReadId();
			parser.PassChar(',');
			String value = parser.ReadString();
			parser.PassChar(')');
			out_settings.Add(name, value);
			if(value == "exp")
				exp = true;
		}
		else if(bid == "IMAGE_BEGIN" && parser.Char('(') && !IsNull(name = parser.ReadId()) && parser.Char(')'))
		{
			String encoded_data;
			out_settings.GetAdd("wince_16bit", "0");
			String id;
			bool first = true;
			while((id = parser.ReadId()) == "IMAGE_SCAN" && parser.Char('('))
			{
				bool first_in_row = true;
				while(parser.IsChar('\"'))
				{
					String scan = parser.ReadOneString();
					if(!first && first_in_row)
						encoded_data.Cat('\x80');
					first_in_row = first = false;
					encoded_data.Cat(scan);
				}
				if(!parser.Char(')'))
					break;
			}
			AlphaImageInfo image;
			bool accepted = false;
			if(parser.Char('(') && parser.ReadId() == name && parser.Char(',')) {
				if(id == "IMAGE_END"
				&& (image.size.cx = parser.ReadInt()) > 0 && parser.Char(',')
				&& (image.size.cy = parser.ReadInt()) > 0 && parser.Char(')')) {
					accepted = true;
				}
				else if(id == "IMAGE_PACKED" && parser.IsChar('\"')) {
					String d = parser.ReadOneString();
					if(parser.Char(')'))
					{
						StringStream ss(d);
						ss % image;
						if(!ss.IsError())
							accepted = true;
					}
				}
			}
			if(name.GetLength() >= 6 && !memcmp(name, "_java_", 6))
				accepted = false;

			if(accepted) {
				if(name.GetLength() >= 4 && !memcmp(name, "im__", 4))
					name = Null;

				Image m = RLEToAlpha(encoded_data, image.size);
				ImageBuffer ib(m);
				ib.SetHotSpot(image.hotspot);
				m = ib;
				ImlImage& c = out_images.Add();
				c.name = name;
				c.image = m;
				c.exp = exp;
				exp = false;
			}
		}
		else if(bid == "IMAGE_BEGIN16" && parser.Char('(') && !IsNull(name = parser.ReadId()) && parser.Char(')'))
		{ //TODO: FIX THESE!!!
			out_settings.GetAdd("wince_16bit", "1");
			String encoded_data;
			String id;
			bool first = true;
			while((id = parser.ReadId()) == "IMAGE_SCAN16" && parser.Char('(') && parser.Char('L'))
			{
				bool first_in_row = true;
				while(parser.Char('\"'))
				{
					CParser::Pos pos = parser.GetPos();
					const char *end;
					end = pos.ptr; // TODO - remove
					String scan; // TODO = GetUnicodeScan(pos.ptr, &end);
					pos.ptr = end;
					parser.SetPos(pos);
					if(!parser.Char('\"'))
						break;
					if(!first && first_in_row)
						encoded_data.Cat('\x80');
					first_in_row = first = false;
					encoded_data.Cat(scan);
				}
				if(!parser.Char(')'))
					break;
			}
			AlphaImageInfo idata;
			bool accepted = false;
			if(id == "IMAGE_END16" && parser.Char('(') && parser.ReadId() == name && parser.Char(',')
			&& (idata.size.cx = parser.ReadInt()) > 0 && parser.Char(',')
			&& (idata.size.cy = parser.ReadInt()) > 0 && parser.Char(',')
			&& !IsNull(idata.hotspot.x = parser.ReadInt()) && parser.Char(',')
			&& !IsNull(idata.hotspot.y = parser.ReadInt()) && parser.Char(')'))
			{
				accepted = true;
			}

			if(accepted)
			{
				if(name.GetLength() >= 4 && !memcmp(name, "im__", 4))
					name = Null;

				Image m = RLEToAlpha(encoded_data, idata.size);
				ImageBuffer ib(m);
				ib.SetHotSpot(idata.hotspot);
				m = ib;
				ImlImage& c = out_images.Add();
				c.name = name;
				c.image = m;
				c.exp = exp;
				exp = false;
			}
		}
		else
			break;
	}
}

bool LoadIml(const String& data, Array<ImlImage>& img, int& format)
{
	CParser p(data);
	format = 0;
	try {
		bool premultiply = !p.Id("PREMULTIPLIED");
		Vector<String> name;
		Vector<bool> exp;
		while(p.Id("IMAGE_ID")) {
			p.PassChar('(');
			String n;
			if(p.IsId()) {
				n = p.ReadId();
				if(n.StartsWith("im__", 4))
					n = Null;
				p.PassChar(')');
			}
			else
				while(!p.IsEof()) {
					if(p.Char(')'))
						break;
					p.SkipTerm();
				}
			name.Add(n);
			bool e = false;
			if(p.Id("IMAGE_META")) {
				p.PassChar('(');
				e = p.ReadString() == "exp";
				if(p.Char(',') && p.IsString())
					p.ReadString();
				p.PassChar(')');
			}
			exp.Add(e);
		}
		int ii = 0;
		while(p.Id("IMAGE_BEGIN_DATA")) {
			String data;
			while(p.Id("IMAGE_DATA")) {
				p.PassChar('(');
				for(int j = 0; j < 32; j++) {
					if(j) p.PassChar(',');
					data.Cat(p.ReadInt());
				}
				p.PassChar(')');
			}
			p.PassId("IMAGE_END_DATA");
			p.PassChar('(');
			int zlen = p.ReadInt();
			p.PassChar(',');
			int count = p.ReadInt();
			p.PassChar(')');

			data.Trim(zlen);
			Vector<ImageIml> m = UnpackImlData(data, data.GetCount());
			if(m.GetCount() != count || ii + count > name.GetCount())
				p.ThrowError("");
			for(int i = 0; i < count; i++) {
				ImlImage& c = img.Add();
				(ImageIml &)c = m[i];
				c.name = name[ii];
				c.exp = exp[ii++];
				c.name.TrimEnd("__DARK");
				c.name.TrimEnd("__UHD");
				if(premultiply)
					c.image = Premultiply(c.image);
			}
		}
		if(!p.IsEof())
			p.ThrowError("");
	}
	catch(CParser::Error) {
		try {
			CParser p(data);
			Array<ImlImage> m;
			VectorMap<String, String> s;
			ScanIML(p, img, s);
			if(img.GetCount())
				format = 1;
		}
		catch(...) {
			return false;
		}
	}
	return true;
}

String SaveIml(const Array<ImlImage>& iml, int format, const String& eol) {
	StringStream out;
	out << "PREMULTIPLIED" << eol;
	Index<String> names;
	Index<String> saved_names;
	for(int i = 0; i < iml.GetCount(); i++) {
		const ImlImage& c = iml[i];
		names.FindAdd(c.name);
		out << "IMAGE_ID(" << c.name;
		if((c.flags & (IML_IMAGE_FLAG_UHD|IML_IMAGE_FLAG_DARK)) == 0)
			saved_names.FindAdd(c.name);
		if(c.flags & IML_IMAGE_FLAG_UHD)
			out << "__UHD";
		if(c.flags & IML_IMAGE_FLAG_DARK)
			out << "__DARK";
		out << ")";
		if(c.exp)
			out << " IMAGE_META(\"exp\", \"\")";
		out << eol;
	}

	for(String id : names) // allow UHD versions to be downscaled
		if(saved_names.Find(id) < 0)
			out << "IMAGE_ID(" << id << ")" << eol;

	int ii = 0;
	while(ii < iml.GetCount()) {
		int bl = 0;
		int bn = 0;
		Vector<ImageIml> bimg;
		while(bl < 4096 && ii < iml.GetCount()) {
			const ImlImage& c = iml[ii++];
			ImageIml& m = bimg.Add();
			m.image = c.image;
			m.flags = c.flags;
			bl += (int)c.image.GetLength();
			bn++;
		}
		String bs = PackImlData(bimg);
		out << eol << "IMAGE_BEGIN_DATA" << eol;
		bs.Cat(0, ((bs.GetCount() + 31) & ~31) - bs.GetCount());
		const byte *s = bs;
		for(int n = bs.GetCount() / 32; n--;) {
			out << "IMAGE_DATA(";
			for(int j = 0; j < 32; j++) {
				if(j) out << ',';
				out << (int)*s++;
			}
			out << ")" << eol;
		}
		out << "IMAGE_END_DATA(" << bs.GetCount() << ", " << bn << ")" << eol;
	}
	return out.GetResult();
}
