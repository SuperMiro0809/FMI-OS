#include <unistd.h>
#include <err.h>
#include <sys/wait.h>

// cat /etc/passwd | cut -d: -f7 | sort | uniq -c | sort -n
int main(void) {
    
    int fd1[2], fd2[2], fd3[2], fd4[2];
    pipe(fd1);
    pipe(fd2);
    pipe(fd3);
    pipe(fd4);

    pid_t pid = fork();

    if (pid < 0) {
        err(1, "fork error");
    } else if (pid == 0) {
        close(fd1[0]);

        dup2(fd1[1], 1);
        close(fd1[1]);

        execlp("/bin/cat", "cat", "/etc/passwd", (char*)NULL);
        err(1, "bad execling");
    }

    int status;
    wait(&status);

    if (WIFEXITED(status) && WEXITSTATUS(status) != 0) return 0;
    
    close(fd1[1]); 
    pid = fork();

    if (pid < 0) {
        err(1, "fork error");
    } else if (pid == 0) {
        close(fd2[0]);

        dup2(fd1[0], 0);
        close(fd1[0]);
        
        dup2(fd2[1], 1);
        close(fd2[1]);


        execlp("/bin/cut", "cut", "-d:", "-f7", (char*)NULL);
        err(1, "bad execling");
    }

    wait(&status);
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0) return 0;
    
    close(fd1[0]);
    close(fd2[1]);
    pid = fork();
    
    if (pid < 0) {
        err(1, "fork error");
    } else if (pid == 0) {
        close(fd3[0]);

        dup2(fd2[0], 0);
        close(fd2[0]);

        dup2(fd3[1], 1);
        close(fd3[1]);

        execl("/bin/sort", "sort", (char*)NULL);
        err(1, "bad execling");
    }

    wait(&status);
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0) return 0;
    
    close(fd2[0]);
    close(fd3[1]);
    
    pid = fork();
    
    if (pid < 0) {
        err(1, "fork error");
    } else if (pid == 0) {
        close(fd4[0]);
        
        dup2(fd3[0], 0);
        close(fd3[0]);

        dup2(fd4[1], 1);
        close(fd4[1]);

        execlp("/bin/uniq", "uniq", "-c", (char*)NULL);
        err(1, "bad execling");
    }

    wait(&status);
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0) return 0;
    
    close(fd3[0]);
    close(fd4[1]);

    pid = fork();
    if (pid < 0) {
        err(1, "fork error");
    } else if (pid == 0) {
        dup2(fd4[0], 0);
        close(fd4[0]);

        execlp("/bin/sort", "sort", "-n", (char*)NULL);
        err(1, "bad execling");
    }

    wait(&status);
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0) return 0;
    
    close(fd4[0]);

    return 0;
}
