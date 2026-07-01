#include <fcntl.h>
#include <err.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "invalid args");
    }

    int file1 = open(argv[1], O_RDONLY);
    if (file1 < 0) {
        err(1, "file open failed");
    }

    int file2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file2 < 0) {
        err(1, "file open failed");
    }

    char buf[4096];
    ssize_t n;

    while ((n = read(file1, buf, sizeof(buf))) > 0) {
        if (write(file2, buf, n) != n) {
            err(1, "write failed");
        }
    }

    if (n < 0) {
        err(1, "read failed");
    }

    close(file1);
    close(file2);

    return 0;
}
