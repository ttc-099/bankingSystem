#include <stdio.h>
#include <stdlib.h>
#include "createAccount.h"   
#include "deleteAccount.h"
#include "deposit.h"

int main()
{
    int input;

    while (1)
    {
        printf("---- BANKING SYSTEM ---- \n\n"
               "Welcome to the bank! Please select an option to start. (Enter the corresponding number.)\n"
               "1. Create Account\n"
               "2. Delete Account\n"
               "3. Deposit\n"
               "4. Withdrawal\n"
               "5. Remittance\n"
               "6. Quit\n"
               "--------------------------------\n"
               "Select Option: ");

        if (scanf("%d", &input) != 1)
        {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (input)
        {
        case 1:
            createAccount(); 
            break;
        case 2:
            deleteAccount();
            break;
        case 3:
            deposit();
            break;
        case 4:
            printf("Withdrawal feature coming soon.\n");
            break;
        case 5:
            printf("Remittance feature coming soon.\n");
            break;
        case 6:
            printf("Program exited. Have a good day.\n");
            exit(0);
        default:
            printf("Invalid option. Please try again.\n");
        }

        printf("\n\n"); 
    }

    return 0;
}
