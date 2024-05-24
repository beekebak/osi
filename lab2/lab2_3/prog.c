#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>


void PrintAndExit(char* error){
  perror(error);
  exit(EXIT_FAILURE);
}

void PrintErrnoAndExit(){
  PrintAndExit("error");
}

int main(){
  pid_t pid = fork();
  if(pid == -1){
    PrintErrnoAndExit();
  }
  if(pid != 0){
    int status;
    struct user_regs_struct regs;
    while(1){
      if(waitpid(pid, &status, 0) == -1) PrintErrnoAndExit();
      if (WIFEXITED(status) || WIFSIGNALED(status)){
        break;
      }
      if(ptrace(PTRACE_GETREGS, pid, NULL, &regs) == -1) PrintErrnoAndExit();
      printf("Syscall:%lld\n", regs.orig_rax);
      if(ptrace(PTRACE_SYSCALL, pid, NULL, NULL) == -1) PrintErrnoAndExit();
    }
  }
  else{
    if(ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) PrintErrnoAndExit();
    if(execl("/bin/ls", "/osi/lab2/lab2_3", NULL) == -1) PrintErrnoAndExit();
  }
  return 0;
}
