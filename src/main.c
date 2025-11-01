#include <stdio.h>
#include <string.h>
#include <time.h>
#include "create-account.h"

int checkAction(char *action) {
    int length = strlen(action);
    
    if (strcmp(action, "1") == 0 || strcmp(action, "create") == 0) {
        return 1;
    } else if (strcmp(action, "2") == 0 || strcmp(action, "delete") == 0) {
        return 2;
    } else if (strcmp(action, "3") == 0 || strcmp(action, "deposit") == 0) {
        return 3;
    } else if (strcmp(action, "4") == 0 || strcmp(action, "withdrawal") == 0) {
        return 4;
    } else if (strcmp(action, "5") == 0 || strcmp(action, "remittance") == 0) {
        return 5;
    } else if (strcmp(action, "EXIT") == 0) {
        return 0;
    }
    return -1;
}

void logAction(char *action) {
    FILE *transactionPtr;
    transactionPtr = fopen("/Users/kuanhuakow/Library/CloudStorage/OneDrive-UniversityofSouthampton/banking_system_azck1e25/database/transaction.log", "a");
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
    printf("[4] withdrawl: Withdrawal Money\n");
    printf("[5] remittance: Transfer Money to Another Account\n");
    printf("Enter 'EXIT' to exit the program.");
    printf("\n------------------------------------\n");
    printf("Selected Action: ");
    scanf("%10s", input);
    clearInputBuffer();
    
    action = checkAction(input);
    while (action == -1) {
        printf("\033[31m**ERROR: INVALID ACTION**\033[0m\n");
        printf("Please Enter a Valid Action: ");
        scanf("%10s", input);
        clearInputBuffer();
        action = checkAction(input);
    }

    if (action == 0) {
        printf("\n\033[2;3mSession Ended.\033[0m\n");
        return 0;
    } else {
        logAction(input);
        printf("\n*\n*\n*\n*\n*\n*\n*\n");
        return action;
    }
}

void displaySessionInfo() {
    time_t now = time(NULL);
    struct tm *time = localtime(&now);
    char buffer[100];
    int loadedAccounts = 0;
    
    strftime(buffer, sizeof(buffer), "%B %d, %A %H:%M:%S", time);
    printf("\n\033[2;3mSession Start: %s\033[0m\n", buffer);
    printf("\033[2;3mCount of Loaded Accounts: %d\033[0m\n", loadedAccounts);
    //add loaded accounts
}

int main() {
    char formattedTime[64];
    int action = -1;

    displaySessionInfo();

    while (action != 0) {
        action = getAction();
    }

    return 0;
}