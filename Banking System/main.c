#include <stdio.h>

void createAccount() {



}

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

    return 0;
}