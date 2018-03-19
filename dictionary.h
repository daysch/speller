// Declares a dictionary's functionality

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>
#include <stdint.h>

// Maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

// define node structure
typedef struct node
{
    const char *word;
    struct node *next;
}
node;

// Prototypes
bool check(const char *word);
bool load(const char *dictionary);
unsigned int size(void);
bool unload(void);
bool link_list(node **link, char *entry);
unsigned long djb_hash_and_length(unsigned char *str, int *);
unsigned long djb_hash(unsigned char *str);
bool check_hashtable(const char *word, node *link);
bool free_link(node *link);

#endif // DICTIONARY_H
