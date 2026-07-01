#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        errx(1, "bad args");
    }

    int index = open(argv[1], O_RDONLY);
    if (index < 0) {
        err(1, "file open failed");
    }

    int values = open(argv[2], O_RDONLY);
    if (values < 0) {
        err(1, "file open failed");
    }

    int count = 0;
    uint8_t byte;
    
    while (read(index, &byte, sizeof(byte)) == (ssize_t)sizeof(byte)) {
        uint8_t N = byte & 0x7F;
        uint8_t type = (byte >> 7) & 1;

        char buf[255];
        ssize_t r = read(index, buf, N);
        if (r == -1) {
            err(1, "read failed");
        }

        buf[N] = '\0';
        if (strcmp(argv[3], buf) == 0) {
            if (lseek(values, count * sizeof(float), SEEK_SET) == -1) {
                err(1, "seek failed");
            }

            if (type == 0) {
                uint32_t num;
                if (read(values, &num, sizeof(num)) == -1) {
                    err(1, "read failed");
                }

                char res[255];
                int l = snprintf(res, sizeof(res), "%d\n", num);
                write(1, res, l);
            } else {
                float num;
                if (read(values, &num, sizeof(num)) == -1) {
                    err(1, "reed failed");
                }

                char res[255];
                int l = snprintf(res, sizeof(res), "%.3f\n", num);
                write(1, res, l);

            }
            
            close(index);
            close(values);
            return 0;
        }

        count++;
    }

    errx(1, "word not found");

    return 0;
}
