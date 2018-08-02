#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct { size_t _i; } HashIndex;

HashIndex hashBytes(const uint8_t*, size_t);

/// Murmur avalanche
static inline HashIndex hash32(uint32_t x) {
    x ^= x >> 16;
    x *= 0x85ebca6b;
    x ^= x >> 13;
    x *= 0xc2b2ae35;
    x ^= x >> 16;
    return (HashIndex){ x };
}

/// Murmur avalanche
static inline HashIndex hash64(uint64_t x) {
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccd;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53;
    x ^= x >> 33;
    return (HashIndex){ x };
}

static inline HashIndex hashPtr(const void* p) {
    return sizeof (uintptr_t) == 4 ? hash32((uintptr_t)p) : hash64((uintptr_t)p);
}
