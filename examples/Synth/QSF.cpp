#include "Synth.h"

struct QSFStatus : Moveable<QSFStatus> {
	double tempo = 120;
	int    sound = -1;
	double duration = 1;
	int    shift = 24;

	double volume = 1;
	double volume_low = 0;
	double volume_high = 1;
	double volume_start_at = 0;
	double volume_mul = 0;
	
	double GetVolume(double at) const { return clamp(volume + (at - volume_start_at) * volume_mul, volume_low, volume_high); }
};

struct QSFParser : SoundSequence, QSFStatus {
	int         lineno;
	String      line;
	const char *ptr;
	VectorMap<String, String> macro;
	VectorMap<int, String> errors;

	double at = 0;
	double at0 = 0;
	double end = 0;

	Vector<QSFStatus> stack;
	Vector<double> at_stack;
	
	void   Error(const char *s);
	void   Spaces();
	String ReadID();
	void   Expand();
	
	void   Sound(const char *s);
	void   Tone(int tone, double duration);
	void   Process();
	
	void   Parse(Stream& s);
	
	QSFParser() {
		Sound("130.81:L250V100R30::::");
	}
};

void QSFParser::Error(const char *s)
{
	errors.Add(lineno, s);
}

void QSFParser::Spaces()
{
	while(IsSpace(*ptr)) ptr++;
}

String QSFParser::ReadID()
{
	Spaces();
	String id;
	if(IsAlpha(*ptr) || *ptr == '_') {
		const char *b = ptr;
		while(IsAlNum(*ptr) || *ptr == '_')
			ptr++;
		id = String(b, ptr);
	}
	return id;
}

void QSFParser::Expand()
{
	bool expanded;
	auto Error = [&](const char *s) { errors.Add(lineno, s); };
	Index<int> used_macro;
	do {
		expanded = false;
		String r;
		ptr = line;
		Index<int> pm;
		auto GetMacro = [&]()->String {
			String id = ReadID();
			if(id.GetCount()) {
				int q = macro.Find(id);
				if(q >= 0) {
					if(used_macro.Find(q) < 0) {
						pm.FindAdd(q);
						expanded = true;
						return macro[q];
					}
					else
						Error("recursive macro " + id);
				}
				else
					Error("uknown macro " + id);
			}
			else
				Error("missing macro id");
			return Null;
		};
		while(*ptr) {
			if(*ptr == '$') {
				ptr++;
				Spaces();
				if(IsDigit(*ptr)) {
					int n = 0;
					while(IsDigit(*ptr)) {
						n = 10 * n + *ptr - '0';
						ptr++;
					}
					Spaces();
					String txt;
					int lvl = 0;
					auto Pars = [&](int l, int r) {
						while(*ptr) {
							if(*ptr == l)
								lvl++;
							if(*ptr == r && lvl-- == 0)
								break;
							ptr++;
						}
					};
					if(*ptr == '[') {
						const char *b = ptr++;
						Pars('[', ']');
						if(*ptr) ptr++;
						txt = String(b, ptr);
					}
					else
					if(*ptr == '(') {
						const char *b = ++ptr;
						Pars('(', ')');
						txt = String(b, ptr);
						if(*ptr) ptr++;
					}
					else
						txt = GetMacro();
					
					if(n > 0 && n < 100000) {
						while(n--)
							r << txt << ' ';
						expanded = true;
					}
					else
						Error("invalid repetition number");
				}
				else
					r << GetMacro();
			}
			else
				r << *ptr++;
		}
		line = r;
		for(int q : pm)
			used_macro.Add(q);
	}
	while(expanded);
}

void QSFParser::Parse(Stream& src)
{
	while(!src.IsEof()) {
		lineno++;
		line = src.GetLine();
		int q = line.Find("//");
		if(q >= 0)
			line.Trim(q);
		line = TrimBoth(line);
		if(*line == '#') {
			ptr = ~line + 1;
			String id = ReadID();
			Spaces();
			if(id.GetCount())
				macro.GetAdd(id) = ptr;
			else
				Error("missing macro name");
		}
		else {
			Expand();
			Process();
		}
	}
	
}

void QSFParser::Sound(const char *s)
{
	sound = SoundIndex(s);
}

void QSFParser::Tone(int tone, double duration)
{
	duration = duration / tempo * 240;
	if(tone == -1)
		At(at);
	else
		Put(at, sound, 100 * GetVolume(at), 65.406 * exp2(tone / 12.0), 1000 * duration, IsNull(tone));
	at += duration;
	end = max(at, end);
}

void QSFParser::Process()
{
	ptr = line;
	auto ReadNumber = [&]() -> double {
		CParser p(ptr);
		p.NoSkipSpaces().NoSkipComments();
		p.SkipSpaces();
		if(p.IsDouble()) {
			double h = p.ReadDouble();
			ptr = p.GetPtr();
			return h;
		}
		return 0;
	};
	auto ReadCNumber = [&]() -> double {
		Spaces();
		if(*ptr == ':')
			ptr++;
		return ReadNumber();
	};

	auto DoTone = [&](int tone) {
		ptr++;
		double ln = 1;
		bool chrd = false;
		for(;;) {
			if(*ptr == '\'' || *ptr == '^')
				tone += 12;
			else
			if(*ptr == ',')
				tone -= 12;
			else
			if(*ptr == '=')
				ln = ln + 1;
			else
			if(*ptr == '.')
				ln = ln + 0.5;
			else
			if(*ptr == ':')
				ln = ln + 0.75;
			else
			if(*ptr == '&')
				chrd = true;
			else
			if(*ptr != ' ')
				break;
			ptr++;
		}
		double h = at;
		Tone(shift + tone, ln * duration);
		if(chrd)
			at = h;
	};
	while(*ptr) {
		if(*ptr == ';') {
			ptr++;
			if(*ptr == ';') {
				at0 = at;
				ptr++;
			}
			else
				at = at0;
			duration = 1;
			shift = 24;
		}
		else
		if(*ptr == '$') {
			ptr++;
			tempo = ReadNumber();
			if(tempo < 1) {
				Error("invalid tempo value");
				tempo = 600;
			}
		}
		else
		if(*ptr == '+') {
			ptr++;
			shift += 12;
		}
		else
		if(*ptr == '-') {
			ptr++;
			shift -= 12;
		}
		else
		if(*ptr == '@') {
			ptr++;
			shift += (int)ReadNumber();
		}
		else
		if(*ptr >= '0' && *ptr <= '9')
			DoTone(*ptr - '0');
		else
		if(*ptr == 't')
			DoTone(10);
		else
		if(*ptr == 'e')
			DoTone(11);
		else
		if(*ptr == '_') {
			Tone(-1, duration);
			ptr++;
		}
		else
		if(*ptr == '*') {
			Tone(Null, duration);
			ptr++;
		}
		else
		if(*ptr == '/') {
			ptr++;
			duration = 1.0 / ReadNumber();
		}
		else
		if(*ptr == 'q') {
			ptr++;
			duration = 1.0 / 4;
		}
		else
		if(*ptr == 'o' || *ptr == 'w') {
			ptr++;
			duration = 1.0 / 8;
		}
		else
		if(*ptr == 'x') {
			ptr++;
			duration = 1.0 / 16;
		}
		else
		if(*ptr == 'y') {
			ptr++;
			duration = 1.0 / 32;
		}
		else
		if(*ptr == 'm') {
			ptr++;
			duration = 1.0 / 2;
		}
		else
		if(*ptr == 'n') {
			ptr++;
			duration = 1.0;
		}
		else
		if(*ptr == '%') {
			ptr++;
			duration *= ReadNumber();
			if(duration <= 0) {
				Error("invalid tone length");
				duration = 1;
			}
		}
		else
		if(*ptr == '!') {
			ptr++;
			String snd;
			Spaces();
			if(IsAlpha(*ptr)) {
				String id = ReadID();
				if(id == "loop")
					LoopAt(at);
				if(id == "shift")
					shift += (int)ReadCNumber();
				if(id == "volume") {
					volume = ReadCNumber();
					volume_mul = 0;
				}
				if(id == "volume_to") {
					volume = GetVolume(at);
					double v = ReadCNumber();
					double len = ReadCNumber();
					volume_low = min(v, volume);
					volume_high = max(v, volume);
					volume_start_at = at;
					volume_mul = (v - volume) / (240 / tempo) / len;
				}
				if(id == "cursor")
					SoundSequence::cursor = GetAt(at);
				if(id == "cut") {
					event.Trim(GetAt(at));
					end = at;
				}
				if(id == "tempo") {
					double t = ReadCNumber();
					if(t < 1 || t > 10000) {
						Error("invalid tempo value");
					}
					else
						tempo = t;
				}
			}
			else {
				while(*ptr && *ptr != ' ')
					snd.Cat(*ptr++);
				Sound(snd);
			}
		}
		else
		if(*ptr == '[') {
			ptr++;
			if(stack.GetCount() < 100)
				stack.Add() = *this;
			else
				Error("stack full");
		}
		else
		if(*ptr == ']') {
			ptr++;
			if(stack.GetCount())
				(QSFStatus&)*this = stack.Pop();
			else
				Error("stack empty");
		}
		else
		if(*ptr == '{') {
			ptr++;
			if(stack.GetCount() < 100) {
				stack.Add() = *this;
				at_stack.Add(at);
			}
			else
				Error("stack full");
		}
		else
		if(*ptr == '}') {
			ptr++;
			if(stack.GetCount() && at_stack.GetCount()) {
				(QSFStatus&)*this = stack.Pop();
				at = at_stack.Pop();
			}
			else
				Error("stack empty");
		}
		else
			ptr++;
	}
	int q = GetAt(end);
	if(q > 0)
		event.At(q);
}

SoundSequence ParseQSF(const String& data)
{
	StringStream ss(data);
	QSFParser p;
	p.Parse(ss);
	return pick(p);
}
