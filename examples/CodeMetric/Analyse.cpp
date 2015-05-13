#include <CppBase/CppBase.h>
#include "Analyse.h"


bool ContainsAt(const String &source, const String &pattern, int pos = 0)
{
	return    pos >= 0
	       && pos + pattern.GetLength() <= source.GetLength()
	       && 0 == memcmp(source.Begin() + pos, pattern.Begin(), pattern.GetLength());
}

bool StartsWith(const String &source, const String &pattern)
{
	return ContainsAt(source, pattern, 0);
}

bool EndsWith(const String &source, const String &pattern)
{
	return ContainsAt(source, pattern, source.GetLength() - pattern.GetLength());
}

String InsertNestingToSignature(String natural, String nesting)
{
	if(StartsWith(nesting, "::"))
		nesting.Remove(0, 2);
	if(nesting.GetCount() && !EndsWith(nesting, "::"))
		nesting << "::";

	int pos = natural.Find('('); // find the first opening parenthesis
	pos--;
	while(pos >= 0 && !iscid(natural[pos])) // skip over non-id chars before paren.
		pos--;
	if(pos < 0) return "";
	while(pos >= 0 && iscid(natural[pos])) // skip over last id before paren
		pos--;
	natural.Insert(pos+1, nesting);
	return natural;
}

CodeMetric::CodeMetric(const String &fileContent) :
  orphanLines(0), blankLines(0), commentLines(0)
{
	StringStream stream(fileContent);
	CppBase base;
	Parser parser;
	parser.whenFnEnd = THISBACK(StoreMetric);

	parser.Do(stream, base, 0, 0,
	          "file", CNULL,
	          Vector<String>(),
	          Vector<String>(),
	          Index<String>());

	const SrcFile &srcFile = parser.getPreprocessedFile();

	commentLines = srcFile.commentLinesRemoved;
	blankLines   = srcFile.blankLinesRemoved;
	orphanLines  = parser.symbolsOutsideFunctions.GetStat(';');
	totalLLOC    = orphanLines;
	sumCC1 = sumCC2 = sumDepth = 0;

	for(int i = 0; i < functions.GetCount(); i++) {
		totalLLOC += functions[i].logicalLinesOfCode;
		sumCC1    += functions[i].cyclomaticComplexity1;
		sumCC2    += functions[i].cyclomaticComplexity2;
		sumDepth  += functions[i].scopeDepth;
	}
}


String CodeMetric::ToString() const
{
	String s;
	s << "LLOC: " << totalLLOC
	  << ", Blank: " << blankLines
	  << ", Comments: " << commentLines;
	if(errors != "")
		s << "\nErrors:\n" << errors;
	return s;
}

void CodeMetric::StoreError(int line, const String &msg)
{
	errors << "line " << line << ": " << msg << "\n";
}

int CodeMetric::LogicalLinesOfCode(const LexSymbolStat &symbolStat)
{
	static Vector<int> oneLiners(pick(
	  Vector<int>() << tk_if << tk_else << tk_switch << tk_case
	                << tk_for << tk_do << tk_while << tk_try << tk_catch
	                << tk_struct << tk_class << tk_namespace
	                << tk_public << tk_private << tk_protected
	                << ';'));
	return symbolStat.SumStat( oneLiners );
}

void CodeMetric::StoreMetric(const Parser::FunctionStat & functionStat)
{
	static Vector<int> cc1_symbols(pick(
	  Vector<int>() << tk_if << tk_case << tk_for << tk_while << tk_catch));

	static Vector<int> cc2_symbols(pick(
	  Vector<int>() << t_and << t_or << '?'));

	if(!functionStat.cppItem.impl)
		return;
	
	FunctionEntry &entry = functions.Add();

	entry.pos                   = functionStat.cppItem.line;
	entry.name                  = InsertNestingToSignature(functionStat.cppItem.natural,
	                                                       functionStat.scope);
	int cc1 = 1 + functionStat.symbolStat.SumStat( cc1_symbols );
	entry.cyclomaticComplexity1 = cc1;
	entry.cyclomaticComplexity2 = cc1 + functionStat.symbolStat.SumStat( cc2_symbols );
	entry.logicalLinesOfCode    = 2 + LogicalLinesOfCode(functionStat.symbolStat);
	entry.scopeDepth            = functionStat.maxScopeDepth;
}
