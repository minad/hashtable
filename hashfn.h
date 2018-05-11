#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct { uint32_t _i; } HashIndex;

static inline uint32_t hashMurmur32(uint32_t x) {
    x ^= x >> 16;
    x *= 0x85ebca6b;
    x ^= x >> 13;
    x *= 0xc2b2ae35;
    x ^= x >> 16;
    return x;
}

static inline uint32_t hashMurmur64(uint64_t x) {
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccd;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53;
    x ^= x >> 33;
    return (uint32_t)x;
}

/// Thomas Wang
static inline uint32_t hashWang32(uint32_t x) {
    x += ~(x<<15);
    x ^=  (x>>10);
    x +=  (x<<3);
    x ^=  (x>>6);
    x += ~(x<<11);
    x ^=  (x>>16);
    return x;
}

/// Thomas Wang
static inline uint32_t hashWang64(uint64_t x) {
    x += ~(x<<32);
    x ^=  (x>>22);
    x += ~(x<<13);
    x ^=  (x>>8);
    x +=  (x<<3);
    x ^=  (x>>15);
    x += ~(x<<27);
    x ^=  (x>>31);
    return (uint32_t)x;
}

/// Fowler-Noll-Vo
static inline uint32_t hashFNV(const uint8_t* p, size_t n, bool zeroTerminated) {
    uint32_t h = 16777619U;
    for (size_t i = 0; zeroTerminated ? p[i] : i < n; ++i)
        h = (h * 2166136261U) ^ p[i];
    return h;
}

static inline uint32_t hashUInt32(uint32_t x) {
    return hashMurmur32(x);
}

static inline uint32_t hashUInt64(uint64_t x) {
    return hashMurmur64(x);
}

static inline uint32_t hashBytes(const uint8_t* p, size_t n) {
    return hashFNV(p, n, false);
}

static inline uint32_t hashPtr(const void* p) {
    return hashUInt64((uint64_t)p);
}

static inline uint32_t hashString(const char* p) {
    return hashFNV((const uint8_t*)p, 0, true);
}
