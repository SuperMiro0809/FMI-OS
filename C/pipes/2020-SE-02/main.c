#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "bad args");
    }
    
    int fd[2];
    if (pipe(fd) == -1) {
        err(1, "pipe error");
    }

    pid_t pid = fork();

    if (pid < 0) {
        err(1, "fork failed");
    } else if (pid == 0) {
        close(fd[0]);

        dup2(fd[1], 1);
        close(fd[1]);

        execlp("cat", "cat", argv[1], (char*)NULL);
        err(1, "bad execling");
    }
    
    close(fd[1]);
    
    int output = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output < 0) {
        err(1, "file open failed");
    }

    uint8_t b;
    while(read(fd[0], &b, sizeof(b)) == (ssize_t)sizeof(b)) {
        if (b == 0x55) continue;

        if (b == 0x7D) {
            uint8_t next;
            if (read(fd[0], &next, 1) != 1) errx(1, "truncated escape");
            uint8_t r = next ^ 0x20;
            
            if (write(output, &r, sizeof(r)) != (ssize_t)sizeof(r)) {
                err(1, "write failed");
            }
        } else {
            if (write(output, &b, sizeof(b)) != (ssize_t)sizeof(b)) {
                err(1, "write failed");
            }
        }
    }

    close(fd[0]);
    close(output);

    wait(NULL); 

    return 0;
}
