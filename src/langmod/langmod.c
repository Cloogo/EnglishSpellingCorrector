#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <mem.h>
#include <args.h>
#include <file.h>
#include <str.h>
#include "langmod.h"

#define MAX_HELEM_N 1024*1024
#define MAX_WORD_LEN 128
#define foreach(op) do{\
    for(int _i=0;_i<MAX_HELEM_N;_i++){\
        for(WordList w=table[_i],next;w!=NULL;w=next){\
            next=w->next;\
            op;\
        }\
    }\
}while(0)


typedef struct WordList{
    const char* word;
    int num;
    struct WordList* next;
}*WordList;

static WordList table[MAX_HELEM_N];

static int factor=31;
static int n;//number of all words of English text.
static char** words;//different words of English text.
static int ndiff;//number of different words of English text.

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
Langmod_put(char* word){
    int v=hash(word);
    WordList first=table[v];
    for(WordList w=first;w!=NULL;w=w->next){
        if(strcmp(w->word,word)==0){
            w->num++;
            return;
        }
    }
    WordList new;
    safer_malloc(new,sizeof(*new));
    new->num=1;
    new->word=word;
    new->next=table[v];
    table[v]=new;
    ndiff++;
}

int
Langmod_get_num_of(char* word){
    int v=hash(word);
    WordList first=table[v];
    for(WordList w=first;w!=NULL;w=w->next){
        if(strcmp(w->word,word)==0){
            return w->num;
        }
    }
    return 0;
}

int
Langmod_diff_words_num(){
    return ndiff;
}

int 
Langmod_all_words_num(){
    return n;
}

void
Langmod_print(){
    printf("%-30s%-6s%-10s\n","word","count","probability");
    foreach(do{
            printf("%-30s%-6d%-10f\n",w->word,w->num,Langmod_get_probability((char*)w->word));
            }while(0));
    printf("\n\nSummary:\n");
    printf("Langmod_all_words_num:%d\n",Langmod_all_words_num());
    printf("Langmod_diff_words_num:%d\n",Langmod_diff_words_num());
    Langmod_top20();
}

void
Langmod_top20(){
    printf("Langmod_top20:\n");
    WordList top[20];
    int i=0;
    foreach(
            do{
                if(i<20){
                    top[i]=w;
                    i++;
                }else{
                    int min=INT_MAX;
                    int idx=0;
                    for(int j=0;j<20;j++){
                        if(top[j]->num<min){
                            min=top[j]->num;
                            idx=j;
                        }
                    }
                    if(top[idx]->num<w->num){
                        top[idx]=w;
                    }
                }
            }while(0)
            );
    for(;--i>=0;){
        printf("P{\"%s\",%d}=%f\n",top[i]->word,top[i]->num,Langmod_get_probability((char*)top[i]->word));
    }
}

void 
Langmod_build(char* fpath){
    File f=File_open(fpath,"r");
    for(int i=0;i<MAX_HELEM_N;i++){
        table[i]=NULL;
    }
    char* delim=" \n\t\f\r\b|/\\\"'~!@#$%^&*()_+=<>{}[];,.:?-0123456789";
    words=Args_get(f->ctx,delim,&n);
    for(int i=0;i<n;i++){
        to_lower(words[i],words[i]);
        Langmod_put(words[i]);
    }
    File_close(f);
}

/*
 * calculate the probability that w appears as a word of English text
 */
float
Langmod_get_probability(char* word){
    to_lower(word,word);
    return (float)Langmod_get_num_of(word)/n;
}

void
Langmod_destroy(){
    Args_free(&words,n);
    foreach(safer_free(w));
}
