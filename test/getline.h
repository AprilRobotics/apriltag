#pragma once

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

typedef intptr_t ssize_t;

ssize_t getline(char **lineptr, size_t *n, FILE *stream);
