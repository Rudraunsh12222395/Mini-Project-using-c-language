#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    char name[100];
    int age;
    float income;
    float tax;
} TaxPayer;
void addNewRecord(FILE* file) {
    TaxPayer taxpayer;
    printf("Enter name: ");
    scanf("%s", taxpayer.name);
    printf("Enter age: ");
    scanf("%d", &taxpayer.age);
    printf("Enter income: ");
    scanf("%f", &taxpayer.income);
    if (taxpayer.income <= 250000) {
        taxpayer.tax = 0;
    } else if (taxpayer.income <= 500000) {
        taxpayer.tax = (taxpayer.income - 250000) * 0.05;
    } else if (taxpayer.income <= 1000000) {
        taxpayer.tax = (taxpayer.income - 500000) * 0.2 + 25000;
    } else {
        taxpayer.tax = (taxpayer.income - 1000000) * 0.3 + 125000;
    }
    fwrite(&taxpayer, sizeof(TaxPayer), 1, file);
    printf("Record added successfully.\n");
}
void listAllTaxPayers(FILE* file) {
    TaxPayer taxpayer;
    printf("List of all tax payers:\n");
    printf("--------------------------------------------------------\n");
    printf("Name\t\tAge\t\tIncome\t\tTax\n");
    printf("--------------------------------------------------------\n");
    while (fread(&taxpayer, sizeof(TaxPayer), 1, file)) {
        printf("%s\t\t%d\t\t%.2f\t%.2f\n", taxpayer.name, taxpayer.age, taxpayer.income, taxpayer.tax);
    }
    printf("--------------------------------------------------------\n");
}
void searchRecord(FILE* file) {
    TaxPayer taxpayer;
    char name[100];
    printf("Enter name to search: ");
    scanf("%s", name);
    int found = 0;
    while (fread(&taxpayer, sizeof(TaxPayer), 1, file)) {
        if (strcmp(taxpayer.name, name) == 0) {
            printf("Record found:\n");
            printf("Name: %s\n", taxpayer.name);
            printf("Age: %d\n", taxpayer.age);
            printf("Income: %.2f\n", taxpayer.income);
            printf("Tax: %.2f\n", taxpayer.tax);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Record not found.\n");
    }
}
void editRecord(FILE* file) {
    TaxPayer taxpayer;
    char name[100];
    FILE* tempFile = fopen("temp.txt", "wb+");
    printf("Enter name to edit: ");
    scanf("%s", name);
    int found = 0;
    while (fread(&taxpayer, sizeof(TaxPayer), 1, file)) {
        if (strcmp(taxpayer.name, name) == 0) {
            printf("Record found:\n");
            printf("Name: %s\n", taxpayer.name);
            printf("Age: %d\n", taxpayer.age);
            printf("Income: %.2f\n", taxpayer.income);
            printf("Tax: %.2f\n", taxpayer.tax);
            found = 1;
            printf("Enter new age: ");
            scanf("%d", &taxpayer.age);
            printf("Enter new income: ");
            scanf("%f", &taxpayer.income);
            if (taxpayer.income <= 250000) {
                taxpayer.tax = 0;
            } else if (taxpayer.income <= 500000) {
                taxpayer.tax = (taxpayer.income - 250000) * 0.05;
            } else if (taxpayer.income <= 1000000) {
                taxpayer.tax = (taxpayer.income - 500000) * 0.2 + 25000;
            } else {
                taxpayer.tax = (taxpayer.income - 1000000) * 0.3 + 125000;
            }
            fwrite(&taxpayer, sizeof(TaxPayer), 1, tempFile);
            printf("Record updated successfully.\n");
        } else {
            fwrite(&taxpayer, sizeof(TaxPayer), 1, tempFile);
        }
    }
    if (!found) {
        printf("Record not found.\n");
    }
    fclose(file);
    fclose(tempFile);
    remove("taxpayers.txt");
    rename("temp.txt", "taxpayers.txt");
    file = fopen("taxpayers.txt", "rb+");
}
void deleteRecord(FILE* file) {
TaxPayer taxpayer;
char name[100];
printf("Enter name to delete: ");
scanf("%s", name);
FILE* tempFile = fopen("temp.txt", "wb");
if (tempFile == NULL) {
    printf("Error creating temporary file.\n");
    return;
}
while (fread(&taxpayer, sizeof(TaxPayer), 1, file)) {
    if (strcmp(taxpayer.name, name) != 0) {
        fwrite(&taxpayer, sizeof(TaxPayer), 1, tempFile);
    }
}
fclose(file);
fclose(tempFile);
remove("taxpayers.txt");
rename("temp.txt", "taxpayers.txt");
printf("Record deleted successfully.\n");
}
#include <stdio.h>

int main() {
    FILE* file;
    int choice;
    file = fopen("taxpayers.txt", "r+b");
    if (file == NULL) {
        file = fopen("taxpayers.txt", "w+b");
        if (file == NULL) {
            printf("Error creating file.\n");
            return 1;
        }
    }
    do {
        printf("\nIncome Tax Calculator\n");
        printf("----------------------------\n");
        printf("1. Add New Record\n");
        printf("2. List all Tax Payers\n");
        printf("3. Search Record\n");
        printf("4. Edit Record\n");
        printf("5. Delete Record\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please try again.\n");
            scanf("%*s"); // discard invalid input from input buffer
            continue;
        }
        switch (choice) {
            case 1:
                addNewRecord(file);
                break;
            case 2:
                listAllTaxPayers(file);
                break;
            case 3:
                searchRecord(file);
                break;
            case 4:
                editRecord(file);
                break;
            case 5:
                deleteRecord(file);
                break;
            case 6:
                printf("Thank you for using Income Tax Calculator!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 6);
    fclose(file);
    return 0;
}

