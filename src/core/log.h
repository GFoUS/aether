#pragma once

#include <stdio.h>
#include <assert.h>

#define DEBUG(...) printf("DEBUG - %s:%d - ", __FILE__, __LINE__); printf(__VA_ARGS__); printf("\n")
#define INFO(...) printf("INFO - %s:%d - ", __FILE__, __LINE__); printf(__VA_ARGS__); printf("\n")
#define WARN(...) printf("WARN - %s:%d - ", __FILE__, __LINE__); printf(__VA_ARGS__); printf("\n")
#define ERROR(...) printf("ERROR - %s:%d - ", __FILE__, __LINE__); printf(__VA_ARGS__); printf("\n")
#define FATAL(...) printf("FATAL - %s:%d - ", __FILE__, __LINE__); printf(__VA_ARGS__); printf("\n"); assert(0)