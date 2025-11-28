#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

bool isNum(char *string) {
    for (int i=0;string[i]!='\0';i++) {
        if (!isdigit(string[i])) {
            return false;
        }
    }
    return true;
}

bool nameCheck(char *name) {
    // gets rid of whitespaces
    char temp[101];
    strcpy(temp, name);
    int currentChar = 0;
    int nextChar = 0;
    while (temp[currentChar] != '\0') {
        if (temp[currentChar] != ' ') {
            temp[nextChar] = temp[currentChar];
            nextChar++;
        }
        currentChar++;
    }

    // ensure not an empty string
    temp[nextChar] = '\0';
    if (temp[0] == '\0') {
        return false;
    }

    // ensure only alphabets are used
    for (int i=0;temp[i]!='\0';i++) {
        if (!isalpha(temp[i])) {
            return false;
        }
    }
    return true;
}

bool idCheck(char *id) {
    if (strlen(id) != 8) {
        return false;
    } else if (!isNum(id)) {
        return false;
    }
    return true;
}

bool accountTypeCheck (char *type) {
    if (strcasecmp(type, "savings") == 0 || strcasecmp(type, "saving") == 0 || strcasecmp(type, "current") == 0) {
        return true;
    }
    return false;
}

bool pinCheck (char *pin) {
    if (strlen(pin) != 4) {
        return false;
    } else if (!isNum(pin)) {
        return false;
    }
    return true;
}