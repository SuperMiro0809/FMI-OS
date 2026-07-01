#include <unistd.h>
#include <err.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        errx(1, "bad args");
    }

    if (execlp("/bin/ls", "ls", argv[1], (char*)NULL) == -1) {
        err(1, "err execling");
    }
    return 0;
}
