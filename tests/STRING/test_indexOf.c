#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/STRING/string_custom.h"

#define TOTAL_TESTS 7   // Test quantity. Make sure to have them

const char *TEST_NAME = "STRING CUSTOM - test for indexOf()";

const uint8_t *TEST_STRINGS[] = {
    "Hola","Roberto Parra","Arriba el America","Chingue su madre shiBas","Cadena FAKE","Vaquilostina","Royksoop"
    }; 
const uint8_t *TEST_TO_SEARCH[] = {
    "H","rr","Ame","madre","XXX","lostina","yk"
};
    
const int16_t EXPECTED_INDEXES[] = {
    0,10,10,11,-1,5,2
};

bool comparison = false;

int main(int argc, char *argv[])
{
    printf("%s\n",TEST_NAME);

    for (size_t i = 0; i != TOTAL_TESTS; i++)
    {
        int16_t obtained_index = string_indexOf(TEST_TO_SEARCH[i],TEST_STRINGS[i]);
        printf("#%u --> Search <<%s>> in <<%s>>. Expected index: %d Obtained: %d\n",i,TEST_TO_SEARCH[i],TEST_STRINGS[i],EXPECTED_INDEXES[i],obtained_index);
        if(obtained_index != EXPECTED_INDEXES[i]){
            comparison = false;
            break;
        }
        comparison = true;
    }

    printf("%s %s",TEST_NAME,comparison==true? "OK":"not OK");
    return comparison == true ? EXIT_SUCCESS : EXIT_FAILURE;
}