/** @file
  Testy sprwdzające działanie strukture alphabet z \ref alphabet.h.
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
#include "alphabet.h"
#include <locale.h>
 
 
 
  /**@}*/
/** @name Test alphabet_init 
  @{
 */
 
///Test inicjalizacji alphabet
static void alphabet_init_test(void** state) {

	wchar_t letter=L'ą'; 
	alphabet test;
	
	alphabet_init(&test,letter);
	assert_int_equal(test.size,1);
	assert_true(test.items[0]==letter);
	//free(&test.items[0]);
}
/**@}*/

  /**@}*/
/** @name Test alphabet_resize 
  @{
 */
///Test zwiększania pojemności alphabet
static void vector_resize_test(void** state) {
	
	wchar_t letter=L'ą'; 
	alphabet test;
	
	alphabet_init(&test,letter);
	assert_int_equal(test.size,1);
	assert_true(test.items[0]==letter);
	
	alphabet_resize(&test);
	assert_int_equal(test.size,2);

}

 /**@}*/


 /** @name Test alphabet_add
  @{
 */
 ///Test dodawania nowej litery do pustego alfabetu
 
static void alphabet_add_empty(void** state) {
	
	wchar_t letter=L'ą'; 
	alphabet test;
	test.size=0;
	alphabet_add(&test,letter);
	assert_int_equal(test.size,1);
	assert_true(test.items[0]==letter);
}


///Test dodawania nowego węzła do niepustego alphabetu
static void alphabet_add_test(void** state) {
	wchar_t letter=L'ą'; 
	alphabet test;
	
	test.size=0;
	alphabet_add(&test,letter);
	assert_int_equal(test.size,1);
	assert_true(test.items[0]==letter);
	wchar_t add1=L'a'; 
	wchar_t add2=L'f'; 
	
	alphabet_add(&test,add1);
	alphabet_add(&test,add2);
	assert_int_equal(test.size,3);
	
	
	assert_true(test.items[1]==letter);
	assert_true(test.items[0]==add1);
	assert_true(test.items[2]==add2);
}


///Test dodawania nowych liter - kilka losowych + proba dodania tej samej litery drugi raz
static void alphabet_add_test2(void** state) {
	
	wchar_t letter=L'ą'; 
	alphabet test;
	
	test.size=0;
	alphabet_add(&test,letter);
	assert_int_equal(test.size,1);
	assert_true(test.items[0]==letter);
	wchar_t add1=L'a'; 
	wchar_t add2=L'f'; 
	
	alphabet_add(&test,add1);
	alphabet_add(&test,add2);
	assert_int_equal(test.size,3);
	alphabet_add(&test,add2);
	assert_int_equal(test.size,3);
	wchar_t add3=L'z'; 
	wchar_t add4=L'ę'; 
	alphabet_add(&test,add3);
	alphabet_add(&test,add4);
	assert_int_equal(test.size,5);
	
	assert_true(test.items[4]==add3);
	assert_true(test.items[2]==add4);
	
}


 /**@}*/
 
 /** @name Test alphabet_find
  @{
 */

///Szukaj w pustym alfabecie
static void alphabet_find_empty(void** state) {
	wchar_t letter=L'ą'; 
	alphabet test;
	
	test.size=0;
	assert_int_equal(alphabet_find(&test,letter),-1);
}


///Szukaj w alfabecie z jednym elementem
static void alphabet_find_one(void** state) {
	wchar_t letter=L'ą'; 
	alphabet test;
	test.size=0;
	alphabet_add(&test,letter);
	assert_int_equal(alphabet_find(&test,letter),1);
	
	
}

///Szukaj w niepustym alfabecie
static void alphabet_find_test(void** state) {
	
	 wchar_t letter=L'ą'; 
	alphabet test;

	alphabet_init(&test,letter);
	wchar_t add1=L'a'; 
	wchar_t add2=L'ź'; 
	wchar_t add3=L'h'; 
	wchar_t add4=L'ó'; 
	
	alphabet_add(&test,add1);
	alphabet_add(&test,add2);
	alphabet_add(&test,add3);
	alphabet_add(&test,add4);
	
	wchar_t find1=L'ł'; 
	wchar_t find2=L'z'; 
	
	assert_int_equal(alphabet_find(&test,letter),1);
	assert_int_equal(alphabet_find(&test,add3),1);	
	assert_int_equal(alphabet_find(&test,find1),-1);
	assert_int_equal(alphabet_find(&test,find2),-1);	
}

  /**@}*/
  
 /** @name Test alphabet_done
  @{
 */
 static void alphabet_done_test(void** state) {
 wchar_t letter=L'ą'; 
	alphabet test;

	alphabet_init(&test,letter);
	alphabet_done(&test);
 }
   /**@}*/

int main(void) {

	setlocale(LC_ALL, "pl_PL.UTF-8");
	
    const struct CMUnitTest tests[] = {
			cmocka_unit_test(alphabet_init_test),
			cmocka_unit_test(alphabet_add_empty),
			cmocka_unit_test(alphabet_add_test),
			cmocka_unit_test(alphabet_add_test2),
			cmocka_unit_test(alphabet_find_empty),
			cmocka_unit_test(alphabet_find_one),
			cmocka_unit_test(alphabet_find_test),
			cmocka_unit_test(alphabet_done_test),
    };

    return cmocka_run_group_tests(tests,NULL, NULL);
}

