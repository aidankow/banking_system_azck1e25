#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

struct accountDetails
{
    char name[101];
    char id[9];
    char accountType[8];
    char pin[5];
    char accountNo[10];
    double balance;
};

void clearInputBuffer() {
    while ((getchar()) != '\n');
}

void generateBankAccountNo(char *accountNo) {
    char temp[100];
    bool valid = false;
    int range = rand() % (9-7+1)+7; //(max-min+1)+min = range of random numbers (min, max)
    srand(time(NULL));

    FILE *accountPtr;
    accountPtr = fopen("../database/index.txt", "r");
    if (accountPtr == NULL) {
        printf("\033[31m(1)Error opening index.txt\033[0m\n");
    }

    //check if account number exists in index.txt
    while (!valid) {
        for(int i=0;i<range;i++) {
            char temp = (rand() % (9-0+1)) + '0';
            accountNo[i] = temp;
        accountNo[range] = '\0'; //null terminator to ignore empty cells
        }
        valid = true;
        rewind(accountPtr);
        while (fgets(temp, 100, accountPtr)) {
            temp[strcspn(temp, "\n")] = 0; //get rid of \n from line read from file
            if (strcmp(temp, accountNo) == 0) {
                valid = false;
                printf("ERROR: %s | %s\n", temp, accountNo);
                break;
            }
        }
    }
    fclose(accountPtr);
}

void logBankAccountNo(char *accountNo) {
    FILE *accountPtr;
    accountPtr = fopen("../database/index.txt", "a");
    if (accountPtr == NULL) {
        printf("\033[31m(2)Error opening index.txt\033[0m\n");
    }
    fprintf(accountPtr, "%s\n", accountNo);
    fclose(accountPtr);
}

void logAccountDetails(struct accountDetails *newAccount) {
    char fileDirectory[512] = "../database/";
    char fileName[100];
    strcpy(fileName, newAccount->accountNo);
    strcat(fileName, ".txt");
    strcat(fileDirectory, fileName);

    logBankAccountNo(newAccount->accountNo);
    FILE *accountPtr;
    accountPtr = fopen(fileDirectory, "w");
    if (accountPtr == NULL) {
        printf("\033[31mError opening %s.txt\033[0m\n", fileDirectory);
    }
    fprintf(accountPtr, "%s,%s,%s,%s,%s,%.2lf\n", newAccount->accountNo, newAccount->id, newAccount->name, newAccount->pin, newAccount->accountType, newAccount->balance);
    fclose(accountPtr);
}

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

void inputDetails(struct accountDetails *newAccount) {
    printf("\nPlease Enter Your Full Name: ");
    fgets(newAccount->name, sizeof(newAccount->name), stdin);
    newAccount->name[strcspn(newAccount->name, "\n")] = '\0';
    while (!nameCheck(newAccount->name)) {
        printf("\033[31m**ERROR: INVALID NAME**\033[0m");
        printf("\nPlease Enter Your Full Name: ");
        fgets(newAccount->name, sizeof(newAccount->name), stdin);
        newAccount->name[strcspn(newAccount->name, "\n")] = '\0';
    }

    printf("Please Enter Your Identification Number (ID): ");
    scanf("%[^\n]", newAccount->id);
    clearInputBuffer();
    while (!idCheck(newAccount->id)) {
        printf("\033[31m**ERROR: INVALID ID**\033[0m");
        printf("\nPlease Enter Your Identification Number (ID): ");
        scanf("%[^\n]", newAccount->id);
        clearInputBuffer();
    }

    printf("Please Enter Your Account Type (Savings/Current): ");
    scanf("%[^\n]", newAccount->accountType);
    clearInputBuffer();
    while (!accountTypeCheck(newAccount->accountType)) {
        printf("\033[31m**ERROR: INVALID ACCOUNT TYPE**\033[0m");
        printf("\nPlease Enter Your Account Type (Savings/Current): ");
        scanf("%[^\n]", newAccount->accountType);
        clearInputBuffer();
    }

    printf("Please Enter Your 4-Digit Pin: ");
    scanf("%[^\n]", newAccount->pin);
    clearInputBuffer();
    while (!idCheck(newAccount->pin)) {
        printf("\033[31m**ERROR: INVALID PIN**\033[0m");
        printf("\nPlease Enter Your 4-Digit Pin: ");
        scanf("%[^\n]", newAccount->pin);
        clearInputBuffer();
    }
}

void generateDetails(struct accountDetails *newAccount) {
    char accountNo[10];
    
    inputDetails(newAccount);
    generateBankAccountNo(accountNo);
    strcpy(newAccount->accountNo, accountNo);
    newAccount->balance = 0;
}