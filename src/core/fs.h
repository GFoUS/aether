#pragma once

#include "types.h"
#include "strutils.h"

// file dialog
typedef struct {
    const char* name;
    const char* extension;
} fs_dialog_filter;

const char* fs_open_dialog(u32 numFilters, fs_dialog_filter* filters, str_allocator* allocator);
const char* fs_save_dialog(u32 numFilters, fs_dialog_filter* filters, const char* defaultName, str_allocator* allocator);
char* path_get_parent(str_allocator* allocator, const char* path);