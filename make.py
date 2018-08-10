#!/usr/bin/env python

import glob
import os

gcc = ""
nasm = "" 
ld = ""
ldfile = ""
objcopy = ""
strip = ""

def gcc(filepath,out):
    return os.system(gcc+" -m32 -c -ffreestanding +filepath+" -o +out)


def nasm(filepath,out):
    return os.system(nasm+" -f elf32 "+filepath+" -o "+out)


def ld(filepaths,out,type = 1):
    if type:
        return os.system(ld+" -r -m elf_i386 -T "+ldfile+" -o "+out+" "+filepaths)
    else:
        return os.system(ld+" -m elf_i386 -T "+ldfile+" -o "+out+" "+filepaths)


def get_dirs(dirpath):
    return glob.glob(dirpath+"/*/")


def get_c_files(dirpath):
    return glob.glob(dirpath+"/*.c")


def get_asm_files(dirpath):
    return glob.glob(dirpath+"/*.asm")


def get_object_files(dirpath):
    return glob.glob(dirpath+"/*.o")


"""
make dir tree

for each inner dir compile all into object files

for each dir make object file

link root objects into kernel and strip
"""