#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        errx(1, "bad args");
    }

    int file = open(argv[1], O_RDONLY);
    if (file < 0) {
        err(1, "file open failed");
    }

    int lines = 0;
    int words = 0;
    int chars = 0;
    char c;
    
    bool word = false;

    while (read(file, &c, sizeof(c)) == sizeof(c)) {
        chars++;

        if (c == '\n') {
            lines++;

            if (word) {
                words++;
                word = false;
            }
        } else if (c == ' ') {
            if (word) {
                words++;
                word = false;
            }
        } else {
            word = true;
        }
    }

    if (word) {
        words++;
    }

    close(file);
    
    printf("File has %d lines;\nFile has %d words;\nFile has %d chars;\n", lines, words, chars);

    return 0;
}
