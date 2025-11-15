#ifndef REMITTANCE_H
#define REMITTANCE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "helpers.h"

void remittance(void) {
    long senderAccount, receiverAccount;
    int senderPIN;
    double amount;
    double senderBalance, receiverBalance;
    char filename[100];
    char line[256];
    
    printf("\n=== REMITTANCE (TRANSFER) ===\n");
    
    // Get sender account number
    printf("Enter your account number (sender): ");
    if (scanf("%ld", &senderAccount) != 1) {
        printf("Invalid account number.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    
    // Get sender PIN
    printf("Enter your 4-digit PIN: ");
    if (scanf("%d", &senderPIN) != 1 || senderPIN < 1000 || senderPIN > 9999) {
        printf("Invalid PIN. Must be 4 digits.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    
    // Check if sender account exists and authenticate
    sprintf(filename, "database/%ld.txt", senderAccount);
    FILE *senderFile = fopen(filename, "r");
    if (!senderFile) {
        printf("Sender account not found.\n");
        return;
    }
    
    // Read sender account info
    int storedSenderPIN = -1;
    senderBalance = -1;
    char senderName[50], senderAccType[20];
    int senderID;
    
    while (fgets(line, sizeof(line), senderFile)) {
        if (strstr(line, "PIN:") != NULL) sscanf(line, "PIN: %d", &storedSenderPIN);
        else if (strstr(line, "Balance:") != NULL) sscanf(line, "Balance: %lf", &senderBalance);
        else if (strstr(line, "Name:") != NULL) sscanf(line, "Name: %49[^\n]", senderName);
        else if (strstr(line, "Account Type:") != NULL) sscanf(line, "Account Type: %19[^\n]", senderAccType);
        else if (strstr(line, "ID:") != NULL) sscanf(line, "ID: %d", &senderID);
    }
    fclose(senderFile);
    
    // Verify sender PIN
    if (senderPIN != storedSenderPIN) {
        printf("Wrong PIN for sender account.\n");
        return;
    }
    
    // Get receiver account number
    printf("Enter receiver account number: ");
    if (scanf("%ld", &receiverAccount) != 1) {
        printf("Invalid receiver account number.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    
    // Validate accounts are distinct
    if (senderAccount == receiverAccount) {
        printf("Cannot transfer to the same account.\n");
        return;
    }
    
    // Check if receiver account exists
    sprintf(filename, "database/%ld.txt", receiverAccount);
    FILE *receiverFile = fopen(filename, "r");
    if (!receiverFile) {
        printf("Receiver account not found.\n");
        return;
    }
    
    // Read receiver account info
    receiverBalance = -1;
    char receiverName[50], receiverAccType[20];
    int receiverID, receiverPIN;
    
    while (fgets(line, sizeof(line), receiverFile)) {
        if (strstr(line, "Balance:") != NULL) sscanf(line, "Balance: %lf", &receiverBalance);
        else if (strstr(line, "Name:") != NULL) sscanf(line, "Name: %49[^\n]", receiverName);
        else if (strstr(line, "Account Type:") != NULL) sscanf(line, "Account Type: %19[^\n]", receiverAccType);
        else if (strstr(line, "ID:") != NULL) sscanf(line, "ID: %d", &receiverID);
        else if (strstr(line, "PIN:") != NULL) sscanf(line, "PIN: %d", &receiverPIN);
    }
    fclose(receiverFile);
    
    printf("Sender current balance: RM %.2f\n", senderBalance);
    printf("Receiver account found: %s\n", receiverName);
    
    // Get transfer amount
    printf("Enter amount to transfer: RM ");
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
    
    // Calculate remittance fee based on account types
    double fee = 0.0;
    double totalDeduction = amount;
    
    if (strcmp(senderAccType, "Savings") == 0 && strcmp(receiverAccType, "Current") == 0) {
        fee = amount * 0.02;
        totalDeduction = amount + fee;
        printf("Remittance fee (2%%): RM %.2f\n", fee);
    } else if (strcmp(senderAccType, "Current") == 0 && strcmp(receiverAccType, "Savings") == 0) {
        fee = amount * 0.03;
        totalDeduction = amount + fee;
        printf("Remittance fee (3%%): RM %.2f\n", fee);
    }
    
    // Check if sender has sufficient funds (amount + fee)
    if (totalDeduction > senderBalance) {
        printf("Insufficient funds. You need RM %.2f (amount + fee) but have RM %.2f\n", 
               totalDeduction, senderBalance);
        return;
    }
    
    // Calculate new balances
    double newSenderBalance = senderBalance - totalDeduction;
    double newReceiverBalance = receiverBalance + amount;
    
    // Update sender account
    updateAccountFile(senderAccount, senderName, senderID, senderAccType, senderPIN, newSenderBalance);
    
    // Update receiver account  
    updateAccountFile(receiverAccount, receiverName, receiverID, receiverAccType, receiverPIN, newReceiverBalance);
    
    // Log transactions
    logTransaction("remittance_out", senderAccount, -totalDeduction);
    logTransaction("remittance_in", receiverAccount, amount);
    
    printf("Transfer successful!\n");
    printf("New sender balance: RM %.2f\n", newSenderBalance);
    printf("Amount transferred to %s: RM %.2f\n", receiverName, amount);
    if (fee > 0) {
        printf("Remittance fee charged: RM %.2f\n", fee);
    }
}

#endif