#include <gtk/gtk.h>

static void 
print_hello1 (GtkApplication *app,
		  gpointer data)
{
	g_print ("Hello World 1\n");
}
static void 
print_hello2 (GtkApplication *app,
		  gpointer data)
{
	g_print ("Hello World 2\n");
}

int 
main (int argc,
	  char **argv)
{

	GtkBuilder *builder;
	GObject *window;
	GObject *button;

	gtk_init (&argc, &argv);

	builder = gtk_builder_new ();
	gtk_builder_add_from_file (builder, "builder.ui", NULL);

	window = gtk_builder_get_object (builder, "window");
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	button = gtk_builder_get_object (builder, "button1");
	g_signal_connect (button, "clicked", G_CALLBACK (print_hello1), NULL);

	button = gtk_builder_get_object (builder, "button2");
	g_signal_connect (button, "clicked", G_CALLBACK (print_hello2), NULL);
	
	button = gtk_builder_get_object (builder, "quit");
	g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);

	gtk_main ();

	return 0;

}
