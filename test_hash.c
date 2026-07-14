#include "hash.h"
#include <stdio.h>







int main(void) {

    hash_table *table=new_hash_table(sizeof(char *), sizeof(int), hash_str, equal_str);
    void * ptr = ",ndjede";



    put(table, "chaine 1", &(int) {5});
    put(table, "bonjour", &(int) {8});
    put(table, "chaine 3", & (int) {8} );
    put(table, "chaine 1", &(int) {919});

    int * res = get_value(table, "chaine 1");
    if (res!=NULL) {printf("get de chaine 1 (res attendu 919) : %d\n", *res);}
    printf("%d\n", contains_key(table, "bonjour"));


    print_hash_table(table, &print_str, &print_int);



    return 0;
}