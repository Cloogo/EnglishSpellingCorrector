#ifndef _ARGS_H
#define _ARGS_H
extern char** Args_get(char* line,char* delim,int* nargs);
extern char* Args_join(char** args,int nargs,char* delim);
extern void Args_free(char*** args,int nargs);
#endif
