#ifndef DELETE_ACCOUNT_H
#define DELETE_ACCOUNT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"


// --- USE --- Delete an existing bank account
void deleteAccount()
{

    printf("\n=== DELETE ACCOUNT ===\n");

    FILE *indexFile = fopen("database/index.txt", "r");
    if (!indexFile)
    {
        printf("No index file found! (No accounts exist yet)\n");
        return;
    }

    // --- USE --- Read all account numbers
    char accounts[100][20]; // up to 100 accs max
    int count = 0;
    while (fgets(accounts[count], sizeof(accounts[count]), indexFile))
    {
        accounts[count][strcspn(accounts[count], "\n")] = '\0'; // remove newline
        count++;
    }
    fclose(indexFile);

    if (count == 0)
    {
        printf("No accounts to delete.\n");
        return;
    }

    // --- USE --- Display account list
    printf("\n--- Existing Accounts ---\n");
    for (int i = 0; i < count; i++)
    {
        printf("%d. %s\n", i + 1, accounts[i]);
    }

    // --- USE --- Ask user to choose which to delete
    int choice;
    printf("\nEnter the number of the account to delete: ");
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > count)
    {
        printf("Invalid choice.\n");
        clearInputBuffer();
        return;
    }

    char selectedAccount[20];
    strcpy(selectedAccount, accounts[choice - 1]);

    // --- USE --- Confirm identity
    char enteredAccount[20];
    int last4ID, pin;
    printf("\nTo confirm deletion, please enter the following:\n");

    printf("Account number: ");
    scanf("%s", enteredAccount);

    printf("Last 4 digits of your ID: ");
    scanf("%d", &last4ID);

    printf("4-digit PIN: ");
    scanf("%d", &pin);

    // --- USE --- Check account match
    if (strcmp(selectedAccount, enteredAccount) != 0)
    {
        printf("\nError: Account number mismatch.\n");
        return;
    }

    // --- USE --- Delete account file
    char filename[100];
    sprintf(filename, "database/%s.txt", selectedAccount);

    if (remove(filename) == 0)
    {
        printf("\nAccount file deleted successfully.\n");

        // --- USE --- Update index file (rewrite without deleted account)
        FILE *tempFile = fopen("database/temp.txt", "w");
        if (!tempFile)
        {
            printf("Error updating index file.\n");
            return;
        }

        for (int i = 0; i < count; i++)
        {
            if (i != (choice - 1))
                fprintf(tempFile, "%s\n", accounts[i]);
        }

        fclose(tempFile);
        remove("database/index.txt");
        rename("database/temp.txt", "database/index.txt");

        printf("Index updated.\n");
    }
    else
    {
        printf("\nError: Could not delete file (maybe it doesn't exist?).\n");
    }

    logTransaction("delete_account", atol(selectedAccount), 0.00);
}

#endif
