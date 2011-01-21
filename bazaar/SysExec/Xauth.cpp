static String GetXauthToken(String const &disp)
{
	String bin;

	// find xauth binary executable
	if(FileExists("/usr/bin/xauth"))
		bin = "/usr/bin/xauth";
	else if(FileExists("/usr/X11R6/bin/xauth"))
		bin = "/usr/X11R6/bin/xauth";
	else
		return "";
	
	// executes it and get token from its output
	FILE *xauthOutput = popen(FormatString("%s list %s | head -1 | awk '{ print $3 }'", bin, disp), "r");
	if(!xauthOutput)
		return "<NULL>";
	Buffer<char *>buf(256);
	fread(buf, sizeof(char), 255, xauthOutput);
	pclose(xauthOutput);
	
	// return token
	return buf;
}
	
static bool PrepareXauth(String &disp, String &token)
{
	disp = Environment().Get("DISPLAY", "");
	token = GetXauthToken(disp);
	if(token == "<NULL>")
		return false;
	// if empty token, try to get it stripping the hostname
	// part from DISPLAY environment var
	if(token == "")
	{
		int i = disp.ReverseFind(':');
		if(i >= 0)
		{
			disp = disp.Mid(i);
			token = GetXauthToken(disp);
		}
	}
	return true;
}



static gboolean sudo_prepare_xauth (GksuContext *context)
{
	gchar template[] = "/tmp/" PACKAGE "-XXXXXX";
	gboolean error_copying = FALSE;
	gchar *xauth = NULL;

	context->dir = g_strdup (mkdtemp(template));

	if (!context->dir)
	{
		fprintf (stderr, strerror(errno));
		return FALSE;
	}

	xauth = g_strdup(g_getenv ("XAUTHORITY"));

	if (xauth == NULL)
		xauth = g_strdup_printf ("%s/.Xauthority", g_get_home_dir());

	error_copying = !copy (xauth, context->dir);

	g_free (xauth);

	if (error_copying)
		return FALSE;

	return TRUE;
}




static gboolean copy (const char *fn, const char *dir)
{
	int in, out;
	int r;
	char *newfn;
	char buf[BUFSIZ] = "";

	newfn = g_strdup_printf("%s/.Xauthority", dir);

	out = open(newfn, O_WRONLY | O_CREAT | O_EXCL, 0600);

	if (out == -1)
	{
		if (errno == EEXIST)
			fprintf (stderr,
					 "Impossible to create the .Xauthority file: a file "
					 "already exists. This might be a security issue; "
					 "please investigate.");
		else
			fprintf (stderr,
					 "Error copying '%s' to '%s': %s",
					 fn, dir, strerror(errno));

		return FALSE;
	}

	in = open(fn, O_RDONLY);

	if (in == -1)
	{
		fprintf (stderr,
				 "Error copying '%s' to '%s': %s",
				 fn, dir, strerror(errno));
		return FALSE;
	}

	while ((r = read(in, buf, BUFSIZ)) > 0)
	{
		if (full_write(out, buf, r) == -1)
		{
			fprintf (stderr,
					 "Error copying '%s' to '%s': %s",
					 fn, dir, strerror(errno));
			return FALSE;
		}
	}

	if (r == -1)
	{
		fprintf (stderr,
				 "Error copying '%s' to '%s': %s",
				 fn, dir, strerror(errno));
		return FALSE;
	}

	return TRUE;
}



