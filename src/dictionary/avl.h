/** @file
  Interfejs obsługujący drzewo binarne.
  Przechowuje stany.

  @ingroup structers
  @author Michał Jaroń
  @copyright Uniwerstet Warszawski
  @date 2015-06-29
 */
#ifndef __AVL_H__
#define __AVL_H__

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include "states.h"

/**
  Struktura reprezentująca węzeł drzewa AVL
  */
struct bin
{
    state* key;///<Wskaźnik na stan
    struct bin *left;///<Dziecko
    struct bin *right;///<Dziecko
    //int height;///<Wysokość wezła
};


/**
  Dodaje wezeł wskazujący na stan do drzewa.
  @param[in] struct bin* Edytoane drzewo.
  @param[in] state* Wstawiany stan.
  @return struct bin* Zeedytowane drzewo
  */
struct bin* insert(struct bin* leaf, state* key);


/**
  Szuka danego stanu w drzewie
  @param[in] state* Szukany klucz.
  @param[in] struct bin * Drzewo, w którym szukamy.
  @return 1 jeśli węzeł w drzewie, -1 w p.p
  */
int search(state* key, struct bin *leaf);



/**
  Tworzy wezeł z danym stanem.
  @param[in] state* Dany stan.
  @return struct bin* Nowy wezeł
  */
struct bin* newNode(state* key);



/**
  Rotacja drzewa, przywraca równowagę w drzewie.
  @param[in] struct bin * Wezeł do balnsu
  @return struct bin * Zbalansowane drzewo
  */
struct bin *leftRotate(struct bin *x);



/**
  Rotacja drzewa, przywraca równowagę w drzewie.
  @param[in] struct bin * Wezeł do balansu
  @return struct bin * Zbalansowane drzewo
  */
struct bin *rightRotate(struct bin *y);



/**
  Wysokość danego węzła
  @param[in] struct bin * Dany wezeł
  @return wysokość węzła
  */
int height(struct bin *N);


/**
  Jaka różnica wysokości dla danego węzła
   @param[in] struct bin * Wezeł do balansu
  @return int Różnica wysokości
  */
int getBalance(struct bin *N);

#endif
