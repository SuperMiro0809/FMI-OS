#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        err(1, "bad args");
    }
    
    int dic = open(argv[2], O_RDONLY);
    if (dic < 0) {
        err(1, "file open failed");
    }

    struct stat info;
    fstat(dic, &info);

    int l = 0;
    int r = info.st_size;

    while (l < r) {
        int mid = l + (r - l) / 2;

        if (lseek(dic, mid, SEEK_SET) == -1) {
            err(1, "seek failed");
        }

        char b;
        int start = 0;
        while (read(dic, &b, sizeof(b)) == (ssize_t)sizeof(b)) {
            if (b == 0x00) {
                start = lseek(dic, 0, SEEK_CUR) - 1;
                break;
            }

            if (lseek(dic, -2, SEEK_CUR) == -1) {
                err(1, "seek failed");
            }
        }

        char buf[64];
        int i = 0;
        while (read(dic, &b, sizeof(b)) == (ssize_t)sizeof(b)) {
            if (b == '\n') break;

            buf[i++] = b;
        }
        buf[i] = '\0';

        if (strcmp(buf, argv[1]) == 0) {
            while(read(dic, &b, sizeof(b)) == (ssize_t)sizeof(b)) {
                if (b == 0x00) break;
                write(1, &b, 1);
            }
            
            char newLine = '\n';
            write(1, &newLine, 1);
            break;
        } else if (strcmp(buf, argv[1]) < 0) {
            while (read(dic, &b, 1) == 1) {
                if (b == 0x00) break;
            }
            l = lseek(dic, 0, SEEK_CUR);
        } else {
            r = start;
        }
    }

    if (l >= r) {
        write(2, "word not found\n", 15);
    }

    close(dic);

    return 0;
}
