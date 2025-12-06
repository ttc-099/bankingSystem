#ifndef DEPOSIT_H
#define DEPOSIT_H

#include <stdio.h>
#include <stdlib.h>
#include "helpers_DepositWithdraw.h"
#include <dirent.h>

// --------------------------------------------------
// Deposit money into an existing account
// --------------------------------------------------
void deposit() {
    char accInput[20];
    int pin;
    double depositAmount;
    double balance = 0.0;

    printf("\n=== DEPOSIT ===\n");

    // --------------------------------------------------
    // Early check: ensure database folder has account files
    // --------------------------------------------------
    DIR *dir = opendir("database");
    if (!dir) {
        printf("Database folder not found.\n");
        return;
    }

    struct dirent *entry;
    int fileFound = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".txt") != NULL && strcmp(entry->d_name, "index.txt") != 0) {
            fileFound = 1;
            break;
        }
    }
    closedir(dir);

    if (!fileFound) {
        printf("No accounts found.\n");
        return;
    }

    // --------------------------------------------------
    // Ask for account number and PIN
    // --------------------------------------------------
    printf("Enter Account Number: ");
    scanf("%19s", accInput);

    printf("Enter 4-digit PIN: ");
    scanf("%d", &pin);
    clearInputBuffer();

    long accNumber = atol(accInput);

    // --------------------------------------------------
    // Get current balance from account file
    // --------------------------------------------------
    if (getAccountBalance(accNumber, &balance) != 0) {
        printf("Error: Account not found.\n");
        return;
    }

    // --------------------------------------------------
    // Verify PIN stored in the account file
    // --------------------------------------------------
    char filename[100];
    sprintf(filename, "database/%ld.txt", accNumber);

    FILE *file = fopen(filename, "r");
    if (!file) return;

    int storedPIN = -1;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "PIN: %d", &storedPIN) == 1) break;
    }
    fclose(file);

    if (pin != storedPIN) {
        printf("Incorrect PIN.\n");
        return;
    }

    // --------------------------------------------------
    // Ask for deposit amount with validation
    // --------------------------------------------------
    do {
        printf("Enter amount to deposit (max RM50,000): RM");
        if (scanf("%lf", &depositAmount) != 1 || depositAmount <= 0 || depositAmount > 50000) {
            printf("Invalid amount. Try again.\n");
            clearInputBuffer();
        } else break;
    } while (1);
    clearInputBuffer();

    balance += depositAmount;

    // --------------------------------------------------
    // Update account balance in file
    // --------------------------------------------------
    if (updateAccountBalance(accNumber, balance) != 0) {
        printf("Error updating balance.\n");
        return;
    }

    // --------------------------------------------------
    // Success message + log transaction
    // --------------------------------------------------
    printf("Deposit successful! New balance: RM%.2f\n", balance);
    logTransaction("deposit", accNumber, depositAmount);
}

#endif
