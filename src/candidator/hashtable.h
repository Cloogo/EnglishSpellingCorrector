#ifndef _HASHSTR_H
#define _HASHSTR_H
extern void Hashtable_init();
extern void Hashtable_destroy();
extern void Hashtable_put(const char* key,int len);
extern int Hashtable_get(const char* key,int len);
extern char* Hashtable_print();
#endif
