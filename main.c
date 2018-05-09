#include <stdio.h>

typedef struct {
    const char* k;
    int         v;
} Entry;

#define HASH        Hash
#define ENTRY       Entry
#define PREFIX      hash
#define KEY(e)      e->k
#define EXISTS(e)   e->k
#define KEYEQ(a, b) !strcmp(a, b)
#define HASHFN      hashString
#include "hash.h"

int main(void) {
    Hash hash = HASH_INIT;

    Entry* e;

    hashCreate(&hash, "a", &e);
    e->k = "a";
    e->v = 1;

    hashCreate(&hash, "b", &e);
    e->k = "b";
    e->v = 2;

    hashCreate(&hash, "c", &e);
    e->k = "c";
    e->v = 3;

    hashCreate(&hash, "c", &e);
    e->k = "c";
    e->v = 4;

    e = hashFind(&hash, "a");
    if (e)
        printf("FOUND a %d\n", e->v);
    else
        printf("NOT FOUND a\n");

    e = hashFind(&hash, "d");
    if (e)
        printf("FOUND d %d\n", e->v);
    else
        printf("NOT FOUND d\n");

    HASH_FOREACH(hash, f, &hash)
        printf("%s %d\n", f->k, f->v);
}
