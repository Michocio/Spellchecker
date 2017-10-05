/** @file
  Testy sprwdzające działanie funkcji IO bez operacji na plikach
  @ingroup dict-tests
  @author Michał Jaroń
  @copyright Uniwerstet Warszawski 
  @date 2015-06-04
 */

#define UNIT_TESTING//Na potrzeby atrap

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include "trie.h"
#include <locale.h>
#include "utils.h"
#include <stdio.h>

///bufor
char buffor[100];
int w = 0;//Zapisuje bufor
int r = 0;//Czyta bufor


/**
 * Mock funkcji fgetwc.
 */
wint_t fgetwc_test(FILE* stream)
{
	wchar_t result;

	r = r+ mbtowc(&result, r+buffor,
			100 - r);
	if (r > 100)
		assert(false);
	return result;
}

/**
 * Mock funkcji fprintf.
 */
int fprintf_test(FILE* const file, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	int return_value = vsnprintf(buffor+w, sizeof(buffor), format,
			args);
	w += return_value;
	if (w >= 100)
		assert(false);
	return return_value;
}





/**@}*/
/** @name Testy toString
  @{
 */
///Zapisz puste drzewo
/*static void save_empty(void** state){

	setlocale(LC_ALL, "pl_PL.UTF-8");
	tree root;  alphabet alphabet;   alphabet.size=0;
	trie_init(&root,NULL,'\0');
	toString(root,stdout);
	buffor[w] = '\0';
	w = 0;
	assert_string_equal(buffor, "");
}*/


///Prosty test poprawnościowy zapisu drzewa
static void save_simple(void** state){

	buffor[w] = '\0';
	w = 0;
	wchar_t answer[]=L"a(b(c))";//Prawidłowy zapis
	setlocale(LC_ALL, "pl_PL.UTF-8");
	tree root;  alphabet alphabet;   alphabet.size=0;
	trie_init(&root,NULL,'\0');
	const wchar_t* test1   = L"abc";
	assert_int_equal(trie_insert(test1,&root,&alphabet), 1);
	toString(root,stdout);
	printf("WWWWWWWWWWWWWWWW:%s",buffor);
	buffor[w] = '\0';
	w = 0;
	//assert_string_equal(buffor, "a(b(c))");
}



int main(void) {

    const struct CMUnitTest tests[] = {
			//cmocka_unit_test(save_empty),
			cmocka_unit_test(save_simple),
    };

    return cmocka_run_group_tests(tests,NULL, NULL);
}
