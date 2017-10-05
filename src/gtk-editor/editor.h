
/** @file
   Deklaracje funkcjonalności gtk-editor operujących na pliku oraz wstawiania słowa
    @ingroup gtk-editor
  */

#include "dictionary.h"

#ifndef __EDITOR_H__
#define __EDITOR_H__

GtkWidget *editor_window;   // Main Gtk window
GtkWidget *editor_view;     // Text view widget where all the typing is done
GtkTextBuffer *editor_buf;  // Textview buffer

char *filename;

void show_about (void);
void show_help (void);
void extend_menu (GtkWidget *menubar);

void load_file (char *fname);
gboolean save_file(char *fname);
gboolean save_if_modified (void);

void textfind (void);

/**Dodaj słowo do słownika
*[in] gunichar *word słowo do wstawienia
*/ 
void add_word(gunichar *word);
void text_find_replace (void);

GtkWidget* create_menu (GtkAccelGroup *accel);
extern struct dictionary *dict;///<Globalny uchwyt do słownika
extern int loaded;
#endif /* __EDITOR_H__ */
