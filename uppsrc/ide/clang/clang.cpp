#include "clang.h"

String FetchString(CXString cs)
{
	String result = clang_getCString(cs);
	clang_disposeString(cs);
	return result;
}

String GetCursorKindName(CXCursorKind cursorKind)
{
	return FetchString(clang_getCursorKindSpelling(cursorKind));
}

String GetCursorSpelling(CXCursor cursor)
{
	return FetchString(clang_getCursorSpelling(cursor));
}

String GetTypeSpelling(CXCursor cursor)
{
	return FetchString(clang_getTypeSpelling(clang_getCursorType(cursor)));
}

String SourceLocation::ToString() const
{
	return String() << filename << " (" << line << ":" << column << ")";
}

SourceLocation::SourceLocation(CXSourceLocation location)
{
	CXFile file;
	unsigned line_;
	unsigned column_;
	unsigned offset_;
	clang_getExpansionLocation(location, &file, &line_, &column_, &offset_);
	line = line_;
	column = column_;
	offset = offset_;
	filename = FetchString(clang_getFileName(file));
}

CXChildVisitResult visitor( CXCursor cursor, CXCursor /* parent */, CXClientData clientData )
{
#if 0
	static Index<unsigned> visited;
	unsigned h = clang_hashCursor(cursor);
	if(visited.Find(h) >= 0)
		return CXChildVisit_Continue;
	visited.Add(h);
#endif
	CXSourceLocation cxlocation = clang_getCursorLocation( cursor );
	SourceLocation location = cxlocation;
//	if( clang_Location_isFromMainFile( location ) == 0 )
//		return CXChildVisit_Continue;
	
	CXCursorKind cursorKind = clang_getCursorKind( cursor );

	unsigned int curLevel  = *( reinterpret_cast<unsigned int*>(clientData));
	unsigned int nextLevel = curLevel + 1;
	
	if(curLevel > 100000 && curLevel - 100000 > 2)
		return CXChildVisit_Continue;

	if (1 || findarg(cursorKind, CXCursorKind::CXCursor_FunctionDecl, CXCursorKind::CXCursor_CXXMethod,
							CXCursorKind::CXCursor_FunctionTemplate, CXCursorKind::CXCursor_Constructor) >= 0
		|| clang_Location_isFromMainFile(cxlocation)) {

		String indent = curLevel > 100000 ? String('=', curLevel - 100000) : String('-', curLevel);
		LOG(indent << " " << GetCursorKindName(cursorKind)
			<< " '" << GetCursorSpelling(cursor) << "' "
			<< " '" << GetTypeSpelling(cursor) << "' " << location);
		String h = FetchString(clang_getCursorUSR(cursor));
		if(h.GetCount())
			LOG(indent << "  USR: " << h);
		CXCursor c2 = clang_getCursorReferenced(cursor);
		if(!clang_Cursor_isNull(c2)) {
			LOG(indent << "Referenced: " << GetCursorSpelling(c2) << ' ' << GetCursorKindName(clang_getCursorKind(c2)));
			String h = FetchString(clang_getCursorUSR(c2));
			if(h.GetCount())
				LOG(indent << "->USR: " << h);
//			int h = nextLevel < 100000 ? nextLevel + 100000 : nextLevel;
//			clang_visitChildren(c2, visitor, &h);
		}
//		clang_visitChildren(cursor, visitor, &nextLevel);
	}

	return CXChildVisit_Continue;
}

CXTranslationUnit Clang(const String& cmdline, Vector<Tuple2<String, String>> file, unsigned options)
{
	static CXIndex index = clang_createIndex(0, 0);
	if (!index) {
		DLOG("createIndex failed");
		return nullptr;
	}

	Vector<String> sa = Split(cmdline, ' ');
	
	Buffer<const char *> argv(sa.GetCount());
	int i = 0;
	for(const String& s : sa)
		argv[i++] = ~s;
	
	CXTranslationUnit tu;

	Buffer<CXUnsavedFile> ufile(file.GetCount());
	CXUnsavedFile *t = ufile;
	for(const auto& f : file) {
		t->Filename = ~f.a;
		t->Contents = ~f.b;
		t->Length = f.b.GetCount();
		t++;
	}
	
	tu = clang_parseTranslationUnit(index, nullptr, argv, sa.GetCount(), ufile, file.GetCount(), options);
	
	return tu;
}

String PrecompileHeader(String& content, const Vector<String>& includes)
{
	String out;
	String hdr;

	StringStream ss(content);
	while(!ss.IsEof()) {
		String l0 = ss.GetLine();
		String l = TrimLeft(ss.GetLine()); // todo COMMENTS!
		if(l.GetCount() == 0)
			continue;
		if(l.StartsWith("#include")) {
			hdr << l << "\n";
			out << "\n";
		}
		else {
			out << l0 << "\n";
			break;
		}
	}
	
	if(hdr.GetCount() == 0)
		return Null;
	
	out << LoadStream(ss);
	content = out;
	
	DDUMP(content);
	
	String pch_header2 = "c:/xxx/$pch.h";
	String pch_file = pch_header2 + ".pch";
	SaveFile(pch_header2, hdr); // CLANG needs a copy of header

	String cmdline;
	for(String s : includes)
		cmdline << " -I" << s;

	cmdline << " -xc++ ";
	cmdline << pch_header2;
/*	
	CXTranslationUnit tu;
	{
		TIMING("Precompile");
		
		tu = Clang(cmdline, Vector<Tuple<String, String>>(), CXTranslationUnit_ForSerialization);
		if(tu) {
			clang_saveTranslationUnit(tu, pch_file, clang_defaultSaveOptions(tu));
			clang_disposeTranslationUnit(tu);
			content = out;
			return pch_file;
		}
	}
*/	
	return Null;
}

void ClangFile(const String& filename, const String& content_, const Vector<String>& includes, int line, int column)
{
	String content = content_;
//	String pch = PrecompileHeader(content, includes);
	
	String cmdline;
	
//	if(pch.GetCount())
//		cmdline << "-include-pch " << pch << ' ';
	
	cmdline << filename << RedefineMacros() << " -DflagDEBUG -DflagDEBUG_FULL -DflagBLITZ -DflagWIN32 -xc++ -std=c++17 ";
;

//	if(pch.GetCount())
//		cmdline << " -I" + GetFileFolder(pch) + " -include " + GetFileName(pch) + " -Winvalid-pch ";
	
	for(String s : includes)
		cmdline << " -I" << s;
	
	DDUMP(cmdline);
	
	CXTranslationUnit tu;
	{
		RTIMING("Translate");
		tu = Clang(cmdline, { { filename, content } }, CXTranslationUnit_PrecompiledPreamble|CXTranslationUnit_CreatePreambleOnFirstParse|CXTranslationUnit_KeepGoing|CXTranslationUnit_RetainExcludedConditionalBlocks);
	}

//	CXCursor rootCursor  = clang_getTranslationUnitCursor(tu);
//	unsigned int treeLevel = 0;
//	clang_visitChildren(rootCursor, visitor, &treeLevel);

	
    unsigned int completionFlags = (CXCodeComplete_IncludeCodePatterns|CXCodeComplete_IncludeBriefComments);
//    completionFlags |= CXCodeComplete_IncludeMacros;

	CXUnsavedFile ufile = { ~filename, ~content, (unsigned)content.GetCount() };
	CXCodeCompleteResults *results;
	{
		RTIMING("Complete");
		DDUMP(line);
		DDUMP(column);
		DDUMP(filename);
		results = clang_codeCompleteAt(tu, filename, line, column, &ufile, 1, completionFlags);
	}
    if (results) {
		RTIMING("Parse results");
        DLOG("=====================================");
        DDUMP(FetchString(clang_codeCompleteGetContainerUSR(results)));
        for (unsigned int i = 0; i < results->NumResults; ++i) {
            const CXCursorKind kind = results->Results[i].CursorKind;
            const CXCompletionString& string = results->Results[i].CompletionString;
/*
            const CXAvailabilityKind availabilityKind = clang_getCompletionAvailability(string);
            if (!(options.options & Server::CompletionsNoFilter)) {
                switch (availabilityKind) {
                case CXAvailability_Available:
                    break;
                case CXAvailability_Deprecated:
                    break;
                case CXAvailability_NotAccessible:
                    continue;
                case CXAvailability_NotAvailable: // protected members are erroneously flagged as NotAvailable in clang 3.6
                    continue;
                }
            }
*/
			if(kind == CXCursor_MacroDefinition) // we probably want this only on Ctrl+Space
				continue;

			String name;
			String signature;
			int    args = -1;

            const int priority = clang_getCompletionPriority(string);
            const int chunkCount = clang_getNumCompletionChunks(string);
            for (int j=0; j<chunkCount; ++j) {
                const CXCompletionChunkKind chunkKind = clang_getCompletionChunkKind(string, j);
                String text = FetchString(clang_getCompletionChunkText(string, j));
                if (chunkKind == CXCompletionChunk_TypedText) {
                    name = text;
                    signature << text;
                    args = signature.GetCount();
                }
                else
				if (chunkKind == CXCompletionChunk_Placeholder) {
					signature << text;
                } else {
                    signature << text;
                    if (chunkKind == CXCompletionChunk_ResultType) {
                        signature << ' ';
                    }
                }
            }
/*
			String annotation;
            const unsigned int annotations = clang_getCompletionNumAnnotations(string);
            for (unsigned j=0; j<annotations; ++j)
                MergeWith(annotation, ", ", FetchString(clang_getCompletionAnnotation(string, j)));
*/ 
            DLOG(GetCursorKindName(kind) << ", @" << priority << " " << name << ": " << signature);
            if(args >= 0)
                DLOG(" Args: " << signature.Mid(args));
            DLOG(" Parent: " << FetchString(clang_getCompletionParent(string, NULL)));
            
/*
            DLOG(" Annotation: " << annotation);
            if (candidate) {
                candidate->kind = RTags::eatString(clang_getCursorKindSpelling(kind));
                candidate->priority = priority;
                candidate->parent = RTags::eatString(clang_getCompletionParent(string, nullptr));
                candidate->brief_comment = RTags::eatString(clang_getCompletionBriefComment(string));
                candidates.push_back(candidate);
                const unsigned int annotations = clang_getCompletionNumAnnotations(string);
                for (unsigned j=0; j<annotations; ++j) {
                    const CXStringScope annotation = clang_getCompletionAnnotation(string, j);
                    const char *cstr = clang_getCString(annotation);
                    if (strlen(cstr)) {
                        if (!candidate->annotation.empty())
                            candidate->annotation += ' ';
                        candidate->annotation + cstr;
                    }
                }
            }
  */
  			
		}

//        if (options.options & Server::CompletionDiagnostics)
  //          processDiagnostics(request, results, cache->translationUnit->unit);
        clang_disposeCodeCompleteResults(results);
    }

	clang_disposeTranslationUnit(tu);
}

#if 0
	Vector<String> sa = Split(cmdline, ' ');
	
	Buffer<const char *> argv(sa.GetCount());
	int i = 0;
	for(const String& s : sa)
		argv[i++] = ~s;
	
	CXIndex index = clang_createIndex(0, 0);
	if (!index)
		Panic("createIndex failed");

	unsigned options = clang_defaultEditingTranslationUnitOptions();
	
	DDUMP(options);

	CXTranslationUnit tu;
	{
		TIMING("Parse");
		CXUnsavedFile file = { ~filename, ~content, (unsigned)content.GetCount() };
		tu = clang_parseTranslationUnit(index, nullptr, argv, sa.GetCount(), nullptr, 0, options);
	
		if(!tu) {
			DLOG("Parse failed");
			return;
		}
	}

	if(0) {
		TIMING("Reparse");
		int failure = clang_reparseTranslationUnit(tu, 0, nullptr, options);
	
		if (failure)
			Panic("Reparse failed");
	}
#endif

void DumpDiagnostics(CXTranslationUnit tu)
{
	size_t num_diagnostics = clang_getNumDiagnostics(tu);

	for (size_t i = 0; i < num_diagnostics; ++i) {
		CXDiagnostic diagnostic = clang_getDiagnostic(tu, i);
		auto Dump = [&](CXDiagnostic diagnostic) {
			CXFile file;
			unsigned line;
			unsigned column;
			unsigned offset;
			CXSourceLocation location = clang_getDiagnosticLocation(diagnostic);
			clang_getExpansionLocation(location, &file, &line, &column, &offset);
			DLOG(FetchString(clang_getFileName(file)) <<
				" (" << line << ":" << column << ") " <<
				FetchString(clang_getDiagnosticSpelling(diagnostic)));
		};
		Dump(diagnostic);
	#if 0
		CXDiagnosticSet set = clang_getChildDiagnostics(diagnostic);
		int n = clang_getNumDiagnosticsInSet(set);
		for(int i = 0; i < n; i++) {
			CXDiagnostic d = clang_getDiagnosticInSet(set, i);
			Dump(d);
			clang_disposeDiagnostic(d);
		}
	#endif
		clang_disposeDiagnostic(diagnostic);
	}
}