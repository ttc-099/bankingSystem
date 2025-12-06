#ifndef HELPERS3_H
#define HELPERS3_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int isValidIC(const char *ic) {

    // --- Check length ---
    if (strlen(ic) != 12)
        return 0;

    // --- Check all digits ---
    for (int i = 0; i < 12; i++) {
        if (!isdigit(ic[i]))
            return 0;
    }

    // --- Extract state code ---
    int stateCode = (ic[6] - '0') * 10 + (ic[7] - '0');

    // --- State code validity ---
    // Ranges based on JPN documentation (Peninsula, Sabah, Sarawak, special)
    if (!(
        (stateCode >= 1  && stateCode <= 16) ||   // Peninsular Malaysia
        (stateCode >= 21 && stateCode <= 29) ||   // Sabah
        (stateCode >= 30 && stateCode <= 39) ||   // Sarawak
        (stateCode >= 40 && stateCode <= 49) ||   // Special/Multi-purpose
        (stateCode >= 50 && stateCode <= 59) ||   // KL/Sabah/Sarawak extensions
        (stateCode >= 80 && stateCode <= 89) ||   // Sabah (extended)
        (stateCode >= 90 && stateCode <= 99)      // Sarawak (extended)
    )) {
        return 0;
    }

    return 1;   // IC is valid
}

#endif
