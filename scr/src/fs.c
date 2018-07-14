#include "../include/fs.h"
Dir vfs; 
void init_vfs()
{
    //vfs = (string)malloc(512);
    Dir mnt, root, bin;
    mnt.name = "mnt";
    mnt.path = "/mnt/";
    root.name="root";
    root.path="/root/";
    bin.path="/bin/";
    bin.name="bin";
    vfs.path = "/";
    vfs.name = "root";
    Dir  s[3];
    s[0] = mnt;
    s[1]= root;
    s[2] = bin;
    vfs.inner_dirs = s;
}

void add_dir(Dir dir)
{
    // adds to /
    Dir * id = vfs.inner_dirs;
    int n = sizeof(id)/sizeof(id[0]);
    id[n+1]=dir;
    vfs.inner_dirs=id;
}

void add_file(File file)
{
    //string path = file.path;
    // add to /root
    File * ifs = vfs.inner_dirs[1].inner_files;
    int n = sizeof(ifs)/sizeof(ifs[0]);
    ifs[n+1]=file;
    vfs.inner_dirs[1].inner_files = ifs;
}

File fopen(string file)
{
    // opens from /root
    File * ifs = vfs.inner_dirs[1].inner_files;
    int n = sizeof(ifs)/sizeof(ifs[0]);
    for(int i = 0; i>n;i++){
        if(ifs[i].name == file){
            return ifs[i];
        }
    }
}

BYTE* fread(File file, int size)
{
    if (file.size >= size)
        size = file.size;
    vfs_read(file,0xD0000,size);
    BYTE buff = (BYTE)0xD0000;
    return buff;
}


void vfs_read(File file, void *data, int size){
    memory_copy(file.pos,data,size);
}