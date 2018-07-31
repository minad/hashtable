#include <stdio.h>

typedef struct {
    const char* k;
    int         v;
} MyEntry;

#define HASH        MyHash
#define ENTRY       MyEntry
#define PREFIX      myhash
#define KEY(e)      e->k
#define EXISTS(e)   e->k
#define KEYEQ(a, b) !strcmp(a, b)
#define HASHFN(s)   hashBytes((const uint8_t*)(s), strlen(s))
#include "hash.h"

int main(void) {
    MyHash h = HASH_INIT;

    MyEntry* e;

    if (myhashCreate(&h, "a", &e)) {
        printf("ENTRY CREATED\n");
        e->k = "a";
        e->v = 1;
    }

    myhashCreate(&h, "b", &e);
    e->k = "b";
    e->v = 2;

    myhashCreate(&h, "c", &e);
    e->k = "c";
    e->v = 3;

    myhashCreate(&h, "c", &e);
    e->k = "c";
    e->v = 4;

    e = myhashFind(&h, "a");
    if (e)
        printf("FOUND a %d\n", e->v);
    else
        printf("NOT FOUND a\n");

    e = myhashFind(&h, "d");
    if (e)
        printf("FOUND d %d\n", e->v);
    else
        printf("NOT FOUND d\n");

    HASH_FOREACH(myhash, f, &h)
        printf("%s %d\n", f->k, f->v);
}
