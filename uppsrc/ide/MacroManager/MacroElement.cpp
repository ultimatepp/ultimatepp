#include "MacroManager.h"

namespace Upp {

MacroElement::MacroElement(Type type, const String& fileName, int line, const String& comment)
	: type(type)
	, comment(comment)
	, fileName(fileName)
	, line(line)
{}

Image MacroElement::GetImage() const
{
	switch(type)
	{
		case(Type::MACRO):
			return MacroManagerImg::Macro();
		case(Type::FUNCTION):
			return MacroManagerImg::Fn();
		case(Type::UNKNOWN): // silence CLANG warning
			break;
	}
	return Image();
}

String MacroElement::GetContent() const
{
	String content = TrimBoth(comment);
	if(content.GetCount())
		content << "\n";

	content << prototype << code;
	
	return content;
}

}
