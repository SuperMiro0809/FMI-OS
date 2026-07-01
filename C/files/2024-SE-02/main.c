#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>

typedef struct {
    uint32_t magic;
    uint32_t packet_count;
    uint64_t original_size;
} header;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "bad args");
    }
    
    int input = open(argv[1], O_RDONLY);
    if (input < 0) {
        err(1, "file open failed");
    }

    int output = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output < 0) {
        err(1, "file open failed");
    }

    header h;
    int r = read(input, &h, sizeof(h));
    if (r == -1) {
        err(1, "read failed");
    }
    if (r != (ssize_t)sizeof(h)) {
        errx(1, "wrong size");
    }

    if (h.magic != 0x21494D46) {
        errx(1, "wrong magic");
    }
    
    uint8_t b;
    for (uint32_t i = 0; i < h.packet_count; i++) {
        r = read(input, &b, sizeof(b));
        if (r == -1) {
            err(1, "read failed");
        }
        if (r != sizeof(b)) {
            errx(1, "wrong size");
        }

        uint8_t type = (b >> 7) & 1;
        uint8_t n = b & 0x7F;

        if (type == 0) {
            for (uint8_t j = 0; j <= n; j++) {
                r = read(input, &b, sizeof(b));
                if (r == -1) {
                    err(1, "read failed");
                }
                if (write(output, &b, sizeof(b)) != (ssize_t)sizeof(b)) {
                    err(1, "write failed");
                }
            }
        } else {
            r = read(input, &b, sizeof(b));
            if (r == -1) {
                err(1, "read failed");
            } 
            for (uint8_t j = 0; j <= n; j++) {
                if (write(output, &b, sizeof(b)) != (ssize_t)sizeof(b)) {
                    err(1, "write failed");
                }
            }
        }
    }

    close(input);
    close(output);

    return 0;
}
