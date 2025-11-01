#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

void clearInputBuffer() {
    while ((getchar()) != '\n');
}

bool validAction(char *action) {
    int length = strlen(action);
    bool valid = false;
    if (
        strcmp(action, "1") == 0 || strcmp(action, "create") == 0 ||
        strcmp(action, "2") == 0 || strcmp(action, "delete") == 0 ||
        strcmp(action, "3") == 0 || strcmp(action, "deposit") == 0 ||
        strcmp(action, "4") == 0 || strcmp(action, "withdrawal") == 0 ||
        strcmp(action, "5") == 0 || strcmp(action, "remittance") == 0
    ) {
        valid = true;
    }
    return valid;
}

int logAction(char *action) {
    FILE *transactionPtr;
    transactionPtr = fopen("/Users/kuanhuakow/Library/CloudStorage/OneDrive-UniversityofSouthampton/banking_system_azck1e25/database/transaction.log", "a");
    if (transactionPtr == NULL) {
        perror("Error opening transaction.log");
        return -1;
    }
    fprintf(transactionPtr, "%s\n", action);
    fclose(transactionPtr);
    return 1;
}

int getAction() {
    char action[11];

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
    scanf("%10s", action);
    clearInputBuffer();

    if (strcmp(action, "EXIT") == 0) {
        printf("\n\033[2;3mSession Ended.\033[0m\n");
        return 0;
    }

    while (!validAction(action)) {
        printf("\033[31m**ERROR: INVALID ACTION**\033[0m\n");
        printf("Please Enter a Valid Action: ");
        scanf("%10s", action);
        clearInputBuffer();
    }
    
    printf("\n*\n*\n*\n*\n*\n*\n*\n");
    return logAction(action);
}

int main() {
    char formattedTime[64];
    int action = -1;

    time_t now = time(NULL);
    struct tm *time = localtime(&now);
    char buffer[100];
    
    strftime(buffer, sizeof(buffer), "%B %d, %A %H:%M:%S", time);
    printf("\n\033[2;3mSession Start: %s\033[0m\n", buffer);

    while (action != 0) {
        action = getAction();
    }

    return 0;
}