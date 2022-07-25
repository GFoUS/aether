#include "strutils.h"

str_allocator* str_allocator_get_global() {
    static str_allocator* globalAllocator = NULL;
    if (globalAllocator == NULL) {
        globalAllocator = str_allocator_create();
    }

    return globalAllocator;
}

str_allocator* str_allocator_create() {
    str_allocator* allocator = malloc(sizeof(str_allocator));
    memset(allocator, 0, sizeof(str_allocator));
    allocator->allocs = malloc(0);
    return allocator;
}

void str_allocator_destroy(str_allocator* allocator) {
    for (u32 i = 0; i < allocator->numAllocs; i++) {
        free(allocator->allocs[i]);
    }
    free(allocator->allocs);

    free(allocator);
}

char* str_allocator_allocate(str_allocator* allocator, size_t length) {
    allocator->numAllocs++;
    allocator->allocs = realloc(allocator->allocs, sizeof(void*) * allocator->numAllocs);
    allocator->allocs[allocator->numAllocs - 1] = malloc(sizeof(char) * (length + 1));
    memset(allocator->allocs[allocator->numAllocs - 1], 0, length + 1);
    return allocator->allocs[allocator->numAllocs - 1];
}

bool str_starts_with(const char* string, const char* search) {
    if (strncmp(string, search, strlen(search)) == 0) {
        return true;
    } else {
        return false;
    }
}

char* str_substr(str_allocator* allocator, const char* from, size_t offset, size_t length) {
    char* to = str_allocator_allocate(allocator, length);
    memcpy(to, &from[offset], length);
    return to;
} 

char* str_find(const char* string, const char* search) {
    return strchr(string, *search);
}

char* str_find_last(const char* string, const char* search) {
    return strrchr(string, *search);
}

u64 str_get_character_pos(const char* string, const char* character) {
    return character - string;
}

char* str_replace_all(str_allocator* allocator, const char* original, const char* replace, const char* with) {
    char* output = str_allocator_allocate(allocator, strlen(original));
    strcpy(output, original);
    for (u32 i = 0; i < strlen(original); i++) {
        if (original[i] == *replace) {
            output[i] = *with;
        } else {
            output[i] = original[i];
        }
    }

    return output;
}