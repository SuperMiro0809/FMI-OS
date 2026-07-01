#include <unistd.h>
#include <err.h>

int main(void) {
    if (execlp("/bin/sleep", "sleep", "60", (char*)NULL) == -1) {
        err(1, "err execling");
    }
    return 0;
}
