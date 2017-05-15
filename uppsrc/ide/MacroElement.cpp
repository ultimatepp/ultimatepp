#include "ide.h"

MacroElement::MacroElement(Type type, const String& fileName, int line, const String& comment)
	: type(type)
	, comment(comment)
	, fileName(fileName)
	, line(line)
{}

Image MacroElement::GetImage(Type type)
{
	switch(type)
	{
		case(Type::MACRO):
			return IdeImg::Macro();
		case(Type::FUNCTION):
			return IdeImg::Fn();
		case(Type::UNKNOWN):
			return Image();
	}
}
