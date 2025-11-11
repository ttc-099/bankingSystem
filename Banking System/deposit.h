#ifndef DEPOSIT_H
#define DEPOSIT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"

// --- HOW TO USE ---
// deposit() lets a user log in using their account number + PIN,
// then add money to their balance (0 < amount ≤ 50000).
// The balance is updated directly in their account file.

void deposit()
{
    char accountNumber[20];
    int pin;
    double depositAmount;
    double balance = 0.0;

    // --- ASK FOR ACCOUNT + PIN ---
    printf("\nEnter your Account Number: ");
    scanf("%s", accountNumber);

    printf("Enter your 4-digit PIN: ");
    scanf("%d", &pin);

    // --- OPEN ACCOUNT FILE ---
    char filename[100];
    sprintf(filename, "database/%s.txt", accountNumber);

    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error: Account not found.\n");
        return;
    }

    // --- VERIFY PIN + GET CURRENT BALANCE ---
    char line[100];
    int storedPIN = 0;
    int foundPIN = 0;

    while (fgets(line, sizeof(line), file))
    {
        if (sscanf(line, "PIN: %d", &storedPIN) == 1)
            foundPIN = 1;

        if (sscanf(line, "Balance: %lf", &balance) == 1)
            ; // do nothing
    }

    fclose(file);

    if (!foundPIN || storedPIN != pin)
    {
        printf("Error: Incorrect PIN.\n");
        return;
    }

    // --- ASK FOR DEPOSIT AMOUNT ---
    do
    {
        printf("Enter amount to deposit (max RM50,000): RM");
        if (scanf("%lf", &depositAmount) != 1 || depositAmount <= 0 || depositAmount > 50000)
        {
            printf("Invalid amount. Try again.\n");
            clearInputBuffer();
        }
        else
            break;
    } while (1);

    // --- UPDATE BALANCE ---
    balance += depositAmount;

    // --- REWRITE ACCOUNT FILE WITH NEW BALANCE ---
    FILE *inFile = fopen(filename, "r");
    FILE *tempFile = fopen("database/temp.txt", "w");
    if (!inFile || !tempFile)
    {
        printf("Error updating balance.\n");
        if (inFile) fclose(inFile);
        if (tempFile) fclose(tempFile);
        return;
    }

    while (fgets(line, sizeof(line), inFile))
    {
        if (strncmp(line, "Balance:", 8) == 0)
            fprintf(tempFile, "Balance: %.2f\n", balance);
        else
            fputs(line, tempFile);
    }

    fclose(inFile);
    fclose(tempFile);

    remove(filename);
    rename("database/temp.txt", filename);

    printf("\nDeposit successful!\n");
    printf("New balance: RM%.2f\n", balance);
}

#endif
