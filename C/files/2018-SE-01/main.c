#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc == 1 || argc > 4) {
        errx(1, "bad args");
    }
    
    char c;

    if (strcmp(argv[1], "-d") == 0) {
        if (!argv[2]) {
            errx(1, "SET1 is missing");
        }

        while (read(0, &c, sizeof(c)) == sizeof(c)) {
            int isInSet = 0;

            for (size_t i = 0; i < strlen(argv[2]); i++) {
                if (argv[2][i] == c) {
                    isInSet = 1;
                    break;
                }
            }

            if (isInSet == 0) {
                write(1, &c, 1);
            }
        }
    } else if (strcmp(argv[1], "-s") == 0) {
        if (!argv[2]) {
            errx(1, "SET1 is missing");
        }

        char tmp;
        int isInSet = 0;
        while (read(0, &c, sizeof(c)) == sizeof(c)) {
            if (isInSet == 1 && c != tmp) {
                write(1, &tmp, 1);
                isInSet = 0;
            }

            for (size_t i = 0; i < strlen(argv[2]); i++) {
                if (argv[2][i] == c) {
                    isInSet = 1;
                    tmp = c;
                    break;
                }
            }

            if (isInSet == 0) {
                write(1, &c, 1);
            }
        }

        if (isInSet == 1) {
            write(1, &tmp, 1);
        }
    } else {
        if (!argv[1]) {
            errx(1, "SET1 is missiong");
        }

        if (!argv[2]) {
            errx(1, "SET2 is missing");
        }
        
        while (read(0, &c, sizeof(c)) == sizeof(c)) {
            size_t pos = 0;
            int isInSet = 0;
            for (size_t i = 0; i < strlen(argv[1]); i++) {
                if (argv[1][i] == c) {
                    pos = i;
                    isInSet = 1;
                    break;
                }
            }

            if (isInSet == 1) {
                write(1, &argv[2][pos], 1);
            } else {
                write(1, &c, 1);
            }
        }
    }

    return 0;
}
