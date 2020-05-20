// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table: pow(2, 9) = 512
const unsigned int N = 512;

// count size of dictionary
int counter = 0;

// Hash table
node *table[N] = {NULL};

// Hashes word to a number
unsigned int hash(const char *word)
{
    // sum of ASCII characters
    int sum = 0;
    int i = 0;
    while (word[i] != '\0')
    {
        int ascii = (int) tolower(word[i]);
        sum += ascii;
        i++;
    }
    // choice of hash function is to take sum of ascii values mod number of buckets
    // index to hash table
    int index = sum % N;
    return index;
}

// Loads dictionary into memory, returning true if successful else false
// calls hash function to place words into hash table
bool load(const char *dictionary)
{
    // buffer to store words read from dictionary file
    char buffer[LENGTH + 1] = {'\0'};
    FILE* inptr = fopen(dictionary, "r");
    // index of hash table
    int index;
    // read through each line of input dictionary
    while (fgets(buffer, LENGTH + 1, inptr))
    {
        // count words
        counter++;
        // terminate string
        for (int j = 0; j < LENGTH + 1; j++)
        {
            if (isalpha(buffer[j]) == 0 && buffer[j] != 39)
                buffer[j] = '\0';
        }
        // run hash function on string to obtain index of hash table
        index = hash(buffer);
        // create entry in hash table
        node* n = malloc(sizeof(node));
        // always check if pointer is returned successfully by malloc
        if (n == NULL)
        {
            printf("Insufficient memory, failed to load dictionary. \n");
            return false;
        }
        // copy elements in buffer over to word array in node
        int i = 0;
        while (buffer[i] != '\0')
        {
            n -> word[i] = buffer[i];
            i++;
        }
        // terminate string
        n -> word[i] = '\0';
        // add new node to front of LL
        n -> next = table[index];
        table[index] = n;
        // node* tmp = table[index];
        // while(tmp != NULL)
        // {
        //     printf("%s \n", tmp -> word);
        //     tmp = tmp -> next;
        // }
    }
    fclose(inptr);
    return true;
}

// Returns number of words loaded from dictionary
unsigned int size(void)
{
    return counter;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // convert word to lowercase
    char buffer[LENGTH + 1];
    int i = 0;
    while (word[i] != '\0')
    {
        buffer[i] = tolower(word[i]);
        i++;
    }
    // terminate word
    for (int j = i; j < LENGTH + 1; j++)
    {
        buffer[j] = '\0';
    }
    // obtain index of hash table
    int index = hash(buffer);
    node* tmp = table[index];
    // compare word against dictionary
    while (tmp != NULL)
    {
        int diff = strcmp(tmp -> word, buffer);
        if (diff == 0)
            return true;
        else
            tmp = tmp -> next;
    }
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // free each bucket in hash table
    for (int i = 0; i < N; i++)
    {
        // free nodes in LL starting from the front
        node* tmp = table[i];
        while (tmp != NULL)
        {
            tmp = tmp -> next;
            free(table[i]);
            table[i] = tmp;
        }
    }
    return true;
}
