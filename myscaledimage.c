#include <gtk/gtk.h>
#include "myscaledimage.h"

struct _MyScaledImage
{
	GtkImage parent_instance;
	GdkPixbuf *pixbuf;
};

G_DEFINE_TYPE(MyScaledImage, my_scaled_image, GTK_TYPE_IMAGE)

// static void 
// print_preferred_size (GtkWidget *widget) 
// {
// 	gint natural_width;
// 	gint natural_height;
// 	gint minimum_width;
// 	gint minimum_height;
// 	gtk_widget_get_preferred_width(widget, &minimum_width, &natural_width);
// 	gtk_widget_get_preferred_height(widget, &minimum_height, &natural_height);
// 	g_print("preferred size - min: %dx%d, natural: %dx%d\n", minimum_width, minimum_height, natural_width, natural_height);
// }

static void
my_scaled_image_size_allocate (MyScaledImage *image, 
                               GdkRectangle *rect, 
                               gpointer user_data)
{
	float k_pixbuf;
	float k_rect;
	int img_width;
	int img_height;
	int new_width;
	int new_height;
	int old_width;
	int old_height;
	GdkPixbuf * base_pixbuf;

	if (!image->pixbuf) {
		g_print("my_scaled_image_size_allocate: no pixbuf");
	}
	// g_print("my_scaled_image_size_allocate: pixbuf: %x\n", (guint) image->pixbuf);

	img_width = gdk_pixbuf_get_width (image->pixbuf);
	img_height = gdk_pixbuf_get_height (image->pixbuf);
	// g_print("Original size: %dx%d\n", img_width, img_height);

	k_pixbuf = (float) img_height / img_width;
	k_rect = (float) rect->height / rect->width;

	if (k_pixbuf < k_rect) {
		new_width = rect->width;
		new_height = (int) (new_width * k_pixbuf);
	} 
	else {
		new_height = rect->height;
		new_width = (int) (new_height / k_pixbuf);
	}

	base_pixbuf = gtk_image_get_pixbuf (GTK_IMAGE (image));
	old_width = gdk_pixbuf_get_width (base_pixbuf);
	old_height = gdk_pixbuf_get_height (base_pixbuf);
	// g_print("Old size: %dx%d\n", old_width, old_height);
	if (gdk_pixbuf_get_width(base_pixbuf) == new_width && gdk_pixbuf_get_height (base_pixbuf) == new_height) {
		return;
	}
	g_print("scaling image to %dx%d\n", new_width, new_height);
	base_pixbuf = gdk_pixbuf_scale_simple (image->pixbuf, new_width, new_height, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf (GTK_IMAGE (image), base_pixbuf);

	// gtk_widget_set_size_request (GTK_WIDGET (image), 320, 240);
	// print_preferred_size (GTK_WIDGET (image));
}

// static void
// my_scaled_image_get_preferred_width_for_height (GtkWidget *widget,
//                                                 gint height,
//                                                 gint *minimum_width,
//                                                 gint *natural_width) 
// {
// 	g_print ("my_scaled_image_get_preferred_width_for_height: %d, minimum_width: %d, natural_width: %d", height, *minimum_width, *natural_width);
// }

static void
my_scaled_image_init (MyScaledImage *image)
{
	// init stuff
	g_signal_connect (image, "size-allocate", G_CALLBACK (my_scaled_image_size_allocate), NULL);
}

static void
my_scaled_image_class_init (MyScaledImageClass *class)
{
	// virtual function overrides go here
	// GTK_WIDGET_CLASS (class)->get_preferred_width_for_height = my_scaled_image_get_preferred_width_for_height;
	// property and signal definitions go here
}

MyScaledImage *
my_scaled_image_new (void)
{
	return g_object_new (MY_SCALED_IMAGE_TYPE, NULL);
}


void
my_scaled_image_set_pixbuf (MyScaledImage *image, GdkPixbuf *pixbuf)
{
	// g_print("my_scaled_image_set_pixbuf %x\n", (guint) pixbuf);
	image->pixbuf = pixbuf;
	if (pixbuf) {
		gtk_image_set_from_pixbuf (GTK_IMAGE (image), image->pixbuf);
	}
}