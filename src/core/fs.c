#include "fs.h"

#include "nfd.h"
#include "log.h"
#include "strutils.h"

#include <string.h>
#include <stdlib.h>

const char* fs_open_dialog(u32 numFilters, fs_dialog_filter* filters, str_allocator* allocator) {
    char* path;
    nfdresult_t result = NFD_OpenDialog(&path, (const nfdu8filteritem_t*)filters, numFilters, NULL);
    if (result == NFD_CANCEL) {
        return NULL;
    } else if (result == NFD_OKAY) {
        char* pathCopy = str_replace_all(allocator, path, "\\", "/");
        return pathCopy;
    } else {
        FATAL("File dialog failed with error code: %d", result);
        return NULL;
    }
}

const char* fs_save_dialog(u32 numFilters, fs_dialog_filter* filters, const char* defaultName, str_allocator* allocator) {
    char* path;
    nfdresult_t result = NFD_SaveDialog(&path, (const nfdu8filteritem_t*)filters, numFilters, NULL, defaultName);
    if (result == NFD_CANCEL) {
        return NULL;
    } else if (result == NFD_OKAY) {
        char* pathCopy = str_replace_all(allocator, path, "\\", "/");
        return pathCopy;
    } else {
        FATAL("File dialog failed with error code: %d", result);
        return NULL;
    }
}

char* path_get_parent(str_allocator* allocator, const char* path) {
    u64 lastSlash = str_get_character_pos(path, str_find_last(path, "/"));
    return str_substr(allocator, path, 0, lastSlash);
}