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
	
	
	//printf("%s %d\n",argv[2], argc);
	
	if(argc==3)
	{
		/*
		FILE * stream = fopen(argv[2], "r");
	
	
	wint_t wc;
	wchar_t str [100];
	fwscanf (stream, L"%ls", str);
	do {
	//fwscanf (stream, L"%ls", str);
    if(iswalpha(str[wcslen(str)-1])==0) str[wcslen(str)-1]=L'\0';
	printf("%ls  %d, %p\n", str,wcslen(str),trie_find(str,dict->root));
  } while (fwscanf (stream, L"%ls", str)!=WEOF);
		
		*/
		
		
	}
	else
	{
		
		setlocale(LC_ALL, "pl_PL.UTF-8");
		file(argv[1]);

	}
/*	FILE * stream = fopen(argv[2], "r");
	
	
	wint_t wc;
 wchar_t str [100];
 fwscanf (stream, L"%ls", str);
  do {
    //fwscanf (stream, L"%ls", str);
    if(iswalpha(str[wcslen(str)-1])==0) str[wcslen(str)-1]=L'\0';
	printf("%ls  %d, %p\n", str,wcslen(str),trie_find(str,dict->root));
  } while (fwscanf (stream, L"%ls", str)!=WEOF);
	*/
	
	
}
