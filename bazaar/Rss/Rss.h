//**********************************************************************************************
//* Rss Class
//* ---------
//* RSS 2.0 Web content syndication format encapsulation for U++
//* For more information on RSS format, 
//* See: 
//*		http://www.rss-specifications.com/
//*		http://www.rssboard.org/rss-specification		
//*

#ifndef _Rss_Rss_h
#define _Rss_Rss_h

#include <Core/Core.h>

using namespace Upp;

class 	RssTag;
class 	RssNode;
class	RssChannel;
class	Rss;

#define RSS_HEADER			"rss"
#define RSS_VERSION			"version"
#define RSS_ENCODING		"encoding"
#define RSS_CHANNEL			"channel"
#define RSS_ITEM			"item"
#define RSS_TITLE			"title"
#define RSS_LINK			"link"
#define RSS_DESCRIPTION		"description"
#define RSS_URL				"url"
#define RSS_NAME			"name"
#define RSS_AUTHOR			"author"
#define RSS_COMMENTS		"comments"
#define RSS_LANGUAGE		"language"
#define RSS_COPYRIGHT		"copyright"
#define RSS_GENERATOR		"generator"
#define RSS_WEBMASTER		"webMaster"
#define RSS_PUBDATE			"pubDate"
#define RSS_MANAGINGEDITOR	"managingEditor"
#define RSS_LASTBUILDDATE	"lastBuildDate"
#define RSS_DOCS			"docs"
#define RSS_GUID			"guid"
#define RSS_TTL				"ttl"
#define RSS_ENCLOSURE		"enclosure"
#define RSS_SOURCE			"source"
#define RSS_IMAGE			"image"
#define RSS_CATEGORY		"category"
#define RSS_TEXTINPUT		"textInput"
#define RSS_CLOUD			"cloud"
#define RSS_SKIPHOURS		"skipHours"
#define RSS_SKIPDAYS		"skipDays"

// RSS:<channel> sub-structures/elements.
RssNode RssItem(const char* title, const char* link, const char* desc, const char* author = NULL, 
	const char* comments = NULL, const char* pubdate = NULL, RssTag* category = NULL, RssTag* guid = NULL, 
	RssTag* source = NULL, RssTag* enclosure = NULL);
RssNode RssImage(const char* title, const char* link, const char* url, const char* desc = NULL, Size sz = Null);
RssNode RssTextInput(const char* title, const char* link, const char* desc, const char* name);
RssTag 	RssCloud(const char* domain, int port, const char* path, const char* registerProcedure, const char* protocol);
RssTag 	RssCategory(const char* category, const char* domain = NULL);

// RSS:<item> sub-structures/elements.
RssTag 	RssGuid(const char *id, bool islink = true);
RssTag 	RssSource(const char* url, const char* src);
RssTag 	RssEnclosure(const char* url, double length, const char* type);

// RSS general type definitions, and declarations;
typedef Vector<RssTag> RssTags;
typedef Vector<RssNode> RssNodes;
typedef VectorMap<String, String> RssAttributes;
typedef Vector<RssChannel> RssChannels;


class RssTag : Moveable<RssTag> {
public:
	RssTag&			Tag(const char* tag)								{ if(tag)  tag0 = tag;	return *this;				}
	RssTag&			Text(const char* text)								{ if(text) txt0 = text;	return *this;				}
	RssTag&			Attr(const char* attr, const char* val)				{ if(val) attrs.FindPut(attr, val); return *this; 	}						           
	String			Tag() const											{ return tag0;	}					
	String			Text() const										{ return txt0;	}	
 	RssAttributes& 	Attrs() 											{ return attrs;	}

	RssTag&			operator=(const RssTag& src)						{ Copy(src); return *this;			}
	RssTag&			operator()(const char* attr, const char *val)		{ return Attr(attr, val);			}
	RssTag&			operator()(const char* attr, int q)					{ return Attr(attr, AsString(q));	}
	RssTag&			operator()(const char* attr, double q)				{ return Attr(attr, AsString(q));	}
	RssTag&			operator()(const char* tag)							{ return Tag(tag);					}
	
	bool			HasAttr() const										{ return bool(attrs.GetCount());	}
	bool			IsNullInstance() const								{ return IsNull(tag0) && IsNull(txt0) && !HasAttr();} 

	virtual	void	Clear()												{ tag0 = Null; txt0 = Null; attrs.Clear(); }
	
private:
	void			Copy(const RssTag& src);
	String			tag0, txt0;
	RssAttributes	attrs;

public:	
	RssTag(){}
	RssTag(const RssTag& src)											{ Copy(src); 					}
	RssTag(const char* tag, const char* text = NULL)					{ Tag(tag).Text(text); 			}	
	RssTag(const Nuller&)												{ Clear();						}
};

class RssNode : Moveable<RssNode>, public RssTag {
public:
	RssNode&		operator=(const RssNode& src)						{ Copy(src);return *this;   }
	RssNode& 		operator()(const char* tag)							{ Tag(tag); return *this;		}
	RssNode&		operator<<(const RssTag& tag)						{ if(!IsNull(tag)) tags.Add(tag); 		return *this;}
	RssNode&		operator<<(const RssNode& node)						{ if(!IsNull(node)) nodes.Add(node); 	return *this;}
	
	RssTags&		Tags() 												{ return tags; 				}
	RssNodes&		Nodes()												{ return nodes;				}
	
	bool			HasNode() const										{ return bool(nodes.GetCount());}
	bool			HasTag() const										{ return bool(tags.GetCount());	}
	bool			IsNullInstance() const								{ return !HasNode() && !HasTag(); }

	virtual	void	Clear()												{ RssTag::Clear(); tags.Clear(); nodes.Clear(); }	
		
private:
	void			Copy(const RssNode& src);
	Vector<RssTag> 	tags;
	Vector<RssNode>	nodes;

public:
	RssNode() 															{ 				}
	RssNode(const char *tag)											{ Tag(tag);		}
	RssNode(const RssNode& src)											{ Copy(src); 	}
	RssNode(const Nuller&)												{ Clear();	 	}	
};


class RssChannel : Moveable<RssChannel> {
public:
	typedef RssChannel CLASSNAME;

	RssChannel&					Tag(const RssTag& t); 
	RssChannel&					Node(const RssNode& t);
	
	RssChannel&					Title(const char* txt)				{ Tag(RssTag(RSS_TITLE, txt));				return *this;	}
	RssChannel&					Link(const char* txt)				{ Tag(RssTag(RSS_LINK, txt));				return *this;	}
	RssChannel&					Description(const char* txt)		{ Tag(RssTag(RSS_DESCRIPTION, txt));			return *this;	}
	RssChannel&					Language(const char* txt)			{ Tag(RssTag(RSS_LANGUAGE, txt)); 			return *this;	}
	RssChannel&					Copyright(const char* txt)			{ Tag(RssTag(RSS_COPYRIGHT, txt));			return *this;	}
	RssChannel&					Generator(const char* txt)			{ Tag(RssTag(RSS_GENERATOR, txt));			return *this;	}
	RssChannel&					ManagingEditor(const char* txt)		{ Tag(RssTag(RSS_MANAGINGEDITOR, txt));		return *this;	}
	RssChannel&					PubDate(const char* txt)			{ Tag(RssTag(RSS_PUBDATE, txt));				return *this;	}
	RssChannel&					LastBuildDate(const char* txt)		{ Tag(RssTag(RSS_LASTBUILDDATE, txt));		return *this;	}
	RssChannel&					WebMaster(const char* txt)			{ Tag(RssTag(RSS_WEBMASTER, txt));			return *this;	}
	RssChannel&					Docs(const char* txt)				{ Tag(RssTag(RSS_DOCS, txt));				return *this;	}
	RssChannel&					TTL(int ttl)						{ Tag(RssTag(RSS_TTL, AsString(ttl)));		return *this;	}
	RssChannel&					Category(const RssTag& t)			{ Tag(t); return *this; 	}
	RssChannel&					Cloud(const RssTag& t)				{ Tag(t); return *this; 	}
	RssChannel&					Item(const RssNode& n)				{ Node(n); return *this; 	}
	RssChannel&					Image(const RssNode& n)				{ Node(n); return *this; 	}
	RssChannel&					TextInput(const RssNode& n)			{ Node(n); return *this; 	}
	RssChannel&					SkipDays(const char* d);			
	RssChannel&					SkipHours(int h);						


	String						Title()								{ return GetTag(tags, RSS_TITLE).Text();  			}
	String						Link()								{ return GetTag(tags, RSS_LINK).Text();				}
	String						Description() 						{ return GetTag(tags, RSS_DESCRIPTION).Text();		}
	String						Language() 							{ return GetTag(tags, RSS_LANGUAGE).Text(); 		}
	String						Encoding() 							{ return GetTag(tags, RSS_ENCODING).Text();			}
	String						Copyright() 						{ return GetTag(tags, RSS_COPYRIGHT).Text(); 		}
	String						Generator() 						{ return GetTag(tags, RSS_GENERATOR).Text(); 		}
	String						ManagingEditor() 					{ return GetTag(tags, RSS_MANAGINGEDITOR).Text();	}
	String						PubDate() 							{ return GetTag(tags, RSS_PUBDATE).Text(); 			}
	String						LastBuildDate() 					{ return GetTag(tags, RSS_LASTBUILDDATE).Text();	}
	String						WebMaster() 						{ return GetTag(tags, RSS_WEBMASTER).Text(); 		}
	String						Docs() 								{ return GetTag(tags, RSS_DOCS).Text(); 			}
	String						Version() 							{ return GetTag(tags, RSS_VERSION).Text();			}
	String						TTL() 								{ return GetTag(tags, RSS_TTL).Text();				}
	RssTag						Category() 							{ return GetTag(tags, RSS_CATEGORY);				}
	RssTag						Cloud() 							{ return GetTag(tags, RSS_CLOUD);					}
	RssNode						Image() 							{ return GetNode(nodes, RSS_IMAGE);					}
	RssNode						TextInput() 						{ return GetNode(nodes, RSS_TEXTINPUT);				}
	RssNode						Item()								{ return GetNode(nodes, RSS_ITEM);					}
	Vector<String>&				SkipDays() 							{ return skipdays;									}
	Vector<int>&				SkipHours() 						{ return skiphours;									}

	RssTags&					Tags()								{ return (RssTags&)tags.GetValues();		}
	RssNodes&					Nodes()								{ return (RssNodes&)nodes.GetValues();		}

	RssChannel&					operator=(const RssChannel& src)	{ Copy(src); return *this;	}

	bool						HasItem() const						{ return bool(tags.GetCount()) || bool(nodes.GetCount());}
	bool 						IsNullInstance() const				{ return !HasItem();			}
	virtual	void				Clear()								{ tags.Clear(); nodes.Clear(); 	}	
			

protected:
	void						Copy(const RssChannel& src);
	RssTag						GetTag(VectorMap<String, RssTag>& t, const char*  tag);
	RssNode						GetNode(VectorMap<String, RssNode>& n, const char* tag);

private:
	VectorMap<String, RssTag>	tags;
	VectorMap<String, RssNode>	nodes;
	Vector<String> 				skipdays;
	Vector<int>					skiphours;
	
public:
	RssChannel() {}
	RssChannel(const RssChannel& src)								{ Copy(src); 	}	
	RssChannel(const Nuller&)										{ Clear(); 		}
};

class Rss : Moveable<Rss> {
public:
	typedef Rss CLASSNAME;
	
	Rss&						Encoding(byte c)					{ charset = CharsetName(c);  return *this;	}
	Rss&						Version(const char* v)				{ version = v;  			 return *this;	}
	Rss&						Channel(const RssChannel& channel);					

	String						Version()							{ return version;				}
	String						Encoding()							{ return charset;				}
	RssChannels&				Channels()							{ return channels;				}
	int							ChannelCount()						{ return channels.GetCount();	}

	virtual bool				Compose(String& out);
	virtual bool				Parse(String& in);

	String						GetError() const					{ return error;					}
	Rss&						operator=(const Rss& src)			{ Copy(src); return *this;		}

	bool						HasChannel() const					{ return bool(channels.GetCount()); }
	bool 						IsNullInstance() const				{ return !HasChannel();				}
	virtual void				Clear()								{ channels.Clear(); Version("2.0"); Encoding(CHARSET_DEFAULT); error = Null; }
	
protected:
	String						MakeTags(RssTags& tags);
	String						MakeNodes(RssNodes& nodes);
	void						Copy(const Rss& src);

	String						error;

private:
	RssChannels					channels;
	String 						version;
	String 						charset;
	
public:
	Rss()															{ 		 		}							
	Rss(const Rss& src)												{ Copy(src);	}
	Rss(const Nuller&)												{ Clear();		}
	Rss(RssChannel& channel, const char* version = RSS_VERSION, byte charset = CHARSET_DEFAULT)	 {}	
};

#endif

