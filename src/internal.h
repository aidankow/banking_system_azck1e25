#include <sys/stat.h>
#include <stdio.h>

void createIndex() {
    FILE *Ptr;
    Ptr = fopen("index.txt", "w");
    fclose(Ptr);
}

void createTransactionLog() {
    FILE *Ptr;
    Ptr = fopen("transaction.log", "w");
    fclose(Ptr);
}

void createDatabase() {
    mkdir("database", 0777);
    createIndex();
    createTransactionLog();
}