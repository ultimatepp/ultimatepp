gboolean
gksu_sudo_fuller (GksuContext *context,
				  GksuAskPassFunc ask_pass,
				  gpointer ask_pass_data,
				  GksuPassNotNeededFunc pass_not_needed,
				  gpointer pass_not_needed_data,
				  gint8 *exit_status,
				  GError **error)
{
	char **cmd;
	char buffer[256] = {0};
	char *child_stderr = NULL;
	/* This command is used to gain a token */
	char *const verifycmd[] =
	{
		"/usr/bin/sudo", "-p", "GNOME_SUDO_PASS", "-v", NULL
	};
	int argcount = 8;
	int i, j;

	GQuark gksu_quark;

	gchar *xauth = NULL,
				   *xauth_env = NULL;

	pid_t pid;
	int status;
	FILE *fdfile = NULL;
	int fdpty = -1;

	context->sudo_mode = TRUE;

	gksu_quark = g_quark_from_string (PACKAGE);

	if (!context->command)
	{
		g_set_error (error, gksu_quark, GKSU_ERROR_NOCOMMAND,
					 _("gksu_sudo_run needs a command to be run, "
					   "none was provided."));
		return FALSE;
	}

	if (!context->user)
		context->user = g_strdup ("root");

	if (ask_pass == NULL)
	{
		if (context->debug)
			fprintf (stderr, "No ask_pass set, using default!\n");

		ask_pass = su_ask_password;
	}

	if (pass_not_needed == NULL)
	{
		pass_not_needed = no_pass;
	}

	if (context->always_ask_password)
	{
		gint exit_status;
		g_spawn_command_line_sync("/usr/bin/sudo -K", NULL, NULL, &exit_status, NULL);
	}


	/*
	   FIXME: need to set GError in a more detailed way
	*/

	if (!sudo_prepare_xauth (context))
	{
		g_set_error (error, gksu_quark, GKSU_ERROR_XAUTH,
					 _("Unable to copy the user's Xauthorization file."));
		return FALSE;
	}

	/* sets XAUTHORITY */
	xauth = g_strdup_printf ("%s/.Xauthority", context->dir);

	xauth_env = getenv ("XAUTHORITY");

	setenv ("XAUTHORITY", xauth, TRUE);

	if (context->debug)
		fprintf (stderr, "xauth: %s\n", xauth);

	/* set startup id */
	if (context->sn_context)
		gksu_context_launch_initiate (context);

	cmd = g_new (gchar *, argcount + 1);

	argcount = 0;

	/* sudo binary */
	cmd[argcount] = g_strdup("/usr/bin/sudo");

	argcount++;

	if (!context->keep_env)
	{
		/* Make sudo set $HOME */
		cmd[argcount] = g_strdup("-H");
		argcount++;
	}

	/* Make sudo read from stdin */
	cmd[argcount] = g_strdup("-S");

	argcount++;

	/* Make sudo noninteractive (we should already have a token) */
	cmd[argcount] = g_strdup("-n");

	argcount++;

	/* Make sudo use next arg as prompt */
	cmd[argcount] = g_strdup("-p");

	argcount++;

	/* prompt */
	cmd[argcount] = g_strdup("GNOME_SUDO_PASS");

	argcount++;

	/* Make sudo use the selected user */
	cmd[argcount] = g_strdup("-u");

	argcount++;

	/* user */
	cmd[argcount] = g_strdup(context->user);

	argcount++;

	/* sudo does not understand this if we do not use -H
	   weird.
	*/
	if (!context->keep_env)
	{
		/* Make sudo stop processing options */
		cmd[argcount] = g_strdup("--");
		argcount++;
	}

	{
		gchar *tmp_arg = g_malloc (sizeof(gchar) * 1);
		gboolean inside_quotes = FALSE;

		tmp_arg[0] = '\0';

		for (i = j = 0; ; i++)
		{
			if ((context->command[i] == '\'') && (context->command[i-1] != '\\'))
			{
				i = i + 1;
				inside_quotes = !inside_quotes;
			}

			if ((context->command[i] == ' ' && inside_quotes == FALSE)
				|| context->command[i] == '\0')
			{
				tmp_arg = g_realloc (tmp_arg, sizeof(gchar) * (j + 1));
				tmp_arg[j] = '\0';
				cmd = g_realloc (cmd, sizeof(gchar*) * (argcount + 1));
				cmd[argcount] = g_strdup (tmp_arg);

				g_free (tmp_arg);

				argcount = argcount + 1;
				j = 0;

				if (context->command[i] == '\0')
					break;

				tmp_arg = g_malloc (sizeof(gchar) * 1);

				tmp_arg[0] = '\0';
			}

			else
			{
				if (context->command[i] == '\\' && context->command[i+1] != '\\')
					i = i + 1;

				tmp_arg = g_realloc (tmp_arg, sizeof(gchar) * (j + 1));

				tmp_arg[j] = context->command[i];

				j = j + 1;
			}
		}
	}

	cmd = g_realloc (cmd, sizeof(gchar*) * (argcount + 1));

	cmd[argcount] = NULL;

	if (context->debug)
	{
		for (i = 0; cmd[i] != NULL; i++)
			fprintf (stderr, "cmd[%d]: %s\n", i, cmd[i]);
	}

	pid = forkpty(&fdpty, NULL, NULL, NULL);

	if (pid == 0)
	{
		// Child
		setsid();   // make us session leader

		execv(verifycmd[0], verifycmd);

		g_set_error (error, gksu_quark, GKSU_ERROR_EXEC,
					 _("Failed to exec new process: %s"),
					 strerror(errno));
		sudo_reset_xauth (context, xauth, xauth_env);
		return FALSE;
	}

	else
		if (pid == -1)
		{
			g_set_error (error, gksu_quark, GKSU_ERROR_FORK,
						 _("Failed to fork new process: %s"),
						 strerror(errno));
			sudo_reset_xauth (context, xauth, xauth_env);
			return FALSE;
		}

		else
		{
			gint counter = 0;
			gchar *cmdline = NULL;

			struct termios tio;

			// Parent
			fdfile = fdopen(fdpty, "w+");

			/* make sure we notice that ECHO is turned off, if it gets
			   turned off */
			tcgetattr (fdpty, &tio);

			for (counter = 0; (tio.c_lflag & ECHO) && counter < 15; counter++)
			{
				usleep (1000);
				tcgetattr (fdpty, &tio);
			}

			fcntl (fdpty, F_SETFL, O_NONBLOCK);

			{ /* no matter if we can read, since we're using
	   O_NONBLOCK; this is just to avoid the prompt
	   showing up after the read */
				fd_set rfds;

				struct timeval tv;

				FD_ZERO(&rfds);
				FD_SET(fdpty, &rfds);
				tv.tv_sec = 1;
				tv.tv_usec = 0;

				select (fdpty + 1, &rfds, NULL, NULL, &tv);
			}

			/* Try hard to find the prompt; it may happen that we're
			 * seeing sudo's lecture, or that some pam module is spitting
			 * debugging stuff at the screen
			 */

			for (counter = 0; counter < 50; counter++)
			{
				if (strncmp (buffer, "GNOME_SUDO_PASS", 15) == 0)
					break;

				read_line (fdpty, buffer, 256);

				if (context->debug)
					fprintf (stderr, "buffer: -%s-\n", buffer);

				usleep(1000);
			}

			if (context->debug)
				fprintf (stderr, "brute force GNOME_SUDO_PASS ended...\n");

			if (strncmp(buffer, "GNOME_SUDO_PASS", 15) == 0)
			{
				gchar *password = NULL;
				gboolean prompt_grab;

				if (context->debug)
					fprintf (stderr, "Yeah, we're in...\n");

				prompt_grab = gconf_client_get_bool (context->gconf_client, BASE_PATH "prompt",
							  NULL);

				if (prompt_grab)
					gksu_prompt_grab (context);

				password = ask_pass (context, _("Password: "),
						   ask_pass_data, error);

				if (password == NULL || (*error))
				{
					nullify_password (password);
					return FALSE;
				}

				usleep (1000);

				write (fdpty, password, strlen(password) + 1);
				write (fdpty, "\n", 1);

				nullify_password (password);

				fcntl(fdpty, F_SETFL, fcntl(fdpty, F_GETFL) & ~O_NONBLOCK);
				/* ignore the first newline that comes right after sudo receives
				   the password */
				fgets (buffer, 255, fdfile);
				/* this is the status we are interested in */
				fgets (buffer, 255, fdfile);
			}

			else
			{
				gboolean should_display;

				if (context->debug)
					fprintf (stderr, "No password prompt found; we'll assume we don't need a password.\n");

				/* turn NONBLOCK off, also if have no prompt */
				fcntl(fdpty, F_SETFL, fcntl(fdpty, F_GETFL) & ~O_NONBLOCK);

				should_display = gconf_client_get_bool (context->gconf_client,
								 BASE_PATH "display-no-pass-info", NULL);

				/* configuration tells us to show this message */
				if (should_display)
				{
					if (context->debug)
						fprintf (stderr, "Calling pass_not_needed window...\n");

					pass_not_needed (context, pass_not_needed_data);

					/* make sure it is displayed */
					while (gtk_events_pending ())
						gtk_main_iteration ();
				}

				fprintf (stderr, "%s", buffer);
			}

			if (g_str_has_prefix (buffer, "Sorry, try again."))
				g_set_error (error, gksu_quark, GKSU_ERROR_WRONGPASS,
							 _("Wrong password."));
			else
			{
				gchar *haystack = buffer;
				gchar *needle;

				needle = g_strstr_len (haystack, strlen (haystack), " ");

				if (needle && (needle + 1))
				{
					needle += 1;

					if (!strncmp (needle, "is not in", 9))
						g_set_error (error, gksu_quark, GKSU_ERROR_NOT_ALLOWED,
									 _("The underlying authorization mechanism (sudo) "
									   "does not allow you to run this program. Contact "
									   "the system administrator."));
				}
			}

			/* If we have an error, let's just stop sudo right there. */

			if (error)
				close(fdpty);

			cmdline = g_strdup("sudo");

			/* wait for the child process to end or become something other
			than sudo */
			pid_t pid_exited;

			while ((!(pid_exited = waitpid (pid, &status, WNOHANG))) &&
				   (g_str_has_suffix(cmdline, "sudo")))
			{
				if (cmdline)
					g_free (cmdline);

				cmdline = get_process_name (pid);

				usleep(100000);
			}

			if (context->sn_context)
				gksu_context_launch_complete (context);

			/* if the process is still active waitpid() on it */
			if (pid_exited != pid)
				waitpid(pid, &status, 0);

			sudo_reset_xauth (context, xauth, xauth_env);

			/*
			 * Did token acquisition succeed? If so, spawn sudo in
			 * non-interactive mode. It should either succeed or die
			 * immediately if you're not allowed to run the command.
			 */
			if (WEXITSTATUS(status) == 0)
			{
				g_spawn_sync(NULL, cmd, NULL, 0, NULL, NULL,
							 NULL, &child_stderr, &status,
							 error);
			}

			if (exit_status)
			{
				if (WIFEXITED(status))
				{
					*exit_status = WEXITSTATUS(status);
				}

				else
					if (WIFSIGNALED(status))
					{
						*exit_status = -1;
					}
			}

			if (WEXITSTATUS(status))
			{
				if (g_str_has_prefix(child_stderr, "Sorry, user "))
				{
					g_set_error (error, gksu_quark, GKSU_ERROR_NOT_ALLOWED,
								 _("The underlying authorization mechanism (sudo) "
								   "does not allow you to run this program. Contact "
								   "the system administrator."));
				}

				if (cmdline)
				{
					/* sudo already exec()ed something else, don't report
					 * exit status errors in that case
					 */
					if (!g_str_has_suffix (cmdline, "sudo"))
					{
						g_free (cmdline);
						g_free (child_stderr);
						return FALSE;
					}

					g_free (cmdline);
				}

				if (error == NULL)
					g_set_error (error, gksu_quark,
								 GKSU_ERROR_CHILDFAILED,
								 _("sudo terminated with %d status"),
								 WEXITSTATUS(status));
			}
		}

	fprintf(stderr, child_stderr);

	g_free(child_stderr);

	/* if error is set we have found an error condition */
	return (error == NULL);
}
