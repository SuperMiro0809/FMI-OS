#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        errx(1, "bad args");
    }

    int patch = open(argv[1], O_RDONLY);
    if (patch < 0) {
        err(1, "file open failed");
    }
    
    int f1 = open(argv[2], O_RDONLY);
    if (f1 < 0) {
        err(1, "file open failed");
    }

    int f2 = open(argv[3], O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (f2 < 0) {
        err(1, "file open failed");
    }

    char buf[4096];
    ssize_t n;

    while ((n = read(f1, buf, sizeof(buf))) > 0) {
        if (write(f2, buf, n) != n) {
            err(1, "write failed");
        }
    }

    if (n < 0) {
        err(1, "read failed");
    }

    uint16_t offset;
    uint8_t byte1, byte2, current;

    while (read(patch, &offset, sizeof(offset)) == sizeof(offset) && read(patch, &byte1, sizeof(byte1)) == sizeof(byte1) && read(patch, &byte2, sizeof(byte2)) == sizeof(byte2)) {
        if (lseek(f2, offset * sizeof(uint8_t), SEEK_SET) == -1) {
            err(1, "seek failed");
        }

        if (read(f2, &current, sizeof(current)) != sizeof(current)) {
            err(1, "read failed");
        }

        if (byte1 != current) {
            err(1, "original byte mismatch");
        }

        if (lseek(f2, offset * sizeof(uint8_t), SEEK_SET) == -1) {
            err(1, "seek failed");
        }

        if (write(f2, &byte2, sizeof(byte2)) != sizeof(byte2)) {
            err(1, "write failed");
        }
    }

    close(patch);
    close(f1);
    close(f2);

    return 0;
}
