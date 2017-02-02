#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <spellchecker.h>
#include <candidator.h>
#include <langmod.h>
#include <mcheck.h>

char*
max(CandList cand){
    static char buf[MAX_WORD_LEN];
    int factor=100000000;
    int m=-1;
    Word cur=cand->head;
    while(cur!=NULL){
        int tmp=Langmod_get_probability(cur->word)*factor;
        if(tmp>=m){
            strcpy(buf,cur->word);
            m=tmp;
        }
        cur=cur->next;
    }
    Cand_free(cand);
    return buf;
}

char*
correct(char* w){
    if(Schecker_check(w)){
        return w;
    }
    CandList small=Cand_dist1(w);
    if(small->len>0){
        return max(small);
    }else{
        CandList big=Cand_dist2(w);
        if(big->len>0){
            return max(big);
        }else{
            return w;
        }
    }
}

int
main(int argc,char* argv[]){
    if(argc!=2){
        fprintf(stderr,"usage:%s <wordlist>\n",argv[0]);
        exit(1);
    }
    setenv("MALLOC_TRACE","spellcorrector_mtrace",1);
    mtrace();
    Schecker_install();
    Langmod_build("../../res/big.txt");
    printf("Do you mean \"%s\"?\n",correct(argv[1]));
    Langmod_destroy();
    Schecker_uninstall();
    return 0;
}
