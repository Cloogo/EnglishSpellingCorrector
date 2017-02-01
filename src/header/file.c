#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include "mem.h"
#include "err.h"
#include "file.h"

static size_t get_size(File f);
static char* get_ctx(File f);
static void free_ctx(File f);

File 
File_open(const char* name,const char* mode){
    File file;
    safer_malloc(file,sizeof(*file));
    FILE* f=fopen(name,mode);
    if(f==NULL){
        report_bug("fopen(%s,%s)",name,mode);
    }
    file->fd=f;
    get_size(file);
    get_ctx(file);
    return file;
}

void
File_close(File f){
    if(fclose(f->fd)==EOF){
        report_bug("fclose()");
    }
    free_ctx(f);
    safer_free(f);
}

static size_t
get_size(File f){
    if(fseek(f->fd,0,SEEK_END)==-1){
        report_bug("fseek(f,0,SEEK_END)");
    }
    f->size=ftell(f->fd);
    return f->size;
}

static char*
get_ctx(File f){
    f->ctx=mmap(NULL,f->size,PROT_WRITE,MAP_PRIVATE,fileno(f->fd),0);
    if(f->ctx==MAP_FAILED){
        report_bug("mmap failed");
    }
    return f->ctx;
}

static void
free_ctx(File f){
    munmap(f->ctx,f->size);
}
