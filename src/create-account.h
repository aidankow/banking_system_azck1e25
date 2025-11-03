#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include "internal.h"

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

    createDatabase();
    FILE *accountPtr;
    accountPtr = fopen("database/index.txt", "r");
    if (accountPtr == NULL) {
        printf("\033[31mError opening index.txt\033[0m\n");
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
            // printf("CMP: %s | %s\n", temp, accountNo);
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
    accountPtr = fopen("/Users/kuanhuakow/Library/CloudStorage/OneDrive-UniversityofSouthampton/banking_system_azck1e25/database/index.txt", "a");
    if (accountPtr == NULL) {
        printf("\033[31mError opening index.txt\033[0m\n");
    }
    fprintf(accountPtr, "%s\n", accountNo);
    fclose(accountPtr);
}

void logAccountDetails(struct accountDetails *newAccount) {
    char fileDirectory[512] = "/Users/kuanhuakow/Library/CloudStorage/OneDrive-UniversityofSouthampton/banking_system_azck1e25/database/";
    char fileName[100];
    strcpy(fileName, newAccount->accountNo);
    strcat(fileName, ".txt");
    strcat(fileDirectory, fileName);

    logBankAccountNo(newAccount->accountNo);
    FILE *accountPtr;
    accountPtr = fopen("", "w");
    if (accountPtr == NULL) {
        printf("\033[31mError opening index.txt\033[0m\n");
    }

    fclose(accountPtr);
}

void inputDetails(struct accountDetails *newAccount) {
    printf("\nPlease Enter Your Full Name: ");
    scanf("%100[^\n]", newAccount->name);
    clearInputBuffer();
    printf("Please Enter Your Identification Number (ID): ");
    scanf("%8s", newAccount->id);
    clearInputBuffer();
    printf("Please Enter Your Account Type (Savings/Current): ");
    scanf("%7s", newAccount->accountType);
    clearInputBuffer();
    printf("Please Enter Your 4-Digit Pin: ");
    scanf("%4s", newAccount->pin);
    clearInputBuffer();
}

void generateDetails(struct accountDetails *newAccount) {
    char accountNo[10];
    
    // inputDetails(newAccount);
    generateBankAccountNo(accountNo);
    strcpy(newAccount->accountNo, accountNo);
    newAccount->balance = 0;
}