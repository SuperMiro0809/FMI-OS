#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>

typedef struct {
    uint16_t magic;
    uint16_t filetype;
    uint32_t count;
} header;

static header file_header(int fd, int type) {
    header h;

    if (read(fd, &h, sizeof(h)) != (ssize_t)sizeof(h)) {
        err(1, "read failed");
    }

    if (h.magic != 0x5A4D) {
        errx(1, "wrong magic");
    }

    if (h.filetype != type) {
        errx(1, "wrong file type");
    }

    return h;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        errx(1, "bad args");
    }

    int list = open(argv[1], O_RDONLY);
    if (list < 0) {
        err(1, "file open failed");
    }

    int data = open(argv[2], O_RDONLY);
    if (data < 0) {
        err(1, "file open failed");
    }

    int out = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out < 0) {
        err(1, "file open failed");
    }

    header listH = file_header(list, 1);
    header dataH = file_header(data, 2);

    header outH;
    outH.magic = 0x5A4D;
    outH.filetype = 3;
    outH.count = 0;

    if (write(out, &outH, sizeof(outH)) != (ssize_t)sizeof(outH)) {
        err(1, "write failed");
    }

    uint16_t l;
    uint32_t d;
    uint32_t outCount = 0;

    for (uint32_t i = 0; i < listH.count; i++) {
        if (read(list, &l, sizeof(l)) != (ssize_t)sizeof(l)) {
            err(1, "read failed");
        }

        if (i >= dataH.count) {
            errx(1, "position is out of data's range");
        }

        if (lseek(data, sizeof(header) + i * sizeof(uint32_t), SEEK_SET) == -1) {
            err(1, "seek failed");
        }

        if (read(data, &d, sizeof(d)) != (ssize_t)sizeof(d)) {
            err(1, "read failed");
        }

        uint64_t v = d;

        if (lseek(out, sizeof(header) + l * sizeof(uint64_t), SEEK_SET) == -1) {
            err(1, "seek failed");
        }

        if (write(out, &v, sizeof(v)) != (ssize_t)sizeof(v)) {
            err(1, "write failed");
        }

        if((uint32_t)l + 1 > outCount) {
            outCount = (uint32_t)l + 1;
        }
    }

    if (lseek(out, 4, SEEK_SET) == -1) {
        err(1, "seek failed");
    }

    if (write(out, &outCount, sizeof(outCount)) != (ssize_t)sizeof(outCount)) {
        err(1, "write failed");
    }
    
    close(list);
    close(data);
    close(out);

    return 0;
}
