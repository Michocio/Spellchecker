/** @file
 Definicje atrap stosowanych do testowania IO 

  @ingroup dict-tests
  @author Michał Jaroń
  @copyright Uniwerstet Warszawski 
  @date 2015-06-04
 */

#include<stdio.h>
#include<assert.h>
#include<stdbool.h>
#include<wchar.h>

#ifndef UTILS_H
#define UTILS_H

#ifdef UNIT_TESTING

///atrapa fprintf
#define fprintf fprintf_test

///atrapa fgetwc
#define fgetwc fgetwc_test

///dekleracja fprintf_test
extern int fprintf_test(FILE* const file, const char *format, ...);

///dekleracja fgetwc_test
extern wint_t fgetwc_test(FILE* stream);

#endif 
#endif 

