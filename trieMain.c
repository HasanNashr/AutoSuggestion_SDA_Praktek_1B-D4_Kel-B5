#include "trie.h"

int main() {
    // ===== Inisialisasi =====
    Root T = createTNode();
    loadDataset(&T, DATASET_FILE);
    loadFrequencies(T, FREQ_FILE);

    char input[MAX_WORD_LEN];
    int pilihan;

    printf("=== Auto Suggestion System ===\n");

    int cariLagi = 1;
    while (cariLagi) {
        // ===== Tahap 2: Terima input prefix =====
        printf("\nKetik awalan kata: ");
        if (scanf("%99s", input) != 1) break;

        // Loop inner untuk "Tambah huruf" tanpa kembali ke prompt awal
        int tambahHuruf = 1;
        while (tambahHuruf) {
            tambahHuruf = 0;

            // ===== Tahap 3-5: Navigasi trie & kumpulkan suggestion =====
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
                break; // keluar inner loop, minta prefix baru
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

            // ===== Tahap 6: Tampilkan hasil (maks 10) =====
            int limit = (count < 10) ? count : 10;
            printf("\nSuggestion untuk \"%s\":\n", input);
            for (int i = 0; i < limit; i++) {
                printf("  [%d] %s (freq: %d)\n", i + 1, results[i].word, results[i].frequency);
            }
            printf("  [0] Tambah huruf\n");

            // ===== Tahap 7: Validasi pilihan =====
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
                    // Tambah huruf ke prefix yang sudah ada
                    char tambahan[MAX_WORD_LEN];
                    printf("Tambah huruf (prefix saat ini \"%s\"): ", input);
                    if (scanf("%99s", tambahan) == 1) {
                        strncat(input, tambahan, MAX_WORD_LEN - strlen(input) - 1);
                    }
                    pilihanValid = 1;
                    tambahHuruf = 1; // ulangi inner loop dengan prefix baru
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

            if (tambahHuruf) continue; // kembali ke awal inner loop dengan prefix baru

            // ===== Tahap 8: Proses pilihan & update frekuensi =====
            char *kataKata = results[pilihan - 1].word;
            printf("\nKata yang dipilih: %s\n", kataKata);

            address node = searchTNode(T, kataKata);
            if (node != NULL) {
                node->frequency++;
            }
            saveFrequencies(T, FREQ_FILE);

            // ===== Tahap 9: Reset atau keluar =====
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
        } // end inner loop
    } // end outer loop

    printf("\nTerima kasih telah menggunakan Auto Suggestion System!\n");

    freeTrie(T);
    return 0;
}