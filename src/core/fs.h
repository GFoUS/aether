#pragma once

#include "types.h"

// file dialog
typedef struct {
    const char* name;
    const char* extension;
} fs_dialog_filter;

const char* fs_open_dialog(u32 numFilters, fs_dialog_filter* filters);
const char* fs_save_dialog(u32 numFilters, fs_dialog_filter* filters, const char* defaultName);
