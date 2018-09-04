#include <math.h>
#include <gtk/gtk.h>
#include "myscaledimage.h"
#include "s3-reticle.h"

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


// static void 
// attach_overlays(MyApp *myapp) 
// {
// 	GtkWidget *grid;
// 	gpointer background;

// 	GtkWidget *overlay;
// 	GtkWidget *image;
// 	GtkWidget *label;
// 	PangoAttribute *attr;
// 	PangoAttrList *attr_list;
// 	g_print("Attaching overlay to video drawing area\n");



// 	background = g_object_ref (myapp->background);
// 	grid = gtk_widget_get_parent (myapp->background);
// 	g_print("drawing area widget name: %s\n", gtk_widget_get_name (background));
// 	g_print("parent widget name: %s\n", gtk_widget_get_name (grid));
// 	gtk_grid_remove_row (GTK_GRID (grid), 1);

// 	overlay = gtk_overlay_new ();
// 	gtk_container_add (GTK_CONTAINER (overlay), background);
// 	gtk_grid_insert_row(GTK_GRID (grid), 1);
// 	gtk_grid_attach (GTK_GRID (grid), overlay, 0, 1, 2, 1);
// 	g_object_unref (background);

// 	// overlay = gtk_widget_get_parent (myapp->background);
// 	image = gtk_image_new_from_file ("thumbs_up_emoji.png");
// 	gtk_overlay_add_overlay (GTK_OVERLAY (overlay), image);
	
// 	attr_list = pango_attr_list_new ();
// 	attr = pango_attr_background_new (0, 65535, 65535);
// 	pango_attr_list_insert (attr_list, attr);
// 	attr = pango_attr_background_alpha_new (65000);
// 	pango_attr_list_insert (attr_list, attr);
// 	attr = pango_attr_scale_new (3.0);
// 	pango_attr_list_insert (attr_list, attr);

// 	label = gtk_label_new ("My Label");
// 	gtk_label_set_attributes (GTK_LABEL (label), attr_list);
// 	gtk_label_set_xalign (GTK_LABEL (label), 0.5);
// 	gtk_label_set_yalign (GTK_LABEL (label), 0.5);
// 	gtk_overlay_add_overlay (GTK_OVERLAY (overlay), label);
// 	// g_signal_connect (overlay, "realize", G_CALLBACK (overlay_realize_cb), myapp);
// 	gtk_widget_show_all (grid);
// }


static void 
attach_reticle (GtkOverlay *overlay)
{
	S3Reticle *reticle;
	g_print ("Attaching reticle to %s\n", gtk_widget_get_name (GTK_WIDGET (overlay)));

	reticle = s3_reticle_new ();
	gtk_overlay_add_overlay (overlay, GTK_WIDGET (reticle));
	gtk_widget_show_all (GTK_WIDGET (overlay));
}

MyApp *
my_app_new (void)
{
	MyApp *myapp;
	GtkBuilder *builder;
	GObject *window;
	GObject *button;
	GObject *overlay;
	GError *gerror;

	g_print ("my_app_new()\n");
	myapp = g_new0 (MyApp, 1);

	builder = gtk_builder_new ();
	gtk_builder_add_from_file (builder, "builder.ui", NULL);
	window = gtk_builder_get_object (builder, "window");
	// g_print ("window: %d\n", GTK_IS_WIDGET (window));
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	button = gtk_builder_get_object (builder, "button1");
	// g_print ("button1: %d\n", GTK_IS_WIDGET (button));
	g_signal_connect (button, "clicked", G_CALLBACK (print_hello1), NULL);

	button = gtk_builder_get_object (builder, "button2");
	// g_print ("button2: %d\n", GTK_IS_WIDGET (button));
	g_signal_connect (button, "clicked", G_CALLBACK (print_hello2), NULL);
	
	button = gtk_builder_get_object (builder, "quit");
	// g_print ("button3: %d\n", GTK_IS_WIDGET (button));
	g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);

	myapp->pixbuf = gdk_pixbuf_new_from_file (BACKGROUND_FILE, &gerror);
	if (!myapp-> pixbuf) {
		g_error("Unable to load %s\n", BACKGROUND_FILE);
	}
	g_print("Loaded pixbuf from %s: %lx\n", BACKGROUND_FILE, (long unsigned int) myapp->pixbuf);
	myapp->background = GTK_WIDGET (gtk_builder_get_object (builder, "background"));
	// g_print ("background: %d\n", GTK_IS_WIDGET (myapp->background));
	// g_print("background widget name: %s\n", gtk_widget_get_name (myapp->background));
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

	overlay = gtk_builder_get_object (builder, "overlay-container");
	attach_reticle(GTK_OVERLAY (overlay));
	// attach_overlays (myapp);
	return myapp;
}

// static void 
// print_supported_pixbuf_formats (void)
// {
// 	GSList *gslist;
// 	GdkPixbufFormat *fmt;
	
// 	gslist = gdk_pixbuf_get_formats ();

// }

int 
main (int argc,
	  char **argv)
{
	MyApp *myapp;
	gtk_init (&argc, &argv);
	// print_supported_pixbuf_formats ();
	myapp = my_app_new ();

	gtk_main ();

	return 0;

}
