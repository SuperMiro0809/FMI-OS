#include <unistd.h>
#include <err.h>
#include <sys/wait.h>

// find DIR -printf "%T@ %p\n" | sort -rn | head -1 | cut -d' ' -f2-
int main(int argc, char* argv[]) {
    if (argc != 2) {
        errx(1, "bad args");
    }
    
    int fd1[2], fd2[2], fd3[2];
    if (pipe(fd1) == -1 || pipe(fd2) == -1 || pipe(fd3) == -1) {
        err(1, "pipe error");
    }
    
    pid_t pid = fork();

    if (pid < 0) {
        err(1, "fork error");        
    } else if (pid == 0) {
        dup2(fd1[1], 1);
        close(fd1[0]); close(fd1[1]);
        close(fd2[0]); close(fd2[1]);
        close(fd3[0]); close(fd3[1]);

        execlp("find", "find", argv[1], "-printf", "%T@ %p\n", (char*)NULL);
        err(1, "bad execeling");
    }

    pid = fork();
    if (pid < 0) {
        err(1, "fork failed");
    } else if (pid == 0) {
        dup2(fd1[0], 0);
        dup2(fd2[1], 1);
        close(fd1[0]); close(fd1[1]);
        close(fd2[0]); close(fd2[1]);
        close(fd3[0]); close(fd3[1]);

        execlp("sort", "sort", "-rn", (char*)NULL);
        err(1, "bad execeling");
    }

    pid = fork();
    if (pid < 0) {
        err(1, "fork failed");
    } else if (pid == 0) {
        dup2(fd2[0], 0);
        dup2(fd3[1], 1);
        close(fd1[0]); close(fd1[1]); 
        close(fd2[0]); close(fd2[1]);    
        close(fd3[0]); close(fd3[1]);    

        execlp("head", "head", "-1", (char*)NULL);
        err(1, "bad execeling"); 
    }

    pid = fork();
    if (pid < 0) {
        err(1, "fork failed");
    } else if (pid == 0) {
        dup2(fd3[0], 0);
        close(fd1[0]); close(fd1[1]); 
        close(fd2[0]); close(fd2[1]);        
        close(fd3[0]); close(fd3[1]);

        execlp("cut", "cut", "-d ", "-f2-", (char*)NULL);
        err(1, "bad execling");
    }

    close(fd1[0]); close(fd1[1]);
    close(fd2[0]); close(fd2[1]);
    close(fd3[0]); close(fd3[1]);
    
    for (int i = 0; i < 4; i++) {
        wait(NULL);
    }

    return 0;
}
