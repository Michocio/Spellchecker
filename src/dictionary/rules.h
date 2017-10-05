/** @file
  Interfejs obsługujący tablice dynamiczną rules i typ rule

  @ingroup structers
  @author Michał Jaroń
  @copyright Uniwerstet Warszawski
  @date 2015-05-31
 */

#ifndef __RULES_H__
#define __RULES_H__

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include "dictionary.h"

/**
  Struktura reprezentująca regułe
  */
typedef struct rule {
	wchar_t * left, *right;///<Lewa i prawa część reguły
	int cost;///<Koszt
	int flag ;///<Flaga, przkonwertowana na ina w \ref dictionary_rule_add
	int var;///<1 - zawiera zmienną wolną
} rule;

///Dodaj nową regułę
rule new_rule(wchar_t * left,wchar_t * right,int way,int cost, int flag);

/**
  Struktura reprezentująca zbiór regul
  * Dynamiczna tablica przechowująca reguły
  */
typedef struct rules {
    rule **items;
    int size;///< aktualna liczba przechowywanych elementów
     rule * costs[100];///<Sensownym założeniem wydaję się, że koszt żadnej rguł← nie przekroczy 100
} rules;


/**@}*/
/** @name Elementy interfejsu rules
  @{
 */


/**
  Inicjalizacja tablicy reguł
  @param[in,out] rules * Tablica reguł do inicjalizacji.
  @param[in] rule * Pierwszy wstawiany element.
  */ 
void rules_init(rules *v, const rule * item);



/**
 * Zwraca słowo z zastosowaną regułą
 * Funkcja używana w testach
 */ 
wchar_t * edit(wchar_t *word,int index, rule make);


/**
  Inicjalizacja tablicy reguł
  @param[in,out] rules * Tablica reguł do inicjalizacji.
  @param[in] rule * Pierwszy wstawiany element.
  */ 
int rules_save(rules *v, FILE *stream);



/**
  Rozszerza dynamiczną tablicę przechowująca reguły
  @param[in,out] rules Tablic do poszerzenia.
  */ 
void rules_resize(rules *v);




/**
  Dodaje daną regułę do listy reguł
  @param[in,out] rules Tablica do którego wstawiamy.
  @param[in] wchar_t Wstawiana reguła.
  */ 
void rules_add(rules *v,const rule * item);


/**
  Zniszcz tablicę reguł
  @param[in,out] rules Tablica do usunięcia.
  */ 
void rules_done(rules *v);



/**
 * Czy dana reguła pasuje do danego suffixu
 */ 
rule * rule_match(rule * match, wchar_t * word, int index);

/**
 * Dla danego słowo tablicuje możliwe reguły dla kolejnych suffixów
 */ 
rules* fill_rules(rules v,wchar_t * word, int* size);

///Do testów
rules  fill_empty(rules v);

		
#endif
