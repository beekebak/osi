#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc) {
  char pagemap_path[256];
  int pid = getpid();
  snprintf(pagemap_path, sizeof(pagemap_path), "/proc/%d/pagemap", pid);

  int fd = open(pagemap_path, O_RDONLY);
  if (fd == -1) {
    perror("open");
    return 0;
  }

  unsigned char buffer[1024];
  int bytes_read;
  while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
    for (int i = 0; i < bytes_read; ++i) {
      printf("%02x ", buffer[i]);
    }
    printf("\n");
  }
  if (bytes_read == 0) {
    close(fd);
    return 0;
  }
  perror(strerror(errno));
  close(fd);
  return 1;
}
