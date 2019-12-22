#pragma once

#define bool _Bool
#define true 1
#define false 0
#define static_assert _Static_assert

typedef short int16_t;
typedef unsigned short uint16_t;
typedef uint16_t size_t;

typedef unsigned char uint8_t;
typedef signed char int8_t;

typedef signed int int32_t;
typedef unsigned int uint32_t;


typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;
typedef dword uintptr_t;
typedef int32_t intptr_t;


#define MULTILINE(...) #__VA_ARGS__

static_assert(sizeof(int16_t) == 2, "int16 isn't 2 bytes");
static_assert(sizeof(uint16_t) == 2, "uint16 isn't 2 bytes");

static_assert(sizeof(uint8_t) == 1, "uint8 isn't 1 byte");
static_assert(sizeof(int8_t) == 1, "int8 isn't 1 byte");

static_assert(sizeof(uint32_t) == 4, "uint32 isn't 4 bytes");
static_assert(sizeof(int32_t) == 4, "int32 isn't 4 bytes");

#define DEBUG 1

typedef union PtrUnion {
    const void* ptr;
    uint32_t dword;
    uint16_t words[2];
    uint8_t bytes[4];
} PtrUnion;

typedef union WordUnion {
    word word;
    byte bytes[2];
} WordUnion;
