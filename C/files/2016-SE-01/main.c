#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        errx(1, "bad args");
    }

    int file = open(argv[1], O_RDWR);
    if (file < 0) {
        err(1, "open file failed");
    }

    uint64_t counts[256] = {0};
    uint8_t b;

    while (read(file, &b, sizeof(b)) == sizeof(b)) {
        counts[b]++;
    }

    if (lseek(file, 0, SEEK_SET) == -1) {
        err(1, "seek failed");
    }

    for (int i = 0; i < 256; i++) {
        uint8_t byte = (uint8_t)i;
        for (uint64_t j = 0; j < counts[i]; j++) {
            write(file, &byte, 1);
        }
    }

    close(file);
    return 0;
}
