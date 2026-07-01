#include <unistd.h>
#include <err.h>

int main(void) {
    int n = 1000000;
    pid_t pid = fork();
    
    if (pid == -1) {
        err(1, "err");
    } else if (pid == 0) {
        for (int i = 0; i < n; i++) {
			write(1,"C", 1);
		}
    } else {
        for (int i = 0; i < n; i++) {
            write(1, "P", 1);
        }
    }

    write(1, "\n", 1);

    return 0;
}
