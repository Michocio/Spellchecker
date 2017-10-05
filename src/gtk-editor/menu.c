#include <gtk/gtk.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "editor.h"

// Used for selecting the text
GdkAtom sel_atom = GDK_SELECTION_CLIPBOARD;

// Identify the requests menu handler will manage.
#define MENU_NEW 1
#define MENU_OPEN 2
#define MENU_SAVE 3
#define MENU_SAVE_AS 4
#define MENU_QUIT 5
#define MENU_CUT 6
#define MENU_COPY 7
#define MENU_PASTE 8
#define MENU_USE_CLIPBOARD 9
#define MENU_SELECT_ALL 10
#define MENU_FIND 11
#define MENU_REPLACE 12
#define MENU_HELP 14
#define MENU_ABOUT 15

static GtkUIManager *uimanager;

static GtkWidget *get_named_widget (char *name) {
  return gtk_ui_manager_get_widget(uimanager, name);
}

static void menu_new (GtkMenuItem *menuitem, gpointer data) {
  if (save_if_modified()) {
    // get all the current tag table and put them in the new buffer
    editor_buf = gtk_text_buffer_new(gtk_text_buffer_get_tag_table(editor_buf));
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(editor_view), editor_buf);
    g_object_unref(G_OBJECT(editor_buf)); 
    // needed for freeing memory by the buffer when a new buffer is created
  }
}

static void menu_open (GtkMenuItem *menuitem, gpointer data) {
  if (save_if_modified()) {
    // call save if modified when user opens a new file
    editor_buf = gtk_text_buffer_new(gtk_text_buffer_get_tag_table(editor_buf));
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(editor_view), editor_buf);
    // needed for freeing memory by the buffer when a new buffer is created
    g_object_unref(G_OBJECT(editor_buf));
    load_file(NULL);
  }
}

static void menu_save (GtkMenuItem *menuitem, gpointer data) {
  save_file(filename);
}

static void menu_save_as (GtkMenuItem *menuitem, gpointer data) {
  save_file(NULL);
}

static void menu_quit (GtkMenuItem *menuitem, gpointer data) {
  if (save_if_modified())
    gtk_widget_destroy(editor_window);
}

static void menu_cut (GtkMenuItem *menuitem, gpointer data) {
  gtk_text_buffer_cut_clipboard(editor_buf, gtk_clipboard_get(sel_atom), TRUE);
}

static void menu_copy (GtkMenuItem *menuitem, gpointer data) {
  gtk_text_buffer_copy_clipboard(editor_buf, gtk_clipboard_get(sel_atom));
}

static void menu_paste (GtkMenuItem *menuitem, gpointer data) {
  // If null text is inserted at the current cursor position
  gtk_text_buffer_paste_clipboard(editor_buf, gtk_clipboard_get(sel_atom),
                                  NULL, TRUE);
}

static void menu_select_all (GtkMenuItem *menuitem, gpointer data) {
  GtkTextIter p; 
    
  // Get the starting point of the buffer
  gtk_text_buffer_get_start_iter(editor_buf, &p);
  // Ignore the selection made by the mouse
  gtk_text_buffer_place_cursor(editor_buf, &p);
  // Get the ending point of the buffer
  gtk_text_buffer_get_end_iter(editor_buf, &p);
  gtk_text_buffer_move_mark_by_name(editor_buf, "selection_bound", &p);
}

static void menu_find (GtkMenuItem *menuitem, gpointer data) {
  textfind();
}

static void menu_replace (GtkMenuItem *menuitem, gpointer data) {
  text_find_replace();	
}

static void menu_help (GtkMenuItem *menuitem, gpointer data) {
  show_help();
}

static void menu_about (GtkMenuItem *menuitem, gpointer data) {
  show_about();
}

static GtkActionEntry entries[] = {
  {"File", NULL, "_File", NULL, NULL, NULL},
  {"New", GTK_STOCK_NEW, NULL, NULL, NULL, G_CALLBACK(menu_new)},
  {"Open", GTK_STOCK_OPEN, NULL, NULL, "Open an existing file",
   G_CALLBACK(menu_open)},
  {"Save", GTK_STOCK_SAVE, NULL, NULL, NULL, G_CALLBACK(menu_save)},
  {"SaveAs", GTK_STOCK_SAVE_AS, NULL, NULL, NULL, G_CALLBACK(menu_save_as)},
  {"Quit", GTK_STOCK_QUIT, NULL, NULL, NULL, G_CALLBACK(menu_quit)},
  {"Edit", NULL, "_Edit", NULL, NULL, NULL},
  {"Cut", GTK_STOCK_CUT, NULL, NULL, NULL, G_CALLBACK(menu_cut)},
  {"Copy", GTK_STOCK_COPY, NULL, NULL, NULL, G_CALLBACK(menu_copy)},
  {"Paste", GTK_STOCK_PASTE, NULL, NULL, NULL, G_CALLBACK(menu_paste)},
  {"SelectAll", GTK_STOCK_SELECT_ALL, NULL, NULL, NULL, G_CALLBACK(menu_select_all)},
  {"Search", NULL, "_Search", NULL, NULL, NULL},
  {"Find", GTK_STOCK_FIND, NULL, NULL, NULL, G_CALLBACK(menu_find)},
  {"Replace", GTK_STOCK_FIND_AND_REPLACE, NULL, NULL, NULL, G_CALLBACK(menu_replace)},
  {"Help", NULL, "_Help", NULL, NULL, NULL},
  {"Contents", GTK_STOCK_HELP, NULL, NULL, NULL, G_CALLBACK(menu_help)},
  {"About", GTK_STOCK_ABOUT, NULL, NULL, NULL, G_CALLBACK(menu_about)}
};
#define NUM_ENTRIES 17

GtkWidget* create_menu (GtkAccelGroup *accel) {
  // UI Manager creates a menu from XML description
  GtkActionGroup *group = gtk_action_group_new("MainActionGroup");
  GtkWidget *menubar;

  uimanager = gtk_ui_manager_new();
  gtk_action_group_add_actions(group, entries, NUM_ENTRIES, NULL);

  gtk_ui_manager_insert_action_group(uimanager, group, 0);
  gtk_ui_manager_add_ui_from_file(uimanager, UI_DIR "/menu.ui", NULL);

  menubar = gtk_ui_manager_get_widget(uimanager, "/MenuBar");
  gtk_window_add_accel_group(GTK_WINDOW(editor_window),
                             gtk_ui_manager_get_accel_group(uimanager));
    
  return menubar;
}

/*EOF*/
