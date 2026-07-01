#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

static int read_full(int fd, void* buf, size_t len) {
    size_t got = 0;
    char* p = buf;
    while (got < len) {
        ssize_t r = read(fd, p + got, len - got);
        if (r <= 0) return -1;
        got += (size_t)r;
    }
    return 0;
}

static int write_full(int fd, const void* buf, size_t len) {
    size_t put = 0;
    const char* p = buf;
    while (put < len) {
        ssize_t w = write(fd, p + put, len - put);
        if (w == -1) return -1;
        put += (size_t)w;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        err(1, "bad args");
    }
    
    char* end;
    long N = strtol(argv[2], &end, 10);
    if (*argv[2] == '\0' || *end != '\0') {
        err(1, "not a number");
    }
    
    char* prog = argv[1];
    char* resultPath = argv[3];

    int urandom = open("/dev/urandom", O_RDONLY);
    if (urandom < 0) {
        err(1, "file open failed");
    }
    
    int result = open(resultPath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (result < 0) {
        err(1, "file open failed");
    }
    
    uint8_t input[65536];
    int crashed = 0;
    for (int iter = 0; iter < N && !crashed; iter++) {
        uint16_t S;
        if (read_full(urandom, &S, sizeof(S)) == -1) return 1;

        if (S > 0 && read_full(urandom, input, S) == -1) return 1;

        int fd[2];
        if (pipe(fd) == -1) return 1;

        pid_t pid = fork();

        if (pid < 0) {
            err(1, "fork");
        } else if (pid == 0) {
            dup2(fd[0], 0);
            int devnull = open("/dev/null", O_WRONLY);
            if (devnull < 0) _exit(127);
            dup2(devnull, 1);
            dup2(devnull, 2);
            close(devnull);
            close(fd[0]);
            close(fd[1]);
            execlp(prog, prog, (char*)NULL);
            _exit(127);
        }

        close(fd[0]);
        write_full(fd[1], input, S);
        close(fd[1]);

        int status;
        if (waitpid(pid, &status, 0) == -1) return 1;

        if (WIFSIGNALED(status)) {
             if (write_full(result, input, S) == -1) return 1;
             crashed = 1;
        }
    }

    close(result);
    close(urandom);
    return crashed ? 42 : 0;

    return 0;
}
