#ifndef HELPERS3_H
#define HELPERS3_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
    isValidIC
    ---------
    Validates Malaysian IC structure:
    - Must be exactly 12 digits
    - All characters must be numeric
    - State code (digits 7-8) must be valid
*/
int isValidIC(const char *ic) {

    // --- Check length ---
    if (strlen(ic) != 12)
        return 0;

    // --- Check all digits ---
    for (int i = 0; i < 12; i++) {
        if (ic[i] < '0' || ic[i] > '9')
            return 0;
    }

    // --- Validate state code ---
    int stateCode = (ic[6] - '0') * 10 + (ic[7] - '0');

    int validStates[] = {
        1,2,3,4,5,6,7,8,9,
        10,11,12,13,14,15,16,
        21,22,23,24,25,26,27,
        30,31,32,33,34,35,36,
        40,41,42,43,44,45,46,
        50,51,52,53,54,55,56
    };

    int found = 0;
    for (int i = 0; i < sizeof(validStates)/sizeof(validStates[0]); i++) {
        if (stateCode == validStates[i]) {
            found = 1;
            break;
        }
    }

    return found;
}

/*
    updateAccountFile
    ------------------
    Rewrites an existing account file with updated details.
    Used by remittance(), withdraw(), deposit(), etc.
*/

/* hold on
void updateAccountFile(long accountNumber,
                       const char *name,
                       long id,
                       const char *accType,
                       int pin,
                       double balance)
{
    char filename[100];
    sprintf(filename, "database/%ld.txt", accountNumber);

    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not update account file for %ld\n", accountNumber);
        return;
    }

    fprintf(file, "Account Number: %ld\n", accountNumber);
    fprintf(file, "Name: %s\n", name);
    fprintf(file, "IC: %ld\n", id);
    fprintf(file, "Account Type: %s\n", accType);
    fprintf(file, "PIN: %d\n", pin);
    fprintf(file, "Balance: %.2f\n", balance);

    fclose(file);
}

*/

#endif


