#include "Pcre.h"

NAMESPACE_UPP


RegExp::RegExp(int options)
{
	Clear();
	SetOptions(options);
}

RegExp::RegExp(const char * p, int options)
{
	Clear();
	SetOptions(options);
	SetPattern(p);
}

RegExp::RegExp(const String &p, int options)
{
	Clear();
	SetOptions(options);
	SetPattern(p);
}

RegExp::~RegExp()
{
	Clear(true);
}

void RegExp::Clear(bool freemem)
{
	if(freemem && study)
		pcre_free(study);
	if(freemem && cpattern)
		pcre_free(cpattern);

	first = false;
	cpattern = NULL;
	study = NULL;
	rc = 0;
	compile_options = 0;
	execute_options = 0;
	error_code = 0;
}

void RegExp::SetOptions(int options)
{
	compile_options = options & COMPILE_OPTIONS;
	execute_options = options & EXECUTE_OPTIONS;
}

void RegExp::SetPattern(const char * p)
{
	pattern = p;
}

void RegExp::SetPattern(const String &p)
{
	pattern = p;
}

bool RegExp::Compile(bool recompile)
{
	if(cpattern)
	{
		if(recompile)
			pcre_free(cpattern);
		else
			return true;
	}
	cpattern = pcre_compile2(pattern, compile_options, &error_code, &error_string, &error_offset, NULL);
	return error_code == 0;
}

bool RegExp::Study(bool restudy)
{
	if(!cpattern)
		Compile();
	if(study){
		if(restudy)
			pcre_free(study);
		else
			return true;
	}
	study = pcre_study(cpattern, 0, &error_string);
	if(error_string != NULL)
		error_code = -1; // unfortunatelly, pcre_study doesn't return error codes...
	return error_code == 0;
}

int RegExp::Execute(const String &t, int offset)
{
	rc = pcre_exec(cpattern, study, t, t.GetLength(), offset, execute_options, pos, 30);
	if(rc <= 0)
		first = false;
	return rc;
}

bool RegExp::Match(const String &t, bool copy)
{
	if(copy)
		text = t;
	if(!Compile())
		return false;
	return Execute(t) > 0;
}

bool RegExp::FastMatch(const String &t)
{
	return Match(t, false);
}

bool RegExp::GlobalMatch(const String &t)
{
	if(!first)
	{
		first = true;
		return Match(t);
	}
	int offset = pos[1];
	if(pos[0] == pos[1])
	{
		if(pos[0] == t.GetLength())
		{
			first = false;
			return false;
		}
		execute_options |= PCRE_NOTEMPTY | PCRE_ANCHORED;
	}
	return Execute(t, offset) > 0;
}

String RegExp::operator[](const int i)
{
	return GetString(i);
}

int RegExp::GetCount()
{
	return rc - 1;
}

Vector<String> RegExp::GetStrings()
{
	Vector<String> subs;
	for(int i = 0; i < GetCount(); i++)
		subs.Add(GetString(i));
	return subs;
}

String RegExp::GetString(int i)
{
	i = 2 * (i + 1);
	return text.Mid(pos[i], pos[i + 1] - pos[i]);
}

void RegExp::GetMatchPos(int i, int& iPosStart, int& iPosAfterEnd)
{
	i = 2 * (i + 1);
	iPosStart = pos[i];
	iPosAfterEnd = pos[i + 1];
}

int RegExp::GetOffset() const
{
	return rc > 0 ? pos[0] : -1;
}

int RegExp::GetLength() const
{
	return rc > 0 ? pos[1] - pos[0] : -1;
}

int RegExp::GetSubOffset(int i) const
{
	ASSERT(i < rc - 1);
	return pos[2 * i + 2];
}

int RegExp::GetSubLength(int i) const
{
	return pos[2 * i + 3] - pos[2 * i + 2];
}

bool RegExp::ReplacePos(String& t, int p, int q, const String& r)
{
	if(p>=0 && q<=t.GetLength() && q>p){		
		
		t.Remove(p, q-p);
		if(!r.IsEmpty()) t.Insert(p, r);	
		
		return true;
	}
	
	return false;					
}

Vector<String> RegExp::Make_rv(const String& r)
{	
	RegExp reg("\\((.*?)\\)");
	
	Vector<String> rv;
	
	while(reg.GlobalMatch(r)){
		rv.Add(reg.GetString(0));	
	}
	
	return rv;
}

Vector<String> RegExp::ResolveBackRef(const Vector<String>& rv)
{	
	Vector<String> new_rv;
	
	int rv_count = rv.GetCount();
	int match_count = GetCount();
	
	RegExp reg("\\\\(\\d+)");
	
	String log;
		
	if(match_count>0 && rv_count>0) {
		
		// deep copy rv vector.
		new_rv<<=rv;
		
		// loop throug number of replace string
		// run reg regexp on each to find if they
		// have any "match referance" ie /1 /2 /3 etc
		for(int i=0; i<rv_count; i++){
						
			while(reg.GlobalMatch(new_rv[i])){				
				
				// for each matched "match ref"
				for(int j=0, jMx=reg.GetCount(); j< jMx; j++){
					
					String p;
					
					// convert the ref to number
					int m=StrIntValue(reg.GetString(j));
					
					// make "find" string litaral					
					p<<"\\"<<m;	

					// reduce 1 from back ref to be used as index.
					m--;		
					
					// doing actual replacement.
					if(m<match_count) new_rv[i].Replace(p, GetString(m));																
				}									
			}						
		}
	}
	
	return new_rv;		
}



int RegExp::Replace0(String& t, const Vector<String>& rv, const int& rv_count, int& offset)
{
	
	int x=0, y=0;
	int count=GetCount();

		
	for(int i=0; i<count; i++){
				
		GetMatchPos(i, x, y);
		
		if(i<rv_count) {
			
			ReplacePos(t, x+offset, y+offset, rv[i]);
							
			offset = offset + rv[i].GetLength() - (y-x);
			
		}
		else{
	
			ReplacePos(t, x+offset, y+offset, "");	
			
			offset = offset - (y-x);				 
		}										
	}
		
	return count;		
}

int RegExp::Replace(String& t, const Vector<String>& rv, bool backref)
{
	String t_copy(t);
	int count=0;
	int pos_offset=0;
	int rv_count=rv.GetCount();	
	Vector<String> new_rv; 
	
	new_rv.Shrink();
	first=false;
	
	Match(t_copy);
	
	if(backref){
		
		 new_rv.Clear();
		 new_rv = ResolveBackRef(rv);	
		 count  = Replace0(t, new_rv, rv_count, pos_offset);				
		 
	}
	else count = Replace0(t, rv, rv_count, pos_offset);	

	return count;
}

int RegExp::Replace(String& t, const String& r, bool backref)
{
	Vector<String> rv(Make_rv(r)); 
	return Replace(t, rv, backref);
}


int RegExp::Replace(String& t, Callback1<Vector<String>&> cbr)
{
	String t_copy(t);
	int count=0;
	int pos_offset=0;
	int rv_count=0;
	Vector<String> rv; 

	first=false;
	
	Match(t_copy);
	
	rv=GetStrings();	
	
	cbr(rv);	
	
	rv_count=rv.GetCount();
	
	Replace0(t, rv, rv_count, pos_offset);	

	return count;	
	
}

int RegExp::ReplaceGlobal(String& t, const Vector<String>& rv, bool backref)
{
	String t_copy(t);
	int count=0;
	int pos_offset=0;
	int rv_count=rv.GetCount();	
	Vector<String> new_rv; 
	
	new_rv.Shrink();
	first=false;
	
	while(GlobalMatch(t_copy)){
		
			if(backref){
				
				 new_rv.Clear();
				 new_rv = ResolveBackRef(rv);	
				 count += Replace0(t, new_rv, rv_count, pos_offset);				
				 
			}
			else count += Replace0(t, rv, rv_count, pos_offset);	
			
	}
	
	return count;
	
}

int RegExp::ReplaceGlobal(String& t, const String& r, bool backref)
{
	Vector<String> rv(Make_rv(r)); 
	return ReplaceGlobal(t, rv, backref);
}

int RegExp::ReplaceGlobal(String& t, Callback1<Vector<String>&> cbr)
{
	String t_copy(t);
	int count=0;
	int pos_offset=0;
	int rv_count=0;
	Vector<String> rv; 
	
	first = false;
	while(GlobalMatch(t_copy)) {
		rv.Clear();
		rv=GetStrings();
		cbr(rv);
		rv_count=rv.GetCount();
		count += Replace0(t, rv, rv_count, pos_offset);	
	}
	
	return count;
	
}

END_UPP_NAMESPACE
