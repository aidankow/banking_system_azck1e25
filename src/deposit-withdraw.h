#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool accountExists(char *input) {
    char fileDirectory[512] = "../database/";
    char fileName[100];

    strcpy(fileName, input);
    strcat(fileName, ".txt");
    strcat(fileDirectory, fileName);

    FILE *accountPtr;
    accountPtr = fopen(fileDirectory, "r");
    if (accountPtr == NULL) {
        return false;
    } else {
        return true;
    }
}

void updateAccount(char *fileDirectory, double newBalance, struct verifyAccount *account) {
    char newFileName[100] = "../database/";
    strcat(newFileName, account->accountNo);
    strcat(newFileName, ".txt");

    FILE *tempPtr;
    tempPtr = fopen("../database/temp.txt", "w");
    if (tempPtr == NULL) {
        printf("\033[31m**Error opening temp.txt**\033[0m\n");
    }
    fprintf(tempPtr, "%s,%s,%s,%s,%s,%.2lf", account->accountNo, account->id, account->name, account->pin, account->accountType, newBalance);
    fclose(tempPtr);
    
    remove(fileDirectory);
    rename("../database/temp.txt", newFileName);
}

void depositToAccount(char *accountNo) {
    char fileDirectory[512] = "../database/";
    char fileName[100];
    struct verifyAccount account;

    strcpy(fileName, accountNo);
    strcat(fileName, ".txt");
    strcat(fileDirectory, fileName);

    readAccountCSV(fileDirectory, &account);

    char pin[5];
    printf("Enter your 4-Digit Pin: ");
    scanf("%[^\n]", pin);
    clearInputBuffer();
    while (strcmp(pin, account.pin) != 0) {
        printf("\033[31m**INCORRECT PIN**\033[0m\n");
        printf("Re-enter your 4-Digit Pin: ");
        scanf("%[^\n]", pin);
        clearInputBuffer();
    }

    double oldBalance = atof(account.balance);
    double newBalance;
    char *amountToDeposit;
    printf("Enter the amount you wish to deposit: ");
    scanf("%[^\n]", amountToDeposit);
    clearInputBuffer();
    removeChar(amountToDeposit, ',');
    while (!isDouble(amountToDeposit)) {
        printf("\033[31m**INVALID AMOUNT: DEPOSITS MUST BE WITHIN 0-50,000**\033[0m\n");
        printf("Enter the amount you wish to deposit: ");
        scanf("%[^\n]", amountToDeposit);
        clearInputBuffer();
    }
    while (atof(amountToDeposit) < 0 || atof(amountToDeposit) > 50000) {
        printf("\033[31m**INVALID AMOUNT: DEPOSITS MUST BE WITHIN 0-50,000**\033[0m\n");
        printf("Enter the amount you wish to deposit: ");
        scanf("%[^\n]", amountToDeposit);
        clearInputBuffer();
    }
    newBalance = atof(amountToDeposit) + oldBalance;
    updateAccount(fileDirectory, newBalance, &account);
    printf("\n\033[1mRM%s Deposited!\033[0m\n", amountToDeposit);
}