#include "IniConfig.hpp"

static bool __slash = false;

// Изменяет значение флага в соответствии с установленными правилами, указывает,
// что делать с текущим символом/буфером
void Rule(int& flags, int cur, bool& skip, bool& complete)
{
	complete = false;
	skip = false;

	bool string = flags / 1000 % 10 == 1;

	if (cur == '\\')
	{
		__slash = true;
		skip = true;
		return;
	}

	if (cur == '\"' && !__slash)
	{
		skip  = (!string ? true : complete = true);
		flags = (!string ? 1000 : 0) + flags % 1000;
		return;
	}
	else if (cur == '\"' && __slash)
		return;
	
	if (cur == '[' && __slash) return;
	if (cur == ']' && __slash) return;
	if (cur == ',' && __slash) return;
	if (cur == '=' && __slash) return;

	if (cur == ' '  && !string) { skip = true; return; }
	if (cur == '\t' && !string) { skip = true; return; }
	if (cur == '\n' &&  string) { skip = true; return; }

	if (flags == 10  && cur == '[' ) { flags = 100; skip = true; return; }
	if (flags == 100 && cur != ']' ) { return; }
	if (flags == 10  && cur != '=' ) { return; }
	if (flags == 0   && cur != '[' ) { skip = true; return; }
	if (flags != 1   && cur == '\n') { skip = true; return; }
	if (flags == 0   && cur == '[' ) { flags = 100; skip = true; }
	if (flags == 100 && cur == ']' ) { complete = true; skip = true; flags = 10; }
	if (flags == 10  && cur == '=' ) { complete = true; skip = true; flags = 1; }
	if (flags == 1   && cur == ',' ) { complete = true; skip = true; flags = 1; }
	if (flags == 1   && cur == '\n') { complete = true; flags = 10; }
}

// Загружает INI-структуру из файла. При возникновении ошибки возвращает false
// file - файл для загрузки
bool IniFile::Load(const String& file, int charset)
{
	FileIn in;
	if (!in.Open(file)) return false;
	bool r = Load(in, charset);
	in.Close();
	return r;
}

// Загружает INI-структуру из потока. При возникновении ошибки возвращает false
// s - поток для загрузки
bool IniFile::Load(Stream& s, int charset)
{
	Content.Clear();

	int sIndex = -1;
	int nIndex = -1;

	String buffer;
	bool complete;
	bool skip;
	int last;
	int flags = 0;
	String line;

	while(!s.IsEof())
	{
		line = ::ToCharset(CHARSET_DEFAULT, s.GetLine(), charset);
		line << '\n';
		
		for (int i = 0; i < line.GetCount(); ++i)
		{
			int c = line[i];

			bool lastSplash = __slash;
			last = flags % 1000;
			Rule(flags, c, skip, complete);

			if (!__slash)
				if (!skip)
					if (c != '\n' && c != '\r' && c != '\"') buffer << (char)c;
	
			if (lastSplash && __slash)
			{
				if (c == '\"') { buffer << '\"'; __slash = false; }
				else if (c == '[') { buffer << '['; __slash = false; }
				else if (c == ']') { buffer << ']'; __slash = false; }
				else if (c == '=') { buffer << '='; __slash = false; }
				else if (c == ',') { buffer << ','; __slash = false; }
				else { buffer << '\\'; __slash = false; }
			}

			if (complete && !buffer.IsEmpty())
			{
				if (last == 100)
				{
					sIndex++;
					nIndex = -1;
					Content.Add(IniConvert(buffer, false));
				}

				if (last == 10)
				{
					nIndex++;
					Content[sIndex].Content.Add(IniConvert(buffer, false));
				}

				if (last == 1)
					Content[sIndex].Content[nIndex].Content.Add(IniConvert(buffer, false));

				buffer.Clear();
			}
		}
	}

	return true;
}

// Возвращает массив с перечислением всех секций файла
Vector<String> IniFile::EnumSections()
{
	Vector<String> result;
	for (int i = 0; i < Content.GetCount(); ++i)
		result << Content[i].GetText();
	return result;
}

// Возвращает массив с перечислением всех ключей определенной секции файла
// s - секция для поиска ключей
Vector<String> IniFile::EnumNames(const String& s)
{
	Vector<String> result;
	for (int i = 0; i < Content.GetCount(); ++i)
		if (s == Content[i].GetText())
		{
			for (int j = 0; j < Content[i].Content.GetCount(); ++j)
				result << Content[i].Content[j].GetText();
		}
	return result;
}
