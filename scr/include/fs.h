#ifndef FS_H
#define FS_H
#include "screen.h"
#include "system.h"
#include "util.h"
#include "drivers.h"
int fat16_init(unsigned int partition_num);
void fat16_display_vid(void);
void fat16_display_root(void);
void fat16_display_dir_entry(const char *name);

size_t fat16_get_file_size(const char *filename);
size_t fat16_load(const char *filename, void *buf, const size_t buf_size);
#endif