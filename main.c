#include <math.h>
#include <gtk/gtk.h>
#include "myscaledimage.h"

#define BACKGROUND_FILE "test_pattern.png"

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



typedef struct {
	GdkPixbuf *pixbuf;
	GtkWidget *background;
} MyApp;


// from python example: 
// https://stackoverflow.com/questions/5067310/pygtk-how-do-i-make-an-image-automatically-scale-to-fit-its-parent-widget
static gboolean
draw_background_image (GtkWidget *widget, 
                       cairo_t *cr, 
                       MyApp *myapp)
{
	int width;
	int height;
	int img_width;
	int img_height;
	
	double width_ratio;
	double height_ratio;
	double scale_xy;
	double off_x;
	double off_y;

	// g_print("draw_background_image\n");
	width = gtk_widget_get_allocated_width(widget);
	height = gtk_widget_get_allocated_height(widget);

	img_width = gdk_pixbuf_get_width (myapp->pixbuf);
	img_height = gdk_pixbuf_get_height (myapp->pixbuf);

	// Scale
	width_ratio = (double) width / img_width;
	height_ratio = (double) height / img_height;
	scale_xy = fmin (width_ratio, height_ratio);
	// Center
	off_x =  ((double) width - img_width * scale_xy) / 2;
	off_y = ((double) height - img_height * scale_xy) / 2;

	// Paint
	cairo_save (cr);

	cairo_translate (cr, off_x, off_y);
	cairo_scale (cr, scale_xy, scale_xy);

	gdk_cairo_set_source_pixbuf (cr, myapp->pixbuf, 0, 0);
	cairo_paint (cr);
	cairo_restore (cr);

	return FALSE;
}






MyApp *
my_app_new (void)
{
	MyApp *myapp;
	GtkBuilder *builder;
	GObject *window;
	GObject *button;
	GError *gerror;

	myapp = g_new0 (MyApp, 1);

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

	myapp->pixbuf = gdk_pixbuf_new_from_file (BACKGROUND_FILE, &gerror);
	if (!myapp-> pixbuf) {
		g_error("Unable to load %s\n", BACKGROUND_FILE);
	}
	g_print("Loaded pixbuf from %s: %x\n", BACKGROUND_FILE, (guint) myapp->pixbuf);
	myapp->background = GTK_WIDGET (gtk_builder_get_object (builder, "background"));
	if (MY_IS_SCALED_IMAGE (myapp->background)) {
		g_print("background is MyScaledImage\n");
		my_scaled_image_set_pixbuf (MY_SCALED_IMAGE (myapp->background), myapp->pixbuf);
	}
	else if (GTK_IS_IMAGE (myapp->background)) {
		g_print("background is GtkImage\n");
		gtk_image_set_from_pixbuf(GTK_IMAGE (myapp->background), myapp->pixbuf);
	}
	else if (GTK_IS_DRAWING_AREA (myapp->background)) {
		g_print("background is GtkDrawingArea\n");
		g_signal_connect (G_OBJECT (myapp->background), "draw",
                    G_CALLBACK (draw_background_image), myapp);

	} else {
		g_print("unknown background type\n");
	}
	return myapp;
}

int 
main (int argc,
	  char **argv)
{
	MyApp *myapp;

	gtk_init (&argc, &argv);

	myapp = my_app_new ();

	gtk_main ();

	return 0;

}
