#include <fcntl.h>
#include <err.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc == 1) {
        errx(1, "bad args");
    }

    char buf[4096];
    ssize_t n;

    for (int i = 1; i < argc; i++) {
        int file = open(argv[i], O_RDONLY);
        if (file < 0) {
            err(1, "file open failed");
        }

        while ((n = read(file, buf, sizeof(buf))) > 0) {
            if (write(1, buf, n) != n) {
                err(1, "write failed");
            }
        }

        if (n < 0) {
            err(1, "read failed");
        }

        close(file);
    }

    return 0;
}
