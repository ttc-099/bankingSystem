#ifndef HELPERS_DEPOSITWITHDRAW_H
#define HELPERS_DEPOSITWITHDRAW_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "helpers_CreateDelete.h"  // for logTransaction and clearInputBuffer

// --- DATABASE BALANCE HELPERS ---

// Reads current balance from account file
static int getAccountBalance(long accountNumber, double *balance) {
    if (!balance) return -1;

    char filename[100];
    sprintf(filename, "database/%ld.txt", accountNumber);

    FILE *file = fopen(filename, "r");
    if (!file) return -1;

    char line[256];
    *balance = 0.0;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "Balance: %lf", balance) == 1) {
            break;
        }
    }

    fclose(file);
    return 0;
}

// Updates balance in account file
static int updateAccountBalance(long accountNumber, double newBalance) {
    char filename[100];
    sprintf(filename, "database/%ld.txt", accountNumber);

    FILE *file = fopen(filename, "r");
    if (!file) return -1;

    FILE *tempFile = fopen("database/temp.txt", "w");
    if (!tempFile) { fclose(file); return -1; }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "Balance:", 8) == 0)
            fprintf(tempFile, "Balance: %.2f\n", newBalance);
        else
            fputs(line, tempFile);
    }

    fclose(file);
    fclose(tempFile);

    remove(filename);
    rename("database/temp.txt", filename);
    return 0;
}

#endif
