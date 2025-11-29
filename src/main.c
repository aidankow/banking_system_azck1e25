#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include "create-account.h"
#include "delete-account.h"
#include "dep-wit-rem.h"

// Library/CloudStorage/OneDrive-UniversityofSouthampton/banking_system_azck1e25/src

int checkAction(char *action) {
    if (strcmp(action, "1") == 0 || strcasecmp(action, "create") == 0) {
        return 1;
    } else if (strcmp(action, "2") == 0 || strcasecmp(action, "delete") == 0) {
        return 2;
    } else if (strcmp(action, "3") == 0 || strcasecmp(action, "deposit") == 0) {
        return 3;
    } else if (strcmp(action, "4") == 0 || strcasecmp(action, "withdrawal") == 0) {
        return 4;
    } else if (strcmp(action, "5") == 0 || strcasecmp(action, "remittance") == 0) {
        return 5;
    } else if (strcasecmp(action, "EXIT") == 0) {
        return 0;
    }
    return -1;
}

void logAction(char *action) {
    FILE *transactionPtr;
    transactionPtr = fopen("../database/transaction.log", "a");
    if (transactionPtr == NULL) {
        printf("\033[31mError opening transaction.log\033[0m\n");
    }
    fprintf(transactionPtr, "%s\n", action);
    fclose(transactionPtr);
}

int getAction() {
    char input[11];
    int action;

    printf("\n------------------------------------\n");
    printf("\033[1mChoose an Action by Entering a Number: \033[0m\n");
    printf("[1] create: Create New Bank Account\n");
    printf("[2] delete: Delete Bank Account\n");
    printf("[3] deposit: Deposit Money\n");
    printf("[4] withdrawal: Withdrawal Money\n");
    printf("[5] remittance: Transfer Money to Another Account\n");
    printf("Enter 'EXIT' to exit the program.");
    printf("\n------------------------------------\n");
    printf("Selected Action: ");
    scanf("%[^\n]", input);
    clearInputBuffer();
    
    action = checkAction(input);
    while (action == -1) {
        printf("\033[31m**ERROR: INVALID ACTION**\033[0m\n");
        printf("Please Enter a Valid Action: ");
        scanf("%[^\n]", input);
        clearInputBuffer();
        action = checkAction(input);
    }

    if (action == 0) {
        printf("\n\033[2;3mSession Ended.\033[0m\n");
        return 0;
    } else {
        logAction(input);
        printf("\n*\n*\n*\n*\n*\n*\n");
        return action;
    }
}

int getLoadedAccounts() {
    char temp[100];
    int counter = 0;

    FILE *accountPtr;
    accountPtr = fopen("../database/index.txt", "r");
    if (accountPtr == NULL) {
        printf("\033[31m(1)Error opening index.txt\033[0m\n");
        return 0;
    }

    while (fgets(temp, 100, accountPtr)) {
        counter++;
    }
    fclose(accountPtr);
    return counter;
}

void displaySessionInfo() {
    time_t now = time(NULL);
    struct tm *time = localtime(&now);
    char timeString[100];
    int loadedAccounts = getLoadedAccounts();
    
    strftime(timeString, sizeof(timeString), "%B %d, %A %H:%M:%S", time);
    printf("\n\033[2;3mSession Start: %s\033[0m\n", timeString);
    printf("\033[2;3mCount of Loaded Accounts: %d\033[0m\n", loadedAccounts);
}

void performAction(int action, struct accountDetails *newAccount) {
    if (action == 1) {
        generateDetails(newAccount);
        char temp[8];

        printf("\n\nDETAILS ENTERED:\n");
        printf("%s\n", newAccount->name);
        printf("%s\n", newAccount->id);
        printf("%s\n", newAccount->accountType);
        printf("%s\n", newAccount->pin);
        printf("Enter 'CONFIRM' To Confirm Details (Anything Else To Cancel): ");
        scanf("%[^\n]", temp);
        clearInputBuffer();

        if (strcasecmp(temp, "CONFIRM") == 0) {
            logAccountDetails(newAccount);
            printf("ACCOUNT ADDED: %s\n", newAccount->accountNo);
        } else {
            printf("\n\033[31m**ACTION CANCELLED**\033[0m\n");
        }

    } else if (action == 2) {

        char temp[20];
        int numOfLines = 0;

        FILE *accountPtr;
        accountPtr = fopen("../database/index.txt", "r");
        if (accountPtr == NULL) {
            printf("\033[31mError opening index.txt\033[0m\n");
        }
        while (fgets(temp, 20, accountPtr)) {
            numOfLines++;
        }
        
        rewind(accountPtr);
        char accounts[numOfLines][10];
        int index = 0;
        while (fgets(temp, 20, accountPtr)) {
            temp[strcspn(temp, "\n")] = '\0';
            if (temp[0] == '\0') {
                continue;
            }
            strcpy(accounts[index], temp);
            index++;
        }
        fclose(accountPtr);

        printf("\n\033[1mLIST OF EXISTING BANK ACCOUNTS:\033[0m\n");
        if (numOfLines == 1) { //there will always be an empty newline in index.txt
            printf("\n---No Accounts Found---\n\n");
        } else {
            for (int i=0;i<numOfLines;i++) {
                printf("%d. %s\n", i+1, accounts[i]);
            }
        }

        printf("Which account do you wish to delete?: ");
        char input[10];
        scanf("%[^\n]", input);
        clearInputBuffer();
        while (!checkAccountNo(input, numOfLines, accounts)) {
            printf("\033[31m**ACCOUNT NOT FOUND**\033[0m\n");
            printf("Which account do you wish to delete?: ");
            scanf("%[^\n]", input);
            clearInputBuffer();
        }

        char accountNo[10];
        strcpy(accountNo, getAccountNo(input, accounts));
        performDeletion(accountNo);

    } else if (action == 3) {
        char input[10];
        printf("\nEnter your account number: ");
        scanf("%[^\n]", input);
        clearInputBuffer();
        while (!accountExists(input)) {
            printf("\033[31m**ACCOUNT NOT FOUND**\033[0m\n");
            printf("Enter your account number: ");
            scanf("%[^\n]", input);
            clearInputBuffer();
        }
        depositToAccount(input);

    } else if (action == 4) {
        char input[10];
        printf("\nEnter your account number: ");
        scanf("%[^\n]", input);
        clearInputBuffer();
        while (!accountExists(input)) {
            printf("\033[31m**ACCOUNT NOT FOUND**\033[0m\n");
            printf("Enter your account number: ");
            scanf("%[^\n]", input);
            clearInputBuffer();
        }
        withdrawFromAccount(input);

    } else if (action == 5) {
        char sender[10];
        char receiver[10];
        printf("\nEnter your account number: ");
        scanf("%[^\n]", sender);
        clearInputBuffer();
        while (!accountExists(sender)) {
            printf("\033[31m**ACCOUNT NOT FOUND**\033[0m\n");
            printf("Enter your account number: ");
            scanf("%[^\n]", sender);
            clearInputBuffer();
        }

        printf("Enter the account number you want to transfer to: ");
        scanf("%[^\n]", receiver);
        clearInputBuffer();
        while (strcmp(sender, receiver) == 0) {
            printf("\033[31m**YOU CANNOT TRANSFER MONEY TO YOURSELF**\033[0m\n");
            printf("Enter the account number you want to transfer to: ");
            scanf("%[^\n]", receiver);
            clearInputBuffer();
        }
        while (!accountExists(receiver)) {
            printf("\033[31m**ACCOUNT NOT FOUND**\033[0m\n");
            printf("Enter the account number you want to transfer to: ");
            scanf("%[^\n]", receiver);
            clearInputBuffer();
        }

        transferToAccount(sender, receiver);
    }
}

int main() {
    char formattedTime[64];
    int action = -1;
    struct accountDetails newAccount;

    displaySessionInfo();

    while (action != 0) {
        action = getAction();
        performAction(action, &newAccount);
    }

    return 0;
}