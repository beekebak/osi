#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

const int buff_size = 4096;

void PrintAndExit(char* error){
  perror(error);
  exit(EXIT_FAILURE);
}

void PrintErrnoAndExit(){
  PrintAndExit("error");
}

char* GetReverseString(char* str){
  int n = strlen(str);
  char* reversed = malloc(n + 1);
  if(reversed == NULL) {
    perror("Couldnt get memory");
    return NULL;
  }
  for (int i = 0; i < n; i++) {
    reversed[i] = str[n - 1 - i];
  }
  reversed[n] = '\0';
  return reversed;
}

void ReverseAndCopy(FILE* src_file, FILE* dest_file){
  fseek(src_file, 0, SEEK_END);
  long file_size = ftell(src_file);
  rewind(src_file);
  char* buffer = malloc(file_size);
  if(buffer == NULL) {
    perror("couldnt get memory");
    return;
  }
  fread(buffer, 1, file_size, src_file);
  for(int i = file_size-1; i >= 0; i--){
    putc(buffer[i], dest_file);
  }
  free(buffer);
}

int CopyReverseFile(char* src, char* dest){
  FILE* f_src = fopen(src, "r");
  FILE* f_dest = fopen(dest, "w");
  if(f_src == NULL || f_dest == NULL){
    perror("Cant open files");
    return 1;
  }
  ReverseAndCopy(f_src, f_dest);
  if(fclose(f_src) == EOF || fclose(f_dest) == EOF){
    perror("problem during file close procedure");
    return 1;
  }
}

int CopyDir(char* src, char* dest){
  DIR *dp;
  struct dirent *entry;
  struct stat statbuf;
  char src_path[buff_size];
  char dest_path[buff_size];
  if ((dp = opendir(src)) == NULL) {
    fprintf(stderr, "Cannot open directory: %s\n", src);
    return 1;
  }
  while ((entry = readdir(dp)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
    snprintf(src_path, sizeof(src_path), "%s/%s", src, entry->d_name);
    char* reversed_entry_name = GetReverseString(entry->d_name);
    if(reversed_entry_name == NULL) return 1;
    snprintf(dest_path, sizeof(dest_path), "%s/%s", dest, reversed_entry_name);
    if (lstat(src_path, &statbuf) == -1) {
      free(reversed_entry_name);
      continue;
    }
    if (S_ISREG(statbuf.st_mode)) {
      if(CopyReverseFile(src_path, dest_path) == 1){
        free(reversed_entry_name);
        return 1;
      }
    }
    free(reversed_entry_name);
  }
  if(closedir(dp) == -1){
    perror(strerror(errno));
  }
  return 0;
}

int main(int argc, char** argv){
  if(argc != 2){
    PrintAndExit("wrong arguments count");
  }
  char* in_dir_name = strrchr(argv[1], '/');
  in_dir_name = in_dir_name ? in_dir_name + 1 : argv[1];
  char* out_dir_name = GetReverseString(in_dir_name);
  if(out_dir_name == NULL){
    return 1;
  }
  char out_path[buff_size];
  snprintf(out_path, buff_size, "./%s", out_dir_name);
  if(mkdir(out_dir_name, 0755) == -1) PrintErrnoAndExit();

  int res = CopyDir(argv[1], out_path);

  free(out_dir_name);
  return res;
}
