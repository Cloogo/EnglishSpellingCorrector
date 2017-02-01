#ifndef _CANDICATE_H
#define _CANDICATE_H
#define MAX_WORD_LEN 24
typedef struct CandList* CandList;
typedef struct Word* Word;

struct CandList{
    Word head;
    Word tail;
    int len;
};

struct Word{
    char word[MAX_WORD_LEN];
    Word next;
};

extern CandList Cand_dist1(char* w);
extern CandList Cand_dist2(char* w);
extern void Cand_print(CandList list);
extern void Cand_free(CandList list);
#endif
