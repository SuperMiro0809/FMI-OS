#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint32_t magic;
    uint8_t hVersion;
    uint8_t dVersion;
    uint16_t count;
    uint32_t reserved1;
    uint32_t reserved2;
} header;

typedef struct {
    uint16_t offset;
    uint8_t oByte;
    uint8_t nByte;
} data0;

typedef struct {
    uint32_t offset;
    uint16_t oWord;
    uint16_t nWord;
} data1;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        errx(1, "bad args");
    }
    
    int patch = open(argv[1], O_RDONLY);
    if (patch < 0) {
        err(1, "file open failed");
    }

    int f1 = open(argv[2], O_RDONLY);
    if (f1 < 0) {
        err(1, "file open failed");
    }

    int f2 = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (f2 < 0) {
        err(1, "file open failed");
    }
    
    header h;
    if (read(patch, &h, sizeof(h)) != sizeof(h)) {
        err(1, "read failed");
    }

    if (h.magic != 0xEFBEADDE) {
        errx(1, "wrong specification");
    }

    if (h.hVersion != 0x01) {
        errx(1, "unsupported header version");
    }

    if (h.dVersion == 0x00) {
        uint8_t buf[4096];
        ssize_t n;
        
        while ((n = read(f1, buf, sizeof(buf))) > 0) {
            if (write(f2, buf, n) != n) {
                err(1, "write failed");
            }
        }

        if (n < 0) {
            err(1, "read failed");
        }
        
        data0 d;
        uint8_t b;
        for (unsigned i = 0; i < h.count; i++) {
            if (read(patch, &d, sizeof(d)) != sizeof(d)) {    
                errx(1, "patch data section is truncated");
            }

            if (lseek(f1, d.offset * sizeof(uint8_t), SEEK_SET) == -1) {
                err(1, "seek failed");
            }
    
            if (read(f1, &b, sizeof(b)) != sizeof(b)) {
                errx(1, "read failed, offset does not exist");
            }

            if (b != d.oByte) {
                errx(1, "original byte is incorrect");
            }

            if (lseek(f2, d.offset * sizeof(uint8_t), SEEK_SET) == -1) {
                err(1, "seek failed");
            }

            if (write(f2, &d.nByte, sizeof(d.nByte)) != sizeof(d.nByte)) {
                err(1, "write failed");
            }
        }
    } else if (h.dVersion == 0x01) {
        uint16_t buf[4096];
        ssize_t n;

        while ((n = read(f1, buf, sizeof(buf))) > 0) {
            if (write(f2, buf, n) != n) {
                err(1, "write failed");
            }
        }

        if (n < 0) {
            err(1, "read failed");
        }

        data1 d;
        uint16_t w;
        for (unsigned i = 0; i < h.count; i++) {
            if (read(patch, &d, sizeof(d)) != sizeof(d)) {
                errx(1, "patch data section is truncated");
            }

            if (lseek(f1, d.offset * sizeof(uint16_t), SEEK_SET) == -1) {
                err(1, "seek failed");
            }

            if (read(f1, &w, sizeof(w)) != sizeof(w)) {
                errx(1, "read failed, offset does not exist");
            }

            if (w != d.oWord) {
                errx(1, "original word is incorrect");
            }

            if (lseek(f2, d.offset * sizeof(uint16_t), SEEK_SET) == -1) {
                err(1, "seek failed");
            }

            if (write(f2, &d.nWord, sizeof(d.nWord)) != sizeof(d.nWord)) {
                err(1, "write failed");
            }
        }
    } else {
        errx(1, "unsupported data version");
    }
    
    uint8_t extra;                                                
    if (read(patch, &extra, sizeof(extra)) != 0) {                
        errx(1, "patch has data beyond declared count");
    }

    close(patch);
    close(f1);
    close(f2);

    return 0;
}
