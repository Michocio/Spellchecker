/** @file
  Implementacja funkcjonalności editora
  *
  @ingroup gtk-editor
 */


#include <gtk/gtk.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "editor.h"
#include "word_list.h"
#include "dictionary.h"

#include <unistd.h>



GtkWidget *add_button, *abort_button, *add_entry;


void show_about () {
  GtkWidget *dialog = gtk_about_dialog_new();

  gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "Text Editor");
  //gtk_window_set_title(GTK_WINDOW(dialog), "About Text Editor");

  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog),
     "Text Editor for IPP exercises\n");

  gtk_dialog_run(GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
}

void show_help (void) {
  GtkWidget *help_window;
  GtkWidget *label;
  char help[5000];

  help_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW (help_window), "Help - Text Editor");
  gtk_window_set_default_size(GTK_WINDOW(help_window), 300, 300);

  strcpy(help,
         "\nAby podłączyć usługę spell-checkera do programu trzeba:\n\n"
         "Dołączyć ją do menu 'Spell' w menubar.\n\n"
         "Pobrać zawartość bufora tekstu z edytora: całą lub fragment,\n"
         "  zapamiętując pozycję.\n\n");
  strcat(help, "\0");

  label = gtk_label_new(help);

  gtk_container_add(GTK_CONTAINER(help_window), label);

  gtk_widget_show_all(help_window);
}

// Zaślepeczki słownika (wchar_t i gunichar to prawie to samo)
//
// Oczywiście do zastąpienia prawdziwymi funkcjami
/*
struct dictionary {
  int foo;
} dict;

*/



///Procedura obsługi modułów spellcheckera w editorze gtk
static void WhatCheck (GtkMenuItem *item, gpointer data) {
 GtkWidget *dialog,*dialog2;
 if(loaded!=0)
 {
 
  
  GtkTextIter start, end;
  char *word;
  gunichar *wword;
 


  // Znajdujemy pozycję kursora
  gtk_text_buffer_get_iter_at_mark(editor_buf, &start,
                                   gtk_text_buffer_get_insert(editor_buf));

  // Jeśli nie wewnątrz słowa, kończymy
  if (!gtk_text_iter_inside_word(&start)) {
    dialog = gtk_message_dialog_new(NULL, 0, GTK_MESSAGE_ERROR,
                                    GTK_BUTTONS_OK,
                                    "Kursor musi być w środku słowa");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }

  // Znajdujemy początek i koniec słowa, a potem samo słowo
  end = start;
  gtk_text_iter_backward_word_start(&start);
  gtk_text_iter_forward_word_end(&end);
  word = gtk_text_iter_get_text(&start, &end);
	
	
  // Zamieniamy na wide char (no prawie)
  wword = g_utf8_to_ucs4_fast(word, -1, NULL);

  // Sprawdzamy
  int result=dictionary_find(dict,(wchar_t *)wword);
  if (result==1) {//Jeżeli słowo odnalezione w słowniku
   dialog = gtk_message_dialog_new(NULL, 0, GTK_MESSAGE_ERROR,
                                    GTK_BUTTONS_OK,
                                    "Wyraz poprawny");
    gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
  }
  else {
    // Czas korekty
    GtkWidget *vbox, *label, *combo;
    struct word_list hints;
    int i;
    wchar_t **words;


	//Wygeneruj podpowiedzi dla danego słowa
    dictionary_hints(dict, (wchar_t *)wword, &hints);
    words = word_list_get(&hints);

    dialog =gtk_dialog_new_with_buttons("Korekta", NULL, 0,
                                         GTK_STOCK_OK,
                                         GTK_RESPONSE_ACCEPT,
                                         GTK_STOCK_CANCEL,
                                         GTK_RESPONSE_REJECT,
                                         NULL);
                                         
                                         
    //Button z propozycją dodania nowego słowo do słownika                                     
	GtkWidget* add_button =    gtk_dialog_add_button (dialog,
                       "Dodaj",
                        GTK_RESPONSE_APPLY);                            
                                

    // W treści dialogu dwa elementy
    vbox = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    // Tekst
    label = gtk_label_new("Coś nie tak, mam kilka propozycji");
    gtk_widget_show(label);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);

    // Spuszczane menu
    combo = gtk_combo_box_text_new();

    for (i = 0; i < word_list_size(&hints); i++) {
      // Combo box lubi mieć Gtk
      char *uword = g_ucs4_to_utf8((gunichar *)words[i], -1, NULL, NULL, NULL);

      // Dodajemy kolejny element
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), uword);
      g_free(uword);
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
    gtk_box_pack_start(GTK_BOX(vbox), combo, FALSE, FALSE, 1);
    gtk_widget_show(combo);


	gint response=  gtk_dialog_run(GTK_DIALOG(dialog));
    if ((response == GTK_RESPONSE_ACCEPT)&&(word_list_size(&hints)!=0)) {
      char *korekta =
        gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo));

      // Usuwamy stare
      gtk_text_buffer_delete(editor_buf, &start, &end);
      // Wstawiamy nowe
      gtk_text_buffer_insert(editor_buf, &start, korekta, -1);
      g_free(korekta);
      gtk_widget_destroy(dialog);
    }
    else if (response ==   GTK_RESPONSE_APPLY)
    {//Jeżeli użytkownik chce dodać słowo do słownika
		add_word(word);
		gtk_widget_destroy(dialog);
	}
	else//Nie rób nic
		gtk_widget_destroy(dialog);


  }
   g_free(word);
  g_free(wword);
}
else
{//Niewczytany słownik
	dialog = gtk_message_dialog_new(NULL, 0, GTK_MESSAGE_ERROR,
                                    GTK_BUTTONS_OK,
                                    "Wczytaj najpierw słownik!");
    gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	
}
 
}


/**
 * Sprawdza poprawnośc i ew. koloruje na czerowno
 * niepoprawne wyrazy. 
 */ 
static void kolejne (GtkWidget *widget) {
	
	
	
	if(loaded!=0)//Niepusty slownik
 {
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor_view));
	GtkTextIter start, end;//Iterator do wyłapywania wyrazów
	gunichar *wword;
	
	//Dodaj dwie opcje kolorowania
	
	//Czerwony kolor
	gtk_text_buffer_create_tag(buffer, "red_fg",
                             "foreground", "red",
                             "weight", PANGO_WEIGHT_BOLD, NULL);
     
     
   //Normalne, czarne litery                        
	gtk_text_buffer_create_tag(buffer, "black_fg",
                             "foreground", "black",
                             "weight", PANGO_WEIGHT_BOLD, NULL);

  int i;

	gtk_text_buffer_get_start_iter(buffer, &end);
	
	//Przegladaj wszyskie wyraz w editorze
	while(!gtk_text_iter_is_end(&end)) {
    char *word;

    gtk_text_iter_forward_word_end(&end);
    start = end;
    gtk_text_iter_backward_word_start(&start);
    word = gtk_text_iter_get_text(&start, &end);
    // Zamieniamy na wide char
	wword = g_utf8_to_ucs4_fast(word, -1, NULL);
	lowercase((wchar_t *)wword);//Małe litery
	
	//Szukaj słowo w słowniku
	int result=dictionary_find(dict,(wchar_t *)wword);
	if (result!=1){//Nie znalezione - koloruj na czerwono
    // Usuwamy stare
      gtk_text_buffer_delete(editor_buf, &start, &end);
      // Wstawiamy nowe
  
	//Czerwony kolor
    gtk_text_buffer_insert_with_tags_by_name(buffer, &end, word, -1,
                                         "red_fg",  NULL);
    }
    else
	{//Słowo w słowniku, kolor czarny
		
		
		 // Usuwamy stare
      gtk_text_buffer_delete(editor_buf, &start, &end);
      // Wstawiamy nowe
    gtk_text_buffer_insert(buffer, &end, word, -1);
	}
	
	
	
    g_print("%s\n", word);
    g_free(word);
    g_free(wword);
  }
}
else
{
		GtkWidget *dialog = gtk_message_dialog_new(NULL, 0, GTK_MESSAGE_ERROR,
                                    GTK_BUTTONS_OK,
                                    "Wczytaj najpierw słownik!");
    gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	
	
}
}

/**
 * Obługa wczytywania języka
 * 
 */ 
static void load_language2 (GtkWidget *widget)
{
	 GtkWidget *dialog,*dialog2;
	char** lista;
	char *tab;
	int size;
	int ile=0;
	//Pobierz listę języków dostępnych
	dictionary_lang_list(&tab,&size);
	lista=lang_list( tab, size);//Teraz zamien liste na vektor z kolejnymi nazwami języków

if(size>0)	{
	GtkWidget *vbox, *label, *combo;
    struct word_list hints;
    int i;
    wchar_t **words;

    words = word_list_get(&hints);


	//Okienko wyboru języka
    dialog =gtk_dialog_new_with_buttons("Wybór słownika", NULL, 0,
                                         GTK_STOCK_OK,
                                         GTK_RESPONSE_ACCEPT,
                                         GTK_STOCK_CANCEL,
                                         GTK_RESPONSE_REJECT,
                                         NULL);
                                         
                          
                                
	//Ile slownikow na liscie
	for(int i=0;i<size;i++)
	{
		if(*(tab+i)=='\0') ile++;
		
	}
 
    vbox = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    // Tekst
    label = gtk_label_new("Lista dostępnych słowników");
    gtk_widget_show(label);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);

    // Spuszczane menu
    combo = gtk_combo_box_text_new();
    for (i = 0; i < ile; i++) {


      // Dodajemy kolejny element
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), lista[i]);
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
    gtk_box_pack_start(GTK_BOX(vbox), combo, FALSE, FALSE, 1);
    gtk_widget_show(combo);

	
	gint response=  gtk_dialog_run(GTK_DIALOG(dialog));
    if ((response == GTK_RESPONSE_ACCEPT)) {
      char *slownik =
        gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo));

		dict=dictionary_load_lang(slownik);
		loaded=1;
      gtk_widget_destroy(dialog);
    }
	else
		gtk_widget_destroy(dialog);
	
}
else
{//Pusta lista słowników
	dialog = gtk_message_dialog_new(NULL, 0, GTK_MESSAGE_ERROR,
                                    GTK_BUTTONS_OK,
                                    "Brak dostępnych słowników do wczytania!");
    gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}	
}







/**
 * Obsługa anulowania działan w widżecie
 */ 

void abort_button_clicked2 (GtkWidget *add_button,GtkWidget *win)
{
	
	gtk_widget_destroy(win);
}


/**
 * Obsługa wciśnięcia buttona zapisującego słownik
 */ 
void savebutton(GtkWidget *add_button,GtkWidget *win)
{
	char * text = (char *)gtk_entry_get_text(GTK_ENTRY(add_entry));
	dictionary_save_lang(dict,text);
	gtk_widget_destroy(win);
}



/**
 * Obługa zapisu słownika z poprawkami użytkownika
 */ 
static void save_language2(GtkWidget *widget)
{
	GtkWidget *dialog,*dialog2;
	 GtkWidget *win;
	
  GtkWidget *vbox;
  GtkWidget *hbox;
 
  win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  vbox = gtk_vbox_new(FALSE, 2);
  gtk_container_add(GTK_CONTAINER(win), vbox);

  hbox = gtk_hbox_new(FALSE, 2);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
  
  add_entry = gtk_entry_new();

	 GtkWidget* label = gtk_label_new("Zapisz słownik pod nazwą:");
  
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
  gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
	
	
  gtk_box_pack_start(GTK_BOX(hbox), add_entry, TRUE, TRUE, 0);

  add_button = gtk_button_new_with_label("Zapisz");
  abort_button = gtk_button_new_with_label("Anuluj");
  
  
  gtk_box_pack_start(GTK_BOX(hbox), add_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), abort_button, FALSE, FALSE, 0);
    
  g_signal_connect(G_OBJECT(add_button), "clicked",
                   G_CALLBACK(savebutton), win);
                   
			
   g_signal_connect(G_OBJECT(abort_button), "clicked",
                   G_CALLBACK(abort_button_clicked2), win);
 
  gtk_widget_show_all(win);
}

void extend_menu (GtkWidget *menubar) {
  GtkWidget *spell_menu_item, *spell_menu, *check_item, *show_errors, *load_language, *save_language;

  spell_menu_item = gtk_menu_item_new_with_label("Spell");
  spell_menu = gtk_menu_new();
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(spell_menu_item), spell_menu);
  gtk_widget_show(spell_menu_item);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), spell_menu_item);

  check_item = gtk_menu_item_new_with_label("Check Word");
  g_signal_connect(G_OBJECT(check_item), "activate",
                   G_CALLBACK(WhatCheck), NULL);


                 show_errors = gtk_menu_item_new_with_label("Show errors");
load_language = gtk_menu_item_new_with_label("Load language");
save_language = gtk_menu_item_new_with_label("Save Language");

  g_signal_connect(G_OBJECT(show_errors), "activate",
                   G_CALLBACK(kolejne), NULL);
                   
                   
    g_signal_connect(G_OBJECT(load_language), "activate",
                   G_CALLBACK(load_language2), NULL);
                   
     g_signal_connect(G_OBJECT(save_language), "activate",
                   G_CALLBACK(save_language2), NULL);                               

  gtk_menu_shell_append(GTK_MENU_SHELL(spell_menu), check_item);
   gtk_menu_shell_append(GTK_MENU_SHELL(spell_menu), show_errors);
     gtk_menu_shell_append(GTK_MENU_SHELL(spell_menu), load_language);
       gtk_menu_shell_append(GTK_MENU_SHELL(spell_menu), save_language);
  gtk_widget_show(check_item);
}

/*EOF*/
