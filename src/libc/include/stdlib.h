#pragma once

#include <stddef.h>

__attribute__((noreturn)) void abort(void);
__attribute__((noreturn)) void exit(int status);

void* malloc(size_t size);
void free(void* ptr);

