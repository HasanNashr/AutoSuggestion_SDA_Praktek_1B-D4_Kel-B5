#include "trie.h"

int main() {
    Root T = createTNode();
    loadDataset(&T, DATASET_FILE);
    loadFrequencies(T, FREQ_FILE);

    char input[MAX_WORD_LEN];
    int pilihan;

    printf("=== Auto Suggestion System ===\n");

    int cariLagi = 1;
    while (cariLagi) {
        printf("\nKetik awalan kata: ");
        if (scanf("%99s", input) != 1) break;

        int tambahHuruf = 1;
        while (tambahHuruf) {
            tambahHuruf = 0;

            address current = T;
            int prefixValid = 1;
            for (int i = 0; input[i] != '\0'; i++) {
                int idx = input[i] - 'a';
                if (idx < 0 || idx >= ALPHABET_SIZE || current->children[idx] == NULL) {
                    prefixValid = 0;
                    break;
                }
                current = current->children[idx];
            }

            if (!prefixValid) {
                printf("Kata tidak ditemukan dalam daftar kata.\n");
                break;
            }

            Suggestion results[MAX_RESULTS];
            int count = 0;
            char buffer[MAX_WORD_LEN];
            strcpy(buffer, input);
            collectWords(current, buffer, strlen(input), results, &count);

            if (count == 0) {
                printf("Kata tidak ditemukan dalam daftar kata.\n");
                break;
            }

            qsort(results, count, sizeof(Suggestion), compareSuggestion);

            int limit = (count < 10) ? count : 10;
            printf("\nSuggestion untuk \"%s\":\n", input);
            for (int i = 0; i < limit; i++) {
                printf("  [%d] %s (freq: %d)\n", i + 1, results[i].word, results[i].frequency);
            }
            printf("  [0] Tambah huruf\n");

            int pilihanValid = 0;
            while (!pilihanValid) {
                printf("Pilihan: ");
                if (scanf("%d", &pilihan) != 1) {
                    while (getchar() != '\n');
                    printf("Pilihan tidak valid, silakan masukkan angka sesuai daftar.\n");
                    printf("\nSuggestion untuk \"%s\":\n", input);
                    for (int i = 0; i < limit; i++) {
                        printf("  [%d] %s (freq: %d)\n", i + 1, results[i].word, results[i].frequency);
                    }
                    printf("  [0] Tambah huruf\n");
                    continue;
                }

                if (pilihan == 0) {
                    char tambahan[MAX_WORD_LEN];
                    printf("Tambah huruf (prefix saat ini \"%s\"): ", input);
                    if (scanf("%99s", tambahan) == 1) {
                        strncat(input, tambahan, MAX_WORD_LEN - strlen(input) - 1);
                    }
                    pilihanValid = 1;
                    tambahHuruf = 1;
                } else if (pilihan >= 1 && pilihan <= limit) {
                    pilihanValid = 1;
                } else {
                    printf("Pilihan tidak valid, silakan masukkan angka sesuai daftar.\n");
                    printf("\nSuggestion untuk \"%s\":\n", input);
                    for (int i = 0; i < limit; i++) {
                        printf("  [%d] %s (freq: %d)\n", i + 1, results[i].word, results[i].frequency);
                    }
                    printf("  [0] Tambah huruf\n");
                }
            }

            if (tambahHuruf) continue;

            char *kataKata = results[pilihan - 1].word;
            printf("\nKata yang dipilih: %s\n", kataKata);

            address node = searchTNode(T, kataKata);
            if (node != NULL) {
                node->frequency++;
            }
            saveFrequencies(T, FREQ_FILE);

            int reset = -1;
            while (reset != 0 && reset != 1) {
                printf("\n[1] Cari kata baru\n[0] Keluar\nPilihan: ");
                if (scanf("%d", &reset) != 1) {
                    while (getchar() != '\n');
                    reset = -1;
                }
            }

            if (reset == 0) {
                cariLagi = 0;
            }
        }
    }

    printf("\nTerima kasih telah menggunakan Auto Suggestion System!\n");

    freeTrie(T);
    return 0;
}