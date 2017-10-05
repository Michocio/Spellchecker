/** @file
  Implementacja listy słów.

  @ingroup dictionary
  @author Jakub Pawlewicz <pan@mimuw.edu.pl>
  @copyright Uniwerstet Warszawski
  @date 2015-05-10
 */

#include "word_list.h"

/** @name Elementy interfejsu 
   @{
 */

void word_list_done(struct word_list *list)
{
	return 0;
}

void word_list_sort(struct word_list *list)
{//Proste sortowanie bąbelkowe
for(int j = word_list_size(list) - 1; j > 0; j--)
  {
    int p = 1;
    for(int i = 0; i < j; i++)
      if(wcscoll(list->array[i],list->array[i + 1])>0)
      {
		wchar_t *  pomoc= list->array[i];
        list->array[i]=list->array[i+1];
        list->array[i+1]=pomoc;
        p = 0;
      }
    if(p) break;
  }
}


void word_list_init(struct word_list *list)
{
    list->size = 0;
    list->buffer_size = 0;
}


int word_list_add(struct word_list *list, const wchar_t *word)
{
	if(word!=NULL)
	{
    if (list->size >= WORD_LIST_MAX_WORDS)
        return 0;
    size_t len = wcslen(word) + 1;
    if (list->buffer_size + len > WORD_LIST_SUM)
        return 0;
    wchar_t *pos = list->buffer + list->buffer_size;
    list->array[list->size++] = pos;
    wcscpy(pos, word);
    list->buffer_size += len;
    return 1;
	}
}

/**@}*/
