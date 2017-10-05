/** @file
  Implementacja struktury drzewa binarnego.
Używane do przechowywanie stanów.
* Pozwala szybko dodawać i wyszukiwać stany.
  @ingroup structers
  @author Michał Jaroń
  @copyright Uniwerstet Warszawski
  @date 2015-06-29
 */

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include "avl.h"
#include "dictionary.h"


struct bin* newNode(state* key)
{
	
    struct bin* new = (struct bin*)
                        malloc(sizeof(struct bin));
    new->key   = key;
    new->left   = NULL;
    new->right  = NULL;
    return new;
}

struct bin* insert(struct bin* leaf, state * key)
{
    if (leaf== NULL)
        return(newNode(key));
      //Wlasna relacja porządku
	int comp=comperator(key,leaf->key,key->cost,leaf->key->cost);
	
    if (comp==-1)//(key < leaf->key)
        leaf->left  = insert(leaf->left, key);
    else
        leaf->right = insert(leaf->right, key);
 
    
    return leaf;
}

int search(state* key, struct bin *leaf)
{
	//Wlasny komperator
	//int x=comperator(key,leaf->key,1,1);

  if( leaf != 0 )
  {
	  int x=comperator(key,leaf->key,key->cost,leaf->key->cost);
      if(x==0)//Równe
      {
		  if(key->first_word!=leaf->key->first_word) return -1;
          return 1;
      }
      else if(x==-1)
      {
          return search(key, leaf->left);
      }
      else
      {
          return search(key, leaf->right);
      }
  }
  else return -1;
}


void destroy_tree(struct bin **leaf)
{
  if( leaf != 0 )
  {
      destroy_tree((*leaf)->left);
      destroy_tree((*leaf)->right);
      free( (*leaf) );
  }
}
