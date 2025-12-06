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

    // --- Early validation: check if the database folder exists & contains account files ---
    DIR *dir = opendir("database");
    if (!dir) {
        printf("Database folder not found.\n");
        return;
    }

    struct dirent *entry;
    int fileFound = 0;

    // Look for any .txt file that is not index.txt (meaning at least one account exists)
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

    // --- Ask for account number ---
    printf("Enter Account Number: ");
    if (scanf("%ld", &accNumber) != 1) {
        printf("Invalid input.\n");
        clearInputBuffer();  // remove leftover input from buffer
        return;
    }

    // --- Ask for PIN ---
    printf("Enter 4-digit PIN: ");
    if (scanf("%d", &pin) != 1) {
        printf("Invalid PIN.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer(); // clears newline if user presses enter

    // --- Retrieve current balance from account file ---
    if (getAccountBalance(accNumber, &balance) != 0) {
        printf("Account not found.\n");
        return;
    }

    // --- Verify PIN by reading stored PIN inside the account file ---
    char filename[100];
    sprintf(filename, "database/%ld.txt", accNumber);
    FILE *file = fopen(filename, "r");
    if (!file) return;

    int storedPIN = -1;
    char line[256];

    // Find the line beginning with "PIN:" and extract PIN
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "PIN: %d", &storedPIN) == 1) break;
    }
    fclose(file);

    // Compare entered PIN with stored PIN
    if (pin != storedPIN) {
        printf("Incorrect PIN.\n");
        return;
    }

    // --- Show current balance before withdrawal ---
    printf("Current balance: RM%.2f\n", balance);

    // --- Amount user wants to withdraw ---
    printf("Enter amount to withdraw: RM");
    if (scanf("%lf", &amount) != 1 || amount <= 0 || amount > balance) {
        printf("Invalid or insufficient amount.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    // --- Deduct amount from balance ---
    balance -= amount;

    // --- Save updated balance back into the account file ---
    if (updateAccountBalance(accNumber, balance) != 0) {
        printf("Error updating balance.\n");
        return;
    }

    // --- Confirm to user ---
    printf("Withdrawal successful! New balance: RM%.2f\n", balance);

    // --- Log transaction in transaction history ---
    logTransaction("withdrawal", accNumber, amount);
}

#endif
