/** @file
    Interfejs listy słów.

    @ingroup dictionary
    @author Jakub Pawlewicz <pan@mimuw.edu.pl>
    @copyright Uniwerstet Warszawski
    @date 2015-05-10
    @todo Poszerzyć implementację, aby można było trzymać dowolną
      liczbę słów.
 */

#ifndef __WORD_LIST_H__
#define __WORD_LIST_H__

#include <wchar.h>

/**
  Maksymalna liczba słów przechowywana w liście słów.
  */
#define WORD_LIST_MAX_WORDS 1150


/**
  Łączna liczba znaków słów przechowywanych w liście słów
  włącznie ze znakami \0 kończącymi słowo.
  */
#define WORD_LIST_SUM 8024

/**
  Struktura przechowująca listę słów.
  Należy używać funkcji operujących na strukturze,
  gdyż jej implementacja może się zmienić.
  */
struct word_list
{
    /// Liczba słów.
    size_t size;
    /// Łączna liczba znaków.
    size_t buffer_size;
    /// Tablica słów.
    const wchar_t *array[WORD_LIST_MAX_WORDS];
    /// Bufor, w którym pamiętane są słowa.
    wchar_t buffer[WORD_LIST_SUM];
};

/**
  Inicjuje listę słów.
  @param[in,out] list Lista słów.
  */
void word_list_init(struct word_list *list);

/**
  Sortuje alfabetycznie listę
  @param[in,out] list Lista słów.
  */
void word_list_sort(struct word_list *list);
/**
  Destrukcja listy słów.
  @param[in,out] list Lista słów.
  */
void word_list_done(struct word_list *list);

/**
  Dodaje słowo do listy.
  @param[in,out] list Lista słów.
  @param[in] word Dodawane słowo.
  @return 1 jeśli się udało, 0 w p.p.
  */
int word_list_add(struct word_list *list, const wchar_t *word);

/**
  Zwraca liczę słów w liście.
  @param[in] list Lista słów.
  @return Liczba słów w liście.
  */
static inline
size_t word_list_size(const struct word_list *list)
{
    return list->size;
}

/**
  Zwraca tablicę słów w liście.
  @param[in] list Lista słów.
  @return Tablica słów.
  */
static inline
const wchar_t * const * word_list_get(const struct word_list *list)
{
    return list->array;
}

#endif /* __WORD_LIST_H__ */
