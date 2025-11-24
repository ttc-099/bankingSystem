#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>  // for _mkdir on Windows
#define MKDIR(path) _mkdir(path)
#endif

// Ensure "database" directory exists
void createDatabaseDir() {
    MKDIR("database"); // works on both Windows and Linux
}

// Log an action with timestamp
void logAction(const char* action) {
    createDatabaseDir();

    FILE *logFile = fopen("database/transaction.log", "a");
    if (!logFile) {
        printf("ERROR: Could not open log file.\n");
        return;
    }

    time_t now = time(NULL);
    char timestamp[26];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

    fprintf(logFile, "[%s] - %s\n", timestamp, action);
    fclose(logFile);
}

// Count numeric .txt files in "database"
int getAccountCount(int *out_count) {
    if (!out_count) return -1; // pointer check

    DIR *dir = opendir("database");
    if (!dir) { 
        *out_count = 0; 
        return 0; 
    }

    int count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        int len = strlen(entry->d_name);

        // Check if filename is like "123.txt"
        if (len > 4 && strcmp(entry->d_name + len - 4, ".txt") == 0) {
            int isNumber = 1;
            for (int i = 0; i < len - 4; i++)
                if (!isdigit(entry->d_name[i])) { isNumber = 0; break; }

            if (isNumber) count++;
        }
    }

    closedir(dir);
    *out_count = count;
    return 0;
}
