#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <spellchecker.h>
#include <candidator.h>
#include <langmod.h>
#include <test.h>

#define testcase(w,expected) strcpy(casebuf,w);test_cond(strcmp(correct(casebuf),expected)==0,"%s correct(%s)=%s",w,w,correct(casebuf))

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
main(){
    Schecker_install();
    Langmod_build((char*)"../../res/big.txt");
    char casebuf[1024];
    testcase("speling","spelling");
    testcase("korrectud","corrected");
    testcase("bycycle","bicycle");
    testcase("inconvient","inconvenient");
    testcase("arrainged","arranged");
    testcase("peotryy","poetry");
    testcase("word","word");
    testcase("somthing","something");
    testcase("quintessential","quintessential");
    testcase("reciet","recite");
    testcase("adres","acres");
    testcase("rember","member");
    testcase("juse","just");
    testcase("accesing","acceding");
    testcase("hown","how");
    testcase("natior","nation");
    testcase("quies","quiet");
    testcase("oranisation","organization");
    strcpy(casebuf,"the");
    test_cond(0.07<Langmod_get_probability(casebuf),"");
    test_cond(Langmod_get_probability(casebuf)<0.08,"");
    strcpy(casebuf,"quintessential");
    test_cond(Langmod_get_probability(casebuf)==0,"");
    test_report();
    Langmod_destroy();
    Schecker_uninstall();
    return 0;
}
