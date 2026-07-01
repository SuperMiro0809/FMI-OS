#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint32_t uid;
    uint16_t saved1;
    uint16_t saved2;
    uint32_t from;
    uint32_t end;
} person;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        errx(1, "bad args");
    }

    int file = open(argv[1], O_RDONLY);
    if (file < 0) {
        err(1, "file open failed");
    }

    int n = 0;
    uint64_t sum = 0;
    person p;
    while (read(file, &p, sizeof(p)) == sizeof(p)) {
        n++;
        sum += (uint32_t)(p.end - p.from);
    }

    double sr = (double)sum / n;

    if (lseek(file, 0, SEEK_SET) == -1) {
        err(1, "seek failed");
    }
    
    double sum1 = 0;
    while (read(file, &p, sizeof(p)) == sizeof(p)) {
        sum1 += ((p.end - p.from) - sr) * ((p.end - p.from) - sr);
    }

    double dis = sum1 / n;

    if (lseek(file, 0, SEEK_SET) == -1) {
        err(1, "seek failed");
    }
    
    uint32_t uids[2048] = {0};
    uint32_t max_dur[2048] = {0};
    int count = 0;

    while (read(file, &p, sizeof(p)) == sizeof(p)) {
        uint32_t dur = p.end - p.from;

        int idx = -1;
        for (int i = 0; i < count; i++) {
            if (uids[i] == p.uid) {
                idx = i;
                break;
            }
        }

        if (idx == -1) {
            idx = count++;
            uids[idx] = p.uid;
            max_dur[idx] = dur;
        }

        if (dur > max_dur[idx]) {
            max_dur[idx] = dur;
        }
    }

    for (int i = 0; i < count; i++) {
        if ((double)max_dur[i] * max_dur[i] > dis) {
            char buf[100];
            int a = snprintf(buf, sizeof(buf), "uid: %u, time: %u\n", uids[i], max_dur[i]);
            write(1, buf, a);
        }
    }

    close(file);
    
    return 0;
}
