#include "morse_filter.h"
#include <ctype.h>
#include <stdio.h>

const char *morse_code[] = {
    ".-",    // A
    "-...",  // B
    "-.-.",  // C
    "-..",   // D
    ".",     // E
    "..-.",  // F
    "--.",   // G
    "....",  // H
    "..",    // I
    ".---",  // J
    "-.-",   // K
    ".-..",  // L
    "--",    // M
    "-.",    // N
    "---",   // O
    ".--.",  // P
    "--.-",  // Q
    ".-.",   // R
    "...",   // S
    "-",     // T
    "..-",   // U
    "...-",  // V
    ".--",   // W
    "-..-",  // X
    "-.--",  // Y
    "--..",  // Z
    "-----", // 0
    ".----", // 1
    "..---", // 2
    "...--", // 3
    "....-", // 4
    ".....", // 5
    "-....", // 6
    "--...", // 7
    "---..", // 8
    "----.", // 9
    "--..--",// ,
    ".-.-.-",// .
    "..--..",// ?
    "-.-.--" // !
};

void morse_filter(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        if (isalpha(str[i])) {
            printf("%s ", morse_code[toupper(str[i]) - 'A']);
        } else if (isdigit(str[i])) {
            printf("%s ", morse_code[str[i] - '0' + 26]);
        } else if (isspace(str[i])) {
            printf(" ");
        } else if (str[i] == ',') {
            printf("%s ", morse_code[36]);
        } else if (str[i] == '.') {
            printf("%s ", morse_code[37]);
        } else if (str[i] == '?') {
            printf("%s ", morse_code[38]);
        } else if (str[i] == '!') {
            printf("%s ", morse_code[39]);
        } else {
            printf("Unknown character ");
        }
        i++;
    }
    printf("\n");
}

