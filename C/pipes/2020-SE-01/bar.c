#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        err(1, "bad args");
    }
    
    int fd = open("mypipe", O_RDONLY);

    pid_t pid = fork();
    
    if (pid < 0) {
        err(1, "fork error");
    } else if (pid == 0) {
        dup2(fd, 0);
        close(fd);

        execl(argv[1], argv[1], (char*)NULL);
        err(1, "bad execling");
    }

    close(fd);
    wait(NULL);
    return 0;
}
