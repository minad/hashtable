#include "hashfn.h"
#include <assert.h>
#include <stdbool.h>
#include <malloc.h>

#define LIKELY(x)     __builtin_expect((x), 1)
#define UNLIKELY(x)   __builtin_expect((x), 0)
#define _CAT(x, y)    x##y
#define CAT(x, y)     _CAT(x, y)
#define CREATE        CAT(PREFIX, Create)
#define DESTROY       CAT(PREFIX, Destroy)
#define FIND          CAT(PREFIX, Find)
#define FINDPOS       CAT(PREFIX, FindPos)
#define GROW          CAT(PREFIX, Grow)
#define INDEX         CAT(PREFIX, Index)
#define INSERT        CAT(PREFIX, Insert)
#define INSERTPOS     CAT(PREFIX, InsertPos)
#define KEYTYPE       const typeof (KEY(((ENTRY*)0)))
#define NEXT          CAT(PREFIX, Next)

#ifndef CALLOC
#  define CALLOC(h, x) calloc((x), 1)
#  define FREE(h, x)   free(x)
#endif
#ifndef MAXFILL
#  define MAXFILL 80
#endif
#ifndef INITSHIFT
#  define INITSHIFT 8
#endif
#ifndef EXISTS
#  define EXISTS KEY
#endif
#ifndef KEYEQ
#  define KEYEQ(a, b) (a == b)
#endif
#ifndef HASHFN
#  define HASHFN(k) hashPtr(k)
#endif

#define INVARIANT                                 \
    ({                                            \
        assert(h != 0);                           \
        assert((!h->entry) == (!h->size));        \
        assert(h->count <= h->size);              \
    })
#define FULL      (h->count >= MAXFILL * h->size / 100)
#define HASH_INIT { 0, 0, 0 }
#define HASH_FOREACH(p, e, h) for (typeof(CAT(p, Next)((h), (h)->entry)) e = CAT(p, Next)((h), (h)->entry); e; e = CAT(p, Next)((h), e + 1))
#define HASH_AUTO(h, n) h n __attribute__ ((cleanup(CAT(h, _auto_destroy)))) = HASH_INIT

#ifndef NOHASH
typedef struct {
    ENTRY* entry;
    size_t count, size;
} HASH;
#endif

static inline HashIndex INDEX(KEYTYPE k) {
    return (HashIndex){ HASHFN(k) };
}

static inline void DESTROY(HASH* h) {
    INVARIANT;
    FREE(h, h->entry);
    h->entry = 0;
    h->size = h->count = 0;
}

static inline void CAT(HASH, _auto_destroy)(HASH* h) {
    DESTROY(h);
}

static inline ENTRY* NEXT(const HASH* h, ENTRY* e) {
    INVARIANT;
    for (; e < h->entry + h->size; ++e) {
        bool x = EXISTS(e);
        if (x)
            return e;
    }
    return 0;
}

static inline ENTRY* INSERTPOS(HASH* h, HashIndex i) {
    INVARIANT;
    assert(h && h->entry);
    for (size_t n = 0; ; ++n) {
        size_t a = (i.i + n) & (h->size - 1);
        ENTRY* e = h->entry + a;
        bool x = EXISTS(e);
        if (!x)
            return e;
    }
}

static void GROW(HASH* h) {
    INVARIANT;
    HASH l = *h;
    l.size = h->size ? 2 * h->size : (1UL << INITSHIFT);
    size_t n = sizeof (ENTRY) * l.size;
    l.entry = (ENTRY*)CALLOC(h, n);
    HASH_FOREACH(PREFIX, e, h)
        memcpy(INSERTPOS(&l, INDEX(KEY(e))), e, sizeof (ENTRY));
    DESTROY(h);
    *h = l;
}

static inline ENTRY* INSERT(HASH* h, HashIndex i) {
    INVARIANT;
    if (UNLIKELY(FULL))
        GROW(h);
    ++h->count;
    return INSERTPOS(h, i);
}

static inline bool FINDPOS(const HASH* h, KEYTYPE k, HashIndex i, ENTRY** ret) {
    INVARIANT;
    if (UNLIKELY(!h->entry)) {
        *ret = 0;
        return false;
    }
    for (size_t n = 0; ; ++n) {
        size_t a = (i.i + n) & (h->size - 1);
        ENTRY* e = h->entry + a;
        bool x = EXISTS(e);
        if (!x) {
            *ret = e;
            return false;
        }
        KEYTYPE l = KEY(e);
        bool q = KEYEQ(l, k);
        if (q) {
            *ret = e;
            return true;
        }
    }
}

static inline ENTRY* FIND(const HASH* h, KEYTYPE k) {
    ENTRY* e;
    return FINDPOS(h, k, INDEX(k), &e) ? e : 0;
}

static inline bool CREATE(HASH* h, KEYTYPE k, ENTRY** ret) {
    INVARIANT;
    HashIndex i = INDEX(k);
    ENTRY* e;
    if (FINDPOS(h, k, i, &e)) {
        *ret = e;
        return false;
    }
    if (LIKELY(e && !FULL))
        ++h->count;
    else
        e = INSERT(h, i);
    *ret = e;
    return true;
}

#undef CALLOC
#undef CREATE
#undef DESTROY
#undef ENTRY
#undef EXISTS
#undef FIND
#undef FINDPOS
#undef FREE
#undef FULL
#undef GROW
#undef HASH
#undef HASHFN
#undef INDEX
#undef INITSHIFT
#undef INSERT
#undef INSERT
#undef INVARIANT
#undef KEY
#undef KEYEQ
#undef KEYTYPE
#undef MAXFILL
#undef NEXT
#undef NOHASH
#undef PREFIX
