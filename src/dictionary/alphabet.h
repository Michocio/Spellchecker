/** @file
  Interfejs obsługująca alphabet.

  @ingroup structers
  @author Michał Jaroń
  @copyright Uniwerstet Warszawski
  @date 2015-05-31
 */

#ifndef __ALPHABET_H__
#define __ALPHABET_H__

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include "trie.h"

/**
  Struktura reprezentująca zbiór liter danego języka.
  * Dynamiczna tablica przechowująca znaki, bez powtórzeń
  */
typedef struct alphabet {
    wchar_t **items;
    int size;///< aktualna liczba przechowywanych elementów
} alphabet;


/**@}*/
/** @name Elementy interfejsu alphabet
  @{
 */


/**
  Inicjalizacja słownika
  @param[in,out] alphabet Alfabet do inicjalizacji.
  @param[in] wchar_t Pierwszy wstawiany element.
  */ 
void alphabet_init(alphabet *v, const wchar_t item);


/**
  Rozszerza dynamiczną tablicę przechowująca alfabet
  @param[in,out] alphabet Alfabet do poszerzenia.
  */ 
void alphabet_resize(alphabet *v);


/**
  Dodaje daną literę do alfabetu
  @param[in,out] alphabet Alfabet do którego wstawiamy.
  @param[in] wchar_t Wstawiana liter.
  */ 
void alphabet_add(alphabet *v, const wchar_t letter);


/**
  Zniszcz alfabet
  @param[in,out] alphabet Alfabet do usunięcia.
  */ 
void alphabet_done(alphabet *v);


/**
  Szukaj w alfabecie danej litery 
  @param[in] alphabet Alfabet w którym poszukiwanie.
  @param[in] wchar_t Szukana litera.
  @return 1 jeśli słowo w drzewie, -1 w p.p.
  */ 
int alphabet_find(alphabet *v,const wchar_t letter);



#endif
