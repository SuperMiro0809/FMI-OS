#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "bad args");
    }

    uint16_t num;
    int file = open(argv[2], O_RDONLY);
    if (file < 0) {
        err(1, "file open failed");
    }
    
    if (strcmp(argv[1], "--min") == 0) {
        uint16_t minNum = UINT16_MAX;
        while (read(file, &num, sizeof(num)) == sizeof(num)) {
            if (num < minNum) {
                minNum = num;
            }
        }

        char buf[32];
        int len = snprintf(buf, sizeof(buf), "%u\n", minNum);
        write(1, buf, len);
    } else if (strcmp(argv[1], "--max") == 0) {
        uint16_t maxNum = 0;
        while (read(file, &num, sizeof(num)) == sizeof(num)) {
            if (num > maxNum) {
                maxNum = num;
            }
        }

        char buf[32];
        int len = snprintf(buf, sizeof(buf), "%u\n", maxNum);
        write(1, buf, len);
    } else if (strcmp(argv[1], "--print") == 0) {
        char buf[32];
        while (read(file, &num, sizeof(num)) == sizeof(num)) {
            int len = snprintf(buf, sizeof(buf), "%u\n", num);
            write(1, buf, len);
        }
    } else {
        errx(1, "unknown option: %s", argv[1]); 
    }
    
    close(file);

    return 0;
}
