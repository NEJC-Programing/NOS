#!/bin

import glob

def gcc(filepath,out):

def nasm(filepath,out):

def ld(filepath,out):

def get_dirs(dirpath):
    return glob.glob(dirpath+"/*/")

def get_c_files(dirpath):
    return glob.glob(dirpath+"/*.c")

def get_asm_files(dirpath):
    return glob.glob(dirpath+"/*.asm")

