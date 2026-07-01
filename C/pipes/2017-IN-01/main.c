#include <unistd.h>
#include <err.h>
#include <sys/wait.h>

/* cat /etc/passwd | cut -d: -f7 | sort | uniq -c | sort -n */
int main(void) {
    int fd1[2], fd2[2], fd3[2], fd4[2];
    if (pipe(fd1) == -1 || pipe(fd2) == -1 ||
        pipe(fd3) == -1 || pipe(fd4) == -1) {
        err(1, "pipe");
    }

    pid_t pid;

    /* --- 1) cat: stdout -> fd1 --- */
    pid = fork();
    if (pid < 0) err(1, "fork");
    if (pid == 0) {
        dup2(fd1[1], 1);
        close(fd1[0]); close(fd1[1]);
        close(fd2[0]); close(fd2[1]);
        close(fd3[0]); close(fd3[1]);
        close(fd4[0]); close(fd4[1]);
        execlp("cat", "cat", "/etc/passwd", (char*)NULL);
        err(1, "exec cat");
    }

    /* --- 2) cut: fd1 -> fd2 --- */
    pid = fork();
    if (pid < 0) err(1, "fork");
    if (pid == 0) {
        dup2(fd1[0], 0);
        dup2(fd2[1], 1);
        close(fd1[0]); close(fd1[1]);
        close(fd2[0]); close(fd2[1]);
        close(fd3[0]); close(fd3[1]);
        close(fd4[0]); close(fd4[1]);
        execlp("cut", "cut", "-d:", "-f7", (char*)NULL);
        err(1, "exec cut");
    }

    /* --- 3) sort: fd2 -> fd3 --- */
    pid = fork();
    if (pid < 0) err(1, "fork");
    if (pid == 0) {
        dup2(fd2[0], 0);
        dup2(fd3[1], 1);
        close(fd1[0]); close(fd1[1]);
        close(fd2[0]); close(fd2[1]);
        close(fd3[0]); close(fd3[1]);
        close(fd4[0]); close(fd4[1]);
        execlp("sort", "sort", (char*)NULL);
        err(1, "exec sort");
    }

    /* --- 4) uniq -c: fd3 -> fd4 --- */
    pid = fork();
    if (pid < 0) err(1, "fork");
    if (pid == 0) {
        dup2(fd3[0], 0);
        dup2(fd4[1], 1);
        close(fd1[0]); close(fd1[1]);
        close(fd2[0]); close(fd2[1]);
        close(fd3[0]); close(fd3[1]);
        close(fd4[0]); close(fd4[1]);
        execlp("uniq", "uniq", "-c", (char*)NULL);
        err(1, "exec uniq");
    }

    /* --- 5) sort -n: fd4 -> stdout --- */
    pid = fork();
    if (pid < 0) err(1, "fork");
    if (pid == 0) {
        dup2(fd4[0], 0);
        close(fd1[0]); close(fd1[1]);
        close(fd2[0]); close(fd2[1]);
        close(fd3[0]); close(fd3[1]);
        close(fd4[0]); close(fd4[1]);
        execlp("sort", "sort", "-n", (char*)NULL);
        err(1, "exec sort -n");
    }

    /* --- родителят затваря ВСИЧКИ краища --- */
    close(fd1[0]); close(fd1[1]);
    close(fd2[0]); close(fd2[1]);
    close(fd3[0]); close(fd3[1]);
    close(fd4[0]); close(fd4[1]);

    /* --- чак сега чакаме всички 5 деца --- */
    for (int i = 0; i < 5; i++) {
        wait(NULL);
    }

    return 0;
}
