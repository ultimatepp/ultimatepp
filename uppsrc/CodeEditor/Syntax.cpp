#include "CodeEditor.h"

NAMESPACE_UPP

void EditorSyntax::Clear()
{
}

void EditorSyntax::ScanSyntax(const wchar *ln, const wchar *e, int line, int tab_size)
{
}

void EditorSyntax::Serialize(Stream& s)
{
}

void EditorSyntax::IndentInsert(CodeEditor& editor, int chr, int count)
{
	editor.InsertChar(chr, count);
}

bool EditorSyntax::CheckBrackets(CodeEditor& e, int& bpos0, int& bpos)
{
	return false;
}

bool EditorSyntax::CanAssist() const
{
	return false;
}

void EditorSyntax::Highlight(const wchar *s, const wchar *end, HighlightOutput& hls, CodeEditor *editor, int line, int pos)
{
}

Vector<IfState> EditorSyntax::PickIfStack()
{
	return Vector<IfState>();
}

Color EditorSyntax::IfColor(char c)
{
	switch(c){
	case IfState::IF:          return LtBlue();
	case IfState::ELIF:        return Gray();
	case IfState::ELSE:        return Green();
	case IfState::ELSE_ERROR:  return LtRed();
	case IfState::ENDIF_ERROR: return LtMagenta();
	default:                   return Null;
	}
}

void EditorSyntax::CheckSyntaxRefresh(CodeEditor& e, int pos, const WString& text)
{
}

EditorSyntax::~EditorSyntax() {}

END_UPP_NAMESPACE
