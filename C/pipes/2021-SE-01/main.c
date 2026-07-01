#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <pwd.h>
#include <sys/time.h>
#include <stdio.h>
#include <time.h>
#include <sys/wait.h>

static void run_via_pipe(char* argv[]) {
    int fd[2];
    if (pipe(fd) == -1) {
        err(1, "pipe");
    }

    pid_t pid = fork();

    if (pid < 0) {
        err(1, "fork");
    } else if (pid == 0) {
        close(fd[0]);
        close(fd[1]);
        execvp(argv[0], argv);
        err(1, "execvp");
    }

    close(fd[0]);
    close(fd[1]);
    if (wait(NULL) == -1) {
        err(1, "wait");
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        errx(1, "bad args");
    }
    
    uid_t uid = getuid();
    struct passwd* pw = getpwuid(uid);
    if (pw == NULL) {
        err(1, "getpwuid");
    }
    char* username = pw->pw_name;

    struct timeval tv;
    if (gettimeofday(&tv, NULL) == -1) {
        err(1, "gettimeofday");
    }
    struct tm* t = localtime(&tv.tv_sec);
    if (t == NULL) err(1, "localtime");
    char datebuf[32];
    if (strftime(datebuf, sizeof(datebuf), "%Y-%m-%d %H:%M:%S", t) == 0) errx(1, "strftime");

     
    char line[4096];
    int pos = snprintf(line, sizeof(line), "%s.%06ld %s", datebuf, (long)tv.tv_usec, username);
    for (int i = 1; i < argc; i++) {
        pos += snprintf(line + pos, sizeof(line) - pos, " %s", argv[i]);
    }
    pos += snprintf(line + pos, sizeof(line) - pos, "\n");

    int log = open("foo.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log < 0) {
        err(1, "file open failed");
    }
    if (write(log, line, pos) != pos) {
        err(1, "write failed");
    }
    close(log);

    char c_echo[]   = "echo";
    char c_passwd[] = "passwd";
    char c_pkill[]  = "pkill";
    char c_l[]      = "-l";
    char c_u[]      = "-u";

    char* lock[] = { c_echo, c_passwd, c_l, username, NULL };
    run_via_pipe(lock);

    char* kill[] = { c_echo, c_pkill, c_u, username, NULL };
    run_via_pipe(kill);
    
    return 0;
}
