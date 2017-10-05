/** @defgroup dict-tests Moduł dict-tests
    Testy jednostkowe spellchecker.
    Umożliwiają testowanie:
    * -# funkcji interfejsu dictionary, 
    * z wyjątkiem load, save oraz funkcji
    *  z rozszerzenia o podawanie języka.
    * -# implementacji użytych struktur danych.\n
    *	- trie.
    * 	- vector.
    * 	- alphabet.
    * -# funkcji używanych do wczytywania i wypisywania całego drzewa i pojedynczych jego fragmentów (np. węzła).
  */
/** @file
  Testy sprwdzające działanie funkcjonalności \ref dictionary.h.
  * Lista funkcjonalośći:
  * -# \ref dictionary_insert 
  * -# \ref dictionary_find
  * -# \ref dictionary_hints
  * -# \ref dictionary_new
  * -# \ref dictionary_done
  * -# \ref dictionary_delete
  * -# \ref file
  * UWAGA: dict_done testowane w większości testów
  @ingroup dict-tests
  @author Michał Jaroń
  @copyright Uniwerstet Warszawski 
  @date 2015-06-04
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include "dictionary.h"
#include <locale.h>

/**@}*/
/** @name Testy dictionary_new 
  @{
 */
///Czy słownikowi zostaje prawidłowo przydzielona pamięć
static void dict_new(void** state)
{
	struct dictionary *dict = dictionary_new();
	assert_non_null(dict);
	dictionary_done(dict);
}

/**@}*/

/**@}*/
/** @name Testy dictionary_insert 
  @{
 */

///Dodaj słowo do pustego słownika
static void dict_insert_empty(void** state)
{
	setlocale(LC_ALL, "pl_PL.UTF-8");
	const wchar_t* pl_test   = L"źdźbło";
	struct dictionary *dict = dictionary_new();
	assert_int_equal(dictionary_insert(dict,pl_test), 1);
	assert_true(dictionary_find(dict,pl_test));
	dictionary_done(dict);
}

///Dodaj słowo do niepustego słownika
static void dict_insert_existing(void** state)
{
    setlocale(LC_ALL, "pl_PL.UTF-8");
  
	//Słowa do wstawienia
	const wchar_t* pl_test = L"źdźbło";
	const wchar_t* test1   = L"źabc";
	const wchar_t* test2   = L"ghi";
	const wchar_t* test3   = L"abc";
	const wchar_t* test4   = L"ghfa";
	
	const wchar_t* find   = L"źabd";
	//Najpierw zbuduj drzewo
	struct dictionary *dict = dictionary_new();
	assert_int_equal(dictionary_insert(dict,pl_test), 1);
	
	//Dodawaj do niepustego drzewa
	assert_int_equal(dictionary_insert(dict,test1), 1);
	assert_int_equal(dictionary_insert(dict,test2), 1);
	assert_int_equal(dictionary_insert(dict,test3), 1);
	assert_int_equal(dictionary_insert(dict,test4), 1);
	
	//Czy słowa faktycznie w słowniku?
	assert_true(dictionary_find(dict,pl_test));
	assert_true(dictionary_find(dict,test1));
	assert_true(dictionary_find(dict,test2));
	assert_true(dictionary_find(dict,test3));
	assert_true(dictionary_find(dict,test4));
	
	
	//Słowo którego nie ma w słowniku
	assert_false(dictionary_find(dict,find));
	
	dictionary_done(dict);
}

///Dodaj słowo będace już w słowniku
static void dict_insert(void** state)
{
     setlocale(LC_ALL, "pl_PL.UTF-8");
     
	const wchar_t* test   = L"óąfęćńxa";
	struct dictionary *dict = dictionary_new();
	assert_int_equal(dictionary_insert(dict,test), 1);
	assert_true(dictionary_find(dict,test));
		
	//Nie wstawione! Drzewo już w słowniku!	
	assert_int_equal(dictionary_insert(dict,test), 0);
	assert_true(dictionary_find(dict,test));	
	
	dictionary_done(dict);	
}

///Dodaj słowo do wyczyszczonego słownika
static void dict_insert_cleared(void** state)
{
     setlocale(LC_ALL, "pl_PL.UTF-8");
  
	const wchar_t* test   = L"óąfęćńxa";
	struct dictionary *dict = dictionary_new();
	assert_int_equal(dictionary_insert(dict,test), 1);
	assert_true(dictionary_find(dict,test));
	
	//Czyść
	dictionary_done(dict);
	
	//Zacznij od nowa i sprawdź czy się nie psuje!
	dict = dictionary_new();	
	assert_int_equal(dictionary_insert(dict,test), 1);
	assert_true(dictionary_find(dict,test));		
	assert_int_equal(dictionary_insert(dict,test), 0);
	dictionary_done(dict);
}

///Dodaj słowo uprzednio usunięte
static void dict_insert_deleted(void** state)
{
	setlocale(LC_ALL, "pl_PL.UTF-8");
  
	const wchar_t* test   = L"óąfęćńxa";
	struct dictionary *dict = dictionary_new();
	//Dodaj
	assert_int_equal(dictionary_insert(dict,test), 1);
	assert_true(dictionary_find(dict,test));
	//Nie wstawi drugi raz
	assert_int_equal(dictionary_insert(dict,test), 0);
	//Usun
	dictionary_delete(dict,test);
	//Ale teraz już wstawi!
	assert_int_equal(dictionary_insert(dict,test), 1);
	dictionary_done(dict);
}

/**
	Dodaj słowo z kilkoma takimi samymi literami na końcu.
  * Na pozór absurdalny - pozwolił mi odnaleźć błąd
 */
static void dict_insert_same(void** state)
{
	setlocale(LC_ALL, "pl_PL.UTF-8");
  
	const wchar_t* test   = L"xxxx";
	const wchar_t* find   = L"xxx";
	const wchar_t* find2  = L"xxxxx";
	struct dictionary *dict = dictionary_new();
	assert_int_equal(dictionary_insert(dict,test), 1);
	assert_true(dictionary_find(dict,test));
		
	assert_false(dictionary_find(dict,find));	
	assert_false(dictionary_find(dict,find2));	
	dictionary_done(dict);
}

///Dodaj słowo jednoliterowe
static void dict_insert_letter(void** state)
{
	setlocale(LC_ALL, "pl_PL.UTF-8");
  
	const wchar_t* test1   = L"x";
	const wchar_t* test2  = L"ą";
	const wchar_t* test3  = L"ą";
	struct dictionary *dict = dictionary_new();
	
	assert_int_equal(dictionary_insert(dict,test1), 1);
	assert_true(dictionary_find(dict,test1));
	
	//Drugi raz nie wstawi
	assert_int_equal(dictionary_insert(dict,test1), 0);	
	assert_false(dictionary_find(dict,test2));
	
	//Wstaw pozostałe
	assert_int_equal(dictionary_insert(dict,test2), 1);
	assert_int_equal(dictionary_insert(dict,test3), 1);
	
	//Czy faktycznie wstawione?
	assert_true(dictionary_find(dict,test2));
	assert_true(dictionary_find(dict,test3));
	dictionary_done(dict);
}

/**
  Dodaj słowo będące prefixem słowa będącego już w słowniku.
  * Sprawdza czy program działa poprawnie gdy węzęł w drzewie
  * nie jest liściem, ale jest końcem słowa
  * "node->word=1"
 */
static void dict_insert_prefix(void** state)
{
	setlocale(LC_ALL, "pl_PL.UTF-8");
  
	const wchar_t* test   = L"ąbcąę";
	const wchar_t* insert   = L"ąbc";
	
	struct dictionary *dict = dictionary_new();
	assert_int_equal(dictionary_insert(dict,test), 1);
	assert_true(dictionary_find(dict,test));
	//Jeszcze nie ma słowa, więc insert
	assert_false(dictionary_find(dict,insert));
	assert_int_equal(dictionary_insert(dict,insert), 1);///<Dodaj słowo prefix
	assert_int_equal(dictionary_insert(dict,insert), 0);///<Spróbuj dodać drugi raz
	//Wyszukaj	
	assert_true(dictionary_find(dict,insert));
	assert_true(dictionary_find(dict,test));
	dictionary_done(dict);
}

///Dodaj słowo będące sufiksem słowa będącego już w słowniku.
static void dict_insert_sufix(void** state)
{
	setlocale(LC_ALL, "pl_PL.UTF-8");
  
	const wchar_t* test   = L"ąbc";
	const wchar_t* insert   = L"ąbcąę";
	
	struct dictionary *dict = dictionary_new();
	
	assert_int_equal(dictionary_insert(dict,test), 1);
	
	assert_true(dictionary_find(dict,test));
	assert_false(dictionary_find(dict,insert));
	
	assert_int_equal(dictionary_insert(dict,insert), 1);
	assert_int_equal(dictionary_insert(dict,insert), 0);///<Próba dodanie drugi raz	
	//Szukaj
	assert_true(dictionary_find(dict,insert));
	assert_true(dictionary_find(dict,test));
	dictionary_done(dict);
}

/**@}*/

/**@}*/
/** @name Testy dictionary_find
  @{
 */
///Szukaj słowa w pustym słowniku
static void dict_find_empty(void** state)
{
	const wchar_t* pl_test   = L"bączek";
	struct dictionary *dict = dictionary_new();
	
	//Nie ma!
	assert_false(dictionary_find(dict,pl_test));
	dictionary_done(dict);
}

///Szukaj słowa które jest w słowniku oraz takiego którego nie ma
static void dict_find(void** state)
{
	const wchar_t* pl_test   = L"onegdajóź";
	const wchar_t* find   = L"nxiemax";
	struct dictionary *dict = dictionary_new();
	//Wstaw pl_test
	assert_int_equal(dictionary_insert(dict,pl_test), 1);
	//Jest
	assert_true(dictionary_find(dict,pl_test));
	//Nie ma
	assert_false(dictionary_find(dict,find));
	dictionary_done(dict);
}

///Szukaj usuniętego słowa
static void dict_find_deleted(void** state)
{
	const wchar_t* pl_test   = L"ćueer";
	struct dictionary *dict = dictionary_new();
	assert_int_equal(dictionary_insert(dict,pl_test), 1);
	assert_true(dictionary_find(dict,pl_test));
	
	//Usuń słowo
	assert_int_equal(dictionary_delete(dict,pl_test), 1);
	//Wyszukaj ponownie
	assert_false(dictionary_find(dict,pl_test));
	dictionary_done(dict);
}

///Szukaj słowo wstawionego, następnie skasowanego i znowu dodanego
static void dict_find_deleted_inserted(void** state)
{
	const wchar_t* pl_test   = L"źdźbło";
	struct dictionary *dict = dictionary_new();
	assert_int_equal(dictionary_insert(dict,pl_test), 1);
	assert_true(dictionary_find(dict,pl_test));
	assert_int_equal(dictionary_delete(dict,pl_test), 1);
	assert_false(dictionary_find(dict,pl_test));
	assert_int_equal(dictionary_insert(dict,pl_test), 1);
	assert_true(dictionary_find(dict,pl_test));
	dictionary_done(dict);
}

///Kilka losowych słów dodanych i kilka losowych szukanych
static void dict_find_random(void** state)
{
	const wchar_t* test1   = L"ęgoofyą";
	const wchar_t* test2  = L"abcź";
	const wchar_t* test3   = L"xóęa";
	const wchar_t* test4   = L"aalxxx";
	const wchar_t* test5   = L"zż";
	const wchar_t* test6   = L"abcdefhiaaaz";
	const wchar_t* test7   = L"a";
	const wchar_t* test8  = L"zqqąłóp";
	const wchar_t* test9  = L"abc";
	const wchar_t* test0  = L"grek";
	
	
	struct dictionary *dict = dictionary_new();
	
	assert_int_equal(dictionary_insert(dict,test1), 1);
	assert_int_equal(dictionary_insert(dict,test2), 1);
	assert_int_equal(dictionary_insert(dict,test3), 1);
	assert_int_equal(dictionary_insert(dict,test4), 1);
	assert_int_equal(dictionary_insert(dict,test5), 1);
	assert_int_equal(dictionary_insert(dict,test6), 1);
	
	//Odnalezione
	assert_true(dictionary_find(dict,test1));
	assert_true(dictionary_find(dict,test2));	
	assert_true(dictionary_find(dict,test3));
	assert_true(dictionary_find(dict,test4));
	assert_true(dictionary_find(dict,test5));
	assert_true(dictionary_find(dict,test6));
	
	//Nie ma w słowniku
	assert_false(dictionary_find(dict,test7));
	assert_false(dictionary_find(dict,test8));
	assert_false(dictionary_find(dict,test9));
	assert_false(dictionary_find(dict,test0));

	dictionary_done(dict);
}

///Szukaj słowo jednoliterowego
static void dict_find_letter(void** state)
{
	const wchar_t* test1   = L"ę";
	const wchar_t* test2  = L"r";
	const wchar_t* test3   = L"x";
	
	struct dictionary *dict = dictionary_new();
	
	assert_int_equal(dictionary_insert(dict,test1), 1);
	assert_int_equal(dictionary_insert(dict,test2), 1);
	assert_true(dictionary_find(dict,test1));
	assert_true(dictionary_find(dict,test2));
	assert_false(dictionary_find(dict,test3));
	dictionary_done(dict);
}

/**Szukaj słowa będącego prefixem innego w słowniku.
* Sprawdza czy program działa poprawnie gdy węzęł w drzewie
  * nie jest liściem, ale jest końcem słowa
  * "node->word=1"
  *  */
static void dict_find_prefix(void** state)
{
	const wchar_t* pl_test   = L"abcdef";
	const wchar_t* prefix   = L"abc";
	struct dictionary *dict = dictionary_new();
	assert_int_equal(dictionary_insert(dict,pl_test), 1);
	assert_int_equal(dictionary_insert(dict,prefix), 1);
	assert_true(dictionary_find(dict,pl_test));
	assert_true(dictionary_find(dict,prefix));
	
	//Czy coś się nie psuje w podsłowach?
	assert_false(dictionary_find(dict,"abcd"));
	dictionary_done(dict);
}

///Szukaj słowa w wyczyszczonym słowniku
static void dict_find_cleared(void** state)
{
	const wchar_t* pl_test   = L"endrtół";
	struct dictionary *dict = dictionary_new();
	assert_int_equal(dictionary_insert(dict,pl_test), 1);
	assert_true(dictionary_find(dict,pl_test));
	dictionary_done(dict);
	//expect_assert_failure(dictionary_find(dict,pl_test));
	//assert_false(dictionary_find(dict,pl_test));
	//expect_assert_failure(
	
}

/**@}*/

/**@}*/
/** @name Testy dictionary_hints
  @{
 */

///Słowa niezawarte w slowniku, losowe.
static void dict_hints_not(void** state)
{
	const wchar_t* test1   = L"ęgoofyą";
	const wchar_t* test2  = L"abcź";
	const wchar_t* test3   = L"xóęa";
	const wchar_t* test4   = L"aalxxx";
	const wchar_t* test5   = L"zż";
	
	const wchar_t* hint1   = L"abcdefhiaaaz";///<Za długie na transformacje ze słowem już wstawionym
	const wchar_t* hint2   = L"a";///<Krótkie
	const wchar_t* hint3  = L"lmp";
	
	
	struct word_list list;
	
	struct dictionary *dict = dictionary_new();
    dictionary_hints(dict, hint1, &list);
    const wchar_t * const *a = word_list_get(&list);
    
    //Nie ma hintsów, bo nic nie pasuje
	assert_int_equal(word_list_size(&list), 0);
	dictionary_done(dict);
}



///Slownik pusty
static void dict_hints_empty(void** state)
{
	const wchar_t* test   = L"ęgoofyą";
	
	struct word_list list;
	
	struct dictionary *dict = dictionary_new();
    dictionary_hints(dict, test, &list);
    const wchar_t * const *a = word_list_get(&list);
    //Słownik pusty --> lista pusta
	assert_int_equal(word_list_size(&list), 0);
	dictionary_done(dict);
}

/*
/**Test sortowania znaków
 * Insertuje caly polski alfabet
 * nastepnie szukam hintsow dla pojedynczej litery
 * sprawdzam kolejnosc alfabetyczna podanych hintsow
 */ 
 /*
static void dict_hints_sort(void** state)
{
	
     setlocale(LC_ALL, "pl_PL.UTF-8");
const wchar_t polski_big[]= L"AĄBCĆDEĘFGHIJKLŁMNŃOÓPRSŚTUWYZŹŻ";
const wchar_t polski_small[]= L"aąbcćdeęfghijklłmnńoóprsśtuwyzźż";

	const wchar_t test   = L"x";
	struct dictionary *dict = dictionary_new();
	for(int i=0;i<wcslen(polski_small);i++)
	{
	
		wchar_t wc = *(polski_small+i);
			//printf("a%ls\n",wc);
			assert_int_equal(dictionary_insert(dict,&wc), 1);
		
	}
	
	assert_true(dictionary_find(dict,L"a"));
	//Sprawdź kolejność alfabetyczną
	
	/*
		struct word_list list;
                dictionary_hints(dict, test, &list);
                assert_int_equal(word_list_size(&list), wcslen(polski_small));///<Sprawdz liczbe podpowiedzi
               //	printf("LITERKA:%d", word_list_size(&list));
                const wchar_t * const *a = word_list_get(&list);
                for (size_t i = 0; i < word_list_size(&list); ++i)
                {
					
					wchar_t wc = *(polski_small+i);
					wchar_t ac = *(a[i]);
					printf("LITERKA:%lc %lc", wc, ac);
                    assert_int_equal(ac, wc);
                }*/
                /*dictionary_done(dict);
}
*/

/**Test generowanych hintsow
 * Insertuje losowe słowa
 * nastepnie szukam hintsow losowych słów
 * sprawdzam poprawność wygenerowanych hintsów
 */ 
static void dict_hints_random(void** state)
{
	
     setlocale(LC_ALL, "pl_PL.UTF-8");
     
     wchar_t * ins[]={L"óbć",L"ąxbć",  L"xbć",L"óąbć",L"ąb", L"ąbb", L"óąbć",L"ąbbć", L"ąbćb" ,L"xbć",L"ąbćó", 
		   L"ąbćx",  L"xbć",L"ąbó", L"ąbóć", L"ąbx", L"ąbxć" ,L"ąć", 
		 L"ąóć",  L"ąxć",L"ąxbć", L"bąbć", L"bbć", L"bć",   L"xąbć",  L"ąóbć",  L"ąxć"};
		 
		 
     wchar_t * answers[]={L"ąb", L"ąbb", L"ąbbć", L"ąbćb" ,L"ąbćó", L"ąbćx", L"ąbó", L"ąbóć", L"ąbx", L"ąbxć" ,L"ąć", 
		 L"ąóbć", L"ąóć", L"ąxbć", L"ąxć", L"bąbć", L"bbć", L"bć", L"óąbć", L"óbć", L"xąbć", L"xbć"};
		 
		 
     //Dla alfabetu skladajacego sie z 3 liter: óxb
	//ąb ąbb ąbbć ąbćb ąbćó ąbćx ąbó ąbóć ąbx ąbxć ąć ąóbć ąóć ąxbć ąxć bąbć bbć bć óąbć óbć xąbć xbć
	struct dictionary *dict = dictionary_new();
	
	for(int i=0;i<22;i++)
		dictionary_insert(dict,answers[i]);

		
	for(int i=0;i<22;i++)
		assert_true(dictionary_find(dict,answers[i]));

	struct word_list list;
	const wchar_t test = L"ąbć";

	dictionary_hints(dict, test, &list);
	assert_int_equal(word_list_size(&list),22);///<Sprawdz liczbe podpowiedzi
	const wchar_t * const *a = word_list_get(&list);
	for (size_t i = 0; i < word_list_size(&list); ++i)
		assert_int_equal(wcscmp(answers[i], a[i]),0);
		
		dictionary_done(dict);
}

///Test poprawnosciowy te same litery- czy wypisuje bez powtorzen.
static void dict_hints_same(void** state)
{
	setlocale(LC_ALL, "pl_PL.UTF-8");

	const wchar_t* test1   = L"xxx";
	const wchar_t* test2   = L"xx";
	const wchar_t* test3   = L"xxxx";

	struct dictionary *dict = dictionary_new();

	assert_int_equal(dictionary_insert(dict,test1), 1);
	assert_int_equal(dictionary_insert(dict,test2), 1);
	assert_int_equal(dictionary_insert(dict,test3), 1);
	
	const wchar_t* test   = L"xxx";
	struct word_list list;
	dictionary_hints(dict, test, &list);
	assert_int_equal(word_list_size(&list),3);///<Sprawdz liczbe podpowiedzi
	const wchar_t * const *a = word_list_get(&list);
        
    //Sprawdź co wypisało           
	assert_int_equal(wcscmp(test2, a[0]),0);
	assert_int_equal(wcscmp(test1, a[1]),0);
	assert_int_equal(wcscmp(test3, a[2]),0); 
	
	dictionary_done(dict);      
}

/**@}*/

/**@}*/
/** @name Testy dictionary_delete
  @{
 */

///Usun slowo z pustego slownika
static void dict_delete_empty_word(void** state)
{
	const wchar_t* test   = L"test";
	const wchar_t* pl_test   = L"źdźbło";
	struct dictionary *dict = dictionary_new();
	
	//Nie usuwa, bo słownik pusty
    assert_int_equal(dictionary_delete(dict,test), 0);
    assert_int_equal(dictionary_delete(dict,pl_test), 0);
    
    dictionary_done(dict);
}

///Usun slowo, będące jednym znakiem, z pustego slownika
static void dict_delete_empty_letter(void** state)
{
	const wchar_t* test   = L"x";
	const wchar_t* pl_test   = L"ą";
	struct dictionary *dict = dictionary_new();
    assert_int_equal(dictionary_delete(dict,test), 0);
    assert_int_equal(dictionary_delete(dict,pl_test), 0);
    
    dictionary_done(dict);
}

///Usun losowe slowo z losowego słownika
static void dict_delete(void** state)
{
	 setlocale(LC_ALL, "pl_PL.UTF-8");
  
	const wchar_t* test1   = L"óąfęćńxa";
	const wchar_t* test2   = L"abc";
	const wchar_t* test3   = L"xxxxóxxxx";
	const wchar_t* test4   = L"ólll";
	const wchar_t* test5   = L"ala";
	const wchar_t* test6   = L"abcc";
	
	struct dictionary *dict = dictionary_new();
	assert_int_equal(dictionary_insert(dict,test1), 1);
	assert_int_equal(dictionary_insert(dict,test2), 1);
	assert_int_equal(dictionary_insert(dict,test3), 1);
	assert_int_equal(dictionary_insert(dict,test4), 1);
	
    assert_int_equal(dictionary_delete(dict,test1), 1);
    assert_int_equal(dictionary_delete(dict,test2), 1);
    assert_int_equal(dictionary_delete(dict,test3), 1);
    assert_int_equal(dictionary_delete(dict,test4), 1);
    
    //Nie usuwia bo nie wstawione
	assert_int_equal(dictionary_delete(dict,test5), 0);
    assert_int_equal(dictionary_delete(dict,test6), 0);
    
    dictionary_done(dict);
}

///Usuń jednoliterowe
static void dict_delete_letter(void** state)
{
	const wchar_t* test1   = L"x";
	const wchar_t* test2   = L"y";
	struct dictionary *dict = dictionary_new();
	assert_int_equal(dictionary_insert(dict,test1), 1);

    assert_int_equal(dictionary_delete(dict,test1), 1);
    assert_int_equal(dictionary_delete(dict,test2), 0);
    
    dictionary_done(dict);
}


///Usun prefix
static void dict_delete_prefix(void** state)
{
	const wchar_t* test1   = L"ąbcąę";
	const wchar_t* test2   = L"ąbc";
	
	struct dictionary *dict = dictionary_new();
	assert_int_equal(dictionary_insert(dict,test1), 1);
	assert_int_equal(dictionary_insert(dict,test2), 1);
	assert_int_equal(dictionary_delete(dict,test2), 1);
	assert_true(dictionary_find(dict,test1));
	//Już nie ma w słowniku
	assert_false(dictionary_find(dict,test2));
	
	dictionary_done(dict);
}

///Usuń sufiks
static void dict_delete_sufix(void** state)
{
	const wchar_t* test1   = L"ąbcąę";
	const wchar_t* test2   = L"ąbc";
	
	struct dictionary *dict = dictionary_new();
	assert_int_equal(dictionary_insert(dict,test1), 1);
	assert_int_equal(dictionary_insert(dict,test2), 1);
	assert_int_equal(dictionary_delete(dict,test1), 1);
	assert_true(dictionary_find(dict,test2));
	
	//Już nie ma w słowniku
	assert_false(dictionary_find(dict,test1));
	dictionary_done(dict);
}


/**@}*/


/**@}*/
/** @name Testy dictionary_done 
  @{
 */
///Czy pusty słownik zostaje prawidłowo wyczyszczony
 static void dict_done_empty(void** state)
{
	struct dictionary *dict = dictionary_new();
	assert_non_null(dict);
	dictionary_done(dict);
	//assert_null(dict);
}
 
///Czy niepusty słownik zostaje prawidłowo wyczyszczony
 static void dict_done(void** state)
{
	struct dictionary *dict = dictionary_new();
	assert_non_null(dict);
	const wchar_t* pl_test   = L"źdźbło";

	assert_int_equal(dictionary_insert(dict,pl_test), 1);

	dictionary_done(dict);
	dict=NULL;
	assert_null(dict);
}

///Czy nie ma błędu przy próbie wyczyszczenie uprzednio usuniętego słownika.
static void dict_done_cleared(void** state)
{
	struct dictionary *dict = dictionary_new();
	assert_non_null(dict);
	dictionary_done(dict);
	dict=NULL;
	assert_null(dict);
	dictionary_done(dict);
	dict=NULL;
	assert_null(dict);
}

/**@}*/

int main(void) {

    const struct CMUnitTest tests[] = {
			cmocka_unit_test(dict_new),
			
			cmocka_unit_test(dict_done_empty),
		
			cmocka_unit_test(dict_insert_empty),
			cmocka_unit_test(dict_insert_existing),
			cmocka_unit_test(dict_insert),
			cmocka_unit_test(dict_insert_cleared),
			cmocka_unit_test(dict_insert_deleted),
			cmocka_unit_test(dict_insert_same),
			cmocka_unit_test(dict_insert_same),
			cmocka_unit_test(dict_insert_prefix),
			cmocka_unit_test(dict_insert_sufix),  
			
			cmocka_unit_test(dict_find_empty),  
			cmocka_unit_test(dict_find),  
			cmocka_unit_test(dict_find_deleted), 
			cmocka_unit_test(dict_find_deleted_inserted),  
			cmocka_unit_test(dict_find_random),
			cmocka_unit_test(dict_find_letter),
			cmocka_unit_test(dict_find_prefix),
			cmocka_unit_test(dict_find_cleared),
			
			cmocka_unit_test(dict_hints_empty),
			cmocka_unit_test(dict_hints_not),
			cmocka_unit_test(dict_hints_random),
			cmocka_unit_test(dict_hints_same),
			
			cmocka_unit_test(dict_delete_empty_word),
			cmocka_unit_test(dict_delete_empty_letter),
			cmocka_unit_test(dict_delete),
			cmocka_unit_test(dict_delete_letter),
			cmocka_unit_test(dict_delete_prefix),
			cmocka_unit_test(dict_delete_sufix),
    };

    return cmocka_run_group_tests(tests,NULL, NULL);
}
