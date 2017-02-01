#include <stdio.h>
#include <stdlib.h>
#include <langmod.h>

int
main(){
    Langmod_build((char*)"../../res/english.txt");
    Langmod_print();
    Langmod_destroy();
    return 0;
}
