#include "ide.h"

using namespace astyle;

/////////////////////////////////////////////////////////////////////////////////////////
// CLASS ASStreamIterator
class ASStreamIterator : public ASSourceIterator
{
	protected:

		enum EolMode { crlf, lf, cr, lfcr } ;
		WString const &src;
		int p;
		EolMode eolMode;

	public:
		ASStreamIterator(WString const &Src);
		virtual ~ASStreamIterator() {}
		virtual bool hasMoreLines() const;
		virtual WString nextLine();

		WString getEol();

}; // END Class ASStreamIterator

// Constructor Class ASStreamIterator
ASStreamIterator::ASStreamIterator(WString const &Src) : ASSourceIterator(), src(Src)
{
	// initializes buffer pointer
	p = 0;

	// initializes Eol Mode
	eolMode = crlf;

} // END Constructor Class ASStreamIterator


WString ASStreamIterator::getEol()
{
	switch(eolMode)
	{
		case cr:
			return "\r";
		case lf:
			return "\n";
		case lfcr:
			return "\r\n";
		default:
			return "\n\r";
	}

} // END ASStreamIterator::getEol()

bool ASStreamIterator::hasMoreLines() const
{
	return src[p] != 0;

} // END ASStreamIterator::hasMoreLines()

WString ASStreamIterator::nextLine()
{
	int p2 = p;
	wchar c1, c2;
	while(src[p2] && src[p2] != '\n' && src[p2] != '\r')
		p2++;
	WString line = src.Mid(p, p2 - p);

	if( (c1 = src[p2]) != 0)
	{
		p2++;
		c2 = src[p2];
		if(c1 == '\r' && c2 == '\n')
		{
			p2++;
			eolMode = crlf;
		}
		else if(c1 == '\n' && c2 == '\r')
		{
			p2++;
			eolMode = lfcr;
		}
		else if(c1 == '\n')
			eolMode = lf;
		else
			eolMode = cr;
	}
	p = p2;

	return line;

} // END ASStreamIterator::nextLine()

/////////////////////////////////////////////////////////////////////////////////////////
// Formats a string of code with a given formatter
WString Ide::FormatCodeString(WString const &Src, ASFormatter &Formatter)
{
	// Creates the output string
	WString Dest;

	// Creates the iterator for the formatter
	ASStreamIterator streamIterator(Src);

	// Initializes the formatter to work on selected stream
	Formatter.init(&streamIterator);

	// Processes all text in source file and put them in output file
	while (Formatter.hasMoreLines())
		Dest << Formatter.nextLine() << streamIterator.getEol();

	// returns output string
	return Dest;

} // END Ide::FormatCodeString()

/////////////////////////////////////////////////////////////////////////////////////////
// Formats editor's code with Ide format parameters
void Ide::FormatCode()
{
	// Gets editor contents from editor
	WString Src;
	int l, h;
	bool sel = editor.GetSelection(l, h);
	if(sel)
		Src = editor.GetSelectionW();
	else
	 	Src = editor.GetW();

	// Instantiate the formatter object
	ASFormatter Formatter;

	// Sets up the formatter for C++ formatting
	Formatter.setCStyle();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Sets up astyle options
	Formatter.setBracketIndent(astyle_BracketIndent);
	Formatter.setNamespaceIndent(astyle_NamespaceIndent);
	Formatter.setBlockIndent(astyle_BlockIndent);
	Formatter.setCaseIndent(astyle_CaseIndent);
	Formatter.setClassIndent(astyle_ClassIndent);
	Formatter.setLabelIndent(astyle_LabelIndent);
	Formatter.setSwitchIndent(astyle_SwitchIndent);
	Formatter.setPreprocessorIndent(astyle_PreprocessorIndent);
	Formatter.setMaxInStatementIndentLength(astyle_MaxInStatementIndentLength);
	Formatter.setMinConditionalIndentLength(astyle_MinInStatementIndentLength);
	Formatter.setBreakClosingHeaderBracketsMode(astyle_BreakClosingHeaderBracketsMode);
	Formatter.setBreakElseIfsMode(astyle_BreakElseIfsMode);
	Formatter.setBreakOneLineBlocksMode(astyle_BreakOneLineBlocksMode);
	Formatter.setSingleStatementsMode(astyle_SingleStatementsMode);
	Formatter.setBreakBlocksMode(astyle_BreakBlocksMode);
	Formatter.setBreakClosingHeaderBlocksMode(astyle_BreakClosingHeaderBlocksMode);
	Formatter.setBracketFormatMode((astyle::BracketMode)astyle_BracketFormatMode);
	switch(astyle_ParensPaddingMode)
	{
		case PAD_INSIDE :
			Formatter.setParensInsidePaddingMode(true);
			Formatter.setParensOutsidePaddingMode(false);
			break;
		case PAD_OUTSIDE :
			Formatter.setParensInsidePaddingMode(false);
			Formatter.setParensOutsidePaddingMode(true);
			break;
		case PAD_BOTH :
			Formatter.setParensInsidePaddingMode(true);
			Formatter.setParensOutsidePaddingMode(true);
			break;
		default :
			Formatter.setParensOutsidePaddingMode(false);
			Formatter.setParensInsidePaddingMode(false);
			break;
	}
	Formatter.setParensUnPaddingMode(astyle_ParensUnPaddingMode);
	Formatter.setOperatorPaddingMode(astyle_OperatorPaddingMode);
	Formatter.setEmptyLineFill(astyle_EmptyLineFill);
	Formatter.setTabSpaceConversionMode(astyle_TabSpaceConversionMode);
	if(!indent_spaces)
		Formatter.setTabIndentation(editortabsize, true);
	else
		Formatter.setSpaceIndentation(indent_amount);

	WString Dest = FormatCodeString(Src, Formatter);

	editor.NextUndo();
	if(sel) {
		editor.Remove(l, h - l);
		editor.SetSelection(l, l + editor.Insert(l, Dest));
	}
	else {
		editor.Remove(0, editor.GetLength());
		editor.Insert(0, Dest);
	}
}
