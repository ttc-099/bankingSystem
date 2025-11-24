#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "createAccount.h"
#include "deleteAccount.h"
#include "deposit.h"
#include "withdraw.h"
#include "remittance.h"
#include "helpers.h"  
#include "helpers2.h"

#ifdef _WIN32
#define strcasecmp _stricmp
#endif

int main()
{
    char input[20];
    time_t now = time(NULL);
    int accountCount = 0;

    getAccountCount(&accountCount);  // get current account count

    printf("---- BANKING SYSTEM ----\n");
    printf("Session started: %s", ctime(&now));
    printf("Count of loaded accounts: %d\n\n", accountCount);

    while (1)
    {
        printf("Welcome to the bank! Please select an option to start.\n"
               "1. Create Account (or 'create')\n"
               "2. Delete Account (or 'delete')\n"
               "3. Deposit (or 'deposit')\n"
               "4. Withdrawal (or 'withdrawal')\n"
               "5. Remittance (or 'remittance')\n"
               "6. Quit (or 'quit', 'exit')\n"
               "Select Option: ");

        if (scanf("%19s", input) != 1)
        {
            printf("Invalid input.\n");
            while (getchar() != '\n'); // clear buffer
            continue;
        }
        while (getchar() != '\n'); // clear buffer

        if (strcmp(input, "1") == 0 || strcasecmp(input, "create") == 0)
        {
            createAccount();
            getAccountCount(&accountCount);  // update account count
            printf("Current accounts: %d\n", accountCount);
        }
        else if (strcmp(input, "2") == 0 || strcasecmp(input, "delete") == 0)
        {
            deleteAccount();
            getAccountCount(&accountCount);  // update account count
            printf("Current accounts: %d\n", accountCount);
        }
        else if (strcmp(input, "3") == 0 || strcasecmp(input, "deposit") == 0)
            deposit();
        else if (strcmp(input, "4") == 0 || strcasecmp(input, "withdrawal") == 0)
            withdraw();
        else if (strcmp(input, "5") == 0 || strcasecmp(input, "remittance") == 0)
            remittance();
        else if (strcmp(input, "6") == 0 || strcasecmp(input, "quit") == 0 || strcasecmp(input, "exit") == 0)
        {
            printf("Program exited. Have a good day!\n");
            exit(0);
        }
        else
            printf("Invalid option. Please try again.\n");

        printf("\n"); // spacing for readability
    }

    return 0;
}
