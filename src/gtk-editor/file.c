#include <gtk/gtk.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "editor.h"

#define CHAR_BUF 2000

char *filename;

// Load the file in the UTF-8 format else error

void load_file (char *fname) {
  FILE *f; 
  GtkTextIter p;
  char fbuf[CHAR_BUF];
  size_t l;
    
  if (!fname) {
    // Create file selection dialog
    GtkWidget *dialog =
      gtk_file_chooser_dialog_new("Open File...", NULL,
                                  GTK_FILE_CHOOSER_ACTION_OPEN,
                                  GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
    // Show the file selection dialog
    int resp = gtk_dialog_run(GTK_DIALOG(dialog));

    if (resp == GTK_RESPONSE_OK) {   
      // fname contains the file selected by the user
      fname = g_strdup(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
      gtk_widget_destroy(dialog);
    } 
    else {  // cancel
      gtk_widget_destroy(dialog);
      return;
    }
  }

  if (!(f = fopen(fname, "r"))) {
    // If file cannot be opened
    g_printerr("%s: %s\n", fname, g_strerror(errno));
    return;
  }

  if (fname != filename) {
    gchar *wt = g_strdup_printf("Text Editor (%s)", fname);

    g_free(filename);
    filename = fname;
    // Set titlebar to the file name
    gtk_window_set_title(GTK_WINDOW(editor_window), wt);
    g_free(wt);
  }

  // Get the ending iterator of buffer into p
  gtk_text_buffer_get_end_iter(editor_buf, &p);

  while ((l = fread(fbuf, 1, sizeof(fbuf), f)) > 0) {
    // Only text files in Utf-8 format can be opened
    
    GError *err = NULL;
    gsize br, bw;
    gchar *text;
    if (!(text = g_locale_to_utf8(fbuf, l, &br, &bw, &err))) {
      g_printerr("File is not in UTF-8 format : %s\n", err->message);
      g_clear_error(&err);
      filename = NULL; // if an invalid file is opened enter the file name as NULL
      gtk_window_set_title(GTK_WINDOW(editor_window),
                           "Text Editor (Insert file name)");
      fclose(f);

      return;
    }
    gtk_text_buffer_insert(editor_buf, &p, text, bw);
    g_free(text);
  }

  gtk_text_buffer_set_modified(editor_buf, FALSE);
    
  gtk_text_buffer_get_start_iter(editor_buf, &p);
  gtk_text_buffer_place_cursor(editor_buf, &p);
  if (ferror(f)) {
    g_printerr("%s: %s\n", fname, g_strerror(errno));
    fclose(f);
    return;
  }
  if (fclose(f) == EOF)
    g_printerr("%s: %s\n", fname, g_strerror(errno));
} 

// Save file dialog box

gboolean save_file (char *fname) {
  FILE *f;
  int ok = TRUE;
    
  if (!fname) {
    GtkWidget *dialog =
      gtk_file_chooser_dialog_new("Save File As...", NULL,
                                  GTK_FILE_CHOOSER_ACTION_SAVE,
                                  GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);
    int resp = gtk_dialog_run(GTK_DIALOG(dialog));

    if (resp == GTK_RESPONSE_ACCEPT) {
      fname =
        g_strdup(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
      gtk_widget_destroy(dialog);
    }
    else {
      gtk_widget_destroy(dialog);
      return FALSE;
    }
  }
  if (!(f = fopen(fname, "w"))) {
    // Error opening file
    g_printerr("%s: %s\n", fname, g_strerror(errno));
    ok = FALSE;
  }
  else {
    GtkTextIter start, end, p;

    // Get the starting and ending position
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(editor_buf), &start, &end);
    p = start;
    while (!gtk_text_iter_equal(&start, &end)) {
      gchar *buf, *fbuf;
      gsize br, bw;
      GError *err = NULL;

      gtk_text_iter_forward_chars(&p, CHAR_BUF);
      buf = gtk_text_iter_get_slice(&start, &p);
      fbuf = g_locale_from_utf8(buf, -1, &br, &bw, &err);
      g_free(buf);

      if (!fbuf) {
        g_printerr("Failed UTF-8 to locale conversion: %s\n",
                   err->message);
        g_clear_error(&err);
        ok = FALSE;
        break;
      }
      fwrite(fbuf, bw, 1, f);
      g_free(fbuf);
      if (ferror(f)) {
        g_printerr("%s: %s\n", fname, g_strerror(errno));
        ok = FALSE;
        break;
      }
      start = p;
    }
    if (fclose(f) == EOF) {
      g_printerr("%s: %s\n", fname, g_strerror(errno));
      ok = FALSE;
    }
  }
    
  if (ok) {
    gtk_text_buffer_set_modified(editor_buf, FALSE);
    if (fname != filename) {
      gchar *wt = g_strdup_printf("TextView (%s)", fname);

      g_free(filename);
      filename = fname;
      gtk_window_set_title(GTK_WINDOW(editor_window), wt);
      g_free(wt);
    }
  }
  return ok;
}

// If the buffer  is modified then prompt the user to save if he
// quits the application or loads a new file

gboolean save_if_modified () {
  int resp;
  GtkWidget *dialog;

  if (!gtk_text_buffer_get_modified(GTK_TEXT_BUFFER(editor_buf)))
    return TRUE;

  dialog = gtk_message_dialog_new(GTK_WINDOW(editor_window), 0,
                                  GTK_MESSAGE_QUESTION,
                                  GTK_BUTTONS_NONE,
                                  "Buffer Modified. Do you want to save it now?");
  gtk_dialog_add_buttons(GTK_DIALOG(dialog), GTK_STOCK_YES, GTK_RESPONSE_YES,
                         GTK_STOCK_NO, GTK_RESPONSE_NO,
                         GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
  resp = gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
  switch (resp) {
  case GTK_RESPONSE_YES:
    return save_file(filename);
  case GTK_RESPONSE_NO:
    return TRUE;
  case GTK_RESPONSE_CANCEL:
  case GTK_RESPONSE_DELETE_EVENT:
    return FALSE;
  default:
    g_printerr("Error response %d from dialog\n", resp);
    return FALSE;
  }
}

/*EOF*/
