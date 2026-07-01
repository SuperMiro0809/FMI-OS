#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

typedef struct {
    char name[8];
    uint32_t offset;
    uint32_t length;
} entry;

void doWork(const entry* e, int toParent);
void doWork(const entry* e, int toParent) {
    int fd = open(e->name, O_RDONLY);
    if (fd < 0) {
        err(1, "file open failed");
    }

    struct stat info;
    if (fstat(fd, &info) == -1) {
        err(1, "fstat");
    }

    if ((e->offset + e->length) * sizeof(uint16_t) > (size_t)info.st_size) {
        errx(1, "bad offset and length");
    }

    if (lseek(fd, e->offset * sizeof(uint16_t), SEEK_SET) == -1) {
        err(1, "seek failed");
    }

    uint16_t result = 0;
    uint16_t buf;
    for (uint32_t i = 0; i < e->length; i++) {
        if (read(fd, &buf, sizeof(buf)) == -1) {
            err(1, "read failed");
        }
        result ^= buf;
    }

    if (write(toParent, &result, sizeof(result)) == -1) {
        err(1, "write failed");
    }

    close(fd);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        errx(1, "bad args");
    }

    int file = open(argv[1], O_RDONLY);
    if (file < 0) {
        err(1, "file open failed");
    }

    struct stat info;
    if (fstat(file, &info) == -1) {
        err(1, "fstat");
    }

    if (info.st_size % sizeof(entry) != 0) {
        errx(1, "wrong size");
    }

    if (info.st_size / sizeof(entry) > 8) {
        errx(1, "too big");
    }

    entry e;
    int fd[2];
    if (pipe(fd) == -1) {
        err(1, "pipe");
    }

    int numChildren = 0;
    ssize_t readSize;

    while ((readSize = read(file, &e, sizeof(e))) > 0) {
        numChildren++;

        pid_t pid = fork();
        if (pid < 0) {
            err(1, "fork");
        } else if (pid == 0) {
            close(fd[0]);
            doWork(&e, fd[1]);
            close(fd[1]);
            exit(0);
        }
    }

    if (readSize == -1) {
        err(1, "read failed");
    }

    close(fd[1]);

    uint16_t res = 0;
    uint16_t buf;
    while ((readSize = read(fd[0], &buf, sizeof(buf))) > 0) {
        res ^= buf;
    }

    if (readSize == -1) {
        err(1, "read failed");
    }

    close(fd[0]);
    
    for (int i = 0; i < numChildren; i++) {
        if (wait(NULL) == -1) {
            err(1, "wait");
        }
    }

    char output[1024];
    int l = snprintf(output, sizeof(output), "result: %.4X\n", res);
    if (write(1, output, l) == -1) {
        err(1, "write failed");
    }

    close(file);

    return 0;
}
