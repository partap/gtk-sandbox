#ifndef _s3_reticle_h_
#define _s3_reticle_h_

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define S3_RETICLE_TYPE (s3_reticle_get_type ())
G_DECLARE_FINAL_TYPE(S3Reticle, s3_reticle, S3, RETICLE, GtkDrawingArea)

S3Reticle * s3_reticle_new (void);

// void
// s3_reticle_set_pixbuf (S3Reticle *image, GdkPixbuf *pixbuf);

G_END_DECLS
#endif