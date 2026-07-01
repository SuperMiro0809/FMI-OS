#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "bad args");
    }

    int input = open(argv[1], O_RDONLY);
    if (input < 0) {
        err(1, "file open failed");
    }

    int output = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (output < 0) {
        err(1, "file open failed");
    }
    
    struct stat info;
    fstat(input, &info);
    
    if (info.st_size % sizeof(uint32_t) != 0) {
        errx(1, "error");
    }

    int n = info.st_size / sizeof(uint32_t);
    
    uint32_t num, minNum;
    while (read(input, &num, sizeof(num)) == sizeof(num)) {
        if (write(output, &num, sizeof(num)) != sizeof(num)) {
            err(1, "write failed");
        }
    }

    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        
        for (int j = i + 1; j < n; j++) {
            lseek(output, min_idx * sizeof(uint32_t), SEEK_SET);
            read(output, &minNum, sizeof(minNum));

            lseek(output, j * sizeof(uint32_t), SEEK_SET);
            read(output, &num, sizeof(num));

            if (num < minNum) {
                min_idx = j;
            }
        }

        lseek(output, i * sizeof(uint32_t), SEEK_SET);
        read(output, &num, sizeof(num));

        lseek(output, i * sizeof(uint32_t), SEEK_SET); 
        write(output, &minNum, sizeof(minNum));

        lseek(output, min_idx * sizeof(uint32_t), SEEK_SET);
        write(output, &num, sizeof(num));
    }
    
    close(input);
    close(output);

    return 0;
}
