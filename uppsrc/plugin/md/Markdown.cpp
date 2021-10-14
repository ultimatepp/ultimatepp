#include "Markdown.h"

#define LLOG(x)	// DLOG("MarkdownConverter: " << x)

namespace Upp {

class sMarkdownContext : NoCopy {
	struct Block
	{
		MD_BLOCKTYPE	type;
		String			text;
		Value			data;
		int				level;
		Block			*parent;
		Array<Block>	children;
		Block(MD_BLOCKTYPE t, Block *p);
	};

	enum NestedBlockPosition {
		BLOCK_IN_OLIST   = 1,
		BLOCK_IN_ULIST   = 2,
		BLOCK_IN_QUOTE   = 4,
		BLOCK_IN_THEAD   = 8,
		BLOCK_IN_TBODY   = 16
	};

	Array<Block>	document;
	Block*			current_block;
	int				current_level;

	String	Compose(const Array<Block>& doc, int data = 0, bool notext = false, dword flags = 0) const;

public:
	void	BeginBlock(MD_BLOCKTYPE type, void *detail);
	void	EndBlock(MD_BLOCKTYPE type, void *detail);

	String	ToQtf()									{ return Compose(document); }

	sMarkdownContext& operator<<(const String& s)	{ ASSERT(current_block); current_block->text << s; return *this; }
	sMarkdownContext& operator<<(const char *s)		{ ASSERT(current_block); current_block->text << s; return *this; }

	sMarkdownContext()
		: current_block(nullptr)
		, current_level(0)
		, no_text(false)
	{}

	VectorMap<String, String> images;
	bool no_text; // Image alt text is propagated as normal text. This is to suppress it.
};

sMarkdownContext::Block::Block(MD_BLOCKTYPE t, Block *p)
: type(t)
, level(0)
, parent(p)
{
}

void sMarkdownContext::BeginBlock(MD_BLOCKTYPE type, void *detail)
{
	if(!current_block) {
		current_block = &document.Create<Block>(type, current_block);
	}
	else {
		current_block = &current_block->children.Create<Block>(type, current_block);
	}

	switch(type) {
	case MD_BLOCK_UL:
		current_block->data  = reinterpret_cast<MD_BLOCK_UL_DETAIL*>(detail)->mark;
		current_block->level = ++current_level;
		break;
	case MD_BLOCK_OL:
		current_block->data  = '1';
		current_block->level = ++current_level;
		break;
	case MD_BLOCK_H:
		current_block->data  = (int) reinterpret_cast<MD_BLOCK_H_DETAIL*>(detail)->level;
		current_block->level = current_level;
		break;
	default:
		current_block->level = current_level;
		break;
	}
}

void sMarkdownContext::EndBlock(MD_BLOCKTYPE type, void *detail)
{
	if(findarg(type, MD_BLOCK_UL, MD_BLOCK_OL) >= 0)
		--current_level;

	if(current_block)
		current_block = current_block->parent;
}

String sMarkdownContext::Compose(const Array<Block>& doc, int data, bool notext, dword flags) const
{
	// TODO:
	// 1) Refactor this method.
	// 2) Make certain block styles and page properties (e.g. margins, indentation, etc.) configurable.

	String txt;

	for(int i = 0; i < doc.GetCount(); i++) {
		const Block& b = doc[i];
		switch(b.type) {
		case MD_BLOCK_DOC:
		{
			txt << "[G2 "
				<< Compose(b.children, data, false, flags)
				<< "&]";
			break;
		}
		case MD_BLOCK_HR:
		{
			txt << "[H1;L0;h(220.220.220) &]";
			break;
		}
		case MD_BLOCK_H:
		{
			txt << "[*;a20;b20;pht300;"
				<< clamp(6 - b.data.To<int>(), 1, 6)
				<< " "
				<< b.text
				<< "&]";
			break;
		}
		case MD_BLOCK_UL:
		{
			txt << Compose(b.children, b.data, false, flags|BLOCK_IN_ULIST);
			break;
		}
		case MD_BLOCK_OL:
		{
			txt << "[N!;wt300; "
				<< Compose(b.children, b.data, false, flags|BLOCK_IN_OLIST)
				<< "&]";
			break;
		}
		case MD_BLOCK_LI:
		{
			bool q = b.text.IsEmpty();
			if(!q) {
				txt << "[b20;l"
					<< b.level * ((flags & BLOCK_IN_QUOTE) ? 100 : 200)
					<< ";i200;"
					<< decode(data, '*', "OO ", '-', "O1 ", '+', "O2 ", "N1 ")
					<< b.text
					<< "&]";
			}
			txt << Compose(b.children, data, q, flags);
			break;
		}
		case MD_BLOCK_P:
		{
			if(!b.level) {
				txt << "[b20;wt300 " << b.text << " &]";
			}
			else
			if(notext && !i) {
				txt << "[b20;wt300;l"
					<< b.level * ((flags & BLOCK_IN_QUOTE) ? 20 : 200)
					<< ";i200;"
					<< decode(data, '*', "OO ", '-', "O1 ", '+', "O2 ", "N1 ")
					<< b.text
					<< "&]";
			}
			else {
				txt << "[b20;wt300;l"
					<< b.level * ((flags & BLOCK_IN_QUOTE) ? 40 : 400)
					<< ";O_ "
					<< b.text
					<< "&]";
			}
			break;
		}
		case MD_BLOCK_CODE:
		case MD_BLOCK_HTML:		// Treat HTML as code block....
		{
			txt << "{{10000;A20;B20;<"
				<< b.level * ((flags & BLOCK_IN_QUOTE) ? 10 : 400)
			    << ";@(250.250.250);F(230.230.230) [i10;C;2;@5;< "
				<< b.text
				<< " ]}}&";
			break;
		}
		case MD_BLOCK_QUOTE:
		{
			txt << "{{1:500;G4;g20;F0;f0;<"
				<< b.level * ((flags & BLOCK_IN_QUOTE) ? 10 : 400)
				<< ((flags & BLOCK_IN_QUOTE) ? "" : ";A20;B20")
				<< " ::  [i10;<; "
				<< Nvl(b.text, "")
				<< Compose(b.children, data, false, flags|BLOCK_IN_QUOTE)
				<< " ]}}&";
			break;
		}
		case MD_BLOCK_TABLE:
		{
			txt << "{{"
				<< Compose(b.children, data, false, flags)
				<< "}}&";
			break;
		}
		case MD_BLOCK_THEAD:
		{
			txt << Compose(b.children, data, false, flags|BLOCK_IN_THEAD);
			break;
		}
		case MD_BLOCK_TBODY:
		{
			txt << Compose(b.children, data, false, flags|BLOCK_IN_TBODY);
			break;
		}
		case MD_BLOCK_TR:
		{
			int n = b.children.GetCount();
			if(flags & BLOCK_IN_THEAD) {
				for(int j = 0; j < n; j++) {
					txt << '1';
					if(j < n - 1)
						txt << ':';
					else {
						txt << "A20;B20;";
						txt << "@(220.225.230);G(220.220.220);<"
							<< b.level * ((flags & BLOCK_IN_QUOTE) ? 10 : 200)
							<< " ";
					}
				}
			}
			for(int j = 0; j < n; j++) {
				const Block& bb = b.children[j];
				if(j == 0 && bb.type == MD_BLOCK_TD)
					txt << "::@2 ";
				txt << bb.text;
				if(j < n - 1)
					txt << "||";
			}
			break;
		}
		default:
			//txt << b.text;
			break;
		}
	}

	return txt;
}

static String sDeQtfMd(const char *s)
{
	// Here we duplicate DeQtf() function, because we don't want to bring in the RichText package.

	StringBuffer r;
	for(; *s; s++) {
		if(*s == '\n')
			r.Cat('&');
		else {
			if((byte) *s > ' ' && !IsDigit(*s) && !IsAlpha(*s) && (byte) *s < 128)
				r.Cat('`');
			r.Cat(*s);
		}
	}
	return String(r); // Make compilers happy...
}

static int sParseMdObjects(MD_PARSER& parser, const String& txt, void *ctx)
{
	// NOP (ATM...)
	parser.enter_block = [](MD_BLOCKTYPE type, void *detail, void *udata) -> int { return 0; };
	parser.leave_block = [](MD_BLOCKTYPE type, void *detail, void *udata) -> int { return 0; };
	parser.leave_span = [](MD_SPANTYPE type, void *detail, void *udata) -> int   { return 0; };
	parser.text = [](MD_TEXTTYPE type, const MD_CHAR *text, MD_SIZE size, void* udata) -> int { return 0; };

	parser.enter_span = [](MD_SPANTYPE type, void *detail, void *udata) -> int
	{
		auto& ctx = *reinterpret_cast<sMarkdownContext*>(udata);

		if(type == MD_SPAN_IMG) // Handle images
		{
			auto *q = reinterpret_cast<MD_SPAN_IMG_DETAIL*>(detail);
			String url(q->src.text, q->src.size);
			ctx.images.Add(url);
		}

		return 0;
	};

	int rc = md_parse((const MD_CHAR*)~txt, (MD_SIZE) txt.GetLength(), &parser, ctx);
	return rc;
}

static int sParseMdDocument(MD_PARSER& parser, const String& txt, void *ctx)
{
	parser.enter_block = [](MD_BLOCKTYPE type, void *detail, void *udata) -> int
	{
		reinterpret_cast<sMarkdownContext*>(udata)->BeginBlock(type, detail);
		return 0;
	};

	parser.leave_block = [](MD_BLOCKTYPE type, void *detail, void *udata) -> int
	{
		reinterpret_cast<sMarkdownContext*>(udata)->EndBlock(type, detail);
		return 0;
	};

	parser.enter_span = [](MD_SPANTYPE type, void *detail, void *udata) -> int
	{
		auto& ctx = *reinterpret_cast<sMarkdownContext*>(udata);

		switch(type) {
		case MD_SPAN_A:
		{
			auto *q = reinterpret_cast<MD_SPAN_A_DETAIL*>(detail);
			ctx << "[^" << String(q->href.text, q->href.size) << "^ ";
			break;
		}
		case MD_SPAN_IMG:
		{
			auto *q = reinterpret_cast<MD_SPAN_IMG_DETAIL*>(detail);
			String url(q->src.text, q->src.size);
			int i = ctx.images.Find(url);
			if(i >= 0 && !IsNull(ctx.images[i])) {
				ctx.no_text = true;
				ctx << ctx.images[i];
			}
			else {
				ctx.no_text = false;
				ctx << "[^" << url << "^ ";
			}
			break;
		}
		case MD_SPAN_WIKILINK:
		{
			auto *q = reinterpret_cast<MD_SPAN_WIKILINK_DETAIL*>(detail);
			ctx << "[^" << String(q->target.text, q->target.size) << "^ ";
			break;
		}
		default:
		{
			ctx << decode(type,
					MD_SPAN_U,		"[_ ",
					MD_SPAN_EM,		"[/ ",
					MD_SPAN_DEL,	"[- ",
					MD_SPAN_CODE,	"[C;@5;$(245.245.245) ",
					MD_SPAN_STRONG,	"[* ", "");
			break;
		}}
		return 0;
	};

	parser.leave_span = [](MD_SPANTYPE type, void *detail, void *udata) -> int
	{
		auto& ctx = *reinterpret_cast<sMarkdownContext*>(udata);

		if(type == MD_SPAN_IMG) {
			if(!ctx.no_text) {
				ctx << " ]";
			}
			ctx.no_text = false;
		}
		else
		if(findarg(type,
			MD_SPAN_A,
			MD_SPAN_U,
			MD_SPAN_EM,
			MD_SPAN_DEL,
			MD_SPAN_CODE,
			MD_SPAN_STRONG,
			MD_SPAN_WIKILINK) >= 0)
			 ctx << "]";

		return 0;
	};

	parser.text = [](MD_TEXTTYPE type, const MD_CHAR *text, MD_SIZE size, void* udata) -> int
	{
		auto& ctx = *reinterpret_cast<sMarkdownContext*>(udata);
		if(!ctx.no_text)
			ctx << decode(type,
					MD_TEXT_NULLCHAR,	"?",
					MD_TEXT_BR,			"&",
					MD_TEXT_SOFTBR,		" ",	// TODO: See if there is a way to properly imitate this in qtf...
					(const char*) ~sDeQtfMd(String((const char*) text, size)));
		return 0;
	};

	int rc = md_parse((const MD_CHAR*)~txt, (MD_SIZE) txt.GetLength(), &parser, ctx);
	return rc;
}

String MarkdownConverter::ToQtf(const String& mdtext)
{
	MD_PARSER parser;
	parser.abi_version = 0;
	parser.flags  = flags;
	parser.syntax = nullptr;

#ifdef _DEBUG
	parser.debug_log = [](const char *msg, void *udata) -> void
	{
		LLOG(msg);
	};
#else
	parser.debug_log = nullptr;
#endif

	sMarkdownContext ctx;
	int rc = sParseMdObjects(parser, mdtext, &ctx); // Pass 1 (collect the image information)
	if(rc == 0) {
		ctx.no_text = false;
		if(ctx.images.GetCount())
			WhenImages(ctx.images);
		rc = sParseMdDocument(parser, mdtext, &ctx); // Pass 2 (parse the document)
	}
	return rc ? String::GetVoid() : ctx.ToQtf();
}

}
