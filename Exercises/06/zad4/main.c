#include <fcntl.h>
#include <unistd.h>
#include <err.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "bad args");
    }
    
    int file1 = open(argv[1], O_RDWR);
    if (file1 < 0) {
        err(1, "first file open failed");
    }

    int file2 = open(argv[2], O_RDWR);
    if (file2 < 0) {
        err(1, "second file open failed");
    }

    int temp_file = open("temp_file", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    if (temp_file < 0) {
        err(1, "temp file error");
    }

    char buf[4096];
    ssize_t n;

    while ((n = read(file1, buf, sizeof(buf))) > 0) {
        if (write(temp_file, buf, n) != n) {
            err(1, "write failed");
        }
    }

    if (n != 0) {
        err(1, "read error");
    }

    if (lseek(file1, 0, SEEK_SET) == -1) {
        err(1, "seeking failed");
    }
    if (lseek(file2, 0, SEEK_SET) == -1) {
        err(1, "seeking failed");
    }

    if (ftruncate(file1, 0) == -1) {
        err(1, "truncate failed");
    }
    while ((n = read(file2, buf, sizeof(buf))) > 0) {
        if (write(file1, buf, n) != n) {
            err(1, "write failed");
        }
    }

    if (lseek(file2, 0, SEEK_SET) == -1 || lseek(temp_file, 0, SEEK_SET) == -1) {
        err(1, "seeking failed");
    }
    
    if (ftruncate(file2, 0) == -1) {
        err(1, "truncate failed");
    }
    while ((n = read(temp_file, buf, sizeof(buf))) > 0) {
        if (write(file2, buf, n) != n) {
            err(1, "write failed");
        }
    }
    
    if (n != 0) {
        err(1, "read errror");
    }

    close(file1);
    close(file2);
    close(temp_file);

    return 0;
}
