#include <unistd.h>
#include <err.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        errx(1, "bad args");
    }
    
    pid_t pid = fork();
    if (pid == 0) {
        execl(argv[1], argv[1], (char*)NULL);
        err(1, "bad execling");
    } else {
        int status;
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            write(1, argv[1], strlen(argv[1]));
            write(1, "\n", 1);
        }
    }

    return 0;
}
