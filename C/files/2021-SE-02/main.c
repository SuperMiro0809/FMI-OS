#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "bad args");
    }
    
    int input = open(argv[1], O_RDONLY);
    if (input < 0) {
        err(1, "file open failed");
    }

    int output = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output < 0) {
        err(1, "file open failed");
    }

    uint8_t byte;
    uint8_t res = 0;
    int pos = 7;
    while (read(input, &byte, sizeof(byte)) == (ssize_t)sizeof(byte)) {
        for (unsigned i = 0; i < 8; i+=2) {
            uint8_t bit = (byte >> (7 - i)) & 1;
            
            res = res ^ (bit << pos);
            pos--;
        }

        if (pos == -1) {
            pos = 7;
            write(output, &res, sizeof(res));
            res = 0;
        }
    }

    close(input);
    close(output);

    return 0;
}
