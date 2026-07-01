#include <unistd.h>
#include <err.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        errx(1, "bad args");
    }

    int fds[2];
    pipe(fds);

    pid_t pid = fork();

    if (pid < 0) {
        err(1, "fork error");
    } else if (pid == 0) {
        close(fds[0]);

        dup2(fds[1], 1);
        close(fds[1]);

        execlp("/bin/cat", "cat", argv[1], (char*)NULL);
        err(1, "bad execling");
    }

    int status;
    wait(&status);
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0) err(1, "err");

    pid_t pid2 = fork();

    if (pid2 < 0) {
        err(1, "fork error");
    } else if (pid2 == 0) {
        close(fds[1]);
        
        dup2(fds[0], 0);
        close(fds[0]);

        execlp("/bin/sort", "sort", (char*)NULL);
        err(1, "bad execling");
    }

    close(fds[0]);
    close(fds[1]);
    
    wait(NULL);
    return 0;    
}
