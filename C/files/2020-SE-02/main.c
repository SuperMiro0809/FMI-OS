#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        errx(1, "usage: %s input.scl input.sdl output", argv[0]);
    }

    int scl = open(argv[1], O_RDONLY);
    if (scl < 0) {
        err(1, "file open failed");
    }

    int sdl = open(argv[2], O_RDONLY);
    if (sdl < 0) {
        err(1, "file open failed");
    }

    int output = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output < 0) {
        err(1, "file open failed");
    }

    uint16_t val;
    uint8_t bits = 0;
    unsigned i = 0;
    
    while (read(sdl, &val, sizeof(val)) == (ssize_t)sizeof(val)) {
        if (i % 8 == 0) {
            ssize_t r = read(scl, &bits, sizeof(bits));
            if (r == -1) {
                err(1, "read failed");
            }
            if (r != sizeof(bits)) { errx(1, "scl has fewer bits than sdl elements"); }
        }

        uint8_t bit = (bits >> (7 - i % 8)) & 1;

        if (bit) {
            if (write(output, &val, sizeof(val)) != (ssize_t)sizeof(val)) {
                err(1, "write failed");
            }
        }
        i++;
    }

    close(scl);
    close(sdl);
    close(output);

    return 0;
}
