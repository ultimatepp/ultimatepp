#include "www.h"

VectorMap<String, SvnListRev> svndata;
Vector<SvnLogRev> svnlog;

void ParseSvnList(VectorMap<String, SvnListRev> &data, String &out, const String path) {
	String topicFolder;
	
	String line;
	int pos = 0;
	int newpos;
	while (true) {
		int linepos;
		if((linepos = out.Find("kind=\"file\"", pos)) == -1)
			return;
		if((newpos = out.Find("<name>", linepos)) == -1)
			return;
		pos = newpos + strlen("<name>");
		if((newpos = out.Find("<", pos)) == -1)
			return;
		String name = out.Mid(pos, newpos-pos);
		if((newpos = name.Find('.')) != -1)
			name = name.Mid(0, newpos);
		SvnListRev &rev = data.Add(path + name);
		if((newpos = out.Find("revision=\"", linepos)) == -1)
			return;
		pos = newpos + strlen("revision=\"");
		if((newpos = out.Find('\"', pos)) == -1)
			return;
		rev.revision = ScanInt(out.Mid(pos, newpos-pos));
		if((newpos = out.Find("<author>", linepos)) == -1)
			return;
		pos = newpos + strlen("<author>");
		if((newpos = out.Find("<", pos)) == -1)
			return;
		rev.author = out.Mid(pos, newpos-pos);
		if((newpos = out.Find("<date>", linepos)) == -1)
			return;
		pos = newpos + strlen("<date>");
		if((newpos = out.Find("<", pos)) == -1)
			return;
		String time = out.Mid(pos, newpos-pos);		
		rev.time.year = ScanInt(time.Left(4));
		rev.time.month = ScanInt(time.Mid(5, 2));
		rev.time.day = ScanInt(time.Mid(8, 2));
		rev.time.hour = ScanInt(time.Mid(11, 2));
		rev.time.minute = ScanInt(time.Mid(14, 2));
		rev.time.second = ScanInt(time.Mid(17));		
	}
}
void GetSvnFolder(VectorMap<String, SvnListRev> &data, String tppfolder) {
	tppfolder = UnixPath(tppfolder);
	String out = Sys("svn list \"" + tppfolder + "\" --xml --recursive --non-interactive");
	int posp = tppfolder.ReverseFind('.');
	int pos = tppfolder.ReverseFind('/', posp-1);
	pos = tppfolder.ReverseFind('/', pos-1);
	String topic = "topic:/" + tppfolder.Mid(pos, posp-pos) + "/";
	ParseSvnList(data, out, topic);
}
void GetSvnFolderDeep(VectorMap<String, SvnListRev> &data, const String &tppfolder) {
	FindFile fftpp(AppendFileName(tppfolder, "*.tpp"));
	while(fftpp) {
		String name = fftpp.GetName();
		String p = AppendFileName(tppfolder, name);
		if(fftpp.IsFolder())
			GetSvnFolder(data, p);
		fftpp.Next();
	}
	FindFile ff(AppendFileName(tppfolder, "*.*"));
	while(ff) {
		String name = ff.GetName();
		String p = AppendFileName(tppfolder, name);
		if(ff.IsFolder())
			GetSvnFolderDeep(data, p);
		ff.Next();
	}
}
void GetSvnList(VectorMap<String, SvnListRev> &data, const String &rootdir) {
	RLOG("Querying svn for documentation metadata ...");
	GetSvnFolder(data, AppendFileName(rootdir, "uppbox/uppweb/www.tpp"));
	GetSvnFolderDeep(data, AppendFileName(rootdir, "uppsrc"));
	GetSvnFolderDeep(data, AppendFileName(rootdir, "bazaar"));
}

void ParseSvnLog(Vector<SvnLogRev> &log,String& out){
	int pos = 0;
	int newpos;
	while (true) {
		if((newpos = out.Find("revision=\"", pos)) == -1)
			return;
		pos = newpos + strlen("revision=\"");
		if((newpos = out.Find('\"', pos)) == -1)
			return;
		SvnLogRev &rev = log.Add();
		rev.revision = out.Mid(pos, newpos-pos);
		if((newpos = out.Find("<author>", pos)) == -1)
			return;
		pos = newpos + strlen("<author>");
		if((newpos = out.Find("<", pos)) == -1)
			return;
		rev.author = out.Mid(pos, newpos-pos);
		if((newpos = out.Find("<date>", pos)) == -1)
			return;
		pos = newpos + strlen("<date>");
		if((newpos = out.Find("<", pos)) == -1)
			return;
		String time = out.Mid(pos, newpos-pos);
		rev.time.year = ScanInt(time.Left(4));
		rev.time.month = ScanInt(time.Mid(5, 2));
		rev.time.day = ScanInt(time.Mid(8, 2));
		rev.time.hour = ScanInt(time.Mid(11, 2));
		rev.time.minute = ScanInt(time.Mid(14, 2));
		rev.time.second = ScanInt(time.Mid(17));
		if((newpos = out.Find("<paths>", pos)) == -1)
			return;
		pos = newpos + strlen("<paths>");
		int maxpos;
		if((maxpos = out.Find("</paths>", pos)) == -1)
			return;
		while(true) {
			if((newpos = out.Find("kind=\"", pos)) == -1)
				break;
			if(newpos>maxpos)
				break;
			pos = newpos + strlen("kind=\"");
			if((newpos = out.Find('\"', pos)) == -1)
				break;
			SvnLogRev::SvnChange &chng = rev.changes.Add();
			chng.kind = out.Mid(pos, newpos-pos);
			if((newpos = out.Find("action=\"", pos)) == -1)
				break;
			pos = newpos + strlen("action=\"");
			if((newpos = out.Find('\"', pos)) == -1)
				break;
			chng.action = out.Mid(pos, newpos-pos);
			if((newpos = out.Find(">", pos)) == -1)
				break;
			pos = newpos + strlen(">");
			if((newpos = out.Find('<', pos)) == -1)
				break;
			chng.path = out.Mid(pos, newpos-pos);
		}
		if((newpos = out.Find("<msg>", pos)) == -1)
			return;
		pos = newpos + strlen("<msg>");
		if((newpos = out.Find("<", pos)) == -1)
			return;
		rev.msg = out.Mid(pos, newpos-pos);
		pos=newpos;
	}
}

void GetSvnLog(Vector<SvnLogRev> &log){
	RLOG("Querying svn for revisions log ...");
	String out = Sys("svn log \"" + rootdir + "\" --xml --verbose --non-interactive --limit 30");
	ParseSvnLog(log,out);
}
