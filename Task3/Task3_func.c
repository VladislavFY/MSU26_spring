/* search_word.c — поиск самого длинного слова, состоящего только из
   английских букв и содержащего заглавную букву.
   Требования:
     - строки длиной не более 512 символов (мы резервируем буфер чуть больше),
     - слова разделяются пробелами и табуляцией (и переводом строки).
   Предположение: буфер word должен быть как минимум 513 байт (512 + '\0').
*/

#include <stdio.h>
#include <string.h>

/* Функция возвращает:
     0  — если файл удалось открыть (даже если подходящих слов нет),
    -1  — если файл открыть не удалось.
   Результат (самое длинное подходящее слово) записывается в word (C-строка).
*/
int search(const char *SInputFile, char *word) {
    FILE *f = fopen(SInputFile, "r");
    if (!f) {
        return -1; /* не удалось открыть файл */
    }

    char line[1024]; /* достаточно для строки до 512 символов + запас */
    size_t best_len = 0;
    word[0] = '\0';

    while (fgets(line, sizeof(line), f) != NULL) {
        /* разбиваем строку на токены по пробелам/табам/переводу строки */
        char *tok = strtok(line, " \t\n");
        while (tok != NULL) {
            size_t len = strlen(tok);
            int only_letters = 1;
            int has_upper = 0;

            /* проверяем, что токен состоит только из латинских букв и есть заглавная */
            for (size_t i = 0; i < len; ++i) {
                unsigned char c = (unsigned char)tok[i];
                if (c >= 'A' && c <= 'Z') {
                    has_upper = 1;
                }
                if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))) {
                    only_letters = 0;
                    break;
                }
            }

            if (only_letters && has_upper) {
                if (len > best_len) {
                    /* сохраняем (обрезаем до 512 символов на случай эксцесса) */
                    size_t copy_len = (len < 512) ? len : 512;
                    memcpy(word, tok, copy_len);
                    word[copy_len] = '\0';
                    best_len = copy_len;
                }
            }

            tok = strtok(NULL, " \t\n");
        }
    }

    fclose(f);
    return 0;
}


