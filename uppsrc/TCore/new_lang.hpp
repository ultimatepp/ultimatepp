/*
String FormatTime(Time time, const char *f, int language)
{
	if(IsNull(time))
		return Null;
	String out;
	const LanguageInfo& lang = GetLanguageInfo(language);
	for(char c; c = *f++;)
		switch(c)
		{
		case '`':
			if(*f)
				out.Cat(*f++);
			break;

		case 'H':
			if(*f == 'H')
				f++;
			if(*f == '2' && f[1] == '4')
				f += 2, out << (int)time.hour;
			else
			{
				if(*f == '1' && f[1] == '2')
					f += 2;
				out << ((time.hour + 11) % 12 + 1);
			}
			break;

		case 'D':
			if(*f == 'D')
			{
				if(*++f == 'D')
					f++, out << (int)(Date(time) - Date(time.year, 1, 1) + 1);
				else
					out << (int)time.day;
			}
			else if(*f == 'A' && f[1] == 'Y')
				f += 2, out << lang.day_names[DayOfWeek(time)];
			else if(*f == 'Y')
				f++, out << lang.short_day_names[DayOfWeek(time)];
			else
				out << (DayOfWeek(time) + 1);
			break;

		case 'M':
			if(*f == 'I')
				f++, out << (int)time.minute;
			else if(*f == 'O' && f[1] == 'N')
			{
				f += 3;
				if(*f == 'T' && f[1] == 'H')
					f += 2, out << lang.month_names[time.month - 1];
				else
					out << lang.short_month_names[time.month - 1];
			}
			else if(*f == 'M')
				f++, out << FormatIntDec(time.month, 2, '0');
			else
				out << (int)time.month;
			break;

		case 'Y':
			out << tabs<int>(time.year);
			break;

		case 'S':
			if(*f == 'S')
			{
				f++;
				if(*f == 'S' && f[1] == 'S' && f[2] == 'S')
					f += 3, out << (int)(time.second + 60 * time.minute + 3600 * time.hour);
				else
					out << (int)time.second;
			}
			else if(time.year < 0)
				out << '-';
			break;

		default:
			out.Cat(c);
		}
	return out;
}
*/

//////////////////////////////////////////////////////////////////////

/*
String FormatDate(Date date, const char *format, int language)
{
	return FormatTime(ToTime(date), format, language);
}
*/

//////////////////////////////////////////////////////////////////////

/*
Time ScanTime(const char *text, const char **endptr, const char *f, int language, Time out)
{
	return Null;
	if(!f)
	{
		if(endptr)
			*endptr = text;
		return out;
	}
	const LanguageInfo& lang = GetLanguageInfo(language);
	for(char c; c = *f++;)
		switch(c)
		{
		case '`':
			if(*f && *text != *f++)
			{
				if(endptr)
					*endptr = text;
				return Null;
			}
			break;

		case 'H':
			if(*f == 'H')
				f++;
			if(*f == '2' && f[1] == '4' || *f == '1' && f[1] == '2')
				f += 2;
			if(!IsDigit(*text))
			{
				if(endptr)
					*endptr = text;
				return Null;
			}
			out.hour = stou(text, &text);
			if(out.hour > 23)
			{
				if(endptr)
					*endptr = text;
				return Null;
			}
			break;

		case 'D':
			if(*f == 'D')
			{
				if(*++f == 'D')
					f++, out << (int)(Date(time) - Date(time.year, 1, 1) + 1);
				else
					out << (int)time.day;
			}
			else if(*f == 'A' && f[1] == 'Y')
				f += 2, out << lang.day_names[DayOfWeek(time)];
			else if(*f == 'Y')
				f++, out << lang.short_day_names[DayOfWeek(time)];
			else
				out << (DayOfWeek(time) + 1);
			break;

		case 'M':
			if(*f == 'I')
				f++, out << (int)time.minute;
			else if(*f == 'O' && f[1] == 'N')
			{
				f += 3;
				if(*f == 'T' && f[1] == 'H')
					f += 2, out << lang.month_names[time.month - 1];
				else
					out << lang.short_month_names[time.month - 1];
			}
			else if(*f == 'M')
				f++, out << FormatIntDec(time.month, 2, '0');
			else
				out << (int)time.month;
			break;

		case 'Y':
			out << tabs<int>(time.year);
			break;

		case 'S':
			if(*f == 'S')
			{
				f++;
				if(*f == 'S' && f[1] == 'S' && f[2] == 'S')
					f += 3, out << (int)(time.second + 60 * time.minute + 3600 * time.hour);
				else
					out << (int)time.second;
			}
			else if(time.year < 0)
				out << '-';
			break;

		default:
			out.Cat(c);
		}
	if(endptr)
		*endptr = text;
	return out;
}
*/

//////////////////////////////////////////////////////////////////////

/*
Date ScanDate(const char *text, const char **endptr, const char *format, int language, Date base_date)
{
	return ScanTime(text, endptr, format, language, ToTime(base_date));
}
*/
