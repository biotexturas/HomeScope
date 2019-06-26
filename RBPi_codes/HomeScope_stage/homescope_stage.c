#include <gtk/gtk.h>
#include <stdlib.h>

void set_gpio_modes(void)
	{
	// set pins (wiring Pi) to 00,00 state meaning no movement
	system("gpio mode 1 OUT");
	system("gpio write 1 0");

	system("gpio mode 2 OUT");
	system("gpio write 2 0");

	system("gpio mode 3 OUT");
	system("gpio write 3 0");

	system("gpio mode 4 OUT");
	system("gpio write 4 0");

	}


static void move_x_up(GtkWidget *widget, gpointer user_data)
  {
  g_print("Hello x axis: we are going up!\n");
  system("gpio write 1 1");
  system("gpio write 2 0");
  }


static void move_x_down(GtkWidget *widget, gpointer user_data)
  {
  g_print("Hello x axis: we are going down!\n");
  system("gpio write 1 0");
  system("gpio write 2 1");
	}


static void move_y_up(GtkWidget *widget, gpointer user_data)
	{
  g_print("Hello y axis: we are going up!\n");
  system("gpio write 3 1");
  system("gpio write 4 0");
	}


static void move_y_down(GtkWidget *widget, gpointer user_data)
	{
  g_print ("Hello y axis: we are going down!\n");
  system("gpio write 3 0");
  system("gpio write 4 1");
	}


static void stop_moving(GtkWidget *widget, gpointer user_data)
  {
  g_print ("Stop moving stage now!\n");
  system("gpio write 1 0");
	system("gpio write 2 0");
	system("gpio write 3 0");
	system("gpio write 4 0");
  }


	// HERE GOES THE ABOUT DIALOG BOX For info at a website: lab wiki on the contact process
static void show_about(GtkWidget *widget, gpointer user_data)
	{
	//GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("kimero_LAB_transparent.tiff", NULL);
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("biotexturas_logo_text.tiff", NULL);
	GtkWidget *dialog = gtk_about_dialog_new();
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog),
                                    "HomeScope Stage");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "version 1.0");
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),"Biotexturas");
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog),
     "HomeScope is a tool to explore collective intelligence under you own feet!");
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog),
     "http://homescope.biotexturas.org");
	gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);
	g_object_unref(pixbuf), pixbuf = NULL;
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	}


int main(int argc, char *argv[])
	{
	GtkBuilder *builder;

	GObject *window;
	GObject *button;

	gtk_init(&argc, &argv);

	/* Construct a GtkBuilder instance and load our UI description */
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "builder_XY.ui", NULL);

	/* Connect signal handlers to the constructed widgets. */
	window = gtk_builder_get_object (builder, "window");
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	button = gtk_builder_get_object(builder, "stop_button");
	g_signal_connect(button, "clicked", G_CALLBACK(stop_moving), NULL);

	button = gtk_builder_get_object(builder, "x_up_button");
	g_signal_connect(button, "clicked", G_CALLBACK(move_x_up), NULL);

	button = gtk_builder_get_object(builder, "x_down_button");
	g_signal_connect(button, "clicked", G_CALLBACK(move_x_down), NULL);

	button = gtk_builder_get_object(builder, "y_up_button");
	g_signal_connect(button, "clicked", G_CALLBACK(move_y_up), NULL);

	button = gtk_builder_get_object(builder, "y_down_button");
	g_signal_connect(button, "clicked", G_CALLBACK(move_y_down), NULL);

	button = gtk_builder_get_object(builder, "about_button");
	g_signal_connect(button, "clicked", G_CALLBACK(show_about), GTK_WINDOW(window));

	button = gtk_builder_get_object (builder, "quit_button");
	g_signal_connect(button, "clicked", G_CALLBACK(gtk_main_quit), NULL);

	set_gpio_modes();

	gtk_main();

	return 0;
	}
