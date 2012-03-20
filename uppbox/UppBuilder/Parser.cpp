#include <unistd.h>
#include <ctype.h>
#include <new>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

template<class T>
inline void NEVER(T s){
	if(s){
		fprintf(stderr,"ERROR: Something that should never happened has happened...\n");
		exit(1);
	}
};

template<class T>
class Vector{
	int size,alloc;
	T* p;
public:
	Vector():size(0), alloc(0), p(0)        {Resize(2);}
	~Vector()                               {Resize(0);}
	inline int GetCount()const              {return size;}
	inline T& operator[](int n)             {return p[n];}
	inline const T& operator[](int n) const {return p[n];}
	T& Add(){
		if(size>=alloc) 
			Resize(2*alloc);
		return *(new(p+size++) T);
	}
	T& Add(const T& x) { return Add()=x; }
	T& AddUnique(const T& v){
		int pos=Find(v);
		if(pos>=0) return p[pos];
		return Add(v);
	}
	int Find(const T& v) const{
		for(int i=0;i<size;i++){
			if(v==p[i]) return i;
		}
		return -1;
	}
	void Resize(int len){
		if(size>len){
			for(int i=len;i<size;i++)
				(p+i)->~T();
			size=len;
		}
		p = (T*)realloc(p,len*sizeof(T));
		alloc=len;
	}
private:
	Vector<T>& operator=(const Vector<T>& v){}
};

class String {
	char* p;
public:
	String() :                p(strdup("")) {}
	String(const String& s) : p(strdup(s.p)){}
	String(const char* s) :   p(strdup(s))  {}
	~String()                 {free(p);}
	
	char& operator[](const int n) const           {return p[n];}
	bool IsEmpty() const                          {return *p == 0 || strlen(p) == 0;}
	const char* operator~() const                 {return p;}
	operator const char*() const                  {return p;}
	String& operator=(const String& s)            {return operator=(s.p);}
	bool operator==(const char* s) const          {return !strcmp(p, s);}
	bool operator==(const String& s) const        {return !strcmp(p, s.p);}
	void Clear()                                  {free(p); p = strdup("");}
	int GetCount() const                          {return strlen(p);}
	String& operator+=(const String& s){
		const int lenp = strlen(p);
		const int lens = strlen(s.p) + 1;
		p = static_cast<char*>(realloc(p, lenp + lens));
		NEVER(!p);
		memmove(p+lenp, s.p, lens);
		return *this;
	}
	String& operator+=(char s){
		const int lenp = strlen(p);
		p = static_cast<char*>(realloc(p, lenp + 2));
		NEVER(!p);
		p[lenp] = s;
		p[lenp+1] = 0;
		return *this;
	}
	String& operator=(const char* s){
		if (p != s) {
			char *copy = strdup(s);
			free(p);
			p = copy;
		}
		return *this;
	}
	int Compare(const char* s, int len=-1) const {
		return len?strncmp(p,s,len):strcmp(p,s);
	}
	int FindLast(const char c) const {
		for(int i=strlen(p)-1;i>=0;i--)
			if(p[i]==c) return i;
		return -1;
	}
	int Find(const char* s) const {
		int lens=strlen(s);
		int lenp=strlen(p);
		for(int i=0;i<lenp-lens;i++)
			if(strncmp(p+i,s,lens)) return i;
		return -1;
	}
	String Mid(const int start, const int length=-1) const{
		int len = strlen(p)-start;
		if(len<0){
			fprintf(stderr, "Error: Invalid parameters to Mid!");
			exit(1);
		};
		if (start+len>length && length>0)
			len = length;

		String s;
		free(s.p);
		s.p = static_cast<char*>(malloc(len+1));
		NEVER(!s.p);
		memcpy(s.p, p + start, len);
		s.p[len] = '\0';
		return s;
	}
	
	friend String operator+(const String& lhs, const String& rhs);
	friend int operator<(const String& lhs, const String& rhs);
};
inline String operator+(const String& lhs, const String& rhs){
	return String(lhs) += rhs;
}
inline int operator<(const String& lhs, const String& rhs){
	return lhs.Compare(~rhs);
}

String ReadValue(String& s,char delim1 = ',',char delim2 = ','){
	if(s=="") return "";
	String r;
	int skip=0;
	while((s[skip]==delim1 || s[skip] == delim2 || s[skip]<=32) && s[skip]!=0)
		skip++;
	s = s.Mid(skip);
	bool q=false;
	if(s[0]=='\"'){
		s = s.Mid(1);
		q = true;
	}
	const char* c = s;
	for(; *c; c++){
		if(*c==delim1 || *c==delim2 || ((q && *c=='\"') && c[-1] != '\\')) break;
		if(*c<=(q?0:32)) continue;
		r += *c;
	}
	if(q) c++;
	s = c;
	return r;
};

bool Consume(String& s,const char* section){
	int len=strlen(section);
	if(s.Compare(section,len)==0){
		s = s.Mid(len);
		return true;
	}
	return false;
};

enum Opt{NONE, SPEED, SIZE};
enum OptState { UNKNOWN, NO, YES };

struct File{
	Opt opt;
	String name, lang;
	File():opt(NONE){};
};

bool IsSource(File& f){
	int pos=f.name.FindLast('.');
	if(!pos)
		return false;
	String ext = f.name.Mid(pos+1);
	for(int i=0;i<ext.GetCount();i++)
		ext[i]=tolower(ext[i]);
	if (ext.Compare("cpp") == 0
	    || ext.Compare("icpp") == 0
	    || ext.Compare("cc") == 0
	    || ext.Compare("cxx") == 0){
		f.lang="cxx";
		return true;
	} else if (ext.Compare("c") == 0){
		f.lang="cc";
		return true;
	} else if (ext.Compare("brc") == 0){
		f.lang="brc";
		return true;
	}
	return false;
};

void Slashes(String& s){
	for(int i=0; i<s.GetCount(); i++){
		if(s[i]=='\\') s[i]='/';
	}
};

struct OptItem{
	String when;
	String item;
	mutable OptState state;
	OptItem():state(UNKNOWN){};
};

bool LoadOpts(String& s, const char *key, Vector<OptItem>& v) {
	if(Consume(s,key)) {
		if(s[0]==' ')
			s = s.Mid(1);
		String when;
		if(s[0]=='('){
			when=s;
			int len = 0;
			int lvl = 0;
			for(const char* c=when; *c; c++) {
				len++;
				if(*c == '(') lvl++;
				else if(*c == ')'){
					lvl--;
					if(lvl == 0) break;
				}
			}
			when = s.Mid(0,len);
			s = s.Mid(len);
		}
		do {
			OptItem& m=v.Add();
			m.when = when;
			m.item = ReadValue(s,',');
			Slashes(m.item);
		}while(s[0] == ',');
		return true;
	}
	return false;
};

String BaseName(const String& s){
	int pos=s.FindLast('/');
	if(!pos)
		return s;
	return s.Mid(pos+1);
};

String DirName(const String& s){
	int pos=s.FindLast('/');
	if(!pos)
		return "";
	return s.Mid(0,pos);
};

inline String LoadFile(const String& fn){
	String r;
	char b[1000];
	FILE* f=fopen(~fn,"r");
	if(!f) return "";
	int n=0;
	while(!feof(f)){
		n+=fread(b,1,1000,f);
		r+=b;
		//this is a hack, correct implementation would not need it
		r=r.Mid(0,n);
	}
	return r;
}

inline String GetEnv(const char* n,const char* s=""){
	String p("UPP_");
	p+=n;
	const char* r=getenv(p);
	return r?r:s;
};

struct Upp{
	String name,description,dir;
	Opt opt;
	Vector<OptItem> option, link, library, addflags, target, uses, include, custom;
	Vector<String> accepts, cfgk, cfgv, flags;
	Vector<File> files;
	
	Upp() : opt(NONE){};
	Upp(const String& package) : name(package), opt(NONE){};
	Upp(const String& package,const Vector<String>& nests)
		: opt(NONE) {Load(package,nests);}
	bool operator==(const Upp& u)const {return name==u.name;}
	Upp& operator=(const Upp& u){NEVER(true); return *this;}
	String GetClause(FILE* f){
		String r;
		int l=0;
		bool q=false;
		while(1){
			int c = fgetc(f);
			if(c==EOF || (!q && c==';')) break;
			if(c<=32){
				if(!r.IsEmpty() && r[r.GetCount()-1]!=' ')
					r += ' ';
				continue;
			} else
			if (c == '\"' && l != '\\') {
				q=!q;
			}
			l = c;
			r += c;
		}
		return r;
	};
	Upp& Load(const String& package,const Vector<String>& nests){
		name=package;
		char temp[FILENAME_MAX];
		String cwd = getcwd(temp, FILENAME_MAX);
		cwd += "/";
		String fn;
		FILE* f(NULL);
		for(int i=0; i<nests.GetCount(); i++){
			dir = (nests[i][0]=='/'?"":cwd) + nests[i] + "/" + package;
			fn = dir + "/" + BaseName(package) + ".upp";
			f = fopen(fn,"r");
			if(f) break;
		}
		if(!f){
			fprintf(stderr, "Error: Package %s not found!\n", ~package);
			exit(1);
		}
		String s;
		while(1){
			s = GetClause(f);
			if((LoadOpts(s, "options", option) ||
			    LoadOpts(s, "link", link) ||
			    LoadOpts(s, "library", library) ||
			    LoadOpts(s, "flags", addflags) ||
			    LoadOpts(s, "target", target) ||
			    LoadOpts(s, "uses", uses) ||
			    LoadOpts(s, "include", include))){
			} else
			if(Consume(s,"description")){
				int pos=s.Find("\\377");
				if(pos)
					s = s.Mid(0,pos) + "\"";
			} else
			if(Consume(s,"acceptflags")) {
				do{
					accepts.Add(ReadValue(s));
				}while(s[0] == ',');
			} else
			if(Consume(s,"noblitz")){
				// ignored for now
			} else
			if(Consume(s,"mainconfig")) {
				do {
					cfgk.Add(ReadValue(s));
					while(Consume(s," ") || Consume(s,"\""));
					Consume(s,"=");
					cfgv.Add(ReadValue(s));
					while(Consume(s," ") || Consume(s,"\""));
				}while(s[0] == ',');
			} else
			if(Consume(s,"optimize_speed"))
				opt = SPEED;
			else
			if(Consume(s,"optimize_size"))
				opt = SIZE;
			else
			if(Consume(s,"file")){
				do{
					File f;
					f.name = ReadValue(s,' ');
					bool separator=false;
					while(*s && s[0]!=','){
						String str = ReadValue(s,' ');
						if(Consume(str,"optimize_speed"))
							f.opt = SPEED;
						else if(Consume(str,"optimize_size"))
							f.opt = SIZE;
						else if(Consume(str,"separator"))
							separator=true;
					}
					if(!separator && IsSource(f)){
						Slashes(f.name);
						files.Add(f);
					}
				}while(s[0] == ',');
			} else
			if(Consume(s,"custom")){
				if(s[0]==' ')
					s = s.Mid(1);
				OptItem& m=custom.Add();
				int len = 0;
				if(s[0]=='('){
					int lvl = 0;
					for(const char* c=s; *c; c++) {
						len++;
						if(*c == '(') lvl++;
						else if(*c == ')'){
							lvl--;
							if(lvl == 0) break;
						}
					}
					m.when = s.Mid(0,len);
				}
				m.item = s.Mid(len);
			} else
			if(feof(f))
				break;
		}
		fclose(f);
		return *this;
	};
};

inline void Split(String s,Vector<String>& v){
	while(s.GetCount()){
		String val = ReadValue(s,' ');
		if(val.GetCount())
			v.AddUnique(val);
	}
}

String FlagDir(const Vector<String>& v){
	Vector<String> r;
	//simple insertion sort
	r.Resize(v.GetCount());
	for(int i=0;i<v.GetCount();i++)
		r.Add(v[i]);
	for (int j=1; j<v.GetCount(); ++j) {
		const char* c = ~v[j];
		int i=j-1;
		while(i>=0 && r[i].Compare(c)>0) {
			r[i+1]=r[i];
			i--;
		}
		r[i+1]=c;
	}
	String s=r[0];
	for(int i=1;i<r.GetCount();i++)
		s+="."+r[i];
	return s;
}
class Parser{
	Vector<Upp> pkgs;
	String main;
public:
	Vector<String> nests, flags, dflags, libs, linkopts;
	static inline bool IsId(int c) {
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || (c >= '0' && c <= '9');
	}
	bool sMatchFlag(String& s,int pkg){
		if(Consume(s,"!"))
			return !sMatchFlag(s,pkg);
		if(Consume(s,"(")) {
			bool b = sMatchOr(s,pkg);
			Consume(s,")");
			return b;
		}
		if(s.IsEmpty())
			return true;
		int len=0;
		while(IsId(s[len]))
			len++;
		String id = s.Mid(0,len);
		s = s.Mid(len);
		
		if(flags.Find(id)>=0) return true;
		for(int j=0;j<dflags.GetCount();j++)
			if((pkg==0||pkgs[pkg].accepts.Find(dflags[j])>=0) && dflags.Find(id)>=0)
				return true;
		return (pkg==0&&id=="MAIN");
	}
	
	bool sMatchAnd(String& s,int pkg){
		bool b = sMatchFlag(s,pkg);
		while( IsId(s[0]) || s[0]=='!' || s[0]=='(' || Consume(s,"&&") || Consume(s,"&"))
			b = sMatchFlag(s,pkg) && b;
		return b;
	}
	
	bool sMatchOr(String& s,int pkg){
		bool b = sMatchAnd(s,pkg);
		while(Consume(s,"||") || Consume(s,"|"))
			b = sMatchFlag(s,pkg) || b;
		return b;
	}
	
	bool EvalOpt(const OptItem& opt,int pkg){
		if(opt.state!=UNKNOWN)
			return opt.state==YES;
		String s;
		for(const char* c = opt.when; *c; c++)
			if(*c > ' ') s += *c;
		if(s=="()"){
			opt.state=YES;
			return true;
		}
		bool b = sMatchOr(s,pkg);
		if(!s.IsEmpty()){
			fprintf(stderr, "Error: Invalid WHEN string: %s\n",~opt.when);
			exit(1);
		}
		opt.state=b?YES:NO;
		return b;
	}
	void GetDeps(const String& pkg){
		if(pkgs.Find(Upp(pkg))>=0)
			return;
		Upp& added = pkgs.Add().Load(pkg, nests);
		Vector<String> u;
		for(int i=0;i<added.uses.GetCount();i++)
			if(EvalOpt(added.uses[i],pkgs.GetCount()-1))
				u.Add(added.uses[i].item);
		for(int i=0;i<u.GetCount();i++)
			GetDeps(u[i]);
	}
	String EvalVector(const Vector<OptItem>& v,int pkg,const String& prefix){
		String r;
		for(int i=0;i<v.GetCount();i++){
			if(EvalOpt(v[i],pkg))
				r+=" "+prefix+v[i].item;
		}
		return r;
	}
	void EvalFlags(){
		String umc = GetEnv("USEMAINCFG","1");
		int n;
		sscanf(~umc,"%d",&n);
		if(pkgs[0].cfgk.GetCount()==0)
			fprintf(stderr,"Warning: %s doesn't seem to be a main package.\n", ~pkgs[0].name);
		String p;
		bool interactive = GetEnv("INTERACTIVE","0")=="1";
		if (!interactive) {
			n=n?n-1:-1;
		} else if(interactive){
			if (pkgs[0].cfgk.GetCount()){
				printf("Multiple configurations available, select one\n");
				int i=0;
				for(;i<pkgs[0].cfgk.GetCount();i++)
					printf("  %i) '%s': '%s'\n",i+1,~pkgs[0].cfgk[i],~pkgs[0].cfgv[i]);
					printf("  *) Custom flags (type in as space separated list)\n");
				printf("Your selection [%d]:\n",n);
			} else {
				printf("Add custom flags (as space separated list) [\"\"]:\n");
			}
			fflush(0);
			char buf[256];
			fgets(buf, 256, stdin);
			if (strlen(buf) == 0 || sscanf(buf, "%d", &n)) {
				n--;
			} else {
				p = buf;
				n = -1;
			} 
		}
		if(n >= 0 && n < pkgs[0].cfgk.GetCount())
			p = pkgs[0].cfgv[n];
		if(p)
			Split(p,flags);
		p="Using flags";
		for(int i=0; i<flags.GetCount(); i++)
			p+=" "+flags[i];
		printf("%s\n",~p);
		for(int i=0; i<flags.GetCount(); i++)
			if(flags[i][0]=='.'){
				dflags.Add(flags[i].Mid(1));
			}
		pkgs[0].flags.Add("MAIN");
		for(int i=0;i<pkgs.GetCount();i++){
			for(int j=0;j<dflags.GetCount();j++)
				if(i==0 || pkgs[i].accepts.Find(dflags[j])>=0)
					pkgs[i].flags.Add(dflags[j]);
			for(int j=0;j<pkgs[i].addflags.GetCount();j++)
				EvalOpt(pkgs[i].addflags[j],i);
			Vector<String> remflags;
			for(int j=0;j<pkgs[i].addflags.GetCount();j++){
				if(EvalOpt(pkgs[i].addflags[j],i))
					if(pkgs[i].addflags[j].item[0]=='!')
						Split(pkgs[i].addflags[j].item.Mid(1), remflags);
					else
						Split(pkgs[i].addflags[j].item, pkgs[i].flags);
			}
			for(int j=0;j<flags.GetCount();j++){
				if(flags[j][0]=='.' || remflags.Find(flags[j])>=0) continue;
				pkgs[i].flags.Add(flags[j]);
			}
		}
	}
	void EvalOpts(){
		String optimize=GetEnv("OPT");
		for(int i=0;i<pkgs.GetCount();i++){
			if(pkgs[i].opt==NONE && optimize.GetCount()){
				if (optimize=="SPEED")
					pkgs[i].opt=SPEED;
				else if (optimize=="SIZE")
					pkgs[i].opt=SIZE;
			}
			for(int j=0;j<pkgs[i].library.GetCount();j++){
				if(EvalOpt(pkgs[i].library[j],i))
					for(String s=pkgs[i].library[j].item;s.GetCount();){
						String v=ReadValue(s,' ');
						if(v=="" || v==" ") break;
						libs.AddUnique(v);
					}
			}
			for(int j=0;j<pkgs[i].link.GetCount();j++)
				if(EvalOpt(pkgs[i].link[j],i))
					linkopts.Add(pkgs[i].link[j].item);
		}
	}
	Parser(const char* mainpkg) : main(mainpkg){
		Split(GetEnv("NESTS","uppsrc bazaar reference tutorial examples /home/h/MyApps"), nests);
		Split(GetEnv("FLAGS","GCC GUI"), flags);
		Split(GetEnv("PLATFORM","POSIX LINUX"), flags);
		
		GetDeps(main);
		EvalFlags();
		EvalOpts();
		
		Process();
	}
	void Process();
};

#include PROCESS

int main(int argc, const char *argv[]){
	if(argc != 2){
		fprintf(stderr, "Usage: %s package\n", argv[0]);
		return 1;
	}
	Parser r(argv[1]);
	return 0;
}
