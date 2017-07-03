#include "www.h"

#define LLOG(x)

VectorMap<String, SvnListRev> svndata;
Vector<SvnLogRev> svnlog;

void ParseSvnList(VectorMap<String, SvnListRev> &data, String &out, const String path, const String folder) {
	RLOG("SvnList " << path);
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
		String fullPath = AppendFileName(folder, name);
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
		rev.fullPath = fullPath;
	}
}
void GetSvnFolder(VectorMap<String, SvnListRev> &data, String tppfolder) {
	tppfolder = UnixPath(tppfolder);
	String out = Sys("svn list \"" + tppfolder + "\" --xml --recursive --non-interactive");
	int posp = tppfolder.ReverseFind('.');
	int pos = tppfolder.ReverseFind('/', posp-1);
	pos = tppfolder.ReverseFind('/', pos-1);
	String topic = "topic:/" + tppfolder.Mid(pos, posp-pos) + "/";
	ParseSvnList(data, out, topic, tppfolder);
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
	LLOG("Querying svn for documentation metadata ...");
	GetSvnFolder(data, AppendFileName(rootdir, "uppbox/uppweb/www.tpp"));
	GetSvnFolder(data, AppendFileName(rootdir, "uppbox/uppweb/gsoc.tpp"));
	GetSvnFolderDeep(data, AppendFileName(rootdir, "uppsrc"));
	GetSvnFolderDeep(data, AppendFileName(rootdir, "bazaar"));
}

void ParseSvnLog(Vector<SvnLogRev> &log, String& out){
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
			if((newpos = out.Find("action=\"", pos)) == -1)
				break;
			if(newpos > maxpos)
				break;
			pos = newpos + strlen("action=\"");
			if((newpos = out.Find('\"', pos)) == -1)
				break;
			SvnLogRev::SvnChange &chng = rev.changes.Add();
			chng.action = out.Mid(pos, newpos-pos);
			if((newpos = out.Find(">", pos)) == -1)
				break;
			pos = newpos + strlen(">");
			if((newpos = out.Find('<', pos)) == -1)
				break;
			chng.path = out.Mid(pos, newpos-pos);
			String p = chng.path.Mid(7);//skip "/trunk/"
			int t = rev.tags.FindAdd(p.Left(p.FindFirstOf("/")));
			p = p.Mid(rev.tags[t].GetCount()+1);
			rev.tags.FindAdd(p.Left(p.FindFirstOf("/")));
		}
		if((newpos = out.Find("<msg>", pos)) == -1)
			return;
		pos = newpos + strlen("<msg>");
		if((newpos = out.Find("<", pos)) == -1)
			return;
		rev.msg = out.Mid(pos, newpos-pos);
		if (rev.msg[0] == '.') 
			rev.major = false;
		else 
			rev.major = true;
		pos = newpos;
	}
}

void GetSvnLog(Vector<SvnLogRev> &log, int limit) {
	LLOG("Querying svn for revisions log ...");
	String out = Sys("svn log \"" + rootdir + "\" --xml --verbose --non-interactive" + 
					 ((limit > -1) ? " --limit " + FormatInt(limit) : ""));  
	ParseSvnLog(log,out);
}

String SvnChanges(Vector<SvnLogRev> &log, int limit, String filter, bool major) {
	String table = "{{700:800:800:2850:4232f0;g0;^t/25b4/25@(240) [s0;# [*2 Revision]]"
		":: [s0;# [*2 Date]]"
		":: [s0;# [*2 Author]]"
		":: [s0;# [*2 Description]]"
		":: [s0;# [*2 Files]]";
	for(int i = 0, c = 0; (c < limit) && (i < log.GetCount()); i++) {
		if(filter != "" && svnlog[i].tags.Find(filter) < 0) 
			continue; 
		if(major && !svnlog[i].major) 
			continue; 
		c++;
		table += "::t8/8b0/8@2 [s0; [^http`:`/`/code`.google`.com`/p/upp`-mirror`/source`/detail`?r`=" + svnlog[i].revision + "^2 " + svnlog[i].revision + "]]"
				":: [s0; [2 " + DeQtf(Format(svnlog[i].time, false)) + " ]]"
				":: [s0; [2 " + DeQtf(svnlog[i].author) + " ]]"
				":: [s0; [2 " + DeQtf(svnlog[i].msg) + " ]]"
				":: [s0; ";
		for(int j = 0; j < log[i].changes.GetCount(); j++)
			table += "[^http`:`/`/code`.google`.com`/p/upp`-mirror`/source`/diff`?r`=" + svnlog[i].revision + DeQtf("&amp;format=side&amp;path=") + DeQtf(svnlog[i].changes[j].path) + "^2 diff][2  " + svnlog[i].changes[j].action + " ]"
					 "[^http`:`/`/code`.google`.com`/p`/upp`-mirror`/source`/browse" + DeQtf(svnlog[i].changes[j].path) + "^2 " + DeQtf(svnlog[i].changes[j].path) + "]" + String(j == svnlog[i].changes.GetCount()-1 ? " " : "&");
		table+=" ]";
	}
	table += " }}";
	return table;
}

int64 GetFolderSize(String dir) {
	int64 size = 0;
	FindFile ff(AppendFileName(dir, "*.*"));
	while(ff) {
		String name = ff.GetName();
		String p = AppendFileName(dir, name);
		if(ff.IsFile())
			size += ff.GetLength();
		else if(ff.IsFolder())
			size += GetFolderSize(p);
		ff.Next();
	}
	return size;
}

String BytesToString(uint64 _bytes, bool units)
{
	String ret;
	uint64 bytes = _bytes;
	
	if (bytes >= 1024) {
		bytes /= 1024;
		if (bytes >= 1024) {
			bytes /= 1024;
			if (bytes >= 1024) {
				bytes /= 1024;
				if (bytes >= 1024) {
					bytes /= 1024;
					ret = Format("%.1f %s", _bytes/(1024*1024*1024*1024.), units ? "Tb" : "");
				} else
					ret = Format("%.1f %s", _bytes/(1024*1024*1024.), units ? "Gb" : "");
			} else
				ret = Format("%.1f %s", _bytes/(1024*1024.), units ? "Mb" : "");
		} else
			ret = Format("%.1f %s", _bytes/1024., units ? "Kb" : "");
	} else
		ret << _bytes << (units ? "b" : "");
	return ret;
}

Vector <SvnBazaarItems> SvnBazaarList(String bazaarPath, Vector<SvnLogRev> &log) {
	Vector <SvnBazaarItems> items;
	
	FindFile ff(AppendFileName(bazaarPath, "*.*"));
	while(ff) {
		String name = ff.GetName();
		if(ff.IsFolder() && name != "$.tpp" && name != ".svn") {
			SvnBazaarItems &item = items.Add();
			item.name = name;
			String p = AppendFileName(bazaarPath, item.name);
			item.size = GetFolderSize(p);
			item.authors = "";
			item.description = "";
			item.externalDependencies = "";
			item.imagePath = "";
			item.status = "";
			item.supportedOS = "";
		}
		ff.Next();
	}
	Vector<String> logPackages;	// Packages in log vector
	logPackages.SetCount(log.GetCount());
	for (int i = 0; i < log.GetCount(); ++i) {
		for (int j = 0; j < log[i].changes.GetCount(); ++j) {
			String path = log[i].changes[j].path;
			int ppos = path.Find("/bazaar/");
			if (ppos >= 0) {
				String package;
				ppos += strlen("/bazaar/");
				int endppos = path.Find("/", ppos+1);
				package = path.Mid(ppos, endppos-ppos);
				logPackages[i] += package + ", ";		
			}
		}
	}
	for (int i = 0; i < log.GetCount(); ++i) {
		String packages = logPackages[i];
		for (int j = 0; j < items.GetCount(); ++j) {
			if (packages.Find(items[j].name) >= 0) {
				if (IsNull(items[j].lastChange)) 
					items[j].lastChange = log[i].time;
				else if (log[i].time > items[j].lastChange)
					items[j].lastChange = log[i].time;
				if (items[j].authors.Find(log[i].author) < 0) {
					if (!items[j].authors.IsEmpty())
						items[j].authors = ", " + items[j].authors;
					items[j].authors = log[i].author + items[j].authors;
				}
			}
		}
	}
	return items;
	/*
	String table = "{{700:800:800:2850:4232:800:800:800:800f0;g0;^t/25b4/25@(240)"
		"   [s0;# [*2 Name]]"
		":: [s0;# [*2 Basic Description]]"
		":: [s0;# [*2 Authors]]"
		":: [s0;# [*2 Size]]"
		":: [s0;# [*2 Status]]"
		":: [s0;# [*2 Last Release]]"
		":: [s0;# [*2 External Dependencies]]"
		":: [s0;# [*2 Suported OS]]"
		":: [s0;# [*2 Image]]";
	for(int i = 0; i < items.GetCount(); i++) {
		table += "::t8/8b0/8@2"
				"   [s0; [2 " + DeQtf(items[i].name) + " ]]"
				":: [s0; [2 " + DeQtf(items[i].description) + " ]]"
				":: [s0; [2 " + DeQtf(items[i].authors) + " ]]"
				":: [s0; [2 " + BytesToString(items[i].size) + " ]]"				                      
				":: [s0; [2 " + DeQtf(items[i].status) + " ]]"	
				":: [s0; [2 " + Format("%`", Date(items[i].lastChange)) + " ]]"
				":: [s0; [2 " + DeQtf(items[i].externalDependencies) + " ]]"					
				":: [s0; [2 " + DeQtf(items[i].supportedOS) + " ]]"					                      
				":: [s0; ";
	}
	table += " }}";
	return table;*/
}

