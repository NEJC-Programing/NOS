#ifndef FS_H
#define FS_H
#include "../include/screen.h"
#include "../include/system.h"
#include "../include/util.h"
typedef struct File_ {
	string path;
    string name;
	uint32 pos;
	uint32 size;
	void *data;
} File;
typedef struct Dir2_ {
    string path;
    string name;
    void* inner_dirs;
    File* inner_files;
} Dir2;
typedef struct Dir_ {
    string path;
    string name;
    Dir2* inner_dirs;
    File* inner_files;
} Dir;
#endif