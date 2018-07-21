#ifndef FS_H
#define FS_H
#include "screen.h"
#include "system.h"
#include "util.h"
#include "drivers.h"
struct dirent{
    struct dirent *next;
    char *name;
    uint32_t byte_size;
    uint8_t is_dir;
    uint32_t cluster_no;
};
struct fs {
	const char *fs_name;
    int     (*fs_load)(const char *path, uint8_t *buf, uint32_t buf_size);
    int     (*fs_store)(const char *path, uint8_t *buf);
};
int fat_load(const char *path, uint8_t *buf, uint32_t buf_size);
int fat_store(const char *path, uint8_t *buf);
#endif