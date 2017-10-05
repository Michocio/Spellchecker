/** @file
  Implementacja struktury rules i rule.

  @ingroup structers
  @author Michał Jaroń
  @copyright Uniwerstet Warszawski
  @date 2015-05-31
 */

#include <stdio.h>
#include <wchar.h>
#include "dictionary.h"
#include "rules.h"
#include <stdlib.h>



rule new_rule(wchar_t * left,wchar_t * right,int way,int cost,int flag)
{
	rule new;
	new.left=left;
	new.right=right;
	new.cost=cost;
	new.flag=flag;
	return new;
}


/**@}*/
/** @name Elementy interfejsu rules
  @{
 */
void rules_init(rules *v,const rule * item)
{  	
    (v)->items = malloc(sizeof(rule *));//Przydziel pamięc
	(v)->items[0] = item;//"Włóż" pierwszy element
	(v)->size = 1;//Ustal rozmiar 
}


 void rules_resize(rules *v)
{
	rule **items = realloc(v->items, sizeof(rule *)*(v->size+1));//Powiększ tablicę dynamiczną
    if (items) {//Jeżeli się udało
        v->items = items;//Przepisz wskaźnik
        v->size++;//Aktualizuj rozmiar tablicy
    }
}


void rules_add(rules *v,const rule * item)
{
	rules * pomoc;
	if(v->size==0)//Vektor pusty
		rules_init(v,item);
	else//Nie pusty
	{
		int pocz=0;
		int koniec=v->size-1;
		int srodek;
			
		while(pocz<koniec)
		{
			srodek=(pocz+koniec+1)/2;
			if(v->items[srodek]->cost<=item->cost)
				pocz=srodek;
			else
				koniec=srodek-1;
		}
			//Assert pocz==kon
			//Znaleziony ostatni mniejszy/rowny element od item
			//Wstaw tam item
			//Najpierw przesun pozostale
		rules_resize(v);
		int i;
			
		if(v->items[koniec]->cost>item->cost)
		{	
			for(int i=v->size-1;i>=koniec;i--)
				v->items[i]=v->items[i-1];
				//Teraz wstaw nowy element
			v->items[koniec]=item;
			i=koniec;
		}
		else
		{
			for(int i=v->size-1;i>koniec;i--)
				v->items[i]=v->items[i-1];
				//Teraz wstaw nowy element
				v->items[koniec+1]=item;
				i=koniec+1;		
			}	
			//Dosortuj jeszcze alfbarycznie, jeżeli taki sam koszt
			while((i>0)&&(wcscoll(v->items[i-1]->left,item->left)>0)&&(v->items[i-1]->cost==item->cost))
			//Iteruj od konca poszukujac miejsca wstawienia
			{
				pomoc=v->items[i-1];//Przesuwaj elementy
				v->items[i-1]=v->items[i];
				v->items[i]=pomoc;
				i--;
			}
			//if(v->costs[item->cost]==NULL) v->costs[item->cost]=item;
	}
}



void rules_done(rules *v)
{
	free(v->items);//Zwolnij pamięć
}


int rules_save(rules *v, FILE *stream)
{

	if(v->size!=0)
	{
		
		if(fprintf(stream,"\n%d\n",v->size)<0) return -1;
		
		for(int i=0;i<v->size;i++)
		{
			wchar_t * left, *right;
			left=v->items[i]->left;
			right=v->items[i]->right;
			if(wcslen(left)==0) left=L"ε";
			if(wcslen(right)==0) right=L"ε";
			if(fprintf(stream,"%ls ",left)<0) return -1;
		//	if(v->items[i]->way==0)
			//{
				//if(fprintf(stream,",")<0) return -1;
			//}
			//else
			//{
			//	if(fprintf(stream,"; ",v->items[i]->left)<0) return -1;
			//}
			if(fprintf(stream,"%ls %d %d\n",right,v->items[i]->cost,v->items[i]->flag)<0) return -1;
			
		}
	}
	else 
	{
		if(fprintf(stream,"\n0\n")<0) return -1;
	}
	
}
/*
void rules_load(FILE * stream)
{
	rules new;
	char str[1000];
	wchar_t str2[1000];
	char strejt[5][1000];
	int i=0;
	if(strcmp(fgets(str,1000,stream),"RULES:"))
	{
		//fscanf(stream,"%s",strejt[0])!
		while(strcmp(fgets(str,1000,strejt[i])!="DICT:")
		{
			//printf(" %s i:%d\n \n ", strejt[i],i);
			if(strcmp(strejt[i],"!!")==0) 
				i=0;
			else i++;	
			
			if(i==5)
			{
				bool bi=false;
				enum rule_flag flag;
				switch(strejt[4])
				{
					0:
					flag=RULE_NORMAL;
					break;
					1:
					flag=RULE_BEGIN;
					break;
					
					2:
					flag=RULE_END;
					break;
					3:
					flag=RULE_SPLIT;
					break;
					
				}
				dictionary_rule_add(dict,strejt[0],strejt[2],bi,strejt[3],flag);
				
			}  
			
		}
	}
}
*/

rule * rule_match(rule * match, wchar_t * word, int index)
{
	int variable=0;
	wchar_t var[9];///<Tablica z mozliwymi zmiennymi
	int len_word =wcslen(word); 
	int len_rule=wcslen(match->left); 
	int j=0;
	int i=index;
	
	for (int i=0;i<10;i++) var[i]=NULL;//Wstepna inicjalizacja
	
	if(len_rule<=(len_word-index))//Jeżeli suffix do dopasowania krótszy/rowny od reguly
	{	
		while((i<len_word)&&(j<len_rule))
		{
			if(*(word+i)==*(match->left+j))
			{
				j++; i++;
			}
			else if((*(match->left+j)>=48)&&((*(match->left+j)<=57)))//Zmienna w regule
			{
				if(var[*(match->left+j)-48]==NULL)
				{
					var[*(match->left+j)-48]=*(word+i);
					j++; i++;
				}
				else if((var[*(match->left+j)-48]==*(word+i)))
				{
					j++; i++;
				}
				else return NULL;//Nie pasuje
			}
			else return NULL;//Nie pasuje
		}
		len_rule=wcslen(match->right); 
		rule *abc=malloc(sizeof(rule));
		abc->left=match->left;
		abc->right=malloc(sizeof(wchar_t)*wcslen(match->right));
		abc->cost=match->cost;
		abc->flag=match->flag;	
		abc->var=match->var;	
		wcscpy(abc->right,match->right);
		for(int i=0;i<len_rule;i++)
		{
			if((*(abc->right+i)>=48)&&((*(abc->right+i)<=57)))
			{
				if(var[*(match->right+i)-48]!=NULL)
				{
					*(abc->right+i)=var[*(match->right+i)-48];
				}
				
			}
			
		}
		
		
		return abc;
	}
	else
	return NULL;//Regula za dluga dla danego sufiksu
}	


rules* fill_rules(rules v,wchar_t * word,int* size)
{

	int len_word=wcslen(word);
	rules * tab=malloc(sizeof(rules)*(len_word+1000));//[len_word+4];
	int ile=0;

	//Przejdz wszystkie sufiksy
	for(int i=0;i<len_word;i++)
	{
		//Przejdz wszystkie reguly
		for(int j=0;j<v.size;j++)
		{
			//Jezeli dana regula pasuje do poczatku sufixu to dodaj 
			rule * answer=rule_match(v.items[j],word,i);
			if(answer!=NULL)
			{
					rules_add(&tab[i],answer);
			}
		}
	}
for(int j=0;j<v.size;j++)
	{
		if(wcslen(v.items[j]->left)==0)
		{
			rule *abc=malloc(sizeof(rule));
			abc->left=v.items[j]->left;
			abc->right=malloc(sizeof(wchar_t)*wcslen(v.items[j]->right));
			abc->cost=v.items[j]->cost;
			abc->flag=v.items[j]->flag;		
			abc->var=v.items[j]->var;	
			wcscpy(abc->right,v.items[j]->right);
			if(v.items[j]->flag!=3)//Rozne od "s"
				rules_add(&tab[len_word],abc);	
		}
	}
	*size=len_word;
	return tab;
}


rules  fill_empty(rules v)
//Do sufiksu pustego pasuja tylko reguly z lewa strona pusta
{	//Przejdz wszystkie reguły
	rules empty; empty.size=0;
	for(int j=0;j<v.size;j++)
	{
		if(wcslen(v.items[j]->left)==0)
		{
			rule *abc=malloc(sizeof(rule));
			abc->left=v.items[j]->left;
			abc->right=malloc(sizeof(wchar_t)*wcslen(v.items[j]->right));
			abc->cost=v.items[j]->cost;
			abc->flag=v.items[j]->flag;		
			abc->var=v.items[j]->var;	
			wcscpy(abc->right,v.items[j]->right);
			if(v.items[j]->flag!=3)//Rozne od "s"
				rules_add(&empty,abc);	
		}
	}
	printf("\nSIZE %d",empty.size);
	return empty;
}
