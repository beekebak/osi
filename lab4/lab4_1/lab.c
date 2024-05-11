#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int global_init_var = 1;
int global_not_init_var;
const int global_const_var = 2;

void Func(){
  int local_var;
  int local_init_var = 3;
  static int static_local_init_var = 4;
  static int static_local_var;
  const int const_local_init_var = 7;
  const int const_local_var;
  printf(" giv %p\n gniv %p\n gcv %p\n lv %p\n slv %p\n clv %p\n liv %p\n sliv %p\n cliv %p\n",
             &global_init_var, &global_not_init_var, &global_const_var,
             &local_var, &static_local_var, &const_local_var,
             &local_init_var, &static_local_init_var, &const_local_init_var);
}

int* LocalInitValFunc(){
  int local_init_val_again = 10;
  printf(" liva %p\n", &local_init_val_again);
  return &local_init_val_again;
}

void BuffFunc(){
  char* buf = malloc(100 * sizeof(char));
  if(buf == NULL){
    perror("error");
    exit(1);
  }
  strcpy(buf, "hello world");
  printf(" before free %s\n", buf);
  free(buf);
  printf(" after free %s\n", buf);

  char* buf2 = malloc(100 * sizeof(char));
  if(buf2 == NULL){
    perror("error");
    exit(1);
  }
  strcpy(buf, "howdy partner");
  printf(" before free2 %s\n", buf);
  buf += 50;
  //free(buf) causes shutdown
  free(buf-50);
  printf(" after free2 bufstart %s\n buf mid %s\n", buf-50, buf);
}

void PrintEnv(){
  char* res = getenv("OSI_VAR");
  if(res == NULL){
    perror("cant get env var");
    exit(1);
  }
  printf(" Env %s\n", res);
  if((setenv("OSI_VAR", "changed", 1)) == -1){
    perror("error");
    exit(1);
  }
  res = getenv("OSI_VAR");
  if(res == NULL){
    perror("cant get env var");
    exit(1);
  }
  printf(" Env2 %s\n", res);
}

void AddressHelper(){
  printf(" pid: %d", getpid());
  while(1) sleep(1000);
}

int main(){
  Func();
  LocalInitValFunc();
  BuffFunc();
  PrintEnv();
//  AddressHelper();         //function made for looking to /proc/pid/maps
  return 0;
}
