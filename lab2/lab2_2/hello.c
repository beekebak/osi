#include <stdio.h>

int main() {
  char msg[] = "hello world\n";
  __asm__ __volatile__(
      "mov $1, %%rax\n\t"
      "mov $1, %%rbx\n\t"
      "mov %0, %%rcx\n\t"
      "mov $13, %%rdx\n\t"
      "syscall"
      :
      : "r" (msg)
      : "%rax", "%rbx", "%rcx", "%rdx");
  return 0;
}
