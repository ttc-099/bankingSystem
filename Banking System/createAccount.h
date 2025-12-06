#ifndef CREATE_ACCOUNT_H
#define CREATE_ACCOUNT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "helpers_CreateDelete.h"
#include "helpers3.h"   // contains isValidIC()

// ----------------------------------------------------------
// Generate a random 9-digit account number that is unique
// ----------------------------------------------------------
static long generateAccNumber() {
    long accNumber;
    FILE *indexFile;
    char line[20];
    int unique;

    srand((unsigned int)time(NULL));

    do {
        accNumber = rand() % 900000000 + 100000000; // 9-digit
        unique = 1;

        indexFile = fopen("database/index.txt", "r");
        if (indexFile != NULL) {
            while (fgets(line, sizeof(line), indexFile)) {
                line[strcspn(line, "\n")] = '\0';
                if (atol(line) == accNumber) {
                    unique = 0;
                    break;
                }
            }
            fclose(indexFile);
        }
    } while (!unique);

    return accNumber;
}

// ----------------------------------------------------------
// Create a new bank account
// ----------------------------------------------------------
static void createAccount() {
    Account newAccount;
    newAccount.balance = 0.0;

    printf("\n=== CREATE NEW BANK ACCOUNT ===\n");

    // --- Name ---
    while (1) {
        printf("Enter full name (letters and spaces only): ");
        if (fgets(newAccount.name, sizeof(newAccount.name), stdin) == NULL) continue;
        newAccount.name[strcspn(newAccount.name, "\n")] = '\0';

        int valid = 1;
        for (int i = 0; newAccount.name[i] != '\0'; i++) {
            if (isdigit(newAccount.name[i])) { valid = 0; break; }
        }
        if (!valid || strlen(newAccount.name) == 0) {
            printf("Invalid name.\n");
            continue;
        }
        break;
    }

    // --- IC ---
    char icInput[20];
    while (1) {
        printf("Enter Malaysian IC (12 digits, no hyphens): ");
        if (fgets(icInput, sizeof(icInput), stdin) == NULL) continue;
        icInput[strcspn(icInput, "\n")] = '\0';

        if (!isValidIC(icInput)) {
            printf("Invalid IC format or state code.\n");
            continue;
        }
        strncpy(newAccount.id, icInput, sizeof(newAccount.id) - 1);
        newAccount.id[sizeof(newAccount.id) - 1] = '\0';

        break;
    }

    // --- Account type ---
    int type;
    while (1) {
        printf("Select account type [1] Savings  [2] Current: ");
        if (scanf("%d", &type) != 1 || (type != 1 && type != 2)) {
            printf("Invalid selection.\n");
            clearInputBuffer();
            continue;
        }
        newAccount.accountType = type;
        clearInputBuffer();
        break;
    }

    // --- PIN (strictly 4-digit numeric) ---
    char pinInput[10];
    int pin;
    while (1) {
        printf("Create a 4-digit PIN (1000-9999): ");
        if (fgets(pinInput, sizeof(pinInput), stdin) == NULL) continue;
        pinInput[strcspn(pinInput, "\n")] = '\0';

        // Check if all digits
        int valid = 1;
        for (int i = 0; pinInput[i] != '\0'; i++) {
            if (!isdigit(pinInput[i])) { valid = 0; break; }
        }

        if (!valid || strlen(pinInput) != 4) {
            printf("Invalid PIN.\n");
            continue;
        }

        pin = atoi(pinInput);
        if (pin < 1000 || pin > 9999) {
            printf("Invalid PIN.\n");
            continue;
        }

        newAccount.pin = pin;
        break;
    }

    // --- Generate account number ---
    newAccount.accountNumber = generateAccNumber();

    // --- Write account file ---
    char filename[100];
    sprintf(filename, "database/%ld.txt", newAccount.accountNumber);

    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: Unable to create account file.\n");
        return;
    }

    fprintf(file, "Account Number: %ld\n", newAccount.accountNumber);
    fprintf(file, "Name: %s\n", newAccount.name);
    fprintf(file, "ID: %s\n", newAccount.id);
    fprintf(file, "Account Type: %s\n", newAccount.accountType == 1 ? "Savings" : "Current");
    fprintf(file, "PIN: %d\n", newAccount.pin);
    fprintf(file, "Balance: %.2f\n", newAccount.balance);
    fclose(file);

    // --- Append to index file ---
    FILE *indexFile = fopen("database/index.txt", "a");
    if (indexFile != NULL) {
        fprintf(indexFile, "%ld\n", newAccount.accountNumber);
        fclose(indexFile);
    } else {
        printf("Warning: Unable to update index file.\n");
    }

    logTransaction("create_account", newAccount.accountNumber, 0.0);

    printf("\nAccount created successfully.\n");
    printf("Assigned Account Number: %ld\n", newAccount.accountNumber);
    printf("Record saved to: %s\n", filename);
}

#endif
