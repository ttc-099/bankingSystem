#ifndef DELETE_ACCOUNT_H
#define DELETE_ACCOUNT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>       // for directory scanning
#include "helpers_CreateDelete.h"

// --- Delete an existing bank account ---
static void deleteAccount() {

    printf("\n=== DELETE ACCOUNT ===\n");

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
        printf("No accounts to delete.\n");
        return;
    }

    // --- Load index file ---
    FILE *indexFile = fopen("database/index.txt", "r");
    if (!indexFile) {
        printf("No accounts exist yet.\n");
        return;
    }

    char accounts[100][20]; // up to 100 accounts max
    int count = 0;

    while (fgets(accounts[count], sizeof(accounts[count]), indexFile)) {
        accounts[count][strcspn(accounts[count], "\n")] = '\0'; // remove newline
        count++;
    }
    fclose(indexFile);

    if (count == 0) {
        printf("No accounts to delete.\n");
        return;
    }

    // --- Display account list ---
    printf("\n--- Existing Accounts ---\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, accounts[i]);
    }

    // --- Ask which account to delete ---
    int choice;
    printf("\nEnter the number of the account to delete: ");
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > count) {
        printf("Invalid choice.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    char selectedAccount[20];
    strcpy(selectedAccount, accounts[choice - 1]);

    // --- Confirm deletion ---
    char enteredAccount[20];
    int last4ID, pin;

    printf("\nTo confirm deletion, enter the following:\n");
    printf("Account number: ");
    scanf("%19s", enteredAccount);
    clearInputBuffer();

    printf("Last 4 digits of your ID: ");
    if (scanf("%d", &last4ID) != 1) { clearInputBuffer(); return; }
    clearInputBuffer();

    printf("4-digit PIN: ");
    if (scanf("%d", &pin) != 1) { clearInputBuffer(); return; }
    clearInputBuffer();

    if (strcmp(selectedAccount, enteredAccount) != 0) {
        printf("\nError: Account number mismatch.\n");
        return;
    }

    // --- Delete account file ---
    char filename[100];
    sprintf(filename, "database/%s.txt", selectedAccount);

    if (remove(filename) == 0) {
        printf("\nAccount file deleted successfully.\n");

        // --- Update index file ---
        FILE *tempFile = fopen("database/temp.txt", "w");
        if (!tempFile) { printf("Error updating index file.\n"); return; }

        for (int i = 0; i < count; i++) {
            if (i != (choice - 1))
                fprintf(tempFile, "%s\n", accounts[i]);
        }
        fclose(tempFile);

        remove("database/index.txt");
        rename("database/temp.txt", "database/index.txt");

        printf("Index updated.\n");
        logTransaction("delete_account", atol(selectedAccount), 0.0);
    } else {
        printf("\nError: Could not delete account file.\n");
    }
}

#endif
