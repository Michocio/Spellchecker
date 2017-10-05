/** @file
  Interfejs obsługująca vector.

  @ingroup structers
  @author Michał Jaroń
  @copyright Uniwerstet Warszawski
  @date 2015-05-31
 */

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include "trie.h"

/**
  Struktura reprezentująca listę dzieci.
  * Dynamiczna tablica przechowująca wskaźniki na \ref node
  * Używana w \ref node.
  * Przechowuje dzieci węzła
  */
typedef struct vector {
	struct node **items;
    int size;///< aktualna liczba przechowywanych elementów
} vector;

typedef struct node node;
typedef node* tree;
/**@}*/
/** @name Elementy interfejsu vector
  @{
 */
 
/**
  Inicjalizacja vectora
  @param[in,out] vector Vector do inicjalizacji.
  @param[in] tree Pierwszy wstawiany element.
  */  
void vector_init(vector *v, tree item);


/**
  Rozszerza dynamiczną tablicę przechowująca vector
  @param[in,out] vector Vector do poszerzenia.
  */ 
void vector_resize(vector *v);


/**
  Zmniejsza rozmiar dynamicznej tablicy przechowującej węzły
  @param[in,out] vector Vector do pomniejszenia.
  */ 
void vector_resize_minus(vector *v);


/**
  Usuwa ze wskazanego wektora, węzeł przechowujący wskazaną literę
  @param[in,out] vector Wskazany Vector.
  @param[in] wchar_t Wskazana litera
  */ 
void vector_delete(vector *v,const wchar_t letter);


/**
 Szuka danej litery
  @param[in,out] vector Vector w którym szukamy.
  @param[in] wchar_t Wskazana litera
  @return adres węzła przechowującego wskazaną literę , NULL jeśli litera nie znajduję się w vectorze
  */ 
tree* vector_find(vector *v,const wchar_t letter);


/**
  Dodaje dany węzeł do vectora
  @param[in,out] vector Vektor do którego wstawiamy.
  @param[in] node Wstawiana węzeł.
  */ 
void vector_add(vector *v, node *item);


/**
  Zniszcz vector
  @param[in,out] vector Vektor do usunięcia.
  */ 
void vector_done(vector *v);


#endif
