/** @file
  Implementacja struktury vector.

  @ingroup structers
  @author Michał Jaroń
  @copyright Uniwerstet Warszawski
  @date 2015-05-31
 */

#include <stdio.h>
#include <wchar.h>
#include "trie.h"
#include "vector.h"
#include <stdlib.h>


/**@}*/
/** @name Elementy interfejsu vector
  @{
 */
 
 
void vector_init(vector *v, tree item)
{
    (v)->items = malloc(sizeof(node *));//Przydziel pamięc
	(v)->items[0] = item;//"Włóż" pierwszy element
	(v)->size = 1;//Ustal rozmiar alfabetu
}


void vector_resize(vector *v)
{
    node **items = realloc(v->items, sizeof(node *)*(v->size+1));//Powiększ tablicę dynamiczną
    if (items) {//Jeżeli się udało
        v->items = items;//Przepisz wskaźnik
        v->size++;//Aktualizuj rozmiar tablicy
    }
}


void vector_resize_minus(vector *v)
{//Zmniejsza tablicę dynamiczną
    node **items = realloc(v->items, sizeof(node *)*(v->size-1));//Zmniejsz tablicę dynamiczną
        v->items = items;//Przepisz wskaźnik
        v->size--;//Aktualizuj rozmiar tablicy
}


void vector_delete(vector *v,const wchar_t letter)
{

	int i=0;
	while(v->items[i]->letter!=letter)//Szukaj miejsca gdzie jest litera
		i++;

	tree wsk;
	wsk=v->items[i];//Nie tracę uchwytu

	if(v->size>1)//Więcej niż jeden element w wektorze
	{
		while(i<(v->size-1))
		{

			v->items[i]=v->items[i+1];
			i++;
		}
	}

	vector_resize_minus(v);
	//free(wsk->children->items);
	free(wsk->children);
	free(wsk);


}


tree* vector_find(vector *v,const wchar_t letter)
{//Wyszukiwanie binarne
	int srodek=0;
	int poczatek = 0;
	if(v->size>0)
	{
		int koniec = v->size-1;

		while (poczatek < koniec)
		{
			srodek = (poczatek + koniec + 1) / 2;           // dzielenie przez 2 z zaokrągleniem w górę
			
			wchar_t * l1 = v->items[srodek]->letter;
			wchar_t *l2 = letter;
			if (wcscoll(&l1,&l2)<=0)	
				poczatek = srodek;
			else
				koniec = srodek - 1;
		}

		if(v->items[koniec]->letter==letter)//Czy na pewno znaleziony?
			return v->items[koniec];//Tak
		else return NULL;//Nie
	}
	else return NULL;//Przekazany vektor pusty	
}


void vector_add(vector *v, node *item)
{
	vector * pomoc;
		//printf("NUMER:%d, %lc\n",&item->letter);
	if(v->size==0)//Vektor pusty
		vector_init(v,item);

	else
	{
		vector_resize(v);
		int i=0;
	
		v->items[v->size-1] = item;//Wstaw na koniec
		i=v->size-2;

		while((i>=0)&&(wcscoll(&v->items[i]->letter,&item->letter)>0))//Iteruj od konca poszukujac miejsca wstawienia
		{
			pomoc=v->items[i];//Przesuwaj elementy
			v->items[i]=v->items[i+1];
			v->items[i+1]=pomoc;
			i--;
		}
	}
}

void vector_done(vector *v)
{
	free(v->items);
}

