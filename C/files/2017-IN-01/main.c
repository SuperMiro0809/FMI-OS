#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
    if (argc != 5) {
        errx(1, "bad args");
    }

    int iDat = open(argv[1], O_RDONLY);
    if (iDat < 0) {
        err(1, "file open failed");
    }

    int iIdx = open(argv[2], O_RDONLY);
    if (iIdx < 0) {
        err(1, "file open failed");
    }

    int oDat = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (oDat < 0) {
        err(1, "file open failed");
    }

    int oIdx = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (oIdx < 0) {
        err(1, "file open failed");
    }

    uint8_t byte, length, reserved;
    uint16_t offset;
    uint16_t out_offset = 0;

    while (read(iIdx, &offset, sizeof(offset)) == sizeof(offset) && read(iIdx, &length, sizeof(length)) == sizeof(length) && read(iIdx, &reserved, sizeof(reserved)) == sizeof(reserved)) {
        if (lseek(iDat, offset, SEEK_SET) == -1) {
            err(1, "seek failed");
        }

        if (read(iDat, &byte, sizeof(byte)) != sizeof(byte)) {
            err(1, "read failed");
        }

        if (byte >= 0x41 && byte <= 0x5A) {
            write(oDat, &byte, sizeof(byte));

            for (uint8_t i = 1; i < length; i++) {
                if (read(iDat, &byte, sizeof(byte)) != sizeof(byte)) {
                    err(1, "read failed");
                }
            
                write(oDat, &byte, sizeof(byte));
            }

            write(oIdx, &out_offset, sizeof(offset));
            write(oIdx, &length, sizeof(length));
            write(oIdx, &reserved, sizeof(reserved));

            out_offset += length;
        }
    }

    close(iDat);
    close(iIdx);
    close(oDat);
    close(oIdx);

    return 0;
}
