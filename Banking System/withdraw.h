#ifndef WITHDRAW_H
#define WITHDRAW_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "helpers.h"

void withdraw(void) {
    long accountNumber;
    int pin;
    double amount;
    double currentBalance;
    char filename[100];
    char line[256];
    
    printf("\n=== WITHDRAWAL ===\n");
    
    // Get account number
    printf("Enter your account number: ");
    if (scanf("%ld", &accountNumber) != 1) {
        printf("Invalid account number.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    
    // Get PIN
    do {
        printf("Enter your 4-digit PIN: ");
        if (scanf("%d", &pin) != 1 || pin < 1000 || pin > 9999) {
            printf("Invalid PIN. Must be 4 digits.\n");
            clearInputBuffer();
        } else {
            break;
        }
    } while (1);
    clearInputBuffer();
    
    // Check if account exists
    sprintf(filename, "database/%ld.txt", accountNumber);
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Account not found.\n");
        return;
    }
    
    // Read account info
    int storedPIN = -1;
    currentBalance = -1;
    char name[50], accountType[20];
    int id;
    
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "PIN:") != NULL) sscanf(line, "PIN: %d", &storedPIN);
        else if (strstr(line, "Balance:") != NULL) sscanf(line, "Balance: %lf", &currentBalance);
        else if (strstr(line, "Name:") != NULL) sscanf(line, "Name: %49[^\n]", name);
        else if (strstr(line, "Account Type:") != NULL) sscanf(line, "Account Type: %19[^\n]", accountType);
        else if (strstr(line, "ID:") != NULL) sscanf(line, "ID: %d", &id);
    }
    fclose(file);
    
    // Verify PIN
    if (pin != storedPIN) {
        printf("Wrong PIN.\n");
        return;
    }
    
    printf("Current balance: RM %.2f\n", currentBalance);
    
    // Get amount
    printf("Enter amount to withdraw: RM ");
    if (scanf("%lf", &amount) != 1) {
        printf("Invalid amount.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    
    // Validate amount
    if (amount <= 0) {
        printf("Amount must be greater than 0.\n");
        return;
    }
    
    if (amount > currentBalance) {
        printf("Insufficient funds.\n");
        return;
    }
    
    // Update balance
    double newBalance = currentBalance - amount;
    
    file = fopen(filename, "w");
    if (!file) {
        printf("Error updating account.\n");
        return;
    }
    
    // Write updated account info
    fprintf(file, "Account Number: %ld\n", accountNumber);
    fprintf(file, "Name: %s\n", name);
    fprintf(file, "ID: %d\n", id);
    fprintf(file, "Account Type: %s\n", accountType);
    fprintf(file, "PIN: %d\n", pin);
    fprintf(file, "Balance: %.2f\n", newBalance);
    fclose(file);
    
    // Log transaction
    logTransaction("withdrawal", accountNumber, amount);
    
    printf("Withdrawal successful! New balance: RM %.2f\n", newBalance);
}

#endif