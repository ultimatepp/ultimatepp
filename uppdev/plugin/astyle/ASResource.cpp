/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   ASResource.cpp
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
#include "astyle.h"


namespace astyle
{
const WString ASResource::AS_IF = WString("if");
const WString ASResource::AS_ELSE = WString("else");
const WString ASResource::AS_FOR = WString("for");
const WString ASResource::AS_DO = WString("do");
const WString ASResource::AS_WHILE = WString("while");
const WString ASResource::AS_SWITCH = WString("switch");
const WString ASResource::AS_CASE = WString("case");
const WString ASResource::AS_DEFAULT = WString("default");
const WString ASResource::AS_CLASS = WString("class");
const WString ASResource::AS_STRUCT = WString("struct");
const WString ASResource::AS_UNION = WString("union");
const WString ASResource::AS_INTERFACE = WString("interface");
const WString ASResource::AS_NAMESPACE = WString("namespace");
const WString ASResource::AS_EXTERN = WString("extern");
const WString ASResource::AS_PUBLIC = WString("public");
const WString ASResource::AS_PROTECTED = WString("protected");
const WString ASResource::AS_PRIVATE = WString("private");
const WString ASResource::AS_STATIC = WString("static");
const WString ASResource::AS_SYNCHRONIZED = WString("synchronized");
const WString ASResource::AS_OPERATOR = WString("operator");
const WString ASResource::AS_TEMPLATE = WString("template");
const WString ASResource::AS_TRY = WString("try");
const WString ASResource::AS_CATCH = WString("catch");
const WString ASResource::AS_FINALLY = WString("finally");
const WString ASResource::AS_THROWS = WString("throws");
const WString ASResource::AS_CONST = WString("const");

const WString ASResource::AS_ASM = WString("asm");

const WString ASResource::AS_BAR_DEFINE = WString("#define");
const WString ASResource::AS_BAR_INCLUDE = WString("#include");
const WString ASResource::AS_BAR_IF = WString("#if");
const WString ASResource::AS_BAR_EL = WString("#el");
const WString ASResource::AS_BAR_ENDIF = WString("#endif");

const WString ASResource::AS_OPEN_BRACKET = WString("{");
const WString ASResource::AS_CLOSE_BRACKET = WString("}");
const WString ASResource::AS_OPEN_LINE_COMMENT = WString("//");
const WString ASResource::AS_OPEN_COMMENT = WString("/*");
const WString ASResource::AS_CLOSE_COMMENT = WString("*/");

const WString ASResource::AS_ASSIGN = WString("=");
const WString ASResource::AS_PLUS_ASSIGN = WString("+=");
const WString ASResource::AS_MINUS_ASSIGN = WString("-=");
const WString ASResource::AS_MULT_ASSIGN = WString("*=");
const WString ASResource::AS_DIV_ASSIGN = WString("/=");
const WString ASResource::AS_MOD_ASSIGN = WString("%=");
const WString ASResource::AS_OR_ASSIGN = WString("|=");
const WString ASResource::AS_AND_ASSIGN = WString("&=");
const WString ASResource::AS_XOR_ASSIGN = WString("^=");
const WString ASResource::AS_GR_GR_ASSIGN = WString(">>=");
const WString ASResource::AS_LS_LS_ASSIGN = WString("<<=");
const WString ASResource::AS_GR_GR_GR_ASSIGN = WString(">>>=");
const WString ASResource::AS_LS_LS_LS_ASSIGN = WString("<<<=");
const WString ASResource::AS_RETURN = WString("return");

const WString ASResource::AS_EQUAL = WString("==");
const WString ASResource::AS_PLUS_PLUS = WString("++");
const WString ASResource::AS_MINUS_MINUS = WString("--");
const WString ASResource::AS_NOT_EQUAL = WString("!=");
const WString ASResource::AS_GR_EQUAL = WString(">=");
const WString ASResource::AS_GR_GR = WString(">>");
const WString ASResource::AS_GR_GR_GR = WString(">>>");
const WString ASResource::AS_LS_EQUAL = WString("<=");
const WString ASResource::AS_LS_LS = WString("<<");
const WString ASResource::AS_LS_LS_LS = WString("<<<");
const WString ASResource::AS_ARROW = WString("->");
const WString ASResource::AS_AND = WString("&&");
const WString ASResource::AS_OR = WString("||");
const WString ASResource::AS_COLON_COLON = WString("::");
const WString ASResource::AS_PAREN_PAREN = WString("()");
const WString ASResource::AS_BLPAREN_BLPAREN = WString("[]");

const WString ASResource::AS_PLUS = WString("+");
const WString ASResource::AS_MINUS = WString("-");
const WString ASResource::AS_MULT = WString("*");
const WString ASResource::AS_DIV = WString("/");
const WString ASResource::AS_MOD = WString("%");
const WString ASResource::AS_GR = WString(">");
const WString ASResource::AS_LS = WString("<");
const WString ASResource::AS_NOT = WString("!");
const WString ASResource::AS_BIT_OR = WString("|");
const WString ASResource::AS_BIT_AND = WString("&");
const WString ASResource::AS_BIT_NOT = WString("~");
const WString ASResource::AS_BIT_XOR = WString("^");
const WString ASResource::AS_QUESTION = WString("?");
const WString ASResource::AS_COLON = WString(":");
const WString ASResource::AS_COMMA = WString(",");
const WString ASResource::AS_SEMICOLON = WString(";");

const WString ASResource::AS_FOREACH = WString("foreach");
const WString ASResource::AS_LOCK = WString("lock");
const WString ASResource::AS_UNSAFE = WString("unsafe");
const WString ASResource::AS_FIXED = WString("fixed");
const WString ASResource::AS_GET = WString("get");
const WString ASResource::AS_SET = WString("set");
const WString ASResource::AS_ADD = WString("add");
const WString ASResource::AS_REMOVE = WString("remove");

const WString ASResource::AS_CONST_CAST = WString("const_cast");
const WString ASResource::AS_DYNAMIC_CAST = WString("dynamic_cast");
const WString ASResource::AS_REINTERPRET_CAST = WString("reinterpret_cast");
const WString ASResource::AS_STATIC_CAST = WString("static_cast");


/**
 * Build the vector of assignment operators.
 * Used by BOTH ASFormatter.cpp and ASBeautifier.cpp
 *
 * @param assignmentOperators   a reference to the vector to be built.
 */
void ASResource::buildAssignmentOperators(Vector<const WString*> &assignmentOperators)
{
	assignmentOperators.push_back(&AS_ASSIGN);
	assignmentOperators.push_back(&AS_PLUS_ASSIGN);
	assignmentOperators.push_back(&AS_MINUS_ASSIGN);
	assignmentOperators.push_back(&AS_MULT_ASSIGN);
	assignmentOperators.push_back(&AS_DIV_ASSIGN);
	assignmentOperators.push_back(&AS_MOD_ASSIGN);
	assignmentOperators.push_back(&AS_OR_ASSIGN);
	assignmentOperators.push_back(&AS_AND_ASSIGN);
	assignmentOperators.push_back(&AS_XOR_ASSIGN);

	// Java
	assignmentOperators.push_back(&AS_GR_GR_GR_ASSIGN);
	assignmentOperators.push_back(&AS_GR_GR_ASSIGN);
	assignmentOperators.push_back(&AS_LS_LS_ASSIGN);

	// Unknown
	assignmentOperators.push_back(&AS_LS_LS_LS_ASSIGN);

	assignmentOperators.push_back(&AS_RETURN);
}

/**
 * Build the vector of C++ cast operators.
 * Used by ONLY ASFormatter.cpp
 *
 * @param castOperators     a reference to the vector to be built.
 */
void ASResource::buildCastOperators(Vector<const WString*> &castOperators)
{
	castOperators.push_back(&AS_CONST_CAST);
	castOperators.push_back(&AS_DYNAMIC_CAST);
	castOperators.push_back(&AS_REINTERPRET_CAST);
	castOperators.push_back(&AS_STATIC_CAST);
}

/**
 * Build the vector of header words.
 * Used by BOTH ASFormatter.cpp and ASBeautifier.cpp
 *
 * @param headers       a reference to the vector to be built.
 */
void ASResource::buildHeaders(Vector<const WString*> &headers, int fileType, bool beautifier)
{
	headers.push_back(&AS_IF);
	headers.push_back(&AS_ELSE);
	headers.push_back(&AS_FOR);
	headers.push_back(&AS_WHILE);
	headers.push_back(&AS_DO);
	headers.push_back(&AS_SWITCH);
	headers.push_back(&AS_TRY);
	headers.push_back(&AS_CATCH);

	if (beautifier)
	{
		headers.push_back(&AS_CASE);
		headers.push_back(&AS_DEFAULT);
		headers.push_back(&AS_CONST);
		headers.push_back(&AS_STATIC);
		headers.push_back(&AS_EXTERN);
		headers.push_back(&AS_TEMPLATE);
	}

	if (fileType == JAVA_TYPE)
	{
		headers.push_back(&AS_FINALLY);
		headers.push_back(&AS_SYNCHRONIZED);
	}

	if (fileType == SHARP_TYPE)
	{
		headers.push_back(&AS_FINALLY);
		headers.push_back(&AS_FOREACH);
		headers.push_back(&AS_LOCK);
		headers.push_back(&AS_UNSAFE);
		headers.push_back(&AS_FIXED);
		headers.push_back(&AS_GET);
		headers.push_back(&AS_SET);
		headers.push_back(&AS_ADD);
		headers.push_back(&AS_REMOVE);
	}
}

/**
 * Build the vector of non-assignment operators.
 * Used by ONLY ASBeautifier.cpp
 *
 * @param nonParenHeaders       a reference to the vector to be built.
 */
void ASResource::buildNonAssignmentOperators(Vector<const WString*> &nonAssignmentOperators)
{
	nonAssignmentOperators.push_back(&AS_EQUAL);
	nonAssignmentOperators.push_back(&AS_PLUS_PLUS);
	nonAssignmentOperators.push_back(&AS_MINUS_MINUS);
	nonAssignmentOperators.push_back(&AS_NOT_EQUAL);
	nonAssignmentOperators.push_back(&AS_GR_EQUAL);
	nonAssignmentOperators.push_back(&AS_GR_GR_GR);
	nonAssignmentOperators.push_back(&AS_GR_GR);
	nonAssignmentOperators.push_back(&AS_LS_EQUAL);
	nonAssignmentOperators.push_back(&AS_LS_LS_LS);
	nonAssignmentOperators.push_back(&AS_LS_LS);
	nonAssignmentOperators.push_back(&AS_ARROW);
	nonAssignmentOperators.push_back(&AS_AND);
	nonAssignmentOperators.push_back(&AS_OR);
}

/**
 * Build the vector of header non-paren headers.
 * Used by BOTH ASFormatter.cpp and ASBeautifier.cpp
 *
 * @param nonParenHeaders       a reference to the vector to be built.
 */
void ASResource::buildNonParenHeaders(Vector<const WString*> &nonParenHeaders, int fileType, bool beautifier)
{
	nonParenHeaders.push_back(&AS_ELSE);
	nonParenHeaders.push_back(&AS_DO);
	nonParenHeaders.push_back(&AS_TRY);

	if (beautifier)
	{
		nonParenHeaders.push_back(&AS_CASE);
		nonParenHeaders.push_back(&AS_DEFAULT);
		nonParenHeaders.push_back(&AS_CONST);
		nonParenHeaders.push_back(&AS_STATIC);
		nonParenHeaders.push_back(&AS_EXTERN);
		nonParenHeaders.push_back(&AS_TEMPLATE);
	}

	if (fileType == JAVA_TYPE)
	{
		nonParenHeaders.push_back(&AS_FINALLY);
	}

	if (fileType == SHARP_TYPE)
	{
		nonParenHeaders.push_back(&AS_FINALLY);
		nonParenHeaders.push_back(&AS_UNSAFE);
		nonParenHeaders.push_back(&AS_GET);
		nonParenHeaders.push_back(&AS_SET);
		nonParenHeaders.push_back(&AS_ADD);
		nonParenHeaders.push_back(&AS_REMOVE);
	}
}

/**
 * Build the vector of operators.
 * Used by ONLY ASFormatter.cpp
 *
 * @param operators             a reference to the vector to be built.
 */
void ASResource::buildOperators(Vector<const WString*> &operators)
{
	operators.push_back(&AS_PLUS_ASSIGN);
	operators.push_back(&AS_MINUS_ASSIGN);
	operators.push_back(&AS_MULT_ASSIGN);
	operators.push_back(&AS_DIV_ASSIGN);
	operators.push_back(&AS_MOD_ASSIGN);
	operators.push_back(&AS_OR_ASSIGN);
	operators.push_back(&AS_AND_ASSIGN);
	operators.push_back(&AS_XOR_ASSIGN);
	operators.push_back(&AS_EQUAL);
	operators.push_back(&AS_PLUS_PLUS);
	operators.push_back(&AS_MINUS_MINUS);
	operators.push_back(&AS_NOT_EQUAL);
	operators.push_back(&AS_GR_EQUAL);
	operators.push_back(&AS_GR_GR_GR_ASSIGN);
	operators.push_back(&AS_GR_GR_ASSIGN);
	operators.push_back(&AS_GR_GR_GR);
	operators.push_back(&AS_GR_GR);
	operators.push_back(&AS_LS_EQUAL);
	operators.push_back(&AS_LS_LS_LS_ASSIGN);
	operators.push_back(&AS_LS_LS_ASSIGN);
	operators.push_back(&AS_LS_LS_LS);
	operators.push_back(&AS_LS_LS);
	operators.push_back(&AS_ARROW);
	operators.push_back(&AS_AND);
	operators.push_back(&AS_OR);
	operators.push_back(&AS_COLON_COLON);
	operators.push_back(&AS_PLUS);
	operators.push_back(&AS_MINUS);
	operators.push_back(&AS_MULT);
	operators.push_back(&AS_DIV);
	operators.push_back(&AS_MOD);
	operators.push_back(&AS_QUESTION);
	operators.push_back(&AS_COLON);
	operators.push_back(&AS_ASSIGN);
	operators.push_back(&AS_LS);
	operators.push_back(&AS_GR);
	operators.push_back(&AS_NOT);
	operators.push_back(&AS_BIT_OR);
	operators.push_back(&AS_BIT_AND);
	operators.push_back(&AS_BIT_NOT);
	operators.push_back(&AS_BIT_XOR);
	operators.push_back(&AS_OPERATOR);
	operators.push_back(&AS_COMMA);
	operators.push_back(&AS_RETURN);
}

/**
 * Build the vector of pre-block statements.
 * Used by ONLY ASBeautifier.cpp
 *
 * @param preBlockStatements        a reference to the vector to be built.
 */
void ASResource::buildPreBlockStatements(Vector<const WString*> &preBlockStatements)
{
	preBlockStatements.push_back(&AS_CLASS);
	preBlockStatements.push_back(&AS_STRUCT);
	preBlockStatements.push_back(&AS_UNION);
	preBlockStatements.push_back(&AS_INTERFACE);
	preBlockStatements.push_back(&AS_NAMESPACE);
	preBlockStatements.push_back(&AS_THROWS);
	preBlockStatements.push_back(&AS_EXTERN);
}

/**
 * Build the vector of pre-command headers.
 * Used by ONLY ASFormatter.cpp
 *
 * @param preCommandHeaders     a reference to the vector to be built.
 */
void ASResource::buildPreCommandHeaders(Vector<const WString*> &preCommandHeaders)
{
	preCommandHeaders.push_back(&AS_EXTERN);
	preCommandHeaders.push_back(&AS_THROWS);
	preCommandHeaders.push_back(&AS_CONST);
}

/**
 * Build the vector of pre-definition headers.
 * Used by ONLY ASFormatter.cpp
 *
 * @param preDefinitionHeaders      a reference to the vector to be built.
 */
void ASResource::buildPreDefinitionHeaders(Vector<const WString*> &preDefinitionHeaders)
{
	preDefinitionHeaders.push_back(&AS_CLASS);
	preDefinitionHeaders.push_back(&AS_INTERFACE);
	preDefinitionHeaders.push_back(&AS_NAMESPACE);
	preDefinitionHeaders.push_back(&AS_STRUCT);
}


}   // end namespace astyle
