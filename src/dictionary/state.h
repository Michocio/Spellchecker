/** @file
  Interfejs obsługujący strukture state

  @ingroup structers
  @author Michał Jaroń
  @copyright Uniwerstet Warszawski
  @date 2015-05-31
 */

#ifndef __STATE_H__
#define __STATE_H__

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include "dictionary.h"


typedef struct rule {
	wchar_t * left, *right;
	int way;
	int cost;
	int flag ;
} rule;


rule new_rule(wchar_t * left,wchar_t * right,int way,int cost, int flag);

/**
  Struktura reprezentująca zbiór regul
  * Dynamiczna tablica przechowująca reguły
  */
typedef struct rules {
    rule **items;
    int size;///< aktualna liczba przechowywanych elementów
    struct rules * costs[100];
} rules;


/**@}*/
/** @name Elementy interfejsu rules
  @{
 */


/**
  Inicjalizacja słownika
  @param[in,out] alphabet Alfabet do inicjalizacji.
  @param[in] wchar_t Pierwszy wstawiany element.
  */ 
void rules_init(rules *v, const rule * item);

int rules_save(rules *v, FILE *stream);
/**
  Rozszerza dynamiczną tablicę przechowująca alfabet
  @param[in,out] alphabet Alfabet do poszerzenia.
  */ 
void rules_resize(rules *v);
rules rules_load(FILE * stream);

/**
  Dodaje daną literę do alfabetu
  @param[in,out] alphabet Alfabet do którego wstawiamy.
  @param[in] wchar_t Wstawiana liter.
  */ 
void rules_add(rules *v,const rule * item);


/**
  Zniszcz alfabet
  @param[in,out] alphabet Alfabet do usunięcia.
  */ 
void rules_done(rules *v);



//int rules_find(rules *v,const rule letter);

int rule_match(rule * match, wchar_t * word, int index);


void fill_rules(rules *v,wchar_t * word,  rules ** good_rules);

#endif
