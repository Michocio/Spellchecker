#include <gtk/gtk.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <locale.h>
#include "editor.h"


struct dictionary *dict;
int loaded=0;
// gcc editor.c -Wall -o Editor `pkg-config --cflags --libs gtk+-2.0`

GtkWidget *editor_window;   // Main Gtk window

GtkWidget *editor_view;     // Text view widget where all the typing is done
GtkTextBuffer *editor_buf;  // Textview buffer

// Display error message

static void err_msg (const gchar *msg) {
  GtkWidget *dialog;  // pop up window with only OK button

  dialog = gtk_message_dialog_new(GTK_WINDOW(editor_window), 0,
                                  GTK_MESSAGE_ERROR,
                                  GTK_BUTTONS_OK, msg);
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);  // destroy the window after ok is pressed
}

// When the user clicks on quit button or quits the app fire the delete event 
// signal but first check if the file is modified or not

static gboolean delete_event (GtkWidget *widget, GdkEvent *event,
                              gpointer data) {
  return !save_if_modified();
}

// Main function

int main (int argc, char *argv[]) {
	
	setlocale(LC_ALL, "pl_PL.UTF-8");
 
  GtkWidget *box, *scroll, *menu;
  GtkAccelGroup *accel = NULL;
      
  gtk_init(&argc, &argv);
  editor_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  // If the user quits the app  
  g_signal_connect(G_OBJECT(editor_window), "delete_event",
                   G_CALLBACK(delete_event), NULL);
  g_signal_connect(G_OBJECT(editor_window), "destroy",
                   G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_set_title(GTK_WINDOW(editor_window),
                       "Text Editor (Insert file name)");
  gtk_window_set_default_size(GTK_WINDOW(editor_window), 600, 500);
    
  // Create a container where the menus and text widget go
  box = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(editor_window), box);
    
  // Create an accelerator for the item factory, keyboard shortcuts
  //accel = gtk_accel_group_new();
  //gtk_window_add_accel_group(GTK_WINDOW(editor_window), accel);
    
  // Create a menu bar above the text view widget
  menu = create_menu(accel);
  extend_menu(menu);

  // Put the menu in the menu container
  gtk_box_pack_start(GTK_BOX(box), menu, FALSE, FALSE, 0);
    
  //* Put a scroll window widget
  scroll = gtk_scrolled_window_new(NULL, NULL);
  gtk_box_pack_start(GTK_BOX(box), scroll, TRUE, TRUE, 0);
  
  // Create the text view widget
  editor_view = gtk_text_view_new();
  gtk_container_add(GTK_CONTAINER(scroll), editor_view);

  // Get a buffer for the text view where everything gets stored
  editor_buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor_view));

  gtk_widget_show_all(editor_window);

  // Error checking
  g_set_printerr_handler(err_msg);

  // load the file if a filename is specified
  if (argc > 1)
    load_file(g_strdup(argv[1]));

  // Start the main loop
  gtk_main();

  return 0;  // for stupid compilers
}

/*EOF*/
