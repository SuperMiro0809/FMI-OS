#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>
#include <float.h>

typedef struct {
    uint32_t magic;
    uint32_t size;
    uint32_t files;
} header;

int main(int argc, char* argv[]) {
    if (argc < 4 || argc > 10) {
        errx(1, "bad args");
    }

    int res = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (res < 0) {
        err(1, "file open failed");
    }
    
    header h;
    h.magic = 0x0F0110F0;
    h.size = 0;
    h.files = argc - 2;
    
    struct stat info;
    for (int i = 2; i < argc; i++) {
        if (stat(argv[i], &info) == -1) {
            err(1, "stat failed");
        }
        uint32_t s = info.st_size / sizeof(float);
        if (s > h.size) {
            h.size = s;
        }
    }

    if (write(res, &h, sizeof(h)) != (ssize_t)sizeof(h)) {
        err(1, "write failed");
    }

    int fd[8];
    for (int i = 0; i < argc - 2; i++) {
        int f = open(argv[i + 2], O_RDONLY);
        if (f < 0) {
            err(1, "file open failed");
        }
        fd[i] = f;
    }
    
    for (uint32_t i = 0; i < h.size; i++) {
        float minN = FLT_MAX;
        float maxN = -FLT_MAX;

        uint8_t minI = 0;
        uint8_t maxI = 0;
        
        for (int j = 0; j < argc - 2; j++) {
            float num;
            if (lseek(fd[j], i * sizeof(float), SEEK_SET) == -1) {
                err(1, "seek failed");
            }
            
            ssize_t r = read(fd[j], &num, sizeof(num));
            if (r == -1) {
                err(1, "file failed");
            }
            if (r != (ssize_t)sizeof(num)) {
                continue;
            }
            
            if (num < minN) { 
                minN = num;
                minI = j;
            }
            if (num > maxN) {
                maxN = num;
                maxI = j;
            }
        }

        if (write(res, &minI, sizeof(minI)) != (ssize_t)sizeof(minI)) {
            err(1, "write failed");
        }
        if (write(res, &maxI, sizeof(maxI)) != (ssize_t)sizeof(maxI)) {
            err(1, "write failed");
        }
    }

    close(res);
    for (int i = 0; i < argc - 2; i++) {
        close(fd[i]);
    }

    return 0;
}
