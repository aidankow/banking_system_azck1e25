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

int main() {
    time_t currentTime;
    char action[10];
    
    time(&currentTime);
    printf("\n\033[2;3mSession Start: %s\033[0m", ctime(&currentTime));

    printf("\n------------------------------------\n");
    printf("\033[1mChoose an Action by Entering a Number: \033[0m\n");
    printf("[1] create: Create New Bank Account\n");
    printf("[2] delete: Delete Bank Account\n");
    printf("[3] deposit: Deposit Money\n");
    printf("[4] withdrawl: Withdrawal Money\n");
    printf("[5] remittance: Transfer Money to Another Account");
    printf("\n------------------------------------\n");
    printf("Selected Action: ");
    scanf("%s", action);
    clearInputBuffer();
    
    while (!validAction(action)) {
        printf("\033[31m**ERROR: INVALID ACTION**\033[0m\n");
        printf("Please Enter a Valid Action: ");
        scanf("%s", action);
        clearInputBuffer();
    }

    return 0;
}