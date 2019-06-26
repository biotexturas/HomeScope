#include <gtk/gtk.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

// This is a global variable holding the PID of a child to be created when accessing
// the camera with raspivid program
// to be used in the start_video call back
GPid pid;

void set_gpio_modes(void)
	{
	/* We use 2 PINS to communicate Z axis changes to the Arduino */
	/* DOWN - PIN, wPi n. 24 */
  system("gpio mode 24 OUT");
  system("gpio write 24 0");  // SET PIN @ LOW by Default

	/* UP - PIN wPi n. 29 */
	system("gpio mode 29 OUT");
  system("gpio write 29 0"); // SET PIN @ LOW by Default

	// Set the PWM pin and set it to zero
	system("gpio mode 23 pwm");
	system("gpio pwm 23 0");

	/* Done with setting pins */
	g_print("Finished setting up the GPIO PINs with gpio utility \n");
	system("gpio readall");
	}


	// HERE GOES THE ABOUT DIALOG BOX For info at a website: lab wiki on the process
static void show_about(GtkWidget *widget, gpointer user_data)
	{
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("biotexturas_logo_text.tiff", NULL);
	GtkWidget *dialog = gtk_about_dialog_new();
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog),
    "HomeScope Core");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "version 1.0");
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),"Biotexturas");
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog),
     "HomeScope is a tool to explore collective intelligence under you own feet!");
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog),
     "http://homescope.biotexturas.org");
	gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);
	g_object_unref(pixbuf), pixbuf = NULL;
	gtk_dialog_run(GTK_DIALOG (dialog));
	gtk_widget_destroy(dialog);
	}


	// the call back needs this casting or it doesn't work properly
static void start_video(GObject *switcher, GParamSpec *pspec, gpointer user_data)
	{
	if(gtk_switch_get_active(GTK_SWITCH(switcher)))
		{
    /* Here goes the code when the video swich is set ON */
		g_print("Video streaming started...\n");
		/* Argument vector to run as child process */
		gchar *child_argv[] = {"/usr/bin/raspivid", "-t", "0", "-ex", "off", "-p", "10,-120,750,750" ,"-v", "-awb" ,"sun", NULL};
		/* we need these also
    (besides the global process ID to kill the video raspivid later...) */
		gint stdout;
		GError *error = NULL;
		/* Now we spawn the child pricess to be handled with PID pid */
		g_spawn_async(NULL, child_argv, NULL, G_SPAWN_DEFAULT, NULL, NULL, &pid, &error);
		g_print("We have spawned a child process which pid is %i,"
            "and we did from process %i which was in turn made from parent %i"
            "(the running shell)\n", pid, getpid(),getppid());
		}else
			{
      /* Here goes the code when the video swich is set OFF */
			g_print("Video streaming stopped...\n");
			/* For some wierd rason, the line bellow doesnt close the child process */
			g_spawn_close_pid(pid);  // in UNIX doesn nieks, but could be useful in Windows
			kill(pid, 9);  // so we kill the process UNIX-style
			}
	}


/* not working properly...cannot recognize the switch... need to quit and kill raspivid if on */
static void clean_up_b4_go(GtkWidget *widget, gpointer user_data)
	{
	if(gtk_switch_get_state(GTK_SWITCH(user_data))){kill(pid, 9);}
	}


static void take_picture(GtkWidget *widget, gpointer user_data)
	{
	if(gtk_switch_get_state(GTK_SWITCH(user_data)))
		{
		gtk_switch_set_state(GTK_SWITCH(user_data), FALSE);
		system("raspistill -o open_scope_capture_picture.jpg -p 10,20,400,400 -v");
		gtk_switch_set_state(GTK_SWITCH(user_data), TRUE);
		}else{system("raspistill -o open_scope_capture_picture.jpg -p 10,20,400,400 -v");}
	}


static void move_up(GtkWidget *widget, gpointer user_data)
	{
	// LOW blue , HIGH purple goes up
  g_print("Hello Open Scope: we are going up!\n");
  system("gpio write 24 0");
	system("gpio write 29 1");
	}


static void move_down(GtkWidget *widget, gpointer user_data)
	{
  	g_print("Hello Open Scope: we are going down!\n");
  	system("gpio write 24 1");
  	system("gpio write 29 0");
	}


 static void stop(GtkWidget *widget, gpointer user_data)
	{
	g_print("Hello Open Scope: we stop moving!\n");
	system("gpio write 24 0");
	system("gpio write 29 0");
	}

static void pwm_scale_moved(GtkRange *range, gpointer user_data)
	{
	gdouble pos = gtk_range_get_value(GTK_RANGE(range));
	gint pwm_value;
	pwm_value = (gint)pos;
	gchar *str = g_strdup_printf("gpio pwm 23 %d", pwm_value);
	system(str);
	}


int main(int  argc, char *argv[])
  {
  /* Notice here we use builder (GLADE) to generate the interface */
  GtkBuilder *builder;

  GObject *window;
  GObject *button;
  GObject *switcher;
  GObject *scale;

 	gtk_init(&argc, &argv);

	/* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "builder_Z.ui", NULL);

 	/* Connect signal handlers to the constructed widgets. */
  window = gtk_builder_get_object(builder, "window");
  g_signal_connect(window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  button = gtk_builder_get_object(builder, "button_up");
  g_signal_connect(button, "clicked", G_CALLBACK (move_up), NULL);

  button = gtk_builder_get_object(builder, "button_down");
  g_signal_connect(button, "clicked", G_CALLBACK (move_down), NULL);

	button = gtk_builder_get_object(builder, "button_stop");
	g_signal_connect(button, "clicked", G_CALLBACK(stop), NULL);

	button = gtk_builder_get_object(builder, "button_about");
	g_signal_connect(button, "clicked", G_CALLBACK(show_about), GTK_WINDOW(window));

	/* Here we play with the switcher and THEN with buttons which are connected to it. */
	switcher = gtk_builder_get_object(builder, "switch_video");
	gtk_switch_set_active(GTK_SWITCH(switcher), FALSE);
	g_signal_connect(GTK_SWITCH(switcher), "notify::active",
                   G_CALLBACK(start_video), switcher);

	button = gtk_builder_get_object(builder, "button_take_photo");
  g_signal_connect(button, "clicked", G_CALLBACK(take_picture), switcher);

  button = gtk_builder_get_object(builder, "quit");
	g_signal_connect(button, "clicked", G_CALLBACK(clean_up_b4_go), switcher);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_main_quit), NULL);

	scale = gtk_builder_get_object(builder, "pwm_scale");
	g_signal_connect(scale, "value-changed", G_CALLBACK(pwm_scale_moved), NULL);

	/*Here we configure the modes of the GPIO PINs*/
  set_gpio_modes();

	gtk_main();
  return 0;
	}
