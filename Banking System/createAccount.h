#ifndef CREATE_ACCOUNT_H
#define CREATE_ACCOUNT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h> 
#include "helpers_CreateDelete.h"
#include "helpers3.h"   // contains isValidIC()

// ----------------------------------------------------------
// Generate a UNIQUE random account number (7–9 digits)
// ----------------------------------------------------------
static long generateAccNumber() {
    long accNumber;
    FILE *indexFile;
    char line[20];
    int unique;

    srand((unsigned int)time(NULL));

    do {
        // Generate a number between 7 and 9 digits
        int digits = (rand() % 3) + 7;  // 7, 8, or 9
        long min = (long) pow(10, digits - 1);
        long max = (long) pow(10, digits) - 1;

        accNumber = (rand() % (max - min + 1)) + min;

        unique = 1;

        indexFile = fopen("database/index.txt", "r");
        if (indexFile != NULL) {
            while (fgets(line, sizeof(line), indexFile)) {
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

// NAME
while (1) {
    printf("Enter full name (letters, spaces, and hyphens only): ");
    if (fgets(newAccount.name, sizeof(newAccount.name), stdin) == NULL) continue;

    newAccount.name[strcspn(newAccount.name, "\n")] = '\0';

    int valid = 1;
    for (int i = 0; newAccount.name[i] != '\0'; i++) {
        char c = newAccount.name[i];
        if (!isalpha(c) && c != ' ' && c != '-') {
            valid = 0;
            break;
        }
    }

    if (!valid || strlen(newAccount.name) == 0) {
        printf("Invalid name.\n");
        continue;
    }
    break;
}


    // IC
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

    // ACCOUNT TYPE
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

    // PIN
    char pinInput[10];
    int pin;
    while (1) {
        printf("Create a 4-digit PIN (1000-9999): ");
        if (fgets(pinInput, sizeof(pinInput), stdin) == NULL) continue;
        pinInput[strcspn(pinInput, "\n")] = '\0';

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

    // ACCOUNT NUMBER
    newAccount.accountNumber = generateAccNumber();

    // WRITE FILE
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
    fprintf(file, "Account Type: %s\n",
            newAccount.accountType == 1 ? "Savings" : "Current");
    fprintf(file, "PIN: %d\n", newAccount.pin);
    fprintf(file, "Balance: %.2f\n", newAccount.balance);
    fclose(file);

    // UPDATE INDEX
    FILE *indexFile = fopen("database/index.txt", "a");
    if (indexFile != NULL) {
        fprintf(indexFile, "%ld\n", newAccount.accountNumber);
        fclose(indexFile);
    } else {
        printf("Warning: Unable to update index file.\n");
    }

    // LOG
    logTransaction("create_account", newAccount.accountNumber, 0.0);

    // DONE
    printf("\nAccount created successfully.\n");
    printf("Assigned Account Number: %ld\n", newAccount.accountNumber);
    printf("Record saved to: %s\n", filename);
}

#endif
