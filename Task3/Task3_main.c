int main(void)
{
    char filename[] = "input.txt";   /* имя файла */
    char result[513];                /* буфер для найденного слова */
    int r;

    r = search(filename, result);

    if (r == -1) {
        printf("Cannot open file\n");
        return 1;
    }

    if (result[0] != '\0') {
        printf("Longest word: %s\n", result);
    } else {
        printf("No suitable word found\n");
    }

    return 0;
}
