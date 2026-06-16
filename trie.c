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
    current->EndOfTheWord = 1;  
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

// ===== freeTrie =====

void freeTrie(address node){
    if (node  == NULL){
        return;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freeTrie(node->children[i]);
    }

    free(node);
}

// ===== loadDataset =====

void loadDataset(Root *T, const char *filename){
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf("File tidak ditemukan!");
        return;
    } 

    char line[MAX_WORD_LEN];
    while (fgets(line, MAX_WORD_LEN, file) != NULL){
        int len = strlen(line);
        if (len > 0 && line[len-1] == '\n'){
            line[len - 1] = '\0';
        }

        if (strlen(line) > 0){
            insertTNode(T, line);
        }
    }

    fclose (file);
    
}

// ===== collectAllWords =====

void collectAllWords(address node, char *prefix, int depth, Suggestion *results, int *count){
    if (node == NULL) return;
    if (*count >= MAX_RESULT) return;
    if (node->EndOfTheWord == 1 && node->frequency > 0){
        prefix[depth] = '\0';
        strcpy(result[*count].word, prefix);
        result[*count].frequency = node->frequency;
        (*count)++;
    }

    for (int i = 0; i < ALPHABET_SIZE, i++){
        if (node->children[i] != NULL){
            prefix[depth] = 'a' + i;
            collectAllWords(node->children[i], prefix, depth + 1, result, count);
        }
    }
}

// ===== saveFrequencies =====

void saveFrequencies(Root T, const char *filename) {
    Suggestion results[MAX_RESULTS];
    int count = 0;
    char buffer[MAX_WORD_LEN];

    collectAllWords(T, buffer, 0, results, &count);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: gagal menyimpan frekuensi!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %d\n", results[i].word, results[i].frequency);
    }

    fclose(file);
}

// ===== loadFrequencies =====

void loadFrequencies(Root T, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }

    char word[MAX_WORD_LEN];
    int freq;
    char line[MAX_WORD_LEN];

    while (fgets(line, MAX_WORD_LEN, file) != NULL) {
        if (sscanf(line, "%s %d", word, &freq) == 2) {
            address node = searchTNode(T, word);
            if (node != NULL) {
                node->frequency = freq;
            }
        }
    }

    fclose(file);
}

