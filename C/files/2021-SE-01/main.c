#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>

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
    while(read(input, &byte, sizeof(byte)) == (ssize_t)sizeof(byte)) {
        uint8_t nByte = 0;

        for (unsigned i = 0; i < 8; i++) {
            uint8_t bit = (byte >> (7 - i)) & 1;
            int pos = 7 - (i % 4) * 2;

            if (bit == 1) {
                nByte = nByte ^ (1 << pos);
            } else {
                nByte = nByte ^ (1 << (pos - 1));
            }

            if (i % 4 == 3) {
                write(output, &nByte, sizeof(nByte));
                nByte = 0;
            }
        }
    }

    close(input);
    close(output);

    return 0;
}
