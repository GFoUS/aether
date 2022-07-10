#include "fs.h"

#include "nfd.h"
#include "log.h"

const char* fs_open_dialog(u32 numFilters, fs_dialog_filter* filters) {
    char* path;
    nfdresult_t result = NFD_OpenDialog(&path, (const nfdu8filteritem_t*)filters, numFilters, NULL);
    if (result == NFD_CANCEL) {
        return NULL;
    } else if (result == NFD_OKAY) {
        return path;
    } else {
        FATAL("File dialog failed with error code: %d", result);
        return NULL;
    }
}

const char* fs_save_dialog(u32 numFilters, fs_dialog_filter* filters, const char* defaultName) {
    char* path;
    nfdresult_t result = NFD_SaveDialog(&path, (const nfdu8filteritem_t*)filters, numFilters, NULL, defaultName);
    if (result == NFD_CANCEL) {
        return NULL;
    } else if (result == NFD_OKAY) {
        return path;
    } else {
        FATAL("File dialog failed with error code: %d", result);
        return NULL;
    }
}