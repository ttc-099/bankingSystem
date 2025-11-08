#ifndef CREATE_ACCOUNT_H
#define CREATE_ACCOUNT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "helpers.h"

// 1. Setup files

long generateAccNumber() {
    long choices[] = {
        rand() % 9000000 + 1000000,
        rand() % 90000000 + 10000000,
        rand() % 900000000 + 100000000
    };
    return choices[rand() % 3];  // Randomly pick one of the three
}

// 2. Main
void createAccount() {
    char name[50];
    int id;
    int accountType;
    int pin; 
    long accountNumber;
    double balance = 0.00;

    srand(time(NULL)); // for the random number generator

    // 2.1 Name
    do {
        printf("Enter your full name: ");
        fgets(name, sizeof(name), stdin);
        
        // Remove newline
        name[strcspn(name, "\n")] = '\0';
        
        if (strlen(name) == 0) {
            printf("Invalid input. Name cannot be empty.\n");
            clearInputBuffer();  // Clear any extra input
        } else {
            break;
        }
    } while (1);

    // 2.2 ID
    do {
        printf("Enter your ID: ");
        if (scanf("%d", &id) != 1) {
            printf("Invalid input. Please enter numbers only.\n");
            clearInputBuffer();
        } else if (id <= 0) {
            printf("ID must be a positive number.\n");
        } else {
            break;
        }
    } while (1);

    // 2.3 Account Type 
    do {
        printf("Would you like a [1]Savings Account or [2]Current Account? ");
        if (scanf("%d", &accountType) != 1 || (accountType != 1 && accountType != 2))
        {
            printf("Invalid input. Enter 1 for Savings or 2 for Current.\n");
            clearInputBuffer();
        }
        else
            break;
    } while (1);

    // 2.4 PIN

    do {
        printf("Enter your 4-digit PIN: ");
        if (scanf("%d", &pin) != 1 || pin < 1000 || pin > 9999)
        {
            printf("Invalid PIN. Must be 4 digits.\n");
            clearInputBuffer();
        }
        else
            break;
     } while (1);

     // 2.5 Generate Account Number

     accountNumber = generateAccNumber();

     // 2.6 Save to File
    char filename[100];
    sprintf(filename, "database/%ld.txt", accountNumber);

    FILE *file = fopen(filename, "w");
    if (!file)
    {
        printf("\nError: Folder 'database' not found.\n");
        return;
    }

    fprintf(file, "Account Number: %ld\n", accountNumber);
    fprintf(file, "Name: %s\n", name);
    fprintf(file, "ID: %d\n", id);
    fprintf(file, "Account Type: %s\n", accountType == 1 ? "Savings" : "Current");
    fprintf(file, "PIN: %d\n", pin);
    fprintf(file, "Balance: %.2f\n", balance);
    fclose(file);

    printf("\nAccount created successfully!\n");
    printf("Account Number: %ld\n", accountNumber);
    printf("Saved to: %s\n", filename);

    //2.7 Update Index File
    FILE *indexFile = fopen("database/index.txt", "a");  // "a" = append
    if (indexFile)
    {
        fprintf(indexFile, "%ld\n", accountNumber);
        fclose(indexFile);
    }
    else
    {
        printf("Warning: Could not update index file.\n");
    }

}


#endif
