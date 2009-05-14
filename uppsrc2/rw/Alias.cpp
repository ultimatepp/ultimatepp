#include "Designer.h"

String AliasMap::StdFileName()
{
	return ConfigFile("rw.ali");
}

bool AliasMap::Load(const char *path)
{
	String file = LoadFile(path);
	if(file.IsVoid())
		return false;
	obj_source.Clear();
	obj_dest.Clear();
	for(const char *s = file; *s;)
	{
		bool is_dest = (*s == ' ' || *s == '\t');
		while(*s == ' ' || *s == '\t')
			s++;
		const char *b = s;
		while(*s && *s != '\n')
			s++;
		const char *e = s;
		while(e > b && (byte)e[-1] <= ' ')
			e--;
		String part(b, e);
		if(is_dest)
		{
			while(obj_dest.GetCount() < obj_source.GetCount())
				obj_dest.Add(part);
		}
		else
			obj_source.Add(part);
		if(*s)
			s++;
	}
	if(obj_dest.GetCount() < obj_source.GetCount())
		obj_source.SetCount(obj_dest.GetCount());
	IndexSort(obj_source, obj_dest, StdLess<String>());
	return true;
}

bool AliasMap::Save(const char *path)
{
	IndexSort(obj_dest, obj_source, StdLess<String>());
	String out;
	for(int i = 0; i < obj_source.GetCount(); i++)
	{
		out.Cat(obj_source[i]);
		out.Cat("\r\n");
		if(i + 1 >= obj_source.GetCount() || obj_dest[i + 1] != obj_dest[i])
		{
			out.Cat('\t');
			out.Cat(obj_dest[i]);
			out.Cat("\r\n");
		}
	}
	IndexSort(obj_source, obj_dest, StdLess<String>());
	return SaveFile(path, out);
}

void AliasMap::SetDefault()
{
	obj_source.Add("WithKeyMap<*>");         obj_dest.Add("@1");
	obj_source.Add("WithDropChoice<*>");     obj_dest.Add("@1");
	obj_source.Add("WithChoiceList<*>");     obj_dest.Add("@1");
	obj_source.Add("EditDoubleSpin");        obj_dest.Add("EditDouble");
	obj_source.Add("EditDoubleNotNullSpin"); obj_dest.Add("EditDouble");
	obj_source.Add("Ctrl");                  obj_dest.Add("Label");
	obj_source.Add("NotNullCtrl<*>");        obj_dest.Add("@1");
	obj_source.Add("SqlArray");              obj_dest.Add("ArrayCtrl");
	obj_source.Add("DataPusher");            obj_dest.Add("EditField");
}

static bool MatchAlias(const char *s, const char *t, Vector<String>& a)
{
	while(*t)
		switch(*t++)
		{
		case '*':
			{
				const char *b = s;
				do
				{
					a.Add(String(b, s));
					if(MatchAlias(s, t, a))
						return true;
					a.Drop();
				}
				while(*s++);
				return false;
			}

		default:
			if(t[-1] != *s++)
				return false;
			break;
		}
	return !*s;
}

String AliasMap::Convert(const String& name) const
{
	for(int i = 0; i < obj_source.GetCount(); i++)
	{
		Vector<String> args;
		if(MatchAlias(name, obj_source[i], args))
		{
			const char *s = obj_dest[i];
			String out;
			while(*s)
				if(*s == '@' && (s[1] >= '1' && s[1] < '1' + min(args.GetCount(), 10)))
				{
					s += 2;
					out.Cat(args[s[-1] - '1']);
				}
				else
					out.Cat(*s++);
			return out;
		}
	}
	return name;
}

class DlgAlias : public WithAliasLayout<TopWindow>
{
public:
	typedef DlgAlias CLASSNAME;
	DlgAlias();

	bool Run(AliasMap& map);

private:
	EditStringNotNull obj_source;
	EditStringNotNull obj_dest;
};

bool AliasMap::Edit()
{
	return DlgAlias().Run(*this);
}

DlgAlias::DlgAlias()
{
	CtrlLayoutOKCancel(*this, "Object alias map");
	list.AutoHideSb();
	list.AddColumn("Source object").Edit(obj_source);
	list.AddColumn("Alias object").Edit(obj_dest);
	list.Inserting().Removing().NoAskRemove();
}

bool DlgAlias::Run(AliasMap& map)
{
	list.SetCount(map.obj_source.GetCount());
	int i;
	for(i = 0; i < map.obj_source.GetCount(); i++)
	{
		list.Set(i, 0, map.obj_source[i]);
		list.Set(i, 1, map.obj_dest[i]);
	}
	if(!map.obj_source.IsEmpty())
		list.SetCursor(0);
	if(TopWindow::Run() != IDOK)
		return false;
	int n = list.GetCount();
	map.obj_source.SetCount(n);
	map.obj_dest.SetCount(n);
	for(i = 0; i < n; i++)
	{
		map.obj_source[i] = list.Get(i, 0);
		map.obj_dest[i]   = list.Get(i, 1);
	}
	return true;
}
