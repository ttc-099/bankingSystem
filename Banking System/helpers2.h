#ifndef HELPERS2_H
#define HELPERS2_H

#include <stdio.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#define MKDIR(path) mkdir(path, 0777)
#endif

void createDatabaseDir();
void logAction(const char* action);
int getAccountCount(int *out_count);

#endif
