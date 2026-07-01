#include <unistd.h>
#include <err.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>

int main(void) {
    char prompt[] = "Enter command> "; 
    
    while (1) {
        write(1, prompt, strlen(prompt));
        
        char cmd[32];
        ssize_t n = read(0, cmd, sizeof(cmd) - 1); // '\n' is at the end so we remove it
        
        if (n == 0) break;
        if (n < 0) continue;    

        cmd[n - 1] = '\0';
        
        if (strlen(cmd) == 0) continue;

        if (strcmp(cmd, "exit") == 0) {
            break;
        }

        pid_t pid = fork();

        if (pid == 0) {
            char buf[64];
            snprintf(buf, sizeof(buf), "/bin/%s", cmd);

            execl(buf, cmd, (char*)NULL);
            err(1, "bad execling");
        }

        int status;
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) break;
    }

    return 0;
}
