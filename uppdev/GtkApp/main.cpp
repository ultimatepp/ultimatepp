#include <CtrlLib/CtrlLib.h>

void gtk_my_patter_add(GtkWidget   *filechooser,
                  const gchar *title,
                  const gchar *pattern,
                  const gint   id)
{
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, title);
    gtk_file_filter_add_pattern(filter, pattern);
    g_object_set_data( G_OBJECT( filter ), "id", GINT_TO_POINTER( id ) );
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filechooser), filter);
}

int main( int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	

	// codes
	GtkWidget *ofd = gtk_file_chooser_dialog_new("open", NULL/*GTK_WINDOW(pmw->window1)*/,
	   GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL,
	   GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_OK, NULL);
	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(ofd), TRUE);
	gtk_my_patter_add(ofd, "Plain text (*.txt)", "*.txt", 0);
	gtk_my_patter_add(ofd, "XML files (*.xml)", "*.xml", 1);
	gtk_my_patter_add(ofd, "All files (*.*)", "*", 2);
	int ret = gtk_dialog_run(GTK_DIALOG(ofd));
	//
	int id = GPOINTER_TO_INT( g_object_get_data(
	                          G_OBJECT( gtk_file_chooser_get_filter( GTK_FILE_CHOOSER( ofd ) ) ), "id" ) );
	//
	gtk_widget_destroy(ofd);


/*

	GtkWidget *window;
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_widget_show_all(window);
	
	gtk_main();
*/
	return 0;
}
