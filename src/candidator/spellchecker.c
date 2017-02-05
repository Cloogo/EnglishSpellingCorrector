#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <file.h>
#include <args.h>
#include <str.h>
#include "hashtable.h"
#include "spellchecker.h"

typedef struct Dict{
    char** words;
    int nwords;
}Dict;

typedef Dict Fixes;

Dict dict;
Fixes pre,suf;

static void load_dict();
static void destroy_dict();
static void load_fixes();
static void destroy_fixes();
static int trim_word_from_begin(char* check,int len);
static int trim_word_from_last(char* check,int len);

static void
load_dict(){
    Hashtable_init();
    File f=File_read("../../res/american-english");
    char** words=Args_get(f->ctx,"\n",&(dict.nwords));
    for(int i=1;i<dict.nwords;i++){
        int len=to_lower(words[i],words[i]);
        Hashtable_put(words[i],len);
    }
    dict.words=words;
    File_close(f);
}

static void
load_fixes(){
    File f=File_read("../res/prefixes.txt");
    pre.words=Args_get(f->ctx,"\n",&pre.nwords);
    File_close(f);
    f=File_read("../res/suffixes.txt");
    suf.words=Args_get(f->ctx,"\n",&suf.nwords);
    File_close(f);
}

static void 
destroy_fixes(){
    Args_free(&pre.words,pre.nwords);
    Args_free(&suf.words,suf.nwords);
}

static void
destroy_dict(){
    Args_free(&dict.words,dict.nwords);
    Hashtable_destroy();
}

/*trim word according to fixes from beginning to end and 
 * check whether it's in dictionary.
 * return val:
 * 0-not found.
 * 1-found.
*/
static int
trim_word_from_begin(char* check,int len){
    for(int i=0;i<pre.nwords;i++){
        for(int j=0;j<pre.nwords;j++){
            ltrim(check,pre.words[j]);
            if(Hashtable_get(check,len)!=0){
                return 1;
            }
        }
    }
    for(int i=0;i<suf.nwords;i++){
        for(int j=0;j<suf.nwords;j++){
            rtrim(check,suf.words[j]);
            if(Hashtable_get(check,len)!=0){
                return 1;
            }
        }
    }
    return 0;
}

/*trim word according to fixes from end to beginning and 
 * check whether it's in dictionary.
 * return val:
 * 0-not found.
 * 1-found.
*/
static int
trim_word_from_last(char* check,int len){
    for(int i=0;i<pre.nwords;i++){
        for(int j=pre.nwords-1;j>=0;j--){
            ltrim(check,pre.words[j]);
            if(Hashtable_get(check,len)!=0){
                return 1;
            }
        }
    }
    for(int i=0;i<suf.nwords;i++){
        for(int j=suf.nwords-1;j>=0;j--){
            rtrim(check,suf.words[j]);
            if(Hashtable_get(check,len)!=0){
                return 1;
            }
        }
    }
    return 0;
}

void
Schecker_install(){
    load_dict();
//    load_fixes();
}

void
Schecker_uninstall(){
//    destroy_fixes();
    destroy_dict();
}


/*
 * retval:
 * 0-check does not exist in dictionary.
 * 1-check exists in dictionary.
 */
int
Schecker_check(char* check){
    char cp1[1024];
    char cp2[1024];
    int len=to_lower(check,check);
    if(Hashtable_get(check,len)==0){
/*      
        sprintf(cp1,"%s",checks);
        sprintf(cp2,"%s",checks);
        if(trim_word_from_begin(cp1,len)==0&&trim_word_from_last(cp2,len)==0){
            return 0;
        }
        */
        return 0;
    }
    return 1;
}
