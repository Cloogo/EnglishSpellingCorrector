#ifndef _LANGMOD_H
#define _LANGMOD_H
extern void Langmod_build(char* fpath);
extern void Langmod_top20();
extern void Langmod_print();
extern int Langmod_all_words_num();
extern int Langmod_diff_words_num();
extern int Langmod_get_num_of(char* word);
extern float Langmod_get_probability(char* word);
extern void Langmod_put(char* word);
extern void Langmod_destroy();
#endif
