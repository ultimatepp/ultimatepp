#include "ArgEnv.h"

NAMESPACE_UPP

#ifdef PLATFORM_WIN32
static void fixSpecials(String &s)
{
	// this stuff just for windows... sig
	if(s.FindFirstOf(" \t\n\v\"") >= 0)
	{
		String sOld = s;
		s.Clear();
		s = "\"";

		const char *sp = sOld;
		for(;;) {
			int num_backslashes = 0;
			while(*sp == '\\') {
				sp++;
				num_backslashes++;
			}
			if(*sp == '\0') {
				s.Cat('\\', 2 * num_backslashes);
				break;
			}
			else
			if(*sp == '\"') {
				s.Cat('\\', 2 * num_backslashes + 1);
				s << '\"';
			}
			else {
				s.Cat('\\', num_backslashes);
				s.Cat(*sp);
			}
			sp++;
		}
		s << '\"';
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////
// parses an args line to be useable by spawnxx functions
char **BuildArgs(String command, String const &argline)
{
	Array<String> args;
	
	// first arg should be command name
#ifdef PLATFORM_WIN32
		fixSpecials(command);
#endif
	args.Add(command);

	// skips leading spaces
	char c;
	int pos = 0;
	while ((c = argline[pos]) != 0 && isspace(c))
		pos++;

	// loop reading args and putting to array
	while (c)
	{
		String &s = args.Add();
		while (c && !isspace(c))
		{
			// reads enquoted strings
			if (c == '"')
			{
				c = argline[++pos];
				while (c && c != '"')
				{
					s << c;
					c = argline[++pos];
				}
				if (c)
					c = argline[++pos];
			}
			// read single quoted string
			else if(c == '\'')
			{
				c = argline[++pos];
				while (c && c != '\'')
				{
					s << c;
					c = argline[++pos];
				}
				if (c)
					c = argline[++pos];
			}
			else
			{
				s << c;
				c = argline[++pos];
			}
		}

#ifdef PLATFORM_WIN32
		fixSpecials(s);
#endif
		// skips trailing spaces
		while (c && isspace(c))
			c = argline[++pos];
	}
	
	// calculates buffer size (as byte)
	int buflen = 0;
	for(int i = 0; i < args.GetCount(); i++)
		buflen += args[i].GetCount() + 1;
	buflen += (args.GetCount() + 1) * sizeof(char *);
	
	// here we've got an array of args and the total size (in bytes) of them
	// we allocates a  buffer for arg array
	char **buf = (char **)malloc(buflen);

	// we fill the buffer with arg strings
	char **bufindex = buf;
	char *bufpos = (char *)(buf + args.GetCount() + 1);
	int i = 0;
	while (i < args.GetCount())
	{
		String s = args[i];
		strcpy(bufpos, ~s);
		*bufindex++ = bufpos;
		bufpos += s.GetCount() + 1 ;
		i++;
	}
	*bufindex = 0;

	// returns array of args
	return buf;

} // END _BuildArgs()


///////////////////////////////////////////////////////////////////////////////////////////////
// parses environment map and builds env array
char **BuildEnv(const VectorMap<String, String> &env)
{
	// calculates total environment size
	int envSize = 0;
	for (int i = 0; i < env.GetCount(); i++)
		envSize += env.GetKey(i).GetCount() + env[i].GetCount() + 2 + sizeof(char *);
	envSize+=sizeof(char *);

	// we allocates a  buffer for env array
	char **buf = (char **)malloc(envSize);

	// we fill the buffer with env strings
	char **bufindex = buf;
	char *bufpos = (char *)(buf + env.GetCount() + 1);
	int i = 0;
	while (i < env.GetCount())
	{
		String s = env.GetKey(i) + "=" + env[i];
		strcpy(bufpos, ~s);
		*bufindex++ = bufpos;
		bufpos += s.GetCount() + 1 ;
		i++;
	}
	*bufindex = 0;

	// returns array of args
	return buf;

} // END _BuildEnv()

END_UPP_NAMESPACE
