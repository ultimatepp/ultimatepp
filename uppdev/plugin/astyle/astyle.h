/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   astyle.h
 *
 *   This file is a part of "Artistic Style" - an indentation and
 *   reformatting tool for C, C++, C# and Java source files.
 *   http://astyle.sourceforge.net
 *
 *   The "Artistic Style" project, including all files needed to
 *   compile it, is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later
 *   version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this project; if not, write to the
 *   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *   Boston, MA  02110-1301, USA.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
 
 /*
 	2008-01-26 Patches by Massimo Del Fedele :
 		- modified sources to use Ultimate++ containers instead std:: ones
 		- fixed memory leaks based on bug report 1804791 submitted by Eran Ifrah
 		- modified to work with unicode
 */

#ifndef ASTYLE_H
#define ASTYLE_H

#include <Core/Core.h>
#include "ASStringTools.hpp"

// 4996 - secure version deprecation warnings for .NET 2005
// 4267 - 64 bit signed/unsigned loss of data
#ifdef _MSC_VER
#pragma warning(disable: 4996)
#pragma warning(disable: 4267)
#endif

namespace astyle
{

enum FileType      { C_TYPE=0, JAVA_TYPE=1, SHARP_TYPE=2 };

/* The enums below are not recognized by 'vectors' in Microsoft Visual C++
   V5 when they are part of a namespace!!!  Use Visual C++ V6 or higher.
*/
enum BracketMode   { NONE_MODE, ATTACH_MODE, BREAK_MODE, BDAC_MODE };

// added by Massimo Del Fedele - just an helper for dialog setup
enum ParenthesisPad { PAD_NONE, PAD_INSIDE, PAD_OUTSIDE, PAD_BOTH };

enum BracketType   { NULL_TYPE = 0,
                     NAMESPACE_TYPE = 1,		// also a DEFINITION_TYPE
                     CLASS_TYPE = 2,			// also a DEFINITION_TYPE
                     DEFINITION_TYPE = 4,
                     COMMAND_TYPE = 8,
                     ARRAY_TYPE  = 16,          // arrays and enums
                     SINGLE_LINE_TYPE = 32
                   };
class ASSourceIterator
{
	public:
		int eolWindows;
		int eolLinux;
		int eolMacOld;
		char outputEOL[4];    // output end of line char
		ASSourceIterator() { eolWindows = eolLinux = eolMacOld = 0; }
		virtual ~ASSourceIterator() {}
		virtual bool hasMoreLines() const = 0;
		virtual WString nextLine() = 0;
};

class ASResource
{
	public:
		void buildAssignmentOperators(Vector<const WString*> &assignmentOperators);
		void buildCastOperators(Vector<const WString*> &castOperators);
		void buildHeaders(Vector<const WString*> &headers, int fileType, bool beautifier=false);
		void buildNonAssignmentOperators(Vector<const WString*> &nonAssignmentOperators);
		void buildNonParenHeaders(Vector<const WString*> &nonParenHeaders, int fileType, bool beautifier=false);
		void buildOperators(Vector<const WString*> &operators);
		void buildPreBlockStatements(Vector<const WString*> &preBlockStatements);
		void buildPreCommandHeaders(Vector<const WString*> &preCommandHeaders);
		void buildPreDefinitionHeaders(Vector<const WString*> &preDefinitionHeaders);

	public:
		static const WString AS_IF, AS_ELSE;
		static const WString AS_DO, AS_WHILE;
		static const WString AS_FOR;
		static const WString AS_SWITCH, AS_CASE, AS_DEFAULT;
		static const WString AS_TRY, AS_CATCH, AS_THROWS, AS_FINALLY;
		static const WString AS_PUBLIC, AS_PROTECTED, AS_PRIVATE;
		static const WString AS_CLASS, AS_STRUCT, AS_UNION, AS_INTERFACE, AS_NAMESPACE, AS_EXTERN;
		static const WString AS_STATIC;
		static const WString AS_CONST;
		static const WString AS_SYNCHRONIZED;
		static const WString AS_OPERATOR, AS_TEMPLATE;
		static const WString AS_OPEN_BRACKET, AS_CLOSE_BRACKET;
		static const WString AS_OPEN_LINE_COMMENT, AS_OPEN_COMMENT, AS_CLOSE_COMMENT;
		static const WString AS_BAR_DEFINE, AS_BAR_INCLUDE, AS_BAR_IF, AS_BAR_EL, AS_BAR_ENDIF;
		static const WString AS_RETURN;
		static const WString AS_ASSIGN, AS_PLUS_ASSIGN, AS_MINUS_ASSIGN, AS_MULT_ASSIGN;
		static const WString AS_DIV_ASSIGN, AS_MOD_ASSIGN, AS_XOR_ASSIGN, AS_OR_ASSIGN, AS_AND_ASSIGN;
		static const WString AS_GR_GR_ASSIGN, AS_LS_LS_ASSIGN, AS_GR_GR_GR_ASSIGN, AS_LS_LS_LS_ASSIGN;
		static const WString AS_EQUAL, AS_PLUS_PLUS, AS_MINUS_MINUS, AS_NOT_EQUAL, AS_GR_EQUAL, AS_GR_GR_GR, AS_GR_GR;
		static const WString AS_LS_EQUAL, AS_LS_LS_LS, AS_LS_LS, AS_ARROW, AS_AND, AS_OR;
		static const WString AS_COLON_COLON, AS_PAREN_PAREN, AS_BLPAREN_BLPAREN;
		static const WString AS_PLUS, AS_MINUS, AS_MULT, AS_DIV, AS_MOD, AS_GR, AS_LS;
		static const WString AS_NOT, AS_BIT_XOR, AS_BIT_OR, AS_BIT_AND, AS_BIT_NOT;
		static const WString AS_QUESTION, AS_COLON, AS_SEMICOLON, AS_COMMA;
		static const WString AS_ASM;
		static const WString AS_FOREACH, AS_LOCK, AS_UNSAFE, AS_FIXED;
		static const WString AS_GET, AS_SET, AS_ADD, AS_REMOVE;
		static const WString AS_CONST_CAST, AS_DYNAMIC_CAST, AS_REINTERPRET_CAST, AS_STATIC_CAST;
};

class ASBeautifier : protected ASResource
{
	public:
		ASBeautifier();
		virtual ~ASBeautifier();
		virtual void init(ASSourceIterator* iter); // pointer to dynamically created iterator.
		void init();
		virtual bool hasMoreLines() const;
		virtual WString nextLine();
		virtual WString beautify(const WString &line);
		void setTabIndentation(int length = 4, bool forceTabs = false);
		void setSpaceIndentation(int length = 4);
		void setMaxInStatementIndentLength(int max);
		void setMinConditionalIndentLength(int min);
		void setClassIndent(bool state);
		void setSwitchIndent(bool state);
		void setCaseIndent(bool state);
		void setBracketIndent(bool state);
		void setBlockIndent(bool state);
		void setNamespaceIndent(bool state);
		void setLabelIndent(bool state);
		void setCStyle();
		void setJavaStyle();
		void setSharpStyle();
		void setEmptyLineFill(bool state);
		void setPreprocessorIndent(bool state);
		int  getIndentLength(void);
		WString getIndentString(void);
		bool getCaseIndent(void);
		bool getCStyle(void);
		bool getJavaStyle(void);
		bool getSharpStyle(void);
		bool getEmptyLineFill(void);

	protected:
		int getNextProgramCharDistance(const WString &line, int i);
//		bool isLegalNameChar(char ch) const;
		const WString *findHeader(const WString &line, int i,
		                         const Vector<const WString*> &possibleHeaders,
		                         bool checkBoundry = true);
		WString trim(const WString &str);
		int  indexOf(Vector<const WString*> &container, const WString *element);
		int  fileType;
		bool isCStyle;
		bool isJavaStyle;
		bool isSharpStyle;

		// variables set by ASFormatter - must be updated in preprocessor
		int  inLineNumber;              // for debugging
		int  outLineNumber;				// for debugging
		bool lineCommentNoBeautify;
		bool isNonInStatementArray;

	private:
		ASBeautifier(const ASBeautifier &copy);
		void operator=(ASBeautifier&); // not to be implemented

		void initStatic();
		void registerInStatementIndent(const WString &line, int i, int spaceTabCount,
		                               int minIndent, bool updateParenStack);
		WString preLineWS(int spaceTabCount, int tabCount);

		static Vector<const WString*> headers;
		static Vector<const WString*> nonParenHeaders;
		static Vector<const WString*> preBlockStatements;
		static Vector<const WString*> assignmentOperators;
		static Vector<const WString*> nonAssignmentOperators;

		ASSourceIterator *sourceIterator;
		Vector<ASBeautifier*> *waitingBeautifierStack;
		Vector<ASBeautifier*> *activeBeautifierStack;
		Vector<int> *waitingBeautifierStackLengthStack;
		Vector<int> *activeBeautifierStackLengthStack;
		WithDeepCopy<Vector<const WString*> > *headerStack;
		WithDeepCopy<Vector< Vector<const WString*>* > >*tempStacks;
		WithDeepCopy<Vector<int> > *blockParenDepthStack;
		WithDeepCopy<Vector<bool> > *blockStatementStack;
		WithDeepCopy<Vector<bool> >*parenStatementStack;
		WithDeepCopy<Vector<int> > *inStatementIndentStack;
		WithDeepCopy<Vector<int> > *inStatementIndentStackSizeStack;
		WithDeepCopy<Vector<int> > *parenIndentStack;
		WithDeepCopy<Vector<bool> >*bracketBlockStateStack;
		WString indentString;
		const WString *currentHeader;
		const WString *previousLastLineHeader;
		const WString *immediatelyPreviousAssignmentOp;
		const WString *probationHeader;
		bool isInQuote;
		bool isInComment;
		bool isInCase;
		bool isInQuestion;
		bool isInStatement;
		bool isInHeader;
		bool isInOperator;
		bool isInTemplate;
		bool isInDefine;
		bool isInDefineDefinition;
		bool classIndent;
		bool isInClassHeader;
		bool isInClassHeaderTab;
		bool switchIndent;
		bool caseIndent;
		bool namespaceIndent;
		bool bracketIndent;
		bool blockIndent;
		bool labelIndent;
		bool preprocessorIndent;
		bool isInConditional;
		bool isMinimalConditinalIndentSet;
		bool shouldForceTabIndentation;
		bool emptyLineFill;
		bool backslashEndsPrevLine;
		bool blockCommentNoIndent;
		bool blockCommentNoBeautify;
		bool previousLineProbationTab;
		int  minConditionalIndent;
		int  parenDepth;
		int  indentLength;
		int  blockTabCount;
		int  leadingWhiteSpaces;
		int  maxInStatementIndent;
		int  templateDepth;
		int  prevFinalLineSpaceTabCount;
		int  prevFinalLineTabCount;
		int  defineTabCount;
		char quoteChar;
		char prevNonSpaceCh;
		char currentNonSpaceCh;
		char currentNonLegalCh;
		char prevNonLegalCh;
		char peekNextChar(WString &line, int i);

	protected:    // inline functions
		// check if a specific character can be used in a legal variable/method/class name
		inline bool isLegalNameChar(char ch) const {
			return (isalnum(ch) || ch == '.' || ch == '_' || (isJavaStyle && ch == '$') || (isCStyle && ch == '~'));
		}

		// check if a specific character is a whitespace character
		inline bool isWhiteSpace(char ch) const {
			return (ch == ' ' || ch == '\t');
		}
};


class ASEnhancer
{
	public:
		// functions
		ASEnhancer();
		~ASEnhancer();
		void init(int, WString, bool, bool, bool, bool, bool);
		void enhance(WString &line);

	private:
		// set by init function
		int    indentLength;
		bool   useTabs;
		bool   isCStyle;
		bool   isJavaStyle;
		bool   isSharpStyle;
		bool   caseIndent;
		bool   emptyLineFill;

		// parsing variables
		int  lineNumber;
		bool isInQuote;
		bool isInComment;
		char quoteChar;

		// unindent variables
		int  bracketCount;
		int  switchDepth;
		bool lookingForCaseBracket;
		bool unindentNextLine;

		// StringStream for trace
		StringStream *traceOut;

	private:    // private functions
		bool findKeyword(const WString &line, int i, const char *header) const;
		int  indentLine(WString  &line, const int indent) const;
		int  unindentLine(WString  &line, const int unindent) const;

	private:
		// struct used by ParseFormattedLine function
		// contains variables used to unindent the case blocks
		struct switchVariables {
			int  switchBracketCount;
			int  unindentDepth;
			bool unindentCase;

			switchVariables() {                 // constructor
				switchBracketCount = 0;
				unindentDepth = 0;
				unindentCase = false;
			}
		};

	private:    // inline functions
		// check if a specific character can be used in a legal variable/method/class name
		inline bool isLegalNameCharX(char ch) const {
			return (isalnum(ch) || ch == '.' || ch == '_' || (isJavaStyle && ch == '$') || (isCStyle && ch == '~'));
		}

		// check if a specific character is a whitespace character
		inline bool isWhiteSpaceX(char ch) const {
			return (ch == ' ' || ch == '\t');
		}
};


class ASFormatter : public ASBeautifier, private ASEnhancer
{
	public:
		ASFormatter();
		virtual ~ASFormatter();
		virtual void init(ASSourceIterator* iter);
		virtual bool hasMoreLines() const;
		virtual WString nextLine();
		void setBracketFormatMode(BracketMode mode);
		void setBreakClosingHeaderBracketsMode(bool state);
		void setOperatorPaddingMode(bool mode);
		void setParensOutsidePaddingMode(bool mode);
		void setParensInsidePaddingMode(bool mode);
		void setParensUnPaddingMode(bool state);
		void setBreakOneLineBlocksMode(bool state);
		void setSingleStatementsMode(bool state);
		void setTabSpaceConversionMode(bool state);
		void setBreakBlocksMode(bool state);
		void setBreakClosingHeaderBlocksMode(bool state);
		void setBreakElseIfsMode(bool state);
		WString fileName;

	private:
		void ASformatter(ASFormatter &copy);            // not to be imlpemented
		void operator=(ASFormatter&);                  // not to be implemented
		void staticInit();
		void goForward(int i);
		void trimNewLine();
		char peekNextChar() const;
		BracketType getBracketType() const;
		bool getNextChar();
		bool isBeforeComment() const;
		bool isBeforeLineEndComment(int startPos) const;
		bool isPointerOrReference() const;
		bool isUnaryMinus() const;
		bool isInExponent() const;
		bool isOneLineBlockReached() const;
//		bool isNextCharWhiteSpace() const;
		bool lineBeginsWith(char charToCheck) const;
		void appendChar(char ch, bool canBreakLine = true);
		void appendCharInsideComments();
		void appendSequence(const WString &sequence, bool canBreakLine = true);
		void appendSpacePad();
		void appendSpaceAfter();
		void breakLine();
		void padOperators(const WString *newOperator);
		void padParens();
		void formatBrackets(BracketType bracketType);
		void formatArrayBrackets(BracketType bracketType, bool isOpeningArrayBracket);
		void adjustComments();
		const WString *findHeader(const Vector<const WString*> &headers, bool checkBoundry = true);

		static Vector<const WString*> headers;
		static Vector<const WString*> nonParenHeaders;
		static Vector<const WString*> preDefinitionHeaders;
		static Vector<const WString*> preCommandHeaders;
		static Vector<const WString*> operators;
		static Vector<const WString*> assignmentOperators;
		static Vector<const WString*> castOperators;

		ASSourceIterator *sourceIterator;
		Vector<const WString*> *preBracketHeaderStack;
		Vector<BracketType> *bracketTypeStack;
		Vector<int> *parenStack;
		WString readyFormattedLine;
		WString currentLine;
		WString formattedLine;
		const WString *currentHeader;
		const WString *previousOperator;    // used ONLY by pad=oper
		char currentChar;
		char previousChar;
		char previousNonWSChar;
		char previousCommandChar;
		char quoteChar;
		int  charNum;
		int  spacePadNum;
		int  templateDepth;
		int  traceFileNumber;
		int  formattedLineCommentNum;		// comment location on formattedLine
		int  previousReadyFormattedLineLength;
		BracketMode bracketFormatMode;
		BracketType previousBracketType;
		bool isVirgin;
		bool shouldPadOperators;
		bool shouldPadParensOutside;
		bool shouldPadParensInside;
		bool shouldUnPadParens;
		bool shouldConvertTabs;
		bool isInLineComment;
		bool isInComment;
		bool isInPreprocessor;
		bool isInTemplate;   // true both in template definitions (e.g. template<class A>) and template usage (e.g. F<int>).
		bool doesLineStartComment;
		bool isInQuote;
		bool isInBlParen;
		bool isSpecialChar;
		bool isNonParenHeader;
		bool foundQuestionMark;
		bool foundPreDefinitionHeader;
		bool foundNamespaceHeader;
		bool foundClassHeader;
		bool foundPreCommandHeader;
		bool foundCastOperator;
		bool isInLineBreak;
//		bool isInClosingBracketLineBreak;
		bool endOfCodeReached;
		bool lineCommentNoIndent;
		bool isLineReady;
		bool isPreviousBracketBlockRelated;
		bool isInPotentialCalculation;
		bool isCharImmediatelyPostComment;
		bool isPreviousCharPostComment;
		bool isCharImmediatelyPostLineComment;
		bool isCharImmediatelyPostOpenBlock;
		bool isCharImmediatelyPostCloseBlock;
		bool isCharImmediatelyPostTemplate;
		bool shouldBreakOneLineBlocks;
		bool shouldReparseCurrentChar;
		bool shouldBreakOneLineStatements;
		bool shouldBreakLineAfterComments;
		bool shouldBreakClosingHeaderBrackets;
		bool shouldBreakElseIfs;
		bool passedSemicolon;
		bool passedColon;
		bool isImmediatelyPostComment;
		bool isImmediatelyPostLineComment;
		bool isImmediatelyPostEmptyBlock;
		bool isImmediatelyPostPreprocessor;

		bool shouldBreakBlocks;
		bool shouldBreakClosingHeaderBlocks;
		bool isPrependPostBlockEmptyLineRequested;
		bool isAppendPostBlockEmptyLineRequested;

		bool prependEmptyLine;
		bool appendOpeningBracket;
		bool foundClosingHeader;

		bool isInHeader;
		bool isImmediatelyPostHeader;

	private:    // inline functions
		// append the CURRENT character (curentChar)to the current formatted line.
		inline void appendCurrentChar(bool canBreakLine = true) {
			appendChar(currentChar, canBreakLine);
		}

		// check if a specific sequence exists in the current placement of the current line
		inline bool isSequenceReached(const char *sequence) const {
//			return currentLine.compare(charNum, strlen(sequence), sequence) == 0;
			return currentLine.Mid(charNum, strlen(sequence)) == WString(sequence);
		}
};

}   // end of namespace astyle

// NEEDED FOR Upp::Vector<>
// marks enum BracketType as moveable so it can be used by Upp containers
namespace Upp
{
	NTL_MOVEABLE(astyle::BracketType);

} // end namespace Upp

#endif // closes ASTYLE_H

