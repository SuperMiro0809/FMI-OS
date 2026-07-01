#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc == 1) {
        errx(1, "bad args");
    }

    char newLine = '\n';

    if (strcmp(argv[1], "-c") == 0) {
        if (!argv[2]) {
            errx(1, "missing another argument");
        }

        if (strlen(argv[2]) == 1) {
            char* end;
            int n = strtol(argv[2], &end, 10);
            
            if (*end != '\0') {
                errx(1, "not a number"); 
            }

            char c;
            int count = 0;

            while (read(0, &c, sizeof(c)) == sizeof(c)) {
                count++;

                if (count == n) {
                    write(1, &c, 1);
                    write(1, &newLine, 1);
                }

                if (c == '\n') {
                    count = 0;
                }
            }
        } else if (strlen(argv[2]) == 3) {
            if (argv[2][0] < '0' || argv[2][0] > '9') {
                errx(1, "error");
            }

            if (argv[2][2] < '0' || argv[2][2] > '9') {
                errx(1, "error");
            }
            
            int n = argv[2][0] - '0';
            int m = argv[2][2] - '0';
            
            char c;
            int count = 0;

            while (read(0, &c, sizeof(c)) == sizeof(c)) {
                count++;

                if (count >= n && count <= m && c != '\n') {
                    write(1, &c, 1);
                }

                if (c == '\n') {
                    count = 0;
                    write(1, &newLine, 1);
                }
            }
        } else {
            errx(1, "invalid format");
        }

    } else if (strcmp(argv[1], "-d") == 0) {
        if (argc != 5) {
            errx(1, "bad args");
        }

        if (strcmp(argv[3], "-f") != 0) {
            errx(1, "wrong argument");           
        }

        char delim = argv[2][0];

        if (strlen(argv[4]) == 1) {
            char* end;
            int n = strtol(argv[4], &end, 10);
 
            if (*end != '\0') {
                errx(1, "not a number");
            }

            char c;
            int count = 1;

            while (read(0, &c, sizeof(c)) == sizeof(c)) {
                if (c == delim) {
                    count++;
                    continue;
                }

                if (c == '\n') {
                    count = 1;
                    write(1, &newLine, 1);
                }
                
                if (count == n) {
                    write(1, &c, 1);
                }
            }
        } else if (strlen(argv[4]) == 3) {
            if (argv[4][0] < '0' || argv[4][0] > '9') {
                 errx(1, "error");
             }
 
             if (argv[4][2] < '0' || argv[4][2] > '9') {
                 errx(1, "error");
             }
 
             int n = argv[4][0] - '0';
             int m = argv[4][2] - '0';
 
             char c;
             int count = 1;
 
             while (read(0, &c, sizeof(c)) == sizeof(c)) {
                 if (c == delim) {
                    count++;

                    if (count > n && count <= m) {
                        write(1, &delim, 1);
                    }
                    continue;
                 }
 
                 if (c == '\n') {
                     count = 1;
                     write(1, &newLine, 1);
                 }
 
                 if (count >= n && count <= m) {
                     write(1, &c, 1);
                 }
            }
        } else {
            errx(1, "invalid format");
        }
    }

    return 0;
}
