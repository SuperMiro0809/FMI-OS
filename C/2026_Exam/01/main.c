#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>

typedef struct {
	uint32_t p;
	uint32_t n;
} meta;

int main(int argc, char* argv[]) {
	if (argc != 3) {
		errx(1, "bad args");
	}

	int input = open(argv[1], O_RDONLY);
	if (input < 0) {
		err(1, "file open failed");
	}

	int output = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output < 0) {
		err(1, "file open failed");
	}

	meta d;
	while(read(input, &d, sizeof(d)) == (ssize_t)sizeof(d)) {
		uint8_t num;
		
		if (lseek(output, d.p * sizeof(uint8_t), SEEK_SET) == -1) {
			err(1, "seek failed");
		}

		for (uint32_t i = 0; i < d.n; i++) {
			if (read(input, &num, sizeof(num)) == -1) {
				err(1, "read failed");
			}

			if (write(output, &num, sizeof(num)) != (ssize_t)sizeof(num)) {
				err(1, "write failed");
			}
		}
	}

	close(input);
	close(output);

	return 42;
}
