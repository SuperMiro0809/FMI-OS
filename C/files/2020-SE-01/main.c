#include <fcntl.h>
#include <err.h>
#include <unistd.h>

static uint16_t read_count(int fd, const char *name) {
    uint8_t hdr[16];
    if (read(fd, hdr, sizeof(hdr)) != (ssize_t)sizeof(hdr)) {
        errx(1, "%s: header is too short", name);
    }
    return (uint16_t)(hdr[4] | (hdr[5] << 8));   /* little-endian */
}

int main(int argc, char* argv[]) {
    if (argc != 7) {
        errx(1, "bad args");
    }

    int affix   = open(argv[1], O_RDONLY);
    int postfix = open(argv[2], O_RDONLY);
    int prefix  = open(argv[3], O_RDONLY);
    int infix   = open(argv[4], O_RDONLY);
    int suffix  = open(argv[5], O_RDONLY);
    int cruci   = open(argv[6], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (affix < 0 || postfix < 0 || prefix < 0 || infix < 0 || suffix < 0 || cruci < 0) {
        err(1, "open");
    }

    uint16_t affix_n   = read_count(affix,   "affix");
    uint16_t postfix_n = read_count(postfix, "postfix");
    uint16_t prefix_n  = read_count(prefix,  "prefix");
    uint16_t infix_n   = read_count(infix,   "infix");
    uint16_t suffix_n  = read_count(suffix,  "suffix");

    return 0;
}
