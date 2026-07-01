#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        errx(1, "bad args");
    }

    int file = open(argv[1], O_RDWR);
    if (file < 0) {
        err(1, "file open failed");
    }

    struct stat info;
    int result = fstat(file, &info);
    if (result < 0) {
        err(1, "could not stat");
    }
    
    if (info.st_size % sizeof(uint32_t) != 0) {
        errx(1, "incorrect");
    }

    int n = info.st_size / sizeof(uint32_t);
    
    uint32_t num1, num2;

    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;

        for (int j = i + 1; j < n; j++) {
            lseek(file, i * sizeof(uint32_t), SEEK_SET);
            read(file, &num1, sizeof(num1));

            lseek(file, j * sizeof(uint32_t), SEEK_SET);
            read(file, &num2, sizeof(num2));

            if (num2 < num1) {
                min_idx = j;
            }
        }

        lseek(file, min_idx * sizeof(uint32_t), SEEK_SET);
        read(file, &num1, sizeof(num1));

        lseek(file, i * sizeof(uint32_t), SEEK_SET);
        read(file, &num2, sizeof(num2));

        lseek(file, min_idx * sizeof(uint32_t), SEEK_SET);
        write(file, &num2, sizeof(num2));

        lseek(file, i * sizeof(uint32_t), SEEK_SET);
        write(file, &num1, sizeof(num1));
    }

    close(file);

    return 0;
}
