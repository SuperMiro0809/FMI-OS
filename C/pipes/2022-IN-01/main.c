#include <unistd.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/wait.h>

const char* DING = "DING ";
const char* DONG = "DONG \n";

void doParent(int, int, int, int);
void doParent(int fromChild, int toChild, int N, int D) {
    uint8_t buf = 0;

    for (int i = 0; i < N; i++) {
        write(1, DING, strlen(DING)); 
        write(toChild, &buf, sizeof(buf));
        read(fromChild, &buf, sizeof(buf));
        sleep(D);
    }
}

void doChild(int, int, int);
void doChild(int fromParent, int toParent, int N) {
    uint8_t buf = 0;

    for (int i = 0; i < N; i++) {
        read(fromParent, &buf, sizeof(buf));
        write(1, DONG, strlen(DONG));
        write(toParent, &buf, sizeof(buf));
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "bad args");
    }
    
    char* end;
    long N = strtol(argv[1], &end, 10);
    if (*argv[1] == '\0' || *end != '\0') {
        errx(1, "N is not a number");
    }

    long D = strtol(argv[2], &end, 10);
    if (*argv[2] == '\0' || *end != '\0') {
        errx(1, "D is not a number");
    }

    int parentToChild[2];
    if (pipe(parentToChild) == -1) { 
        err(1, "pipe");
    }
     
    int childToParent[2];
    if (pipe(childToParent) == -1) {
        err(1, "pipe");
    }

    pid_t pid = fork();

    if (pid < 0) {
        err(1, "fork");
    } else if (pid == 0) {
        close(parentToChild[1]);
        close(childToParent[0]);

        int fromParent = parentToChild[0];
        int toParent = childToParent[1];
        
        doChild(fromParent, toParent, N);

        close(parentToChild[0]);
        close(childToParent[1]);

        exit(0);
    }

    close(childToParent[1]);
    close(parentToChild[0]);

    int fromChild = childToParent[0];
    int toChild = parentToChild[1];

    doParent(fromChild, toChild, N, D);

    close(childToParent[0]);
    close(parentToChild[1]);

    int status;
    if (wait(&status) == -1) {
        err(1, "wait");
    }

    if (!WIFEXITED(status)) {
         warnx("child was killed");
    }

    return 0;
}
