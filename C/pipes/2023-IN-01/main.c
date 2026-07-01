#include <unistd.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

const char* words[] = { "tic ", "tac ", "toe\n" };

void doWork(int, int, int);
void doWork(int readFrom, int writeTo, int wordCount) {
    int toPrint = 0;
    ssize_t r;

    while ((r = read(readFrom, &toPrint, sizeof(toPrint))) > 0) {
        toPrint += 1;

        if (toPrint >= wordCount) {
            if (write(writeTo, &toPrint, sizeof(toPrint)) == -1) {
                err(1, "write failed");
            }
            return;
        };

        if (write(1, words[toPrint % 3], strlen(words[toPrint % 3])) == -1) {
            err(1, "write failed");
        }
 
        if (write(writeTo, &toPrint, sizeof(toPrint)) == -1) {
          err(1, "write failed");
        }
    }
    
    if (r == -1) {
        err(1, "read failed");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "bad args");
    }

    char* end;
    long NC = strtol(argv[1], &end, 10);
    if (*argv[1] == '\0' || *end != '\0') {
        errx(1, "not a number");
    }
    
    if (NC < 0 || NC > 7) {
        errx(1, "NC is incorrect");
    }

    long WC = strtol(argv[2], &end, 10);
    if (*argv[2] == '\0' || *end != '\0') {
        errx(1, "not a number");
    }

    if (WC < 0 || WC > 35) {
        errx(1, "WC is incorrect");
    }
    
    int pipes[8][2];
    int numPipes = NC + 1;
    for (int i = 0; i < numPipes; i++) {
        if (pipe(pipes[i]) == -1) {
            err(1, "pipe");
        }
    }
    
    for (int i = 1; i <= NC; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            err(1, "fork");
        } else if (pid == 0) {
            int readFrom = pipes[i - 1][0];
            int writeTo = pipes[i][1];
            
            for (int j = 0; j < numPipes; j++) {
                if (j != i - 1) {
                    close(pipes[j][0]);
                }
                if (j != i) {
                    close(pipes[j][1]);
                }
            }

            doWork(readFrom, writeTo, WC);
            
            close(readFrom);
            close(writeTo);
            exit(0);
        }
    }
    
    int readFrom = pipes[NC][0];
    int writeTo = pipes[0][1];

    for (int i = 0; i < numPipes; i++) {
        if (i != NC) {
            close(pipes[i][0]);
        }
        if (i != 0) {
            close(pipes[i][1]);
        }
    }

    int toPrint = 0;
    if (write(1, words[toPrint % 3], strlen(words[toPrint % 3])) == -1) {
        err(1, "write failed");
    }

    if (write(writeTo, &toPrint, sizeof(toPrint)) == -1) {
        err(1, "write failed");
    }

    doWork(readFrom, writeTo, WC);

    close(readFrom);
    close(writeTo);
    
    for (int i = 0; i < NC; i++) {
        if (wait(NULL) == -1) {
            err(1, "wait");
        }
    }

    return 0;
}
