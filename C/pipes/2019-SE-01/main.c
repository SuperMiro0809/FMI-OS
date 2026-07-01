#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        errx(1, "bad args");
    }
    
    if (argv[1][0] < '0' || argv[1][0] > '9') {
        errx(1, "incorrect number");
    }

    int dur = argv[1][0] - '0';
    
    int log = open("run.log", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (log < 0) {
        err(1, "file open failed");
    }
    
    int prev_res = 0;
    while (1) {
        int start = time(NULL);
        
        pid_t pid = fork();

        if (pid < 0) {
            err(1, "fork failed");
        } else if (pid == 0) {
            char* args[10];
            int k = 0;
            args[k++] = argv[2];
            for (int i = 3; i < argc; i++) {
                args[k++] = argv[i];
            }
            args[k] = (char*)NULL;    

            execvp(argv[2], args);
            err(1, "bad execling");
        }
        
        int status, code;
        wait(&status);
        if (WIFEXITED(status)) {
            code = WEXITSTATUS(status);
        } else {
            code = 129;
        }

        int end = time(NULL);

        char buf[64];
        int n = snprintf(buf, sizeof(buf), "%d %d %d\n", start, end, code);
        if (write(log, buf, n) != n) {
            err(1, "write failed");
        }

        int cur_res = (code != 0) && (end - start < dur);
        if (cur_res == 1 && prev_res == 1) {
            break;
        }
        
        prev_res = cur_res;
    }

    return 0;
}
