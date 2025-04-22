#include "CtrlLib.h"

namespace Upp {

#ifdef GUI_GTK

FileSelNative::FileSelNative()
{
	confirm = true;
	multi = hidden = false;
	activetype = 0;
}

FileSelNative& FileSelNative::AllFilesType() {
	return Type(t_("All files"), "*.*");
}

void FileSelNative::Serialize(Stream& s) {
	int version = 1;
	s / version;
	s / activetype % ipath;
}

bool FileSelNative::Execute0(int mode, const char *title)
{
	Ctrl::ReleaseCtrlCapture();
	if(!title)
		title = decode(mode, 1, t_("Open"), 0, t_("Save as"), t_("Select folder"));
	Ctrl *w = Ctrl::GetActiveWindow();
	GtkWidget *fc = gtk_file_chooser_dialog_new(title, w ? w->gtk() : NULL,
	                                            decode(mode, 0, GTK_FILE_CHOOSER_ACTION_SAVE,
	                                                         1, GTK_FILE_CHOOSER_ACTION_OPEN,
	                                                         GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER),
	                                            t_("Cancel"), GTK_RESPONSE_CANCEL,
	                                            decode(mode, 1, t_("Open"), 0, t_("Save"), t_("Select")),
	                                            GTK_RESPONSE_OK,
	                                            NULL);
	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(fc), confirm);
	gtk_file_chooser_set_local_only(GTK_FILE_CHOOSER(fc), true);
	gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(fc), multi);
	gtk_file_chooser_set_show_hidden(GTK_FILE_CHOOSER(fc), hidden);
	
	if(IsFullPath(ipath)) {
		FindFile ff(ipath);
		if(ff)
			if(ff.IsFolder())
				gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(fc), ipath);
			else
				gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(fc), ipath);
		else {
			gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(fc), GetFileFolder(ipath));
			gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(fc), GetFileName(ipath));
		}
	}
	else
		gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(fc), ipath);
	ipath.Clear();
	for(int i = 0; i < type.GetCount(); i++) {
	    GtkFileFilter *filter = gtk_file_filter_new();
	    gtk_file_filter_set_name(filter, type[i].a);
	    Vector<String> patterns = Split(type[i].b, [](int c){ return (int)(c == ';' || c == ' '); });
	    for(int j = 0; j < patterns.GetCount(); j++)
			gtk_file_filter_add_pattern(filter, decode(patterns[j], "*.*", "*", patterns[j]));
	    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(fc), filter);
	    if(i == activetype)
			gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(fc), filter);
	}
	bool ret = false;
	path.Clear();
	if(gtk_dialog_run(GTK_DIALOG(fc)) == GTK_RESPONSE_OK) {
		GSList *list = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(fc));
		if(list) {
			GSList *l;
			for(l = list; l; l = l->next) {
				path.Add((const char *)l->data);
				g_free(l->data);
			}
			g_slist_free (list);
		}
		ret = true;
		gchar *h = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER(fc));
		ipath = h;
		g_free(h);
	}
	gtk_widget_destroy(fc);
	return ret;
}

#endif

}
