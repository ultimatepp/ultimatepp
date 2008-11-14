void ItemList::Clear(String x)
{
}

String Worker::ValuesDelete(CParser& p, bool del, bool pcre)
{
	Vector<String> pat = Split(p.GetPtr(), ' ');
	if(del && pat.GetCount() != 3)
		return "You have to provide 3 patterns in order to delete values\n";
	pat.SetCount(3);
	pat ;
	RegExp rx[4];
	rx[0].
	RegExp rx1, rx2, rx3;
	if(pcre) {
		rx1.SetPattern(Nvl(pat[0], ".*"));
		rx2.SetPattern(Nvl(pat[1], ".*"));
		rx3.SetPattern(Nvl(pat[2], ".*"));
	}
	LocalSqlSession mysql;
	Sql sql(mysql);
	S_TAG_VALUE tv;
	sql * Select(tv).From(TAG_VALUE);
	Array<ValueEntry> m;
	while(sql.Fetch(tv)) {
		ValueEntry e;
		e.target = GetSeqTag(sql[SUBJECT_SEQ]);
		e.id = GetSeqTag(sql[ID_SEQ]);
		e.value = sql[TEXT];
		if(pcre ? rx1.FastMatch(e.target) && rx2.FastMatch(e.id) && rx3.FastMatch(e.value) :
		   WildMatch(pat[0], e.target) && WildMatch(pat[1], e.id) && WildMatch(pat[2], e.value)) {
			if(del)
				Remove(tv.SUBJECT_SEQ, tv.ID_SEQ);
			m.Add(e);
		}
	}
	Sort(m);
	String r;
	for(int i = 0; i < m.GetCount(); i++)
		r << m[i].target << ' ' << m[i].id << ' ' << AsCString(m[i].value) << '\n';
	r << '#' << m.GetCount() << '\n';
	return r;
}
