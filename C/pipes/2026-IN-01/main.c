#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <stdint.h>

int toProg[3][2];
int fromProg[3][2];

void sendByte(uint8_t b, int shouldWait);
void sendByte(uint8_t b, int shouldWait) {
    for (int i = 0; i < 3; i++) {
        if (write(toProg[i][1], &b, sizeof(b)) != 1) {
            err(1, "write");
        }
    }
    
    if (shouldWait == 1) {
    uint8_t ack = 0;
    for (int i = 0; i < 3; i++) {
        if (read(fromProg[i][0], &ack, sizeof(ack)) != 1) {
            errx(1, "no ack");
        }
    }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        errx(1, "bad args");
    }
    
    pid_t pids[3];
    
    for (int i = 0; i < 3; i++) {
        if (pipe(toProg[i]) == -1 || pipe(fromProg[i]) == -1) {
            err(1, "pipe");
        }
    }

    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            err(1, "fork");
        } else if (pid == 0) {
            dup2(toProg[i][0], 0);
            dup2(fromProg[i][1], 1);

            for (int j = 0; j < 3; j++) {
                close(toProg[j][0]); close(toProg[j][1]);
                close(fromProg[j][0]); close(fromProg[j][1]);
            }

            execl(argv[i + 1], argv[i + 1], (char*)NULL);
            _exit(127);
        }
        pids[i] = pid;
    }

    for (int i = 0; i < 3; i++) {
        close(toProg[i][0]);
        close(fromProg[i][1]);
    }

    uint8_t b;
    uint8_t prev = 0;
    uint8_t esc = 0x7D;
    uint8_t fake = 0x55;
    int havePrev = 0;
    while (read(0, &b, sizeof(b)) == (ssize_t)sizeof(b)) {
        if (b == 0x00 || b == 0x55 || b == 0x7D || b == 0xFF) {
            uint8_t r = b ^ 0x20;
            
            sendByte(esc, 0);
            sendByte(r, 1);
            havePrev = 0;
        } else {
            if (havePrev && prev == b) {
                sendByte(fake, 0);
            }
            sendByte(b, 1);
            prev = b;
            havePrev = 1;
        }
    }
    
    for (int i = 0; i < 3; i++) {
        close(toProg[i][1]);
        close(fromProg[i][0]);
    }

    for (int i = 0; i < 3; i++) {
        waitpid(pids[i], NULL, 0);
    }

    return 0;
}
