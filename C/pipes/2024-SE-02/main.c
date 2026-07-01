#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <signal.h>

pid_t start(char* prog);
pid_t start(char* prog) {
    pid_t pid = fork();

    if (pid < 0) {
        err(1, "fork");
    } else if (pid == 0) {
        execl(prog, prog, (char*)NULL);
        _exit(127);
    }

    return pid;
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 11) {
        errx(1, "bad args");
    }

    int n = argc - 1;
    pid_t pids[10];

    for (int i = 0; i < n; i++) {
        pids[i] = start(argv[i + 1]);
    }

    int done = 0;
    while (done < n) {
        int status;
        pid_t finished = wait(&status);
        
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (pids[i] == finished) {
                idx = i;
                break;
            }
        }

        if (idx == -1) continue;

        if (WIFSIGNALED(status)) {
            for (int i = 0; i < n; i++) {
                if (i != idx && pids[i] > 0) {
                    kill(pids[i], SIGTERM);
                }
            }

            for (int i = 0; i < n; i++) {
                if (i != idx && pids[i] > 0) {
                    waitpid(pids[i], NULL, 0);
                }
            }

            return idx + 1;
        } else if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            pids[idx] = 0;
            done++;
        } else {
            pids[idx] = start(argv[idx + 1]);
        }
    }
    
    return 0;
}
