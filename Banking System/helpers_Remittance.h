#ifndef HELPERS_REMITTANCE_H
#define HELPERS_REMITTANCE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers_CreateDelete.h"

// -----------------------------
// Read account info into pointers
// -----------------------------
static int readAccount(long accountNumber, char *name, char *id, char *accountType, int *pin, double *balance) {
    char filename[100];
    sprintf(filename, "database/%ld.txt", accountNumber);

    FILE *file = fopen(filename, "r");
    if (!file) return 0; // account not found

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Name:") != NULL) sscanf(line, "Name: %49[^\n]", name);
        else if (strstr(line, "ID:") != NULL) sscanf(line, "ID: %12s", id);
        else if (strstr(line, "Account Type:") != NULL) sscanf(line, "Account Type: %19[^\n]", accountType);
        else if (strstr(line, "PIN:") != NULL) sscanf(line, "PIN: %d", pin);
        else if (strstr(line, "Balance:") != NULL) sscanf(line, "Balance: %lf", balance);
    }
    fclose(file);
    return 1; // success
}

// -----------------------------
// Validate sufficient funds
// -----------------------------
static int hasSufficientFunds(double balance, double amount, double fee) {
    return (balance >= (amount + fee));
}

// -----------------------------
// Calculate remittance fee
// -----------------------------
static double calculateFee(const char *senderType, const char *receiverType) {
    if (strcmp(senderType, "Savings") == 0 && strcmp(receiverType, "Current") == 0)
        return 0.02;
    else if (strcmp(senderType, "Current") == 0 && strcmp(receiverType, "Savings") == 0)
        return 0.03;
    return 0.0;
}

#endif
