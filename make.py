#!/usr/bin/env python

import glob
import os

root = "/mnt/c/Users/noam/Desktop/NOS/scr/"
compiler = "gcc"
asembler = "nasm" 
linker = "ld"
ldfile = root+"src/link.ld"
objcopy = ""
strip = ""

def gcc(filepath,out):
    return os.system(compiler+" -m32 -c -ffreestanding " +filepath+" -o "+out)


def nasm(filepath,out):
    return os.system(asembler+" -f elf32 "+filepath+" -o "+out)


def ld(filepaths,out,types = True):
    if types:
        return os.system(linker+" -r -m elf_i386 -T "+ldfile+" -o "+out+" "+filepaths)
    else:
        return os.system(linker+" -m elf_i386 -T "+ldfile+" -o "+out+" "+filepaths)


def get_dirs(dirpath):
    return glob.glob(dirpath+"/*/")


def get_c_files(dirpath):
    return glob.glob(dirpath+"/*.c")


def get_asm_files(dirpath):
    return glob.glob(dirpath+"/*.asm")


def get_object_files(dirpath):
    return glob.glob(dirpath+"/*.o")



def run(dir_to_run, notroot = True):
    dirs = get_dirs(dir_to_run)
    for x in dirs:
        run(x)
    cs = get_c_files(dir_to_run)
    for x in cs:
        gcc('"'+x+'"', '"'+x+'.o"')
    asms = get_asm_files(dir_to_run)
    for x in asms:
        nasm('"'+x+'"', '"'+x+'.o"')
    objects = get_object_files(dir_to_run)
    o = ""
    for x in objects:
        o = o+" \""+x+'"'
    ld(o,dir_to_run+"../",notroot)


run(root,False)

"""
make dir tree

for each inner dir compile all into object files

for each dir make object file

link root objects into kernel and strip
"""