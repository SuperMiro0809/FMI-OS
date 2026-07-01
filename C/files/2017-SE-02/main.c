#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

static void process(int fd, int opt_n, int *line) {
    char c;
    int isPrinted = 0;

    while (read(fd, &c, sizeof(c)) == sizeof(c)) {
        if (opt_n == 1) {
            if (isPrinted == 0) {
                char buf[1000];
                int n = snprintf(buf, sizeof(buf), "%d ", *line);
                write(1, buf, n);
                isPrinted = 1;
            }

            if (c == '\n') {
                (*line)++;
                isPrinted = 0;
            }
        }

        write(1, &c, 1);
    }
}

int main(int argc, char* argv[]) {
    
    int opt_n = 0;
    int start = 1;

    if (argc > 1 && strcmp(argv[1], "-n") == 0) {
        opt_n = 1;
        start = 2;
    }

    int line = 1;

    if (start == argc) {
        process(0, opt_n, &line);
        return 0;
    }

    for (int i = start; i < argc; i++) {
        int fd;
        if (strcmp(argv[i], "-") == 0) {
            fd = 0;
        } else {
            fd = open(argv[i], O_RDONLY);
            if (fd < 0) {
                err(1, "file open failed");
            }
        }

        process(fd, opt_n, &line);

        if (fd != 0) {
            close(fd);
        }
    }

    return 0;
}
