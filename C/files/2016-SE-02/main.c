#include <stdint.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>
 
int main(void) {
     uint32_t x, y, num;
     
     int f1 = open("f1.bin", O_RDONLY);
     if (f1 < 0) {
        err(1, "file open faield");
     }
 
     int f2 = open("f2.bin", O_RDONLY);
     if (f2 < 0) {
         err(1, "file open failed");
     }
 
     int f3 = open("f3.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);
     if (f3 < 0) {
         err(1, "file open failed");
     }
 
     while (read(f1, &x, sizeof(x)) == sizeof(x) && read(f1, &y, sizeof(y)) == sizeof(y)) {
         if (lseek(f2, x * sizeof(x), SEEK_SET) == -1) {
             err(1, "seek failed");
         }
 
         for (uint32_t i = 1; i <= y; i++) {
            if (read(f2, &num, sizeof(num)) != sizeof(num)) {
                err(1, "read failed");
            }
 
            if (write(f3, &num, sizeof(num)) == -1) {
                err(1, "write failed");
            }
         }
     }
 
     close(f1);
     close(f2);
     close(f3);

     return 0;
}

