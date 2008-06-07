#include "TCtrlLibCalc.h"
#pragma hdrstop

NAMESPACE_UPP

void UseHelpCalc() {}

HelpCalcMap GetHelpCalcGroup(const String& groupname)
{
	HelpCalcMap out_map;
	const HelpCalcMap& map = GetHelpCalcMap();
	for(int i = 0; i < map.GetCount(); i++)
		if(map[i]->groupfn && map[i]->groupfn() == groupname)
			out_map.Add(map[i]);
	Sort(out_map, HelpCalc::Compare(HelpCalc::Compare::ALPHABETIC));
	return out_map;
}

//////////////////////////////////////////////////////////////////////
// HelpCalcIndex::

/*
HelpCalcIndex& HelpCalcIndex::Get()
{
	static HelpCalcIndex index;
	return index;
}
*/

static void HelpCalcPutSummary(String& out)
{
	HelpCalcMap ix;
	ix <<= GetHelpCalcMap();
	Sort(ix, HelpCalc::Compare(HelpCalc::Compare::ALPHABETIC));

	Table table;
	table.Before(100).FrameWidth(0);

	const Color PAPER(255, 255, 224);
	table(0, 0).Ratio(10).Paper(PAPER).FrameWidth(0).Top().Qtf(t_("[*3 Function"));
	table(0, 1).Ratio(4).Paper(PAPER).FrameWidth(0).Top().Qtf(t_("[*3 Group"));

	RichPara::CharFormat groupstyle;
	groupstyle.Height(84);

	RichPara::CharFormat titlestyle;
	titlestyle.Height(100);
	titlestyle.Bold();
	titlestyle.paper = WhiteGray;

	RichPara::CharFormat indexstyle = titlestyle;

	String styles = HelpTopicGet(HelpHostDPP("style")).GetText() + "\r\n";

	RichText text;

	int row = 0;
	for(int i = 0; i < ix.GetCount(); i++)
	{
		String p0grp = ix[i]->groupfn();
		String top = ix[i]->topic, tit = ix[i]->GetTitle();
//		int begin = text.GetParaCount();
		{
			RichPara title;
			title.format.before = 200;
			indexstyle.indexentry = ("@@" + HelpAppDPP(top)).ToWString();
			title.Cat(RichObject(RichObjectTypeEmpty(), Null), indexstyle);
			MQTFCat(title, tit, titlestyle);
			text.Cat(title);
		}
		{
			RichPara group;
			group.format.after = 100;
			groupstyle.NoBold();
			group.Cat(WString("Skupina: "), groupstyle);
			groupstyle.Bold();
			MQTFCat(group, p0grp, groupstyle);
			text.Cat(group);
		}
		AppendQTF(text, styles + HelpTopicGet(HelpAppDPP(top)).GetText());
	}

	out = BodyAsQTF(text);
//	HelpBody() << table;
}

RegisterHelpTopicHost("calc_summary", "Seznam funkcí výrazového kalkulátoru", callback(&HelpCalcPutSummary))

static void HelpCalcPutGroups(String& out, bool book)
{
	HelpCalcMap ix;
	ix <<= GetHelpCalcMap();
	Sort(ix, HelpCalc::Compare(HelpCalc::Compare::GROUPS));
//	int cycle = 0;
	for(const HelpCalc *const *b = ix.Begin(), *const *p = b, *const *e = ix.End(); p < e;)
	{
		bool start = false;
		String p0grp = p[0]->groupfn();
		if(p == b || p0grp != p[-1]->groupfn())
		{
			String s;
			if(!IsNull(p0grp))
				s = p0grp;
			else if(p > b)
				s << t_("Miscellaneous");
			if(!IsNull(s))
				out << "[3* &" << s << "]&&\n";
			start = true;
		}

		const HelpCalc *const *o = p;
		while(++p < e && p[0]->ident == o[0]->ident
			&& p[0]->groupfn() == o[0]->groupfn())
			;

		for(const HelpCalc *const *t = o; t < p; t++)
		{
			String top = (**t).topic, tit = (**t).GetTitle();
			out
//			<< "[" << "123456789"[++cycle % 9] << " [^=shit^ SHIT][_ SHIT]&]\n";
			<< "[2i150";
			if(!book)
				out << '^' << top << '^';
			out << ' ' << tit;
			if(book)
				out << ", \n\n{:INDEX:" << DeQtf(top) << ":}\n";
			out << "&]\n";
		}
	}
}

RegisterHelpTopicHost("calc_groups", "Skupiny funkcí výrazového kalkulátoru", callback1(&HelpCalcPutGroups, false))
RegisterHelpTopicHost("calc_groups_book", "Skupiny funkcí výrazového kalkulátoru", callback1(&HelpCalcPutGroups, true))

#if 0
void HelpCalcIndex::AddIndexEntries() const
{
	for(int i = 0; i < index.GetCount(); i++)
	{
		const HelpCalc& calc = *index[i];
		String entry;
		entry << "[/ \1" << calc.id << "\1]";
		if(IsAlNum(*calc.id) || *calc.id == '_')
			entry << t_(", function");
		else
			entry << t_(", operator");
		HelpIndex::Get().AddIndex(entry, "Calc#" + IntStr(calc.ord));
	}
}
#endif

/*
HelpCalcIndex::HelpCalcIndex()
{
}
*/

#if 0
String HelpCalcLink(const char *id, const char *ref, bool bold)
{
	Vector<const HelpCalc *> candidates;
	const HelpCalcIndex::IdMap& map = HelpCalcIndex::Get().GetIndex();
	for(int i = map.Find(id); i >= 0; i = map.FindNext(i))
		candidates.Add(map[i]);
	if(candidates.IsEmpty())
		return id;
	SortX(candidates, XDeref(XRef<const HelpCalc>()), HelpCalc::Compare(HelpCalc::Compare::GROUPS));
	String anchor = "Calc#";
	anchor << candidates[0]->ord;
	return HelpTagLink(bold ? "*" : 0, anchor, id, ref);
}
#endif

/*
Index<Callback>& HelpCalcInit::Get()
{
	static bool guard;
	MtDblChkGuard __(guard);
	static Index<Callback> list;
	return list;
}

void HelpCalcInit::Run()
{
	Index<Callback>& fns = Get();
	for(int i = 0; i < fns.GetCount(); i++)
		fns[i]();
	fns.Clear();
}
*/

bool HelpCalc::Compare::operator () (const HelpCalc *a, const HelpCalc *b) const
{
	String agrp = MQTFStrip(a->groupfn()), bgrp = MQTFStrip(b->groupfn());
	int gcmp;
	if(IsNull(agrp) != IsNull(bgrp))
		gcmp = IsNull(bgrp) ? -1 : 1;
	else
		gcmp = lang.Compare(agrp, bgrp);

	int acmp;
	bool aoper = !CharFilterCalcIdent(*a->ident);
	bool boper = !CharFilterCalcIdent(*b->ident);
	if(aoper != boper)
		acmp = aoper ? -1 : 1;
	else
		acmp = lang.Compare(a->ident, b->ident);
	if(acmp == 0)
		acmp = lang.Compare(MQTFStrip(a->GetTitle()), MQTFStrip(b->GetTitle()));
	if(mode == GROUPS)
		return gcmp ? gcmp < 0 : acmp < 0;
	else if(mode == ALPHABETIC)
		return acmp ? acmp < 0 : gcmp < 0;
	else
	{
		NEVER();
		return false;
	}
}

void AddHelpCalcGroups(DropList& droplist, const String& prefix)
{
	Vector<String> groups = GetHelpCalcGroups();
	for(int i = 0; i < groups.GetCount(); i++)
		droplist.Add(prefix + groups[i], "Kalkulátor: " + Nvl(groups[i], "Ostatní"));
}

void AddHelpCalcGroup(DropList& dest, const String& group)
{
	HelpCalcMap map = GetHelpCalcGroup(group);
	for(int i = 0; i < map.GetCount(); i++)
		dest.Add(map[i]->ident, MQTFStrip(map[i]->GetTitle()));
}

/*
String HelpCalc::FormatArg(int i) const
{
	ASSERT(i >= 0 && i < arg_types.GetCount() && i < arg_names.GetCount());
	if(IsNull(arg_types[i]) && IsNull(arg_names[i]))
		return s_(HelpCalcArgTypeAny);
	String s;
	if(!IsNull(arg_types[i]))
		s << "\1" << arg_types[i] << "\1";
	if(!IsNull(arg_names[i]))
	{
		if(*s)
			s << ' ';
		s << "[/ " << arg_names[i] << "]";
	}
	return s;
}
*/

/*
String HelpCalc::FormatArgs() const
{
	String s;
	for(int i = 0; i < arg_types.GetCount(); i++)
	{
		if(i)
			s << ", ";
		s << FormatArg(i);
	}
	return s;
}
*/

/*
String HelpCalc::FormatResult() const
{
	if(IsNull(result))
		return Null;
	return "\1" + result + "\1";
}
*/

/*
String HelpCalc::FormatFunction() const
{
	String s;
	bool oper = !IsAlNum(*id) && *id != '_';
	if(oper)
		s << s_(HelpCalcFormatFunctionOperator);
	s << "[* \1" << id << "\1]";
	if(!arg_types.IsEmpty())
		s << " (" << FormatArgs() << ')';
	if(!IsNull(result))
		s << " [S \xAE] " << FormatResult();
	return s;
}
*/

/*
static void CalcInit()
{
	HelpCalcInit::Run();
	HelpCalcIndex::Get().AddIndexEntries();
}

static HelpInit MK__s = callback(CalcInit);
*/

/*
static void CalcPutHelp()
{
	HelpCalcIndex::Get().PutGroups();
}

STATIC_HELP(Calc, s_(CalcHelpTopic),
	s_(CalcHelpIndex)
)
*/

/*
static void CalcIndexPutHelp()
{
	HelpCalcIndex::Get().PutSummary();
}

STATIC_HELP(CalcIndex, s_(CalcIndexHelpTopic),
	s_(CalcIndexHelpIndex)
)
*/

END_UPP_NAMESPACE
