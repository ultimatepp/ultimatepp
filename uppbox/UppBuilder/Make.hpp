#include <time.h>

String ParseCustomStep(const String& s){
	String cmd;
	for(const char* c=~s;*c;c++){
		if(c[0]=='\\' && c[1]=='\"') continue;
		if(strncmp(c,"$(PATH)",7)==0){
			cmd+="$$f";
			c+=6;
		} else if(strncmp(c,"$(DIR)",6)==0){
			cmd+="`dirname $$f`";
			c+=5;
		} else if(strncmp(c,"$(FILE)",7)==0){
			cmd+="`basename $$f`";
			c+=6;
		} else if(strncmp(c,"$(TITLE)",8)==0){
			cmd+="`basename $$f | sed 's/[.][^.]*$//'`";
			c+=7;
		} else if(strncmp(c,"$(OUTDIR)",9)==0){
			cmd+="$(BIN)/";
			c+=8;
		} else
			cmd+=*c;
//		$(INCLUDE),$(!) etc. not implemented yet
	}
	return cmd;
}

String CustomRule(String rule, const String& pkg){
	String ext=ReadValue(rule);
	String cmd=ParseCustomStep(ReadValue(rule));
	//ignore last part for simplicity
	if (cmd.IsEmpty() || ext.IsEmpty())
		return "";
	if (ext=="pre-link")
		return "	$E cd $("+pkg+"_src) &&"
	           "echo \"    Applying custom build pre-link step "
	           "(warning: this is an experimental feature)\"; "
	           +cmd+";\n";
	if (ext=="post-link")
		return "	$E cd $("+pkg+"_src) &&"
	           "echo \"    Applying custom build post-link step "
	           "(warning: this is an experimental feature)\"; "
	           +cmd+";\n";
	return "	$E cd $("+pkg+"_src) && for f in *"+ext+"; "
	       "do echo \"    Applying custom build step on $$f "
	       "(warning: this is an experimental feature)\"; "
	       +cmd+"; done;\n";
}

void Parser::Process(){
	String uppout=GetEnv("OUT","/tmp/_out");
	String makefile=uppout+"/Makefile";
	FILE* mf=fopen(makefile,"w");
	if(!mf){
		printf("Failed to open file %s\n", ~makefile);
		exit(1);
	}
	
	fprintf(mf, "# This is an automatically generated makefile for %s package\n"
	            "# You should never need to modify it...\n", ~main);
	
	bool dep = GetEnv("DEPS","0")=="1";
	bool color = GetEnv("COLOR","0")=="1";
	String includes;
	Vector<String> inc;
	Split(GetEnv("INCPATHS"),inc);
	for(int i=0; i<nests.GetCount(); i++)
		includes+=" -I"+nests[i];
	for(int i=0; i<inc.GetCount(); i++)
		includes+=" -I"+inc[i];

	String libpaths;
	Vector<String> lpaths;
	Split(GetEnv("LIBPATHS"),lpaths);
	for(int i=0; i<lpaths.GetCount(); i++)
		libpaths+=" -L"+lpaths[i];

	time_t rawtime;
	time(&rawtime);
	char date[90];
	struct tm* t = localtime(&rawtime);
	sprintf(date," -DbmYEAR=%i -DbmMONTH=%i -DbmDAY=%i -DbmHOUR=%i -DbmMINUTE=%i -DbmSECOND=%i",
	             t->tm_year+1900, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	
	fprintf(mf, "OUT:=%s\n"
	            "BIN:=%s\n"
	            "CFLAGS:=%s\n"
	            "CXXFLAGS:=%s\n"
	            "SPEEDFLAGS:=%s\n"
	            "SIZEFLAGS:=%s\n"
	            "LDFLAGS:=%s\n"
	            "CC:=%s\n"
	            "CXX:=%s\n"
	            "AR:=%s\n"
	            "MKDIRP:=%s\n"
	            "E:=%s\n"
	            "M:=%s\n"
	            "C:=%s\n"
	            "B:=%s\n"
	            "N:=%s\n"
	            "mc=$M \"    Compiling $C$<$N\"\n"
	            "ma=$M \"=> $BCreating archive$N $C$@$N\"\n"
	            "cc=$E $(CC) -c -x c %s $< -o $@ $(CPPFLAGS) %s\n"
	            "cxx=$E $(CXX) -c -x c++ %s $< -o $@ $(CPPFLAGS) %s\n"
	            "includes:=%s\n"
	            "LIBPATHS:=%s\n"
	            "default: build\n"
	            ".PHONY:build clean createdirs custom default\n\n",
	        ~uppout, ~GetEnv("BIN","$(OUT)/bin"),
	        ~GetEnv("CFLAGS","-O2"),
	        ~GetEnv("CXXFLAGS","$(CFLAGS)"),
	        ~GetEnv("SPEEDFLAGS","-O3 -ffunction-sections -fdata-sections"),
	        ~GetEnv("SIZEFLAGS","-Os -finline-limit=20 -ffunction-sections -fdata-sections"),
	        ~GetEnv("LDFLAGS","-Wl,--gc-sections -Wl,-s -Wl,-O,2"),
	        ~GetEnv("CC","$(CC)"), ~GetEnv("CXX","$(CXX)"),
	        ~GetEnv("AR","ar -src"), ~GetEnv("MKDIRP","mkdir -p"),
	        GetEnv("ECHO","0")=="1"?"":"@",
	        GetEnv("SILENT","0")=="1"?"@true":(color?"@echo -e":"@echo"),
	        color?(const char*)("\\e[3"+GetEnv("HIGHLIGHT","4")+"m"):"", 
	        color?"\\e[1m":"", color?"\\e[0m":"",
	        dep?"-MMD":"", date, dep?"-MMD":"",
	        date, ~includes, ~libpaths);
	String icpps, archives, prelink, postlink, custom;
	Vector<String> dirs;
	for(int i=0;i<pkgs.GetCount();i++){
		const String& pkg=pkgs[i].name;
		String base=BaseName(pkg);
		String objlist;
		Vector<String> pflags;

		String opt;
		//we add the internal includes twice, to fix the case with relative paths... isn't there a better solution?
		opt=EvalVector(pkgs[i].include,i,"-I");
		opt+=EvalVector(pkgs[i].include,i,"-I$("+pkg+"_src)/"); 
		opt+=EvalVector(pkgs[i].option,i,"");
		for(int j=0;j<pkgs[i].flags.GetCount();j++)
			opt+=" -Dflag"+pkgs[i].flags[j];
		String fdir=pkg+"/"+FlagDir(pkgs[i].flags);
		fprintf(mf, "%s", ~(pkg+"_src="+pkgs[i].dir+"\n"+
		            pkg+"_out=$(OUT)/"+fdir+"\n"+
		            pkg+"_opt=$(includes) "+opt+"\n"));
		for(int j=0; j<pkgs[i].files.GetCount(); j++){
			const File& f=pkgs[i].files[j];
			dirs.AddUnique(DirName("$("+pkg+"_out)/"+f.name));
			if(f.lang == "brc"){
				fprintf(mf, "$(%s_out)/%s.o: $(%s_src)/%s\n"
				            "	$(mc)\n"
				            "	$E sh $(OUT)/brc.sh $< > $(%s_out)/%s.cpp\n"
				            "	$E $(CXX) -c -x c++ $(%s_out)/%s.cpp -o $@ $(CPPFLAGS) $(CXXFLAGS)\n",
				            ~pkg, ~f.name, ~pkg, ~f.name, ~pkg, ~f.name, ~pkg, ~f.name);
				objlist+=" $("+pkg+"_out)/"+f.name+".o";
				continue;
			}
			String ext,fopt;
			if(f.lang=="c++")
				opt="$(CXXFLAGS) ";
			else if(f.lang=="c")
				opt="$(CFLAGS) ";
			if(pkgs[i].opt==SPEED) ext=".speed.o";
			else if(pkgs[i].opt==SIZE) ext=".size.o";
			else{
				if(f.opt==SPEED) ext=".speed.o";
				if(f.opt==SIZE) ext=".size.o";
				else ext=".o";
			}
			if(ext==".speed.o")
				fopt+="$(SPEEDFLAGS)";
			else if(ext==".size.o")
				fopt+="$(SIZEFLAGS)";
			String fn="$("+pkg+"_out)/" + f.name  + ext;
			if(dep){
				String dep = uppout + "/" + fdir + "/" + f.name + ext;
				dep[dep.GetCount()-1]='d';
				dep=LoadFile(dep);
				if(dep.GetCount())
					fprintf(mf, "%s", ~dep);
				else
					fprintf(mf, "%s: $("+pkg+"_src)/%s\n", ~fn, ~f.name);
			} else {
				fprintf(mf, "%s: $("+pkg+"_src)/%s\n", ~fn, ~f.name);
			}
			fprintf(mf,"	$(mc)\n"
			           "	$(%s) %s $(%s_opt)\n",
			           ~f.lang, ~fopt, ~pkg);
			
			if(f.name.Mid(f.name.GetCount()-4)=="icpp")
				icpps+=" "+fn;
			else
				objlist+=" "+fn;
		}
		fprintf(mf,"%s_custom:\n", ~pkgs[i].name);
		for(int j=0;j<pkgs[i].custom.GetCount();j++){
			if(EvalOpt(pkgs[i].custom[j],i)){
				if(pkgs[i].custom[j].item.Compare("\"pre-link\"",10)==0)
					prelink+=CustomRule(pkgs[i].custom[j].item, pkgs[i].name);
				else if(pkgs[i].custom[j].item.Compare("\"post-link\"",11)==0)
					postlink+=CustomRule(pkgs[i].custom[j].item, pkgs[i].name);
				else{
					String rule = CustomRule(pkgs[i].custom[j].item, pkgs[i].name);
					if(rule=="") continue;
					fprintf(mf,"%s",~rule);
					custom+=" "+pkgs[i].name+"_custom";
				}
			}
		}
		fprintf(mf,"$(%s_out)/%s.a:%s\n"
		           "	$(ma)\n"
		           "	$E $(AR) $@ $^\n\n",
		        ~pkg, ~base, ~objlist);
		archives="$("+pkg+"_out)/"+base+".a "+archives;
	}
	String liblist,optlist,dirlist;
	for(int i=0;i<libs.GetCount();i++)
		liblist+=" -l"+libs[i];
	for(int i=0;i<linkopts.GetCount();i++)
		optlist+=" "+linkopts[i];
	String target = GetEnv("TARGET");
	if(target.IsEmpty()){
		target="$(BIN)/" + BaseName(main);
		for(int i=0;i<pkgs[0].target.GetCount();i++){
			if(EvalOpt(pkgs[0].target[i],0)){
				target = pkgs[0].target[i].item;
				break;
			}
		}
	}
	if(target.Find("/")==-1)
		target="$(BIN)/"+target;
	dirs.AddUnique(DirName(target));
	for(int i=0;i<dirs.GetCount();i++)
		dirlist+=" "+dirs[i];
	fprintf(mf, "build: createdirs %s %s\n", ~custom, ~target);
	fprintf(mf, "%s: %s %s\n"
	            "	$M \"$BLinking$N $C$@$N\"\n"
	            "	$E $(CXX) -o $@ $(LDFLAGS) $(LIBPATHS) %s -Wl,--start-group $^ -Wl,--end-group -Wl,--start-group %s -Wl,--end-group\n"
	            "%s",
	        /*~prelink,*/ ~target, ~icpps, ~archives, ~optlist, ~liblist, ~postlink);
	fprintf(mf, "createdirs:\n"
	            "	$M \"  $BCreating necessary directories ...$N\"\n"
	            "	$E mkdir -p%s\n",
	        ~dirlist);
	String clean;
	for(int i=0;i<pkgs.GetCount();i++)
		clean+=" $("+pkgs[i].name+"_out)";
	fprintf(mf,"clean:\n"
	           "	$E rm -rf %s\n",~clean);
	fclose(mf);
}
