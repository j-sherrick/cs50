// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Hash table
node *table[HASH_MAX];

// Set to true once dict is loaded
bool loaded = false;

// Number of words loaded
int word_count;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int h = hash(word);
    // If there is no entry at this location, word is not in the dictionary
    if (!table[h])
    {
        return false;
    }

    node* p = table[h];
    while (p)
    {
        if (strcicmp(p->word, word))
        {
            return true;
        }
        p = p->next;
    }

    // If no match was found at this point, the word is not present
    return false;
}

// "Cubic" hash: Take the sum of the cube of (index + 1) of each letter times numerical value of the letter.
unsigned int hash(const char *word)
{
    unsigned int hashcode = 0, f;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        f = i + 1;
        hashcode += (tolower(word[i]) - 'a' + 1) * (f * f * f * f);
    }
    return hashcode % HASH_MAX;
}

bool insert(const unsigned int hash, const char *word)
{
    if(!table[hash])
    {
        table[hash] = malloc(sizeof(node));
        if (!table[hash])
        {
            return false;
        }
        table[hash]->next = NULL;
        strcpy(table[hash]->word, word);
        return true;
    }
    else
    {
        node *p = table[hash];
        while (p->next)
        {
            p = p->next;
        }

        p->next = malloc(sizeof(node));
        if (!p->next)
        {
            return false;
        }
        p = p->next;
        p->next = NULL;
        strcpy(p->word, word);

        return true;
    }
    
    return false;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *dict = fopen(dictionary, "r");
    if (!dict)
    {
        return false;
    }

    char word[LENGTH + 1];
    // Keep looping while there's still another word in the dictionary
    while (get_next_word(word, dict))
    {
        if(!insert(hash(word), word))
        {
            // Return false if inserting the word failed for some reason
            return false;
        }
        // Increment word_count after a word has been successfully loaded
        ++word_count;
    }
    fclose(dict);

    // Dictionary has been loaded. Set loaded to true.
    loaded = true;
    return loaded;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return loaded ? word_count : 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < HASH_MAX; ++i)
    {
        if(table[i])
        {
            freelist(table[i]);
            table[i] = NULL;
        }
    }
    return true;
}

// Recursive function to free all nodes in a linked list
void freelist(node *n)
{
    if (n->next)
    {
        freelist(n->next);
    }
    free(n);
}

// Get the next word from a text file, return number of chars read
int get_next_word(char *buff, FILE *f)
{
    int count = 0;
    char c;
    while(fread(&c, sizeof(char), 1, f) && c != '\n')
    {
        buff[count++] = c;
    }
    buff[count] = '\0';

    return count;
}

// Case insensitive string compare
bool strcicmp(const char *dword, const char *txtword)
{
    int dlen = strlen(dword), txtlen = strlen(txtword);
    // If the strings are not equal length, they're obviously not equal
    if(dlen != txtlen)
    {
        return false;
    }
    for (int i = 0; i < dlen; ++i)
    {
        if (dword[i] != tolower(txtword[i]))
        {
            return false;
        }
    }

    // If the strings are equal length and no mismatch was found, strings are a match
    return true;
}