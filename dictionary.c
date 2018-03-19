// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

/*****************************************
Lots of ugly global declarations because I'm not
allowed to change paramaters of functions. Avoiding pointers because when I
wrote this code, we hadn't learned about them yet.
******************************************/
// declare global pointer to dictionary hash table
struct node **hashtable_pointer;

// declare global pointer to unhashed dictionary;
char *dictionary_pointer;

// declare and initialize global dictionary number of words
unsigned int dictionary_words = 0;

// declare global size of dictionary in bytes
int dictionary_bytes;

/******************************************
Functions
*******************************************/
// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // deterime hashtable location
    int hashtable_location = djb_hash((unsigned char *) word) % dictionary_words;

    // check hashtable location for word
    return check_hashtable(word, hashtable_pointer[hashtable_location]);
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // determine file size. from: https://www.linuxquestions.org/questions/programming-9/how-to-get-size-of-file-in-c-183360/
    struct stat st;
    if (stat(dictionary, &st) < 0) // stat() returns negative value on error.
    {
        return false;
    }
    dictionary_bytes = st.st_size;

    // open dictionary file
    FILE *inptr = fopen(dictionary, "r");
    if (!inptr)
    {
        return false;
    }

    // allocate memory for dictionary
    dictionary_pointer = malloc(dictionary_bytes);
    if (!dictionary_pointer)
    {
        fclose(inptr);
        return false;
    }

    // read dictionary into memory and error check
    if (fread(dictionary_pointer, dictionary_bytes, 1, inptr) != 1)
    {
        return false;
    }

    // close file
    fclose(inptr);

    // determine number of words in dictionary
    if (size() == 0)
    {
        return false;
    }

    // allocate memory for hash table
    hashtable_pointer = calloc(dictionary_words, sizeof(node *));
    if (!hashtable_pointer)
    {
        free(dictionary_pointer);
        return false;
    }

    // declare word length
    int length;

    // declare pointer to entry in dictionary
    char *entry_pointer = dictionary_pointer;

    // populate hash table
    for (int i = 0; i < dictionary_words; i++)
    {
        // determine hash table location and length of word
        int hashtable_location = djb_hash_and_length((unsigned char *) entry_pointer,
                                 &length) % dictionary_words;

        // map word to hashtable
        if (!link_list(&hashtable_pointer[hashtable_location], entry_pointer))
        {
            return false;
        }

        // move to next word in dictionary
        entry_pointer += length + 1;
    }

    return true;
}

// Returns number of words in dictionary
// Changes '\n\' in dictionary to '\0'
unsigned int size(void)
{
    // check whether already determined size
    if (dictionary_words != 0)
    {
        return dictionary_words;
    }

    // determine size
    else
    {
        // go through dictionary one character at a time
        for (int i = 0; i < dictionary_bytes; i++)
        {
            // if found \n, finished a word
            if (dictionary_pointer[i] == '\n')
            {
                // change '\n' to '\0' for later usage as a string
                dictionary_pointer[i] = '\0';

                // increment words
                dictionary_words++;
            }
        }

        // return calculated number of words
        return dictionary_words;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // unload linked lists
    for (int i = 0; i < dictionary_words; i++)
    {
        free_link(hashtable_pointer[i]);
    }

    // unload unhashed dictionary
    free(dictionary_pointer);

    // unload hash table
    free(hashtable_pointer);
    return true;
}

// Hashes string and gives length of word. Adapted from: http://www.cse.yorku.ca/~oz/hash.html
// This function and following separated in an attempt to speed up the code
// (class had a competition).
unsigned long djb_hash_and_length(unsigned char *str, int *length)
{
    // not exactly sure how this hashing function works
    unsigned long hash = 5381;
    int c;

    // declare variable for determining word length
    int word_length = 0;

    // increment over each letter
    while ((c = *str++))
    {
        // do hashy stuff
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

        // increment word length
        word_length++;
    }

    // make word length available
    *length = word_length;

    // return hash
    return hash;
}

// Hashes string (in lowercase). Adapted from: http://www.cse.yorku.ca/~oz/hash.html
unsigned long djb_hash(unsigned char *str)
{
    // not exactly sure how this hashing function works
    unsigned long hash = 5381;
    int c;

    // do hashy stuff for each letter
    while ((c = tolower(*str++)))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    // return hash
    return hash;
}

// Adds word provided to head of linked list; returns true if successful, otherwise false
bool link_list(node **link, char *entry_pointer)
{
    // store old head location
    node *old_head = *link;

    // create new head
    *link = calloc(sizeof(node), 1);
    if (!link)
    {
        return false;
    }

    // update new head's word
    (*link)->word = entry_pointer;

    // update new head's next pointer
    (*link)->next = old_head;

    // success
    return true;
}

// Returns true if word is in dictionary else false
bool check_hashtable(const char *word, node *link)
{
    // if word here, check
    if (link != NULL)
    {
        // if word is same, return true
        if (strcasecmp(word, link->word) == 0)
        {
            return true;
        }
        // else, check next link
        else
        {
            return check_hashtable(word, link->next);
        }
    }

    // if not, word is not in dictionary
    else
    {
        return false;
    }
}

bool free_link(node *link)
{
    // if there are more links, continue down list, then free current node
    if (link != NULL)
    {
        // free later nodes
        free_link(link->next);

        // free this node
        free(link);
    }

    // otherwise, we're at the bottom, and that node is free already
    return true;
}
