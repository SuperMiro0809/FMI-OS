#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return 26;
    }
    
    pid_t pids[256];
    for (int i = 1; i < argc; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            err(1, "fork");
        } else if (pid == 0) {
            int fd[2];

            if (pipe(fd) == -1) {
                err(1, "pipe");
            }

            pid_t inner = fork();
            
            if (inner < 0) {
                exit(26);
            } else if (inner == 0) {
                dup2(fd[1], 1);
                close(fd[0]);
                close(fd[1]);
                execlp(argv[i], argv[i], (char*)NULL);
                exit(26);
            }
            
            close(fd[1]);
            dup2(fd[0], 0);
            close(fd[0]);
            execlp("grep", "grep", "-q", "found it!", (char*)NULL);
            exit(26);
        }

        pids[i-1] = pid;
    }

    int found = 0;
    int remaining = argc - 1;
    while (remaining > 0) {
        int status;
        pid_t done = wait(&status);
        if (done == -1) break;
        remaining--;
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            found = 1;
            break;
        }
    }
    
    for (int i = 0; i < argc - 1; i++) kill(pids[i], SIGTERM);
    for (int i = 0; i < argc - 1; i++) waitpid(pids[i], NULL, 0);
    
    return found ? 0 : 1;
}
