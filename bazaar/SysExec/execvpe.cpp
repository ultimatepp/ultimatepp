///////////////////////////////////////////////////////////////////////////////////////////////
// this one is let here just for reference -- superseded by linux builtin call
#if 0

#define DEFAULT_PATH 	"/bin:/usr/bin:."
static int execvpe(const char *file, char * const *argv, char * const *envp)
{
	char path[PATH_MAX];
	const char *searchpath, *esp;
	size_t prefixlen, filelen, totallen;

	if (strchr(file, '/'))	/* Specific path */
		return execve(file, argv, envp);

	filelen = strlen(file);

	searchpath = getenv("PATH");
	if (!searchpath)
		searchpath = DEFAULT_PATH;

	errno = ENOENT; /* Default errno, if execve() doesn't change it */

	do
	{
		esp = strchr(searchpath, ':');
		if (esp)
			prefixlen = esp - searchpath;
		else
			prefixlen = strlen(searchpath);

		if (prefixlen == 0 || searchpath[prefixlen-1] == '/')
		{
			totallen = prefixlen + filelen;
			if (totallen >= PATH_MAX)
				continue;
			memcpy(path, searchpath, prefixlen);
			memcpy(path + prefixlen, file, filelen);
		}
		else
		{
			totallen = prefixlen + filelen + 1;
			if (totallen >= PATH_MAX)
				continue;
			memcpy(path, searchpath, prefixlen);
			path[prefixlen] = '/';
			memcpy(path + prefixlen + 1, file, filelen);
		}
		path[totallen] = '\0';

		execve(path, argv, envp);
		if (errno == E2BIG || errno == ENOEXEC ||
			errno == ENOMEM || errno == ETXTBSY)
			break;			/* Report this as an error, no more search */

		searchpath = esp + 1;
	}
	while (esp);

	return -1;
}
#endif

