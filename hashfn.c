#include "hashfn.h"
#define XXH_INLINE_ALL
#include "xxhash.h"

HashIndex hashBytes(const uint8_t* p, size_t n) {
    return (HashIndex) { sizeof (long) == 4 ? XXH32(p, n, 0) : (uint32_t)XXH64(p, n, 0) };
}
