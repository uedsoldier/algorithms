#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/STRING/string_custom.h"

#define TOTAL_TESTS 6   // Test quantity. Make sure to have them

const char *TEST_NAME = "STRING CUSTOM - test for validate_int()";

const uint8_t *TEST_STRINGS[] = {
    "123","j4j4","1560","9876i5","5510787328","PIC16F1823",
    }; 

const bool EXPECTED_RESPONSES[] = {
    true,false,true,false,true,false
};

bool comparison = false;

int main(int argc, char *argv[])
{
    printf("%s\n",TEST_NAME);

    for (size_t i = 0; i != TOTAL_TESTS; i++)
    {
        bool isInt = string_validate_int(TEST_STRINGS[i]);
        printf("#%u --> Validate if <<%s>> is an integer. Expected: %s Obtained: %s\n",i,TEST_STRINGS[i],EXPECTED_RESPONSES[i]? "true":"false",isInt? "true":"false");
        if(isInt != EXPECTED_RESPONSES[i]){
            comparison = false;
            break;
        }
        comparison = true;
    }

    printf("%s %s",TEST_NAME,comparison==true? "OK":"not OK");
    return comparison == true ? EXIT_SUCCESS : EXIT_FAILURE;
}