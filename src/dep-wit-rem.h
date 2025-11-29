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

void updateAccount(char *fileDirectory, double newBalance, struct accountCSV *account) {
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
    char pin[5] = "";
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
    struct accountCSV accountToDeposit;

    strcpy(fileName, accountNo);
    strcat(fileName, ".txt");
    strcat(fileDirectory, fileName);

    readAccountCSV(fileDirectory, &accountToDeposit);

    verifyPin(accountToDeposit.pin);

    double oldBalance = atof(accountToDeposit.balance);
    double newBalance;
    char amountToDeposit[100];
    printf("Enter the amount you wish to deposit: RM");
    scanf("%[^\n]", amountToDeposit);
    clearInputBuffer();
    removeChar(amountToDeposit, ',');
    while (!isDouble(amountToDeposit)) {
        printf("\033[31m**INVALID AMOUNT**\033[0m\n");
        printf("Enter the amount you wish to deposit: RM");
        scanf("%[^\n]", amountToDeposit);
        clearInputBuffer();
        removeChar(amountToDeposit, ',');
    }
    while (atof(amountToDeposit) < 0 || atof(amountToDeposit) > 50000) {
        printf("\033[31m**INVALID AMOUNT: DEPOSITS MUST BE WITHIN 0-50,000**\033[0m\n");
        printf("Enter the amount you wish to deposit: RM");
        scanf("%[^\n]", amountToDeposit);
        clearInputBuffer();
        removeChar(amountToDeposit, ',');
    }
    newBalance = atof(amountToDeposit) + oldBalance;
    updateAccount(fileDirectory, newBalance, &accountToDeposit);
    printf("\n\033[1;32mRM%.2lf Deposited!\033[0m\n", atof(amountToDeposit));
}

void withdrawFromAccount(char *accountNo) {
    char fileDirectory[512] = "../database/";
    char fileName[100];
    struct accountCSV accountToWithdraw;

    strcpy(fileName, accountNo);
    strcat(fileName, ".txt");
    strcat(fileDirectory, fileName);

    readAccountCSV(fileDirectory, &accountToWithdraw);

    verifyPin(accountToWithdraw.pin);

    double oldBalance = atof(accountToWithdraw.balance);
    double newBalance;
    char amountToWithdraw[100];

    printf("\n\033[1mAccount Balance: RM%s\033[0m\n", accountToWithdraw.balance);

    printf("\nEnter the amount you wish to withdraw: RM");
    scanf("%[^\n]", amountToWithdraw);
    clearInputBuffer();
    removeChar(amountToWithdraw, ',');
    while (!isDouble(amountToWithdraw)) {
        printf("\033[31m**INVALID AMOUNT**\033[0m\n");
        printf("Enter the amount you wish to withdraw: RM");
        scanf("%[^\n]", amountToWithdraw);
        clearInputBuffer();
        removeChar(amountToWithdraw, ',');
    }
    while (atof(amountToWithdraw) > oldBalance) {
        printf("\033[31m**INVALID AMOUNT: NOT ENOUGH FUNDS**\033[0m\n");
        printf("Enter the amount you wish to withdraw: RM");
        scanf("%[^\n]", amountToWithdraw);
        clearInputBuffer();
        removeChar(amountToWithdraw, ',');
    }
    newBalance = oldBalance - atof(amountToWithdraw);
    updateAccount(fileDirectory, newBalance, &accountToWithdraw);
    printf("\n\033[1;32mRM%.2lf Withdrawn!\033[0m\n", atof(amountToWithdraw));
}

double checkRemittanceFee(struct accountCSV *senderAccount, struct accountCSV *receiverAccount) {
    double percentage = 0;
    if (strcmp(senderAccount->accountType, "Savings") == 0 && strcmp(receiverAccount->accountType, "Current") == 0) {
        percentage = 2;
    } else if (strcmp(senderAccount->accountType, "Current") == 0 && strcmp(receiverAccount->accountType, "Savings") == 0) {
        percentage = 3;
    }
    return percentage;
}

void transferToAccount(char *senderNo, char *receiverNo) {
    char fileDirectorySender[512] = "../database/";
    char fileDirectoryReceiver[512] = "../database/";
    char tempFileName[100];
    struct accountCSV senderAccount;
    struct accountCSV receiverAccount;

    strcpy(tempFileName, senderNo);
    strcat(tempFileName, ".txt");
    strcat(fileDirectorySender, tempFileName);

    strcpy(tempFileName, receiverNo);
    strcat(tempFileName, ".txt");
    strcat(fileDirectoryReceiver, tempFileName);

    readAccountCSV(fileDirectorySender, &senderAccount);
    readAccountCSV(fileDirectoryReceiver, &receiverAccount);

    verifyPin(senderAccount.pin);

    double senderOldBalance = atof(senderAccount.balance);
    double senderNewBalance;
    double receiverOldBalance = atof(receiverAccount.balance);
    double receiverrNewBalance;
    
    double remittanceFeePercentage = checkRemittanceFee(&senderAccount, &receiverAccount);
    bool confirm = true;

    if (remittanceFeePercentage) {
        char temp[8];
        printf("\n\033[3mTransferring to this account will charge a \033[3;31m%.0lf%% remittance fee.\033[0m", remittanceFeePercentage);
        printf("\nEnter 'CONFIRM' To Confirm Transfer (Anything Else To Cancel): ");
        scanf("%[^\n]", temp);
        clearInputBuffer();
        if (strcasecmp(temp, "CONFIRM") != 0) {
            confirm = false;
        }
    }

    char amountToTransfer[100];
    if (confirm) {
        printf("\n\033[1mAccount Balance: RM%s\033[0m\n", senderAccount.balance);

        printf("\nEnter the amount you wish to transfer: RM");
        scanf("%[^\n]", amountToTransfer);
        clearInputBuffer();
        removeChar(amountToTransfer, ',');
        while (!isDouble(amountToTransfer)) {
            printf("\033[31m**INVALID AMOUNT**\033[0m\n");
            printf("Enter the amount you wish to transfer: RM");
            scanf("%[^\n]", amountToTransfer);
            clearInputBuffer();
            removeChar(amountToTransfer, ',');
        }

        printf("1:%lf\n", senderOldBalance);
        double remittanceFee = atof(amountToTransfer) * (remittanceFeePercentage / 100);
        double availableBalance = senderOldBalance / (1 + remittanceFeePercentage / 100);

        while (atof(amountToTransfer) > availableBalance) {
            printf("\033[31m**INVALID AMOUNT: NOT ENOUGH FUNDS**\033[0m\n");
            printf("\033[31m**TRANSFERRABLE BALANCE (AFTER REMITTANCE FEE):\033[1mRM%.2lf\033[0;31m**\033[0m\n", availableBalance);
            printf("Enter the amount you wish to transfer: RM");
            scanf("%[^\n]", amountToTransfer);
            clearInputBuffer();
            removeChar(amountToTransfer, ',');
            remittanceFee = atof(amountToTransfer) * (remittanceFeePercentage / 100);
            availableBalance = senderOldBalance / (1 + remittanceFeePercentage / 100);
        }

        senderNewBalance = senderOldBalance - atof(amountToTransfer) - remittanceFee;
        updateAccount(fileDirectorySender, senderNewBalance, &senderAccount);

        receiverrNewBalance = receiverOldBalance + atof(amountToTransfer);
        updateAccount(fileDirectoryReceiver, receiverrNewBalance, &receiverAccount);

        printf("\n\033[1;32mRM%.2lf Transferred!\033[0m\n", atof(amountToTransfer));
        if (remittanceFee) {
            printf("\033[1;31mRemittance Fee of RM%.2lf Was Charged\033[0m\n", remittanceFee);
        }
    } else {
        printf("\n\033[31m**ACTION CANCELLED**\033[0m\n");
    }
}