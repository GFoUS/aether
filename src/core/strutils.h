#pragma once

#include "types.h"

#include <string.h>
#include <stdlib.h>
#include <memory.h>

typedef struct {
    u32 numAllocs;
    void** allocs;
} str_allocator;

str_allocator* str_allocator_get_global();
str_allocator* str_allocator_create();
void str_allocator_destroy(str_allocator* allocator);
char* str_allocator_allocate(str_allocator* allocator, size_t size);

u64 str_get_character_pos(const char* string, const char* character);

char* str_substr(str_allocator* allocator, const char* from, size_t offset, size_t length);

bool str_starts_with(const char* string, const char* search);
char* str_find(const char* string, const char* search);
char* str_find_last(const char* string, const char* search);
char* str_replace_all(str_allocator* allocator, const char* string, const char* replace, const char* with);