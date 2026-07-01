#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        errx(1, "bad args");
    }

    int f1 = open(argv[1], O_RDONLY);
    if (f1 < 0) {
        err(1, "file open failed");
    }

    int f2 = open(argv[2], O_RDONLY);
    if (f2 < 0) {
        err(1, "file open failed");
    }

    int patch = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (patch < 0) {
        err(1, "file open failed");
    }

    uint8_t b1, b2;
    ssize_t r1, r2;

    while (1) {
        if ((r1 = read(f1, &b1, sizeof(b1))) < 0) {
            err(1, "read failed");
        }
        if ((r2 = read(f2, &b2, sizeof(b2))) < 0) {
            err(1, "read failed");
        }

        if (r1 != r2) {
            errx(1, "files have different sizes");
        } else if (r1 == 0 && r2 == 0) {
            break;
        }

        if (b1 != b2) {
            uint16_t offset = lseek(f1, 0, SEEK_CUR) - 1;

            write(patch, &offset, sizeof(offset));
            write(patch, &b1, sizeof(b1));
            write(patch, &b2, sizeof(b2));
        }
    }

    close(f1);
    close(f2);
    close(patch);

    return 0;
}
