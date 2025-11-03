#include <stdio.h>

void createAccount()
{
    // ----- Name ---------
    char name[50];
    do
    {
        printf("Enter your name: ");
        if (scanf("%49s", name) != 1)
        {
            printf("Invalid input. Try again?\n");
            while (getchar() != '\n'); // clearing leftover input
        }
        else
            break;
    } while (1);

    // ----- ID ---------
    int id;
    do
    {
        printf("Enter your ID: ");
        if (scanf("%d", &id) != 1)
        {
            printf("Invalid input. Try again?\n");
            while (getchar() != '\n');
        }
        else
            break;
    } while (1);

    // ----- Savings/Current Prompt---------
    int accountType;
    do
    {
        printf("Would you like a [1]Savings Account or [2]Current Account? ");
        if (scanf("%d", &accountType) != 1 || (accountType != 1 && accountType != 2))
        {
            printf("Invalid input. Enter 1 for Savings or 2 for Current.\n");
            while (getchar() != '\n');
        }
        else
            break;
    } while (1);

    // ----- PIN ---------
    int pin;
        do
    {
        printf("Enter your PIN: ");
        if (scanf("%d", &pin) != 1)
        {
            printf("Invalid input. Try again?\n");
            while (getchar() != '\n');
        }
        else
            break;
    } while (1);
};

// ------------------------------------------------

void deleteAccount() {


}

int main() {

    printf( "---- BANKING SYSTEM ---- \n\n"
        "Welcome to the bank! Please select an option to start. (Enter the corresponding number.)\n"
        "1. Create Account\n"
        "2. Delete Account\n"
        "3. Deposit \n"
        "4. Withdrawal \n"
        "5. Remittance \n"
        "6. Quit\n"
        "--------------------------------\n"
        "Select Option: "
    );
    int input;
    scanf("%d", &input); 

    createAccount();


    return 0;
}