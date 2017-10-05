/** @defgroup dict-check Moduł dict-check
    Sprawdza pisownię w danym tekście
  */
/** @file
    Główny plik modułu dict-check
    @ingroup dict-check
    @author Michał Jaroń 
    @date 2015-05-31
    @copyright Uniwersytet Warszawski
  */


#include "word_list.h"
#include "trie.h"
#include "dictionary.h"
#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wctype.h>

int main (int argc, char ** argv)
{
	
	setlocale(LC_ALL, "pl_PL.UTF-8");
	
	if(argc==3)//Trzy parametry wywołania
	{
		if((argv[1][0]=='-')&&((argv[1][1]=='v')))//Wywołanie z parametrem -v
		{
			/// \ref struct dictionary * file( int v,const char * plik)
			file(1,argv[2]);
			return 0;
		}
		else return -1;
	}
	else if  (argc==2)//Dwa parametry. W argv[1] nazwa słownika wejściowego
	{
		file(0,argv[1]);
		return 0;
	}
	else//Nieprawidłowe wywołanie
		return -1;
}
