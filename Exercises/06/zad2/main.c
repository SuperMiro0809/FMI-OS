#include <fcntl.h>
#include <unistd.h>
#include <err.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        err(1, "bad args");
    }
    
    int file = open(argv[1], O_RDONLY);
    if (file < 0) {
        err(1, "file open failed");
    }

    char c;
    int lines = 0;

    while (read(file, &c, sizeof(c)) == sizeof(c)) {
        if (c == '\n') {
            lines++;
        }

        write(1, &c, 1);

        if (lines == 10) {
            break;
        }
    }

    close(file);

    return 0;
}
