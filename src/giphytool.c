/* giphytool (c) 2017 Ceysun Sucu */

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>


static void destroy(void) 
{
  gtk_main_quit();
}

static void usage(void)
{
  fprintf (stderr, "\nUsage:\n giphytool [options] [gif-path]\n\n");
  fprintf (stderr, "Options:\n \
      -p      -Set position by x and y cordinates. -p x y (i.e -p 100 100)\n \
      -c      -Center\n \
      -tl     -Posiftion top-left\n \
      -tr     -Posiftion top-right\n \
      -bl     -Posiftion bottom-left\n \
      -br     -Posiftion bottom-right\n");
}

int main (int argc, char **argv)
{

  int i;
  pid_t pid, sid;
  gint x_pos, y_pos;

  GtkWidget *window;
  GtkWidget *layout;
  GtkWidget *image;
  GdkPixbuf *pb;

  int center_window = 0;

  gtk_init(NULL,NULL);
  pb = gdk_pixbuf_new_from_file (argv[argc-1],NULL);
  gint image_width = gdk_pixbuf_get_width(pb);
  gint image_height = gdk_pixbuf_get_height(pb);

  for (i = 1; i < argc; i++)
  {
    if(strcmp(argv[i], "-p") == 0)
    {
      x_pos = atoi(argv[i+1]);
      y_pos = atoi(argv[i+2]);    
    }
    else if(strcmp(argv[i], "-tl") == 0)
    {
      x_pos = 0;
      y_pos = 0;;
    }
    else if(strcmp(argv[i], "-tr") == 0)
    {
      x_pos = gdk_screen_width()-image_width;
      y_pos = 0;
    }
    else if(strcmp(argv[i], "-c") == 0)
    {
      center_window = 1;
    }
    else if(strcmp(argv[i], "-bl") == 0)
    {
      x_pos = 0;
      y_pos = gdk_screen_height()-image_height;
    }
    else if(strcmp(argv[i], "-br") == 0)
    {
      x_pos = gdk_screen_width()-image_width;
      y_pos = gdk_screen_height()-image_height;
    }
    else if(strcmp(argv[i], "--help") == 0)
    {
      usage();
      return 0;
    }
  }
  
  pid = fork();
  if (pid < 0)
    exit(EXIT_FAILURE);
  
  if (pid > 0)
    exit(EXIT_SUCCESS);
  
  umask(0);

  sid = setsid();
  if (sid < 0)
    exit(EXIT_FAILURE);

  if ((chdir("/")) < 0)
    exit(EXIT_FAILURE);

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  layout = gtk_layout_new(NULL, NULL);
  image = gtk_image_new_from_file(argv[argc-1]);

  gtk_container_add(GTK_CONTAINER (window), layout);
  gtk_widget_show(layout);
  gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), image_width, image_height);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_widget_set_app_paintable(window, TRUE);
  gtk_window_set_type_hint((GtkWindow*)window, (GdkWindowTypeHint) GDK_WINDOW_TYPE_HINT_DESKTOP);
  gtk_window_set_decorated((GtkWindow*)window, 0);
  gtk_window_set_keep_below((GtkWindow*)window, 1);
  gtk_window_set_accept_focus((GtkWindow*)window, 0);

  g_signal_connect((gpointer)window, "destroy", G_CALLBACK(destroy), NULL);

  if(!center_window)
    gtk_window_move((GtkWindow*)window, x_pos, y_pos);

  gtk_widget_show_all (window);
  gtk_main();

  return 0;
}

