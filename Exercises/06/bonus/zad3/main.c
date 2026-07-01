#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        errx(1, "bad args");
    }
    
    char* dest_dir = argv[argc - 1];
    char buf[4096];
    ssize_t n;

    for (int i = 1; i < argc - 1; i++) {
        char* filename = strrchr(argv[i], '/');
        if (filename == NULL) {
            filename = argv[i];
        } else {
            filename++;
        }

        char dest_path[4096];
        snprintf(dest_path, sizeof(dest_path), "%s/%s", dest_dir, filename);

        int src = open(argv[i], O_RDONLY);
        if (src < 0) {
            err(1, "file open failed");
        }

        int dst = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (dst < 0) {
            err(1, "file open failed");
        }

        while((n = read(src, buf, sizeof(buf))) > 0) {
            if (write(dst, buf, n) != n) {
                err(1, "write failed");
            }
        }

        if (n < 0) {
            err(1, "read failed");
        }

        close(src);
        close(dst);
    }

    return 0;
}
