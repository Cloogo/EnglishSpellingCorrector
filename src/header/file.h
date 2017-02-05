#ifndef _FILE_H
#define _FILE_H
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct File{
    FILE* fd;
    size_t size;
    char* ctx;
}*File;
extern File File_read(const char* name);
extern File File_open(const char* name,const char* mode);
extern void File_close(File f);
#endif
