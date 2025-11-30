#ifndef WITHDRAW_H
#define WITHDRAW_H

#include <stdio.h>
#include <stdlib.h>
#include "helpers_DepositWithdraw.h"
#include <dirent.h>

void withdraw() {
    long accNumber;
    int pin;
    double amount;
    double balance = 0.0;

    printf("\n=== WITHDRAWAL ===\n");

        // --- Early check: does database folder contain any .txt account files? ---
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

    printf("Enter Account Number: ");
    if (scanf("%ld", &accNumber) != 1) { 
        printf("Invalid input.\n"); 
        clearInputBuffer(); 
        return; 
    }

    printf("Enter 4-digit PIN: ");
    if (scanf("%d", &pin) != 1) {
        printf("Invalid PIN.\n"); 
        clearInputBuffer(); 
        return; 
    }
    clearInputBuffer();

    if (getAccountBalance(accNumber, &balance) != 0) {
        printf("Account not found.\n");
        return;
    }

    // Verify PIN
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

    printf("Current balance: RM%.2f\n", balance);

    printf("Enter amount to withdraw: RM");
    if (scanf("%lf", &amount) != 1 || amount <= 0 || amount > balance) {
        printf("Invalid or insufficient amount.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    balance -= amount;

    if (updateAccountBalance(accNumber, balance) != 0) {
        printf("Error updating balance.\n");
        return;
    }

    printf("Withdrawal successful! New balance: RM%.2f\n", balance);
    logTransaction("withdrawal", accNumber, amount);
}

#endif
