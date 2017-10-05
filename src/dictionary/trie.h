/** @defgroup structers Moduł structers
    Struktury danych obsługujące \ref dictionary
  */

/** @file
  Interfejs obsługująca drzewo.

  @ingroup structers
  @author Michał Jaroń
  @copyright Uniwerstet Warszawski
  @date 2015-05-31
 */

#ifndef __TRIE_H__
#define __TRIE_H__

#include <wchar.h>
#include "vector.h"
#include "alphabet.h"
#include "states.h"
/**
  Struktura reprezentująca węzeł drzewa Trie
  */
typedef struct node {
	struct vector * children; ///< dynamiczna tablica przechowującą dzieci \ref vector
	wchar_t letter;///< litera przechowywana w danym węźle
	struct node* parent;///< wskaznik typu \ref node* na rodzica węzła
	int word;///< 1 gdy dany wezel jest końcem słowa, 0 w p.p
}node;


typedef node* tree; ///<Wskaźnik na węzeł \ref node


/**@}*/
/** @name Elementy interfejsu node
  @{
 */

/**
  Usuwa z drzewa wskazane słowo
  @param[in,out] tree Korzeń drzewa.
  @param[in] word Słowo do usunięcia.
  @return 0 jeśli słowo w drzewie, -1 w p.p.
  */
int node_delete(tree * root, const wchar_t * word);


/**
  Inicjalizuje wezel/drzewo
  @param[in,out] tree wskźnik na węzeł.
  @param[in] word Rodzic inicjalizowanego węzła.
  */
void trie_init(tree* root,const tree parent,const wchar_t letter);


/**
  Usuwa drzewo
  @param[in,out] tree Korzeń drzewa.
  */
void trie_clear(tree* root);


tree find(const wchar_t * word,const tree root,int i,int len);

/**
  Szuka w drzewie danego słowa
  @param[in] word Szukane słowo.
  @param[in] tree Korzeń drzewa.
  @return adres węzła końcowego słowa, jeśli słowo w drzewie, NULL w p.p.
  */
int trie_find(const wchar_t * word,const tree root);

tree find_prefix(const wchar_t * word,const tree root,int i,int len,wchar_t letter);
/**
  Wstawia do drzewa dane słowo oraz litery z danego słowa do alfabetu danego jezyka
  @param[in] word Wstawiane słowo.
  @param[in,out] tree Korzeń drzewa.
  @return -1 jeśli słowo w drzewie, 1 w p.p.
  */
int trie_insert(const wchar_t * word,tree* root);


/**
  Zapisuje do pliku drzewo
  @param[in] word Korzeń drzewa.
  @param[in,out] FILE Wskazany plik.
  @return -1 jeśli bład zapisu, 1 w p.p.
  */
int toString(const tree node,FILE* stream);


/**
  Ładuje z podanego pliku drzewo oraz uzupełnia alfabet języka
  @param[in] FILE Plik do czytania.
  @return drzewo jeśli sukces wczytania, NULL w p.p.
  */
tree trie_load(FILE* stream);

/** @name Funkcje pomocnicze
  @{
 */

/**
  Procedura do debugowania.
  Wypisuje strukutre drzewa
  @param[in] tree Korzeń drzewo do wyświetlenia
  */
void debug(tree root);

#endif
