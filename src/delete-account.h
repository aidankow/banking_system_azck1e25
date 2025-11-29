#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct verifyAccount
{
    char accountNo[10];
    char id[9];
    char name[101];
    char pin[5];
    char accountType[8];
    char balance[100];
};

bool checkAccountNo(char *input, int numberOfAccounts, char accounts[][10]) {
    if (!isNum(input)) {
        return false;
    } else if (strlen(input) == 1) {
        if (atoi(input) > numberOfAccounts || atoi(input) < 1) {
            return false;
        }
    } else {
        bool exists = false;
        for (int i=0;i<numberOfAccounts;i++) {
            accounts[i][strcspn(accounts[i], "\n")] = '\0';
            if (strcmp(input, accounts[i]) == 0) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            return false;
        }
    }
    return true;
}

char *getAccountNo(char *input, char accounts[][10]) {
    if (strlen(input) > 1) {
        return input;
    } else {
        return accounts[atoi(input)-1];
    }
}

void deleteAccount(char *accountNo, char *fileDirectory) {
    char temp[100];

    FILE *accountPtr;
    accountPtr = fopen("../database/index.txt", "r");
    if (accountPtr == NULL) {
        printf("\033[31m(1)Error opening index.txt\033[0m\n");
    }
    FILE *tempPtr;
    tempPtr = fopen("../database/temp.txt", "w");
    if (tempPtr == NULL) {
        printf("\033[31m(1)Error opening index.txt\033[0m\n");
    }

    while (fgets(temp, 100, accountPtr)) {
        temp[strcspn(temp, "\n")] = '\0';
        if (strcmp(temp, accountNo) != 0) {
            fputs(temp, tempPtr);
            fputc('\n', tempPtr);
        }
    }
    fclose(accountPtr);
    fclose(tempPtr);

    remove("../database/index.txt");
    rename("../database/temp.txt", "../database/index.txt");
    remove(fileDirectory);
}

void readAccountCSV(char *fileDirectory, struct verifyAccount *account) {
    FILE *accountPtr;
    accountPtr = fopen(fileDirectory, "r");
    if (accountPtr == NULL) {
        printf("\033[31mError opening file directory\033[0m\n");
    }

    char line[512];
    while (fgets(line, sizeof(line), accountPtr)) {
        line[strcspn(line, "\n")] = '\0';

        char *token = strtok(line, ",");
        if (token) {
            strncpy(account->accountNo, token, sizeof(account->accountNo) - 1);
            account->accountNo[sizeof(account->accountNo) - 1] = '\0';
        }
        token = strtok(NULL, ",");
        if (token) {
            strncpy(account->id, token, sizeof(account->id) - 1);
            account->id[sizeof(account->id) - 1] = '\0';
        }
        token = strtok(NULL, ",");
        if (token) {
            strncpy(account->name, token, sizeof(account->name) - 1);
            account->name[sizeof(account->name) - 1] = '\0';
        }
        token = strtok(NULL, ",");
        if (token) {
            strncpy(account->pin, token, sizeof(account->pin) - 1);
            account->pin[sizeof(account->pin) - 1] = '\0';
        }
        token = strtok(NULL, ",");
        if (token) {
            strncpy(account->accountType, token, sizeof(account->accountType) - 1);
            account->accountType[sizeof(account->accountType) - 1] = '\0';
        }
        token = strtok(NULL, ",");
        if (token) {
            strncpy(account->balance, token, sizeof(account->balance) - 1);
            account->balance[sizeof(account->balance) - 1] = '\0';
        }
    }
    fclose(accountPtr);
}

void performDeletion(char *accountNo) {
    struct verifyAccount account;
    char fileDirectory[512] = "../database/";
    char fileName[100];

    strcpy(fileName, accountNo);
    strcat(fileName, ".txt");
    strcat(fileDirectory, fileName);

    readAccountCSV(fileDirectory, &account);

    char accountNum[10];
    char idDigits[5];
    char pin[5];
    printf("Re-Confirm the Account Number: ");
    scanf("%[^\n]", accountNum);
    clearInputBuffer();
    while (strcmp(accountNum, account.accountNo) != 0) {
        printf("\033[31m**ACCOUNT NUMBERS DO NOT MATCH**\033[0m\n");
        printf("Re-Confirm the Account Number: ");
        scanf("%[^\n]", accountNum);
        clearInputBuffer();
    }

    printf("Re-Confirm the Last 4-Digits of the ID: ");
    scanf("%[^\n]", idDigits);
    clearInputBuffer();
    bool equal = true;
    if (strlen(idDigits) != 4) {
        equal = false;
    } else {
        for (int i=4;i<8;i++) {
            if (idDigits[i-4] != account.id[i]) {
                equal = false;
            }
        }
    }
    while (!equal) {
        printf("\033[31m**ID NUMBERS DO NOT MATCH**\033[0m\n");
        printf("Re-Confirm the Last 4-Digits of the ID: ");
        scanf("%[^\n]", idDigits);
        clearInputBuffer();
        equal = true;
        if (strlen(idDigits) != 4) {
            equal = false;
        } else {
            for (int i=4;i<8;i++) {
                if (idDigits[i-4] != account.id[i]) {
                    equal = false;
                }
            }
        }
    }

    printf("Re-Confirm the 4-Digit Pin: ");
    scanf("%[^\n]", pin);
    clearInputBuffer();
    while (strcmp(pin, account.pin) != 0) {
        printf("\033[31m**PINS DO NOT MATCH**\033[0m\n");
        printf("Re-Confirm the 4-Digit Pin: ");
        scanf("%[^\n]", pin);
        clearInputBuffer();
    }

    deleteAccount(accountNo, fileDirectory);
}