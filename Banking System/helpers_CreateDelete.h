#ifndef HELPERS_CREATEDELETE_H
#define HELPERS_CREATEDELETE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <dirent.h>

#ifdef _WIN32
#include <direct.h>  // for _mkdir
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#define MKDIR(path) mkdir(path, 0777)
#endif

// -----------------------------
// ACCOUNT STRUCT
// -----------------------------
typedef struct {
    char name[50];
    char id[20];       // IC as string (12 digits + null terminator)
    int accountType;
    int pin;
    long accountNumber;
    double balance;
} Account;

// -----------------------------
// DATABASE HELPERS
// -----------------------------
static void createDatabaseDir() {
    MKDIR("database");
}

static void clearInputBuffer() {
    while (getchar() != '\n');
}

static void logTransaction(const char* action, long accountNumber, double amount) {
    createDatabaseDir();
    FILE *logFile = fopen("database/transaction.log", "a");
    if (!logFile) return;

    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

    fprintf(logFile, "[%s] %s - Account: %ld - Amount: RM %.2f\n",
            timestamp, action, accountNumber, amount);
    fclose(logFile);
}

static int getAccountCount(int *out_count) {
    if (!out_count) return -1;

    createDatabaseDir();

    DIR *dir = opendir("database");
    if (!dir) { *out_count = 0; return 0; }

    int count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        int len = strlen(entry->d_name);
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

// -----------------------------
// ACCOUNT FILE HELPERS
// -----------------------------
static char* getAccountType(long accountNumber) {
    static char accountType[20];
    char filename[100];
    sprintf(filename, "database/%ld.txt", accountNumber);

    FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Account Type:") != NULL) {
            sscanf(line, "Account Type: %19[^\n]", accountType);
            break;
        }
    }
    fclose(file);
    return accountType;
}

static void updateAccountFile(long accountNumber, char* name, char* id, char* accountType, int pin, double balance) {
    char filename[100];
    sprintf(filename, "database/%ld.txt", accountNumber);

    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error updating account file.\n");
        return;
    }

    fprintf(file, "Account Number: %ld\n", accountNumber);
    fprintf(file, "Name: %s\n", name);
    fprintf(file, "ID: %s\n", id);
    fprintf(file, "Account Type: %s\n", accountType);
    fprintf(file, "PIN: %d\n", pin);
    fprintf(file, "Balance: %.2f\n", balance);
    fclose(file);
}

#endif
