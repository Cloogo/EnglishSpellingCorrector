#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "hashtable.h"
#include <mem.h>
#include <str.h>

#define MAX_HELEM_N 1024*1024
#define foreach(op) do{\
    for(int i=0;i<MAX_HELEM_N;i++){\
        for(WordList w=table[i],next;w!=NULL;w=next){\
            next=w->next;\
            op;\
        }\
    }\
}while(0)

static int factor=31;

typedef struct WordList{
    const char* word;
    struct WordList* next;
}*WordList;

static WordList table[MAX_HELEM_N];

static int
hash(const char* word){
    int v=0;
    for(const char* p=word;*p!='\0';p++){
        v=v*factor+*p;
    }
    if(v<0){
        v=v+INT_MAX+1;
    }
    return v&(MAX_HELEM_N-1);
}

void
Hashtable_init(){
    for(int i=0;i<MAX_HELEM_N;i++){
        table[i]=NULL;
    }
}

void
Hashtable_destroy(){
    foreach(safer_free(w));
}

void
Hashtable_put(const char* word,int len){
    int v=hash(word);
    WordList first=table[v];
    for(WordList w=first;w!=NULL;w=w->next){
        if(memcmp(w->word,word,len)==0){
            return;
        }
    }
    WordList new;
    safer_malloc(new,sizeof(*new));
    new->word=word;
    new->next=table[v];
    table[v]=new;
}

int
Hashtable_get(const char* word,int len){
    int v=hash(word);
    WordList first=table[v];
    for(WordList w=first;w!=NULL;w=w->next){
        if(memcmp(w->word,word,len)==0){
            return 1;
        }
    }
    return 0;
}

char*
Hashtable_print(){
    char buf[1024*1024];//overbuffer?
    foreach(do{
            sprintf(buf,"%s\n",w->word);
            append(buf);
            }while(0));
    return append(NULL);
}
