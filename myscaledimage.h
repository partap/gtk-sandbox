#ifndef _my_scaled_image_h_
#define _my_scaled_image_h_

// #include <gtk/gtk.h>

G_BEGIN_DECLS

#define MY_SCALED_IMAGE_TYPE (my_scaled_image_get_type ())
G_DECLARE_FINAL_TYPE(MyScaledImage, my_scaled_image, MY, SCALED_IMAGE, GtkImage)

MyScaledImage * my_scaled_image_new (void);

void
my_scaled_image_set_pixbuf (MyScaledImage *image, GdkPixbuf *pixbuf);

G_END_DECLS
#endif