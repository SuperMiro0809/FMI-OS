#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint64_t next;
    uint8_t data[504];
} object;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        errx(1, "bad args");
    }

    int file = open(argv[1], O_RDWR);
    if (file < 0) {
        err(1, "file open failed");
    }

    char tmpl[] = "/tmp/censor.XXXXXX";
    int tmp = mkstemp(tmpl);
    if (tmp == -1) {
        err(1, "mkstemp failed");
    }
    
    object o;
    uint8_t f = 1;

    while (read(file, &o, sizeof(o)) == (ssize_t)sizeof(o)) {
        long cur = lseek(file, 0, SEEK_CUR);
            
        if (lseek(tmp, (cur / sizeof(object) - 1) * sizeof(uint8_t), SEEK_SET) == -1) {
            err(1, "seek failed");
        }

        if (write(tmp, &f, sizeof(f)) != (ssize_t)sizeof(f)) {
            err(1, "write failed");
        }

        if (o.next == 0) break;

        if (lseek(file, o.next * sizeof(object), SEEK_SET) == -1) {
            err(1, "seek failed");
        }
    }

    if (lseek(file, 0, SEEK_SET) == -1) {
        err(1, "seek failed");
    }

    int i = 0;
    uint8_t zeros[512] = {0};
    while (read(file, &o, sizeof(o)) == (ssize_t)sizeof(o)) {
        if (lseek(tmp, i * sizeof(uint8_t), SEEK_SET) == -1) {
            err(1, "seek failed");
        }

        uint8_t c = 0;
        int r = read(tmp, &c, sizeof(c));
        if (r == -1) {
            err(1, "read failed");
        }

        if (c == 0) {
            if (lseek(file, i * sizeof(object), SEEK_SET) == -1) {
                err(1, "seek failed");
            }

            write(file, zeros, sizeof(zeros));
        }
        i++;
    }

    close(file);
    unlink(tmpl);

    return 0;
}
