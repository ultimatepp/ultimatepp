#ifndef _CppAnalyse_Analyze_h_
#define _CppAnalyse_Analyze_h_

#include <CppBase/CppBase.h>

using namespace Upp;

struct CodeMetric
{
public:
	struct FunctionEntry : public Moveable<FunctionEntry>
	{
		String name;
		int pos;
		int cyclomaticComplexity1;
		int cyclomaticComplexity2;
		int logicalLinesOfCode;
		int scopeDepth;
	};

	int orphanLines, blankLines, commentLines;
	int totalLLOC, sumCC1, sumCC2, sumDepth;
	Vector<FunctionEntry> functions;
	String errors;

	explicit CodeMetric(const String &fileContent);
	String ToString() const;

private:
	typedef CodeMetric CLASSNAME;
	void StoreError(int line, const String &msg);
	void StoreMetric(const Parser::FunctionStat & functionStat);
	int LogicalLinesOfCode(const LexSymbolStat &symbolStat);
};

#endif
