#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <mem.h>
#include <str.h>
#include "candidator.h"
#include "spellchecker.h"
#define CHECK_WORD_ON 1
#define CHECK_WORD_OFF 0
#define init_list(name) do{\
    safer_malloc(name,sizeof(*name));\
    name->head=NULL;\
    name->tail=NULL;\
    name->len=0;\
}while(0)

/*delete str[idx] from str*/
static char*
delete(char* str,int len,int idx){
    static char buf[MAX_WORD_LEN]={'\0'};
    char* pbuf=buf;
    for(int i=0;i<len;i++){
        if(i==idx){
        }else{
            *pbuf++=str[i];
        }
    }
    *pbuf='\0';
    return buf;
}

/*replace str[idx] with repl*/
static char*
replace(char* str,int len,int idx,char repl){
    static char buf[MAX_WORD_LEN]={'\0'};
    for(int i=0;i<len;i++){
        if(i==idx){
            buf[i]=repl;
        }else{
            buf[i]=str[i];
        }
    }
    buf[len]='\0';
    return buf;
}

/*swap adjacent characters(str[idx] and str[idx+1])*/
static char*
swapadja(char* str,int idx){
    static char buf[MAX_WORD_LEN]={'\0'};
    strcpy(buf,str);
    char tmp=buf[idx];
    buf[idx]=buf[idx+1];
    buf[idx+1]=tmp;
    return buf;
}

/*insert c at position pos*/
static char*
insert_at(char* str,int len,int pos,char c){
    static char buf[MAX_WORD_LEN]={'\0'};
    strncpy(buf,str,pos);
    buf[pos]=c;
    strcpy(buf+pos+1,str+pos);
    buf[len+1]='\0';
    return buf;
}

static int
find_word(CandList list,char* word){
    Word cur=list->head;
    while(cur!=NULL){
        if(strcmp(cur->word,word)==0){
            return 1;
        }
        cur=cur->next;
    }
    return 0;
}

static void 
add_word(CandList list,char* word,int cswitch){
    if(cswitch&&Schecker_check(word)==0){
        return;
    }else{
        if(find_word(list,word)==1){
            return;
        }
        Word new;
        safer_malloc(new,sizeof(*new));
        strcpy(new->word,word);
        if(list->head==NULL){
            new->next=NULL;
            list->head=list->tail=new;
        }else{
            new->next=list->head;
            list->head=new;
        }
        list->len++;
    }
}

static CandList
merge_list(CandList dst,CandList src){
    Word cur=src->head;
    while(cur!=NULL){
        if(find_word(dst,cur->word)==0){
            add_word(dst,cur->word,CHECK_WORD_OFF);
        }else{
        }
        cur=cur->next;
    }
    Cand_free(src);
    return dst;
}

//modify str once.
static CandList
gen_small_list(char* str,int cswitch){
    int len=strlen(str);
    assert(len>0);
    if(len>=MAX_WORD_LEN){
        return NULL;
    }
    to_lower(str,str);
    CandList list;
    init_list(list);
    //remove one letter
    for(int i=0;i<len;i++){
        add_word(list,delete(str,len,i),cswitch);
    }
    //change one letter to another
    for(int i=0;i<len;i++){
        for(int j='a';j<='z';j++){
            if(str[i]!=j){
                add_word(list,replace(str,len,i,j),cswitch);
            }
        }
    }
    //swap two adjacent letters
    for(int i=0;i<len-1;i++){
        if(str[i]!=str[i+1]){
            add_word(list,swapadja(str,i),cswitch);
        }
    }
    //add a letter
    for(int i=0;i<len+1;i++){
        for(int j='a';j<='z';j++){
            add_word(list,insert_at(str,len,i,j),cswitch);
        }
    }
    return list;
}

/*modify str twice.
 * cswitch-turn on/off spellchecker.
 */
static CandList
gen_big_list(char* str,int cswitch){
    CandList small_list=gen_small_list(str,CHECK_WORD_OFF);
    CandList big_list;
    init_list(big_list);
    Word w=small_list->head;
    while(w!=NULL){
        CandList slip=gen_small_list(w->word,cswitch*CHECK_WORD_ON);
        if(slip->len==0){
            safer_free(slip);
            goto next;
        }
        if(big_list->head==NULL){
            big_list->head=slip->head;
            big_list->tail=slip->tail;
            big_list->len=slip->len;
        }else{
            merge_list(big_list,slip);
        }
next:
        w=w->next;
    }
    return big_list;
}

//generate words exists in dictionary according to the str.
CandList
Cand_dist1(char* str){
    return gen_small_list(str,CHECK_WORD_ON);
}

CandList 
Cand_dist2(char* str){
    return gen_big_list(str,CHECK_WORD_ON);
}

void
Cand_free(CandList list){
    Word cur;
    while(list->head!=NULL){
        cur=list->head;
        free(cur);
        list->head=list->head->next;
    }
    free(list);
}

void
Cand_print(CandList list){
    printf("Candidator list:\n");
    Word w=list->head;
    while(w!=NULL){
        printf("%s\n",w->word);
        w=w->next;
    }
}
