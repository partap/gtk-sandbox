#include <gtk/gtk.h>
#include "s3-reticle.h"

struct _S3Reticle
{
	GtkImage parent_instance;

	int width;
	int height;

	double x_pos;
	double y_pos;

	double x_cur;
	double y_cur;
};

G_DEFINE_TYPE(S3Reticle, s3_reticle, GTK_TYPE_DRAWING_AREA)

static gboolean
s3_reticle_draw (S3Reticle *reticle, 
                 cairo_t *cr, 
                 gpointer user_data)
{
	int width;
	int height;
	double off_x;
	double off_y;

	width = reticle->width = gtk_widget_get_allocated_width (GTK_WIDGET (reticle));
	height = reticle->height = gtk_widget_get_allocated_height (GTK_WIDGET (reticle));

	off_x = reticle->x_pos * width;
	off_y = reticle->y_pos * height;

	// g_print ("s3_reticle_draw: %dx%d (%lf, %lf)\n", width, height, off_x, off_y);

	// Paint
	cairo_save (cr);
	cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.25);
	cairo_set_line_width (cr, 5);
	cairo_move_to (cr, off_x, 0);
	cairo_line_to (cr, off_x, (double) height - 1);
	cairo_move_to (cr, 0, off_y);
	cairo_line_to (cr, (double) width - 1, off_y);
	cairo_stroke (cr);

	cairo_set_source_rgba (cr, 0, 0, 0, 1.0);
	cairo_set_line_width (cr, 1);
	cairo_move_to (cr, off_x, 0);
	cairo_line_to (cr, off_x, (double) height - 1);
	cairo_move_to (cr, 0, off_y);
	cairo_line_to (cr, (double) width - 1, off_y);
	cairo_stroke (cr);
	// cairo_translate (cr, off_x, off_y);
	// cairo_scale (cr, scale_xy, scale_xy);

	// gdk_cairo_set_source_pixbuf (cr, myapp->pixbuf, 0, 0);
	// cairo_paint (cr);
	cairo_restore (cr);


	return FALSE;
}

static gboolean 
s3_reticle_button_pressed (S3Reticle *reticle, GdkEventButton *event, gpointer user_data)
{
	if (event->button == GDK_BUTTON_PRIMARY) {
		// g_print ("s3_reticle_button_pressed (%lf, %lf)\n", event->x, event->y);
		reticle->x_cur = event->x;
		reticle->y_cur = event->y;
		reticle->x_pos = reticle->x_cur / reticle->width;
		reticle->y_pos = reticle->y_cur / reticle->height;
		gtk_widget_queue_draw (GTK_WIDGET (reticle));
		return TRUE;
	}
	return FALSE;
}

static gboolean 
s3_reticle_button_released (S3Reticle *reticle, GdkEventButton *event, gpointer user_data)
{
	if (event->button == GDK_BUTTON_PRIMARY) {
		g_print ("s3_reticle_button_released (%lf, %lf)\n", event->x, event->y);
		reticle->x_cur = event->x;
		reticle->y_cur = event->y;
		reticle->x_pos = reticle->x_cur / reticle->width;
		reticle->y_pos = reticle->y_cur / reticle->height;
		return TRUE;
	}
	return FALSE;
}

static gboolean 
s3_reticle_mouse_moved (S3Reticle *reticle, GdkEventMotion *event, gpointer user_data)
{
	reticle->x_cur = event->x;
	reticle->y_cur = event->y;
	if (!(event->state & GDK_BUTTON1_MASK)) {
		return FALSE;
	}
	// g_print ("s3_reticle_mouse_moved (%lf, %lf)\n", event->x, event->y);
	reticle->x_pos = reticle->x_cur / reticle->width;
	reticle->y_pos = reticle->y_cur / reticle->height;
	gtk_widget_queue_draw (GTK_WIDGET (reticle));

	return TRUE;
}

static void
s3_reticle_init (S3Reticle *reticle)
{
	// init stuff
	gint event_mask;
	reticle->x_pos = 0.5;
	reticle->y_pos = 0.5;

	g_signal_connect (reticle, "draw", G_CALLBACK (s3_reticle_draw), NULL);
	g_signal_connect (reticle, "button-press-event", G_CALLBACK (s3_reticle_button_pressed), NULL);
	g_signal_connect (reticle, "button-release-event", G_CALLBACK (s3_reticle_button_released), NULL);
	g_signal_connect (reticle, "motion-notify-event", G_CALLBACK (s3_reticle_mouse_moved), NULL);
	event_mask = gtk_widget_get_events (GTK_WIDGET (reticle));
	// event_mask |= GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK;
	event_mask |= GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK;
	gtk_widget_set_events (GTK_WIDGET (reticle), event_mask);

}

static void
s3_reticle_class_init (S3ReticleClass *class)
{
	// virtual function overrides go here
	// property and signal definitions go here
}

S3Reticle *
s3_reticle_new (void)
{
	return g_object_new (S3_RETICLE_TYPE, NULL);
}


