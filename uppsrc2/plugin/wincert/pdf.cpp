#include "wincert.h"

class PDFInfo {
public:
	struct Reference : Moveable<Reference> {
		Reference(const Nuller& = Null) : index(0) {}
		Reference(int index_, word revision_) : index(index_), revision(revision_) {}

		String ToString() const;

		bool IsNullInstance() const { return index == 0; }

		int  index;
		word revision;
	};

	PDFInfo();

	void         Clear();

	bool         Open(const String& pdf_data);

public:
	bool         SkipWhite();
	bool         Check(const char *text);
	int          ScanInteger();
	Reference    ScanReference();

	bool         ParseXRef(int startxref);
	bool         ParseRoot();
	bool         ParsePageTree(Reference root);

	bool         Seek(Reference ref);

public:
	enum { MAX_PDF_OBJECTS = 1000000 };

	String       pdf_data;
	Vector<int>  objects;
	Vector<word> revisions;
	Vector<Reference> pages;
	int          startxref;
	Reference    trailer_root;
	Reference    trailer_info;
	Reference    page_tree;
	String       root_object;
	const char   *term;
};

String PDFInfo::Reference::ToString() const
{
	StringBuffer out;
	out << index << ' ' << (int)revision << " R";
	return out;
}

PDFInfo::PDFInfo()
{
	Clear();
}

void PDFInfo::Clear()
{
	pdf_data = Null;
	objects.Clear();
	revisions.Clear();
	startxref = -1;
	trailer_root = Null;
	trailer_info = Null;
	page_tree = Null;
	root_object = Null;
	term = NULL;
}

bool PDFInfo::Open(const String& pdf_data_)
{
	Clear();
	pdf_data = pdf_data_;
	const char *begin = pdf_data.Begin();
	term = pdf_data.End();
	while(term > begin && (byte)term[-1] <= ' ')
		term--;
	if(term - begin <= 5 || memcmp(term - 5, "%%EOF", 5))
		return false;
	term -= 5;
	while(term > begin && !IsDigit(*--term))
		;
	while(term > begin && IsDigit(term[-1]))
		term--;
	startxref = ScanInt(term);
	if(!ParseXRef(startxref))
		return false;
	if(!ParseRoot() || !ParsePageTree(page_tree))
		return false;
	return true;
}

bool PDFInfo::ParseXRef(int start)
{
	if(start < 0 || start >= pdf_data.GetLength() - 20)
		return false;
	term = pdf_data.GetIter(start);
	if(!Check("xref"))
		return false;
	while(!Check("trailer")) {
		if(!SkipWhite())
			return false;
		int start = ScanInteger();
		int count = ScanInteger();
		if(start < 0 || count <= 0 || start + count > MAX_PDF_OBJECTS || !SkipWhite())
			return false;
		int end = start + count;
		if(objects.GetCount() < end) {
			objects.SetCountR(end, -1);
			revisions.SetCountR(end, 0xffffu);
		}
		for(int i = 0; i < count; i++) {
			int offset = ScanInteger();
			int rev = ScanInteger();
			if(!SkipWhite())
				return false;
			if(*term != 'f' && *term != 'n')
				return false;
			if(*term++ == 'f') {
				offset = 0;
				rev = 0xffffu;
			}
			if(objects[start + i] == -1) {
				objects[start + i] = offset;
				revisions[start + i] = rev;
			}
		}
	}
	if(!Check("<<"))
		return false;
	while(!Check(">>")) {
		if(!SkipWhite())
			return false;
		if(Check("/Size")) {
			int sz = ScanInteger();
			if(sz < objects.GetCount())
				return false;
			objects.SetCount(sz, -1);
			revisions.SetCount(sz, 0xffffu);
		}
		else if(Check("/Root")) {
			Reference root = ScanReference();
			if(IsNull(trailer_root))
				trailer_root = root;
		}
		else if(Check("/Info")) {
			Reference info = ScanReference();
			if(IsNull(trailer_info))
				trailer_info = info;
		}
		else if(Check("/Prev")) {
			int prev = ScanInteger();
			const char *store_term = term;
			if(!ParseXRef(prev))
				return false;
			term = store_term;
		}
		else {
			while(*++term && *term != '/' && !(*term == '>' && term[1] == '>'))
				;
		}
	}
	return true;
}

bool PDFInfo::Seek(Reference r)
{
	if(r.index <= 0 || r.index >= objects.GetCount() || objects[r.index] == -1)
		return false;
	term = pdf_data.GetIter(objects[r.index]);
	int index = ScanInteger();
	if(index != r.index)
		return false;
	int rev = ScanInteger();
	if(rev != r.revision)
		return false;
	if(!Check("obj") || !Check("<<"))
		return false;
	return true;
}

bool PDFInfo::ParseRoot()
{
	if(!Seek(trailer_root))
		return false;
	const char *begin = term;
	while(*term) {
		if(Check("/Pages")) {
			page_tree = ScanReference();
		}
		else
		if(*term == '>' && term[1] == '>')
			break;
		else
			term++;
	}
	root_object = String(begin, term);
	return true;
}

bool PDFInfo::ParsePageTree(Reference node)
{
	if(!Seek(node))
		return false;
	if(!Check("/Type"))
		return false;
	if(Check("/Pages")) {
		if(!Check("/Kids") || !Check("["))
			return false;
		while(*term && !Check("]")) {
			Reference kid = ScanReference();
			const char *stack = term;
			if(!ParsePageTree(kid))
				return false;
			term = stack;
		}
		return true;
	}
	else if(Check("/Page")) {
		pages.Add(node);
		return true;
	}
	else
		return false;
}

bool PDFInfo::SkipWhite()
{
	while(*term && (byte)*term <= ' ')
		term++;
	return !!*term;
}

bool PDFInfo::Check(const char *text)
{
	if(!SkipWhite())
		return false;
	int l = strlen(text);
	if(memcmp(term, text, l))
		return false;
	term += l;
	return true;
}

int PDFInfo::ScanInteger()
{
	if(!SkipWhite())
		return Null;
	return ScanInt(term, &term);
}

PDFInfo::Reference PDFInfo::ScanReference()
{
	int obj = ScanInteger();
	if(IsNull(obj))
		return Reference();
	int rev = ScanInteger();
	if(IsNull(rev))
		return Reference();
	if(SkipWhite() && *term == 'R') {
		term++;
		return Reference(obj, rev);
	}
	return Reference();
}

String CertSignPDF(PCCERT_CONTEXT cert_context, const String& pdf_data)
{
	PDFInfo pdfinfo;
	if(!pdfinfo.Open(pdf_data) || pdfinfo.pages.IsEmpty())
		return String::GetVoid();
	StringBuffer out;
	out.Cat(pdf_data);
	int obj_index = pdfinfo.objects.GetCount();
	int sign_object = obj_index++;
	int sign_dict = obj_index++;
	int new_root = obj_index++;
	int sign_off = out.GetLength();
	out << sign_object << " 0 obj\n"
	"<</FT/Sig/T(Signature1)"
	"/V " << sign_dict << " 0 R"
	"/F 132/Type/Annot/Subtype/Widget/Rect[0 0 0 0]";
	if(!IsNull(pdfinfo.page_tree))
		out << "/P " << pdfinfo.pages[0];
	out << ">>\n"
	"endobj\n";
	int dict_off = out.GetLength();
	Time tm = GetSysTime();
	out << sign_dict << " 0 obj\n"
	"<</Type/Sig/Filter/Adobe.PPKLite/SubFilter/adbe.pkcs7.detached"
	"/Reason()"
	"/Location()"
	"/ContactInfo()"
	"/M(D:" << NFormat("%04d%02d%02d%02d%02d%02d+02'00'",
		tm.year, tm.month, tm.day, tm.hour, tm.minute, tm.second)
	<< ")/ByteRange [0 ";
	int br_offset = out.GetLength();
	int cert_length = 4096;
	String tail = "/Contents <";
	static const int RCL_CONT = 60;
	for(;;) {
		out.SetLength(br_offset);
		out.Cat(' ', RCL_CONT);
		out.Cat(tail);
		int cert_offset = out.GetLength() - 1;
		out.Cat('0', cert_length);
		int post_offset = out.GetLength() + 1;
		out << ">>>\n"
		"endobj\n";
		int root_off = out.GetLength();
		out
		<< new_root << " 0 obj\n"
		"<<" << pdfinfo.root_object
		<< "/AcroForm<</Fields[" << sign_object << " 0 R]/SigFlags 3>>>>\n"
		"endobj\n";
		int xref_offset = out.GetLength();
		out << "xref\n"
		<< sign_object << " 3\n"
		<< FormatIntDec(sign_off, 10, '0') << " 00000 n\r\n"
		<< FormatIntDec(dict_off, 10, '0') << " 00000 n\r\n"
		<< FormatIntDec(root_off, 10, '0') << " 00000 n\r\n"
		"trailer\n"
		"<</Size " << obj_index << "/Root " << new_root << " 0 R";
		if(!IsNull(pdfinfo.trailer_info))
			out << "/Info " << pdfinfo.trailer_info;
		out <<
		"/ID [ <" << Uuid::Create() << "> <" << Uuid::Create() << "> ]\n"
		"/Prev " << pdfinfo.startxref
		<< ">>\n"
		"startxref\n"
		<< xref_offset
		<< "\n"
		"%%EOF\n";
		int end_offset = out.GetLength();

		String rcl_fmt = NFormat("%d %d %d ]", cert_offset,
			post_offset, end_offset - post_offset);
		ASSERT(rcl_fmt.GetLength() <= RCL_CONT);
		memcpy(out.Begin() + br_offset, rcl_fmt, rcl_fmt.GetLength());

		Vector<const BYTE *> sections;
		Vector<DWORD> sizes;
		sections.SetCount(2);
		sizes.SetCount(2);
		sections[0] = (const BYTE *)out.Begin();
		sizes[0] = cert_offset;
		sections[1] = (const BYTE *)out.Begin() + post_offset;
		sizes[1] = end_offset - post_offset;
		int sig_len = 2 * CertSignLength(cert_context, sections, sizes);
		if(sig_len <= cert_length) {
			String signature = BinHexEncode(CertSign(cert_context, sections, sizes));
			ASSERT(signature.GetLength() <= cert_length);
			memcpy(out.Begin() + cert_offset + 1, signature, signature.GetLength());
			return out;
		}
		cert_length = sig_len;
	}
}
