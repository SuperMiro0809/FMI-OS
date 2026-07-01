#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        errx(1, "bad args");
    }
    
    mkfifo("mypipe", 0666);

    int fd = open("mypipe", O_WRONLY);

    pid_t pid = fork();

    if (pid < 0) {
        err(1, "fork failed");
    } else if (pid == 0) {
        dup2(fd, 1);
        close(fd);

        execlp("cat", "cat", argv[1], (char*)NULL);
        err(1, "bad execling");
    }
    
    close(fd);
    wait(NULL);
    return 0;
}
