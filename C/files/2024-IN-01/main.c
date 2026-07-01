#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint16_t magic;
    uint16_t ver;
    uint16_t cp;
    uint16_t co;
} header;

typedef struct {
    uint16_t v1;
    uint16_t v2;
    uint32_t v3;
} preamble;

typedef struct {
    uint32_t ctime;
    uint16_t opt;
    uint16_t parent_id;
    uint32_t size;
    uint32_t ssize;
} object;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        errx(1, "bad args");
    }

    int file = open(argv[1], O_RDONLY);
    if (file < 0) {
        err(1, "file open failed");
    }

    header h;
    ssize_t r = read(file, &h, sizeof(h));
    if (r == -1) {
        err(1, "read failed");
    }
    if (r != (ssize_t)sizeof(h)) {
        errx(1, "header is wrong size");
    }

    if (h.magic != 0x6963) {
        errx(1, "wrong magic");
    }
    if (h.ver != 0x6e73) {
        errx(1, "wrong ver");
    }
    
    preamble p;
    for (uint16_t i = 0; i < h.cp; i++) {
        r = read(file, &p, sizeof(p));
        if (r == -1) {
            err(1, "read failed");
        }
        if (r != (ssize_t)sizeof(p)) {
            errx(1, "preamble is wrong size");
        }
    }

    object data, parent;
    long start = lseek(file, 0, SEEK_CUR);
    
    uint64_t size_sum = 0;
    uint64_t koef_sum = 0;

    for (uint16_t i = 0; i < h.co; i++) {
        r = read(file, &data, sizeof(data));
        if (r == -1) {
            err(1, "read file");
        }
        if (r != (ssize_t)sizeof(data)) {
            errx(1, "object is wrong size");
        }
        
        if (data.parent_id == 0) {
            continue;
        }

        uint16_t kind = (data.opt >> 14) & 0x3;
        if (kind != 2) continue;

        long parent_pos = start + (long)data.parent_id * (long)sizeof(object);
        long cur = lseek(file, 0, SEEK_CUR);

        if (lseek(file, parent_pos, SEEK_SET) == -1) {
            err(1, "seek failed");
        }

        r = read(file, &parent, sizeof(parent));
        if (r == -1) {
            err(1, "read failed");
        }
        if (r != sizeof(parent)) {
            errx(1, "object is wrong size");
        }

        if (lseek(file, cur, SEEK_SET) == -1) {
            err(1, "seek failed");
        }

        uint32_t diff = data.ctime > parent.ctime
                      ? data.ctime - parent.ctime
                      : parent.ctime - data.ctime;

        if (diff >= 85800 && diff <= 87000) {
            koef_sum += data.ssize; // data.ssize / data.size * data.size
            size_sum += data.size;
        }
    }

    close(file);
    
    double res = (double)koef_sum / (double)size_sum;
    char buf[1000];
    int l = snprintf(buf, sizeof(buf), "%f\n", res);
    
    write(1, buf, l);

    return 0;
}
