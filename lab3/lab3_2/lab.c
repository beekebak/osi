#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

typedef void (*Func)(char*);

typedef struct{
  const char* name;
  Func function;
} MapEntry;

void PrintAndExit(char* error){
  perror(error);
  exit(EXIT_FAILURE);
}

void PrintErrnoAndExit(){
  PrintAndExit(strerror(errno));
}

void MakeDirectory(char* name){
  if(mkdir(name, 0755) == -1){
    PrintErrnoAndExit();
  }
}

void ShowDirectory(char* path){
  DIR* d;
  struct dirent* file;
  d = opendir(path);
  if (d) {
    while ((file = readdir(d)) != NULL) {
      printf("%s\n", file->d_name);
    }
    closedir(d);
  }
  else {
    PrintErrnoAndExit();
  }
}

void RemoveDirectory(char* path){
  if(rmdir(path) == -1) PrintErrnoAndExit();
}

void CreateFile(char* name){
  FILE* f = fopen(name, "w");
  if(!f) PrintAndExit("couldnt open file");
  if(!fclose(f) == -1) PrintErrnoAndExit();
}

void PrintFile(char* path){
  FILE* file = fopen(path, "r");
  if(file == NULL) PrintAndExit("couldnt open file");
  char c;
  while((c = fgetc(file)) != EOF){
    putchar(c);
  }
  if(!fclose(file) == -1) PrintErrnoAndExit();
}

void RemoveFile(char* path){
  if(remove(path) != 0) PrintErrnoAndExit();
}

void CreateSymbolLink(char* path){
  if(symlink(path, "./symlink")) PrintErrnoAndExit();
}

void PrintSymbolLinkSelf(char* path){
  char buf[4096];
  int len;
  if((len = readlink(path, buf, sizeof(buf)-1)) != 1){
    buf[len] = '\0';
    printf("%s\n", buf);
  }
  else PrintErrnoAndExit();
}

void PrintSymbolLinkFile(char* path){
  char buf[4096];
  int len;
  if((len = readlink(path, buf, sizeof(buf)-1)) != 1){
    buf[len] = '\0';
  }
  else PrintErrnoAndExit();
  PrintFile(buf);
}

void CreateHardLink(char* path){
  if(link(path, "./hardlink") != 0) PrintErrnoAndExit();
}

void RemoveHardLink(char* path){
  if(unlink(path) != 0) PrintErrnoAndExit();
}

void PrintAccess(char* path){
  struct stat info;
  if(stat(path, &info) == -1) PrintErrnoAndExit();
  printf("%o\n%lu\n", info.st_mode&0777, info.st_nlink);
}

void ChangeAccess(char* path){
  mode_t perms = 0644;
  if(chmod(path, perms) != 0) PrintErrnoAndExit();
}

int main(int argc, char** argv){
  if(argc < 2){
    PrintAndExit("not enough args");
  }
  MapEntry funcs[] = {{"MakeDirectory", MakeDirectory},
                      {"ShowDirectory", ShowDirectory},
                      {"RemoveDirectory", RemoveDirectory},
                      {"CreateFile", CreateFile},
                      {"PrintFile", PrintFile},
                      {"RemoveFile", RemoveFile},
                      {"CreateSymbolLink", CreateSymbolLink},
                      {"PrintSymbolLinkSelf", PrintSymbolLinkSelf},
                      {"PrintSymbolLinkFile", PrintSymbolLinkFile},
                      {"CreateHardLink", CreateHardLink},
                      {"RemoveHardLink", RemoveHardLink},
                      {"PrintAccess", PrintAccess},
                      {"ChangeAccess", ChangeAccess}};
  char prog_name[256];
  strncpy(prog_name, argv[0]+2, 256);
  for(int i = 0; i < 13; i++){
    if(!strcmp(funcs[i].name, prog_name)){
      funcs[i].function(argv[1]);
      return 0;
    }
  }
  perror("no function matched");
  return 1;
}
