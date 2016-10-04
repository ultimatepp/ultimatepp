#include <Web/Web.h>
#include <Browser/Browser.h>

HtmlTag HtmlLine();
HtmlTag HtmlPara();
HtmlTag HtmlCourier(int size);
HtmlTag HtmlArial(int size);
HtmlTag HtmlRoman(int size);
HtmlTag HtmlCourier();
HtmlTag HtmlArial();
HtmlTag HtmlRoman();
HtmlTag HtmlFntSize(int size);

String  HtmlFontStyle(Font f, Font base);
String  HtmlFontStyle(Font f);

String AsHtml(const RichText& text, Index<String>& css, const VectorMap<String, String>& links,
              const String& path, const String& base = Null);
String AsCss(Index<String>& ss);
