/** @file
  Implementacja struktury drzewa.

  @ingroup structers
  @author Michał Jaroń
  @copyright Uniwerstet Warszawski
  @date 2015-05-31
 */

#include <stdio.h>
#include <wchar.h>
#include "trie.h"
#include <stdlib.h>
#include "states.h"

/**@}*/
/** @name Elementy interfejsu node
  @{
 */
 
tree find(const wchar_t * word,const tree root,int i,int len)
	{
			tree pomoc= vector_find(root->children,*(word+i));//bez rzutwoania?
			if((pomoc!=NULL))//Istnieje węzeł z taką literą
			{
				if(len==i)//Ostatnia litera słowa wyszukiwana
				{
					if(pomoc->word==1)
						return pomoc;//found!
					else return NULL;
				}
				else
				{
					i++;
					return find(word,pomoc,i,len);//Szukaj dalej rekurencyjnie
				}
			}
			else return NULL;
};
 
 
tree find_prefix(const wchar_t * word,const tree root,int i,int len, wchar_t letter)
{
	
	wchar_t wstaw =*(word+i);	
	if((*(word+i)>=48)&&((*(word+i)<=57)))
		wstaw=letter;
	tree pomoc= vector_find(root->children,wstaw);//bez rzutwoania?
	if((pomoc!=NULL))//Istnieje węzeł z taką literą
	{
		if(len==i)//Ostatnia litera słowa wyszukiwana
			return pomoc;//found!
		else
		{
			i++;
			return find_prefix(word,pomoc,i,len,letter);//Szukaj dalej rekurencyjnie
		}
	}
	else return NULL;
}
 
 
int node_delete(tree * root,const wchar_t * word)
{//Kasuje wyraz word z drzewa
	 if(trie_find(word,*root)!=-1)
	 {
		 wchar_t letter;
		 tree wsk;
		 wsk=find(word,*root,0,wcslen(word)-1);
		 wsk->word=0;
		 while((wsk->letter!=NULL)&&(wsk->word!=1)&&(wsk->children->size==0))//Idź w górę drzewa
		 { 
				//debug((*root));
				tree uchwyt=wsk->parent;
				letter=wsk->letter;
				vector_delete(uchwyt->children,letter);
				wsk=uchwyt;
		 }
		 return 0;//Poprawnie skasowany!
	 }
	 else return -1; //Nie znaleziono noda
}


void trie_init(tree* root,const tree parent,const wchar_t letter)
{
	
	(*root)=malloc(sizeof(node));
	(*root)->children=malloc(sizeof(vector));
	(*root)->children->size=0;
	(*root)->letter=letter;
	(*root)->word=0;
	(*root)->parent=(parent);
}; 


int trie_find(const wchar_t * word,const tree  root)//Funkcja "otoczka"
{ //Szukaj w drzewie rekurencyjnie danego słowa
	//Rozpocznij szukanie
	if(find(word,root,0,wcslen(word)-1)==NULL) return -1;
	else return 1;
}


int trie_insert(const wchar_t * word,tree* root)
{
		
		int insert(const wchar_t * word,tree *root,int i,int len)
		{	
			tree pomoc= vector_find((*root)->children,*(word+i));//bez rzutwoania?
			if((pomoc!=NULL))//Istnieje węzeł z taką literą
			{
				if(len==i)//Ostatnia litera słowa 
				{
					if((pomoc)->word==1)
						return -1;//Już w słowniku, więc ignored!
					else 
					{
						(pomoc)->word=1;
						return 1;//Wstawiamy słowo, będące prefixem już istniejącego
					}
				}
				else
				{
					i++;
					return insert(word,&pomoc,i,len);//Szukaj dalej rekurencyjnie
				}
			}
			else//Nie istnieje wezęł z daną literą
			{
				tree new; //
				trie_init(&new,*root,*(word+i));
				vector_add((*root)->children,new);
				if(len==i)//Ostatnia litera słowa 
				{
					new->word=1;
					return 1;
				}
				else
				{
					i++;
					return insert(word,&new,i,len);//Wstawiaj dalej rekurencyjnie
				}
			}
		};//Koniec funkcji wewnętrznej

	return insert(word,(root),0,wcslen(word)-1);
}


void trie_clear(tree* root)
{
	//Czyść całe drzewo
	for(int i=0;i<((*root)->children->size);++i)
			trie_clear(&(*root)->children->items[i]);

	if(((*root)->children->size)>0)
		vector_done((*root)->children);
	else
	{
		for(int i=0;i<((*root)->children->size);++i)
		{
			if((*root)->letter!=NULL)
			free(&(*root)->children->items[i]);
		}
	}
	vector_done(&(*root)->children);
	free((*root));
}


int toString(tree node,FILE* stream)
{//Zapisuj do pliku drzewo
//Obieg prefixowy	
	if((node->children->size)!=0)
	{
		if(node->letter!=L'\0')//Z dziećmi, otwórz "(" CHANGE!
		{
			if(node->word==1)//Koniec słowa, zapisz z ";"
			{
				if(fprintf(stream,"%lc;(",node->letter)<0) return -1;
			}
			else
			{
				if(fprintf(stream,"%lc(",node->letter)<0) return -1;
			}
		}
		
		for(int i=0;i<node->children->size;i++)
		{
				toString(node->children->items[i],stream);//Rekurencja po wszystkich dzieciach
		}
		if(node->letter!=NULL)//Zamyka listę dzieci ")"
		{
			
			if(fprintf(stream,")")<0) return -1;
		}
	}
	else
	{//CHANGE! Drzewo puste!
		if(node->letter!=L'\0')
		{
			if(fprintf(stream,"%lc",node->letter)<0) return -1;	
		}
	}
		
		if(ferror(stream)) return -1;
		else return 1;	
}


tree trie_load(FILE* stream)
{//Wczytywanie drzewa z pliku
	
	int error=0;
	tree root;
	trie_init(&root,NULL,'\0');	
	
	//Procedura wewnetrzna
	void recursive_add(FILE* stream, tree* root)
	{
		tree  uchwyt=NULL;
		wchar_t wc;
		int flaga=0;
		int stop=0;
		while(stop!=1)
		{
			wc = fgetwc (stream);	//Wczytuj znaki
			if (ferror(stream))
			{
				stop=1;
				error=1;
			}
			if(wc==EOF) stop=1; //Koniec pliku
			else if(wc==40)// (
			{
				if(flaga==0)// Aktulizuj czy node końcem słowa
					uchwyt->word=0;
				flaga=0;	
				recursive_add(stream,&uchwyt);//debu&	
			}
			else if(wc==41)// )
				stop=1;
			else if(wc==43)// +
				stop=1;
			else if(wc==L';')//Koniec słowa
				flaga=1;
			else //if(wc!=10)
			{
				if(wc==EOF) stop=1;
				else
				{ //Dodaj nowy node
					flaga=0;
					tree help; 
					trie_init(&help,(*root),wc);
					//help->letter=wc;
					help->word=1;	
					vector_add((*root)->children,help);
					uchwyt=help;
				}
			}		
		}
	}
	recursive_add(stream,&root);
	if(error==1) return NULL;
	else return root;	//Zwróc adres do korzenia nowego drzewa
}

//Procedura robocza do debugowania
void debug(tree root)
{//Wypisuje strukturę drzewa. Proste PreOrder
		//printf("\n ITEMS:, %d \n",root->children->size);
		/*
		
		 state x;
    new_state(0,root->children->items[0]);
    
    
    //state y=
    //new_state(0,&dict->root->children->items[1]);
    
    
    layer t1;layer_init(&t1,&x);
    //layer_add(&t1,&y);
    layers array2; layers_init(&array2,&t1);
		*/
		
		
		
	//	if(root->children->size==13) printf("\nWWWW: %lc, %lc",root->children->items[10]->letter,root->children->items[11]->letter);
	if(root->parent!=NULL)
		printf("Litera: %lc, parent: %lc, word: %d , adres: %p, rozmiar: %d .",root->letter, root->parent->letter, root->word, root, root->children->size);
		for(int i=0;i<((root)->children->size);++i)
	{
			printf("  %lc   ",(root)->children->items[i]->letter);
	}
printf("\n");
	for(int i=0;i<((root)->children->size);++i)
	{
			debug(root->children->items[i]);
	}


}

















