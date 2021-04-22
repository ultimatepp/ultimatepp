#ifndef Upp_Markdown_h
#define Upp_Markdown_h

#include <Core/Core.h>

#include "MD4C/md4c.h"

namespace Upp {

class MarkdownConverter {
public:
    MarkdownConverter() : flags(MD_DIALECT_COMMONMARK) {}

    MarkdownConverter& CollapseWhitespaces(bool b = true)       { if(b) flags |= MD_FLAG_COLLAPSEWHITESPACE;       else flags &= ~MD_FLAG_COLLAPSEWHITESPACE; return *this; }
    MarkdownConverter& NoIndentedCodeblocks(bool b = true)      { if(b) flags |= MD_FLAG_NOINDENTEDCODEBLOCKS;     else flags &= ~MD_FLAG_NOINDENTEDCODEBLOCKS; return *this; }
    MarkdownConverter& Tables(bool b = true)                    { if(b) flags |= MD_FLAG_TABLES;                   else flags &= ~MD_FLAG_TABLES; return *this; }
    MarkdownConverter& WikiLinks(bool b = true)                 { if(b) flags |= MD_FLAG_WIKILINKS;                else flags &= ~MD_FLAG_WIKILINKS; return *this; }
    MarkdownConverter& Strikeout(bool b = true)                 { if(b) flags |= MD_FLAG_STRIKETHROUGH;            else flags &= ~MD_FLAG_STRIKETHROUGH; return *this; }
    MarkdownConverter& Underline(bool b = true)                 { if(b) flags |= MD_FLAG_UNDERLINE;                else flags &= ~MD_FLAG_UNDERLINE; return *this; }
    MarkdownConverter& NoHtmlBlocks(bool b = true)              { if(b) flags |= MD_FLAG_NOHTMLBLOCKS;             else flags &= ~MD_FLAG_NOHTMLBLOCKS; return *this; }
    MarkdownConverter& NoHtmlSpans(bool b = true)               { if(b) flags |= MD_FLAG_NOHTMLSPANS;              else flags &= ~MD_FLAG_NOHTMLSPANS; return *this; }
    MarkdownConverter& NoHtml(bool b = true)                    { if(b) flags |= MD_FLAG_NOHTML;                   else flags &= ~MD_FLAG_NOHTML; return *this; }
    MarkdownConverter& PermissiveAtxHeaders(bool b = true)      { if(b) flags |= MD_FLAG_PERMISSIVEATXHEADERS;     else flags &= ~MD_FLAG_PERMISSIVEATXHEADERS; return *this; }
    MarkdownConverter& PermissiveUrlAutolinks(bool b = true)    { if(b) flags |= MD_FLAG_PERMISSIVEURLAUTOLINKS;   else flags &= ~MD_FLAG_PERMISSIVEURLAUTOLINKS; return *this; }
    MarkdownConverter& PermissiveWWWAutolinks(bool b = true)    { if(b) flags |= MD_FLAG_PERMISSIVEWWWAUTOLINKS;   else flags &= ~MD_FLAG_PERMISSIVEWWWAUTOLINKS; return *this; }
    MarkdownConverter& PermissiveEmailAutolinks(bool b = true)  { if(b) flags |= MD_FLAG_PERMISSIVEEMAILAUTOLINKS; else flags &= ~MD_FLAG_PERMISSIVEEMAILAUTOLINKS; return *this; }
    MarkdownConverter& PermissiveAutolinks(bool b = true)       { if(b) flags |= MD_FLAG_PERMISSIVEAUTOLINKS;      else flags &= ~MD_FLAG_PERMISSIVEAUTOLINKS; return *this; }

    MarkdownConverter& CommonMarkDialect()                      { flags = MD_DIALECT_COMMONMARK; return *this; }
    MarkdownConverter& GitHubDialect()                          { flags = MD_DIALECT_GITHUB; return *this; }

    String ToQtf(const String& markdown_text);

    Event< VectorMap<String, String>& > WhenImages;

private:
    dword           flags;
};

}
#endif
