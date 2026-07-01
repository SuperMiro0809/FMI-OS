#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#define U 16

typedef struct {
    uint64_t magic;
    uint32_t cfsb;
    uint32_t cfsu;
    uint32_t ofsb;
    uint32_t ofsu;
    uint32_t unused1;
    uint32_t cksum;
    uint8_t sectionkey[16];
    uint32_t s0, s1, s2, s3;
} header;

typedef struct {
    int64_t relative_offset;
    uint64_t len;
    uint8_t  datakey[16];
} section;

static void read_unit(int fd, uint64_t idx, uint8_t *out) {
    if (lseek(fd, (off_t)idx * U, SEEK_SET) == -1) {
        err(1, "seek failed");
    }
    ssize_t r = read(fd, out, U);
    if (r == -1) { err(1, "read"); }
    if (r != U) { errx(1, "encrypted file truncated at unit %" PRIu64, idx); }
}

static void xor_key(uint8_t *buf, const uint8_t *key) {
    for (int i = 0; i < U; i++) {
        buf[i] ^= key[i];
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "bad args");
    }
    
    int in = open(argv[1], O_RDONLY);
    if (in < 0) {
        err(1, "file open failed");
    }

    int out = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (out < 0) {
        err(1, "file open failed");
    }

    header h;
    ssize_t r = read(in, &h, sizeof(h));
    if (r == -1) {
        err(1, "read failed");
    }
    if (r != (ssize_t)sizeof(h)) {
        errx(1, "header too short");
    }
    if (h.magic != 0x0000534f44614c47ULL) {
        errx(1, "wrong magic");
    }

    uint32_t slots[4] = { h.s0, h.s1, h.s2, h.s3 };

    uint64_t written = 0;
    int done = 0;

    for (int si = 0; si < 4 && !done; si++) {
        uint32_t sp = slots[si];
        if (sp == 0) { continue; }

        uint8_t secbuf[2 * U];
        read_unit(in, sp,     secbuf);
        read_unit(in, sp + 1, secbuf + U);
        xor_key(secbuf,     h.sectionkey);
        xor_key(secbuf + U, h.sectionkey);

        section s;
        memcpy(&s, secbuf, sizeof(s));

        int64_t dp = (int64_t)sp + s.relative_offset;
        if (dp < 4) { errx(1, "section %d data overlaps header", si); }

        for (uint64_t j = 0; j < s.len; j++) {
            if (written >= h.ofsu) { done = 1; break; } 
            uint8_t unit[U];
            read_unit(in, (uint64_t)dp + j, unit);
            xor_key(unit, s.datakey);
            if (write(out, unit, U) != U) { err(1, "write"); }
            written++;
        }
    }

    if (written < h.ofsu) {
        errx(1, "decoded %" PRIu64 " units but header says %" PRIu32, written, h.ofsu);
    }

    if (ftruncate(out, h.ofsb) == -1) { err(1, "ftruncate"); }

    if (lseek(out, 0, SEEK_SET) == -1) { err(1, "seek"); }
    uint32_t ck = 0;
    uint64_t remaining = h.ofsb;
    while (remaining > 0) {
        size_t take = remaining >= 4 ? 4 : (size_t)remaining;
        uint8_t eb[4] = {0};
        if (read(out, eb, take) != (ssize_t)take) { errx(1, "checksum read"); }
        uint32_t e;
        memcpy(&e, eb, 4);
        ck ^= e;
        remaining -= take;
    }
    if (ck != h.cksum) { errx(1, "checksum mismatch: file may be corrupt"); }

    close(in);
    close(out);


    return 0;
}
