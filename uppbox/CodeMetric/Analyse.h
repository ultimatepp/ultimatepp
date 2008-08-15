#ifndef _CppAnalyse_Analyze_h_
#define _CppAnalyse_Analyze_h_

#include <CppBase/CppBase.h>

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

	struct Metric
	{
		int sum, maximum;
		Metric() : sum(0), maximum(0) {}
		void add(int value);
	};

	int orphanLines, blankLines, commentLines, totalLLOC;
	Metric cc1, cc2, depth, lloc;
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
