#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include <stdbool.h>

char *
last_component(char *path)
{
  char *p;
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  return p;
}

void
find(char *path, char *name, int print_inode)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  
  if ((fd = open(path, O_RDONLY)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }
  
  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
  
  if (strcmp(last_component(path), name) == 0) {
    if (print_inode)
      printf("%s %d\n", path, st.ino);
    else
      printf("%s\n", path);
  }
  
  if (st.type == T_DIR) {
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
      fprintf(2, "find: path too long\n");
      close(fd);
      return;
    }
    
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == 0)
        continue;
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      find(buf, name, print_inode);
    }
  }
  
  close(fd);
}

int
main(int argc, char *argv[])
{
  if (argc < 3) {
    fprintf(2, "find <dir> <name>  prints every path under <dir> whose last component is <name>\n");
    exit(1);
  }
  if (argc == 4 && strcmp(argv[3], "-n") == 0)
    find(argv[1], argv[2], 1);
  else
    find(argv[1], argv[2], 0);
  exit(0);
}
