/** @file
  Implementacja struktury states

  @ingroup structers
  @author Michał Jaroń
  @copyright Uniwerstet Warszawski
  @date 2015-06-29
 */

#include <stdio.h>
#include <wchar.h>
#include "dictionary.h"
#include "states.h"
#include <stdlib.h>
#include "vector.h"
#include "rules.h"
#include "avl.h"
#include <stdbool.h>


state * new_state(int suffix, tree ptr, tree first_word,int cost,int extend)
{
	state * new = malloc(sizeof(state));
	new->suffix=suffix;
	new->pointer=ptr;
	new->first_word=first_word;
	new->cost=cost;
	new->extend=extend;
	
}
/////////////////

void layer_init(layer *v, const state * item)
{
    (v)->items = malloc(sizeof(state *));//Przydziel pamięc
	(v)->items[0] = item;//"Włóż" pierwszy element
	(v)->size = 1;//Ustal rozmiar
}


void layer_resize(layer  *v)
{
	state **items = realloc(v->items, sizeof(state *)*(v->size+1));//Powiększ tablicę dynamiczną
    if (items) {//Jeżeli się udało
        v->items = items;//Przepisz wskaźnik
        v->size++;//Aktualizuj rozmiar tablicy
    }
}

void layer_resize_minus(layer  *v)
{//Zmniejsza tablicę dynamiczną
    state **items = realloc(v->items, sizeof(state *)*(v->size-1));//Zmniejsz tablicę dynamiczną
        v->items = items;//Przepisz wskaźnik
        v->size--;//Aktualizuj rozmiar tablicy
}


void state_delete(layer *v, const state * item)
{
	int i=0;
	while((v->items[i]->pointer!=item->pointer)&&(v->items[i]->suffix!=item->suffix))//Szukaj miejsca gdzie jest litera
		i++;

	state * wsk;
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
	//free(wsk->children);
	free(wsk);
}


void layer_add(layer *v,const state * item)
{
	
	if(v->size==0)//Vektor pusty
		layer_init(v,item);
	else
	{
		layer_resize(v);
		v->items[v->size-1] = item;//Wstaw na koniec
	}
}

void layer_done(layer *v)
{
	free(v->items);
}
///////////////

void layers_init(layers *v, const layer * item)
{
	(v)->items = malloc(sizeof(layer *));//Przydziel pamięc
	(v)->items[0] = item;//"Włóż" pierwszy element
	(v)->size = 1;//Ustal rozmiar alfabetu
	
}

void layers_add(layers *v,const layer* item)
{
	if(v->size==0)//Vektor pusty
		layers_init(v,item);
	else
	{
		layers_resize(v);
		v->items[v->size-1] = item;//Wstaw na koniec
	}
	
}

void layers_done(layers *v)
{
	free(v->items);
}
void layers_resize(layers *v)
{
		layer **items = realloc(v->items, sizeof(layer *)*(v->size+1));//Powiększ tablicę dynamiczną
    if (items) {//Jeżeli się udało
        v->items = items;//Przepisz wskaźnik
        v->size++;//Aktualizuj rozmiar tablicy
    }
	
}



wchar_t * edit(wchar_t *word,int index, rule make)
{//printf("\n \n EDITED:n");
	
	if(make.flag==3)//Reguła z flagą s 
	{
		wchar_t * edited=malloc(sizeof(wchar_t)*wcslen(word));
		wcsncpy(edited,word,index);
	edited[index]='\0';
	wchar_t * spacja =L" ";
	wcscat(edited,spacja);
	wcscat(edited,&*(word+index+1));
		//	printf("\n \n EDITED:%ls\n",edited);
			//return edited;
	}
	else
	{
		//printf("\n \n EDITED:\n");
			//return edited;
	if(word==NULL)
	{
			//printf("\n \n PUSRE!!\n");
		wchar_t *edited=malloc(sizeof(wchar_t)*wcslen(make.right));
		wcscat(edited,make.right);
		return edited;
	}		
	wchar_t *edited=malloc(sizeof(wchar_t)*wcslen(word)-1-wcslen(make.left)+wcslen(make.right));
	if(index>0)
	{

		wcsncpy(edited,word,index);
		edited[index]='\0';
		wcscat(edited,make.right);
		wcscat(edited,&*(word+index+wcslen(make.left)));
		return edited;
	}
	else
	{
	
		wcscpy(edited,make.right);
		edited[wcslen(make.right)]='\0';
		wcscat(edited,&*(word+index+wcslen(make.left)));
		return edited;
		
	}

	}	
}



int comperator(state *o1, state *o2,int cost1,int cost2)
{
	if(o1->suffix>o2->suffix) return 1;
	else if (o1->suffix<o2->suffix) return -1;
	else //Rowne
	{
		if(o1->pointer>o2->pointer) return 1;
		else if (o1->pointer<o2->pointer) return -1;
		else return 0;//Rowne
	}	
}
