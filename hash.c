#include <stdio.h>
#include "hash.h"
#include <stdbool.h>
#include <math.h>



long hash_str(void *p) {
    char *str=p;
    long hash=0;
    int len=strlen(p);
    for (int i=0; i<len;i++) {
        int charI=str[i];
        hash+=charI*pow(31,  len-1-i);
    }
    return hash;
}

bool equal_str(void *p1, void *p2) {
    int cmp=strcmp((char*) p1, (char*) p2);
    return cmp==0;
}

void print_str(void *p) {
    printf("%s", (char*)p);
}

long hash_int(void *p) {
    int val = *(int*)p;
    return (long)val;
}

bool equal_int(void *p1, void *p2) {
    return *(int*)p1 == *(int*)p2;
}

void print_int(void *p) {
    printf("%d", *(int*)p);
}

long hash_long(void *p) {
    return *(long*)p;
}

bool equal_long(void *p1, void *p2) {
    return *(long*)p1 == *(long*)p2;
}

void print_long(void *p) {
    printf("%ld", *(long*)p);
}

long hash_float(void *p) {
    float val = *(float*)p;
    return (long)val;
}

bool equal_float(void *p1, void *p2) {
    return *(float*)p1 == *(float*)p2;
}

void print_float(void *p) {
    printf("%f", *(float*)p);
}

long hash_double(void *p) {
    double val = *(double*)p;
    return (long)val;
}

bool equal_double(void *p1, void *p2) {
    return *(double*)p1 == *(double*)p2;
}

void print_double(void *p) {
    printf("%f", *(double*)p);
}

long hash_char(void *p) {
    char val = *(char*)p;
    return (long)val;
}

bool equal_char(void *p1, void *p2) {
    return *(char*)p1 == *(char*)p2;
}

void print_char(void *p) {
    printf("%c", *(char*)p);
}





hash_table *new_hash_table(size_t key_size, size_t value_size, long (*hash) (void*), bool (*equal) (void*, void*) ) {
    if (hash ==NULL || equal==NULL) {return NULL;}
    hash_table * res = malloc(sizeof(hash_table));
    res->size=16;
    res->buckets=malloc(res->size*sizeof(sentinel_entry ));
    res->elements_count=0;
    res->key_size=key_size;
    res->value_size=value_size;
    res->equal=equal;
    res->hash=hash;
    return res;
}


entry * new_entry(void *key, void *value, size_t key_size, size_t value_size) {
    if (key==NULL || value==NULL) {return NULL;}
    entry * res=malloc(sizeof(entry));
    void * k = malloc(key_size);
    void * v = malloc (value_size);
    memcpy(k, key, key_size);
    memcpy(v, value, value_size);
    res->key=k;
    res->value=v;
    res->next=NULL;
    return res;
}

void put(hash_table * table, void * key,  void* value) {
    if (key==NULL || value==NULL) {return;}
    long hash = table->hash(key);
    int index= hash & (table->size-1);
    // printf("chaine =%s - hash = %ld - index = %d\n",(char*) key , hash, index);

    sentinel_entry * sentinel = table->buckets+index;
    if (sentinel->head==NULL) {
        sentinel->head=new_entry(key, value, table->key_size, table->value_size); 
        table->elements_count++;

        return;
    }
    else {
        entry *current= sentinel->head;
        while (current->next!=NULL && !table->equal(current->key, key)) {
            current=current->next;
        }
        if (table->equal(current->key, key)) {
            memcpy(current->key, key, table->key_size);
            memcpy(current->value, value, table->value_size);
            return;
        }
        else {
            current->next=new_entry(key, value, table->key_size, table->value_size);
        }
    }
    table->elements_count++;

}


void print_hash_table(hash_table *table, void (print_key_function) (void*), void (print_value_function) (void*) ) {
    if (print_key_function==NULL || print_value_function==NULL) {return;}
    printf("{ ");
    int count=0;
    for (int i=0; i<table->size; i++) {
        entry *current=(table->buckets[i]).head;
        while (current!=NULL) {
            count++;
            print_key_function(current->key);
            printf(" : ");
            print_value_function(current->value);
            if (count>=table->elements_count) {

                printf(" }\n");
                return;
            }
            else {
                printf(" , ");
            }
            current=current->next;
        }
    }
    printf("}\n");
}

bool contains_key(hash_table * table, void *key) {
    if (key==NULL) {return NULL;}
    long hash=table->hash(key);
    int index = hash & (table->size-1);

    entry *current=(table->buckets[index]).head;
    while (current!=NULL) {
        if (table->equal(key, current->key)) {
            return true;
        }
        current=current->next;
    }
    return false;
}

void * get_value(hash_table * table, void *key) {
    if (key==NULL) {return NULL;}
    long hash=table->hash(key);
    int index = hash & (table->size-1);
    entry *current=(table->buckets[index]).head;
    while (current!=NULL) {
        if (table->equal(key, current->key)) {
            return current->value;
        }
        current=current->next;
    }
    return NULL;
}

void remove_key(hash_table *table, void * key) {
    if (key==NULL) {return;}
    long hash=table->hash(key);
    int index = hash & (table->size-1);

}

