#ifndef KERNEL_FS_H
#define KERNEL_FS_H

#include <stddef.h>

#define FS_MAX_FILES 64
#define FS_MAX_FILENAME 64

struct vfs_file {
  char name[FS_MAX_FILENAME];
  char *data;
  size_t size;
  int is_used;
};

void fs_init(void);
int fs_create(const char *name, const char *content);
const char *fs_read(const char *name);
void fs_list(void);

#endif /* KERNEL_FS_H */
