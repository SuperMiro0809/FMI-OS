#include <fcntl.h>
#include <err.h>
#include <unistd.h>

int main(void) {
    int file = open("passwd", O_RDWR);
    if (file < 0) {
        err(1, "file open failed");
    }

    int temp = open("temp_passwd", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (temp < 0) {
        err(1, "file open failed");
    }

    char c;
    while (read(file, &c, sizeof(c)) == sizeof(c)) {
        if (c == ':') {
            c = '?';
            write(temp, &c, 1);
        } else {
            write(temp, &c, 1);
        }
    }

    if (lseek(temp, 0, SEEK_SET) == -1) {
        err(1, "seek failed");
    }

    if (ftruncate(file, 0) == -1) {
        err(1, "truncate failed");
    }

    if (lseek(file, 0, SEEK_SET) == -1) {
        err(1, "seek failed");
    }

    char buf[4096];
    ssize_t n;
    while ((n = read(temp, buf, sizeof(buf))) > 0) {
        if (write(file, buf, n) != n) {
            err(1, "write failed");
        }
    }

    if (n < 0) {
        err(1, "read failed");
    }

    close(file);
    close(temp);

    unlink("temp_passwd");

    return 0;
}
