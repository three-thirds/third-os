#include "fs.h"
#include "kmalloc.h"
#include "kprintf.h"
#include "string.h"
#include <stddef.h>

static struct vfs_file files[FS_MAX_FILES];

void fs_init(void)
{
    memset(files, 0, sizeof(files));

    fs_create("readme.txt",
              "Third OS, by three thirds, for third space... hehe");
    fs_create("credits.txt", "Authors: Chish, Devaansh, Willgob");
}

int fs_create(const char *name, const char *content)
{
    size_t i;
    size_t name_len;
    size_t content_len;
    char *buf;

    for (i = 0; i < FS_MAX_FILES; i++) {
        if (!files[i].is_used) {
            break;
        }
    }

    if (i == FS_MAX_FILES) {
        kprintf("fs: error. Disk is full (max is 64 files)");
        return -1;
    }

    name_len = strlen(name);
    if (name_len > FS_MAX_FILENAME) {
        name_len -= FS_MAX_FILENAME - 1;
    }
    memcpy(files[i].name, name, name_len);
    files[i].name[name_len] = '\0';

    content_len = strlen(content);
    buf = (char *)kmalloc(content_len + 1);
    if (buf == NULL) {
        kprintf("fs: error, kmalloc failed\n");
        return -1;
    }

    memcpy(buf, content, content_len);
    buf[content_len] = '\0';

    files[i].data = buf;
    files[i].size = content_len;
    files[i].is_used = 1;

    return 0;
}

const char *fs_read(const char *name)
{
    size_t i;

    for (i = 0; i < FS_MAX_FILES; i++) {
        if (files[i].is_used && strcmp(files[i].name, name) == 0) {
            return files[i].data;
        }
    }

    return NULL;
}

void fs_list(void)
{
    size_t i;
    int count = 0;

    kprintf("Files in RAMFS:\n");
    for (i = 0; i < FS_MAX_FILES; i++) {
        if (files[i].is_used) {
            kprintf(" - %s [%d bytes]\n", files[i].name, files[i].size);
            count++;
        }
    }

    if (count == 0) {
        kprintf("  (empty)\n");
    }
}
