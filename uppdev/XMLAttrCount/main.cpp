#include <ide/Common/Common.h>

void write_out( FileOut& out, const XmlNode& xml, int ident )
{
	switch( xml.GetType() )
	{
	case XML_TEXT:
		out << String(' ', ident) << "Text: " << xml.GetText() << "\n";
		break;
	case XML_TAG:
		out << String(' ', ident) << "Tag: " << xml.GetTag() << "\n";
		for ( int i = 0, total = xml.GetAttrCount(); i < total; i++ )
		{
			out << String( ' ', ident ) << "+> " << xml.AttrId(i) << ": " << xml.Attr(i) << "\n";
		}
		ident += 2;
		break;
	case XML_DOC:
		out << String(' ', ident) << "Doc:\n";
		break;
	default:
		out << String(' ', ident) << "**UNDEFINED TAG**\n";
		break;
	}

	for ( int i = 0, total = xml.GetCount(); i < total; i++ )
	{
		write_out(out, xml[i], ident + 2);
	}
}

GUI_APP_MAIN
{
	Package p;

	String filename("D:\\uppsrc\\ide\\Common\\Common.upp");

	p.Load(filename);

	XmlNode xml;

	XmlNode& makefile = xml.Add("makefile");

	XmlNode& lib = makefile.Add("lib");
	lib.SetAttr("id", GetFileName(filename));

	FileOut out("e:\\tst.dat");
	write_out(out, xml, 2);
}