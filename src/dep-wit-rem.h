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

void verifyPin(char *correctPin) {
    char pin[5];
    printf("Enter your 4-Digit Pin: ");
    scanf("%[^\n]", pin);
    clearInputBuffer();
    while (strcmp(pin, correctPin) != 0) {
        printf("\033[31m**INCORRECT PIN**\033[0m\n");
        printf("Re-enter your 4-Digit Pin: ");
        scanf("%[^\n]", pin);
        clearInputBuffer();
    }
}

void depositToAccount(char *accountNo) {
    char fileDirectory[512] = "../database/";
    char fileName[100];
    struct verifyAccount accountToDeposit;

    strcpy(fileName, accountNo);
    strcat(fileName, ".txt");
    strcat(fileDirectory, fileName);

    readAccountCSV(fileDirectory, &accountToDeposit);

    verifyPin(accountToDeposit.pin);

    double oldBalance = atof(accountToDeposit.balance);
    double newBalance;
    char amountToDeposit[100];
    printf("Enter the amount you wish to deposit: ");
    scanf("%[^\n]", amountToDeposit);
    clearInputBuffer();
    removeChar(amountToDeposit, ',');
    while (!isDouble(amountToDeposit)) {
        printf("\033[31m**INVALID AMOUNT**\033[0m\n");
        printf("Enter the amount you wish to deposit: ");
        scanf("%[^\n]", amountToDeposit);
        clearInputBuffer();
        removeChar(amountToDeposit, ',');
    }
    while (atof(amountToDeposit) < 0 || atof(amountToDeposit) > 50000) {
        printf("\033[31m**INVALID AMOUNT: DEPOSITS MUST BE WITHIN 0-50,000**\033[0m\n");
        printf("Enter the amount you wish to deposit: ");
        scanf("%[^\n]", amountToDeposit);
        clearInputBuffer();
        removeChar(amountToDeposit, ',');
    }
    newBalance = atof(amountToDeposit) + oldBalance;
    updateAccount(fileDirectory, newBalance, &accountToDeposit);
    printf("\n\033[1mRM%.2lf Deposited!\033[0m\n", atof(amountToDeposit));
}

void withdrawFromAccount(char *accountNo) {
    char fileDirectory[512] = "../database/";
    char fileName[100];
    struct verifyAccount accountToWithdraw;

    strcpy(fileName, accountNo);
    strcat(fileName, ".txt");
    strcat(fileDirectory, fileName);

    readAccountCSV(fileDirectory, &accountToWithdraw);

    verifyPin(accountToWithdraw.pin);

    double oldBalance = atof(accountToWithdraw.balance);
    double newBalance;
    char amountToWithdraw[100];

    printf("\n\033[1mAccount Balance: %s\033[0m\n", accountToWithdraw.balance);

    printf("\nEnter the amount you wish to withdraw: ");
    scanf("%[^\n]", amountToWithdraw);
    clearInputBuffer();
    removeChar(amountToWithdraw, ',');
    while (!isDouble(amountToWithdraw)) {
        printf("\033[31m**INVALID AMOUNT**\033[0m\n");
        printf("Enter the amount you wish to withdraw: ");
        scanf("%[^\n]", amountToWithdraw);
        clearInputBuffer();
        removeChar(amountToWithdraw, ',');
    }
    while (atof(amountToWithdraw) > oldBalance) {
        printf("\033[31m**INVALID AMOUNT: NOT ENOUGH FUNDS**\033[0m\n");
        printf("Enter the amount you wish to withdraw: ");
        scanf("%[^\n]", amountToWithdraw);
        clearInputBuffer();
        removeChar(amountToWithdraw, ',');
    }
    newBalance = oldBalance - atof(amountToWithdraw);
    updateAccount(fileDirectory, newBalance, &accountToWithdraw);
    printf("\n\033[1mRM%.2lf Withdrawn!\033[0m\n", atof(amountToWithdraw));
}