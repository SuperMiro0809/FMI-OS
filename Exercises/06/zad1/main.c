#include <fcntl.h>
#include <err.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "bad args");
    }

    int input_file = open(argv[1], O_RDONLY);
    if (input_file < 0) {
        err(1, "input file error");
    }

    int output_file = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_file < 0) {
        err(1, "output file error");
    }

    char buf[4096];
    ssize_t n;

    while((n = read(input_file, buf, sizeof(buf))) > 0) {
        if (write(output_file, buf, n) != n) {
            err(1, "write failed");
        }
    }

    if (n < 0) {
        err(1, "read failed");
    }

    close(input_file);
    close(output_file);

    return 0;
}
