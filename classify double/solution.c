#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>



/**
 * Library-level functions.
 * You should use them in the main sections.
 */

uint64_t convertToUint64(double number) {
    return *((uint64_t*)(&number));
}

bool getBit(const uint64_t number, const uint8_t index) {
    uint64_t mask = 1 << index;
    return (mask & number) >> index;
}

bool isNegative(uint64_t number) {
    return getBit(number, 63);
}

bool isDenormalized(uint64_t number) {
    for (uint8_t i = 62; i > 51; --i) {
        if (getBit(number, i)) {
            return false;
        }
    }
    for (uint8_t i = 51; i > 0; --i) {
        if (getBit(number, i)) {
            return true;
        }
    }
    return false;
}

bool isExponentAllOne(uint64_t number) {
    for (uint8_t i = 62; i > 51; --i) {
        if (!getBit(number, i)) {
            return false;
        }
    }
    return true;
}

bool isNormalized(uint64_t number) {
    bool zeroFlag = false;
    bool oneFlag = false;
    for (uint8_t i = 52; i < 63; ++i) {
        if (getBit(number, i)) {
            oneFlag = true;
        }
        else {
            zeroFlag = true;
        }
    }
    return oneFlag & zeroFlag;
}
/**
 * Checkers here:
 */

bool checkForPlusZero(uint64_t number) {
    return number == 0x0000000000000000;
}

bool checkForMinusZero(uint64_t number) {
    return number == 0x8000000000000000;
}

bool checkForPlusInf(uint64_t number) {
    return number == 0x7FE0000000000000;
}

bool checkForMinusInf(uint64_t number) {
    return number == 0xFFE0000000000000;
}

bool checkForPlusNormal(uint64_t number) {
    return !isNegative(number) & isNormalized(number);
}

bool checkForMinusNormal(uint64_t number) {
    return isNegative(number) & isNormalized(number);
}

bool checkForPlusDenormal(uint64_t number) {
    return !isNegative(number) & isDenormalized(number);
}

bool checkForMinusDenormal(uint64_t number) {
    return isNegative(number) & isDenormalized(number);
}

bool checkForSignalingNan(uint64_t number) {
    bool oneFlag = false;
    for (uint8_t i = 0; i <= 50; ++i) {
        if (getBit(number, i)) {
            oneFlag = true;
        }
    }
    return isExponentAllOne(number) & (!getBit(number, 51)) & oneFlag;
}

bool checkForQuietNan(uint64_t number) {
    return isExponentAllOne(number) & getBit(number, 51);
}


void classify(double number) {

    printf("%u\n", convertToUint64(number));

    if (checkForPlusZero(convertToUint64(number))) {
        printf("Plus zero\n");
    }

    else if (checkForMinusZero(convertToUint64(number))) {
        printf("Minus zero\n");
    }

    else if (checkForPlusInf(convertToUint64(number))) {
        printf("Plus inf\n");
    }

    else if (checkForMinusInf(convertToUint64(number))) {
        printf("Minus inf\n");
    }

    else if (checkForPlusNormal(convertToUint64(number))) {
        printf("Plus normal\n");
    }

    else if (checkForMinusNormal(convertToUint64(number))) {
        printf("Minus normal\n");
    }

    else if (checkForPlusDenormal(convertToUint64(number))) {
        printf("Plus Denormal\n");
    }

    else if (checkForMinusDenormal(convertToUint64(number))) {
        printf("Minus Denormal\n");
    }

    else if (checkForSignalingNan(convertToUint64(number))) {
        printf("Signailing NaN\n");
    }

    else if (checkForQuietNan(convertToUint64(number))) {
        printf("Quiet NaN\n");
    }

    else {
        printf("Error.\n");
    }
}
