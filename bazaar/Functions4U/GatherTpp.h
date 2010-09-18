#ifndef _GatherTpp_h_
#define _GatherTpp_h_

class GatherTpp {
public:
	//GatherTpp();
	void AddFolder(const char *folder) {rootFolders.Add(folder);};
	bool Load(String indexFile, Gate2<int, int> progress = false);
	
	int FindTopic(const String name);
	Topic &GetTopic(int id);
	Topic &AddTopic(const String name);
	String GatherTopics(const char *topic, String& title);
	
	bool MakeHtml(const char *folder,  Gate2<int, int> progress = false);
	bool MakePdf(const char *filename, Gate2<int, int> progress = false);
	
	
private:
	Array <String> rootFolders;
	String dir;	
	VectorMap<String, String> escape;
	VectorMap<String, String> links;
	VectorMap<String, String> labels;
	VectorMap<String, String> reflink;
	VectorMap<String, Topic> tt;
	
	Htmls header;
	String keywords;	//
	String title;		//
	String indexTopic;

	String TopicFileName(const char *topic);
	String GatherTopics(const char *topic);
	void GatherRefLinks(const char *upp);
	void ExportPage(int i, String htmlFolder, String keywords = "");
	String QtfAsHtml(const char *qtf, Index<String>& css, const VectorMap<String, String>& links,
    	const VectorMap<String, String>& labels, const String& outdir, const String& fn = Null);
};

#endif
