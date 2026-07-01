#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>

typedef struct {
    uint32_t magic;
    uint32_t count;
} dataHeader;

typedef struct {
    uint32_t magic1;
    uint16_t magic2;
    uint16_t reserved;
    uint64_t count;
} comparatorHeader;

typedef struct {
    uint16_t type;
    uint16_t reserved1;
    uint16_t reserved2;
    uint16_t reserved3;
    uint32_t offset1;
    uint32_t offset2;
} comparatorData;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "bad args");
    }

    int data = open(argv[1], O_RDWR);
    if (data < 0) {
        err(1, "file open failed");
    }

    int comparator = open(argv[2], O_RDONLY);
    if (comparator < 0) {
        err(1, "file open failed");
    }
    
    dataHeader dh;
    comparatorHeader ch;

    if (read(data, &dh, sizeof(dh)) != (ssize_t)sizeof(dh)) {
        err(1, "read failed");
    }

    if (read(comparator, &ch, sizeof(ch)) != (ssize_t)sizeof(ch)) {
        err(1, "read failed");
    }

    if (dh.magic != 0x21796F4A) {
        errx(1, "wrong magic");
    }

    if (ch.magic1 != 0xAFBC7A37 || ch.magic2 != 0x1C27) {
        errx(1, "wrong magic");
    }
    
    comparatorData cData;
    uint64_t num1, num2;
    for (uint64_t i = 0; i < ch.count; i++) {
        if (read(comparator, &cData, sizeof(cData)) != (ssize_t)sizeof(cData)) {
            err(1, "read failed");
        }

        if (cData.offset1 >= dh.count || cData.offset2 >= dh.count) {
            errx(1, "incorrect offset");
        }
        
        if (lseek(data, sizeof(dataHeader) + cData.offset1 * sizeof(uint64_t), SEEK_SET) == -1) {
            err(1, "seek failed");
        }

        if (read(data, &num1, sizeof(num1)) != (ssize_t)sizeof(num1)) {
            err(1, "read failed");
        }

        if (lseek(data, sizeof(dataHeader) + cData.offset2 * sizeof(uint64_t), SEEK_SET) == -1) {
            err(1, "seek failed");
        }

        if (read(data, &num2, sizeof(num2)) != (ssize_t)sizeof(num2)) {
            err(1, "read failed"); 
        }

        if (num1 == num2) continue;

        if ((cData.type == 0 && num1 > num2) || (cData.type == 1 && num1 < num2)) {
            if (lseek(data, sizeof(dataHeader) + cData.offset1 * sizeof(uint64_t), SEEK_SET) == -1) {
                err(1, "seek failed");
            }

            if (write(data, &num2, sizeof(num2)) != (ssize_t)sizeof(num2)) {
                err(1, "write failed");
            }

            if (lseek(data, sizeof(dataHeader) + cData.offset2 * sizeof(uint64_t), SEEK_SET) == -1) {
                err(1, "seek failed");
            }

            if (write(data, &num1, sizeof(num1)) != (ssize_t)sizeof(num1)) {
                err(1, "write failed");
            }
        }
    }

    close(data);
    close(comparator);

    return 0;
}
