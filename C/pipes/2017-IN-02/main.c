#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdint.h>
#include <string.h>

static void run_command(char* cmd, char args[][5], int count);
static void run_command(char* cmd, char args[][5], int count) {
    pid_t pid = fork();

    if (pid < 0) {
        err(1, "fork error");
    } else if (pid == 0) {
        char* argv[4];
        int k = 0;
        argv[k++] = cmd;
        for (int i = 0; i < count; i++) {
            argv[k++] = args[i];
        }
        argv[k] = NULL;
        
        execvp(cmd, argv);
        err(1, "bad execling");
     
    }
    
    wait(NULL);
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        errx(1, "bad args");
    }

    char args[2][5];
    int count = 0;

    char echo[] = "echo";
    char* cmd = (argc == 2) ? argv[1] : echo;

    char word[5];
    int len = 0;
    uint8_t b;
    while (read(0, &b, sizeof(b)) == (ssize_t)sizeof(b)) {
        if (b == 0x20 || b == 0x0A) {
            if (len > 0) {
                word[len] = '\0';
                len = 0;
                
                strcpy(args[count++], word);

                if (count == 2) {
                    run_command(cmd, args, count);
                    count = 0;
                }
            }
        } else {
            if (len >= 4) {
                errx(1, "too long word");
            }

            word[len++] = b;
        }
    }

    if (len > 0) {
        word[len] = '\0';
        strcpy(args[count++], word);
        if (count == 2) { run_command(cmd, args, count); count = 0; }
    }

    if (count == 1) {
        run_command(cmd, args, 1);
    }

    return 0;
}
