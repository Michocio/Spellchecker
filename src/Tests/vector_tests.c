/** @file
  Testy sprwdzające działanie struktury vector z \ref vector.h.
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
#include "vector.h"
#include <locale.h>
 
 
 
  /**@}*/
/** @name Test vector_init 
  @{
 */
 
///Test inicjalizacji vectora
static void vector_init_test(void** state) {
	//Najpierw utwórz przykładowy wezęł
	tree root; 
	trie_init(&root,NULL,'\0');
	
	
	tree node; 
	trie_init(&node,NULL,L"a");
	
	vector_init(root->children,node);
	assert_int_equal(root->children->size,1);
	assert_true(root->children->items[0]==node);
	//trie_clear(&node);
	trie_clear(&root);
	//trie_clear(&node);
}
/**@}*/

  /**@}*/
/** @name Test vector_resize 
  @{
 */
 ///Test zwiększania pojemności drzewa
static void vector_resize_test(void** state) {
	
	tree root; 
	trie_init(&root,NULL,'\0');
	
	//Najpierw utwórz przykładowy wezęł
	tree node; 
	trie_init(&node,NULL,L"a");
	
	
	vector_init(root->children,node);
	assert_int_equal(root->children->size,1);
	vector_resize(root->children);
	assert_int_equal(root->children->size,2);
	//free(&root->children->items[0]);
	//free(&root->children->items[1]);
	//vector_done(&root->children);
	//trie_clear(&root);
	//trie_clear(&node);
}

 /**@}*/

  /**@}*/
/** @name Test vector_resize_minus
  @{
 */
 ///Test zmniejszania pojemności drzewa
static void vector_resize_minus_test(void** state) {
	
	tree root; 
	trie_init(&root,NULL,'\0');
	
	//Najpierw utwórz przykładowy wezęł
	tree node; 
	trie_init(&node,NULL,L"a");
	
	vector_init(root->children,node);
	assert_int_equal(root->children->size,1);
	vector_resize_minus(root->children);
	assert_int_equal(root->children->size,0);
//	vector_init(root->children,node);
	trie_clear(&root);
	trie_clear(&node);
}

 /**@}*/
 

 /** @name Test vector_resize_minus
  @{
 */
 ///Test dodawania nowego wezla do pustego vektora
static void vector_add_empty(void** state) {
	
	tree root; 
	trie_init(&root,NULL,'\0');
	
	//Najpierw utwórz przykładowy wezęł
	tree node; 
	trie_init(&node,NULL,L"a");

	vector_add(root->children,node);
	assert_int_equal(root->children->size,1);
	trie_clear(&root);
	//trie_clear(&node);
}


///Test dodawania nowego węzła do niepustego vektora
static void vector_add_test(void** state) {
	
	tree root; 
	trie_init(&root,NULL,'\0');
	
	//Najpierw utwórz przykładowe wezęły
	tree node; 
	trie_init(&node,NULL,L"a");
	tree node2; 
	trie_init(&node2,NULL,L"b");
	
	
	
	
	vector_init(root->children,node);
	assert_int_equal(root->children->size,1);
	vector_add(root->children,node2);
	assert_int_equal(root->children->size,2);
	
	assert_true(root->children->items[0]->letter==L"a");
	assert_true(root->children->items[1]->letter==L"b");
	trie_clear(&root);
	//trie_clear(&node);
	//trie_clear(&node2);
}


///Test dodawania nowego węzła do niepustego vektora z polskimi literami
static void vector_add_test_pl(void** state) {
	   setlocale(LC_ALL, "pl_PL.UTF-8");
	   
	  tree root; 
	trie_init(&root,NULL,'\0'); 
	   
	   
	const wchar_t *test   = L"ąbćg";
	//Najpierw utwórz przykładowy wezęł
	tree node; 
	trie_init(&node,root,*(test));
	tree node2; 
	trie_init(&node2,root,*(test+2));
	
	tree node3; 
	trie_init(&node3,root,*(test+3));
	
	tree add; 
	trie_init(&add,root,*(test+1));
	
	//printf("\n \n TESTNUMER:%d, %lc\n",add->letter,add->letter);

	vector_add(root->children,node);
	assert_int_equal(root->children->size,1);
	vector_add(root->children,node2);
	assert_int_equal(root->children->size,2);
	vector_add(root->children,node3);
	assert_int_equal(root->children->size,3);
	vector_add(root->children,add);
	assert_int_equal(root->children->size,4);
	
	
	
	assert_true(root->children->items[0]->letter==*(test));
	assert_true(root->children->items[1]->letter==*(test+1));
	assert_true(root->children->items[2]->letter==*(test+2));
	assert_true(root->children->items[3]->letter==*(test+3));
	
	trie_clear(&root);
	//trie_clear(&node);
	//trie_clear(&node2);
	//trie_clear(&node3);
	//trie_clear(&add);
}


 /**@}*/
 
 /** @name Test vector_find
  @{
 */
 
///Szukaj w pustym pustym vektorze
static void vector_find_empty(void** state) {
	tree root; 
	trie_init(&root,NULL,'\0'); 
	assert_true(vector_find(root->children,L"a")==NULL);
	trie_clear(&root);
}


///Szukaj w vektorze z jednym elementem
static void vector_find_one(void** state) {
	tree root; 
	trie_init(&root,NULL,'\0'); 
	tree node; 
	trie_init(&node,NULL,L"ą");
	vector_init(root->children,node);
	assert_true(vector_find(root->children,L"ą")!=NULL);
	assert_true(vector_find(root->children,L"z")==NULL);
	trie_clear(&root);
	//trie_clear(&node);
}

///Szukaj w niepustym vektorze
static void vector_find_test(void** state) {
	
	 setlocale(LC_ALL, "pl_PL.UTF-8");
	   
	  tree root; 
	trie_init(&root,NULL,'\0'); 
	   
	   
	const wchar_t *test   = L"ąbćgzf";
	//Najpierw utwórz przykładowy wezęł
	tree node; 
	trie_init(&node,root,*(test));
	tree node2; 
	trie_init(&node2,root,*(test+2));
	
	tree node3; 
	trie_init(&node3,root,*(test+3));
	
	tree add; 
	trie_init(&add,root,*(test+1));
	
	
	
	vector_init(root->children,node);
	assert_int_equal(root->children->size,1);
	vector_add(root->children,node2);
	assert_int_equal(root->children->size,2);
	vector_add(root->children,node3);
	assert_int_equal(root->children->size,3);
	vector_add(root->children,add);
	assert_int_equal(root->children->size,4);
	
	assert_true(vector_find(root->children,*(test))!=NULL);
	assert_true(vector_find(root->children,*(test+4))==NULL);
	assert_true(vector_find(root->children,*(test+3))!=NULL);
	assert_true(vector_find(root->children,*(test+5))==NULL);
	
	trie_clear(&root);
	vector_done(root->children);
}

  /**@}*/

 /** @name Test vector_delete
  @{
 */
 
///Usuń wezęł którego jest w vektorze
 static void vector_delete_test(void** state) {
	setlocale(LC_ALL, "pl_PL.UTF-8");
	   
	  tree root; 
	trie_init(&root,NULL,'\0'); 
	   
	   
	const wchar_t *test   = L"ąbćgzf";
	//Najpierw utwórz przykładowy wezęł
	tree node; 
	trie_init(&node,root,*(test));
	tree node2; 
	trie_init(&node2,root,*(test+2));
	
	tree node3; 
	trie_init(&node3,root,*(test+3));
	
	tree add; 
	trie_init(&add,root,*(test+1));
	
	
	
	
	vector_init(root->children,node);
	assert_int_equal(root->children->size,1);
	vector_add(root->children,node2);
	assert_int_equal(root->children->size,2);
	vector_add(root->children,node3);
	assert_int_equal(root->children->size,3);
	vector_add(root->children,add);
	assert_int_equal(root->children->size,4);
	

	vector_delete(root->children,node2->letter);
	assert_true(vector_find(root->children,node2->letter)==NULL);
	
	trie_clear(&root);
	
	
}
 /**@}*/

int main(void) {

    const struct CMUnitTest tests[] = {
			cmocka_unit_test(vector_init_test),
			cmocka_unit_test(vector_resize_test),
			cmocka_unit_test(vector_resize_minus_test),
			cmocka_unit_test(vector_add_empty),
			cmocka_unit_test(vector_add_test),
			cmocka_unit_test(vector_add_test_pl),
			cmocka_unit_test(vector_find_empty),
			cmocka_unit_test(vector_find_one),
			cmocka_unit_test(vector_find_test),
			cmocka_unit_test(vector_delete_test),
    };

    return cmocka_run_group_tests(tests,NULL, NULL);
}

