#include "Browser.h"

String   SchId(CParser& p, String& rr)
{
	p.Char('(');
	String id = p.ReadId();
	rr << "SqlId " << id << "(\"" << id << "\");";
	return id;
}

String ReadId(CParser& p, String& rr)
{
	String id = SchId(p, rr);
	p.Char(',');
	if(p.IsNumber())
		p.ReadNumber();
	p.Char(',');
	if(p.IsNumber())
		p.ReadNumber();
	p.Char(')');
	return id;
}

Vector<String> PreprocessSchFile(const char *fn)
{
	String s = LoadFile(fn);
	CParser p(s);
	String r = "using namespace Upp;";
	String rr = "using namespace Upp;";
	String S_name;
	int line;
	while(!p.IsEof())
		try {
			line = p.GetLine();
			if(p.Id("TABLE") || p.Id("TABLE_") || p.Id("TYPE") || p.Id("TYPE_")) {
				S_name = "S_" + SchId(p, rr);
				r << "struct " << S_name << " {";
			}
			else
			if(p.Id("TABLE_I") || p.Id("TABLE_I_") || p.Id("TYPE_I") || p.Id("TYPE_I_")) {
				S_name = "S_" + SchId(p, rr);
				p.Char(',');
				String S_base = "S_" + p.ReadId();
				r << "struct " << S_name << " : " << S_base << " {";
			}
			else
			if(p.Id("TABLE_II") || p.Id("TABLE_II_") || p.Id("TYPE_II") || p.Id("TYPE_II_")) {
				S_name = "S_" + SchId(p, rr);
				p.Char(',');
				String S_base = "S_" + p.ReadId();
				p.Char(',');
				String S_base2 = "S_" + p.ReadId();
				r << "struct " << S_name << " : " << S_base << ", " << S_base2 << " {";
			}
			else
			if(p.Id("TABLE_III") || p.Id("TABLE_III_") || p.Id("TYPE_III") || p.Id("TYPE_III_")) {
				S_name = "S_" + SchId(p, rr);
				p.Char(',');
				String S_base = "S_" + p.ReadId();
				p.Char(',');
				String S_base2 = "S_" + p.ReadId();
				p.Char(',');
				String S_base3 = "S_" + p.ReadId();
				r << "struct " << S_name << " : " << S_base << ", " << S_base2 << ", " << S_base3 << " {";
			}
			else
			if(p.Id("END_TABLE") || p.Id("END_TYPE"))
				r << "\t"
					"static const char           TableName[];"
					"static const SqlSet&        ColumnSet();"
					"static SqlSet               ColumnSet(const String& prefix);"
					"static SqlSet               Of(SqlId table);"
					"static const Vector<SqlId>& GetColumnIds();"
					""
					"void                        Clear();"
					""
					"void                        FieldLayoutRaw(FieldOperator& f, const String& prefix = String());"
					"void                        FieldLayout(FieldOperator& f);"
					"operator                    Fields();"
					""
					"bool                        operator==(const " << S_name << "& x) const;"
					"bool                        operator!=(const " << S_name << "& x) const;"
					"String                      ToString() const;"
					""
					"int                         GetCount() const;"
					"SqlId                       GetId(int i) const;"
					"Ref                         GetRef(int i);"
					"Ref                         GetRef(const SqlId& id);"
					"Value                       Get(const SqlId& id) const;"
					"Value                       Get(int i) const;"
					"ValueMap                    Get() const;"
					"void                        Set(int i, const Value& v);"
					"void                        Set(const SqlId& id, const Value& v);"
					"void                        Set(const ValueMap& m);"
					<< S_name << "();"
					<< S_name << "(const ValueMap& m);"
					"};";
			else
			if(p.Id("LONGRAW") || p.Id("LONGRAW_") || p.Id("BLOB") || p.Id("BLOB_") ||
			   p.Id("STRING_") || p.Id("STRING") || p.Id("CLOB") || p.Id("CLOB_"))
				r << "\tString " << ReadId(p, rr) << ";";
			else
			if(p.Id("INT") || p.Id("INT_") || p.Id("SERIAL") || p.Id("ISERIAL"))
				r << "\tint " << ReadId(p, rr) << ";";
			else
			if(p.Id("DOUBLE") || p.Id("DOUBLE_"))
				r << "\tdouble " << ReadId(p, rr) << ";";
			else
			if(p.Id("TIME") || p.Id("TIME_"))
				r << "\tTime " << ReadId(p, rr) << ";";
			else
			if(p.Id("DATE") || p.Id("DATE_"))
				r << "\tDate " << ReadId(p, rr) << ";";
			else
			if(p.Id("BIT") || p.Id("BIT_") || p.Id("BOOL") || p.Id("BOOL_"))
				r << "\tbool " << ReadId(p, rr) << ";";
			else
			if(p.Id("INT_ARRAY") || p.Id("INT_ARRAY_"))
				r << "\tint " << ReadId(p, rr) << "[1];";
			else
			if(p.Id("DOUBLE_ARRAY") || p.Id("DOUBLE_ARRAY_"))
				r << "\tdouble " << ReadId(p, rr) << "[1];";
			else
			if(p.Id("TIME_ARRAY") || p.Id("TIME_ARRAY_"))
				r << "\tTime " << ReadId(p, rr) << "[1];";
			else
			if(p.Id("DATE_ARRAY") || p.Id("DATE_ARRAY_"))
				r << "\tDate " << ReadId(p, rr) << "[1];";
			else
			if(p.Id("BOOL_ARRAY") || p.Id("BOOL_ARRAY_") || p.Id("BIT_ARRAY") || p.Id("BIT_ARRAY_"))
				r << "\tbool " << ReadId(p, rr) << "[1];";
			else
			if(p.Id("SEQUENCE") || p.Id("SEQUENCE_"))
				ReadId(p, rr);
			else
				p.SkipTerm();
			line = p.GetLine() - line;
			for(int i = 0; i < line; ++i) {
				r << '\n';
				rr << '\n';
			}
		}
		catch(CParser::Error)
		{}
	return pick(Vector<String>() << r << rr);
}
