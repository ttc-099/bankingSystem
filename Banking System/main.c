#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "helpers_CreateDelete.h"
#include "createAccount.h"
#include "deleteAccount.h"
#include "deposit.h"
#include "withdraw.h"
#include "remittance.h"

int main() {
    char input[20];
    int accountCount = 0;
    getAccountCount(&accountCount);

    time_t now = time(NULL);
    printf("---- BANKING SYSTEM ----\n");
    printf("Session started: %s", ctime(&now));
    printf("Loaded accounts: %d\n\n", accountCount);

    while (1) {
        // --- Display menu ---
        printf("Select an option:\n"
               "1. Create Account\n"
               "2. Delete Account\n"
               "3. Deposit\n"
               "4. Withdraw\n"
               "5. Remittance\n"
               "6. Quit\n"
               "Option: ");

        if (scanf("%19s", input) != 1) {
            printf("Invalid input.\n\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        // --- Quit options ---
        if (strcmp(input, "6") == 0 || strcasecmp(input, "quit") == 0 || strcasecmp(input, "exit") == 0) {
            printf("Program exited. Have a good day!\n");
            break;
        }

        // --- Numeric or keyword options ---
        if (strcmp(input, "1") == 0 || strcasecmp(input, "create") == 0) {
            // Case 1: Create account
            createAccount();
        } 
        else if (strcmp(input, "2") == 0 || strcasecmp(input, "delete") == 0) {
            // Case 2: Delete account
            deleteAccount();
        } 
        else if (strcmp(input, "3") == 0 || strcasecmp(input, "deposit") == 0) {
            // Case 3: Deposit
            deposit();
        } 
        else if (strcmp(input, "4") == 0 || strcasecmp(input, "withdraw") == 0) {
            // Case 4: Withdraw
            withdraw();
        } 
        else if (strcmp(input, "5") == 0 || strcasecmp(input, "remit") == 0) {
            // Case 5: Remittance
            remittance();
        } 
        else {
            printf("Invalid option. Please try again.\n\n");
            continue;
        }

        // --- Update account count after each operation ---
        getAccountCount(&accountCount);
        printf("Current accounts: %d\n\n", accountCount);
    }

    return 0;
}
