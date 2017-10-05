/** @file
  Testy sprwdzające działanie struktury trie z \ref trie.h.
  * -# \ref find
  * -# \ref node_delete
  * -# \ref trie_clear
  * -# \ref trie_find
  * -# \ref trie_init
  * -# \ref trie_insert
  * trie_find i find testowane razem
  * trie_clear zawiera się w większości testów
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
#include "trie.h"
#include <locale.h>

/**@}*/
/** @name Test trie_init 
  @{
 */
 
///Test tworzenia nowego drzewa i inicjalizacji wartości korzenia
static void trie_init_test(void** state) {
	tree root; 
	trie_init(&root,NULL,'\0');
	assert_int_equal(root->children->size, 0);
	assert_int_equal((root)->word, 0);
	assert_true(root->letter=='\0');
	assert_null(root->parent);
	
	trie_clear(&root);
}

/**@}*/

/**@}*/
/** @name Testy  trie_insert
  @{
 */
 
///Włóż słowo do pustego drzewa i sprawdź ręcznie strukturę 
static void trie_insert_empty(void** state) {
	
	tree wsk;
     setlocale(LC_ALL, "pl_PL.UTF-8");
	tree root; alphabet alphabet;   alphabet.size=0;
	const wchar_t* test1   = L"ąbcąę";
	trie_init(&root,NULL,'\0');
	assert_int_equal(trie_insert(test1,&root,&alphabet), 1);
	
	//Sprawdzam ręcznie struktrę drzewa
	assert_int_equal(alphabet.size,4);
	
	assert_true(alphabet.items[0]==L'ą');
	assert_true(alphabet.items[1]==L'b');
	assert_true(alphabet.items[2]==L'c');
	assert_true(alphabet.items[3]==L'ę');

	//Kolejne węzły
	assert_true(root->letter=='\0');
	assert_int_equal(root->children->size,1);
	assert_int_equal((root)->word, 0);
	wsk=root->children->items[0];
	
	assert_true(wsk->letter==L'ą');
	assert_int_equal(wsk->children->size,1);
	assert_int_equal((wsk)->word, 0);
	wsk=wsk->children->items[0];
	
	
	assert_true(wsk->letter==L'b');
	assert_int_equal(wsk->children->size,1);
	assert_int_equal((wsk)->word, 0);
	wsk=wsk->children->items[0];
	
	
	assert_true(wsk->letter==L'c');
	assert_int_equal(wsk->children->size,1);
	assert_int_equal((wsk)->word, 0);
	wsk=wsk->children->items[0];
	
	
	assert_true(wsk->letter==L'ą');
	assert_int_equal(wsk->children->size,1);
	assert_int_equal((wsk)->word, 0);
	wsk=wsk->children->items[0];
	
	
	assert_true(wsk->letter==L'ę');
	assert_int_equal(wsk->children->size,0);
	assert_int_equal((wsk)->word, 1);
	trie_clear(&root);
	//free(*(alphabet.items[0]));
	/*if(alphabet.size!=0)
	{
		for(int i=0;i<alphabet.size;i++)
			free(alphabet.items[i]);
	}*/
}

///Włóż słowo do niepustego drzewa i sprawdź ręcznie strukturę 
static void trie_insert_non_empty(void** state) {
	
	tree root;  alphabet alphabet;   alphabet.size=0;

	trie_init(&root,NULL,'\0');
	const wchar_t* test1   = L"ąbcąę";
	const wchar_t* test2   = L"xghi";
	const wchar_t* test3   = L"ąłp";
	const wchar_t* test4   = L"ollofą";
	trie_init(&root,NULL,'\0');
	//Najpierw buduj drzewo
	assert_int_equal(trie_insert(test1,&root,&alphabet), 1);
	assert_int_equal(trie_insert(test2,&root,&alphabet), 1);
	assert_int_equal(trie_insert(test3,&root,&alphabet), 1);
	assert_int_equal(trie_insert(test4,&root,&alphabet), 1);
	
	const wchar_t* insert   = L"abc";
	//Teraz na niepustym drzewie
	assert_int_equal(trie_insert(insert,&root,&alphabet), 1);
	assert_int_equal(trie_insert(insert,&root,&alphabet), -1);
	
	assert_int_equal(alphabet.size,14);
	assert_int_equal(root->children->size, 4);
	//Ręcznie sprawdzam strukturę
	tree wsk;
	assert_true(root->letter=='\0');
	assert_int_equal((root)->word, 0);
	wsk=root->children->items[0];
	
	assert_true(wsk->letter==L'a');
	assert_int_equal(wsk->children->size,1);
	assert_int_equal((wsk)->word, 0);
	wsk=wsk->children->items[0];
	
	
	assert_true(wsk->letter==L'b');
	assert_int_equal(wsk->children->size,1);
	assert_int_equal((wsk)->word, 0);
	wsk=wsk->children->items[0];
	
	
	assert_true(wsk->letter==L'c');
	assert_int_equal(wsk->children->size,0);
	assert_int_equal((wsk)->word, 1);
	//free(&(alphabet.items[0]));
	
	trie_clear(&root);
}


///Włóż słowo będące prefixem innego słowa, będącego już w drzewie
static void trie_insert_prefix(void** state) {
	const wchar_t* test1   = L"abęłj";
	const wchar_t* prefix1  = L"abę";
	const wchar_t* prefix2  = L"abx";
	tree root; alphabet alphabet;   alphabet.size=0;

	trie_init(&root,NULL,'\0');
	assert_int_equal(trie_insert(test1,&root,&alphabet), 1);
	assert_int_equal(trie_insert(prefix1,&root,&alphabet), 1);
	assert_int_equal(trie_insert(prefix1,&root,&alphabet), -1);
	assert_int_equal(trie_insert(test1,&root,&alphabet), -1);
	
	//Ręcznie sprawdzam strukturę
	tree wsk;
	assert_int_equal(root->children->size, 1);
	assert_true(root->letter=='\0');
	assert_int_equal((root)->word, 0);
	wsk=root->children->items[0];
	
	assert_true(wsk->letter==L'a');
	assert_int_equal(wsk->children->size,1);
	assert_int_equal((wsk)->word, 0);
	wsk=wsk->children->items[0];
	
	
	assert_true(wsk->letter==L'b');
	assert_int_equal(wsk->children->size,1);
	assert_int_equal((wsk)->word, 0);
	wsk=wsk->children->items[0];
	
	
	assert_true(wsk->letter==L'ę');
	assert_int_equal(wsk->children->size,1);
	assert_int_equal((wsk)->word, 1);
	
	
	assert_int_equal(trie_insert(prefix2,&root,&alphabet), 1);
	assert_int_equal(root->children->size, 1);
	assert_true(root->letter=='\0');
	assert_int_equal((root)->word, 0);
	wsk=root->children->items[0];
	
	assert_true(wsk->letter==L'a');
	assert_int_equal(wsk->children->size,1);
	assert_int_equal((wsk)->word, 0);
	wsk=wsk->children->items[0];
	
	
	assert_true(wsk->letter==L'b');
	assert_int_equal(wsk->children->size,2);
	assert_int_equal((wsk)->word, 0);
	wsk=wsk->children->items[1];
	
	assert_true(wsk->letter==L'x');
	assert_int_equal(wsk->children->size,0);
	assert_int_equal((wsk)->word, 1);
	//free(alphabet.items);
	trie_clear(&root);
}


///Włóż słowo będące sufiksem innego słowa, będącego już w drzewie
static void trie_insert_sufiks(void** state) {
	const wchar_t* test1   = L"ab";
	const wchar_t* prefix1  = L"abcd";
	const wchar_t* prefix2  = L"abx";
	tree root; alphabet alphabet;   alphabet.size=0;

	trie_init(&root,NULL,'\0');
	assert_int_equal(trie_insert(test1,&root,&alphabet), 1);
	assert_int_equal(trie_insert(prefix1,&root,&alphabet), 1);
	assert_int_equal(trie_insert(prefix1,&root,&alphabet), -1);
	assert_int_equal(trie_insert(test1,&root,&alphabet), -1);
	assert_int_equal(trie_insert(prefix2,&root,&alphabet), 1);
	assert_int_equal(trie_insert(prefix2,&root,&alphabet), -1);
	
	//Ręcznie sprawdzam strukturę
	tree wsk; tree wsk2;
	assert_int_equal(root->children->size, 1);
	assert_true(root->letter=='\0');
	assert_int_equal((root)->word, 0);
	wsk=root->children->items[0];
	
	assert_true(wsk->letter==L'a');
	assert_int_equal(wsk->children->size,1);
	assert_int_equal((wsk)->word, 0);
	wsk=wsk->children->items[0];
	
	
	assert_true(wsk->letter==L'b');
	assert_int_equal(wsk->children->size,2);
	assert_int_equal((wsk)->word, 1);
	wsk2=wsk->children->items[1];
	wsk=wsk->children->items[0];
	
	
	assert_true(wsk->letter==L'c');
	assert_int_equal(wsk->children->size,1);
	assert_int_equal((wsk)->word, 0);
	wsk=wsk->children->items[0];
	
	assert_true(wsk->letter==L'd');
	assert_int_equal(wsk->children->size,0);
	assert_int_equal((wsk)->word, 1);
	
	
	assert_true(wsk2->letter==L'x');
	assert_int_equal(wsk2->children->size,0);
	assert_int_equal((wsk2)->word, 1);
	//free(alphabet.items);
	trie_clear(&root);
}


/**@}*/

/**@}*/
/** @name Testy find i trie_find
  @{
 */
 
///Szukaj słowo w pustym drzewie
static void trie_find_empty(void** state) {
		
     setlocale(LC_ALL, "pl_PL.UTF-8");
	tree root; 
	const wchar_t* test1   = L"xxx";
	trie_init(&root,NULL,'\0');

	//Drzewo puste, więc:
	assert_int_equal(trie_find(test1,root), -1);
	assert_null(find(test1,root,0,wcslen(test1)-1));
	////free(alphabet.items);
	trie_clear(&root);
}

///Szukaj słowo w niepustym drzewie
static void trie_find_non_empty(void** state) {

	tree wsk;  setlocale(LC_ALL, "pl_PL.UTF-8");
	tree root; alphabet alphabet;   alphabet.size=0;
	const wchar_t* test1   = L"xxx";
	
	const wchar_t* find1   = L"xxx";
	const wchar_t* find2   = L"xxxx";
	const wchar_t* find3   = L"ąębć";
	const wchar_t* find4   = L"xx";
	trie_init(&root,NULL,'\0');

	assert_int_equal(trie_insert(test1,&root,&alphabet), 1);
	//Jest!
	assert_int_equal(trie_find(find1,root), 1);
	//Nie ma!
	assert_int_equal(trie_find(find2,root), -1);
	assert_int_equal(trie_find(find3,root), -1);
	assert_int_equal(trie_find(find4,root), -1);
	
	//Sprawdzaj wskazniki zwracane
	assert_null(find(find2,root,0,wcslen(find2)-1));
	assert_null(find(find3,root,0,wcslen(find3)-1));
	assert_null(find(find4,root,0,wcslen(find4)-1));
	assert_non_null(find(find1,root,0,wcslen(find1)-1));
	trie_clear(&root);
}

///Szukaj słowo będacego prefixem innego słowa z drzewa
static void trie_find_prefix(void** state){
	const wchar_t* test1   = L"alamakota";
	const wchar_t* prefix1  = L"alama";
	const wchar_t* prefix2  = L"albo";
	
	setlocale(LC_ALL, "pl_PL.UTF-8");
	tree root;  alphabet alphabet;   alphabet.size=0;
	trie_init(&root,NULL,'\0');

	assert_int_equal(trie_insert(test1,&root,&alphabet), 1);
	assert_int_equal(trie_find(test1,root), 1);
	assert_int_equal(trie_find(prefix1,root), -1);
	assert_int_equal(trie_find(prefix2,root), -1);
	
	assert_int_equal(trie_insert(prefix1,&root,&alphabet), 1);
	assert_int_equal(trie_insert(prefix2,&root,&alphabet), 1);
	
	assert_int_equal(trie_find(prefix1,root), 1);
	assert_int_equal(trie_find(prefix2,root), 1);
	//free(alphabet.items);
	trie_clear(&root);
}

///Szukaj słowo będacego sufiksem innego słowa z drzewa
static void trie_find_sufiks(void** state){
	const wchar_t* test1   = L"ala";
	const wchar_t* prefix1  = L"alama";
	const wchar_t* prefix2  = L"alół";
	
	setlocale(LC_ALL, "pl_PL.UTF-8");
	tree root;  alphabet alphabet;   alphabet.size=0;
	trie_init(&root,NULL,'\0');

	assert_int_equal(trie_insert(test1,&root,&alphabet), 1);
	assert_int_equal(trie_find(test1,root), 1);
	assert_int_equal(trie_find(prefix1,root), -1);
	assert_int_equal(trie_find(prefix2,root), -1);
	
	assert_int_equal(trie_insert(prefix1,&root,&alphabet), 1);
	assert_int_equal(trie_insert(prefix2,&root,&alphabet), 1);
	
	assert_int_equal(trie_find(prefix1,root), 1);
	assert_int_equal(trie_find(prefix2,root), 1);
	//free(alphabet.items);
	trie_clear(&root);
	
}
/**@}*/



/**@}*/
/** @name Testy node_delete
  @{
 */
 

///Test node_delete - sprobuj skasować nieistniejący wyraz
static void trie_delete_no(void** state) {
	
	tree wsk;
	setlocale(LC_ALL, "pl_PL.UTF-8");
	tree root; alphabet alphabet;   alphabet.size=0;
	const wchar_t* test1   = L"ąbcąę";
	const wchar_t* test2   = L"xxx";
	trie_init(&root,NULL,'\0');
	assert_int_equal(trie_insert(test1,&root,&alphabet), 1);
	assert_int_equal(node_delete(&root,test2), -1);
	assert_int_equal(trie_find(test1,root), 1);
	
	
}

///Test node_delete - sprobuj skasować istniejący wyraz
static void trie_delete_test(void** state) {
	
	tree wsk;
	setlocale(LC_ALL, "pl_PL.UTF-8");
	tree root; alphabet alphabet;   alphabet.size=0;
	const wchar_t* test1   = L"ąbcąę";
	const wchar_t* test2   = L"xxx";
	trie_init(&root,NULL,'\0');
	assert_int_equal(trie_insert(test1,&root,&alphabet), 1);
	assert_int_equal(trie_insert(test2,&root,&alphabet), 1);
	assert_int_equal(node_delete(&root,test1), 0);
	assert_int_equal(trie_find(test1,root), -1);
}


///Test node_delete - sprobuj skasować z pustego drzewa
static void trie_delete_empty(void** state) {
	
	tree root; alphabet alphabet;   alphabet.size=0;
	const wchar_t* test1   = L"ąbcąę";
	const wchar_t* test2   = L"xxx";
	trie_init(&root,NULL,'\0');
	assert_int_equal(node_delete(&root,test1), -1);
	
}


///Test node_delete - sprobuj skasować bedacy prefixem oraz sufiksem
static void trie_delete_pre_suf(void** state) {
	
	tree wsk;
	setlocale(LC_ALL, "pl_PL.UTF-8");
	tree root; alphabet alphabet;   alphabet.size=0;
	const wchar_t* test1   = L"ąbcąęć";
	const wchar_t* test2  = L"ąbc";
	const wchar_t* test3   = L"abcąęćxxx";
	trie_init(&root,NULL,'\0');
	assert_int_equal(trie_insert(test1,&root,&alphabet), 1);
	assert_int_equal(trie_insert(test2,&root,&alphabet), 1);
	assert_int_equal(trie_insert(test3,&root,&alphabet), 1);
	
	
	assert_int_equal(node_delete(&root,test2), 0);
	assert_int_equal(node_delete(&root,test3), 0);
	assert_int_equal(trie_find(test1,root), 1);
	
}

///Test node_delete - sprobuj skasować jedną literę
static void trie_delete_letter(void** state) {
	
	tree wsk;
	setlocale(LC_ALL, "pl_PL.UTF-8");
	tree root; alphabet alphabet;   alphabet.size=0;
	const wchar_t* test1   = L"ą";

	trie_init(&root,NULL,'\0');
	assert_int_equal(trie_insert(test1,&root,&alphabet), 1);
	assert_int_equal(trie_find(test1,root), 1);
	assert_int_equal(node_delete(&root,test1), 0);
	assert_int_equal(trie_find(test1,root), -1);
}



/**@}*/


int main(void) {

    const struct CMUnitTest tests[] = {
			cmocka_unit_test(trie_init_test),
			
			cmocka_unit_test(trie_insert_empty),
			cmocka_unit_test(trie_insert_non_empty),
			cmocka_unit_test(trie_insert_prefix),
			cmocka_unit_test(trie_insert_sufiks),
			
			
			cmocka_unit_test(trie_find_empty),
			cmocka_unit_test(trie_find_non_empty),
			cmocka_unit_test(trie_find_prefix),
			cmocka_unit_test(trie_find_sufiks),
			
			cmocka_unit_test(trie_delete_no),
			cmocka_unit_test(trie_delete_test),
			cmocka_unit_test(trie_delete_empty),
			cmocka_unit_test(trie_delete_pre_suf),
			cmocka_unit_test(trie_delete_letter),
		
    };

    return cmocka_run_group_tests(tests,NULL, NULL);
}
