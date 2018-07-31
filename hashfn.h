#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct { size_t _i; } HashIndex;

HashIndex hashBytes(const uint8_t*, size_t);

/// 32 bit avalanche mixer
static inline HashIndex hash32(uint32_t x) {
    uint32_t y = 0xf5b2d64f, z = __builtin_bswap32(x * y);
    return (HashIndex) { (size_t)(__builtin_bswap32(z * z) * y) };
}

/// 64 bit avalanche mixer
static inline HashIndex hash64(uint64_t x) {
    uint64_t y = 0xf111865d95ca7731, z = __builtin_bswap64(x * y);
    return (HashIndex) { (size_t)(__builtin_bswap64(z * z) * y) };
}

static inline HashIndex hashWord(size_t x) {
    return sizeof (size_t) == 4 ? hash32(x) : hash64(x);
}

static inline HashIndex hashPtr(const void* p) {
    return hashWord((uintptr_t)p);
}
