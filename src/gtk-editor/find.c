/** @defgroup gtk-editor Moduł gtk-editor
   Graficzny interfejs spellchecker
  */
/** @file
    Implementacja funkcjonalności gtk-editor operujących na pliku oraz wstawiania słowa
    @ingroup gtk-editor
  */

#include <gtk/gtk.h>
#include <string.h>
#include <locale.h>
#include "editor.h"

GtkWidget *search_entry, *replace_entry, *add_entry;

void findGTK (GtkTextView *text_view, const gchar *text, GtkTextIter *iter) {
  GtkTextIter mstart, mend;
  GtkTextBuffer *buffer;
  GtkTextMark *last_pos;
  gboolean found;

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
  found = gtk_text_iter_forward_search(iter, text, 0, &mstart, &mend, NULL);

  last_pos = gtk_text_buffer_get_mark(buffer, "last_pos");
  if (found) {
    gtk_text_buffer_select_range(buffer, &mstart, &mend);
    if (last_pos)
      gtk_text_buffer_move_mark(buffer, last_pos, &mend);
    else
      gtk_text_buffer_create_mark(buffer, "last_pos", &mend, FALSE);
  }
  else {
    if (last_pos)
      gtk_text_buffer_delete_mark(buffer, last_pos);
    // Not found
  }
}

void replace (GtkTextView *text_view, const gchar *text,
              const gchar *text1, GtkTextIter *iter) {
  GtkTextIter mstart, mend;
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
  gboolean found;
  
  found = gtk_text_iter_forward_search (iter, text, 0, &mstart, &mend, NULL);
  if (found) {
    gtk_text_buffer_select_range(buffer, &mstart, &mend);
    gtk_text_buffer_create_mark(buffer, "last_pos", &mend, FALSE);
	
    int len = strlen(text1);
    gtk_text_buffer_delete(buffer, &mstart, &mend);
    gtk_text_buffer_insert(buffer, &mstart, text1, len);
  }
}

void win_destroy () {
  gtk_main_quit(); 
} 

void next_button_clicked (GtkWidget *next_button) {
  GtkTextBuffer *buffer;
  GtkTextMark *last_pos;
  GtkTextIter iter;

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor_view));
  last_pos = gtk_text_buffer_get_mark(buffer, "last_pos");
  
  if (last_pos == NULL) {
    GtkWidget *message = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
                                                GTK_MESSAGE_INFO,
                                                GTK_BUTTONS_CLOSE,
                                                "End of Search!!!!  \n\n");
      
    gtk_window_set_title(GTK_WINDOW(message), "Search");
    gtk_dialog_run(GTK_DIALOG(message));
    gtk_widget_destroy(message);
    return;
  }
  
  gtk_text_buffer_get_iter_at_mark(buffer, &iter, last_pos);
  findGTK(GTK_TEXT_VIEW(editor_view),
       gtk_entry_get_text(GTK_ENTRY(search_entry)),
       &iter);
}

void search_button_clicked (GtkWidget *search_button) {
  GtkTextBuffer *buffer;
  GtkTextIter iter;

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor_view));
  gtk_text_buffer_get_start_iter(buffer, &iter);
  
  findGTK(GTK_TEXT_VIEW(editor_view),
       gtk_entry_get_text(GTK_ENTRY(search_entry)),
       &iter);
}

void replace_button_clicked (GtkWidget *replace_button) {
  GtkTextBuffer *buffer;
  GtkTextIter iter;
 
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor_view));
  gtk_text_buffer_get_start_iter(buffer, &iter);
  
  replace(GTK_TEXT_VIEW(editor_view),
          (gchar *)gtk_entry_get_text(GTK_ENTRY(search_entry)),
          (gchar *)gtk_entry_get_text(GTK_ENTRY(replace_entry)),
          &iter);
}


/**
 * 
 * 
 */ 
void textfind () {
  GtkWidget *win;
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *search_button;
  GtkWidget *next_button;

  win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  vbox = gtk_vbox_new(FALSE, 2);
  gtk_container_add(GTK_CONTAINER(win), vbox);

  hbox = gtk_hbox_new(FALSE, 2);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
  
  search_entry = gtk_entry_new();
  
  gtk_box_pack_start(GTK_BOX(hbox), search_entry, TRUE, TRUE, 0);

  search_button = gtk_button_new_with_label("Search");
  gtk_box_pack_start(GTK_BOX(hbox), search_button, FALSE, FALSE, 0);
  g_signal_connect(G_OBJECT(search_button), "clicked",
                   G_CALLBACK(search_button_clicked), NULL);
  
  next_button = gtk_button_new_with_label("Next");
  gtk_box_pack_start(GTK_BOX(hbox), next_button, FALSE, FALSE, 0);
  g_signal_connect(G_OBJECT(next_button), "clicked",
                   G_CALLBACK(next_button_clicked), NULL);

  gtk_widget_show_all(win);
}

///Obłsługa zaniechania działań w widżecie
void abort_button_clicked (GtkWidget *add_button,GtkWidget *win)
{
	
	gtk_widget_destroy(win);
}

/**
 * 
 * Obłsuga od podszewski dodaniwa nowego słowa
 * 
 */ 
void add_button_clicked (GtkWidget *add_button,GtkWidget *win) {
	
	 GtkWidget *dialog,*dialog2;
	GtkTextBuffer *buffer;
  GtkTextIter iter,end,start;
  gunichar *wword;

   setlocale(LC_ALL, "pl_PL.UTF-8");
 char * text = (char *)gtk_entry_get_text(GTK_ENTRY(add_entry));

	
	 wword = g_utf8_to_ucs4_fast(text, -1, NULL);
	
	  lowercase((wchar_t *)wword);
	
	 
	
	 if( dictionary_insert((dict),(wchar_t *)wword)==1)
	 {
	 
	  dialog = gtk_message_dialog_new(NULL, 0, GTK_MESSAGE_ERROR,
                                    GTK_BUTTONS_OK,
                                    "Wstawiono do słownika %ls", wword);      
								}
								else
								{
									 dialog = gtk_message_dialog_new(NULL, 0, GTK_MESSAGE_ERROR,
                                    GTK_BUTTONS_OK,
                                    "Nie udało się wstawić słowa. Słowo %ls może być już w słowniku ", wword);   
								}
    gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	gtk_widget_destroy(win);
	
}


/**
 * Obsługa dodawania nowego słowa do słownika
 * [in] gunichar word Słowo do wstawienia
 */ 
void add_word(gunichar * word)
{
	GtkWidget *dialog,*dialog2;
	 GtkWidget *win;
	 
	  
	  
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *add_button, *abort_button;

  word[wcslen(word)]='\0';
  win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  vbox = gtk_vbox_new(FALSE, 2);
  gtk_container_add(GTK_CONTAINER(win), vbox);

  hbox = gtk_hbox_new(FALSE, 2);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
  
  add_entry = gtk_entry_new();
	//lowercase(word);
	gtk_entry_set_text (add_entry,word);
	 GtkWidget* label = gtk_label_new("Wstaw do słownika słowo:");
  
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
  //gtk_container_add(GTK_CONTAINER(window), label);
  gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
	
	
  gtk_box_pack_start(GTK_BOX(hbox), add_entry, TRUE, TRUE, 0);

  add_button = gtk_button_new_with_label("Dodaj");
  abort_button = gtk_button_new_with_label("Anuluj");
  
  
  gtk_box_pack_start(GTK_BOX(hbox), add_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), abort_button, FALSE, FALSE, 0);
    
  g_signal_connect(G_OBJECT(add_button), "clicked",
                   G_CALLBACK(add_button_clicked), win);
                   
			
   g_signal_connect(G_OBJECT(abort_button), "clicked",
                   G_CALLBACK(abort_button_clicked), win);
 
  gtk_widget_show_all(win);


}

void text_find_replace () {
  GtkWidget *win;
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *search_button;
  GtkWidget *replace_button;  

  win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  vbox = gtk_vbox_new(FALSE, 2);
  gtk_container_add(GTK_CONTAINER (win), vbox);

  hbox = gtk_hbox_new(FALSE, 2);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
  
  search_entry = gtk_entry_new();
  replace_entry = gtk_entry_new();

  gtk_box_pack_start(GTK_BOX(hbox),search_entry, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(hbox),replace_entry, TRUE, TRUE, 0);

  search_button = gtk_button_new_with_label("Search");
  gtk_box_pack_start(GTK_BOX(hbox), search_button, FALSE, FALSE, 0);
  g_signal_connect(G_OBJECT (search_button), "clicked",
                   G_CALLBACK (search_button_clicked), NULL);
  
  replace_button = gtk_button_new_with_label("Replace");
  gtk_box_pack_start(GTK_BOX(hbox), replace_button, FALSE, FALSE, 0);
  g_signal_connect(G_OBJECT(replace_button), "clicked",
                   G_CALLBACK(replace_button_clicked), NULL);

  gtk_widget_show_all(win);
}

/*EOF*/
