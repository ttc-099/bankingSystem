#ifndef REMITTANCE_H
#define REMITTANCE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "helpers_DepositWithdraw.h"  
#include <dirent.h>

// --------------------------------------------------
// Remittance (Transfer) between two accounts
// --------------------------------------------------
void remittance(void) {
    char senderAccStr[20], receiverAccStr[20];
    char senderID[13], receiverID[13];
    char senderName[50], receiverName[50];
    char senderType[20], receiverType[20];
    int senderPIN, receiverPIN;
    double senderBalance = 0.0, receiverBalance = 0.0;
    double amount, fee = 0.0, totalDeduction;
    char line[256], filename[100];

    printf("\n=== REMITTANCE (TRANSFER) ===\n");

    // --------------------------------------------------
    // Early check: ensure database folder has accounts
    // --------------------------------------------------
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

    // --------------------------------------------------
    // Sender Account: Input + Validation + Data Loading
    // --------------------------------------------------
    printf("Enter sender account number: ");
    if (scanf("%19s", senderAccStr) != 1) {
        printf("Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    printf("Enter 4-digit PIN: ");
    if (scanf("%d", &senderPIN) != 1 || senderPIN < 1000 || senderPIN > 9999) {
        printf("Invalid PIN.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    sprintf(filename, "database/%s.txt", senderAccStr);
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Sender account not found.\n");
        return;
    }

    // Load sender account data
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Name:") != NULL) sscanf(line, "Name: %49[^\n]", senderName);
        else if (strstr(line, "ID:") != NULL) sscanf(line, "ID: %12s", senderID);
        else if (strstr(line, "Account Type:") != NULL) sscanf(line, "Account Type: %19[^\n]", senderType);
        else if (strstr(line, "PIN:") != NULL) sscanf(line, "PIN: %d", &senderPIN);
        else if (strstr(line, "Balance:") != NULL) sscanf(line, "Balance: %lf", &senderBalance);
    }
    fclose(file);

    printf("Sender current balance: RM %.2f\n", senderBalance);

    // --------------------------------------------------
    // Receiver Account: Input + Validation + Data Loading
    // --------------------------------------------------
    printf("Enter receiver account number: ");
    if (scanf("%19s", receiverAccStr) != 1) {
        printf("Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (strcmp(senderAccStr, receiverAccStr) == 0) {
        printf("Cannot transfer to the same account.\n");
        return;
    }

    sprintf(filename, "database/%s.txt", receiverAccStr);
    file = fopen(filename, "r");
    if (!file) {
        printf("Receiver account not found.\n");
        return;
    }

    // Load receiver account data
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Name:") != NULL) sscanf(line, "Name: %49[^\n]", receiverName);
        else if (strstr(line, "ID:") != NULL) sscanf(line, "ID: %12s", receiverID);
        else if (strstr(line, "Account Type:") != NULL) sscanf(line, "Account Type: %19[^\n]", receiverType);
        else if (strstr(line, "PIN:") != NULL) sscanf(line, "PIN: %d", &receiverPIN);
        else if (strstr(line, "Balance:") != NULL) sscanf(line, "Balance: %lf", &receiverBalance);
    }
    fclose(file);

    printf("Receiver account found: %s\n", receiverName);

    // --------------------------------------------------
    // Enter transfer amount
    // --------------------------------------------------
    printf("Enter amount to transfer: RM ");
    if (scanf("%lf", &amount) != 1) {
        printf("Invalid amount.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (amount <= 0) {
        printf("Amount must be greater than 0.\n");
        return;
    }

    // --------------------------------------------------
    // Fee Calculation (based on account types)
    // --------------------------------------------------
    fee = 0.0;
    totalDeduction = amount;

    if (strcmp(senderType, "Savings") == 0 && strcmp(receiverType, "Current") == 0) {
        fee = amount * 0.02;
        totalDeduction += fee;
    } else if (strcmp(senderType, "Current") == 0 && strcmp(receiverType, "Savings") == 0) {
        fee = amount * 0.03;
        totalDeduction += fee;
    }

    if (totalDeduction > senderBalance) {
        printf("Insufficient funds. Required: RM %.2f, Available: RM %.2f\n", totalDeduction, senderBalance);
        return;
    }

    // --------------------------------------------------
    // Update balances for both sender and receiver
    // --------------------------------------------------
    double newSenderBalance = senderBalance - totalDeduction;
    double newReceiverBalance = receiverBalance + amount;

    long senderAcc = atol(senderAccStr);
    long receiverAcc = atol(receiverAccStr);

    updateAccountFile(senderAcc, senderName, senderID, senderType, senderPIN, newSenderBalance);
    updateAccountFile(receiverAcc, receiverName, receiverID, receiverType, receiverPIN, newReceiverBalance);

    // --------------------------------------------------
    // Log both sides of the transfer
    // --------------------------------------------------
    logTransaction("remittance_out", senderAcc, -totalDeduction);
    logTransaction("remittance_in", receiverAcc, amount);

    // --------------------------------------------------
    // Final confirmation output
    // --------------------------------------------------
    printf("\nTransfer successful!\n");
    printf("New sender balance: RM %.2f\n", newSenderBalance);
    printf("Amount transferred to %s: RM %.2f\n", receiverName, amount);
    if (fee > 0) printf("Remittance fee charged: RM %.2f\n", fee);
}

#endif
