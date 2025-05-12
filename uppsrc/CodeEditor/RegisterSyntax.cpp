#include "CodeEditor.h"

namespace Upp {

void CreateCSyntax(One<EditorSyntax>& e, int kind)
{
	CSyntax& s = e.Create<CSyntax>();
	s.SetHighlight(kind);
}

void RegisterCSyntax(const char *id, int kind,
                     const char *exts, const char *description) {
	EditorSyntax::Register(id, callback1(CreateCSyntax, kind), exts, description);
}

void CreateDiffSyntax(One<EditorSyntax>& e)
{
	e.Create<DiffSyntax>();
}

void CreatePythonSyntax(One<EditorSyntax>& e)
{
	e.Create<PythonSyntax>();
}

void CreateTagSyntax(One<EditorSyntax>& e, bool html, bool witz)
{
	e.Create<TagSyntax>().Html(html).Witz(witz);
}

void CreateLogSyntax(One<EditorSyntax>& e)
{
	e.Create<LogSyntax>();
}

void RegisterSyntaxModules()
{
	RegisterCSyntax("cpp", CSyntax::HIGHLIGHT_CPP,
	                "*.c *.cpp *.cc *.cxx *.h *.hpp *.hh *.hxx *.m *.mm *.icpp *.conf",
	                "C/C++");
	RegisterCSyntax("cuda", CSyntax::HIGHLIGHT_CUDA, "*.cu *.ci", "Cuda");
	RegisterCSyntax("java", CSyntax::HIGHLIGHT_JAVA, "*.java", "Java");
	RegisterCSyntax("js", CSyntax::HIGHLIGHT_JAVASCRIPT, "*.js *.jsx *.ts *.tsx", "JavaScript");
	RegisterCSyntax("cs", CSyntax::HIGHLIGHT_CS, "*.cs", "C#");
	RegisterCSyntax("json", CSyntax::HIGHLIGHT_JSON, "*.json", "JSON");
	RegisterCSyntax("css", CSyntax::HIGHLIGHT_CSS, "*.css", "Cascading Style Sheet");
	RegisterCSyntax("sql", CSyntax::HIGHLIGHT_SQL, "*.sql *.ddl", "SQL script");
	RegisterCSyntax("lay", CSyntax::HIGHLIGHT_LAY, "*.lay", "U++ layout");
	RegisterCSyntax("sch", CSyntax::HIGHLIGHT_SCH, "*.sch", "U++ SQL schema");
	RegisterCSyntax("t", CSyntax::HIGHLIGHT_T, "*.t *.jt", "U++ translation");
	RegisterCSyntax("usc", CSyntax::HIGHLIGHT_USC, "*.usc", "U++ widget definitions");
	RegisterCSyntax("calc", CSyntax::HIGHLIGHT_CALC, "", "");
	RegisterCSyntax("php", CSyntax::HIGHLIGHT_PHP, "*.php", "PHP");
	RegisterCSyntax("dbg", CSyntax::HIGHLIGHT_DBG, "*.dbg", "Debugger pretty printing scripts");
	RegisterCSyntax("glsl", CSyntax::HIGHLIGHT_GLSL,
	                "*.vert *.tesc *.tese *.geom *.frag *.comp",
	                "GLSL");
	
	EditorSyntax::Register("diff", callback(CreateDiffSyntax), "*.diff *.patch", "Diff");
	
	EditorSyntax::Register("python", callback(CreatePythonSyntax), "*.py, *.pyc, *.pyd, *.pyo, *.pyw, *.pyz", "Python");
	
	EditorSyntax::Register("xml", callback2(CreateTagSyntax, false, false), "*.xml *.xsd", "XML");
	EditorSyntax::Register("html", callback2(CreateTagSyntax, true, false), "*.html *.htm", "HTML");
	EditorSyntax::Register("witz", callback2(CreateTagSyntax, true, true), "*.witz", "Skylark templates");
	
	EditorSyntax::Register("log", callback(CreateLogSyntax), "*.log *.info", "Log");
}

}
