#ifndef _GatherTpp_h_
#define _GatherTpp_h_

namespace Upp {

class GatherTpp {
public:
	void AddFolder(const char *folder) {rootFolders.Add(folder);};
	bool Load(String indexFile, Gate2<int, int> progress = false);
	
	String Www(const char *topic, int lang, String topicLocation = "topic://uppweb/www/");

	int FindTopic(const String name);
	Topic &GetTopic(int id);
	Topic &AddTopic(const String name);
	String GatherTopics(const char *topic, String& title);
	
	bool MakeHtml(const char *folder,  Gate2<int, int> progress = false);
	bool MakePdf(const char *filename, Gate2<int, int> progress = false);
	
	
private:
	Upp::Array <String> rootFolders;
	String dir;	
	VectorMap<String, String> escape;
public:
	VectorMap<String, String> labels;
	VectorMap<String, String> links;
	VectorMap<String, String> reflink;
	VectorMap<String, Topic> tt;

private:
	String keywords;	//
	String title;		//
	String indexTopic;

	String TopicFileName(const char *topic);
	String GatherTopics(const char *topic);
public:
	void GatherRefLinks(const char *upp);
private:
	void ExportPage(int i, String htmlFolder, String keywords = "");
	String QtfAsHtml(const char *qtf, Index<String>& css, const VectorMap<String, String>& links,
    	const VectorMap<String, String>& labels, const String& outdir, const String& fn = Null);
};

String ChangeTopicLanguage(const String &topic, int lang);
	
}

#endif
