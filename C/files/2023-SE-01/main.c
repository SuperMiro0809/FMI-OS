#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>

int main (int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "bad args");
    }
    
    int stream = open(argv[1], O_RDONLY);
    if (stream < 0) {
        err(1, "file open failed");
    }

    int messages = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (messages < 0) {
        err(1, "file open failed");
    }
    
    uint8_t msg[255];
    off_t pos = 0;
    
    while (1) {
        if (lseek(stream, pos, SEEK_SET) == -1) {
            err(1, "seek failed");
        }

        uint8_t hdr[2];
        ssize_t r = read(stream, hdr, sizeof(hdr));
        if (r == -1) {
            err(1, "read failed");
        }
        if (r != sizeof(hdr)) {
            break;
        }

        if (hdr[0] != 0x55) {
            pos++;
            continue;
        }

        uint8_t n = hdr[1];
        if (n < 3) {
            pos++;
            continue;
        }

        if (lseek(stream, pos, SEEK_SET) == -1) {
           err(1, "seek failed");
        }

        r = read(stream, msg, n);
        if (r == -1) {
            err(1, "read failed");
        }
        if (r != n) {
            pos++;
            continue;
        }

        uint8_t ch = 0;
        for (uint8_t k = 0; k < n - 1; k++) {
            ch ^= msg[k];
        }

        if (ch == msg[n - 1]) {
            if (write(messages, msg, n) != (ssize_t)n) {
                err(1, "write failed");
            }
            pos+=n;
        } else {
            pos++;
        }
    
    }

    close(stream);
    close(messages);

    return 0;
}
