#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>

// find DIR -type f ! -name ".hash"
int main(int argc, char* argv[]) {
    if (argc != 2) {
        errx(1, "bad args");
    }

    int fd[2];
    if (pipe(fd) == -1) {
        err(1, "pipe");
    }

    pid_t pid = fork();

    if (pid < 0) {
        err(1, "fork");
    } else if (pid == 0) {
        close(fd[0]);
        
        dup2(fd[1], 1);
        close(fd[1]);

        execlp("find", "find", argv[1], "-type", "f", "!", "-name", "*.hash", (char*)NULL);
        err(1, "bad execling");
    }

    close(fd[1]);
    
    char line[1024];
    char c;
    int len = 0;

    int nChildren = 0;
    while (read(fd[0], &c, sizeof(c)) == (ssize_t)sizeof(c)) {
        if (c == '\n') {
            line[len] = '\0';

            char hashpath[2048];
            snprintf(hashpath, sizeof(hashpath), "%s.hash", line);

            pid = fork();

            if (pid < 0) {
                err(1, "fork");
            } else if (pid == 0) {
                int out = open(hashpath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (out < 0) {
                    err(1, "file open failed");
                }

                dup2(out, 1);
                close(out);
                close(fd[0]);

                execlp("md5sum", "md5sum", line, (char*)NULL);
                err(1, "bad execling");
            }

            len = 0;
            nChildren++;
        } else {
            line[len++] = c;
        }
    }

    close(fd[0]);

    for (int i = 0; i < nChildren + 1; i++) {
        if (wait(NULL) == -1) {
            err(1, "wait");
        }
    }

    return 0;
}
