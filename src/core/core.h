#pragma once

#include "event.h"
#include "log.h"
#include "types.h"
#include "fs.h"
#include "maths.h"
#include "strutils.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define CLEAR_MEMORY(data) memset(data, 0, sizeof(*data))
#define CLEAR_MEMORY_ARRAY(data, length) memset(data, 0, sizeof(*data) * length)
#define MALLOC(size) malloc(size)
#define REALLOC(data, size) data = realloc(data, size)

#define ARRAY_SIZE(array) sizeof(array) / sizeof(*array)
