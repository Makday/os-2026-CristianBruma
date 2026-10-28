#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stdbool.h>

char *
typename(int t)
{
  switch(t) {
    case T_DIR: return "directory";
    case T_FILE: return "file";
    case T_DEVICE: return "device";
    default: return "?";
  }
}

int
main(int argc, char *argv[])
{
  struct stat st;
  for(int i = 1; i < argc; i++){
    if(stat(argv[i], &st) < 0) {
      fprintf(2, "mystat: error for %s\n", argv[i]);
      continue;
    }
    printf("%s: %s, inode %d, links %d, size %d bytes\n", argv[i], typename(st.type), st.ino, st.nlink, (int)st.size);
  }
  exit(0);
}
