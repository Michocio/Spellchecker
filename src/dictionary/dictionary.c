/** @file
  Implementacja słownika.
  Słownik przechowuje listę słów.

  @ingroup dictionary
  @author Michał Jaroń
  @copyright Uniwerstet Warszawski
  @date 2015-06-29
 */
#include "states.c"
#include "avl.c"
#include "trie.h"
#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "rules.h"
#include "conf.h"
#define _GNU_SOURCE

/**
  Struktura przechowująca słownik.
  * Implementacja z użyciem drzewa Trie (\ref node).
  * Strukutra zawiera listę reguł
  * używanych w \ref dictionary_hints
 */
struct dictionary
{
    tree root; ///< Wskażnik na korzeń drzewa Trie.
    size_t K;///<Maksymalny koszt z jakim jest generowana podpowiedź.
    rules rules_list;///<Lista reguł
};


/**@}*/
/** @name Elementy interfejsu 
  @{
 */
struct dictionary * dictionary_new()
{
    struct dictionary *dict =
        (struct dictionary *) malloc(sizeof(struct dictionary));
    trie_init(&dict->root,NULL,'\0');
    (dict)->rules_list.size=0;  
    (dict)->K=0;//Domyślna wartość
    return dict;
}


void dictionary_done(struct dictionary *dict)
{
	trie_clear(&dict->root);
	//dictionary_rule_clear(dict);
	//free((dict));
}


int dictionary_insert(struct dictionary *dict, const wchar_t *word)
{
	if(trie_find(word,dict->root)!=-1) return 0;//Nie wstawione
	trie_insert(word,&dict->root);
	//debug(dict->root);
    return 1;//Wstaw
}


int dictionary_delete(struct dictionary *dict, const wchar_t *word)
{
   if(node_delete(&dict->root,word)!=-1) return 1;//Usunięte!
	else return 0;//Niepowodzenie
}


bool dictionary_find(const struct dictionary *dict, const wchar_t* word)
{
	//debug(dict->root);
	int a =trie_find(word,dict->root);
	if(a!=-1) return true;//Found
	else return false;//Not found
}


int dictionary_save(const struct dictionary *dict, FILE* stream)
{
	if(toString(dict->root,stream)<0) return -1;//Najpierw słownik
	if(fprintf(stream,"+\n%d",dict->K)<0) return -1;//Max koszt
	if(rules_save(&dict->rules_list,stream)<0) return -1;//Teraz reguły
	else return 0;//OK!
}



struct dictionary * dictionary_load(FILE* stream)
{
    struct dictionary *dict = (struct dictionary *) malloc(sizeof(struct dictionary));
    
	int i=0;
	int j=0;
	int ile=0;
	int koszt;
	dict->root=trie_load(stream);    	
   	fseek ( stream , ftell(stream)+1, SEEK_SET );
	fwscanf(stream,L"%d",&koszt);
	dictionary_hints_max_cost(dict,koszt);
	fwscanf(stream,L"%d",&ile);
	for(int i=0;i<ile;i++)
	{
		int koszt;int flaga;
		bool bi=false;
		enum rule_flag flag;
		wchar_t left[1000];  wchar_t right[1000];
		fwscanf(stream,L"%ls",left);
		fwscanf(stream,L"%ls",right);
		fwscanf(stream,L"%d",&koszt);
		fwscanf(stream,L"%d",&flaga);
		switch(flaga)//Jaka flaga, zgodznie z formatem zapisu
		{
			case 0:
			flag=RULE_NORMAL;
			break;
			case 1:
			flag=RULE_BEGIN;
			break;
			case 2:
			flag=RULE_END;
			break;
			case 3:
			flag=RULE_SPLIT;
			break;
		}

		//W przypadku pustej reguly
		if((strcmp(left,L"ε")==0)&&(strcmp(right,L"ε")==0)) 
			dictionary_rule_add(dict,L"",L"",bi,koszt,flag);
		else if(strcmp(left,L"ε")==0) dictionary_rule_add(dict,L"",right,bi,koszt,flag);
		else if(strcmp(right,L"ε")==0)dictionary_rule_add(dict,left,L"",bi,koszt,flag);
		else
			dictionary_rule_add(dict,left,right,bi,koszt,flag);	
		}	


	if (dict->root==NULL)//Nie udało się wczytać
    {//Posprzątaj
        dictionary_done(dict);
        dict = NULL;
    }
   // debug(dict->root);
    return dict;
}



/**
  Idzie z danego stanu w górę drzewa tworząc słowo.
  @param[in] state Stan końcowy.
  @param[in] tree Korzeń drzewa.
  @param[in,out] word_list * Używane na potrzeby testów, może być NULL.
  @return słowo do dodania do \ref word_list
  */
wchar_t * word2(state stan, tree root,struct word_list *list)
{
	//Maxymalna dlugosc slowa z dict-editor to 63, wiec 63 * 2 ~=135 :)
	wchar_t * word=malloc(sizeof(wchar_t)*135);
	wchar_t * word1=malloc(sizeof(wchar_t)*135);
	wchar_t * word2=malloc(sizeof(wchar_t)*135);
	tree wsk=stan.pointer;
	int i=134;
	//Dwa słowa
	if((stan.first_word!=NULL))
	{
		tree ptr=stan.first_word;
		while(ptr->letter!=NULL)
		{
			word1[i]=ptr->letter;
			ptr=ptr->parent;
			i--;
		}
	
		if((wcslen(&*(word1+i+1))==0)||(stan.first_word->word!=1)) return NULL;
		wcscpy(word,&*(word1+i+1));
		wcscat(word,L" ");
	}
	i=134;
	while(wsk->letter!=NULL)
	{
		word2[i]=wsk->letter;
		wsk=wsk->parent;
		i--;
	}

	if(stan.first_word!=NULL)
		wcscat(word,&*(word2+i+1));
	else
		wcscpy(word,&*(word2+i+1));

	

	return word;
}

/**
  Rozwija dany stan.
  @param[in] int Koszt otrzymania danej reguly
  @param[in] state Stan do rozwinięcia.
  @param[in,out] layer * warstwa do której dodajemy nowe stany.
  @param[in] wchar_t * Słowo, dla którego hinty.
  @param[in,out]struct bin* Korzeń drzewa AVL do przechowywania stanów.
  @param[in,out] struct word_list * Lista z hintami do której dodajemy słowa powstałe,
  ze stanów końcowych.
  @param[in] tree Korzeń drzewa ze słownikiem
  */	
void extend(int cost,state  start, layer * to_extend, wchar_t *word,struct bin* root, 
				struct word_list *list,tree dict )
{
	tree found= vector_find(start.pointer->children,*(word+start.suffix));

	if(found!=NULL)
	{
		state *new =malloc(sizeof(state));
		new->suffix= start.suffix+1;
		new->pointer=found;
		new->cost=cost;
		new->first_word=start.first_word;
		int x=search(new,root);

		if(x==-1)
		{
			root=insert(root,new);	
	
			layer_add(to_extend,new);
		
			if((new->suffix==wcslen(word))&&(new->pointer->word==1))
				word_list_add(list,word2(*new,dict,list));//Kolejny hint	
			else if((new->suffix+1)<=wcslen(word))//Dalej rekurencja rozwijania stanu
				extend(cost,*new,to_extend,word,root,list,dict);	
		}
		
	}
}



/**
  Czy dana reguła jest poprawna dla danego stanu
  @param[in] state Stan do sprawdzenia
  @param[in] rule * Regula do dopasowania
  @param[in] wchar_t * word słowo dla którego szukane hinty
  @param[in] tree root Korzeń drzewa ze słownikiem
  @return true - poprawna reguła, false w p.p
  */
bool correct_rule(state * stan, rule * regula, wchar_t * word,tree root)
{
	//Sprawdz flagi reguly z cechami danego stanu
	if(regula->flag==0)//Normalna regula
		return true;
	else if((regula->flag==1)&&(stan->pointer==root)&&(stan->suffix=0))//Pierwszy blok
		return true;
	else if((regula->flag==2)&&((stan->suffix=wcslen(word))||((stan->suffix<wcslen(word)
						&&(stan->suffix+wcslen(regula->left))>=wcslen(word)))))//Drugi blok
		return true;
	else if((regula->flag==3)&&(stan->first_word==NULL))//&&(stan->suffix+wcslen(regula->left))<wcslen(word))
	{

	//Regula s sprawdz czy nie zastosowana juz raz  i czy nie koniec wyrazu
		
		if((stan->suffix==0)&&(wcslen(regula->left))==0) {

			if(stan->pointer==root)
				return false;
			else
			 return true;
		}
		else
		{
			return true;
		}	
		
	}
	else
		return false;
}


/**
	Schodzi w dół drzewa, próbując stosować daną regułe. 
	Uzwględnia podstawianie pod zmienne różnych liter
 @param[in] int Koszt otrzymania danej reguly
  @param[in] tree Node, w którym obecnie jesteśmy, potrzebne w rekurencji
  dla początkowego wywołania dostaje node pokazywany przez edytowany stan.
  @param[in] rule * Regula do zastosowanie.
  @param[in,out] int * ile nowych stanów powstało.
  @param[in] int Potrzebne w rekurencji, początkowo = 0.
  @param[in] wchar_t null jeżeli jeszcze nie natknęliśmy sie na wolną zmienną,
  litera do wstawiania z miejsce wolnej zmiennej, jeżeli już natknęliśmy się,
  podcza rekurencji na wolną zmienną.
  @param[in,out] layer * Wolne zmienne powodują,
  że dla danego stanu po zastosowaniu danej reguły.
  może powstać wiele nowych stanów. Są one dodawane do tej warstwy.
  @param[in] int Długość prawej strony reguły.
  @param[in] int Długość lewej strony reguły.
  @param[in] int Długość suffixu akutalnie edytowanego stanu.
  @param[in] tree Korzeń drzewa ze słownikiem.
  @param[in] int word Długość słowa dla którego hinty.
  @param[in] tree Wskaźnik na pierwsze słowo.
   @param[in] wchar_t Litera ze stanu dla ktorego wywolane.
  jeżeli stan powstał z reguły "s", w p.p NULL.
  */
void edit_state(state stan, rule regula, tree wezel,layer * nowe, tree korzen,int koszt, int dlg)
{
	
		int x=0;
		if(regula.var!=-1)
		{//Wolna zmienna po prawej stronie
			wchar_t * word= malloc(sizeof(wchar_t)*(regula.var));
			wcsncpy(word,regula.right,regula.var);
			tree letter;
			if(regula.var!=0)
			{
				x=1;
				letter=find_prefix(word,stan.pointer,0,wcslen(word),NULL);
			 }
			 else letter= wezel;
			if(letter!=NULL)
			{
				if(letter->children->size>0)
				{//&*(regula.right+wcslen(word)+x),
					for(int i=0;i<letter->children->size;i++)
					{
						tree wsk=
						find_prefix(&*(regula.right+x),
							letter,0,wcslen(regula.right)-1, letter->children->items[i]->letter);
							
						if(wsk!=NULL)
						{
							tree first =stan.first_word;
							tree ptr=wsk;
							int is_extend=1;
							int len=wcslen(regula.left);
							if(regula.flag==3)
							{
	 							first =wsk;
								ptr=korzen;	
								
							}
							if(regula.flag==2)
								is_extend=-1;
										
							state * new = new_state(len+stan.suffix,ptr,first,koszt,is_extend);
							if(((regula.flag==2)&&(new->suffix=dlg)&&(new->pointer->word==1))||(regula.flag!=2))
							{
								if(((regula.flag==3)&&(first->word==1))||(regula.flag!=3))
								{
									layer_add(nowe,new);
								}
							}
						}
					}
					
				}
				
				
			}
		}
		else
		{//Bez wolnych zmiennych
			tree look;
			if(wcslen(regula.right)==0)
			{
				look=stan.pointer;
				
			}
			else	
				look=find_prefix(regula.right,stan.pointer,0,wcslen(regula.right)-1,NULL);
			if(look!=NULL)
			{
				tree first =stan.first_word;
				tree ptr=look;
				int is_extend=1;
				int len=wcslen(regula.left);
				if(regula.flag==3)
				{
					//printf("\nUEGA %lc", look->letter);
					first =look;
					 ptr=korzen;	
				}
				if(regula.flag==2)
					is_extend=-1;
				state * new = new_state(len+stan.suffix,ptr,first,koszt,is_extend);
				layer_add(nowe,new);
			}	
		}
	
		
}




void dictionary_hints(const struct dictionary *dict, const wchar_t* word,
        struct word_list *list)
{
	word_list_init(list);//Lista na hintsy
	int word_len=wcslen(word);
	//Drzewo binarne przechowuje wszystkie unikalne stany
	struct bin *avl = NULL;
	
	
	rules * good_rules=NULL;
	good_rules; int size=0;
	//Stablicuj dobre reguły dla danego słowa i kolejnych jego suffixow
	good_rules=fill_rules(dict->rules_list,word,&size);
	
	
	/*for(int i=0;i<=wcslen(word);i++)
	{
		printf("\nSufiks %ls %d %d \n",&*(word+i),i,good_rules[i].size);
		for(int j=0;j<good_rules[i].size;j++)
		printf("\%ls -> %ls\n",good_rules[i].items[j]->left,good_rules[i].items[j]->right);	
	}*/
	
	
	layers  claster;///<Zbior warstw o kolejnych kosztach
	//claster=NULL;
	layer  war0;///warstwa[0]
	//war0=NULL;
	state   start=*new_state(0,dict->root,NULL,0,1);///Stan poczatkowy

	
	avl=insert(avl,&start);//Dodaj stan początkowy do drzewa AVL	
	layer_init(&war0,&start);
	//Rozwin stan poczatkowy	
	extend(0,start,&war0,word,avl,list,dict->root);
	layers_init(&claster,&war0);//Dodaj warstwe wa0 do klastra

	/*ZbierzStany(k)
     warstwa[k] ← ∅
     dla i = 1, ..., k
         dla każdego stanu stan ∈ warstawa[k - i]
             dla każdej reguły r o koszcie i, która stosuje się do stanu stan // pętla reguł
                 warstwa[k] ← warstwa[k] ∪ Rozwiń(stan otrzymany po przejściu z stan regułą r)*/
               
	int koszt=dict->K;//Maxymalny koszt
	for(int k=1;(k<=koszt)&&word_list_size(list)<20;k++)//Zbieraj kolejne stany
	{
		//ZbierzStany(k)
		layer *warstwa= malloc(sizeof(layer));// warstwa[k] ← ∅
		struct word_list words;
		word_list_init(&words);//Lista na hintsy
		warstwa->size=0;
		//dla i = 1, ..., k
		for(int i=1;i<=k;i++)
		{
			 //dla każdego stanu stan ∈ warstawa[k - i]
			for(int stan=0;stan<claster.items[k-i]->size;stan++)
			{
				//Obecnie przegladany stan
				state *stan_i= claster.items[k-i]->items[stan];
				if(stan_i->extend!=-1)//Czy rozwijalny
				{
					//Tablica dobrych regul dla danego suffixu
					rules  good_suffix= good_rules[stan_i->suffix];
					int len= good_suffix.size;
					int index= find_rule(good_suffix,i);//Znajdz miejsce gdzie zaczyna sie koszt i
					//dla każdej reguły r o koszcie i, która stosuje się do stanu stan 
					if(index!=-1)//Jeżeli jest takie miejsce
					{
						//To dla kazdej reguly o koszcie i
						for(int regula=index;regula<len&&(good_suffix.items[regula]->cost==i);regula++)
						{// pętla reguł
							//Pojemnik na stany, moze byc kilka z powodow zmiennych
							layer * new_states=malloc(sizeof(layer)); new_states->size=0;
							//Sprawdz czy dana regula stosuje się do danego stanu
							if(correct_rule(stan_i,good_suffix.items[regula],word,dict->root))
							{//Jeżeli poprawna to sprobuj zastosowac 
								edit_state(*stan_i,*good_suffix.items[regula],
										stan_i->pointer,new_states,dict->root,k,word_len);
								//Sprawdz ktore ze stanow nowe
								for(int n=0;n<new_states->size;n++)
								{//Po wszystkich nowo stworzonych stanach
									int hh=search(new_states->items[n],avl);//Czy jest juz w drzewie?
									if(hh==-1)//Nie ma
									{
										//Nowy stan = dodaj do warstwy	
										avl=insert(avl,new_states->items[n]);					
										layer_add(warstwa,new_states->items[n]);
										if((new_states->items[n]->suffix==wcslen(word))
											&&(new_states->items[n]->pointer->word==1))//Całe słowo
												word_list_add(&words,word2(*new_states->items[n],dict->root,list));

										//Rozwin nowo dodany stan
										extend(k,*new_states->items[n],warstwa,word,avl,&words,&dict->root);
									}
								}						
							}
						}//Kolejna regula dla stanu
					}
				
				}
			}//Kolejny stan			
		}//Przegladaj kolejna warstwe	
		bool stop=false;
		word_list_sort(&words);	
		const wchar_t * const *a = word_list_get(&words);
        for (size_t i = 0; (i < word_list_size(&words))&&( 20>word_list_size(list)); ++i)
        {
			const wchar_t * const *b = word_list_get(list);
			for(int j=0;j<word_list_size(list);j++)
			{
				if(wcscmp(a[i],b[j])==0)
					stop=true;
			}
			if(stop!=true)
				word_list_add(list,a[i]);
			else 	stop=false;
        }
			
		layers_add(&claster,warstwa);//Kolejna warstwa
	}//Kolejny koszt
	
	 //destroy_tree(&avl);
	//Wyczysc stany        	
}

/**
  Szuka binarnie miejsca w tablicy z regułami,
  dla od której zaczynają się reguły o koszcie cost.
  @param[in] rules Tablica do poszukiwań.
  @param[in] int Szukany koszt.
  @return Index w tablicy o koszcie cost, lub -1 gdy brak reguł o koszcie cost.
  */	
int find_rule(rules suffix, int cost)
{
	
	if(suffix.size>0)
	{
	int pocz=0;
	int koniec=suffix.size-1;
	int srodek;
	while(pocz<koniec)
	{
		srodek=(pocz+koniec)/2;
		//printf("\nkoniec %d n",srodek);
		if(suffix.items[srodek]->cost>=cost)
			koniec=srodek;
		else
			pocz=srodek+1;
	}
	if(suffix.items[koniec]->cost==cost) return koniec;
	else return -1;
}
else return -1;
}

void dictionary_rule_clear(struct dictionary *dict)
{
	for(int i=0;i<dict->rules_list.size;i++)
	{
		free(dict->rules_list.items[i]);
	}
	dict->rules_list.size=0;
}


struct dictionary * file( int v,const char * plik)
{//Obsługa dict-check
	//dict->root=trie_load(stream); 
	
	FILE *stream = fopen(plik, "r");
   struct dictionary *dict = dictionary_load(stream);//(struct dictionary *) malloc(sizeof(struct dictionary));


	int wiersz =1; int znak=0;
	wint_t wc;
	wchar_t str [100];//"String"
	wchar_t org [100];//Z dużymi literami i znakami innymi niż litery. "Orginał"
	int i=0;
	
	int stop=0;
	do
	{
		znak=1;
		do 
		{
			i=0;
			do
			{
				wc=getwchar();//Czytaj znak z wejścia
				
				if(ferror(stream)) return NULL;

				org[i]=wc;
				if(iswalpha(wc)!=0) //CZy litera? Tak!
				{
					str[i]=towlower(wc);
					i++; 
				}

				znak++;	
			} while (iswalpha(wc)!=0);//Pętla, wczytuje wyraz
		
			
			if(wc==WEOF){ stop=1; break;}
			else
			{
				if(i!=0)
				{
					str[i]=L'\0';//Zakończ wyraz
					org[i+1]=L'\0';//Zakończ wyraz
					if(dictionary_find((dict),str)==false)//Wyraz w słowniku
					{ 
						printf("#%ls", org);
						org[i]=L'\0';//Nie [potrzebny już ostatni znak
						if(v==1)//Czy parametr -v
						{
							fprintf (stderr,"%d,%d %ls: ",wiersz,znak-i-1,org);
				
							struct word_list list;
							dictionary_hints(dict, &str, &list);
							//Wword_list_sort(&list);
							const wchar_t * const *a = word_list_get(&list);
							//Wyświetl listę
							for (size_t i = 0; i < word_list_size(&list); ++i)
							{
								if (i)
									fprintf(stderr," ");
								fprintf(stderr,"%ls", a[i]);
							}
							fprintf(stderr,"\n");
						}
					}
					else printf("%ls", org);//Wyraz NIE w słowniku
				}
				else
					printf("%lc",wc);
			}
		}while(wc!='\n');//Pętla po wierszach
	wiersz++;
	}while(stop!=1);//Pętla do końca pliku

fclose(stream);//Zamknji strumień
dictionary_done(dict);
}


/** Zamienia słowo na złożone z małych liter.
  @param[in,out] word Modyfikowane słowo.
  @return 0, jeśli słowo nie jest złożone z samych liter, 1 w p.p.
 */
int lowercase(wchar_t *word)
{
    for (wchar_t *w = word; *w; ++w)
        if (!iswalpha(*w))
            return 0;
        else
            *w = towlower(*w);
    return 1;
}


/*
int dictionary_lang_list(char **list, size_t *list_len)
{
	
	//Utworz tablice na ktora wpisuje
	//Wpisuje pamietajac, ze /n
	//Przypisz
	if( access( CONF_PATH, F_OK ) != -1 ) 
	{
		FILE * stream= fopen(CONF_PATH,"r");
		fseek(stream, 0L, SEEK_END);
		int size = ftell(stream);//Dlugosc pliku
		int capacity=0; int pos=0;

		*list_len=size;
		fclose(stream);
		if(size>0)
		{
			//Wroc na poczatek pliku 
			stream= fopen(CONF_PATH,"r");
			//fseek(stream, 0L, SEEK_SET);

			char * buffor=malloc(sizeof(char)*size);
			char ** array=(char**)malloc(sizeof(char*)); //Tablica wskaznikow na kolejne nazwy jezykow
			capacity=1;
			int i=0;
			char c; char last=' ';

			c=fgetc(stream);
			buffor[i]=c;

			array[pos]=&buffor[i]; pos++;

			while(c!=EOF)
			{
				if(ferror(stream)) return -1;
				buffor[i]=c;
			
				if((c=='\n')||(c==EOF)) buffor[i]='\0';
				if(last!=' ')
				{
					if(last=='\n')
					{
						if(pos<capacity)
						{
							array[pos]=&buffor[i];
							pos++;
						}
						else
						{
							capacity*=2;
							array=realloc(array,sizeof(char*)*capacity);
							array[pos]=&buffor[i];
							pos++;
						}
					}
				}
				last=c;
				c=fgetc(stream);
				i++;
			}

		(*list)=array;

		fclose(stream);
		return 0;
	}
	else{ fclose(stream); return 0;}
}
else{ FILE * stream= fopen(CONF_PATH,"w");//Jeśli takiego plikunie ma to utwórz
	 return -1;}
}
*/
/**
  Zwraca nazwy języków, dla których dostępne są słowniki.
  Powinny to być nazwy lokali bez kodowania. np.
  Przykładowe nazwy pl_PL, albo en_US.
  Reprezentacja listy języków jest podobna do
  list łańcuchów znakowych [argz w glibc'u](http://www.gnu.org/software/libc/manual/html_mono/libc.html#Argz-and-Envz-Vectors).
  `*list` jest wskaźnikiem na początek bufora,
  który ma długość `*list_len`.
  W buforze znajdują się łańcuchy znakowe
  jeden po drugim pooddzielane znakiem '\0'.
  Jeśli lista jest niepusta cały bufor też się kończy znakiem '\0'.
  Użytkownik jest odpowiedzialny za zwolnienie tej listy,
  w tym celu wystarczy wywołać `free(*list)`.
  @param[out] list Lista dostępnych języków.
  @param[out] list_len Długość bufora z listą dostępnych języków.
  @return <0 jeśli operacja się nie powiodła, 0 w p.p.
  */
int dictionary_lang_list(char **list, size_t *list_len)
{
	
	//Utworz tablice na ktora wpisuje
	//Wpisuje pamietajac, ze /n
	//Przypisz
	
	char destination[strlen(CONF_PATH)+259];
	strcpy(destination,CONF_PATH);
	strcat(destination,"langs.txt");//Nazwa pliku ze slownikami

	if( access( destination, F_OK ) != -1 ) //Jeżeli plik z listą słowników istnieje
	{
		FILE * stream= fopen(destination,"r");
		fseek(stream, 0L, SEEK_END);
		int size = ftell(stream);//Dlugosc pliku
		int capacity=0; int pos=0;
	
		if(size>0)
			*list_len=size;
		else *list_len=0;
		fclose(stream);
		if(size>0)
		{
			//Wroc na poczatek pliku 
			stream= fopen(destination,"r");
			char * buffor=malloc(sizeof(char)*size-1);//Buffor ze znakami
			char ** array=(char**)malloc(sizeof(char*)); //Tablica wskaznikow na kolejne nazwy jezykow
			capacity=1;
			int i=0;
			char c; char last=' ';

			c=fgetc(stream);
			buffor[i]=c;

			array[pos]=&buffor[i]; pos++;
			//Wczytuj znaki
			while(c!=EOF)
			{
				if(ferror(stream)) return -1;
				buffor[i]=c;
			
				if((c=='\n')) buffor[i]='\0';
				if(last!=' ')
				{
					if(last=='\n')
					{	//Włóż
						if(pos<capacity)
						{
							array[pos]=&buffor[i];
							pos++;
						}
						else
						{//Powiększ tablicę
							capacity*=2;
							array=realloc(array,sizeof(char*)*capacity);
							array[pos]=&buffor[i];
							pos++;
						}
					}
				}
				last=c;
				c=fgetc(stream);
				i++;
			}
		buffor[size-1]='\0';//Na koniec jeszcze
		/*
		 * Nie jestem pewny interpretacji zadania dlatego drugą opcję zostawiam w komentarzu
		 * Zwracam wskaźnik na bufor, ale nie jestem pewien czy nie powinienem zwrócić wskaźnika
		 * na listę wskaznikow na kolejne nazwy 
		 */ 
		// *list=malloc(sizeof(char)*size-1);
		// strcpy((&list),buffor);
		// printf("%s",buffor[0]);
		
		/*list=(char**)malloc(sizeof(char*)*capacity);
		
		printf(" \n %s \n",array[0]);
		
		for(int i=0;i<capacity;i++)
		{
			//strcpy(&list[i],array[i]);
			list[i]=array[i];
			
			
		}
		*list_len=capacity;
		printf(" \n %d \n", capacity);
		printf(" \n %s \n",list[1]);*/
		(*list)=buffor;
		free(array);//change
		fclose(stream);
		return 0;
	}
	else{  return 0;}//Pusta lista
}
else{ FILE * stream= fopen(destination,"w");//Jeśli takiego pliku nie ma to utwórz
		fclose(stream);
	 return -1;}
}
/**
  Inicjuje i wczytuje słownik dla zadanego języka.
  Słownik ten należy zniszczyć za pomocą dictionary_done().
  @param[in] lang Nazwa języka, patrz dictionary_lang_list().
  @return Słownik dla danego języka lub NULL, jeśli operacja się nie powiedzie.
  */
struct dictionary * dictionary_load_lang(const char *lang)
{
	//Otworz strumien
	//Sprawdz czy jest taki na liscie
	//Wczytaj 
	//Zwroc
	char dict[256];
	char file[strlen(CONF_PATH)+259];
	char destination[strlen(CONF_PATH)+259];
	strcpy(destination,CONF_PATH);
	strcat(destination,"langs.txt");//Nazwa pliku ze slownikami
	printf("sciezka: %s",destination);
	//Czy plik ze sciezki istnieje
	if( access( destination, F_OK ) != -1 ) 
	{//Tak!
		//Otworz go do czytania
		FILE * stream= fopen(destination,"r");
		
		//Sprawdzaj az do konca pliku czy podany jezyk do wczytania jest na liscie
		int stop=0;
		while((stop!=1)&&(fgets(dict,256,stream)!=NULL))
		{
			dict[strlen(dict)-1]='\0';
			if(strcmp(dict,lang)==0) stop=1;
		}
		
		strcpy(file,CONF_PATH);
		strcat(file,lang);
		strcat(file,".txt");
			
		fclose(stream);
		
		if(stop==1)//Slownik w folderze
		{
			//Jeżeli plik ze slownikiem istnieje
			if( access( file, F_OK ) != -1 ) 
			{
				FILE * dict_lang= fopen(file,"r");
				struct dictionary *dict_new = (struct dictionary *)
							malloc(sizeof(struct dictionary));

				dict_new->root=trie_load(dict_lang); 
				fclose(dict_lang);
				//debug(dict_new->root);
				if (dict_new->root==NULL)//Nie udało się wczytać
				{//Posprzątaj
					dictionary_done(dict_new);
					dict_new = NULL;
				}
			return dict_new;
			}
			else return NULL;//Słownik nie w folderze
		}
		else return NULL;
	}
	else 
	{
		FILE * stream= fopen(destination,"w");//Jeśli takiego plikunie ma to utwórz
		fclose(stream);
		return NULL;
	}
}


/**
  Zapisuje słownik jak słownik dla ustalonego języka.
  @param[in] dict Słownik.
  @param[in] lang Nazwa języka, patrz dictionary_lang_list().
  @return <0 jeśli operacja się nie powiedzie, 0 w p.p.
  */
int dictionary_save_lang(const struct dictionary *dict, const char *lang)
{
	char dicton[256];
	char file[strlen(CONF_PATH)+259];
	strcpy(file,CONF_PATH);
	strcat(file,lang);
	strcat(file,".txt");
	
	char destination[strlen(CONF_PATH)+259];
	strcpy(destination,CONF_PATH);
	strcat(destination,"langs.txt");//Nazwa pliku ze slownikami
	
	
	
	FILE * stream= fopen(file,"w");
	
	if( access( destination, F_OK ) != -1 ) //Jezeli plik z lista slownikow istnieje
	{
		FILE * dict_lang= fopen(destination,"r");

		if(dictionary_save(dict,stream)==0)//Zapisz/nadpisz slownik o podanej nazwie
		{
		
			//Sprawdz czy slownik na liscie
			int stop=0;
			while((stop!=1)&&(fgets(dicton,256,dict_lang)!=NULL))
			{
				dicton[strlen(dicton)-1]='\0';
				if(strcmp(dicton,lang)==0) stop=1;
			}
			//printf("FILE88:%s\n",file);
			fclose(dict_lang);
			dict_lang= fopen(destination,"a");
			if(stop!=1)//Slownik nowy
				fprintf(dict_lang,"%s\n",lang);//dopisz do listy
			fclose(stream);
			fclose(dict_lang);
			return 0;
		}
		else return -1;//Nie udało się zapisać
	}
	else //Jezeli plik z lista slownikow nie istnieje
	{
		if(dictionary_save(dict,stream)==0)//Zapisz/nadpisz slownik o podanej nazwie
		{
			FILE * dict_lang= fopen(destination,"w");
			fprintf(dict_lang,"%s\n",lang);//dopisz do listy
			fclose(stream);
			fclose(dict_lang);
			return 0;
		}	
	}
}
 
 
 
char** lang_list( char * buffor, int size)
{int capacity=0; int pos=0;
		char ** array=(char**)malloc(sizeof(char*)); //Tablica wskaznikow na kolejne nazwy jezykow
		capacity=1;
		int i=0;
		
		

			array[pos]=&buffor[i]; pos++;
			i+=strlen(&buffor[i])+1;
			//Wczytuj znaki
			while(i<size)
			{
						//Włóż
						if(pos<capacity)
						{
							array[pos]=&buffor[i];
							pos++;
						}
						else
						{//Powiększ tablicę
							capacity*=2;
							array=realloc(array,sizeof(char*)*capacity);
							array[pos]=&buffor[i];
							pos++;
						}
						//	printf(" \n iii: %d \n",strlen(&buffor[i]));
				i+=strlen(&buffor[i])+1;
			}

	//printf(" \n hehal %s \n",array[0]);
	
	
	//*list=(char**)malloc(sizeof(char*)*pos);

//	printf(" \n heh %s x\n",array[0]);
//	printf(" \n heh %s x\n",array[1]);
	//for(int z=0;z<pos;z++)
		//*list[0]=array[0];
		//*list[0]="abc";
		//*list[1]="abc";
	//		printf(" \n heh %s y\n",array[1]);
			return array;
}                         






int dictionary_hints_max_cost(struct dictionary *dict, int new_cost)
{
	int old_cost= dict->K;
	dict->K=new_cost;
	return old_cost;
};



int dictionary_rule_add(struct dictionary *dict,
                        const wchar_t *left, const wchar_t *right,
                        bool bidirectional,
                        int cost,
                        enum rule_flag flag)
{
	//printf("%ls -> %ls %d %d \n", left,right, wcslen(left),wcslen(right));
	int poz_left=-1,poz_right=-1;
	int add=0;
	if(cost>0)
	{
		int vars_right[9];
		int vars_left[9];
		for(int i=0;i<10;i++){vars_right[i]=0; vars_left[i]=0;}
		int var_left=0, var_right=0;
		/*
		*     RULE_NORMAL, ///< Brak flagi.
		RULE_BEGIN,  ///< Flaga b.
		RULE_END,    ///< Flaga e.
		RULE_SPLIT   ///< Flaga s.
		*/ 
	

		int way,flag_num;

		flag_num=flag;
		if((wcslen(left)==0)&&(wcslen(right)==0)&&(flag_num!=3)) return 0;//Puste ale bez reguły s
		
		for(int i=0;i<wcslen(left);i++)
		{
			if((*(left+i)>=48)&&((*(left+i)<=57)))
			{
				if(vars_left[*(left+i)-48]==0) 
				{
					vars_left[*(left+i)-48]=1;
				}
			}
		}
		
		
		for(int i=0;i<wcslen(right);i++)
		{
			if((*(right+i)>=48)&&((*(right+i)<=57)))
			{
				if((vars_left[*(right+i)-48]==0) &&(vars_right[*(right+i)-48]==0))
				{
					var_right++;	
					if(poz_right==-1)
						poz_right=i;
				}
				if(vars_right[*(right+i)-48]==0) 
				{
					vars_right[*(right+i)-48]=1;
				}
			}
		}
		if((var_right)<2)
		{
		//Dodaj nową regułe
			rule *abc=malloc(sizeof(rule));
			abc->left=malloc(sizeof(wchar_t)*wcslen(left));
			abc->right=malloc(sizeof(wchar_t)*wcslen(right));
			wcscpy(abc->left,left);
			wcscpy(abc->right,right);
			abc->cost=cost;
			abc->flag=flag_num;
			//abc->var=-1;
			//if((var_right)>0)
			abc->var=poz_right;//Ze zmienna wolna
			//printf("%d  ",abc->var );
			rules_add(&dict->rules_list,abc);
			add++;
		}
		
		//Jeżeli dwukierunkowa
		if((bidirectional==true)&&!((wcslen(left)==0)&&(wcslen(right)==0)))
		{
			
			for(int i=0;i<wcslen(left);i++)
			{
				if((*(left+i)>=48)&&((*(left+i)<=57)))
				{
					if((vars_right[*(left+i)-48]==0) &&(vars_left[*(left+i)-48]==0))
					{
						var_left++;	
						if(poz_left==-1)
							poz_left=i;
					}
				}
			
			}
		
			if((var_left)<2)
			{
				//Dodaj nową regułe
				rule *abc=malloc(sizeof(rule));
				abc->left=malloc(sizeof(wchar_t)*wcslen(right));
				abc->right=malloc(sizeof(wchar_t)*wcslen(left));
				wcscpy(abc->left,right);
				wcscpy(abc->right,left);
				abc->cost=cost;
				abc->flag=flag_num;
				//abc->var=-1;
				//if((var_left)>0)
					abc->var=poz_left;//Ze zmienna wolna
				rules_add(&dict->rules_list,abc);
				add++;
			}
		}
	}
	return add;
}

void dictionary_rules_save(struct dictionary *dict, FILE *stream)
{
	rules_save(&dict->rules_list,stream);

}

/**@}*/
