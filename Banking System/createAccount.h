#ifndef CREATE_ACCOUNT_H
#define CREATE_ACCOUNT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 1. Setup files ================================================================

// helper to clear input buffer
void clearInputBuffer()
{
    while (getchar() != '\n');
}

// helper to generate random 7–9 digit account number
long generateAccountNumber()
{
    int length = (rand() % 3) + 7;  // 7, 8, or 9 digits
    long number = 0;

    for (int i = 0; i < length; i++)
    {
        number = number * 10 + (rand() % 10);
    }

    return number;
}

// 2. Main ======================================================================

void createAccount()
{
    char name[50];
    int id;
    int accountType;
    int pin;
    long accountNumber;
    double balance = 0.00;

    srand(time(NULL));

    // ----- NAME -----
    do
    {
        printf("Enter your full name: ");
        getchar();
        fgets(name, sizeof(name), stdin);

        size_t len = strlen(name);
        if (len > 0 && name[len - 1] == '\n')
            name[len - 1] = '\0';

        if (strlen(name) == 0)
            printf("Invalid input. Name cannot be empty.\n");
        else
            break;
    } while (1);

    // ----- ID -----
    do
    {
        printf("Enter your ID: ");
        if (scanf("%d", &id) != 1)
        {
            printf("Invalid input. Try again.\n");
            clearInputBuffer();
        }
        else
            break;
    } while (1);

    // ----- ACCOUNT TYPE -----
    do
    {
        printf("Would you like a [1]Savings Account or [2]Current Account? ");
        if (scanf("%d", &accountType) != 1 || (accountType != 1 && accountType != 2))
        {
            printf("Invalid input. Enter 1 for Savings or 2 for Current.\n");
            clearInputBuffer();
        }
        else
            break;
    } while (1);

    // ----- PIN -----
    do
    {
        printf("Enter your 4-digit PIN: ");
        if (scanf("%d", &pin) != 1 || pin < 1000 || pin > 9999)
        {
            printf("Invalid PIN. Must be 4 digits.\n");
            clearInputBuffer();
        }
        else
            break;
    } while (1);

    // ----- ACCOUNT NUMBER -----
    accountNumber = generateAccountNumber();

    // ----- SAVE TO FILE -----
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
}

#endif
