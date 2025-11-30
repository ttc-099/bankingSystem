#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "helpers_CreateDelete.h"
#include "helpers_DepositWithdraw.h"
#include "helpers_Remittance.h"   

#include "createAccount.h"
#include "deleteAccount.h"
#include "deposit.h"
#include "withdraw.h"
#include "remittance.h"         

// --------------------------------------------------
// Function Pointer Type for Banking Operations
// --------------------------------------------------

typedef void (*BankFunc)();

// Map numeric/word commands → functions
BankFunc bankingFunctions[] = {
    createAccount,   // 1
    deleteAccount,   // 2
    deposit,         // 3
    withdraw,        // 4
    remittance       // 5
};

const char *wordCommands[] = {
    "create",
    "delete",
    "deposit",
    "withdraw",
    "remittance"
};

#define NUM_FUNCTIONS (sizeof(bankingFunctions) / sizeof(bankingFunctions[0]))

// --------------------------------------------------
// Helper: Match word command to index
// --------------------------------------------------
int getWordCommandIndex(const char *input) {
    for (int i = 0; i < NUM_FUNCTIONS; i++) {
        if (strcasecmp(input, wordCommands[i]) == 0)
            return i;
    }
    return -1;
}

// --------------------------------------------------
// MAIN
// --------------------------------------------------
int main() {
    char input[20];
    int accountCount = 0;
    time_t now = time(NULL);

    getAccountCount(&accountCount);

    printf("---- BANKING SYSTEM ----\n");
    printf("Session started: %s", ctime(&now));
    printf("Loaded accounts: %d\n\n", accountCount);

    while (1) {
        printf("Welcome! Select an option:\n"
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

        // --- Exit commands ---
        if (strcasecmp(input, "quit") == 0 || strcasecmp(input, "exit") == 0 || strcmp(input, "6") == 0) {
            printf("Program exited. Have a good day!\n");
            return 0;
        }

        // --- Numeric selection ---
        int option = atoi(input);
        if (option >= 1 && option <= NUM_FUNCTIONS) {
            int idx = option - 1;
            bankingFunctions[idx]();
            getAccountCount(&accountCount);
            printf("Current accounts: %d\n\n", accountCount);
            continue;
        }

        // --- Word commands ---
        int idx = getWordCommandIndex(input);
        if (idx != -1) {
            bankingFunctions[idx]();
            getAccountCount(&accountCount);
            printf("Current accounts: %d\n\n", accountCount);
            continue;
        }

        printf("Invalid option. Please try again.\n\n");
    }

    return 0;
}
