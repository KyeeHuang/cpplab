#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>

bool start_daemon() {
  int fd;
  
  switch (fork()) {
    case -1:
      printf("fork() failed\n");
      return false;
    case 0:
      break;
    default:
      exit(0);
  }

  if (setsid() == -1) {
    printf("setsid() failed\n");
    return false;
  }

  switch (fork()) {
    case -1:
      printf("fork() failed\n");
      return false;
    
    case 0:
      break;
    
    default:
      exit(0);
  }

  umask(0);
  chdir("/");

  long maxfd;
  if ((maxfd = sysconf(_SC_OPEN_MAX)) != -1) {
    for (fd = 0; fd < maxfd; fd++) {
      close(fd);
    }
  }  

  fd = open("/dev/null", 0, O_RDWR);
  if (fd == -1) {
    printf("open(\"/dev/null\") failed\n");
    return false;
  }

  if (dup2(fd, STDIN_FILENO) == -1) {
    printf("dup2(STDIN) failed\n");
    return false;
  }

  if (dup2(fd, STDOUT_FILENO) == -1) {
    printf("dup2(STDOUT) failed\n");
    return false;
  }
  
  if (dup2(fd, STDERR_FILENO) == -1) {
    printf("dup2(STDERR) failed\n");
    return false;
  }

  if (fd > STDERR_FILENO) {
    if (close(fd) == -1) {
      printf("close() failed\n");
      return false;
    }
  }

  return true;
}

int main(int argc, char** argv) {
  start_daemon();
  
  while (true) {
    sleep(100);
  }

  return 0;
}