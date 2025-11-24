#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>

void clearInputBuffer()
{
    while (getchar() != '\n');
}

void logTransaction(const char* action, long accountNumber, double amount) {
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

char* getAccountType(long accountNumber) {
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

void updateAccountFile(long accountNumber, char* name, int id, char* accountType, int pin, double balance) {
    char filename[100];
    sprintf(filename, "database/%ld.txt", accountNumber);
    
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error updating account file.\n");
        return;
    }
    
    fprintf(file, "Account Number: %ld\n", accountNumber);
    fprintf(file, "Name: %s\n", name);
    fprintf(file, "ID: %d\n", id);
    fprintf(file, "Account Type: %s\n", accountType);
    fprintf(file, "PIN: %d\n", pin);
    fprintf(file, "Balance: %.2f\n", balance);
    fclose(file);
}


#endif