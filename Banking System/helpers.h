#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>

void clearInputBuffer()
{
    while (getchar() != '\n');
}

#endif