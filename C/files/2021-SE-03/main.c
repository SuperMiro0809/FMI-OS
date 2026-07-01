#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

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

    char lines[] = "#ifndef ARR_H\n#define ARR_H\n\n#include <stdint.h>\n\nconst uint16_t arr[] = {";

    if (write(output, lines, sizeof(lines) - 1) != sizeof(lines) - 1) {
        err(1, "write failed");
    }

    uint16_t num;
    uint32_t arrN = 0;

    while (read(input, &num, sizeof(num)) == (ssize_t)sizeof(num)) {
        arrN++;
        
        char buf[100];
        int j = snprintf(buf, sizeof(buf), " %u,", num);
        if (write(output, buf, j) != j) {
            err(1, "write failed");
        }
    }
    
    if (arrN > 0) {
        if (lseek(output, -1, SEEK_CUR) == -1) {
            err(1, "seek failed");
        }
    }
    
    char buf[100];
    int j = snprintf(buf, sizeof(buf), " };\nconst uint32_t arrN = %u;\n\n#endif\n", arrN);
    if (write(output, buf, j) != j) {
        err(1, "write failed");
    }
    
    close(input);
    close(output);

    return 0;
}
