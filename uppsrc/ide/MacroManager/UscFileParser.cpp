#include "MacroManager.h"

#define METHOD_NAME "UscFileParser::" << UPP_FUNCTION_NAME << "(): "

namespace Upp {

UscFileParser::UscFileParser(const String& filePath)
	: filePath(filePath)
{}

UscFileParser::MacroList UscFileParser::Parse()
{
	MacroList ret;
	
	String fileContent = LoadFile(filePath);
	if(fileContent.IsEmpty()) {
		Logw() << METHOD_NAME << "Following file \"" << filePath << "\" doesn't exist or is empty.";
		return ret;
	}

	try {
		CParser parser(fileContent, filePath);
		
		while (!parser.IsEof()) {
			String comment = TrimLeft(String(parser.GetSpacePtr(), parser.GetPtr()));

			if (!parser.IsId())
				return ret;
			
			const char* prototypeBegin = parser.GetPtr();
			String id = parser.ReadId();
			
			if(id.IsEqual("fn"))
				ReadFunction(parser, comment, prototypeBegin, ret);
			else
			if(id.IsEqual("macro"))
				ReadMacro(parser, comment, prototypeBegin, ret);
			else
				return ret;
		}
	}
	catch (const CParser::Error& error) {
		Logw() << METHOD_NAME << "Parsing file \"" << filePath << "\" failed with error: " << error << ".";
	}
	
	return ret;
}

bool UscFileParser::IsValid()
{
	return !Parse().IsEmpty();
}

void UscFileParser::ReadFunction(CParser& parser, const String& comment, const char* prototypeBegin, MacroList& list)
{
	String fileName = parser.GetFileName();
	MacroElement fn(MacroElement::Type::FUNCTION, fileName, parser.GetLine(), comment);
	
	if(!parser.IsId()) {
		FindNextElement(parser);
		return;
	}
	fn.name = parser.ReadId();
	if(!parser.Char('(')) {
		FindNextElement(parser);
		return;
	}
	fn.args = ReadArgs(parser);
	
	FinishRead(parser, prototypeBegin, fn, list);
}

void UscFileParser::ReadMacro(CParser& parser, const String& comment, const char* prototypeBegin, MacroList& list)
{
	String fileName = parser.GetFileName();
	MacroElement macro(MacroElement::Type::MACRO, fileName, parser.GetLine(), comment);
	
	if(!parser.IsString()) {
		FindNextElement(parser);
		return;
	}
	macro.name = String() << (parser.IsString() ? parser.ReadString() : "");
	if(parser.Char(':')) {
		if(!parser.IsString()) {
			FindNextElement(parser);
			return;
		}

		macro.name << " : " << (parser.IsString() ? parser.ReadString() : "");
	}
	if (!parser.IsChar('{'))
		ReadKeyDesc(parser);

	FinishRead(parser, prototypeBegin, macro, list);
}

void UscFileParser::FinishRead(CParser& parser, const char* prototypeBegin, MacroElement& element, MacroList& list)
{
	const char* bodyBegin = parser.GetPtr();

	element.prototype = String(prototypeBegin, bodyBegin);
	
	if (!parser.Char('{')) {
		FindNextElement(parser);
		return;
	}

	Upp::SkipBlock(parser);

	if(parser.GetSpacePtr() > bodyBegin)
		element.code = String(bodyBegin, parser.GetSpacePtr());

	list.Add(element);
}

void UscFileParser::FindNextElement(CParser& parser)
{
	while(!parser.IsEof() && !parser.IsId("fn") && !parser.IsId("macro"))
		parser.SkipTerm();
}

String UscFileParser::ReadArgs(CParser& parser)
{
	int level = 1;
	parser.Char('(');
	
	String ret = "(";
	while(level > 0 && !parser.IsEof()) {
		if (parser.Char('('))
			level++;
		else
		if (parser.Char(')'))
			level--;
		else
			ret << parser.GetChar();
	}
	ret << ")";

	return ret;
}

String UscFileParser::ReadKeyDesc(CParser& parser)
{
	if(!parser.IsId())
		return String();

	String ret = parser.ReadId();

	while(!parser.IsEof() && parser.Char('+')) {
		if(parser.IsId())
			ret << "+" << parser.ReadId();

		else
		if(parser.IsNumber())
			ret << "+" << parser.ReadInt();
		else
			break;
	}

	return ret;
}

}
