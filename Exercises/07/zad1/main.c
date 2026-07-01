#include <unistd.h>
#include<err.h>

int main(void) {
    if (execl("/bin/date", "date", (char*)NULL) == -1) {
        err(1, "err execling");
    }


    return 0;
}
