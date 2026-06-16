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

// ===== collectWords =====
void collectWords(address node, char *prefix, int depth, Suggestion *results, int *count) {
    if (node == NULL) return;
    if (*count >= MAX_RESULTS) return;

    if (node->EndOfTheWord == 1) {
        prefix[depth] = '\0';
        strcpy(results[*count].word, prefix);
        results[*count].frequency = node->frequency;
        (*count)++;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != NULL) {
            prefix[depth] = 'a' + i;
            collectWords(node->children[i], prefix, depth + 1, results, count);
        }
    }
}

// ===== autocomplete =====
void autocomplete(Root T, const char *prefix) {
    address current = T;

    // Navigate ke node akhir prefix
    for (int i = 0; prefix[i] != '\0'; i++) {
        int index = prefix[i] - 'a';
        if (current->children[index] == NULL) {
            printf("Tidak ada suggestion untuk prefix \"%s\"\n", prefix);
            return;
        }
        current = current->children[index];
    }

    // Kumpulkan semua kata dari node ini ke bawah
    Suggestion results[MAX_RESULTS];
    int count = 0;
    char buffer[MAX_WORD_LEN];
    strcpy(buffer, prefix);

    collectWords(current, buffer, strlen(prefix), results, &count);

    if (count == 0) {
        printf("Tidak ada suggestion untuk prefix \"%s\"\n", prefix);
        return;
    }

    // Sort: frekuensi desc, lalu abjad asc
    qsort(results, count, sizeof(Suggestion), compareSuggestion);

    // Tampilkan top 5
    int limit = (count < TOP_K) ? count : TOP_K;
    printf("\nSuggestion untuk \"%s\":\n", prefix);
    for (int i = 0; i < limit; i++) {
        printf("  [%d] %s (freq: %d)\n", i + 1, results[i].word, results[i].frequency);
    }
}