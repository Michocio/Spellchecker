/** @defgroup dict-editor Moduł dict-editor
    Prosty klient biblioteki dictionary.
    Umożliwia edycję słownika.
  */
/** @file
    Główny plik modułu dict-editor
    @ingroup dict-editor
    @author Jakub Pawlewicz <pan@mimuw.edu.pl>
    @date 2015-05-11
    @copyright Uniwersytet Warszawski
    @todo Poprawić proste parsowanie na porządniejsze.
  */
  #include "states.h"
#include "rules.h"
#include "dictionary.h"
#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wctype.h>

/** Makro służące do otrzymania stałej w formie napisu */
#define str(x)          # x
/** Makro służące do otrzymania stałej w formie napisu */
#define xstr(x)         str(x)


/** Dostępne polecenia. 
    Odpowiadające komendy w tablicy @ref commands
 */
enum Command {
    INSERT,
    DELETE,
    FIND,
    HINTS,
    SAVE,
    LOAD,
    QUIT,
    CLEAR,
    COMMANDS_COUNT };


/** Komendy wywołujące pocelnia. 
    Kolejność zgodna z tą w enum @ref Command.
  */
static const char *commands[] =
{
    "insert",
    "delete",
    "find",
    "hints",
    "save",
    "load",
    "quit",
    "clear"
};

/** Maksymalna długość komendy.
    Maksymalna długość komendy bez kończącego znaku '\0'
  */
#define MAX_COMMAND_LENGTH 15
/** Maksymalna długość słowa.
    Maksymalna długość słowa bez kończącego znaku '\0'
  */
#define MAX_WORD_LENGTH 63
/** Maksymalna długość nazwy pliku.
    Maksymalna długość nazwy pliku bez kończącego znaku '\0'
  */
#define MAX_FILE_LENGTH 511


/** Wczytuje wejście do napotkania znaku nowej linii.
  */
void skip_line()
{
    if (scanf("%*[^\n]\n") < 0) {
        if (ferror(stdin))
        {
            fprintf(stderr, "Failed to read line\n");
            exit(1);
        }
    }
}


/** Ignoruje wiersz 
 */
int ignored()
{
    printf("ignored\n");
    skip_line();
    return 1;
}


/** Zamienia słowo na złożone z małych liter.
  @param[in,out] word Modyfikowane słowo.
  @return 0, jeśli słowo nie jest złożone z samych liter, 1 w p.p.
 */
int make_lowercase(wchar_t *word)
{
    for (wchar_t *w = word; *w; ++w)
        if (!iswalpha(*w))
            return 0;
        else
            *w = towlower(*w);
    return 1;
}


/** Przetwarza komendę operującą na słowniku.
  @param[in,out] dict Słownik, na którym wykonywane są operacje.
  @param[in] c Komenda.
  @return 0, jeśli należy zakończyć proram, 1 w p.p.
 */
static int dict_command(struct dictionary **dict, enum Command c) 
{
    wchar_t word[MAX_WORD_LENGTH+1];
    if (scanf("%" xstr(MAX_WORD_LENGTH) "ls", word) <= 0)
    {
        fprintf(stderr, "Failed to read word\n");
        exit(1);
    }
    if (!make_lowercase(word))
    {
        fprintf(stderr, "Invalid word '%ls'\n", word);
        return ignored();
    }
    switch (c)
    {
        case INSERT:
            if (dictionary_insert(*dict, word))
                printf("inserted: %ls\n", word);
            else
                return ignored();
            break;
        case DELETE:
            if (dictionary_delete(*dict, word))
                printf("deleted: %ls\n", word);
            else
                return ignored();
            break;
        case FIND:
            if (dictionary_find(*dict, word))
                printf("found: %ls\n", word);
            else
                printf("not found: %ls\n", word);
            break;
        case HINTS:
            {
                struct word_list list;
                dictionary_hints(*dict, word, &list);
                const wchar_t * const *a = word_list_get(&list);
                for (size_t i = 0; i < word_list_size(&list); ++i)
                {
                    if (i)
                        printf(" ");
                    printf("%ls", a[i]);
                }
                printf("\n");
                word_list_done(&list);
                break;
            }
        default:
            assert(false);
    }
    skip_line();
    return 1;
}


/** Przetwarza komendę operującą na plikach.
  @param[in,out] dict Słownik, na którym wykonywane są operacje.
  @param[in] c Komenda.
  @return 0, jeśli należy zakończyć proram, 1 w p.p.
 */
static int file_command(struct dictionary **dict, enum Command c) 
{
    char filename[MAX_FILE_LENGTH+1];
    if (scanf("%" xstr(MAX_FILE_LENGTH) "s", filename) <= 0)
    {
        fprintf(stderr, "Failed to read filename\n");
        exit(1);
    }
    switch (c)
    {
        case SAVE:
            {
                FILE *f = fopen(filename, "w");
                if (!f || dictionary_save(*dict, f))
                {
                    fprintf(stderr, "Failed to save dictionary\n");
                    exit(1);
                }
                fclose(f);
                printf("dictionary saved in file %s\n", filename);
                break;
            }
        case LOAD:
            {
                FILE *f = fopen(filename, "r");
                struct dictionary *new_dict;
                if (!f || !(new_dict = dictionary_load(f)))
                {
                    fprintf(stderr, "Failed to load dictionary\n");
                    exit(1);
                }
                fclose(f);
                printf("dictionary loaded from file %s\n", filename);
                dictionary_done(*dict);
                *dict = new_dict;
                break;
            }
        default:
            assert(false);
    }
    skip_line();
    return 1;
}



/** Przetwarza jedną komendę.
  @param[in,out] dict Słownik, na którym wykonywane są operacje
  @return 0, jeśli należy zakończyć proram, 1 w p.p.
 */
int try_process_command(struct dictionary **dict)
{
    char cmd[MAX_COMMAND_LENGTH+1];
    if (scanf("%" xstr(MAX_COMMAND_LENGTH) "s", cmd) <= 0)
    {
        if (ferror(stdin))
        {
            fprintf(stderr, "Failed to read command\n");
            exit(1);
        }
        return 0;
    }
    int c;
    for (c = 0; c < COMMANDS_COUNT; ++c)
        if (!strcmp(cmd, commands[c]))
            break;
    if (c == COMMANDS_COUNT)
    {
        fprintf(stderr, "Invalid command '%s'\n", cmd);
        return ignored();
    }
    else if (c == QUIT)
        return 0;
    else if (c == CLEAR)
    {
        dictionary_done(*dict);
        *dict = dictionary_new();
        printf("cleared\n");
        skip_line();
        return 1;
    }
    else if (c < SAVE)
    {
        return dict_command(dict, c);
    }
    else
    {
        return file_command(dict, c);
    }
}

/**
  Funkcja main.
  Główna funkcja programu do testowania słownika. 
 */
int main(void)
{	

	 setlocale(LC_ALL, "pl_PL.UTF-8");

   struct dictionary *dict=dictionary_new();


  do {} while (try_process_command(&dict));
	dictionary_done(dict);
    return 0;
    
}
