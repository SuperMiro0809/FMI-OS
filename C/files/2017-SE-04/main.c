#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <string.h>

static void process(int fd) {
    char buf[4096];
    ssize_t n;

    while((n = read(fd, buf, sizeof(buf))) > 0) {
        write(1, buf, n);
    }
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        process(0);
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        int fd;
        
        if (strncmp("-", argv[i], 1) == 0) {
            fd = 0;
        } else {
            fd = open(argv[i], O_RDONLY);
            if (fd < 0) {
                err(1, "file open failed");
            }
        }

        process(fd);

        if (fd != 0) {
            close(fd);
        }
    }

    return 0;
}
