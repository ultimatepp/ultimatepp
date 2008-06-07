#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	const Vector<String>& arg = CommandLine();
	if(arg.GetCount() != 1) {
		Cout() << "Usage: idmap file\n";
		SetExitCode(1);
		return;
	}
	FileIn in(arg[0]);
	if(!in) {
		SetExitCode(2);
		return;
	}
	VectorMap< String, Vector<int> > map;
	int line = 1;
	while(!in.IsEof()) {
		int c = in.Get();
		if(isalpha(c) || c == '_') {
			String id;
			id.Cat(c);
			c = in.Get();
			while(c > 0 && (isalnum(c) || c == '_')) {
				id.Cat(c);
				c = in.Get();
			}
			map.GetAdd(id).Add(line);
		}
		else
		if(c == '\n')
			line++;
	}
	Vector<int> order = GetSortOrder(map.GetKeys());
	for(int i = 0; i < order.GetCount(); i++) {
		Cout() << Format("%-32s: ", ~map.GetKey(order[i]));
		const Vector<int>& l = map[order[i]];
		for(int i = 0; i < l.GetCount(); i++) {
			if(i) Cout() << ", ";
			Cout() << l[i];
		}
		Cout() << '\n';
	}
}
