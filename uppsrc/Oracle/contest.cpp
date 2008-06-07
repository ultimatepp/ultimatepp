#ifdef flagMAIN

#include <Oracle/Oracle8.h>

using namespace Upp;

void Main()
{
	String buffer;
	String prompt = "?";
	Oracle8 ora8;
	Vector<String> cmd;
	cmd <<= CommandLine();
	if(!cmd.IsEmpty())
	{
		if(ora8.Open(cmd[0]))
		{
			puts("Connected.");
			prompt = ora8.GetUser();
		}
		else
		{
			puts("Error connecting to database: " + ora8.GetLastError());
			return;
		}
	}
	for(;;)
	{
		fputs(prompt + '>', stdout);
		fflush(stdout);
		char lbuf[1000];
		*lbuf = 0;
		fgets(lbuf, 1000, stdin);
		if(!*lbuf && feof(stdin))
			return;
		char *e = lbuf + strlen(lbuf);
		while(e > lbuf && (byte)e[-1] <= ' ')
			e--;
		*e = 0;
		if(!stricmp(lbuf, "quit") || !stricmp(lbuf, "exit") || !stricmp(lbuf, "."))
			return;
		buffer.Cat(lbuf, e - lbuf);
		if(buffer.GetLength() >= 8 && !memicmp(buffer, "connect ", 8))
		{
			ora8.Logoff();
			if(ora8.Open(buffer.Mid(8)))
			{
				prompt = ora8.GetUser();
				puts("Connected.");
			}
			else
			{
				puts("Error connecting to database: " + ora8.GetLastError());
				prompt = "?";
			}
			buffer = Null;
			continue;
		}
		if(e > lbuf && e[-1] != ';')
		{
			puts(NFormat("Accumulated query: <%s>", buffer));
			continue;
		}
		String cmd = buffer;
		buffer = Null;
		if(!ora8.IsOpen())
		{
			puts("Not connected.");
			continue;
		}
		if(*cmd.Last() == ';')
			cmd.Trim(cmd.GetLength() - 1);
		Sql cursor(ora8);
		if(cursor.Execute(cmd))
		{
			for(int c = 0; c < cursor.GetColumns(); c++)
			{
				SqlColumnInfo ci = cursor.GetColumnInfo(c);
				fputs(ci.name, stdout);
				putc(';', stdout);
			}
			putc('\n', stdout);
			while(cursor.Fetch())
			{
				for(int c = 0; c < cursor.GetColumns(); c++)
				{
					fputs(SqlFormat(cursor[c]), stdout);
					putc(';', stdout);
				}
				putc('\n', stdout);
			}
		}
		else
			puts(NFormat("Error executing <%s>: %s", cmd, cursor.GetLastError()));
	}
}

#endif//flagMAIN
