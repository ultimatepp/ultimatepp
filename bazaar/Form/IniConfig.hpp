#ifndef INI_CONFIG_HPP
#define INI_CONFIG_HPP

#include <Core/Core.h>
using namespace Upp;

static int IniSaveCharset = CHARSET_DEFAULT;

inline void ReplaceString(String& text, const String& fstr, const String& istr)
{
	if (!text.GetCount()) return;
	if (!fstr.GetCount()) return;

	int pos = 0;

	while (pos >= 0)
	{
		pos = text.Find(fstr, pos);
		if (pos >= 0)
		{
			text.Remove(pos, fstr.GetCount());
			if (!istr.IsEmpty())
				text.Insert(pos, istr);
			pos += istr.GetCount();
		}
	}
}

//! Простой фильтр символов для строк.
//! \brief Простой фильтр символов для строк.
//! Пример:
//! 1) "- +/" - удалит из строки все пробелы и удвоит слэш;
//! 2) "-+" удалит все знаки + из строки
//! 3) "--" удалит все знаки - из строки
//! 4) "---+" удалит все знаки + и - из строки
//! \param text текст для обработки
//! \param filter строка символов фильтра
inline String SetFilter(const String& text, const String& filter)
{
	Vector<char> f[2];
	String result;
	bool flag;
	bool del = false;

	for (int cur = -1, i = 0; i < filter.GetCount(); ++i)
		switch(filter[i])
		{
			case '+': if (del) { f[1] << filter[i]; del = false; } cur = 1; break;
			case '-': if (del) { f[1] << filter[i]; del = false; }
				else del = true; cur = 2; break;

			default:
				if (cur == -1)
					continue;
				cur == 1
					? f[0] << filter[i]
					: f[1] << filter[i];
		}

	for (int i = 0; i < text.GetCount(); ++i)
	{
		for (int j = 0; j < f[1].GetCount(); ++j)
		{
			flag = (char)text[i] == f[1][j];
			if (flag) break;
		}
		if (!flag)
			for (int j = 0; j < f[0].GetCount(); ++j)
			{
				if ((char)text[i] == f[0][j])
				{
					result << (char)text[i];
					flag = false;
				}
			}
		if (!flag) result << (char)text[i];
	}

	return result;
}

//! Младший элемент INI-дерева, хранит текстовое поле, и предоставляет основные ф-и
//! для работы с ним
struct IniText
{
	//! Основной конструктор
	IniText() {}

	//! Расширенный конструктор
	//! \param text текст для элемента
	//! \param str флаг строки
	//! \param manual флаг оптимизации, при ложном значении отключена фильтрация
	//! и ф-я работает быстрее
	IniText(const String& text, bool str, bool manual = true) { SetText(text, str, manual); }

	//! Устанавливает значение текстового поля элемента
	//! \param text текст для элемента
	//! \param str флаг строки
	//! \param manual флаг оптимизации, при ложном значении отключена фильтрация
	//! и ф-я работает быстрее
	void SetText(String text, bool str, bool manual = true)
	{
		if (text.Find('\\') >= 0) ::ReplaceString(text, "\\", "\\\\");
		if (text.Find('\"') >= 0) ::ReplaceString(text, "\"", "\\\"");
		if (text.Find('[' ) >= 0) ::ReplaceString(text, "[", "\\[");
		if (text.Find(']' ) >= 0) ::ReplaceString(text, "]", "\\]");
		if (text.Find('=' ) >= 0) ::ReplaceString(text, "=", "\\=");
		if (text.Find(',' ) >= 0) ::ReplaceString(text, ",", "\\,");

		Text = Format(str ? "\"%s\"" : "%s",
			manual ? SetFilter(text, str ? "-\r\n\t" : "- \r\n\t") : text);
	}

	//! Сохраняет элемент в поток с форматированием
	//! \param s поток данных
	//! \param f формат вывода в поток (%s - значение элемента)
	void Serialize(Stream& s, const String& f = "%s")
	{
		s << ::ToCharset(IniSaveCharset, Format(f, Text), CHARSET_DEFAULT);
	}

	String GetText()
	{
		if (Text.GetCount() == 1 && Text[0] == '\"')
			return "";
		else if (Text.GetCount())
		{
			String t = Text;

			if (Text[0] == '\"' && Text[Text.GetCount() - 1] == '\"')
				t = Text.Mid(1, Text.GetCount()-2);

			if (t.Find("\\\"") >= 0) ::ReplaceString(t, "\\\"", "\"");
			if (t.Find("\\[" ) >= 0) ::ReplaceString(t, "\\[", "[");
			if (t.Find("\\]" ) >= 0) ::ReplaceString(t, "\\]", "]");
			if (t.Find("\\=" ) >= 0) ::ReplaceString(t, "\\=", "=");
			if (t.Find("\\," ) >= 0) ::ReplaceString(t, "\\,", ",");
			if (t.Find("\\\\") >= 0) ::ReplaceString(t, "\\\\", "\\");
			return t;
		}
		return Text;
	}

	//! Текстовое поле элемента, значение поля
	String Text;
};

//! Элемент INI-дерева, значение в структуре INI-файла
//! \brief Элемент INI-дерева, значение в структуре INI-файла.
//! Например, в строке "[Section] Name = Value" значением является Value.
struct IniValue : IniText, Moveable<IniValue>
{
	//! Основной конструктор
	IniValue() {}

	//! Конструктор из другого IniValue-значения
	IniValue(const IniValue& other) { Text = other.Text; }

	//! Расширенный конструктор
	//! \param text текст для элемента
	//! \param str флаг строки
	//! \param manual флаг оптимизации, при ложном значении отключена фильтрация
	//! и ф-я работает быстрее
	IniValue(const String& text, bool str, bool manual = true) : IniText(text, str, manual) {}
};

//! Элемент INI-дерева, ключ в структуре INI-файла, который содержит одно/несколько значений
//! \brief Элемент INI-дерева, ключ в структуре INI-файла, который содержит одно/несколько
//! значений. Например, в строке "[Section] Name = Value" ключом является Name.
struct IniName : IniText, Moveable<IniName>
{
	//! Основной конструктор
	IniName() {}

	//! Конструктор из другого IniName-значения
	IniName(const IniName& other) { Text = other.Text; Content <<= other.Content; }

	//! Расширенный конструктор
	//! \param text текст для элемента
	//! \param str флаг строки
	//! \param manual флаг оптимизации, при ложном значении отключена фильтрация
	//! и ф-я работает быстрее
	IniName(const String& text, bool str, bool manual = true) : IniText(text, str, manual) {}

	//! Устанавливает значение ключа, предыдущие стирает
	//! \param v значение ключа
	void Set(const IniValue& v) { Content.Clear(); Content << v; }

	//! Добавляет значение к имеющимся в ключе
	//! \param v значение ключа
	void Add(const IniValue& v) { Content << v; }

	//! Массив значений, которым назначен ключ
	Vector<IniValue> Content;
};

//! Элемент INI-дерева, является секцией в структуре INI-файла, содержит ключи и значения
//! \brief Элемент INI-дерева, является секцией в структуре INI-файла, содержит ключи и
//! значения. Например, в строке "[Section] Name = Value" секцией является Section.
struct IniSection : IniText, Moveable<IniSection>
{
	//! Основной конструктор
	IniSection() {}

	//! Конструктор из другого IniSection-значения
	IniSection(const IniSection& other) { Text = other.Text; Content <<= other.Content; }

	//! Расширенный конструктор
	//! \param text текст для элемента
	//! \param str флаг строки
	//! \param manual флаг оптимизации, при ложном значении отключена фильтрация
	//! и ф-я работает быстрее
	IniSection(const String& text, bool str, bool manual = true) : IniText(text, str, manual) {}

	//! Сохраняет секцию в поток с форматированием каждого элемента
	//! \param s поток данных
	void Serialize(Stream& s)
	{
		if (s.IsStoring())
		{
			IniText::Serialize(s, "[%s]\r\n");
			for (int i = 0; i < Content.GetCount(); ++i)
			{
				Content[i].Serialize(s, "%s = ");
				
				for (int j = 0; j < Content[i].Content.GetCount(); ++j)
				{
					Content[i].Content[j].
					Serialize(s, j == Content[i].Content.GetCount() - 1 ? "%s\r\n" : "%s, ");
				}
			}
		}
	}

	//! Возвращает индекс ключа в массиве. Если ключ не найден, возвращает -1
	//! \param name название ключа для поиска
	int FindName(const String& name)
	{
		for (int i = 0; i < Content.GetCount(); ++i)
			if (Content[i].Text == name) return i;
		return -1;
	}

	//! Устанавливает значение ключа (если ключ не существует, он будет создан),
	//! предыдущие стирает
	//! \param n ключ для изменения
	//! \param v значение ключа
	void Set(const IniName& n, const IniValue& v)
	{
		if (n.Text.IsEmpty()) return;
		int i = FindName(n.Text);
		if (i < 0) { i = Content.GetCount(); Content << n; }
		if (v.Text.IsEmpty()) return;
		Content[i].Set(v);
	}

	//! Добавляет значение к имеющимся в ключе
	//! \param v значение ключа

	//! Добавляет значение к имеющимся в ключе (если ключ не существует, он будет создан)
	//! \param n ключ для изменения
	//! \param v значение ключа
	void Add(const IniName& n, const IniValue& v)
	{
		if (n.Text.IsEmpty()) return;
		int i = FindName(n.Text);
		if (i < 0) { i = Content.GetCount(); Content << n; }
		if (v.Text.IsEmpty()) return;
		Content[i].Add(v);
	}

	//! Массив ключей со значениям
	Vector<IniName> Content;
};

//! Конверт для строки, из которого может быть создан INI-элемент любого типа
class IniConvert : IniText
{
public:
	//! Контруктор из строкового значения
	//! \param text строка для преобразования
	//! \param manual флаг оптимизации, при ложном значении отключена фильтрация
	//! и преобразование конверта работает быстрее
	IniConvert(const String& text, bool manual) { Text = text; Manual = manual; }

	//! Контруктор из целого числа
	//! \param int целое число для преобразования
	//! \param manual флаг оптимизации, при ложном значении отключена фильтрация
	//! и преобразование конверта работает быстрее
	IniConvert(int num, bool manual) { Text = AsString(num); Manual = manual; }

	//! Контруктор из логического значения
	//! \param int логическое значения для преобразования
	//! \param manual флаг оптимизации, при ложном значении отключена фильтрация
	//! и преобразование конверта работает быстрее
	IniConvert(bool flag, bool manual) { Text = flag ? "1" : "0"; Manual = manual; }

	//! Контруктор из числа с плавающей точкой
	//! \param int число с плавающей точкой для преобразования
	//! \param manual флаг оптимизации, при ложном значении отключена фильтрация
	//! и преобразование конверта работает быстрее
	IniConvert(double num, bool manual) { Text = FormatDouble(num); Manual = manual; }

	//! Оператор преобразования значения в IniName-элемент
	operator IniName() const { return IniName(Text, Text.Find(' ') >= 0, Manual); }

	//! Оператор преобразования значения в IniValue-элемент
	operator IniValue() const { return IniValue(Text, Text.Find(' ') >= 0, Manual); }

	//! Оператор преобразования значения в IniSection-элемент
	operator IniSection() const { return IniSection(Text, Text.Find(' ') >= 0, Manual); }

private:
	//! Флаг оптимизации, при ложном значении отключена фильтрация значения и преобразование
	//! конверта работает быстрее
	bool Manual;
};

//! Простой класс для работы с INI-файлами
//! \brief Простой класс для работы с INI-файлами. Если строка не заключена в кавычки \",
//! некоторые символы будут пропущены (например, пробелы). Можно записывать несколько значений
//! после имени ключа (через запятую).
//! Пример INI-файла:
//! \code
//! [General]
//! Title = "Application Title"
//! Language = Ru-Ru
//!
//! ["Section 1"]                       // Section 1
//! Item 0 = 0,0, Name 0, Path0         // Item0 = 0,0,Name0,Path0   - без кавычек
//! "Item 1" = 0,1, "Name 1", "Path 1"  // Item 1 = 0,1,Name 1,Path 1 - при использовании кавычек
//! \endcode
//! Пример работы с классом:
//! \code
//! IniFile p;
//! p.Load("C:\\Test.ini");
//! p.Add("Test", "String", "Value");
//! p.Add("Test", "Double", 100.01);
//! p.Add("Test", "Number", 1);
//! p.Add("Test", "Boolean", true); // true хранится в файле как 1, false - как 0
//! p.Save("C:\\Test.ini");
//! \endcode
class IniFile
{
public:
	//! Устанавливает строковое значение ключа в секции (если секция или ключ не существуют,
	//! они будут созданы), предыдущие стирает
	//! \param s секция для поиска ключа
	//! \param n ключ в секции для изменения
	//! \param v строковое значение ключа
	//! \param manual флаг оптимизации, при ложном значении отключена фильтрация
	//! и ф-я работает быстрее
	void Set(const String& s, const String& n, const String& v, bool manual = true)
		{ Set(IniConvert(s, manual), IniConvert(n, manual), IniConvert(v, manual)); }

	//! Записывает массив значений в ключ секции (если секция или ключ не существуют,
	//! они будут созданы), предыдущие значения стирает
	//! \param s секция для поиска ключа
	//! \param n ключ в секции для изменения
	//! \param a массив значений для записи
	//! \param manual флаг оптимизации, при ложном значении отключена фильтрация
	//! и ф-я работает быстрее
	void SetArray(const String& s, const String& n, const Array<String>& a, bool manual = true)
	{
		for (int i = 0; i < a.GetCount(); ++i)
			Set(IniConvert(s, manual), IniConvert(n, manual), IniConvert(a[i], manual));
	}

	//! Устанавливает целочисленное значение ключа в секции (если секция или ключ не существуют,
	//! они будут созданы), предыдущие стирает
	//! \param s секция для поиска ключа
	//! \param n ключ в секции для изменения
	//! \param v целочисленное значение ключа
	//! \param manual флаг оптимизации, при ложном значении отключена фильтрация
	//! и ф-я работает быстрее
	void SetInt(const String& s, const String& n, int v, bool manual = true)
		{ Set(IniConvert(s, manual), IniConvert(n, manual), IniConvert(v, manual)); }

	//! Устанавливает число с плавающей точкой как значение ключа в секции (если секция или
	//! ключ не существуют, они будут созданы), предыдущие значения стирает
	//! \param s секция для поиска ключа
	//! \param n ключ в секции для изменения
	//! \param v число с плавающей точкой, значение ключа
	//! \param manual флаг оптимизации, при ложном значении отключена фильтрация
	//! и ф-я работает быстрее
	void SetDouble(const String& s, const String& n, double v, bool manual = true)
		{ Set(IniConvert(s, manual), IniConvert(n, manual), IniConvert(v, manual)); }

	//! Устанавливает значение ключа в секции (если секция или ключ не существуют,
	//! они будут созданы), предыдущие стирает
	//! \param s секция для поиска ключа
	//! \param n ключ в секции для изменения
	//! \param v значение ключа
	void Set(const IniSection& s, const IniName& n, const IniValue& v)
	{
		if (s.Text.IsEmpty()) return;
		int i = FindSection(s.Text);
		if (i < 0) { i = Content.GetCount(); Content << s; }
		if (n.Text.IsEmpty()) return;
		Content[i].Set(n, v);
	}

	//! Добавляет строковое значение к имеющимся в ключе секции (если секция или ключ
	//! не существуют, они будут созданы)
	//! \param s секция для поиска
	//! \param n ключ для изменения
	//! \param v строковое значение ключа
	//! \param manual флаг оптимизации, при ложном значении отключена фильтрация
	//! и ф-я работает быстрее
	void Add(const String& s, const String& n, const String& v, bool manual = true)
		{ Add(IniConvert(s, manual), IniConvert(n, manual), IniConvert(v, manual)); }

	//! Дописывает массив значений в ключ секции (если секция или ключ не существуют,
	//! они будут созданы)
	//! \param s секция для поиска ключа
	//! \param n ключ в секции для изменения
	//! \param a массив значений для записи
	//! \param manual флаг оптимизации, при ложном значении отключена фильтрация
	//! и ф-я работает быстрее
	void AddArray(const String& s, const String& n, const Array<String>& a, bool manual = true)
	{
		for (int i = 0; i < a.GetCount(); ++i)
			Add(IniConvert(s, manual), IniConvert(n, manual), IniConvert(a[i], manual));
	}

	//! Добавляет целочисленное значение к имеющимся в ключе секции (если секция или ключ
	//! не существуют, они будут созданы)
	//! \param s секция для поиска
	//! \param n ключ для изменения
	//! \param v целочисленное значение ключа
	//! \param manual флаг оптимизации, при ложном значении отключена фильтрация
	//! и ф-я работает быстрее
	void AddInt(const String& s, const String& n, int v, bool manual = true)
		{ Add(IniConvert(s, manual), IniConvert(n, manual), IniConvert(v, manual)); }

	//! Добавляет число с плавающей точкой как значение к имеющимся в ключе секции
	//! (если секция или ключ не существуют, они будут созданы)
	//! \param s секция для поиска
	//! \param n ключ для изменения
	//! \param v логическое значение ключа
	//! \param manual флаг оптимизации, при ложном значении отключена фильтрация
	//! и ф-я работает быстрее
	void AddDouble(const String& s, const String& n, double v, bool manual = true)
		{ Add(IniConvert(s, manual), IniConvert(n, manual), IniConvert(v, manual)); }

	//! Добавляет значение к имеющимся в ключе секции (если секция или ключ не существуют,
	//! они будут созданы)
	//! \param s секция для поиска ключа
	//! \param n ключ для изменения
	//! \param v число с плавающей точкой, значение ключа
	void Add(const IniSection& s, const IniName& n, const IniValue& v)
	{
		if (s.Text.IsEmpty()) return;
		int i = FindSection(s.Text);
		if (i < 0) { i = Content.GetCount(); Content << s; }
		if (n.Text.IsEmpty()) return;
		Content[i].Add(n, v);
	}

	//! Возвращает строковое значение ключа в секции
	//! \param s секция для поиска ключа
	//! \param n ключ для поиска значения
	String Get(const String& s, const String& n)
	{
		Vector<String> values = GetArray(s, n);
		return values.GetCount() ? values[0] : "";
	}

	//! Возвращает массив значений ключа в секции
	//! \param s секция для поиска ключа
	//! \param n ключ для поиска значения
	Vector<String> GetArray(const String& s, const String& n)
	{
		int i = FindSection(s);
		if (i < 0) return Vector<String>();

		int j = Content[i].FindName(n);
		if (j < 0) return Vector<String>();

		Vector<String> result;

		for (int v = 0; v < Content[i].Content[j].Content.GetCount(); ++v)
			result << Content[i].Content[j].Content[v].GetText();

		return result;
	}

	//! Возвращает целочисленное значение ключа в секции
	//! \param s секция для поиска ключа
	//! \param n ключ для поиска значения
	int GetInt(const String& s, const String& n)
	{
		Vector<String> values = GetArray(s, n);
		return values.GetCount() ? ScanInt(values[0]) : 0;
	}

	//! Возвращает значение ключа секции в виде числа с плавающей точкой
	//! \param s секция для поиска ключа
	//! \param n ключ для поиска значения
	double GetDouble(const String& s, const String& n)
	{
		Vector<String> values = GetArray(s, n);
		return values.GetCount() ? ScanDouble(values[0]) : 0.0;
	}

	//! Возвращает массив с перечислением всех секций файла
	Vector<String> EnumSections();

	//! Возвращает массив с перечислением всех ключей определенной секции файла
	//! \param s секция для поиска ключей
	Vector<String> EnumNames(const String& s);

	//! Загружает INI-структуру из файла. При возникновении ошибки возвращает false
	//! \param file файл для загрузки
	virtual bool Load(const String& file, int charset = CHARSET_DEFAULT);

	//! Загружает INI-структуру из потока. При возникновении ошибки возвращает false
	//! \param s поток для загрузки
	virtual bool Load(Stream& s, int charset = CHARSET_DEFAULT);

	//! Сохраняет INI-структуру в файл. При возникновении ошибки возвращает false
	//! \param file файл для сохранения
	virtual bool Save(const String& file, int charset = CHARSET_DEFAULT)
	{
		IniSaveCharset = charset;

		FileOut out;
		if (!out.Open(file))
			return false;
		bool r = Save(out);
		out.Close();
		return r;
	}

	//! Сохраняет INI-структуру в поток. При возникновении ошибки возвращает false
	//! \param s поток для сохранения
	virtual bool Save(Stream& s)
	{
		if (!s.IsOpen())
			return false;
		Serialize(s);
		return true;
	}

	//! Возвращает INI-структуру в виде строки
	String Dump()
	{
		StringStream out; out.Create();
		Serialize(out);
		out.Close();
		return out;
	}

	//! Удаляет все элементы в INI-структуре
	void Clear() { Content.Clear(); }

protected:
	//! Сохраняет файл в поток
	//! \param s поток данных
	void Serialize(Stream& s)
	{
		if (s.IsStoring())
			for (int i = 0; i < Content.GetCount(); ++i)
			{
				Content[i].Serialize(s);
				if (i < Content.GetCount() - 1) s << "\r\n";
			}
	}

	//! Возвращает индекс секции в массиве. Если секция не найдена, возвращает -1
	//! \param section название секции для поиска
	int FindSection(const String& section)
	{
		for (int i = 0; i < Content.GetCount(); ++i)
			if (Content[i].Text == section) return i;
		return -1;
	}

	//! Массив секций с ключами и значениями, INI-структура
	Vector<IniSection> Content;
};

#endif // .. INI_CONFIG_HPP
