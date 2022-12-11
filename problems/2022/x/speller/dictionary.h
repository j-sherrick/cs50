// Declares a dictionary's functionality

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// Maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

#define HASH_MAX 100000

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Business logic
bool            check(const char *word);
unsigned int    hash(const char *word);
bool            load(const char *dictionary);
unsigned int    size(void);
bool            unload(void);

// Helper functions
int             get_next_word(char* buff, FILE *f);
bool            insert(const unsigned int hash, const char* word);
bool            strcicmp(const char *dword, const char *txtword);
void            freelist(node *n);

#endif // DICTIONARY_H