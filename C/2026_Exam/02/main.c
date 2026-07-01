#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "bad args");
	}

	int fd[2];
	if (pipe(fd) == -1) {
		err(1, "pipe");
	}
	
	pid_t pid = fork();

	if (pid < 0) {
		err(1, "fork");
	} else if (pid == 0) {
		// child
		close(fd[0]);
		
		dup2(fd[1], 1);
		close(fd[1]);

		execlp("tar", "tar", "cv", argv[1], (char*)NULL);
		err(1, "execlp failed");
	}
	
	// parent
	close(fd[1]);

	uint8_t r = 0;
	uint8_t b;
	while (read(fd[0], &b, sizeof(b)) == (ssize_t)sizeof(b)) {
		r ^= b;
	}

	close(fd[0]);
	
	char buf[64];
	int l = snprintf(buf, sizeof(buf), "%x\n", r);
	if (write(1, buf, l) != l) {
		err(1, "write failed");
	}

	wait(NULL);

	return 42;
}
