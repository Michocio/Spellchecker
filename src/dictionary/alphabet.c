/** @file
  Implementacja struktury alphabet.

  @ingroup structers
  @author Michał Jaroń
  @copyright Uniwerstet Warszawski
  @date 2015-05-31
 */

#include <stdio.h>
#include <wchar.h>
#include "trie.h"
#include "alphabet.h"
#include <stdlib.h>


/**@}*/
/** @name Elementy interfejsu alphabet
  @{
 */
void alphabet_init(alphabet *v,const wchar_t item)
{
    (v)->items = malloc(sizeof(wchar_t *));//Przydziel pamięc
	(v)->items[0] = item;//"Włóż" pierwszy element
	(v)->size = 1;//Ustal rozmiar alfabetu
}


 void alphabet_resize(alphabet *v)
{
	wchar_t **items = realloc(v->items, sizeof(wchar_t *)*(v->size+1));//Powiększ tablicę dynamiczną
    if (items) {//Jeżeli się udało
        v->items = items;//Przepisz wskaźnik
        v->size++;//Aktualizuj rozmiar tablicy
    }
}


void alphabet_add(alphabet *v,const wchar_t letter)
{
	alphabet * pomoc;

	if(v->size==0)//Vektor pusty
		alphabet_init(v,letter);

	else//Nie pusty
	{
		if(alphabet_find(v,letter)!=1)
		{		
			alphabet_resize(v);
		
			int i=0;
			v->items[v->size-1] = letter;//Wstaw na koniec
			i=v->size-2;//Tablica indeksowna od 0

			while((i>=0)&&(wcscoll(&v->items[i],&letter)>0))//Iteruj od konca poszukujac miejsca wstawienia
			{
				//Przesuwaj elementy
				pomoc=v->items[i];
				v->items[i]=v->items[i+1];
				v->items[i+1]=pomoc;
				i--;
			}
			
		}
	}
}


void alphabet_done(alphabet *v)
{
	free(v->items);//Zwolnij pamięć
}


int alphabet_find(alphabet *v,const wchar_t letter)
{ //Wyszukiwanie binarne w tablicy
	
	int srodek=0;
	int poczatek = 0;
	
	if(v->size>0)
	{
		int koniec = v->size-1;

		while (poczatek < koniec)
		{
			srodek = (poczatek + koniec + 1) / 2;           // dzielenie przez 2 z zaokrągleniem w górę
			
			if (wcscoll(&v->items[srodek],&letter)<=0)
				poczatek = srodek;
			else
				koniec = srodek - 1;
		}

		if(v->items[koniec]==letter)// Czy na końcu poszukiwań, znaleźliśmy element?
			return 1;//Tak
		else return -1;//Nie
	}
	else return -1;//Przekazany vektor pusty
	
}


