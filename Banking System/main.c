#include <stdio.h>
#include <stdlib.h>
#include "createAccount.h"   

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
            printf("Delete Account feature coming soon.\n");
            break;
        case 3:
            printf("Deposit feature coming soon.\n");
            break;
        case 4:
            printf("Withdrawal feature coming soon.\n");
            break;
        case 5:
            printf("Remittance feature coming soon.\n");
            break;
        case 6:
            printf("Goodbye!\n");
            exit(0);
        default:
            printf("Invalid option. Please try again.\n");
        }

        printf("\n\n"); 
    }

    return 0;
}
