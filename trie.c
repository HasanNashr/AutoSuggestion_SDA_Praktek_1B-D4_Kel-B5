#include "trie.h"

// ===== createTNode =====
address createTNode() {
    address node = (address)malloc(sizeof(TNode));
    if (node == NULL) {
        printf("Alokasi memori gagal!");
        exit(1);
    }
    node->EndOfTheWord = 0;
    node->frequency = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }
    return node;
}

// ===== insertTNode =====
void insertTNode(Root *T, const char *word) {
    address current = *T;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (current->children[index] == NULL) {
            current->children[index] = createTNode();
        }
        current = current->children[index];
    }
    current->EndOfTheWord = 1;  // fix typo: curret -> current
}

// ===== searchTNode =====
address searchTNode(Root T, const char *word) {
    address current = T;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (current->children[index] == NULL) {
            return NULL;
        }
        current = current->children[index];
    }
    if (current->EndOfTheWord == 1) {
        return current;
    }
    return NULL;
}

// ===== compareSuggestion =====
int compareSuggestion(const void *a, const void *b) {
    Suggestion *sa = (Suggestion *)a;
    Suggestion *sb = (Suggestion *)b;
    if (sb->frequency != sa->frequency) {
        return sb->frequency - sa->frequency;  // frekuensi tinggi dulu
    }
    return strcmp(sa->word, sb->word);          // jika sama, urut abjad
}

