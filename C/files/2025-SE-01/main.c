#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>

static int read_elements(int file, uint64_t* id, uint8_t* n, char* text);

static int read_elements(int file, uint64_t* id, uint8_t* n, char* text) {
    int r = read(file, id, sizeof(*id));
    if (r == 0) {
        return 0;
    }

    if (r == -1) {
        close(file);
        err(1, "read failed");
    }
    if (r != (ssize_t)sizeof(*id)) {
        close(file);
        errx(1, "wrong size");
    }
 
    r = read(file, n, sizeof(*n));
    if (r == -1) {
        close(file);
        err(1, "read failed");
    }
    if (r != (ssize_t)sizeof(*n)) {
        close(file);
        errx(1, "wrong size");
    }

    r = read(file, text, *n);
    if (r == -1) {
        close(file);
        err(1, "read file");
    }
    if (r != (ssize_t)*n) {
        close(file);
        errx(1, "wrong size");
    }
    
    return 1;
}

typedef struct {
    int fd;
    int active;
    char role[256];
    uint8_t role_len;
    uint64_t time;
    uint8_t text_len;
    char text[256];
} role_file;

int main(int argc, char* argv[]) {
    if (argc > 21) {
        errx(1, "bad args");
    }

    role_file files[20];
    int nf = argc - 1;

    for (int i = 1; i < argc; i++) {
        int file = open(argv[i], O_RDONLY);
        if (file < 0) {
            err(1, "file open failed");
        }

        uint64_t id;
        uint8_t n;
        read_elements(file, &id, &n, files[i - 1].role);

        if (id != 133742) {
            close(file);
            errx(1, "wrong header id");
        }

        files[i - 1].fd = file;
        files[i - 1].role_len = n;
        files[i - 1].active = read_elements(file, &files[i - 1].time,
                                            &files[i - 1].text_len, files[i - 1].text);

    }

    while (1) {
        int best = -1;
        for (int i = 0; i < nf; i++) {
            if (!files[i].active) {
                continue;
            }
            if (best == -1 || files[i].time < files[best].time) {
                best = i;
            }
        }

        if (best == -1) {
            break;
        }

        write(1, files[best].role, files[best].role_len);
        write(1, ": ", 2);
        write(1, files[best].text, files[best].text_len);
        write(1, "\n", 1);

        files[best].active = read_elements(files[best].fd, &files[best].time,
                                           &files[best].text_len, files[best].text);
    }

    for (int i = 0; i < nf; i++) {
        close(files[i].fd);
    }

    return 0;
}
