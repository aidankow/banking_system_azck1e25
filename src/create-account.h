#include <stdio.h>

struct accountDetails
{
    char name[64];
    char id[8];
    char accountType;
    char pin[5];
};

void clearInputBuffer() {
    while ((getchar()) != '\n');
}

void enterDetails(struct accountDetails *newAccount) {
    printf("Please Enter Your Full Name: ");
    scanf("%[^\n]", newAccount->name);
    clearInputBuffer();
    printf("Please Enter Your Identification Number (ID): ");
    scanf("%s", newAccount->id);
    clearInputBuffer();
    printf("Please Enter Your Account Type (Savings/Current): ");
    scanf("%c", &newAccount->accountType);
    clearInputBuffer();
    printf("Please Enter Your 4-Digit Pin: ");
    scanf("%4s", newAccount->pin);
    clearInputBuffer();
}