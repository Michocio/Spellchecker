/** @file
  Interfejs obsługujący strukture states

  @ingroup structers
  @author Michał Jaroń
  @copyright Uniwerstet Warszawski
  @date 2015-06-29
 */

#ifndef __STATES_H__
#define __STATES_H__
//#include "avl.h"
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include "trie.h"

#include "dictionary.h"
#include "rules.h"

/**
  Struktura reprezentująca stan
  */
typedef struct state {
	int suffix;///<Index w słowie od ktorego poczatek suffixu
	tree pointer;///<Wskaźnik na wezeł
	int extend;
	tree first_word;///<Jeżeli reguła "s" to tutaj trzymaj uchwyt na pierwsze słowo
	int cost;
} state;

///Dodaj nowy stan
state * new_state(int suffix, tree ptr, tree first_word,int cost,int extend);

/**
 * Funkcja porównująca dwa stany.
 * Zwraca wartość wg. własnej relacji porzaðku na stanach.
 * @return 1 gdy o1>o2, 0 gdy o1==o2, -1 w p.p
 */ 
int comperator(state *o1, state *o2,int cost1,int cost2);
/**
  Struktura reprezentująca zbior stanow
  * Dynamiczna tablica przechowująca stany
  */
typedef struct layer{
    state **items;///<Zbiór stanów
    int size;///< aktualna liczba przechowywanych elementów
} layer;


/**
  Inicjalizacja tablicy stanów
  @param[in,out] layer * Tablica reguł do inicjalizacji.
  @param[in] state *  Pierwszy wstawiany element.
  */ 
void layer_init(layer *v, const state * item);


/**
  Dodaje dany stan do listy stanów
  @param[in,out] layer Tablica do którego wstawiamy.
  @param[in] state * Wstawiana reguła.
  */ 
void layer_add(layer *v,const state * item);


///Usuń dany stan
void state_delete(layer *v, const state * item);


///Znisz daną warstwę
void layer_done(layer *v);


///Poszerz tablicę dynamiczną ze stanami
void layer_resize(layer  *v);



/**
  Struktura reprezentująca warstwe
  * Dynamiczna tablica przechowująca warstwy o danym koszcie
  */
typedef struct layers{
    layer **items;///<Zbiór stanów
    int size;///< aktualna liczba przechowywanych elementów
} layers;

///Testy
void debugger(tree root);


/**
  Inicjalizacja tablicy warstw
  @param[in,out] layers * Tablica warstw do inicjalizacji.
  @param[in] layer*  Pierwszy wstawiany element.
  */ 
void layers_init(layers *v, const layer * item);


/**
  Dodaje daną warstwę do tablicy warstw
  @param[in,out] layers Tablica do którego wstawiamy.
  @param[in] layer* Wstawiana reguła.
  */ 
void layers_add(layers *v,const layer* item);

///Znisz warstwy
void layers_done(layers *v);

///Poszerz tablicę warstw
void layers_resize(layers *v);

#endif
