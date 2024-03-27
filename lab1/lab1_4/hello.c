#include <stdio.h>
#include <dlfcn.h>

int main(){
  void* handle = dlopen("./libdynamic.so", RTLD_LAZY);
  if(!handle){
    fprintf(stderr, "%s\n", dlerror());
    return 1;
  }
  void (*hello)(void) = dlsym(handle, "hello_from_dynamic_lib");
  char* error;
  if((error = dlerror()) != NULL){
    fprintf(stderr, "%s\n", error);
    return 1;
  }
  hello();
  dlclose(handle);
  return 0;
}
