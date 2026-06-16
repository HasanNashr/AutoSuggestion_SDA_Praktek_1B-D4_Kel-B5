#ifndef TRIE_H
#define TRIE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===== KONSTANTA =====
#define ALPHABET_SIZE 26
#define MAX_WORD_LEN  100
#define TOP_K         5
#define MAX_RESULTS   1000
#define DATASET_FILE  "kata_dataset.txt"
#define FREQ_FILE     "user_freq.txt"

// ===== STRUCT =====
typedef struct TNode {
    struct TNode *children[ALPHABET_SIZE];
    int EndOfTheWord;
    int frequency;
} TNode;

typedef TNode* address;
typedef address Root;

typedef struct {
    char word[MAX_WORD_LEN];
    int frequency;
} Suggestion;

// ===== PROTOTYPE FUNGSI =====
address createTNode();
void    insertTNode(Root *T, const char *word);
address searchTNode(Root T, const char *word);

int     compareSuggestion(const void *a, const void *b);
void    collectWords(address node, char *prefix, int depth, Suggestion *results, int *count);
void    autocomplete(Root T, const char *prefix);

void    loadDataset(Root *T, const char *filename);
void    loadFrequencies(Root T, const char *filename);
void    collectAllWords(address node, char *prefix, int depth, Suggestion *results, int *count);
void    saveFrequencies(Root T, const char *filename);

void    freeTrie(address node);

#endif