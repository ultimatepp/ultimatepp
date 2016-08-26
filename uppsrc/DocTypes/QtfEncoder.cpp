#include "DocTypes.h"

namespace Upp {

static Color DocQTFColor[] = {
	Black, LtGray, White, Red, Green, Blue, LtRed, WhiteGray, LtCyan, Yellow
};

static int DocQTFFontHeight[] = {
	50, 67, 84, 100, 134, 167, 200, 234, 300, 400
};

static void SeparateNumber(String& s)
{
	if(*s.Last() >= '0' && *s.Last() <= '9')
		s.Cat(';');
}

//////////////////////////////////////////////////////////////////////

struct RichObjectTypeDrawingData
{
	void    Serialize(Stream& stream);

	Drawing drawing;
	Size    dot_size;
	String  comment;
};

void RichObjectTypeDrawingData::Serialize(Stream& stream)
{
	int version = 1;
	stream % version % dot_size % comment % drawing;
}

static String RichObjectTypeDrawingTypeName()
{
	return "Drawing";

}

static String FmtColor(Color c)
{
	if(IsNull(c)) return "N";
	for(int i = 0; i < 10; i++)
		if(DocQTFColor[i] == c)
			return String(i + '0', 1);
	return NFormat("(%d.%d.%d)", c.GetR(), c.GetG(), c.GetB());
}

String CharFmt(const Paragraph::Part& a, const Paragraph::Part& b)
{
	String fmt;
	if(a.font.IsBold() != b.font.IsBold()) fmt.Cat('*');
	if(a.font.IsItalic() != b.font.IsItalic()) fmt.Cat('/');
	if(a.font.IsUnderline() != b.font.IsUnderline()) fmt.Cat('_');
	if(a.font.IsStrikeout() != b.font.IsStrikeout()) fmt.Cat('-');
//	if(a.capitals != b.capitals) fmt.Cat('c');
	if(a.delta != b.delta)
		fmt.Cat(b.delta == 0 ? a.delta < 0 ? '`' : ',' :
		        b.delta < 0 ? '`' : ',');
	if(a.font.GetFace() != b.font.GetFace())
		switch(b.font.GetFace()) {
		case Font::ARIAL:     fmt.Cat('A'); break;
		case Font::ROMAN:     fmt.Cat('R'); break;
		case Font::COURIER:   fmt.Cat('C'); break;
		case Font::STDFONT:   fmt.Cat('G'); break;
		case Font::SYMBOL:    fmt.Cat('S'); break;
		default:
			fmt << "!" << a.font.GetFaceName() << "!";
		}
	if(a.value != b.value)
		fmt << '^' << DeQtf(StdFormat(b.value)) << '^';
	if(a.color != b.color)
		fmt << "@" << FmtColor(b.color);
	if(a.font.GetHeight() != b.font.GetHeight()) {
		for(int i = 0; i < 10; i++)
			if(b.font.GetHeight() == DocQTFFontHeight[i]) {
				SeparateNumber(fmt);
				fmt.Cat('0' + i);
				return fmt;
			}
		fmt.Cat(NFormat("+%d", tabs(b.font.GetHeight())));
	}
	return fmt;
}

static String sFmtNumber(char c, int former, int current)
{
	return former != current ? NFormat("%c%d", c, current) : String();
}

String QTFEncodeParaFormat(const Paragraph::Style& format, const Paragraph::Part& charformat,
                           const Paragraph::Style& style, const Paragraph::Part& charstyle,
                           bool savecharformat)
{
	String qtf;
	if(format.align != style.align)
		switch(format.align) {
		case ALIGN_LEFT:    qtf << '<'; break;
		case ALIGN_RIGHT:   qtf << '>'; break;
		case ALIGN_CENTER:  qtf << '='; break;
		case ALIGN_JUSTIFY: qtf << '#'; break;
		}
	qtf << sFmtNumber('l', style.lm, format.lm)
	    << sFmtNumber('r', style.rm, format.rm)
	    << sFmtNumber('i', style.indent, format.indent)
	    << sFmtNumber('b', style.before, format.before)
	    << sFmtNumber('a', style.after, format.after);
	if(style.bullet != format.bullet) {
		qtf << 'O';
		qtf << (format.bullet ? "0" : "_");
	}
	if(savecharformat)
		qtf << CharFmt(charstyle, charformat);
	qtf << " ";
	return qtf;
}

String QTFEncodePara(const Paragraph& p,
                     const Paragraph::Style& style, const Paragraph::Part& charstyle)
{
	Paragraph::Part paraformat;
	paraformat.font = p.GetParaFont();
	String qtf = QTFEncodeParaFormat(p.GetStyle(), paraformat, style, charstyle,
		                             p.GetCount() == 0);
	int d = qtf.GetLength();
	for(int i = 0; i < p.GetCount(); i++) {
		const Paragraph::Part& part = p.GetParts()[i];
		if(part.pr) {
//			const RichObject& object = part.object;
			Size sz = part.sz;
			qtf << "\r\n";
			qtf << "@@" << RichObjectTypeDrawingTypeName() << ':' << sz.cx
			    << '*' << sz.cy << "\r\n";
			DrawingDraw ddraw(sz);
			part.pr.Paint(ddraw, sz, Black, White, 0);
			RichObjectTypeDrawingData dwg;
			dwg.drawing = ddraw;
			dwg.dot_size = sz;
			StringStream datastrm;
			datastrm % dwg;
			String data = datastrm;
			const char *q = data.Begin();
			const char *slim = data.End();
			int n = 0;
			qtf.Reserve(qtf.GetLength() + 8 * data.GetLength() / 7);
			while(q < slim - 7) {
				byte seven = 0;
				byte data[8];
				data[0] = ((q[0] & 0x80) >> 7) |
				          ((q[1] & 0x80) >> 6) |
				          ((q[2] & 0x80) >> 5) |
				          ((q[3] & 0x80) >> 4) |
				          ((q[4] & 0x80) >> 3) |
				          ((q[5] & 0x80) >> 2) |
				          ((q[6] & 0x80) >> 1) |
				          0x80;
				data[1] = q[0] | 0x80;
				data[2] = q[1] | 0x80;
				data[3] = q[2] | 0x80;
				data[4] = q[3] | 0x80;
				data[5] = q[4] | 0x80;
				data[6] = q[5] | 0x80;
				data[7] = q[6] | 0x80;
				qtf.Cat(data, 8);
				if(++n % 10 == 0)
					qtf << "\r\n";
				q += 7;
			}
			while(q < slim) {
				byte seven = 0;
				const char *lim = slim;
				const char *s;
				for(s = q; s < lim; s++)
					seven = (seven >> 1) | (*s & 0x80);
				seven >>= 8 - (lim - q);
				qtf.Cat(seven | 0x80);
				for(s = q; s < lim; s++)
					qtf.Cat(*s | 0x80);
				if(++n % 10 == 0)
					qtf << "\r\n";
				q += 7;
			}
		}
		else {
			String cf = CharFmt(charstyle, part);
			if(!cf.IsEmpty()) {
				qtf << '[' << cf << ' ';
				d += cf.GetLength() + 2;
			}
			for(const char *s = part.text.Begin(); s != part.text.End(); s++) {
				byte c = *s;
				if(c < 128) {
					if(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9'
					|| c == ' ' || c == '.' || c == ',' || c == '?' || c == '(' || c == ')'
					|| c == ';')
					{
						qtf.Cat(c);
						d++;
					}
					else if(c == 9)
					{
						qtf.Cat(127);
						d++;
					}
					else if(c == 31)
					{
						qtf.Cat('_');
						d++;
					}
					else
					{
						qtf.Cat('`');
						qtf.Cat(c);
						d += 2;
					}
					if(d > 60 && c == ' ')
					{
						qtf.Cat("\r\n");
						d = 0;
					}
				}
				else
				{
					qtf.Cat(c);
					d++;
				}
				if(d > 80)
				{
					qtf.Cat("\r\n");
					d = 0;
				}
			}
			if(!cf.IsEmpty()) {
				d++;
				qtf << ']';
			}
		}
	}
	return qtf;
}

String AsQtf(const Document& text)
{
	Paragraph::Style empty_format;
	empty_format.Init();
	Paragraph::Part empty_part;
	String qtf;
//	qtf << "[";
	for(int i = 0; i < text.GetCount(); i++)
		if(text.IsParagraph(i)) {
			const Paragraph& p = text.GetParagraph(i);
			if(i) qtf << "\r\n&\r\n";
			qtf << "[";
			qtf << QTFEncodePara(p, empty_format, empty_part);
			qtf << "]";
		}
	qtf << "\r\n"; // ]";
	return qtf;
}

}
