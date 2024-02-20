#include <stdio.h>
#include "morse_filter.h"

int main() {
    char input[100];
    printf("Enter a string: ");
    fgets(input, sizeof(input), stdin);
    
    printf("Morse code: ");
    morse_filter(input);
    
    return 0;
}

